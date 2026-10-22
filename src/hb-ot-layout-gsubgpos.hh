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

#अगर_अघोषित HB_OT_LAYOUT_GSUBGPOS_HH
#घोषणा HB_OT_LAYOUT_GSUBGPOS_HH

#समावेश "hb.hh"
#समावेश "hb-buffer.hh"
#समावेश "hb-map.hh"
#समावेश "hb-set.hh"
#समावेश "hb-ot-map.hh"
#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-depend-data.hh"


namespace OT {


काष्ठा hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, bool>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { वापस obj.intersects (यह->glyphs); }
  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }

  स्थिर hb_set_t *glyphs;

  hb_intersects_context_t (स्थिर hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

काष्ठा hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, bool>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { वापस obj.may_have_non_1to1 (); }
  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }
};

काष्ठा hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  प्रकार return_t (*recurse_func_t) (hb_closure_context_t *c, अचिन्हित lookup_सूचकांक, hb_set_t *covered_seq_indicies, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक);
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { obj.closure (यह); वापस hb_empty_t (); }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }
  व्योम recurse (अचिन्हित lookup_सूचकांक, hb_set_t *covered_seq_indicies, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक)
  {
    अगर (unlikely (nesting_level_बायाँ == 0 || !recurse_func))
      वापस;

    nesting_level_बायाँ--;
    recurse_func (यह, lookup_सूचकांक, covered_seq_indicies, seq_सूचकांक, end_सूचकांक);
    nesting_level_बायाँ++;
  }

  व्योम reset_lookup_visit_गिनती ()
  { lookup_गिनती = 0; }

  bool lookup_limit_exceeded ()
  { वापस lookup_गिनती > HB_MAX_LOOKUP_VISIT_गिनती; }

  bool should_visit_lookup (अचिन्हित पूर्णांक lookup_सूचकांक)
  {
    अगर (lookup_गिनती++ > HB_MAX_LOOKUP_VISIT_गिनती)
      वापस false;

    अगर (is_lookup_done (lookup_सूचकांक))
      वापस false;

    वापस true;
  }

  bool is_lookup_done (अचिन्हित पूर्णांक lookup_सूचकांक)
  {
    अगर (unlikely (done_lookups_glyph_गिनती->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      वापस true;

    /* Have we visited this lookup with the current set of glyphs? */
    अचिन्हित निकालो = glyphs->get_population ();
    अगर (unlikely (निकालो == HB_MAP_मूल्य_INVALID)) // Avoid sentinel interference.
      निकालो--;
    अगर (done_lookups_glyph_गिनती->लो (lookup_सूचकांक) != निकालो)
    {
      done_lookups_glyph_गिनती->set (lookup_सूचकांक, निकालो);

      अगर (!done_lookups_glyph_set->has (lookup_सूचकांक))
      {
	अगर (unlikely (!done_lookups_glyph_set->set (lookup_सूचकांक, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  वापस true;
      }

      done_lookups_glyph_set->लो (lookup_सूचकांक)->सफा ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->लो (lookup_सूचकांक);
    अगर (unlikely (covered_glyph_set->in_error ()))
      वापस true;
    अगर (parent_active_glyphs ().is_subset (*covered_glyph_set))
      वापस true;

    covered_glyph_set->union_ (parent_active_glyphs ());
    वापस false;
  }

  स्थिर hb_set_t& previous_parent_active_glyphs () {
    अगर (active_glyphs_ढेर.लंबाई <= 1)
      वापस *glyphs;

    वापस active_glyphs_ढेर[active_glyphs_ढेर.लंबाई - 2];
  }

  स्थिर hb_set_t& parent_active_glyphs ()
  {
    अगर (!active_glyphs_ढेर)
      वापस *glyphs;

    वापस active_glyphs_ढेर.पुच्छ ();
  }

  hb_set_t* धकेलो_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_ढेर.push ();
    अगर (unlikely (active_glyphs_ढेर.in_error ()))
      वापस nullptr;
    वापस s;
  }

  bool निकालो_cur_done_glyphs ()
  {
    अगर (!active_glyphs_ढेर)
      वापस false;

    active_glyphs_ढेर.निकालो ();
    वापस true;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_ढेर;
  recurse_func_t recurse_func = nullptr;
  अचिन्हित पूर्णांक nesting_level_बायाँ;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_गिनती_2,
			hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			अचिन्हित पूर्णांक nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_बायाँ (nesting_level_बायाँ_2),
			  done_lookups_glyph_गिनती (done_lookups_glyph_गिनती_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { सफाई (); }

  व्योम set_recurse_func (recurse_func_t func) { recurse_func = func; }

  व्योम सफाई ()
  {
    output->del_range (face->get_अंक_glyphs (), HB_SET_मूल्य_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->सफा ();
    active_glyphs_ढेर.निकालो ();
    active_glyphs_ढेर.reset ();
  }

  निजी:
  hb_map_t *done_lookups_glyph_गिनती;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  अचिन्हित पूर्णांक lookup_गिनती = 0;
};



काष्ठा hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  प्रकार return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, अचिन्हित lookup_सूचकांक);
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { obj.closure_lookups (यह); वापस hb_empty_t (); }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }
  व्योम recurse (अचिन्हित lookup_सूचकांक)
  {
    अगर (unlikely (nesting_level_बायाँ == 0 || !recurse_func))
      वापस;

    /* Return if new lookup was recursed to before. */
    अगर (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_सूचकांक))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      वापस;

    nesting_level_बायाँ--;
    recurse_func (यह, lookup_सूचकांक);
    nesting_level_बायाँ++;
  }

  व्योम set_lookup_visited (अचिन्हित lookup_सूचकांक)
  { visited_lookups->add (lookup_सूचकांक); }

  व्योम set_lookup_inactive (अचिन्हित lookup_सूचकांक)
  { inactive_lookups->add (lookup_सूचकांक); }

  bool lookup_limit_exceeded ()
  {
    bool ret = lookup_गिनती > HB_MAX_LOOKUP_VISIT_गिनती;
    अगर (ret)
      DEBUG_संदेश (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    वापस ret; }

  bool is_lookup_visited (अचिन्हित lookup_सूचकांक)
  {
    अगर (unlikely (lookup_गिनती++ > HB_MAX_LOOKUP_VISIT_गिनती))
    {
      DEBUG_संदेश (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_गिनती, lookup_सूचकांक);
      वापस true;
    }

    अगर (unlikely (visited_lookups->in_error ()))
      वापस true;

    वापस visited_lookups->has (lookup_सूचकांक);
  }

  hb_face_t *face;
  स्थिर hb_set_t *glyphs;
  recurse_func_t recurse_func;
  अचिन्हित पूर्णांक nesting_level_बायाँ;

  hb_closure_lookups_context_t (hb_face_t *face_,
				स्थिर hb_set_t *glyphs_,
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

  व्योम set_recurse_func (recurse_func_t func) { recurse_func = func; }

  निजी:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  अचिन्हित पूर्णांक lookup_गिनती;
};

काष्ठा hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, bool>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { वापस obj.would_apply (यह); }
  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }

  hb_face_t *face;
  स्थिर hb_codepoint_t *glyphs;
  अचिन्हित पूर्णांक len;
  bool zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    स्थिर hb_codepoint_t *glyphs_,
			    अचिन्हित पूर्णांक len_,
			    bool zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

काष्ठा hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  प्रकार return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, अचिन्हित पूर्णांक lookup_सूचकांक);
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { obj.collect_glyphs (यह); वापस hb_empty_t (); }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }
  व्योम recurse (अचिन्हित पूर्णांक lookup_सूचकांक)
  {
    अगर (unlikely (nesting_level_बायाँ == 0 || !recurse_func))
      वापस;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    अगर (output == hb_set_get_empty ())
      वापस;

    /* Return if new lookup was recursed to before. */
    अगर (recursed_lookups->has (lookup_सूचकांक))
      वापस;

    hb_set_t *old_before = before;
    hb_set_t *old_निवेश  = निवेश;
    hb_set_t *old_after  = after;
    before = निवेश = after = hb_set_get_empty ();

    nesting_level_बायाँ--;
    recurse_func (यह, lookup_सूचकांक);
    nesting_level_बायाँ++;

    before = old_before;
    निवेश  = old_निवेश;
    after  = old_after;

    recursed_lookups->add (lookup_सूचकांक);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *निवेश;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  अचिन्हित पूर्णांक nesting_level_बायाँ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_निवेश,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       अचिन्हित पूर्णांक nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      निवेश  (glyphs_निवेश  ? glyphs_निवेश  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_बायाँ (nesting_level_बायाँ_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  व्योम set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

काष्ठा hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  क्रमागत glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  अटल constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  काष्ठा glyph_set_cache_कुंजी_t
  {
    glyph_set_cache_कुंजी_t () = शेष;
    glyph_set_cache_कुंजी_t (स्थिर व्योम *तथ्य_2, अचिन्हित मूल्य_3, glyph_set_type_t type_)
      : तथ्य (तथ्य_2), value (मूल्य_3), type (type_) {}

    bool चालक == (स्थिर glyph_set_cache_कुंजी_t &o) स्थिर
    { वापस तथ्य == o.तथ्य && value == o.value && type == o.type; }

    uint32_t hash () स्थिर
    {
      uint32_t current = hb_hash ((uintptr_t) तथ्य);
      current = current * 31 + hb_hash (value);
      वापस current * 31 + hb_hash ((अचिन्हित) type);
    }

    स्थिर व्योम *तथ्य = nullptr;
    अचिन्हित value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  काष्ठा context_set_cache_कुंजी_t
  {
    context_set_cache_कुंजी_t () = शेष;
    context_set_cache_कुंजी_t (स्थिर व्योम *rule_,
			     स्थिर व्योम *record_,
			     स्थिर व्योम *तथ्य_2,
			     hb_codepoint_t active_glyphs_,
			     अचिन्हित मूल्य_3,
			     अचिन्हित format_)
      : rule (rule_), record (record_), तथ्य (तथ्य_2), active_glyphs (active_glyphs_),
	value (मूल्य_3), format (format_) {}

    bool चालक == (स्थिर context_set_cache_कुंजी_t &o) स्थिर
    {
      वापस rule == o.rule &&
	     record == o.record &&
	     तथ्य == o.तथ्य &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () स्थिर
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) तथ्य);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      वापस current * 31 + hb_hash (format);
    }

    स्थिर व्योम *rule = nullptr;
    स्थिर व्योम *record = nullptr;
    स्थिर व्योम *तथ्य = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    अचिन्हित value = 0;
    अचिन्हित format = 0;
  };

  काष्ठा recurse_कुंजी_t
  {
    recurse_कुंजी_t () = शेष;
    recurse_कुंजी_t (अचिन्हित lookup_सूचकांक_3,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_सूचकांक (lookup_सूचकांक_3),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    bool चालक == (स्थिर recurse_कुंजी_t &o) स्थिर
    {
      वापस lookup_सूचकांक == o.lookup_सूचकांक &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () स्थिर
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_सूचकांक)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      वापस current;
    }

    अचिन्हित lookup_सूचकांक = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  प्रकार return_t (*recurse_func_t) (hb_depend_context_t *c, अचिन्हित lookup_सूचकांक, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { obj.depend (यह); वापस hb_empty_t (); }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  व्योम recurse (अचिन्हित lookup_idx, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक)
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
    अगर (lookups_seen.has (lookup_idx)) वापस;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_कुंजी = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_सूचकांक;
    अगर (!recurse_path_to_सूचकांक.has (path_कुंजी, &path))
    {
      path_सूचकांक = recurse_path_to_सूचकांक.get_population () + 1;
      अगर (unlikely (!recurse_path_to_सूचकांक.set (path_कुंजी, path_सूचकांक)))
      {
	depend_तथ्य->हार ();
	lookups_seen.del (lookup_idx);
	वापस;
      }
    }
    अन्यथा
      path_सूचकांक = *path;
    recurse_path = path_सूचकांक;
    recurse_func (यह, lookup_idx, covered_seq_indices, seq_सूचकांक, end_सूचकांक);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  काष्ठा active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    bool use_मूल = false;
  };

  स्थिर hb_set_t& parent_active_glyphs ()
  {
    अगर (!active_glyphs_ढेर_depth)
      वापस *glyphs;

    स्थिर active_glyphs_frame_t &frame = active_glyphs_ढेर[active_glyphs_ढेर_depth - 1];
    वापस frame.use_मूल ? *glyphs : frame.glyphs;
  }

  hb_set_t* धकेलो_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    अगर (active_glyphs_ढेर_depth == active_glyphs_ढेर.लंबाई &&
	unlikely (!active_glyphs_ढेर.धकेलो_or_fail ()))
      वापस nullptr;

    active_glyphs_frame_t &frame = active_glyphs_ढेर[active_glyphs_ढेर_depth++];
    frame.glyphs.सफा ();
    frame.active_idx = active_idx;
    frame.use_मूल = false;
    वापस &frame.glyphs;
  }

  bool धकेलो_मूल_active_glyphs ()
  {
    अगर (active_glyphs_ढेर_depth == active_glyphs_ढेर.लंबाई &&
	unlikely (!active_glyphs_ढेर.धकेलो_or_fail ()))
      वापस false;

    active_glyphs_ढेर[active_glyphs_ढेर_depth++].use_मूल = true;
    वापस true;
  }

  bool निकालो_cur_done_glyphs ()
  {
    अगर (!active_glyphs_ढेर_depth)
      वापस false;

    active_glyphs_ढेर_depth--;
    वापस true;
  }

  bool get_parent_active_glyphs_सूचकांक (hb_codepoint_t *active_idx)
  {
    अगर (!active_glyphs_ढेर_depth ||
	active_glyphs_ढेर[active_glyphs_ढेर_depth - 1].use_मूल)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      वापस true;
    }

    active_glyphs_frame_t &frame = active_glyphs_ढेर[active_glyphs_ढेर_depth - 1];
    *active_idx = frame.active_idx;
    अगर (*active_idx != HB_CODEPOINT_INVALID)
      वापस true;

    hb_codepoint_t glyph;
    अगर (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    अन्यथा अगर (!find_or_create_recurse_set (frame.glyphs, active_idx))
      वापस false;

    frame.active_idx = *active_idx;
    वापस true;
  }

  bool get_context_set_cache_कुंजी (स्थिर व्योम *rule,
				  स्थिर व्योम *record,
				  स्थिर व्योम *तथ्य,
				  अचिन्हित value,
				  अचिन्हित format,
				  context_set_cache_कुंजी_t *key)
  {
    hb_codepoint_t active_idx;
    अगर (!get_parent_active_glyphs_सूचकांक (&active_idx))
      वापस false;
    *key = context_set_cache_कुंजी_t (rule, record, तथ्य, active_idx, value, format);
    वापस true;
  }

  bool get_recurse_कुंजी (अचिन्हित lookup_idx, recurse_कुंजी_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    अगर (!get_parent_active_glyphs_सूचकांक (&active_idx))
      वापस false;

    *key = recurse_कुंजी_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_तथ्य->current_context_set_सूचकांक);
    वापस !completed_recursions.has (*key);
  }

  व्योम finish_recurse (स्थिर recurse_कुंजी_t &key)
  { depend_तथ्य->check_success (completed_recursions.set (key, true)); }

  व्योम reset_recurse_cache ()
  {
    completed_recursions.सफा ();
    recurse_path_to_सूचकांक.सफा ();
    recurse_path = 0;
  }

  bool find_or_create_recurse_set (स्थिर hb_set_t &set, hb_codepoint_t *सूचकांक_2)
  {
    hb_codepoint_t *existing = nullptr;
    अगर (recurse_set_to_सूचकांक.has (&set, &existing))
    {
      *सूचकांक_2 = *existing;
      वापस true;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    अगर (unlikely (!copy))
      वापस depend_तथ्य->हार ();
    copy->set (set);
    अगर (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      वापस depend_तथ्य->हार ();
    }

    *सूचकांक_2 = recurse_sets.लंबाई;
    अगर (unlikely (*सूचकांक_2 & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      वापस depend_तथ्य->हार ();
    }
    अगर (unlikely (!recurse_sets.धकेलो_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      वापस depend_तथ्य->हार ();
    अगर (unlikely (!recurse_set_to_सूचकांक.set (recurse_sets[*सूचकांक_2].लो (), *सूचकांक_2)))
      वापस depend_तथ्य->हार ();
    वापस true;
  }

  स्थिर hb_set_t *find_or_create_glyph_set (स्थिर व्योम *तथ्य,
                                             अचिन्हित value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (स्थिर hb_set_t *set);

  काष्ठा depend_scratch_t
  {
    व्योम सफा ()
    {
      preliminary_context.सफा ();
      position_scratch.सफा ();
      pos_glyphs.सफा ();
      position_context.सफा ();
      disjunctive_indices.सफा ();
      filtered_disjunctive_indices.सफा ();
      backtrack_sets.सफा ();
      lookahead_sets.सफा ();
      निवेश_position_glyphs.सफा ();
      cached_context_sets.सफा ();
      compact_workspace.सफा ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<स्थिर hb_set_t *> backtrack_sets;
    hb_vector_t<स्थिर hb_set_t *> lookahead_sets;
    hb_vector_t<स्थिर hb_set_t *> निवेश_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<अचिन्हित> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    अगर (scratch)
      depend_scratch_pool = scratch->next;
    अन्यथा
    {
      scratch = (depend_scratch_t *) hb_malloc (माप (depend_scratch_t));
      अगर (unlikely (!scratch))
      {
        depend_तथ्य->हार ();
        वापस nullptr;
      }
      नया (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->सफा ();
    वापस scratch;
  }

  व्योम release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_तथ्य_builder_t *depend_तथ्य;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_ढेर;
  अचिन्हित active_glyphs_ढेर_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_सूचकांक = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<स्थिर hb_set_t *, hb_codepoint_t> recurse_set_to_सूचकांक;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_सूचकांक;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_कुंजी_t, bool> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_कुंजी_t, hb_codepoint_t> glyph_set_to_सूचकांक;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_कुंजी_t, hb_codepoint_t> context_set_cache;
  depend_scratch_t *depend_scratch_pool = nullptr;

  hb_depend_context_t (hb_depend_तथ्य_builder_t *depend_तथ्य_2,
                       hb_face_t *face_,
                       hb_set_t *glyphs_)
                      : depend_तथ्य (depend_तथ्य_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (nullptr) {}
  ~hb_depend_context_t ()
  {
    जबतक (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  व्योम set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ढाँचा <typename set_t>
काष्ठा hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, स्थिर Coverage &>
{
  प्रकार स्थिर Coverage &return_t; // Stoopid that we have to dupe this here.
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { वापस obj.get_coverage (); }
  अटल return_t default_return_मूल्य () { वापस Null (Coverage); }
  bool stop_sublookup_iteration (return_t r) स्थिर
  {
    r.collect_coverage (set);
    वापस false;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

काष्ठा matcher_t
{
  प्रकार bool (*match_func_t) (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य);

  ढाँचा <typename context_t>
  व्योम init (स्थिर context_t *c, bool context_match = false)
  {
    set_match_func (nullptr, nullptr);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->table_सूचकांक == 1 || (context_match && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_match || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->table_सूचकांक == 1;
    mask = context_match ? -1 : c->lookup_mask;
    /* Per syllable matching is only for GSUB. */
    per_syllable = c->table_सूचकांक == 0 && c->per_syllable;
    syllable = 0;
  }

  व्योम set_match_func (match_func_t match_func_,
		       स्थिर व्योम *match_तथ्य)
  { match_func = match_func_; match_तथ्य_2 = match_तथ्य; }

  क्रमागत may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_तथ्य) स्थिर
  {
    अगर (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      वापस MATCH_NO;

    अगर (match_func)
      वापस match_func (info, glyph_तथ्य, match_तथ्य_2) ? MATCH_YES : MATCH_NO;

    वापस MATCH_MAYBE;
  }

  क्रमागत may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  ढाँचा <typename context_t>
#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  may_skip_t may_skip (स्थिर context_t *c,
		       स्थिर hb_glyph_info_t &info) स्थिर
  {
    अगर (!c->check_glyph_property (&info, lookup_props))
      वापस SKIP_YES;

    अगर (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      वापस SKIP_MAYBE;

    वापस SKIP_NO;
  }

  खुला:
  अचिन्हित पूर्णांक lookup_props = 0;
  hb_mask_t mask = -1;
  bool ignore_zwnj = false;
  bool ignore_zwj = false;
  bool ignore_hidden = false;
  bool per_syllable = false;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  स्थिर व्योम *match_तथ्य_2 = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
काष्ठा glyph_तथ्य_t
{
  ढाँचा <typename T>
  glyph_तथ्य_t (स्थिर T *तथ्य_2) : तथ्य (तथ्य_2)
#अगर_अघोषित HB_NO_BEYOND_64K
                              , wide (माप (T) == 3)
#पूर्ण_अगर
  {
    static_assert (माप (T) == 2 || माप (T) == 3, "invalid glyph data size");
  }

  अचिन्हित चालक [] (अचिन्हित i) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (wide)
      वापस ((स्थिर HBUINT24 *) तथ्य)[i];
#पूर्ण_अगर
    वापस ((स्थिर HBUINT16 *) तथ्य)[i];
  }

  स्थिर व्योम *तथ्य;
#अगर_अघोषित HB_NO_BEYOND_64K
  bool wide;
#पूर्ण_अगर
};

ढाँचा <typename context_t>
काष्ठा skipping_iterator_t
{
  व्योम init (context_t *c_, bool context_match = false)
  {
    c = c_;
    end = c->बफर->len;
    match_glyph_data16 = nullptr;
#अगर_अघोषित HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#पूर्ण_अगर
    matcher.init (c, context_match);
  }
  व्योम set_lookup_props (अचिन्हित पूर्णांक lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  व्योम set_match_func (matcher_t::match_func_t match_func_,
		       स्थिर व्योम *match_तथ्य)
  {
    matcher.set_match_func (match_func_, match_तथ्य);
  }
  व्योम set_glyph_तथ्य (glyph_तथ्य_t glyph_तथ्य)
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_तथ्य.wide ? nullptr : (स्थिर HBUINT16 *) glyph_तथ्य.तथ्य;
    match_glyph_data24 = glyph_तथ्य.wide ? (स्थिर HBUINT24 *) glyph_तथ्य.तथ्य : nullptr;
#अन्यथा
    match_glyph_data16 = (स्थिर HBUINT16 *) glyph_तथ्य.तथ्य;
#पूर्ण_अगर
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  व्योम reset (अचिन्हित पूर्णांक start_सूचकांक_2)
  {
    // For GSUB forward iterator
    idx = start_सूचकांक_2;
    end = c->बफर->len;
    matcher.syllable = c->बफर->cur().syllable();
  }
  व्योम reset_back (अचिन्हित पूर्णांक start_सूचकांक_2, bool from_out_बफर = false)
  {
    // For GSUB backward iterator
    idx = start_सूचकांक_2;
    matcher.syllable = c->बफर->cur().syllable();
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  व्योम reset_fast (अचिन्हित पूर्णांक start_सूचकांक_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_सूचकांक_2;
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  matcher_t::may_skip_t may_skip (स्थिर hb_glyph_info_t &info) स्थिर
  { वापस matcher.may_skip (c, info); }

  क्रमागत match_t {
    MATCH,
    NOT_MATCH,
    SKIP
  };

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  match_t match (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    अगर (unlikely (skip == matcher_t::SKIP_YES))
      वापस SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_तथ्य ());
    अगर (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      वापस MATCH;

    अगर (skip == matcher_t::SKIP_NO)
      वापस NOT_MATCH;

    वापस SKIP;
  }

  bool next (अचिन्हित *unsafe_to = nullptr)
  {
    स्वतः *info = c->बफर->info;
    स्थिर चिन्हित stop = (चिन्हित) end - 1;
    जबतक ((चिन्हित) idx < stop)
    {
      idx++;
      चयन (match (info[idx]))
      {
	हाल MATCH:
	{
	  advance_glyph_तथ्य ();
	  वापस true;
	}
	हाल NOT_MATCH:
	{
	  अगर (unsafe_to)
	    *unsafe_to = idx + 1;
	  वापस false;
	}
	हाल SKIP:
	  जारी;
      }
    }
    अगर (unsafe_to)
      *unsafe_to = end;
    वापस false;
  }
  bool prev (अचिन्हित *unsafe_from = nullptr)
  {
    स्वतः *out_info = c->बफर->out_info;
    स्थिर अचिन्हित stop = 0;
    जबतक (idx > stop)
    {
      idx--;
      चयन (match (out_info[idx]))
      {
	हाल MATCH:
	{
	  advance_glyph_तथ्य ();
	  वापस true;
	}
	हाल NOT_MATCH:
	{
	  अगर (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  वापस false;
	}
	हाल SKIP:
	  जारी;
      }
    }
    अगर (unsafe_from)
      *unsafe_from = 0;
    वापस false;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_तथ्य ()
  {
    अगर (match_glyph_data16) वापस *match_glyph_data16;
#अगर_अघोषित HB_NO_BEYOND_64K
    अन्यथा
    अगर (match_glyph_data24) वापस *match_glyph_data24;
#पूर्ण_अगर
    वापस 0;
  }
  HB_ALWAYS_INLINE
  व्योम
  advance_glyph_तथ्य ()
  {
    अगर (match_glyph_data16) match_glyph_data16++;
#अगर_अघोषित HB_NO_BEYOND_64K
    अन्यथा
    अगर (match_glyph_data24) match_glyph_data24++;
#पूर्ण_अगर
  }

  अचिन्हित पूर्णांक idx;
  रक्षित:
  context_t *c;
  matcher_t matcher;
  स्थिर HBUINT16 *match_glyph_data16;
#अगर_अघोषित HB_NO_BEYOND_64K
  स्थिर HBUINT24 *match_glyph_data24;
#पूर्ण_अगर

  अचिन्हित पूर्णांक end;
};

काष्ठा hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, bool, HB_DEBUG_APPLY>
{
  स्थिर अक्षर *get_name () { वापस "APPLY"; }
  प्रकार return_t (*recurse_func_t) (hb_ot_apply_context_t *c, अचिन्हित पूर्णांक lookup_सूचकांक);

  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_ (स्थिर T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_ (स्थिर T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { वापस apply_(obj, यह, hb_prioritize); }

  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }
  return_t recurse (अचिन्हित पूर्णांक sub_lookup_सूचकांक)
  {
    निश्चित (recurse_func);
    अगर (unlikely (nesting_level_बायाँ == 0))
    {
      बफर->successful = false;
      वापस default_return_मूल्य ();
    }

    बफर->max_ops--;
    अगर (unlikely (बफर->max_ops < 0))
    {
      बफर->successful = false;
      वापस default_return_मूल्य ();
    }

    nesting_level_बायाँ--;
    bool ret = recurse_func (यह, sub_lookup_सूचकांक);
    nesting_level_बायाँ++;
    वापस ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_निवेश, iter_context;

  अचिन्हित पूर्णांक table_सूचकांक; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_बफर_t *बफर;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  स्थिर GDEF &gdef;
  स्थिर GDEF::accelerator_t &gdef_accel;
  स्थिर ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  अचिन्हित पूर्णांक lookup_सूचकांक = (अचिन्हित) -1;
  अचिन्हित पूर्णांक lookup_props = 0;
  अचिन्हित पूर्णांक nesting_level_बायाँ = HB_MAX_NESTING_LEVEL;

  bool has_glyph_classes;
  bool auto_zwnj = true;
  bool auto_zwj = true;
  bool per_syllable = false;
  bool random = false;
  अचिन्हित new_syllables = (अचिन्हित) -1;

  चिन्हित last_base = -1; // GPOS uses
  अचिन्हित last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t ढेर_match_positions[8];
#अगर_अघोषित HB_NO_बफर_MESSAGE
  hb_बफर_t::changed_func_t orig_changed_func = nullptr;
  व्योम *orig_changed_तथ्य = nullptr;
#पूर्ण_अगर

  hb_ot_apply_context_t (अचिन्हित पूर्णांक table_सूचकांक_2,
			 hb_font_t *font_,
			 hb_बफर_t *बफर_2,
			 hb_blob_t *table_blob_,
			 hb_scalar_cache_t *var_store_cache_ = nullptr) :
			table_सूचकांक (table_सूचकांक_2),
			font (font_), face (font->face), बफर (बफर_2),
			sanitizer (table_blob_),
			gdef (
#अगर_अघोषित HB_NO_OT_LAYOUT
			      *face->table.GDEF->table
#अन्यथा
			      Null (GDEF)
#पूर्ण_अगर
			     ),
			gdef_accel (
#अगर_अघोषित HB_NO_OT_LAYOUT
			      *face->table.GDEF
#अन्यथा
			      Null (GDEF::accelerator_t)
#पूर्ण_अगर
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (बफर_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (ढेर_match_positions);

#अगर_अघोषित HB_NO_बफर_MESSAGE
    अगर (बफर->messaging ())
    {
      निश्चित (बफर->changed_func == nullptr ||
	      बफर->changed_func == बफर_changed_trampoline);
      orig_changed_func = बफर->changed_func;
      orig_changed_तथ्य = बफर->changed_तथ्य;
      बफर->changed_func = बफर_changed_trampoline;
      बफर->changed_तथ्य = यह;
    }
#पूर्ण_अगर
  }
  ~hb_ot_apply_context_t ()
  {
#अगर_अघोषित HB_NO_बफर_MESSAGE
    अगर (बफर->messaging ())
    {
      निश्चित (बफर->changed_func == बफर_changed_trampoline);
      निश्चित (बफर->changed_तथ्य == यह);
      बफर->changed_func = orig_changed_func;
      बफर->changed_तथ्य = orig_changed_तथ्य;
    }
#पूर्ण_अगर
  }

  व्योम init_iters ()
  {
    iter_निवेश.init (यह, false);
    iter_context.init (यह, true);
  }

  व्योम set_lookup_mask (hb_mask_t mask, bool init = true) { lookup_mask = mask; last_base = -1; last_base_until = 0; अगर (init) init_iters (); }
  व्योम set_auto_zwj (bool auto_zwj_, bool init = true) { auto_zwj = auto_zwj_; अगर (init) init_iters (); }
  व्योम set_auto_zwnj (bool auto_zwnj_, bool init = true) { auto_zwnj = auto_zwnj_; अगर (init) init_iters (); }
  व्योम set_per_syllable (bool per_syllable_, bool init = true) { per_syllable = per_syllable_; अगर (init) init_iters (); }
  व्योम set_random (bool random_) { random = random_; }
  व्योम set_recurse_func (recurse_func_t func) { recurse_func = func; }
  व्योम set_lookup_सूचकांक (अचिन्हित पूर्णांक lookup_सूचकांक_3) { lookup_सूचकांक = lookup_सूचकांक_3; }
  व्योम set_lookup_props (अचिन्हित पूर्णांक lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_संख्या ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    बफर->random_state = बफर->random_state * 48271 % 2147483647;
    वापस बफर->random_state;
  }

  अटल व्योम बफर_changed_trampoline (hb_बफर_t *बफर HB_UNUSED, व्योम *user_तथ्य)
  {
    ((hb_ot_apply_context_t *) user_तथ्य)->बफर_changed ();
  }
  व्योम बफर_changed ()
  {
    बफर->update_digest ();
    अगर (likely (has_glyph_classes))
      क्रम (अचिन्हित i = 0; i < बफर->len; i++)
	_set_glyph_class_props (बफर->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  bool match_properties_mark (स्थिर hb_glyph_info_t *info,
			      अचिन्हित पूर्णांक    glyph_props,
			      अचिन्हित पूर्णांक    match_props) स्थिर
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    अगर (match_props & LookupFlag::UseMarkFilteringSet)
    {
      अचिन्हित set_सूचकांक = match_props >> 16;
      वापस gdef_accel.mark_set_covers (set_सूचकांक, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    अगर (match_props & LookupFlag::MarkAttachmentType)
      वापस (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    वापस true;
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool check_glyph_property (स्थिर hb_glyph_info_t *info,
			     अचिन्हित match_props) स्थिर
  {
    अचिन्हित पूर्णांक glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    अगर (glyph_props & match_props & LookupFlag::IgnoreFlags)
      वापस false;

    अगर (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      वापस match_properties_mark (info, glyph_props, match_props);

    वापस true;
  }

  व्योम _set_glyph_class (hb_codepoint_t glyph_सूचकांक,
			  अचिन्हित पूर्णांक class_guess = 0,
			  bool ligature = false,
			  bool component = false)
  {
    बफर->digest.add (glyph_सूचकांक);

    अगर (new_syllables != (अचिन्हित) -1)
      बफर->cur().syllable() = new_syllables;

    अचिन्हित पूर्णांक props = _hb_glyph_info_get_glyph_props (&बफर->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    अगर (ligature)
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
    अगर (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    अगर (likely (has_glyph_classes))
    {
      _set_glyph_class_props (बफर->cur(), props, glyph_सूचकांक);
    }
    अन्यथा अगर (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&बफर->cur(), props | class_guess);
    }
    अन्यथा
      _hb_glyph_info_set_glyph_props (&बफर->cur(), props);
  }
  व्योम _set_glyph_class_props (hb_glyph_info_t &info,
			       अचिन्हित पूर्णांक props,
			       hb_codepoint_t glyph_सूचकांक)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_सूचकांक));
  }
  व्योम _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  व्योम replace_glyph (hb_codepoint_t glyph_सूचकांक)
  {
    _set_glyph_class (glyph_सूचकांक);
    (व्योम) बफर->replace_glyph (glyph_सूचकांक);
  }
  व्योम replace_glyph_inplace (hb_codepoint_t glyph_सूचकांक)
  {
    _set_glyph_class (glyph_सूचकांक);
    बफर->cur().codepoint = glyph_सूचकांक;
  }
  व्योम replace_glyph_with_ligature (hb_codepoint_t glyph_सूचकांक,
				    अचिन्हित पूर्णांक class_guess)
  {
    _set_glyph_class (glyph_सूचकांक, class_guess, true);
    (व्योम) बफर->replace_glyph (glyph_सूचकांक);
  }
  व्योम output_glyph_for_component (hb_codepoint_t glyph_सूचकांक,
				   अचिन्हित पूर्णांक class_guess)
  {
    _set_glyph_class (glyph_सूचकांक, class_guess, false, true);
    (व्योम) बफर->output_glyph (glyph_सूचकांक);
  }
};

क्रमागत श्रेणी hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

काष्ठा hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ढाँचा <typename T>
  अटल अंतरभूत स्वतः collect_second_glyphs_ (स्थिर T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (व्योम, obj.collect_second_glyphs (digest) )
  ढाँचा <typename T>
  अटल अंतरभूत व्योम collect_second_glyphs_ (स्थिर T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_ (स्थिर T *obj, hb_ot_apply_context_t *c, व्योम *external_cache, hb_priority<1>) HB_RETURN (bool, obj->apply (c, external_cache) )
  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_ (स्थिर T *obj, hb_ot_apply_context_t *c, व्योम *external_cache, hb_priority<0>) HB_RETURN (bool, obj->apply (c) )
  ढाँचा <typename T>
  अटल अंतरभूत bool apply_to (स्थिर व्योम *obj, hb_ot_apply_context_t *c, व्योम *external_cache)
  {
    स्थिर T *typed_obj = (स्थिर T *) obj;
    वापस apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_cached_ (स्थिर T *obj, hb_ot_apply_context_t *c, व्योम *external_cache, hb_priority<2>) HB_RETURN (bool, obj->apply_cached (c, external_cache) )
  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_cached_ (स्थिर T *obj, hb_ot_apply_context_t *c, व्योम *external_cache, hb_priority<1>) HB_RETURN (bool, obj->apply (c, external_cache) )
  ढाँचा <typename T>
  अटल अंतरभूत स्वतः apply_cached_ (स्थिर T *obj, hb_ot_apply_context_t *c, व्योम *external_cache, hb_priority<0>) HB_RETURN (bool, obj->apply (c) )
  ढाँचा <typename T>
  अटल अंतरभूत bool apply_cached_to (स्थिर व्योम *obj, hb_ot_apply_context_t *c, व्योम *external_cache)
  {
    स्थिर T *typed_obj = (स्थिर T *) obj;
    वापस apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  ढाँचा <typename T>
  अटल अंतरभूत स्वतः cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (bool, T::cache_func (c, op) )
  ढाँचा <typename T=व्योम>
  अटल अंतरभूत bool cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { वापस false; }
  ढाँचा <typename Type>
  अटल अंतरभूत bool cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    वापस cache_func_<Type> (c, op, hb_prioritize);
  }
#पूर्ण_अगर

  प्रकार bool (*hb_apply_func_t) (स्थिर व्योम *obj, hb_ot_apply_context_t *c, व्योम *external_cache);
  प्रकार bool (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  काष्ठा hb_applicable_t
  {
    मित्र काष्ठा hb_accelerate_subtables_context_t;
    मित्र काष्ठा hb_ot_layout_lookup_accelerator_t;

    ढाँचा <typename T>
    व्योम init (स्थिर T &obj_,
	       hb_apply_func_t apply_func_
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , व्योम *external_cache_
#पूर्ण_अगर
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#पूर्ण_अगर
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#अगर_घोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    bool apply (hb_ot_apply_context_t *c) स्थिर
    {
      वापस digest.may_have (c->बफर->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    bool apply_no_digest (hb_ot_apply_context_t *c) स्थिर
    {
      वापस apply_func (obj, c, nullptr);
    }
#अन्यथा
    bool apply (hb_ot_apply_context_t *c) स्थिर
    {
      वापस digest.may_have (c->बफर->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    bool apply_cached (hb_ot_apply_context_t *c) स्थिर
    {
      वापस digest.may_have (c->बफर->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    bool apply_no_digest (hb_ot_apply_context_t *c) स्थिर
    {
      वापस apply_func (obj, c, external_cache);
    }
    bool apply_cached_no_digest (hb_ot_apply_context_t *c) स्थिर
    {
      वापस apply_cached_func (obj, c, external_cache);
    }

    bool cache_enter (hb_ot_apply_context_t *c) स्थिर
    {
      वापस cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    व्योम cache_leave (hb_ot_apply_context_t *c) स्थिर
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#पूर्ण_अगर

    निजी:
    स्थिर व्योम *obj;
    hb_apply_func_t apply_func;
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    व्योम *external_cache;
#पूर्ण_अगर
    hb_set_digest_t digest;
  };

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ढाँचा <typename T>
  स्वतः cache_cost (स्थिर T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  ढाँचा <typename T>
  स्वतः cache_cost (स्थिर T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  ढाँचा <typename T>
  स्वतः external_cache_create (स्थिर T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  ढाँचा <typename T>
  स्वतः external_cache_create (स्थिर T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#पूर्ण_अगर

  /* Dispatch interface. */
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    व्योम *external_cache = nullptr;
    अगर (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#पूर्ण_अगर

    hb_applicable_t *entry = &array[i++];

    entry->init (obj,
		 apply_to<T>
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_to<T>
		 , cache_func_to<T>
		 , external_cache
#पूर्ण_अगर
		 );

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    अचिन्हित cost = cache_cost (obj, hb_prioritize);
    अगर (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#पूर्ण_अगर

    वापस hb_empty_t ();
  }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  अचिन्हित i = 0;
  hb_set_digest_t digest_second;

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
  अचिन्हित subtable_cache_user_idx = (अचिन्हित) -1;
  अचिन्हित subtable_cache_user_cost = 0;
#पूर्ण_अगर
};


प्रकार bool (*intersects_func_t) (स्थिर hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य, व्योम *cache);
प्रकार व्योम (*intersected_glyphs_func_t) (स्थिर hb_set_t *glyphs, स्थिर व्योम *तथ्य, अचिन्हित value, hb_set_t *intersected_glyphs, व्योम *cache);
प्रकार व्योम (*collect_glyphs_func_t) (hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य);
प्रकार bool (*match_func_t) (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य);
प्रकार अचिन्हित (*get_मूल्य_func_t) (hb_glyph_info_t &info, स्थिर व्योम *तथ्य);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
काष्ठा hb_ot_layout_ruleset_digest_t
{
  व्योम init () { mask = 0; }
  व्योम set_full () { mask = (uint64_t) -1; }
  व्योम add (अचिन्हित value) { mask |= (uint64_t) 1 << (value & 63); }
  bool may_have (अचिन्हित value) स्थिर { वापस mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

काष्ठा ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
काष्ठा ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
काष्ठा ContextApplyFuncs
{
  match_func_t match;
};
काष्ठा ChainContextApplyFuncs
{
  match_func_t match[3];
};


अटल अंतरभूत bool intersects_glyph (स्थिर hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य HB_UNUSED, व्योम *cache HB_UNUSED)
{
  वापस glyphs->has (value);
}
अटल अंतरभूत bool intersects_class (स्थिर hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य, व्योम *cache)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  अगर (map->has (value, &cached_v))
    वापस *cached_v;

  bool v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  वापस v;
}
अटल अंतरभूत bool intersects_coverage (स्थिर hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य, व्योम *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  वापस (तथ्य+coverage).intersects (glyphs);
}


अटल अंतरभूत व्योम intersected_glyph (स्थिर hb_set_t *glyphs HB_UNUSED, स्थिर व्योम *तथ्य, अचिन्हित value, hb_set_t *intersected_glyphs, HB_UNUSED व्योम *cache)
{
  अचिन्हित g = reinterpret_cast<स्थिर HBUINT16 *>(तथ्य)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<अचिन्हित, hb_set_t>;

अटल अंतरभूत व्योम intersected_class_glyphs (स्थिर hb_set_t *glyphs, स्थिर व्योम *तथ्य, अचिन्हित value, hb_set_t *intersected_glyphs, व्योम *cache)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  अगर (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    वापस;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

अटल अंतरभूत व्योम intersected_coverage_glyphs (स्थिर hb_set_t *glyphs, स्थिर व्योम *तथ्य, अचिन्हित value, hb_set_t *intersected_glyphs, HB_UNUSED व्योम *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (तथ्य+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


अटल अंतरभूत bool array_is_subset_of (स्थिर hb_set_t *glyphs,
				       अचिन्हित पूर्णांक गिनती,
				       glyph_तथ्य_t values,
				       intersects_func_t intersects_func,
				       स्थिर व्योम *intersects_तथ्य,
				       व्योम *cache)
{
  क्रम (अचिन्हित i = 0; i < गिनती; i++)
    अगर (!intersects_func (glyphs, values[i], intersects_तथ्य, cache)) वापस false;
  वापस true;
}


अटल अंतरभूत व्योम collect_glyph (hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य HB_UNUSED)
{
  glyphs->add (value);
}
अटल अंतरभूत व्योम collect_class (hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);
  class_def.collect_class (glyphs, value);
}
अटल अंतरभूत व्योम collect_coverage (hb_set_t *glyphs, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (तथ्य+coverage).collect_coverage (glyphs);
}

अंतरभूत स्थिर hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (स्थिर व्योम *तथ्य,
                                                अचिन्हित value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_कुंजी_t key (तथ्य, value, type);
  hb_codepoint_t *existing = nullptr;
  अगर (glyph_set_to_सूचकांक.has (key, &existing))
    वापस glyph_sets[*existing].लो ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  अगर (unlikely (!set))
  {
    depend_तथ्य->हार ();
    वापस nullptr;
  }

  अगर (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, तथ्य);
  अन्यथा अगर (type == GLYPH_SET_CLASS)
    collect_class (set, value, तथ्य);
  अन्यथा
    set->add (value);
  अगर (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_तथ्य->हार ();
    वापस nullptr;
  }

  hb_codepoint_t सूचकांक_2 = glyph_sets.लंबाई;
  अगर (unlikely (!glyph_sets.धकेलो_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_तथ्य->हार ();
    वापस nullptr;
  }
  अगर (unlikely (!glyph_set_to_सूचकांक.set (key, सूचकांक_2)))
  {
    depend_तथ्य->हार ();
    वापस nullptr;
  }
  वापस glyph_sets[सूचकांक_2].लो ();
}

अंतरभूत hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (स्थिर hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  अगर (cached_context_set_indices.has (key, &existing))
    वापस *existing;

  hb_codepoint_t सूचकांक_2 = depend_तथ्य->find_or_create_context_set (*set);
  अगर (unlikely (सूचकांक_2 == HB_CODEPOINT_INVALID))
    वापस HB_CODEPOINT_INVALID;
  अगर (unlikely (!cached_context_set_indices.set (key, सूचकांक_2)))
    वापस depend_तथ्य->fail_invalid ();
  वापस सूचकांक_2;
}

अटल अंतरभूत व्योम collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  अचिन्हित पूर्णांक गिनती,
				  glyph_तथ्य_t values,
				  collect_glyphs_func_t collect_func,
				  स्थिर व्योम *collect_तथ्य)
{
  क्रम (अचिन्हित i = 0; i < गिनती; i++)
    collect_func (glyphs, values[i], collect_तथ्य);
}


अटल अंतरभूत bool match_always (hb_glyph_info_t &info HB_UNUSED, अचिन्हित value HB_UNUSED, स्थिर व्योम *तथ्य HB_UNUSED)
{
  वापस true;
}
अटल अंतरभूत bool match_glyph (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य HB_UNUSED)
{
  वापस info.codepoint == value;
}
अटल अंतरभूत bool match_class (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);
  वापस class_def.get_class (info.codepoint) == value;
}
अटल अंतरभूत अचिन्हित get_class_मूल्य (hb_glyph_info_t &info, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *> (तथ्य);
  वापस class_def.get_class (info.codepoint);
}
काष्ठा match_class_cache_तथ्य_t
{
  स्थिर ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
अटल अंतरभूत bool match_class_cache (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  स्थिर स्वतः &cache_तथ्य = *reinterpret_cast<स्थिर match_class_cache_तथ्य_t *> (तथ्य);
  वापस cache_तथ्य.class_def->get_class (info.codepoint, cache_तथ्य.cache) == value;
}
अटल अंतरभूत अचिन्हित get_class_cache_मूल्य (hb_glyph_info_t &info, स्थिर व्योम *तथ्य)
{
  स्थिर स्वतः &cache_तथ्य = *reinterpret_cast<स्थिर match_class_cache_तथ्य_t *> (तथ्य);
  वापस cache_तथ्य.class_def->get_class (info.codepoint, cache_तथ्य.cache);
}
अटल अंतरभूत अचिन्हित get_class_cached (स्थिर ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = info.syllable();
  अगर (klass < 255)
    वापस klass;
  klass = class_def.get_class (info.codepoint);
  अगर (likely (klass < 255))
    info.syllable() = klass;
  वापस klass;
}
अटल अंतरभूत bool match_class_cached (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);
  वापस get_class_cached (class_def, info) == value;
}
अटल अंतरभूत अचिन्हित get_class_cached_मूल्य (hb_glyph_info_t &info, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *> (तथ्य);
  वापस get_class_cached (class_def, info);
}
अटल अंतरभूत अचिन्हित get_class_cached1 (स्थिर ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = info.syllable() & 0x0F;
  अगर (klass < 15)
    वापस klass;
  klass = class_def.get_class (info.codepoint);
  अगर (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  वापस klass;
}
अटल अंतरभूत bool match_class_cached1 (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);
  वापस get_class_cached1 (class_def, info) == value;
}
अटल अंतरभूत अचिन्हित get_class_cached2 (स्थिर ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = (info.syllable() & 0xF0) >> 4;
  अगर (klass < 15)
    वापस klass;
  klass = class_def.get_class (info.codepoint);
  अगर (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  वापस klass;
}
अटल अंतरभूत bool match_class_cached2 (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *>(तथ्य);
  वापस get_class_cached2 (class_def, info) == value;
}
अटल अंतरभूत अचिन्हित get_class_cached2_मूल्य (hb_glyph_info_t &info, स्थिर व्योम *तथ्य)
{
  स्थिर ClassDef &class_def = *reinterpret_cast<स्थिर ClassDef *> (तथ्य);
  वापस get_class_cached2 (class_def, info);
}
अटल अंतरभूत bool match_coverage (hb_glyph_info_t &info, अचिन्हित value, स्थिर व्योम *तथ्य)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  वापस (तथ्य+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

अटल अंतरभूत bool would_match_निवेश (hb_would_apply_context_t *c,
				      अचिन्हित पूर्णांक गिनती, /* Including the first glyph (not matched) */
				      glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      स्थिर व्योम *match_तथ्य_2)
{
  अगर (गिनती != c->len)
    वापस false;

  क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    अगर (likely (!match_func (info, निवेश[i - 1], match_तथ्य_2)))
      वापस false;
  }

  वापस true;
}
#अगर_अघोषित HB_OPTIMIZE_आकार
HB_ALWAYS_INLINE
#अन्यथा
अंतरभूत
#पूर्ण_अगर
अटल bool match_निवेश (hb_ot_apply_context_t *c,
			 अचिन्हित पूर्णांक गिनती, /* Including the first glyph (not matched) */
			 glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 स्थिर व्योम *match_तथ्य_2,
			 अचिन्हित पूर्णांक *end_position,
			 अचिन्हित पूर्णांक *p_योग_component_गिनती = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_बफर_t *बफर = c->बफर;

  अगर (गिनती == 1)
  {
    *end_position = बफर->idx + 1;
    c->match_positions[0] = बफर->idx;
    अगर (p_योग_component_गिनती)
      *p_योग_component_गिनती = hb_glyph_info_get_lig_अंक_comps (&बफर->cur());
    return_trace (true);
  }

  अगर (unlikely (गिनती > HB_MAX_CONTEXT_लंबाई)) return_trace (false);

  स्वतः &skippy_iter = c->iter_निवेश;
  skippy_iter.reset (बफर->idx);
  skippy_iter.set_match_func (match_func, match_तथ्य_2);
  skippy_iter.set_glyph_तथ्य (निवेश);

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

  अचिन्हित पूर्णांक योग_component_गिनती = 0;

  अचिन्हित पूर्णांक first_lig_id = _hb_glyph_info_get_lig_id (&बफर->cur());
  अचिन्हित पूर्णांक first_lig_comp = _hb_glyph_info_get_lig_comp (&बफर->cur());

  क्रमागत {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
  {
    अचिन्हित unsafe_to;
    अगर (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (false);
    }

    अगर (unlikely (i + 1 > c->match_positions.लंबाई &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (false);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    अचिन्हित पूर्णांक this_lig_id = _hb_glyph_info_get_lig_id (&बफर->info[skippy_iter.idx]);
    अचिन्हित पूर्णांक this_lig_comp = _hb_glyph_info_get_lig_comp (&बफर->info[skippy_iter.idx]);

    अगर (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      अगर (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	अगर (ligbase == LIGBASE_NOT_CHECKED)
	{
	  bool found = false;
	  स्थिर स्वतः *out = बफर->out_info;
	  अचिन्हित पूर्णांक j = बफर->out_len;
	  जबतक (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    अगर (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = true;
	      अवरोध;
	    }
	    j--;
	  }

	  अगर (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  अन्यथा
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	अगर (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (false);
      }
    }
    अन्यथा
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      अगर (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (false);
    }

    योग_component_गिनती += hb_glyph_info_get_lig_अंक_comps_in_ligation (&बफर->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  अगर (p_योग_component_गिनती)
  {
    योग_component_गिनती += hb_glyph_info_get_lig_अंक_comps (&बफर->cur());
    *p_योग_component_गिनती = योग_component_गिनती;
  }

  c->match_positions.arrayZ[0] = बफर->idx;

  return_trace (true);
}
अटल अंतरभूत bool ligate_निवेश (hb_ot_apply_context_t *c,
				 अचिन्हित पूर्णांक गिनती, /* Including the first glyph */
				 अचिन्हित पूर्णांक match_end,
				 hb_codepoint_t lig_glyph,
				 अचिन्हित पूर्णांक योग_component_गिनती)
{
  TRACE_APPLY (nullptr);

  hb_बफर_t *बफर = c->बफर;

  बफर->merge_clusters (बफर->idx, match_end);

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

  bool is_base_ligature = _hb_glyph_info_is_base_glyph (&बफर->info[c->match_positions.arrayZ[0]]);
  bool is_mark_ligature = _hb_glyph_info_is_mark (&बफर->info[c->match_positions.arrayZ[0]]);
  क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
    अगर (!_hb_glyph_info_is_mark (&बफर->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = false;
      is_mark_ligature = false;
      अवरोध;
    }
  bool is_ligature = !is_base_ligature && !is_mark_ligature;

  अचिन्हित पूर्णांक klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  अचिन्हित पूर्णांक lig_id = is_ligature ? _hb_allocate_lig_id (बफर) : 0;
  अचिन्हित पूर्णांक last_lig_id = _hb_glyph_info_get_lig_id (&बफर->cur());
  अचिन्हित पूर्णांक last_अंक_components = hb_glyph_info_get_lig_अंक_comps (&बफर->cur());
  अचिन्हित पूर्णांक components_so_far = last_अंक_components;

  अगर (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&बफर->cur(), lig_id, योग_component_गिनती);
    अगर (_hb_glyph_info_get_general_category (&बफर->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&बफर->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
  {
    जबतक (बफर->idx < c->match_positions.arrayZ[i] && बफर->successful)
    {
      अगर (is_ligature)
      {
	अचिन्हित पूर्णांक this_comp = _hb_glyph_info_get_lig_comp (&बफर->cur());
	अगर (this_comp == 0)
	  this_comp = last_अंक_components;
	निश्चित (components_so_far >= last_अंक_components);
	अचिन्हित पूर्णांक new_lig_comp = components_so_far - last_अंक_components +
				    hb_min (this_comp, last_अंक_components);
	  _hb_glyph_info_set_lig_props_for_mark (&बफर->cur(), lig_id, new_lig_comp);
      }
      (व्योम) बफर->अग्र_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&बफर->cur());
    last_अंक_components = hb_glyph_info_get_lig_अंक_comps_in_ligation (&बफर->cur());
    components_so_far += last_अंक_components;

    /* Skip the base glyph */
    बफर->idx++;
  }

  अगर (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    क्रम (अचिन्हित i = बफर->idx; i < बफर->len; ++i)
    {
      अगर (last_lig_id != _hb_glyph_info_get_lig_id (&बफर->info[i])) अवरोध;

      अचिन्हित this_comp = _hb_glyph_info_get_lig_comp (&बफर->info[i]);
      अगर (!this_comp) अवरोध;

      निश्चित (components_so_far >= last_अंक_components);
      अचिन्हित new_lig_comp = components_so_far - last_अंक_components +
			      hb_min (this_comp, last_अंक_components);
      _hb_glyph_info_set_lig_props_for_mark (&बफर->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (true);
}

#अगर_अघोषित HB_OPTIMIZE_आकार
HB_ALWAYS_INLINE
#अन्यथा
अंतरभूत
#पूर्ण_अगर
अटल bool match_backtrack (hb_ot_apply_context_t *c,
			     अचिन्हित पूर्णांक गिनती,
			     glyph_तथ्य_t backtrack,
			     match_func_t match_func,
			     स्थिर व्योम *match_तथ्य_2,
			     अचिन्हित पूर्णांक *match_start)
{
  TRACE_APPLY (nullptr);

  अगर (!गिनती)
  {
    *match_start = c->बफर->backtrack_len ();
    return_trace (true);
  }

  स्वतः &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->बफर->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_तथ्य_2);
  skippy_iter.set_glyph_तथ्य (backtrack);

  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    अचिन्हित unsafe_from;
    अगर (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (false);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (true);
}

#अगर_अघोषित HB_OPTIMIZE_आकार
HB_ALWAYS_INLINE
#अन्यथा
अंतरभूत
#पूर्ण_अगर
अटल bool match_lookahead (hb_ot_apply_context_t *c,
			     अचिन्हित पूर्णांक गिनती,
			     glyph_तथ्य_t lookahead,
			     match_func_t match_func,
			     स्थिर व्योम *match_तथ्य_2,
			     अचिन्हित पूर्णांक start_सूचकांक,
			     अचिन्हित पूर्णांक *end_सूचकांक)
{
  TRACE_APPLY (nullptr);

  अगर (!गिनती)
  {
    *end_सूचकांक = start_सूचकांक;
    return_trace (true);
  }

  स्वतः &skippy_iter = c->iter_context;
  निश्चित (start_सूचकांक >= 1);
  skippy_iter.reset (start_सूचकांक - 1);
  skippy_iter.set_match_func (match_func, match_तथ्य_2);
  skippy_iter.set_glyph_तथ्य (lookahead);

  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    अचिन्हित unsafe_to;
    अगर (!skippy_iter.next (&unsafe_to))
    {
      *end_सूचकांक = unsafe_to;
      return_trace (false);
    }
  }

  *end_सूचकांक = skippy_iter.idx + 1;
  return_trace (true);
}



काष्ठा LookupRecord
{
  bool serialize (hb_serialize_context_t *c,
		  स्थिर hb_map_t         *lookup_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    return_trace (c->check_assign (out->lookup_List_सूचकांक, lookup_map->लो (lookup_List_सूचकांक), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT16	sequence_सूचकांक;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_सूचकांक;	/* Lookup to apply to that
					 * position--zero--based */
  खुला:
  DEFINE_आकार_STATIC (4);
};

अटल अंतरभूत अचिन्हित serialize_lookuprecord_array (hb_serialize_context_t *c,
						     स्थिर hb_array_t<स्थिर LookupRecord> lookupRecords,
						     स्थिर hb_map_t *lookup_map)
{
  अचिन्हित गिनती = 0;
  क्रम (स्थिर LookupRecord& r : lookupRecords)
  {
    अगर (!lookup_map->has (r.lookup_List_सूचकांक))
      जारी;

    अगर (!r.serialize (c, lookup_map))
      वापस 0;

    गिनती++;
  }
  वापस गिनती;
}

क्रमागत ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

अटल अंतरभूत स्थिर hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			स्थिर व्योम *तथ्य,
			अचिन्हित value,
			bool filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<अचिन्हित> *compact_workspace = nullptr)
{
  चयन (context_format)
  {
    हाल ContextFormat::SimpleContext:
      वापस c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    हाल ContextFormat::ClassBasedContext:
    {
      स्थिर hb_set_t *class_set = c->find_or_create_glyph_set (
        तथ्य, value, hb_depend_context_t::GLYPH_SET_CLASS);
      अगर (unlikely (!class_set))
	वापस nullptr;
      अगर (!filter_class_by_parent)
	वापस class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      अवरोध;
    }
    हाल ContextFormat::CoverageBasedContext:
      वापस c->find_or_create_glyph_set (
        तथ्य, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  अगर (unlikely (scratch->in_error ()))
  {
    c->depend_तथ्य->हार ();
    वापस nullptr;
  }
  वापस scratch;
}

अटल अंतरभूत व्योम context_closure_recurse_lookups (hb_closure_context_t *c,
					     अचिन्हित निवेश_गिनती, glyph_तथ्य_t निवेश,
					     अचिन्हित lookup_गिनती_2,
					     स्थिर LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     अचिन्हित value,
					     ContextFormat context_format,
					     स्थिर व्योम *तथ्य,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     व्योम *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  क्रम (अचिन्हित पूर्णांक i = 0; i < lookup_गिनती_2; i++)
  {
    अचिन्हित seq_सूचकांक_2 = lookupRecord[i].sequence_सूचकांक;
    अगर (seq_सूचकांक_2 >= निवेश_गिनती) जारी;

    bool has_pos_glyphs = false;

    अगर (!covered_seq_indicies.has (seq_सूचकांक_2))
    {
      has_pos_glyphs = true;
      pos_glyphs.सफा ();
      अगर (seq_सूचकांक_2 == 0)
      {
        चयन (context_format) {
        हाल ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          अवरोध;
        हाल ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), तथ्य, value, &pos_glyphs, cache);
          अवरोध;
        हाल ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          अवरोध;
        }
      }
      अन्यथा
      {
        स्थिर व्योम *निवेश_तथ्य = निवेश.तथ्य;
        अचिन्हित निवेश_मूल्य = seq_सूचकांक_2 - 1;
        अगर (context_format != ContextFormat::SimpleContext)
        {
          निवेश_तथ्य = तथ्य;
          निवेश_मूल्य = निवेश[seq_सूचकांक_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, निवेश_तथ्य, निवेश_मूल्य, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seq_सूचकांक_2);
    hb_set_t *cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs))
      वापस;
    अगर (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } अन्यथा {
      *cur_active_glyphs = *c->glyphs;
    }

    अचिन्हित end_सूचकांक_2 = निवेश_गिनती;
    अगर (context_format == ContextFormat::CoverageBasedContext)
      end_सूचकांक_2 += 1;

    c->recurse (lookupRecord[i].lookup_List_सूचकांक, &covered_seq_indicies, seq_सूचकांक_2, end_सूचकांक_2);

    c->निकालो_cur_done_glyphs ();
  }
}

अटल bool
context_depend_sets_are_cached (hb_depend_context_t *c,
				अचिन्हित निवेश_गिनती_2,
				अचिन्हित lookup_गिनती,
				स्थिर LookupRecord lookup_records[],
				स्थिर व्योम *rule,
				अचिन्हित value,
				ContextFormat context_format,
				स्थिर व्योम *तथ्य,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  bool have_active_idx = false;
  क्रम (अचिन्हित i = 0; i < lookup_गिनती; i++)
  {
    अगर (lookup_records[i].sequence_सूचकांक >= निवेश_गिनती_2)
      जारी;

    अगर (!have_active_idx)
    {
      अगर (unlikely (!c->get_parent_active_glyphs_सूचकांक (&active_idx)))
	वापस false;
      have_active_idx = true;
    }

    hb_depend_context_t::context_set_cache_कुंजी_t key (
      rule, &lookup_records[i], तथ्य, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    अगर (!c->context_set_cache.has (key, &cached_context_set))
      वापस false;
    अगर (unlikely (!cached_context_sets->धकेलो_or_fail (*cached_context_set)))
      वापस c->depend_तथ्य->हार ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  वापस have_active_idx;
}

अटल अंतरभूत व्योम context_depend_recurse_lookups (hb_depend_context_t *c,
					     अचिन्हित निवेश_गिनती, glyph_तथ्य_t निवेश,
					     अचिन्हित lookup_गिनती_2,
					     स्थिर LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     स्थिर व्योम *rule,
					     अचिन्हित value,
					     ContextFormat context_format,
					     स्थिर व्योम *तथ्य,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     व्योम *cache,
					     स्थिर hb_set_t &preliminary_context,
					     स्थिर hb_vector_t<स्थिर hb_set_t *> *निवेश_position_glyphs,
					     स्थिर hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  अगर (unlikely (निवेश_गिनती > HB_MAX_CONTEXT_लंबाई))
    वापस;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  अचिन्हित cached_context_सूचकांक = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < lookup_गिनती_2; i++)
  {
    अचिन्हित seq_सूचकांक_2 = lookupRecord[i].sequence_सूचकांक;
    अगर (seq_सूचकांक_2 >= निवेश_गिनती)
      जारी;

    hb_depend_context_t::context_set_cache_कुंजी_t context_cache_कुंजी;
    अगर (unlikely (!c->get_context_set_cache_कुंजी (rule, &lookupRecord[i], तथ्य, value,
						 context_format, &context_cache_कुंजी)))
      वापस;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    अगर (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_सूचकांक++];
    अन्यथा अगर (c->context_set_cache.has (context_cache_कुंजी, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    अन्यथा
    {
      अगर (unlikely (!निवेश_position_glyphs))
      {
	c->depend_तथ्य->हार ();
	वापस;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.सफा ();  // Will hold direct glyphs
      disjunctive_indices.सफा ();  // Will hold encoded set references
      filtered_disjunctive_indices.सफा ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_मूल्य_INVALID;
      जबतक (preliminary_context.next (&elem)) {
	अगर (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} अन्यथा {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      क्रम (अचिन्हित j = 0; j < निवेश_position_glyphs->लंबाई; j++) {
	अगर (j != seq_सूचकांक_2 && (*निवेश_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*निवेश_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_मूल्य_INVALID;
      जबतक (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	स्थिर hb_set_t *original_set = c->depend_तथ्य->get_set_from_सूचकांक (set_idx);
	अगर (unlikely (!original_set)) जारी;

	/* Dependency context sets are never inverted. */
	अगर (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      क्रम (अचिन्हित j = 0; j < निवेश_position_glyphs->लंबाई; j++) {
	अगर (j != seq_सूचकांक_2 && (*निवेश_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  अगर (!(*निवेश_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    bool cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*निवेश_position_glyphs)[j])
	      : c->depend_तथ्य->find_or_create_context_set (*(*निवेश_position_glyphs)[j]);
	    अगर (unlikely (idx == HB_CODEPOINT_INVALID))
	      वापस;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      अगर (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_तथ्य->हार ();
	वापस;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_तथ्य->find_or_create_context_set (position_context);
      अगर (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	वापस;

      अगर (unlikely (!c->context_set_cache.set (context_cache_कुंजी, context_set_idx)))
      {
	c->depend_तथ्य->हार ();
	वापस;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_set_सूचकांक = c->depend_तथ्य->current_context_set_सूचकांक;
    hb_subset_depend_edge_flags_t saved_edge_flags = c->depend_तथ्य->current_edge_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    bool nested_contextual = (saved_context_set_सूचकांक != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_तथ्य->current_context_set_सूचकांक = context_set_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_तथ्य->current_edge_flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE;
    अगर (निवेश_गिनती > 1) {
      c->depend_तथ्य->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    अगर (nested_contextual) {
      c->depend_तथ्य->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    bool has_pos_glyphs = false;

    अगर (!covered_seq_indices.has (seq_सूचकांक_2))
    {
      has_pos_glyphs = true;
      pos_glyphs.सफा ();
      अगर (seq_सूचकांक_2 == 0)
      {
        चयन (context_format) {
        हाल ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          अवरोध;
        हाल ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), तथ्य, value, &pos_glyphs, cache);
          अवरोध;
        हाल ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          अवरोध;
        }
      }
      अन्यथा
      {
        स्थिर व्योम *निवेश_तथ्य = निवेश.तथ्य;
        अचिन्हित निवेश_मूल्य = seq_सूचकांक_2 - 1;
        अगर (context_format != ContextFormat::SimpleContext)
        {
          निवेश_तथ्य = तथ्य;
          निवेश_मूल्य = निवेश[seq_सूचकांक_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, निवेश_तथ्य, निवेश_मूल्य, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seq_सूचकांक_2);
    अगर (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      अगर (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_सूचकांक_2 ? pos_glyphs.get_min () : value;
	अगर (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->धकेलो_cur_active_glyphs (active_idx);
      अगर (unlikely (!cur_active_glyphs))
      {
	c->depend_तथ्य->current_context_set_सूचकांक = saved_context_set_सूचकांक;
	c->depend_तथ्य->current_edge_flags = saved_edge_flags;
	वापस;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    अन्यथा अगर (unlikely (!c->धकेलो_मूल_active_glyphs ()))
    {
      c->depend_तथ्य->current_context_set_सूचकांक = saved_context_set_सूचकांक;
      c->depend_तथ्य->current_edge_flags = saved_edge_flags;
      वापस;
    }

    अचिन्हित end_सूचकांक_2 = निवेश_गिनती;
    अगर (context_format == ContextFormat::CoverageBasedContext)
      end_सूचकांक_2 += 1;

    c->recurse (lookupRecord[i].lookup_List_सूचकांक, &covered_seq_indices, seq_सूचकांक_2, end_सूचकांक_2);
    c->निकालो_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_तथ्य->current_context_set_सूचकांक = saved_context_set_सूचकांक;
    c->depend_तथ्य->current_edge_flags = saved_edge_flags;
  }
}

ढाँचा <typename context_t>
अटल अंतरभूत व्योम recurse_lookups (context_t *c,
                                    अचिन्हित पूर्णांक lookup_गिनती_2,
                                    स्थिर LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  क्रम (अचिन्हित पूर्णांक i = 0; i < lookup_गिनती_2; i++)
    c->recurse (lookupRecord[i].lookup_List_सूचकांक);
}

अटल अंतरभूत व्योम apply_lookup (hb_ot_apply_context_t *c,
				 अचिन्हित पूर्णांक गिनती, /* Including the first glyph */
				 अचिन्हित पूर्णांक lookup_गिनती_2,
				 स्थिर LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 अचिन्हित पूर्णांक match_end)
{
  hb_बफर_t *बफर = c->बफर;
  पूर्णांक end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    अचिन्हित पूर्णांक bl = बफर->backtrack_len ();
    end = bl + match_end - बफर->idx;

    पूर्णांक delta = bl - बफर->idx;
    /* Convert positions to new indexing. */
    क्रम (अचिन्हित पूर्णांक j = 0; j < गिनती; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  क्रम (अचिन्हित पूर्णांक i = 0; i < lookup_गिनती_2 && बफर->successful; i++)
  {
    अचिन्हित पूर्णांक idx = lookupRecord[i].sequence_सूचकांक;
    अगर (idx >= गिनती)
      जारी;

    अचिन्हित पूर्णांक orig_len = बफर->backtrack_len () + बफर->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    अगर (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      जारी;

    अगर (unlikely (!बफर->move_to (c->match_positions.arrayZ[idx])))
      अवरोध;

    अगर (unlikely (बफर->max_ops <= 0))
      अवरोध;

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      अगर (बफर->have_output)
        c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "start recursing to lookup %u at %u",
			  (अचिन्हित) lookupRecord[i].lookup_List_सूचकांक,
			  बफर->idx);
    }

    bool ret = c->recurse (lookupRecord[i].lookup_List_सूचकांक);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      अगर (बफर->have_output)
        c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "end recursing to lookup %u",
			  (अचिन्हित) lookupRecord[i].lookup_List_सूचकांक);
    }

    अगर (!ret)
      जारी;

    अचिन्हित पूर्णांक new_len = बफर->backtrack_len () + बफर->lookahead_len ();
    पूर्णांक delta = new_len - orig_len;

    अगर (!delta)
      जारी;

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
    अगर (end < पूर्णांक (c->match_positions.arrayZ[idx]))
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

    अगर (delta > 0)
    {
      अगर (unlikely (delta + गिनती > HB_MAX_CONTEXT_लंबाई))
	अवरोध;
      अगर (unlikely (गिनती + delta > c->match_positions.लंबाई &&
		    !c->match_positions.resize_dirty  (गिनती + delta)))
        वापस;
    }
    अन्यथा
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (पूर्णांक) next - (पूर्णांक) गिनती);
      next -= delta;
    }

    /* Shift! */
    स_हटाओ (c->match_positions + next + delta, c->match_positions + next,
	     (गिनती - next) * माप (c->match_positions.arrayZ[0]));
    next += delta;
    गिनती += delta;

    /* Fill in new entries. */
    क्रम (अचिन्हित पूर्णांक j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    क्रम (; next < गिनती; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  निश्चित (end >= 0);
  (व्योम) बफर->move_to (end);
}



/* Contextual lookups */

काष्ठा ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  स्थिर व्योम *intersects_तथ्य;
  व्योम *intersects_cache;
  व्योम *intersected_glyphs_cache;
};

प्रकार ContextClosureLookupContext ContextDependLookupContext;

काष्ठा ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  स्थिर व्योम *collect_तथ्य;
};

काष्ठा ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  स्थिर व्योम *match_तथ्य_2;
  get_मूल्य_func_t get_मूल्य;
};

अटल अंतरभूत bool context_intersects (स्थिर hb_set_t *glyphs,
				       अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
				       glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  वापस array_is_subset_of (glyphs,
			     निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_तथ्य,
			     lookup_context.intersects_cache);
}

अटल अंतरभूत व्योम context_closure_lookup (hb_closure_context_t *c,
					   अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
					   glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
					   अचिन्हित पूर्णांक lookup_गिनती_2,
					   स्थिर LookupRecord lookupRecord[],
					   अचिन्हित value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  अगर (context_intersects (c->glyphs,
			  निवेश_गिनती, निवेश,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     निवेश_गिनती, निवेश,
				     lookup_गिनती_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_तथ्य,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

अटल अंतरभूत व्योम context_depend_lookup (hb_depend_context_t *c,
					  अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
					  glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
					  अचिन्हित पूर्णांक lookup_गिनती_2,
					  स्थिर LookupRecord lookupRecord[],
					  स्थिर व्योम *rule,
					  अचिन्हित value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  अगर (!context_intersects (c->glyphs,
			   निवेश_गिनती, निवेश,
			   lookup_context))
    वापस;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  अगर (unlikely (!scratch))
    वापस;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  bool context_sets_cached = context_depend_sets_are_cached (
    c, निवेश_गिनती, lookup_गिनती_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_तथ्य,
    &scratch->cached_context_sets);
  अगर (unlikely (!c->depend_तथ्य->successful))
    वापस;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<स्थिर hb_set_t *> &निवेश_position_glyphs = scratch->निवेश_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  अगर (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    स्थिर hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_तथ्य, value,
      true, &position_scratch, &scratch->compact_workspace);
    अगर (unlikely (!pos0_glyphs ||
		  !निवेश_position_glyphs.धकेलो_or_fail (pos0_glyphs)))
    {
      c->depend_तथ्य->हार ();
      वापस;
    }

    /* Positions 1+ (Input array) */
    क्रम (अचिन्हित i = 0; i + 1 < निवेश_गिनती; i++)
    {
      स्थिर hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_तथ्य,
	निवेश[i], false, &position_scratch);
      अगर (unlikely (!pos_glyphs ||
		    !निवेश_position_glyphs.धकेलो_or_fail (pos_glyphs)))
      {
	c->depend_तथ्य->हार ();
	वापस;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  निवेश_गिनती, निवेश,
				  lookup_गिनती_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_तथ्य,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &निवेश_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

अटल अंतरभूत व्योम context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
						  glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
						  अचिन्हित पूर्णांक lookup_गिनती_2,
						  स्थिर LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->निवेश,
		 निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
		 lookup_context.funcs.collect, lookup_context.collect_तथ्य);
  recurse_lookups (c,
		   lookup_गिनती_2, lookupRecord);
}

अटल अंतरभूत bool context_would_apply_lookup (hb_would_apply_context_t *c,
					       अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
					       glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
					       अचिन्हित पूर्णांक lookup_गिनती_2 HB_UNUSED,
					       स्थिर LookupRecord lookupRecord[] HB_UNUSED,
					       स्थिर ContextApplyLookupContext &lookup_context)
{
  वापस would_match_निवेश (c,
			    निवेश_गिनती, निवेश,
			    lookup_context.funcs.match, lookup_context.match_तथ्य_2);
}

अटल अंतरभूत bool context_apply_lookup (hb_ot_apply_context_t *c,
					 अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
					 glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
					 अचिन्हित पूर्णांक lookup_गिनती_2,
					 स्थिर LookupRecord lookupRecord[],
					 स्थिर ContextApplyLookupContext &lookup_context)
{
  अगर (unlikely (निवेश_गिनती > HB_MAX_CONTEXT_लंबाई)) वापस false;

  अचिन्हित match_end = 0;
  bool ret = false;
  अगर (match_निवेश (c,
		   निवेश_गिनती, निवेश,
		   lookup_context.funcs.match, lookup_context.match_तथ्य_2,
		   &match_end))
  {
    c->बफर->unsafe_to_break (c->बफर->idx, match_end);
    apply_lookup (c,
		  निवेश_गिनती,
		  lookup_गिनती_2, lookupRecord,
		  match_end);
    ret = true;
  }
  अन्यथा
  {
    c->बफर->unsafe_to_concat (c->बफर->idx, match_end);
    ret = false;
  }

  वापस ret;
}

अटल अंतरभूत bool context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  चयन (op)
  {
    हाल hb_ot_subtable_cache_op_t::ENTER:
    {
      अगर (!HB_बफर_TRY_ALLOCATE_VAR (c->बफर, syllable))
	वापस false;
      स्वतः &info = c->बफर->info;
      अचिन्हित गिनती = c->बफर->len;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      वापस true;
    }
    हाल hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (अचिन्हित) -1;
      HB_बफर_DEALLOCATE_VAR (c->बफर, syllable);
      अवरोध;
    }
  }
  वापस false;
}

ढाँचा <typename Types>
काष्ठा Rule
{
  ढाँचा <typename SetTypes, typename RuleType>
  मित्र काष्ठा RuleSetOf;

  bool intersects (स्थिर hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) स्थिर
  {
    वापस context_intersects (glyphs,
			       निवेश_गिनती, निवेश_Z.arrayZ,
			       lookup_context);
  }

  व्योम depend (hb_depend_context_t *c, अचिन्हित value, ContextDependLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array ((निवेश_गिनती ? निवेश_गिनती - 1 : 0)));
    context_depend_lookup (c,
			   निवेश_गिनती, निवेश_Z.arrayZ,
			   lookup_गिनती_2, lookupRecord.arrayZ,
			   यह,
			   value, lookup_context);
  }

  व्योम closure (hb_closure_context_t *c, अचिन्हित value, ContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;

    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array ((निवेश_गिनती ? निवेश_गिनती - 1 : 0)));
    context_closure_lookup (c,
			    निवेश_गिनती, निवेश_Z.arrayZ,
			    lookup_गिनती_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;
    अगर (!intersects (c->glyphs, lookup_context)) वापस;

    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    recurse_lookups (c, lookup_गिनती_2, lookupRecord.arrayZ);
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    context_collect_glyphs_lookup (c,
				   निवेश_गिनती, निवेश_Z.arrayZ,
				   lookup_गिनती_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिर ClassDef *class_def = nullptr) स्थिर
  {
    अगर (निवेश_गिनती <= 1)
    {
      *digest = hb_set_digest_t::full ();
      वापस;
    }

    अचिन्हित second = निवेश_Z.arrayZ[0];
    अगर (!class_def)
      digest->add (second);
    अन्यथा अगर (!second)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      class_def->collect_class (digest, second);
  }

  bool would_apply (hb_would_apply_context_t *c,
		    स्थिर ContextApplyLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    वापस context_would_apply_lookup (c,
				       निवेश_गिनती, निवेश_Z.arrayZ,
				       lookup_गिनती_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  bool apply (hb_ot_apply_context_t *c,
	      स्थिर ContextApplyLookupContext &lookup_context) स्थिर
  {
    TRACE_APPLY (यह);
    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    return_trace (context_apply_lookup (c, निवेश_गिनती, निवेश_Z.arrayZ, lookup_गिनती_2, lookupRecord.arrayZ, lookup_context));
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर hb_map_t *निवेश_mapping, /* old->new glyphid or class mapping */
		  स्थिर hb_map_t *lookup_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->start_embed (यह);
    अगर (unlikely (!c->extend_min (out))) return_trace (false);

    out->निवेश_गिनती = निवेश_गिनती;
    स्थिर स्वतः निवेश = निवेश_Z.as_array (निवेश_गिनती - 1);
    क्रम (स्थिर स्वतः org : निवेश)
    {
      typename Types::HBUINT d;
      d = निवेश_mapping->लो (org);
      c->copy (d);
    }

    स्थिर स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array ((निवेश_गिनती ? निवेश_गिनती - 1 : 0)));

    अचिन्हित गिनती = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_गिनती_2), lookup_map);
    return_trace (c->check_assign (out->lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool subset (hb_subset_context_t *c,
	       स्थिर hb_map_t *lookup_map,
	       स्थिर hb_map_t *klass_map = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (unlikely (!निवेश_गिनती)) return_trace (false);
    स्थिर स्वतः निवेश = निवेश_Z.as_array (निवेश_गिनती - 1);

    स्थिर hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    अगर (!hb_all (निवेश, mapping)) return_trace (false);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  c->check_range (निवेश_Z.arrayZ,
				  निवेश_Z.item_आकार * (निवेश_गिनती ? निवेश_गिनती - 1 : 0) +
				  LookupRecord::static_आकार * lookup_गिनती_2));
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
  खुला:
  DEFINE_आकार_ARRAY (4, निवेश_Z);
};

ढाँचा <typename Types, typename RuleType>
काष्ठा RuleSetOf
{
  bool intersects (स्थिर hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) स्थिर
  {
    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_map ([&] (स्थिर RuleType &_) { वापस _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  व्योम collect_first_निवेश_classes (hb_ot_layout_ruleset_digest_t *digest) स्थिर
  {
    digest->init ();
    क्रम (अचिन्हित i = 0; i < rule.len; i++)
    {
      स्थिर RuleType &r = यह+rule.arrayZ[i];
      अगर (r.निवेश_गिनती <= 1)
      {
	digest->set_full ();
	वापस;
      }
      digest->add (r.निवेश_Z.arrayZ[0]);
    }
  }

  व्योम depend (hb_depend_context_t *c, अचिन्हित value, ContextDependLookupContext &lookup_context) स्थिर
  {
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  व्योम closure (hb_closure_context_t *c, अचिन्हित value,
		ContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;

    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) स्थिर
  {
    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिर ClassDef *class_def = nullptr) स्थिर
  {
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([=] (स्थिर RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c,
		    स्थिर ContextApplyLookupContext &lookup_context) स्थिर
  {
    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_map ([&] (स्थिर RuleType &_) { वापस _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  bool apply (hb_ot_apply_context_t *c,
	      स्थिर ContextApplyLookupContext &lookup_context,
	      स्थिर hb_ot_layout_ruleset_digest_t *digest = nullptr) स्थिर
  {
    TRACE_APPLY (यह);

    अचिन्हित अंक_rules = rule.len;

#अगर_अघोषित HB_NO_OT_RULESETS_FAST_PATH
    अगर (HB_OPTIMIZE_आकार_VAL || अंक_rules <= 4)
#पूर्ण_अगर
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (यह))
      | hb_map ([&] (स्थिर RuleType &_) { वापस _.apply (c, lookup_context); })
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
    skippy_iter.reset (c->बफर->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_तथ्य ((HBUINT16 *) nullptr);
    अचिन्हित unsafe_to = (अचिन्हित) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    bool matched = skippy_iter.next (&unsafe_to1);
    अगर (likely (matched))
    {
      अगर (skippy_iter.may_skip (c->बफर->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जाओ slow;
      }

      first = &c->बफर->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      अगर (digest && lookup_context.get_मूल्य &&
	  !digest->may_have (lookup_context.get_मूल्य (*first, lookup_context.match_तथ्य_2)))
	{
	  c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to1);
	  return_trace (false);
	}
    }
    अन्यथा
    {
      bool unsafe_to_concat = false;
      bool ret =
      + hb_iter (rule)
      | hb_map (hb_add (यह))
      | hb_filter ([&] (स्थिर RuleType &_)
		   {
		     अगर (_.निवेश_गिनती <= 1) वापस true;
		     unsafe_to_concat = true;
		     वापस false;
		   })
      | hb_map ([&] (स्थिर RuleType &_) { वापस _.apply (c, lookup_context); })
      | hb_any
      ;
      अगर (unsafe_to_concat)
	c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    अगर (likely (matched))
    {
      अगर (skippy_iter.may_skip (c->बफर->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जाओ slow;
      }

      second = &c->बफर->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    स्वतः match_निवेश = lookup_context.funcs.match;
    स्वतः *निवेश_तथ्य = lookup_context.match_तथ्य_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_rules; i++)
    {
      स्थिर स्वतः &r = यह+rule.arrayZ[i];

      स्थिर स्वतः &निवेश = r.निवेश_Z;

      अगर (r.निवेश_गिनती <= 1 ||
	  (!match_निवेश ||
	   match_निवेश (*first, निवेश.arrayZ[0], निवेश_तथ्य)))
      {
        अगर (!second ||
	    (r.निवेश_गिनती <= 2 ||
	     (!match_निवेश ||
	      match_निवेश (*second, निवेश.arrayZ[1], निवेश_तथ्य)))
	   )
	{
	  अगर (r.apply (c, lookup_context))
	  {
	    अगर (unsafe_to != (अचिन्हित) -1)
	      c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);
	    return_trace (true);
	  }
	}
	अन्यथा
	  unsafe_to = unsafe_to2;
      }
      अन्यथा
      {
	अगर (unsafe_to == (अचिन्हित) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	क्रम (; i + 1 < अंक_rules; i++)
	{
	  स्थिर स्वतः &r2 = यह+rule.arrayZ[i + 1];
	  स्थिर स्वतः &input2 = r2.निवेश_Z;
	  अगर (r2.निवेश_गिनती <= 1 || input2.arrayZ[0] != निवेश.arrayZ[0])
	    अवरोध;
	}
      }
    }
    अगर (likely (unsafe_to != (अचिन्हित) -1))
      c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);

    return_trace (false);
  }

  bool subset (hb_subset_context_t *c,
	       स्थिर hb_map_t *lookup_map,
	       स्थिर hb_map_t *klass_map = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः snap = c->serializer->snapshot ();
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    क्रम (स्थिर स्वतः& _ : rule)
    {
      अगर (!_) जारी;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = out->rule.serialize_append (c->serializer);
      अगर (unlikely (!o)) जारी;

      अगर (!o->serialize_subset (c, _, यह, lookup_map, klass_map))
      {
	out->rule.निकालो ();
	c->serializer->revert (o_snap);
      }
    }

    bool ret = bool (out->rule);
    अगर (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (rule.sanitize (c, यह));
  }

  रक्षित:
  Array16Of<typename Types::ढाँचा OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  खुला:
  DEFINE_आकार_ARRAY (2, rule);
};

ढाँचा <typename Types>
काष्ठा RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

ढाँचा <typename Types>
काष्ठा ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


ढाँचा <typename Types>
काष्ठा ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    वापस
    + hb_zip (यह+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_map ([&] (स्थिर RuleSet &_) { वापस _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (यह+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      वापस c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिर hb_pair_t<hb_codepoint_t, अचिन्हित> _) { वापस hb_pair_t<अचिन्हित, स्थिर RuleSet&> (_.first, यह+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (यह+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      वापस c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिर hb_pair_t<hb_codepoint_t, अचिन्हित> _) { वापस hb_pair_t<अचिन्हित, स्थिर RuleSet&> (_.first, यह+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर
  {
    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (यह+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    (यह+coverage).collect_coverage (c->निवेश);

    काष्ठा ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([digest] (स्थिर RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    स्थिर RuleSet &rule_set = यह+ruleSet[(यह+coverage).get_coverage (c->glyphs[0])];
    काष्ठा ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    वापस rule_set.would_apply (c, lookup_context);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
    अगर (likely (सूचकांक_2 == NOT_COVERED))
      return_trace (false);

    स्थिर RuleSet &rule_set = यह+ruleSet[सूचकांक_2];
    काष्ठा ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    स्थिर hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, यह, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && ruleSet.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, ruleSet);
};


ढाँचा <typename Types>
काष्ठा ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (!(यह+coverage).intersects (glyphs))
      वापस false;

    स्थिर ClassDef &class_def = यह+classDef;

    hb_map_t cache;
    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (यह+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    वापस
    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_enumerate
    | hb_map ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर RuleSet &> p)
	      { वापस class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    अगर (!(यह+coverage).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिर ClassDef &class_def = यह+classDef;

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
    { वापस class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर typename Types::ढाँचा OffsetTo<RuleSet>&> _)
                {
                  स्थिर RuleSet& rule_set = यह+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    अगर (!(यह+coverage).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिर ClassDef &class_def = यह+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (अचिन्हित _)
    { वापस class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर typename Types::ढाँचा OffsetTo<RuleSet>&> _)
                {
                  स्थिर RuleSet& rule_set = यह+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर
  {
    अगर (!(यह+coverage).intersects (c->glyphs))
      वापस;

    स्थिर ClassDef &class_def = यह+classDef;

    hb_map_t cache;
    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_enumerate
    | hb_filter ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर RuleSet &> p)
    { वापस class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (स्थिर RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    (यह+coverage).collect_coverage (c->निवेश);

    स्थिर ClassDef &class_def = यह+classDef;
    काष्ठा ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    स्थिर ClassDef &class_def = यह+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([=, &class_def] (स्थिर RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    स्थिर ClassDef &class_def = यह+classDef;
    अचिन्हित पूर्णांक सूचकांक_2 = class_def.get_class (c->glyphs[0]);
    स्थिर RuleSet &rule_set = यह+ruleSet[सूचकांक_2];
    काष्ठा ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    वापस rule_set.would_apply (c, lookup_context);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  अचिन्हित cache_cost () स्थिर
  {
    वापस (यह+classDef).cost ();
  }
  अटल bool cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    वापस context_cache_func (c, op);
  }

  काष्ठा external_cache_t
  {
    hb_ot_layout_द्विआधारी_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  व्योम *external_cache_create () स्थिर
  {
    अचिन्हित गिनती = ruleSet.len;
    अचिन्हित आकार = माप (external_cache_t) -
		    HB_VAR_ARRAY * माप (hb_ot_layout_ruleset_digest_t) +
		    गिनती * माप (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (आकार);
    अगर (likely (cache))
    {
      cache->coverage.सफा ();
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
	(यह+ruleSet[i]).collect_first_निवेश_classes (&cache->rule_sets[i]);
    }
    वापस cache;
  }
  bool apply_cached (hb_ot_apply_context_t *c, व्योम *external_cache) स्थिर { वापस _apply (c, true, external_cache); }
  bool apply (hb_ot_apply_context_t *c, व्योम *external_cache) स्थिर { वापस _apply (c, false, external_cache); }
  bool _apply (hb_ot_apply_context_t *c, bool cached, व्योम *external_cache) स्थिर
  {
    TRACE_APPLY (यह);
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage_द्विआधारी (c->बफर->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
#पूर्ण_अगर
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्थिर ClassDef &class_def = यह+classDef;

    काष्ठा ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_मूल्य : get_class_मूल्य
    };

    सूचकांक_2 = cached ? get_class_cached (class_def, c->बफर->cur()) : class_def.get_class (c->बफर->cur().codepoint);
    स्थिर RuleSet &rule_set = यह+ruleSet[सूचकांक_2];
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    स्थिर hb_ot_layout_ruleset_digest_t *digest = cache && सूचकांक_2 < ruleSet.len ? &cache->rule_sets[सूचकांक_2] : nullptr;
#अन्यथा
    स्थिर hb_ot_layout_ruleset_digest_t *digest = nullptr;
#पूर्ण_अगर
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;
    अगर (unlikely (!out->coverage.serialize_subset (c, coverage, यह)))
      return_trace (false);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, यह, &klass_map);

    स्थिर hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (यह+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (यह+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    स्थिर hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    bool ret = true;
    पूर्णांक non_zero_सूचकांक = -1, सूचकांक_2 = 0;
    स्वतः snapshot = c->serializer->snapshot();
    क्रम (स्थिर स्वतः& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      स्वतः *o = out->ruleSet.serialize_append (c->serializer);
      अगर (unlikely (!o))
      {
	ret = false;
	अवरोध;
      }

      अगर (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, यह, lookup_map, &klass_map)) {
	non_zero_सूचकांक = सूचकांक_2;
        snapshot = c->serializer->snapshot();
      }

      सूचकांक_2++;
    }

    अगर (!ret || non_zero_सूचकांक == -1) return_trace (false);

    //prune empty trailing ruleSets
    --सूचकांक_2;
    जबतक (सूचकांक_2 > non_zero_सूचकांक)
    {
      out->ruleSet.निकालो ();
      सूचकांक_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (bool (out->ruleSet));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && classDef.sanitize (c, यह) && ruleSet.sanitize (c, यह));
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
  खुला:
  DEFINE_आकार_ARRAY (2 + 2 * Types::LOffset::static_आकार + Types::HBUINT::static_आकार, ruleSet);
};


ढाँचा <typename Types>
काष्ठा ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (!(यह+coverageZ[0]).intersects (glyphs))
      वापस false;

    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      यह
    };
    वापस context_intersects (glyphs,
			       glyph_गिनती_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    अगर (!(यह+coverageZ[0]).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      यह,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_गिनती_2, (स्थिर HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_गिनती_2, lookupRecord,
			   यह,
			   coverageZ[0], lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    अगर (!(यह+coverageZ[0]).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    काष्ठा ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      यह
    };
    context_closure_lookup (c,
			    glyph_गिनती_2, coverageZ.arrayZ + 1,
			    lookup_गिनती_2, lookupRecord,
			    0, lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर
  {
    अगर (!intersects (c->glyphs))
      वापस;
    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    recurse_lookups (c, lookup_गिनती_2, lookupRecord);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    (यह+coverageZ[0]).collect_coverage (c->निवेश);

    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    काष्ठा ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      यह
    };

    context_collect_glyphs_lookup (c,
				   glyph_गिनती_2, coverageZ.arrayZ + 1,
				   lookup_गिनती_2, lookupRecord,
				   lookup_context);
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    अगर (glyph_गिनती_2 <= 1)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      (यह+coverageZ[1]).collect_coverage (digest);
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    काष्ठा ContextApplyLookupContext lookup_context = {
      {match_coverage},
      यह
    };
    वापस context_would_apply_lookup (c,
				       glyph_गिनती_2, coverageZ.arrayZ + 1,
				       lookup_गिनती_2, lookupRecord,
				       lookup_context);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverageZ[0]; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverageZ[0]).get_coverage (c->बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    काष्ठा ContextApplyLookupContext lookup_context = {
      {match_coverage},
      यह
    };
    return_trace (context_apply_lookup (c,
					glyph_गिनती_2,
					coverageZ.arrayZ + 1,
					lookup_गिनती_2, lookupRecord,
					lookup_context));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->format = format;
    out->glyph_गिनती_2 = glyph_गिनती_2;

    स्वतः coverages = coverageZ.as_array (glyph_गिनती_2);

    क्रम (स्थिर स्वतः& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      स्वतः *o = c->serializer->allocate_आकार<typename Types::ढाँचा OffsetTo<Coverage>> (Types::Offset::static_आकार);
      अगर (unlikely (!o)) return_trace (false);
      अगर (!o->serialize_subset (c, offset, यह)) return_trace (false);
    }

    स्थिर स्वतः& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_गिनती_2));
    स्थिर hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    अचिन्हित गिनती = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_गिनती_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह))) return_trace (false);
    hb_barrier ();
    अचिन्हित पूर्णांक गिनती = glyph_गिनती_2;
    अगर (unlikely (!गिनती)) return_trace (false); /* We want to access coverageZ[0] freely. */
    अगर (unlikely (!c->check_array (coverageZ.arrayZ, गिनती))) return_trace (false);
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!coverageZ[i].sanitize (c, यह))) return_trace (false);
    स्थिर LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
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
  खुला:
  DEFINE_आकार_ARRAY (6, coverageZ);
};

काष्ठा Context
{
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    हाल 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    हाल 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#अगर_अघोषित HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#पूर्ण_अगर
  } u;
};


/* Chaining Contextual lookups */

काष्ठा ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  स्थिर व्योम *intersects_तथ्य[3];
  व्योम *intersects_cache[3];
  व्योम *intersected_glyphs_cache;
};

प्रकार ChainContextClosureLookupContext ChainContextDependLookupContext;

काष्ठा ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  स्थिर व्योम *collect_तथ्य[3];
};

काष्ठा ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  स्थिर व्योम *match_तथ्य_2[3];
  get_मूल्य_func_t get_निवेश_मूल्य;
};

अटल अंतरभूत bool chain_context_intersects (स्थिर hb_set_t *glyphs,
					     अचिन्हित पूर्णांक backtrack_गिनती,
					     glyph_तथ्य_t backtrack,
					     अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
					     glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
					     अचिन्हित पूर्णांक lookahead_गिनती,
					     glyph_तथ्य_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  bool bt_ok = array_is_subset_of (glyphs,
			     backtrack_गिनती, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_तथ्य[0],
			     lookup_context.intersects_cache[0]);
  bool in_ok = array_is_subset_of (glyphs,
			     निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_तथ्य[1],
			     lookup_context.intersects_cache[1]);
  bool la_ok = array_is_subset_of (glyphs,
			     lookahead_गिनती, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_तथ्य[2],
			     lookup_context.intersects_cache[2]);

  वापस bt_ok && in_ok && la_ok;
}

अटल अंतरभूत व्योम chain_context_closure_lookup (hb_closure_context_t *c,
						 अचिन्हित पूर्णांक backtrack_गिनती,
						 glyph_तथ्य_t backtrack,
						 अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
						 glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
						 अचिन्हित पूर्णांक lookahead_गिनती,
						 glyph_तथ्य_t lookahead,
						 अचिन्हित पूर्णांक lookup_गिनती_2,
						 स्थिर LookupRecord lookupRecord[],
						 अचिन्हित value,
						 ChainContextClosureLookupContext &lookup_context)
{
  अगर (chain_context_intersects (c->glyphs,
				backtrack_गिनती, backtrack,
				निवेश_गिनती, निवेश,
				lookahead_गिनती, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     निवेश_गिनती, निवेश,
		     lookup_गिनती_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_तथ्य[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

अटल अंतरभूत व्योम chain_context_depend_lookup (hb_depend_context_t *c,
						अचिन्हित पूर्णांक backtrack_गिनती,
						glyph_तथ्य_t backtrack,
						अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
						glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
						अचिन्हित पूर्णांक lookahead_गिनती,
						glyph_तथ्य_t lookahead,
						अचिन्हित पूर्णांक lookup_गिनती_2,
						स्थिर LookupRecord lookupRecord[],
						स्थिर व्योम *rule,
						अचिन्हित value,
						ChainContextDependLookupContext &lookup_context)
{
  अगर (!chain_context_intersects (c->glyphs,
				 backtrack_गिनती, backtrack,
				 निवेश_गिनती, निवेश,
				 lookahead_गिनती, lookahead,
				 lookup_context))
    वापस;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  अगर (unlikely (!scratch))
    वापस;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  bool context_sets_cached = context_depend_sets_are_cached (
    c, निवेश_गिनती, lookup_गिनती_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_तथ्य[1],
    &scratch->cached_context_sets);
  अगर (unlikely (!c->depend_तथ्य->successful))
    वापस;
  अगर (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    निवेश_गिनती, निवेश,
				    lookup_गिनती_2, lookupRecord,
				    rule,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_तथ्य[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    nullptr,
				    &scratch->cached_context_sets,
				    scratch);
    वापस;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<स्थिर hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<स्थिर hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  क्रम (अचिन्हित i = 0; i < backtrack_गिनती; i++)
  {
    स्थिर hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_तथ्य[0],
      backtrack[i], false, &position_scratch);
    अगर (unlikely (!pos_glyphs))
      वापस;
    अगर (!pos_glyphs->is_empty ())
    {
      अगर (unlikely (!backtrack_sets.धकेलो_or_fail (pos_glyphs)))
      {
        c->depend_तथ्य->हार ();
        वापस;
      }
    }
  }

  /* Extract lookahead glyphs */
  क्रम (अचिन्हित i = 0; i < lookahead_गिनती; i++)
  {
    स्थिर hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_तथ्य[2],
      lookahead[i], false, &position_scratch);
    अगर (unlikely (!pos_glyphs))
      वापस;
    अगर (!pos_glyphs->is_empty ())
    {
      अगर (unlikely (!lookahead_sets.धकेलो_or_fail (pos_glyphs)))
      {
        c->depend_तथ्य->हार ();
        वापस;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  क्रम (स्थिर hb_set_t *back_set : backtrack_sets)
  {
    अगर (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } अन्यथा अगर (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_तथ्य->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      अगर (unlikely (set_idx == HB_CODEPOINT_INVALID))
        वापस;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  क्रम (स्थिर hb_set_t *look_set : lookahead_sets)
  {
    अगर (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } अन्यथा अगर (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_तथ्य->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      अगर (unlikely (set_idx == HB_CODEPOINT_INVALID))
        वापस;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  अगर (unlikely (preliminary_context.in_error ()))
  {
    c->depend_तथ्य->हार ();
    वापस;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<स्थिर hb_set_t *> &निवेश_position_glyphs = scratch->निवेश_position_glyphs;

  अगर (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    स्थिर hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_तथ्य[1], value,
      true, &position_scratch, &scratch->compact_workspace);
    अगर (unlikely (!pos0_glyphs ||
		  !निवेश_position_glyphs.धकेलो_or_fail (pos0_glyphs)))
    {
      c->depend_तथ्य->हार ();
      वापस;
    }

    /* Positions 1+ (Input array) */
    क्रम (अचिन्हित i = 0; i + 1 < निवेश_गिनती; i++)
    {
      स्थिर hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_तथ्य[1],
	निवेश[i], false, &position_scratch);
      अगर (unlikely (!pos_glyphs ||
		    !निवेश_position_glyphs.धकेलो_or_fail (pos_glyphs)))
      {
	c->depend_तथ्य->हार ();
	वापस;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  निवेश_गिनती, निवेश,
				  lookup_गिनती_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_तथ्य[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &निवेश_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

अटल अंतरभूत व्योम chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							अचिन्हित पूर्णांक backtrack_गिनती,
							glyph_तथ्य_t backtrack,
							अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
							glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
							अचिन्हित पूर्णांक lookahead_गिनती,
							glyph_तथ्य_t lookahead,
							अचिन्हित पूर्णांक lookup_गिनती_2,
							स्थिर LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_गिनती, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_तथ्य[0]);
  collect_array (c, c->निवेश,
		 निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
		 lookup_context.funcs.collect, lookup_context.collect_तथ्य[1]);
  collect_array (c, c->after,
		 lookahead_गिनती, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_तथ्य[2]);
  recurse_lookups (c,
		   lookup_गिनती_2, lookupRecord);
}

अटल अंतरभूत bool chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     अचिन्हित पूर्णांक backtrack_गिनती,
						     glyph_तथ्य_t backtrack HB_UNUSED,
						     अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
						     glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
						     अचिन्हित पूर्णांक lookahead_गिनती,
						     glyph_तथ्य_t lookahead HB_UNUSED,
						     अचिन्हित पूर्णांक lookup_गिनती_2 HB_UNUSED,
						     स्थिर LookupRecord lookupRecord[] HB_UNUSED,
						     स्थिर ChainContextApplyLookupContext &lookup_context)
{
  वापस (c->zero_context ? !backtrack_गिनती && !lookahead_गिनती : true)
      && would_match_निवेश (c,
			    निवेश_गिनती, निवेश,
			    lookup_context.funcs.match[1], lookup_context.match_तथ्य_2[1]);
}

अटल अंतरभूत bool chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       अचिन्हित पूर्णांक backtrack_गिनती,
					       glyph_तथ्य_t backtrack,
					       अचिन्हित पूर्णांक निवेश_गिनती, /* Including the first glyph (not matched) */
					       glyph_तथ्य_t निवेश, /* Array of input values--start with second glyph */
					       अचिन्हित पूर्णांक lookahead_गिनती,
					       glyph_तथ्य_t lookahead,
					       अचिन्हित पूर्णांक lookup_गिनती_2,
					       स्थिर LookupRecord lookupRecord[],
					       स्थिर ChainContextApplyLookupContext &lookup_context)
{
  अगर (unlikely (निवेश_गिनती > HB_MAX_CONTEXT_लंबाई)) वापस false;

  अचिन्हित start_सूचकांक = c->बफर->out_len;
  अचिन्हित end_सूचकांक = c->बफर->idx;
  अचिन्हित match_end = 0;
  bool ret = true;
  अगर (!(match_निवेश (c,
		     निवेश_गिनती, निवेश,
		     lookup_context.funcs.match[1], lookup_context.match_तथ्य_2[1],
		     &match_end) && (end_सूचकांक = match_end)
       && match_lookahead (c,
			   lookahead_गिनती, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_तथ्य_2[2],
			   match_end, &end_सूचकांक)))
  {
    c->बफर->unsafe_to_concat (c->बफर->idx, end_सूचकांक);
    वापस false;
  }

  अगर (!match_backtrack (c,
			backtrack_गिनती, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_तथ्य_2[0],
			&start_सूचकांक))
  {
    c->बफर->unsafe_to_concat_from_outbuffer (start_सूचकांक, end_सूचकांक);
    वापस false;
  }

  c->बफर->unsafe_to_break_from_outbuffer (start_सूचकांक, end_सूचकांक);
  apply_lookup (c,
		निवेश_गिनती,
		lookup_गिनती_2, lookupRecord,
		match_end);

  वापस ret;
}

ढाँचा <typename Types>
काष्ठा ChainRule
{
  ढाँचा <typename SetTypes, typename ChainRuleType>
  मित्र काष्ठा ChainRuleSetOf;

  bool intersects (स्थिर hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    वापस chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     निवेश.lenP1, निवेश.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  व्योम depend (hb_depend_context_t *c, अचिन्हित value, ChainContextDependLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 निवेश.lenP1, निवेश.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 यह,
				 value,
				 lookup_context);
  }

  व्योम closure (hb_closure_context_t *c, अचिन्हित value,
		ChainContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;

    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  निवेश.lenP1, निवेश.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;
    अगर (!intersects (c->glyphs, lookup_context)) वापस;

    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 निवेश.lenP1, निवेश.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिर ClassDef *निवेश_class_def = nullptr,
			      स्थिर ClassDef *lookahead_class_def = nullptr) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);

    अचिन्हित second;
    स्थिर ClassDef *class_def;
    अगर (निवेश.lenP1 > 1)
    {
      second = निवेश.arrayZ[0];
      class_def = निवेश_class_def;
    }
    अन्यथा अगर (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    अन्यथा
    {
      *digest = hb_set_digest_t::full ();
      वापस;
    }

    अगर (!class_def)
      digest->add (second);
    अन्यथा अगर (!second)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      class_def->collect_class (digest, second);
  }

  bool would_apply (hb_would_apply_context_t *c,
		    स्थिर ChainContextApplyLookupContext &lookup_context) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    वापस chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     निवेश.lenP1, निवेश.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  bool apply (hb_ot_apply_context_t *c,
	      स्थिर ChainContextApplyLookupContext &lookup_context) स्थिर
  {
    TRACE_APPLY (यह);
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      निवेश.lenP1, निवेश.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) स्थिर
  {
    c->copy (len);
    क्रम (स्थिर स्वतः g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर hb_map_t *lookup_map,
		  स्थिर hb_map_t *backtrack_map,
		  स्थिर hb_map_t *निवेश_map = nullptr,
		  स्थिर hb_map_t *lookahead_map = nullptr) स्थिर
  {
    TRACE_SERIALIZE (यह);

    स्थिर hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    अगर (निवेश_map) mapping = निवेश_map;
    serialize_array (c, निवेश.lenP1, + निवेश.iter ()
				     | hb_map (mapping));

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    अगर (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_गिनती_2 = c->embed (&(lookup.len));
    अगर (!lookup_गिनती_2) return_trace (false);

    अचिन्हित गिनती = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool subset (hb_subset_context_t *c,
	       स्थिर hb_map_t *lookup_map,
	       स्थिर hb_map_t *backtrack_map = nullptr,
	       स्थिर hb_map_t *निवेश_map = nullptr,
	       स्थिर hb_map_t *lookahead_map = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);

    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);

    अगर (!backtrack_map)
    {
      स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      अगर (!hb_all (backtrack, glyphset) ||
	  !hb_all (निवेश, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (false);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    अन्यथा
    {
      अगर (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (निवेश, निवेश_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (false);

      serialize (c->serializer, lookup_map, backtrack_map, निवेश_map, lookahead_map);
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* Hyper-optimized sanitized because this is really hot. */
    अगर (unlikely (!backtrack.len.sanitize (c))) return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    अगर (unlikely (!निवेश.lenP1.sanitize (c))) return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    अगर (unlikely (!lookahead.len.sanitize (c))) return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
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
  खुला:
  DEFINE_आकार_MIN (8);
};

ढाँचा <typename Types, typename ChainRuleType>
काष्ठा ChainRuleSetOf
{
  bool intersects (स्थिर hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) स्थिर
  {
    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_map ([&] (स्थिर ChainRuleType &_) { वापस _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  व्योम collect_first_निवेश_classes (hb_ot_layout_ruleset_digest_t *digest) स्थिर
  {
    digest->init ();
    क्रम (अचिन्हित i = 0; i < rule.len; i++)
    {
      स्थिर ChainRuleType &r = यह+rule.arrayZ[i];
      स्थिर स्वतः &निवेश = StructAfter<decltype (r.निवेश_X)> (r.backtrack);
      अगर (निवेश.lenP1 <= 1)
      {
	digest->set_full ();
	वापस;
      }
      digest->add (निवेश.arrayZ[0]);
    }
  }
  व्योम depend (hb_depend_context_t *c, अचिन्हित value, ChainContextDependLookupContext &lookup_context) स्थिर
  {
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  व्योम closure (hb_closure_context_t *c, अचिन्हित value, ChainContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;

    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) स्थिर
  {
    अगर (unlikely (c->lookup_limit_exceeded ())) वापस;

    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) स्थिर
  {
    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिर ClassDef *निवेश_class_def = nullptr,
			      स्थिर ClassDef *lookahead_class_def = nullptr) स्थिर
  {
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_apply ([=] (स्थिर ChainRuleType &_)
		{ _.collect_second_glyphs (digest, निवेश_class_def, lookahead_class_def); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c,
		    स्थिर ChainContextApplyLookupContext &lookup_context) स्थिर
  {
    वापस
    + hb_iter (rule)
    | hb_map (hb_add (यह))
    | hb_map ([&] (स्थिर ChainRuleType &_) { वापस _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  bool apply (hb_ot_apply_context_t *c,
	      स्थिर ChainContextApplyLookupContext &lookup_context,
	      स्थिर hb_ot_layout_ruleset_digest_t *digest = nullptr) स्थिर
  {
    TRACE_APPLY (यह);

    अचिन्हित अंक_rules = rule.len;

#अगर_अघोषित HB_NO_OT_RULESETS_FAST_PATH
    अगर (HB_OPTIMIZE_आकार_VAL || अंक_rules <= 4)
#पूर्ण_अगर
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (यह))
      | hb_map ([&] (स्थिर ChainRuleType &_) { वापस _.apply (c, lookup_context); })
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
    skippy_iter.reset (c->बफर->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_तथ्य ((HBUINT16 *) nullptr);
    अचिन्हित unsafe_to = (अचिन्हित) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    bool matched = skippy_iter.next (&unsafe_to1);
    अगर (likely (matched))
    {
      अगर (skippy_iter.may_skip (c->बफर->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जाओ slow;
      }

      first = &c->बफर->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      अगर (digest && lookup_context.get_निवेश_मूल्य &&
	  !digest->may_have (lookup_context.get_निवेश_मूल्य (*first, lookup_context.match_तथ्य_2[1])))
	{
	  c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to1);
	  return_trace (false);
	}
    }
    अन्यथा
    {
      bool unsafe_to_concat = false;
      bool ret =
      + hb_iter (rule)
      | hb_map (hb_add (यह))
      | hb_filter ([&] (स्थिर ChainRuleType &_)
		   {
		     स्थिर स्वतः &निवेश = StructAfter<decltype (_.निवेश_X)> (_.backtrack);
		     स्थिर स्वतः &lookahead = StructAfter<decltype (_.lookaheadX)> (निवेश);
		     अगर (निवेश.lenP1 <= 1 && lookahead.len == 0) वापस true;
		     unsafe_to_concat = true;
		     वापस false;
		   })
      | hb_map ([&] (स्थिर ChainRuleType &_) { वापस _.apply (c, lookup_context); })
      | hb_any
      ;
      अगर (unsafe_to_concat)
	c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    अगर (likely (matched))
    {
      अगर (skippy_iter.may_skip (c->बफर->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जाओ slow;
      }

      second = &c->बफर->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    स्वतः match_निवेश = lookup_context.funcs.match[1];
    स्वतः match_lookahead = lookup_context.funcs.match[2];
    स्वतः *निवेश_तथ्य = lookup_context.match_तथ्य_2[1];
    स्वतः *lookahead_तथ्य = lookup_context.match_तथ्य_2[2];
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_rules; i++)
    {
      स्थिर स्वतः &r = यह+rule.arrayZ[i];

      स्थिर स्वतः &निवेश = StructAfter<decltype (r.निवेश_X)> (r.backtrack);
      स्थिर स्वतः &lookahead = StructAfter<decltype (r.lookaheadX)> (निवेश);

      अचिन्हित lenP1 = निवेश.lenP1;
      अगर (lenP1 > 1 ?
	   (!match_निवेश ||
	    match_निवेश (*first, निवेश.arrayZ[0], निवेश_तथ्य))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_तथ्य)))
      {
	lenP1 = hb_max (lenP1, 1u);
        अगर (!second ||
	    (lenP1 > 2 ?
	     (!match_निवेश ||
	      match_निवेश (*second, निवेश.arrayZ[1], निवेश_तथ्य))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_तथ्य))))
	{
	  अगर (r.apply (c, lookup_context))
	  {
	    अगर (unsafe_to != (अचिन्हित) -1)
	      c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);
	    return_trace (true);
	  }
	}
	अन्यथा
	  unsafe_to = unsafe_to2;
      }
      अन्यथा
      {
	अगर (unsafe_to == (अचिन्हित) -1)
	  unsafe_to = unsafe_to1;

	अगर (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  क्रम (; i + 1 < अंक_rules; i++)
	  {
	    स्थिर स्वतः &r2 = यह+rule.arrayZ[i + 1];
	    स्थिर स्वतः &input2 = StructAfter<decltype (r2.निवेश_X)> (r2.backtrack);
	    अगर (input2.lenP1 <= 1 || input2.arrayZ[0] != निवेश.arrayZ[0])
	      अवरोध;
	  }
	}
      }
    }
    अगर (likely (unsafe_to != (अचिन्हित) -1))
      c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);

    return_trace (false);
  }

  bool subset (hb_subset_context_t *c,
	       स्थिर hb_map_t *lookup_map,
	       स्थिर hb_map_t *backtrack_klass_map = nullptr,
	       स्थिर hb_map_t *निवेश_klass_map = nullptr,
	       स्थिर hb_map_t *lookahead_klass_map = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः snap = c->serializer->snapshot ();
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    क्रम (स्थिर स्वतः& _ : rule)
    {
      अगर (!_) जारी;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = out->rule.serialize_append (c->serializer);
      अगर (unlikely (!o)) जारी;

      अगर (!o->serialize_subset (c, _, यह,
				lookup_map,
				backtrack_klass_map,
				निवेश_klass_map,
				lookahead_klass_map))
      {
	out->rule.निकालो ();
	c->serializer->revert (o_snap);
      }
    }

    bool ret = bool (out->rule);
    अगर (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (rule.sanitize (c, यह));
  }

  रक्षित:
  Array16Of<typename Types::ढाँचा OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  खुला:
  DEFINE_आकार_ARRAY (2, rule);
};

ढाँचा <typename Types>
काष्ठा ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (माप (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_आकार + Offset24::static_आकार, "");

ढाँचा <typename Types>
काष्ठा ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

ढाँचा <typename Types>
काष्ठा ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    वापस
    + hb_zip (यह+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_map ([&] (स्थिर ChainRuleSet &_) { वापस _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (यह+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      वापस c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिर hb_pair_t<hb_codepoint_t, अचिन्हित> _) { वापस hb_pair_t<अचिन्हित, स्थिर ChainRuleSet&> (_.first, यह+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (यह+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      वापस c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिर hb_pair_t<hb_codepoint_t, अचिन्हित> _) { वापस hb_pair_t<अचिन्हित, स्थिर ChainRuleSet&> (_.first, यह+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर
  {
    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (यह+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    (यह+coverage).collect_coverage (c->निवेश);

    काष्ठा ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([digest] (स्थिर ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    स्थिर ChainRuleSet &rule_set = यह+ruleSet[(यह+coverage).get_coverage (c->glyphs[0])];
    काष्ठा ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    वापस rule_set.would_apply (c, lookup_context);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्थिर ChainRuleSet &rule_set = यह+ruleSet[सूचकांक_2];
    काष्ठा ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    स्थिर hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, यह, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && ruleSet.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, ruleSet);
};

ढाँचा <typename Types>
काष्ठा ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (!(यह+coverage).intersects (glyphs))
      वापस false;

    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

    hb_map_t caches[3] = {};
    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (यह+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    निवेश_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    वापस
    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_enumerate
    | hb_map ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर ChainRuleSet &> p)
	      { वापस निवेश_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    अगर (!(यह+coverage).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

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
    { वापस निवेश_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर typename Types::ढाँचा OffsetTo<ChainRuleSet>&> _)
                {
                  स्थिर ChainRuleSet& chainrule_set = यह+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    अगर (!(यह+coverage).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs)) वापस;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    काष्ठा ChainContextClosureLookupContext lookup_context = {
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
    { वापस निवेश_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<अचिन्हित, स्थिर typename Types::ढाँचा OffsetTo<ChainRuleSet>&> _)
                {
                  स्थिर ChainRuleSet& chainrule_set = यह+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर
  {
    अगर (!(यह+coverage).intersects (c->glyphs))
      वापस;

    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

    hb_map_t caches[3] = {};
    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_enumerate
    | hb_filter([&] (अचिन्हित klass)
    { वापस निवेश_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (स्थिर ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    (यह+coverage).collect_coverage (c->निवेश);

    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

    काष्ठा ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (यह))
    | hb_apply ([digest, &निवेश_class_def, &lookahead_class_def] (स्थिर ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &निवेश_class_def, &lookahead_class_def); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

    अचिन्हित पूर्णांक सूचकांक_2 = निवेश_class_def.get_class (c->glyphs[0]);
    स्थिर ChainRuleSet &rule_set = यह+ruleSet[सूचकांक_2];
    काष्ठा ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def}
    };
    वापस rule_set.would_apply (c, lookup_context);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  अचिन्हित cache_cost () स्थिर
  {
    वापस (यह+निवेश_Class_Def).cost () + (यह+lookaheadClassDef).cost ();
  }
  अटल bool cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    वापस context_cache_func (c, op);
  }

  काष्ठा external_cache_t
  {
    hb_ot_layout_द्विआधारी_cache_t coverage;
    hb_ot_layout_mapping_cache_t निवेश_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  व्योम *external_cache_create () स्थिर
  {
    अचिन्हित गिनती = ruleSet.len;
    अचिन्हित आकार = माप (external_cache_t) -
		    HB_VAR_ARRAY * माप (hb_ot_layout_ruleset_digest_t) +
		    गिनती * माप (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (आकार);
    अगर (likely (cache))
    {
      cache->coverage.सफा ();
      cache->निवेश_class.सफा ();
      cache->lookahead_class.सफा ();
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
	(यह+ruleSet[i]).collect_first_निवेश_classes (&cache->rule_sets[i]);
    }
    वापस cache;
  }
  bool apply_cached (hb_ot_apply_context_t *c, व्योम *external_cache) स्थिर { वापस _apply (c, true, external_cache); }
  bool apply (hb_ot_apply_context_t *c, व्योम *external_cache) स्थिर { वापस _apply (c, false, external_cache); }
  bool _apply (hb_ot_apply_context_t *c, bool cached, व्योम *external_cache) स्थिर
  {
    TRACE_APPLY (यह);
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage_द्विआधारी (c->बफर->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
#पूर्ण_अगर
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्थिर ClassDef &backtrack_class_def = यह+backtrackClassDef;
    स्थिर ClassDef &निवेश_class_def = यह+निवेश_Class_Def;
    स्थिर ClassDef &lookahead_class_def = यह+lookaheadClassDef;

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_तथ्य_t निवेश_cache_तथ्य = {
      &निवेश_class_def,
      cache ? &cache->निवेश_class : nullptr
    };
    match_class_cache_तथ्य_t lookahead_cache_तथ्य = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    स्थिर व्योम *निवेश_match_तथ्य = cached
      ? static_cast<स्थिर व्योम *> (&निवेश_class_def)
      : static_cast<स्थिर व्योम *> (&निवेश_cache_तथ्य);
    स्थिर व्योम *lookahead_match_तथ्य = cached
      ? static_cast<स्थिर व्योम *> (&lookahead_class_def)
      : static_cast<स्थिर व्योम *> (&lookahead_cache_तथ्य);
    bool backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    स्थिर व्योम *backtrack_match_तथ्य = backtrack_uses_lookahead_cache
      ? static_cast<स्थिर व्योम *> (&lookahead_cache_तथ्य)
      : static_cast<स्थिर व्योम *> (&backtrack_class_def);
#पूर्ण_अगर

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    काष्ठा ChainContextApplyLookupContext lookup_context = {
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_तथ्य,
       निवेश_match_तथ्य,
       lookahead_match_तथ्य},
      cached ? get_class_cached2_मूल्य : get_class_cache_मूल्य
#अन्यथा
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_मूल्य : get_class_मूल्य
#पूर्ण_अगर
    };

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    सूचकांक_2 = cached
         ? get_class_cached2 (निवेश_class_def, c->बफर->cur())
         : निवेश_class_def.get_class (c->बफर->cur().codepoint, निवेश_cache_तथ्य.cache);
#अन्यथा
    सूचकांक_2 = cached
         ? get_class_cached2 (निवेश_class_def, c->बफर->cur())
          : निवेश_class_def.get_class (c->बफर->cur().codepoint);
#पूर्ण_अगर
    स्थिर ChainRuleSet &rule_set = यह+ruleSet[सूचकांक_2];
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    स्थिर hb_ot_layout_ruleset_digest_t *digest = cache && सूचकांक_2 < ruleSet.len ? &cache->rule_sets[सूचकांक_2] : nullptr;
#अन्यथा
    स्थिर hb_ot_layout_ruleset_digest_t *digest = nullptr;
#पूर्ण_अगर
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, यह);

    hb_map_t backtrack_klass_map;
    hb_map_t निवेश_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, यह, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->निवेश_Class_Def.serialize_subset (c, निवेश_Class_Def, यह, &निवेश_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, यह, &lookahead_klass_map);

    अगर (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   निवेश_klass_map,
						   lookahead_klass_map)))
      return_trace (false);

    स्थिर hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (यह+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (यह+निवेश_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    पूर्णांक non_zero_सूचकांक = -1, सूचकांक_2 = 0;
    bool ret = true;
    स्थिर hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    स्वतः last_non_zero = c->serializer->snapshot ();
    क्रम (स्थिर स्वतः& _ : + hb_enumerate (ruleSet)
			 | hb_filter (निवेश_klass_map, hb_first))
    {
      स्वतः *o = out->ruleSet.serialize_append (c->serializer);
      अगर (unlikely (!o))
      {
	ret = false;
	अवरोध;
      }
      अगर (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, यह,
			       lookup_map,
			       &backtrack_klass_map,
			       &निवेश_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_सूचकांक = सूचकांक_2;
      }

      सूचकांक_2++;
    }

    अगर (!ret || non_zero_सूचकांक == -1) return_trace (false);

    // prune empty trailing ruleSets
    अगर (सूचकांक_2 > non_zero_सूचकांक) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_सूचकांक + 1;
    }

    return_trace (bool (out->ruleSet));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) &&
		  backtrackClassDef.sanitize (c, यह) &&
		  निवेश_Class_Def.sanitize (c, यह) &&
		  lookaheadClassDef.sanitize (c, यह) &&
		  ruleSet.sanitize (c, यह));
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
  खुला:
  DEFINE_आकार_ARRAY (4 + 4 * Types::LOffset::static_आकार, ruleSet);
};

ढाँचा <typename Types>
काष्ठा ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    अगर (!(यह+निवेश[0]).intersects (glyphs))
      वापस false;

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {यह, यह, यह}
    };
    वापस chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     निवेश.len, निवेश.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    अगर (!(यह+निवेश[0]).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs))
      वापस;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {यह, यह, यह},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (स्थिर HBUINT16 *) backtrack.arrayZ,
				 निवेश.len, (स्थिर HBUINT16 *) निवेश.arrayZ + 1,
				 lookahead.len, (स्थिर HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 यह,
				 निवेश[0], lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    अगर (!(यह+निवेश[0]).intersects (c->glyphs))
      वापस;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    अगर (unlikely (!cur_active_glyphs))
      वापस;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    काष्ठा ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {यह, यह, यह}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  निवेश.len, निवेश.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर
  {
    अगर (!intersects (c->glyphs))
      वापस;

    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    (यह+निवेश[0]).collect_coverage (c->निवेश);

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);

    काष्ठा ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {यह, यह, यह}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 निवेश.len, निवेश.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    अगर (निवेश.len > 1)
      (यह+निवेश[1]).collect_coverage (digest);
    अन्यथा अगर (lookahead.len)
      (यह+lookahead[0]).collect_coverage (digest);
    अन्यथा
      *digest = hb_set_digest_t::full ();
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    काष्ठा ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {यह, यह, यह}
    };
    वापस chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     निवेश.len, निवेश.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  स्थिर Coverage &get_coverage () स्थिर
  {
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    वापस यह+निवेश[0];
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    अचिन्हित पूर्णांक सूचकांक_2 = (यह+निवेश[0]).get_coverage (c->बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    काष्ठा ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {यह, यह, यह}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      निवेश.len, निवेश.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  bool serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, स्थिर व्योम* base) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    अगर (unlikely (!out->serialize (c->serializer, 0u))) return_trace (false);

    क्रम (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      अगर (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (false);
    }

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    अगर (unlikely (!c->serializer->embed (यह->format))) return_trace (false);

    अगर (!serialize_coverage_offsets (c, backtrack.iter (), यह))
      return_trace (false);

    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    अगर (!serialize_coverage_offsets (c, निवेश.iter (), यह))
      return_trace (false);

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    अगर (!serialize_coverage_offsets (c, lookahead.iter (), यह))
      return_trace (false);

    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    स्थिर hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_गिनती_2 = c->serializer->copy<HBUINT16> (lookup.len);
    अगर (!lookup_गिनती_2) return_trace (false);

    अचिन्हित गिनती = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!backtrack.sanitize (c, यह))) return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    अगर (unlikely (!निवेश.sanitize (c, यह))) return_trace (false);
    hb_barrier ();
    अगर (unlikely (!निवेश.len)) return_trace (false); /* To be consistent with Context. */
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    अगर (unlikely (!lookahead.sanitize (c, यह))) return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
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
  खुला:
  DEFINE_आकार_MIN (10);
};

काष्ठा ChainContext
{
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    हाल 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#अगर_अघोषित HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#पूर्ण_अगर
  } u;
};


ढाँचा <typename T>
काष्ठा ExtensionFormat1
{
  अचिन्हित पूर्णांक get_type () स्थिर { वापस extensionLookupType; }

  ढाँचा <typename X>
  स्थिर X& get_subtable () स्थिर
  { वापस यह + reinterpret_cast<स्थिर Offset32To<typename T::SubTable> &> (extensionOffset); }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, यह))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    स्थिर स्वतः& src_offset =
        reinterpret_cast<स्थिर Offset32To<typename T::SubTable> &> (extensionOffset);
    स्वतः& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, यह, get_type ()));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  खुला:
  DEFINE_आकार_STATIC (8);
};

ढाँचा <typename T>
काष्ठा Extension
{
  अचिन्हित पूर्णांक get_type () स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_type ();
    शेष:वापस 0;
    }
  }
  ढाँचा <typename X>
  स्थिर X& get_subtable () स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.ढाँचा get_subtable<typename T::SubTable> ();
    शेष:वापस Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  ढाँचा <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.subset (c);
    शेष: वापस c->default_return_मूल्य ();
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

काष्ठा hb_ot_layout_lookup_accelerator_t
{
  ढाँचा <typename TLookup>
  अटल hb_ot_layout_lookup_accelerator_t *create (स्थिर TLookup &lookup)
  {
    अचिन्हित गिनती = lookup.get_subtable_गिनती ();

    अचिन्हित आकार = माप (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * माप (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    गिनती * माप (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    स्वतः *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, आकार);
    अगर (unlikely (!thiz))
      वापस nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    जबतक (गिनती && !thiz->subtables[गिनती - 1].apply_func)
      गिनती--;

    thiz->digest.init ();
    क्रम (स्वतः& subtable : hb_iter (thiz->subtables, गिनती))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->गिनती = गिनती;

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      अगर (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#पूर्ण_अगर

    वापस thiz;
  }

  व्योम fini ()
  {
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      hb_free (subtables[i].external_cache);
#पूर्ण_अगर
  }

  bool may_have (hb_codepoint_t g) स्थिर
  { वापस digest.may_have (g); }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  bool apply (hb_ot_apply_context_t *c, bool use_cache) स्थिर
  {
    अगर (गिनती == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
      अगर (use_cache)
	वापस subtables[0].apply_cached_no_digest (c);
#पूर्ण_अगर
      वापस subtables[0].apply_no_digest (c);
    }
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    अगर (use_cache)
    {
      वापस
      + hb_iter (hb_iter (subtables, गिनती))
      | hb_map ([&c] (स्थिर hb_accelerate_subtables_context_t::hb_applicable_t &_) { वापस _.apply_cached (c); })
      | hb_any
      ;
    }
    अन्यथा
#पूर्ण_अगर
    {
      वापस
      + hb_iter (hb_iter (subtables, गिनती))
      | hb_map ([&c] (स्थिर hb_accelerate_subtables_context_t::hb_applicable_t &_) { वापस _.apply (c); })
      | hb_any
      ;
    }
    वापस false;
  }

  bool cache_enter (hb_ot_apply_context_t *c) स्थिर
  {
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    वापस subtable_cache_user_idx != (अचिन्हित) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#अन्यथा
    वापस false;
#पूर्ण_अगर
  }
  व्योम cache_leave (hb_ot_apply_context_t *c) स्थिर
  {
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#पूर्ण_अगर
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  निजी:
  अचिन्हित गिनती = 0; /* Number of subtables in the array. */
#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
  अचिन्हित subtable_cache_user_idx = (अचिन्हित) -1;
#पूर्ण_अगर
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

काष्ठा GSUBGPOS
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

  माप_प्रकार get_आकार () स्थिर
  {
    अगर (version.major != 1) वापस version.static_आकार;

    वापस min_आकार +
	   (version.to_int () >= 0x00010001u ? featureVars.static_आकार : 0) +
#अगर_अघोषित HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_आकार : 0)
#अन्यथा
	   0
#पूर्ण_अगर
	   ;
  }

  स्थिर ScriptList &get_script_list () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u && scriptList2) वापस यह + scriptList2;
#पूर्ण_अगर
    वापस यह + scriptList;
  }
  स्थिर FeatureList &get_feature_list () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u && featureList2) वापस यह + featureList2;
#पूर्ण_अगर
    वापस यह + featureList;
  }
  ढाँचा <typename TLookup>
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (यह, get_आकार ())))
      return_trace (false);
    hb_barrier ();

    bool script_list_ok = scriptList.sanitize (c, यह);
    bool feature_list_ok = featureList.sanitize (c, यह);
    bool lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, यह);

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u)
    {
      अगर (scriptList2) script_list_ok = scriptList2.sanitize (c, यह);
      अगर (featureList2) feature_list_ok = featureList2.sanitize (c, यह);
      अगर (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, यह);
    }
#पूर्ण_अगर

    अगर (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (false);

#अगर_अघोषित HB_NO_VAR
    अगर (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, यह, &get_feature_list ()))))
      return_trace (false);
#पूर्ण_अगर

    return_trace (true);
  }

  ढाँचा <typename TLookup>
  bool subset (hb_subset_layout_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (version.major != 1) return_trace (false);

    स्वतः *out = c->subset_context->serializer->start_embed (यह);
    अगर (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (false);

    out->version = version;

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u)
    {
      अगर (unlikely (!c->subset_context->serializer->extend_आकार (out, get_आकार ())))
        return_trace (false);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      अगर (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 यह, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 यह, c);

      अगर (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 यह, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 यह, c);

      अगर (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, यह, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, यह, c);
    }
    अन्यथा
#पूर्ण_अगर
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     यह,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     यह,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					यह,
					c);
    }

#अगर_अघोषित HB_NO_VAR
    अगर (version.to_int () >= 0x00010001u)
    {
      स्वतः snapshot = c->subset_context->serializer->snapshot ();
      अगर (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (false);

      // if all axes are pinned all feature vars are dropped.
      bool ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, यह, c);
      अगर (!ret)
      {
        out->featureVars = 0;
	अगर (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#पूर्ण_अगर

    return_trace (true);
  }

  अचिन्हित पूर्णांक get_lookup_गिनती () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u && lookupList2)
      वापस (यह + lookupList2).len;
#पूर्ण_अगर
    वापस (यह + lookupList).len;
  }
  स्थिर Lookup& get_lookup (अचिन्हित पूर्णांक i) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u && lookupList2)
      वापस (यह + lookupList2)[i];
#पूर्ण_अगर
    वापस (यह + lookupList)[i];
  }
  स्थिर FeatureVariations &get_feature_variations () स्थिर
  {
    वापस (version.to_int () >= 0x00010001u && hb_barrier ()) ? यह+featureVars : Null (FeatureVariations);
  }

  bool has_तथ्य () स्थिर { वापस version.to_int (); }
  अचिन्हित पूर्णांक get_script_गिनती () स्थिर
  { वापस get_script_list ().len; }
  स्थिर Tag& get_script_tag (अचिन्हित पूर्णांक i) स्थिर
  { वापस get_script_list ().get_tag (i); }
  अचिन्हित पूर्णांक get_script_tags (अचिन्हित पूर्णांक start_offset,
				अचिन्हित पूर्णांक *script_गिनती /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) स्थिर
  { वापस get_script_list ().get_tags (start_offset, script_गिनती, script_tags); }
  स्थिर Script& get_script (अचिन्हित पूर्णांक i) स्थिर
  { वापस get_script_list ()[i]; }
  bool find_script_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  { वापस get_script_list ().find_सूचकांक (tag, सूचकांक_2); }

  अचिन्हित पूर्णांक get_feature_गिनती () स्थिर
  { वापस get_feature_list ().len; }
  hb_tag_t get_feature_tag (अचिन्हित पूर्णांक i) स्थिर
  { वापस i == सूचकांक::NOT_FOUND_सूचकांक ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  अचिन्हित पूर्णांक get_feature_tags (अचिन्हित पूर्णांक start_offset,
				 अचिन्हित पूर्णांक *feature_गिनती_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) स्थिर
  { वापस get_feature_list ().get_tags (start_offset, feature_गिनती_2, feature_tags); }
  स्थिर Feature& get_feature (अचिन्हित पूर्णांक i) स्थिर
  { वापस get_feature_list ()[i]; }
  bool find_feature_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  { वापस get_feature_list ().find_सूचकांक (tag, सूचकांक_2); }

  bool find_variations_सूचकांक (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक अंक_coords,
			      अचिन्हित पूर्णांक *सूचकांक_2,
			      ItemVarStoreInstancer *instancer) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    *सूचकांक_2 = FeatureVariations::NOT_FOUND_सूचकांक;
    वापस false;
#पूर्ण_अगर
    वापस get_feature_variations ().find_सूचकांक (coords, अंक_coords, सूचकांक_2, instancer);
  }
  bool get_feature_variations_state (स्थिर पूर्णांक *coords,
				     अचिन्हित पूर्णांक अंक_coords,
				     अचिन्हित पूर्णांक *सूचकांक_2,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<अचिन्हित> *lookup_conditions) स्थिर
  {
    lookup_conditions->reset ();
#अगर_घोषित HB_NO_VAR
    *सूचकांक_2 = FeatureVariations::NOT_FOUND_सूचकांक;
    वापस true;
#अन्यथा
    find_variations_सूचकांक (coords, अंक_coords, सूचकांक_2, instancer);
    वापस get_feature_variations ().collect_lookup_conditions (
	coords,
	अंक_coords,
	instancer,
	lookup_conditions);
#पूर्ण_अगर
  }
  bool get_lookup_variations (अचिन्हित feature_सूचकांक,
                              अचिन्हित variations_सूचकांक,
                              hb_sorted_array_t<स्थिर अचिन्हित> conditions,
                              hb_set_t *lookup_indexes) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस false;
#अन्यथा
    वापस get_feature_variations ().get_lookup_variations (
	feature_सूचकांक, get_feature_variation (feature_सूचकांक, variations_सूचकांक),
	conditions, lookup_indexes);
#पूर्ण_अगर
  }
  स्थिर Feature& get_feature_variation (अचिन्हित पूर्णांक feature_सूचकांक,
					अचिन्हित पूर्णांक variations_सूचकांक) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    अगर (FeatureVariations::NOT_FOUND_सूचकांक != variations_सूचकांक &&
	version.to_int () >= 0x00010001u)
    {
      स्थिर Feature *feature = get_feature_variations ().find_substitute (variations_सूचकांक,
									  feature_सूचकांक);
      अगर (feature)
	वापस *feature;
    }
#पूर्ण_अगर
    वापस get_feature (feature_सूचकांक);
  }

  व्योम feature_variation_collect_lookups (स्थिर hb_set_t *feature_indexes,
					  स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#पूर्ण_अगर
  }

  व्योम collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#पूर्ण_अगर
  }

#अगर_अघोषित HB_NO_VAR
  व्योम collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिर
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#पूर्ण_अगर

  ढाँचा <typename TLookup>
  व्योम closure_lookups (hb_face_t      *face,
			स्थिर hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) स्थिर
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::ढाँचा dispatch_recurse_func<hb_closure_lookups_context_t>);

    क्रम (अचिन्हित lookup_सूचकांक : *lookup_indexes)
      reinterpret_cast<स्थिर TLookup &> (get_lookup (lookup_सूचकांक)).closure_lookups (&c, lookup_सूचकांक);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  व्योम prune_langsys (स्थिर hb_map_t *duplicate_feature_map,
                      स्थिर hb_set_t *layout_scripts,
                      hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) स्थिर
  {
    hb_prune_langsys_context_t c (यह, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    अचिन्हित गिनती = get_script_गिनती ();
    क्रम (अचिन्हित script_सूचकांक = 0; script_सूचकांक < गिनती; script_सूचकांक++)
    {
      स्थिर Tag& tag = get_script_tag (script_सूचकांक);
      अगर (!layout_scripts->has (tag)) जारी;
      स्थिर Script& s = get_script (script_सूचकांक);
      s.prune_langsys (&c, script_सूचकांक);
    }
  }

  व्योम prune_features (स्थिर hb_map_t *lookup_indices, /* IN */
		       स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       स्थिर hb_hashmap_t<अचिन्हित, स्थिर Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    अगर (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      वापस;
    }
#पूर्ण_अगर

    क्रम (अचिन्हित i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      अगर (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        जारी;


      स्थिर Feature *f = &(get_feature (i));
      स्थिर Feature** p = nullptr;
      अगर (feature_substitutes_map->has (i, &p))
        f = *p;

      अगर (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        जारी;

      अगर (!f->intersects_lookup_indexes (lookup_indices)
#अगर_अघोषित HB_NO_VAR
          && !alternate_feature_indices.has (i)
#पूर्ण_अगर
	  )
	feature_indices->del (i);
    }
  }

  व्योम collect_name_ids (स्थिर hb_map_t *feature_सूचकांक_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         स्थिर hb_hashmap_t<अचिन्हित, स्थिर Feature*> *feature_substitutes_map,
                         स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         स्थिर hb_set_t *catch_all_features,
                         bool all_axes_pinned HB_UNUSED) स्थिर
  {
    अचिन्हित गिनती = get_feature_गिनती ();
    क्रम (अचिन्हित i = 0 ; i < गिनती; i++)
    {
      अगर (!feature_सूचकांक_map->has (i)) जारी;
      hb_tag_t tag = get_feature_tag (i);
      स्थिर Feature *feature = &get_feature (i);
      स्थिर Feature **substitute;
      अगर (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        अगर (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#अगर_अघोषित HB_NO_VAR
    अगर (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_सूचकांक_map, feature_record_cond_idx_map,
                                                  [यह] (अचिन्हित i) { वापस get_feature_tag (i); },
                                                  nameids_to_retain);
#पूर्ण_अगर
  }

  ढाँचा <typename T>
  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = true;
      यह->table = sc.reference_table<T> (face);

      अगर (unlikely (यह->table->is_blocklisted (यह->table.get_blob (), face)))
      {
	hb_blob_destroy (यह->table.get_blob ());
	यह->table = hb_blob_get_empty ();
      }

      यह->lookup_गिनती = table->get_lookup_गिनती ();

      यह->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (यह->lookup_गिनती, माप (*accels));
      अगर (unlikely (!यह->accels))
      {
	यह->lookup_गिनती = 0;
	यह->table.destroy ();
	यह->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      क्रम (अचिन्हित पूर्णांक i = 0; i < यह->lookup_गिनती; i++)
      {
	स्वतः *accel = यह->accels[i].get_relaxed ();
	अगर (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (यह->accels);
      यह->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिर { वापस table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (अचिन्हित lookup_सूचकांक) स्थिर
    {
      अगर (unlikely (lookup_सूचकांक >= lookup_गिनती)) वापस nullptr;

    retry:
      स्वतः *accel = accels[lookup_सूचकांक].get_acquire ();
      अगर (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_सूचकांक));
	अगर (unlikely (!accel))
	  वापस nullptr;

	अगर (unlikely (!accels[lookup_सूचकांक].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  जाओ retry;
	}
      }

      वापस accel;
    }

    hb_blob_ptr_t<T> table;
    अचिन्हित पूर्णांक lookup_गिनती;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  रक्षित:
  ढाँचा <typename TLookup>
  स्थिर Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () स्थिर
  { वापस reinterpret_cast<स्थिर Offset16To<TLookupList<TLookup>>&> (lookupList); }

#अगर_अघोषित HB_NO_BEYOND_64K
  ढाँचा <typename TLookup>
  स्थिर Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () स्थिर
  { वापस reinterpret_cast<स्थिर Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#पूर्ण_अगर

  ढाँचा <typename TLookup>
  स्थिर Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () स्थिर
  { वापस reinterpret_cast<स्थिर Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  ढाँचा <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { वापस reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#अगर_अघोषित HB_NO_BEYOND_64K
  ढाँचा <typename TLookup>
  स्थिर Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () स्थिर
  { वापस reinterpret_cast<स्थिर Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#पूर्ण_अगर

  स्थिर Offset16To<RecordListOfFeature>& subset_feature_list_offset () स्थिर
  { वापस reinterpret_cast<स्थिर Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { वापस reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#अगर_अघोषित HB_NO_BEYOND_64K
  स्थिर Offset32To<RecordListOfFeature>& subset_feature_list2_offset () स्थिर
  { वापस reinterpret_cast<स्थिर Offset32To<RecordListOfFeature>&> (featureList2); }
#पूर्ण_अगर

  स्थिर व्योम* get_lookup_list_field_offset () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.to_int () >= 0x00010002u && lookupList2) वापस &lookupList2;
#पूर्ण_अगर
    वापस &lookupList;
  }

  ढाँचा <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  अटल bool serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       स्थिर OffsetIn& in,
				       स्थिर Base *base,
				       Ts&&... ds)
  {
    out = 0;
    अगर (in.is_null ()) वापस false;

    स्वतः *s = c->serializer;
    s->push ();
    bool ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    अगर (ret) s->add_link (out, s->निकालो_pack ());
    अन्यथा s->निकालो_discard ();
    वापस ret;
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
#अगर_अघोषित HB_NO_BEYOND_64K
  Offset32To<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32To<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32To<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#पूर्ण_अगर
  खुला:
  DEFINE_आकार_MIN (10);
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_LAYOUT_GSUBGPOS_HH */
