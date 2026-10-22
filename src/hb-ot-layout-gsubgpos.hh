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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-buffer.hh"
#അന്തര്ഭാവ "hb-map.hh"
#അന്തര്ഭാവ "hb-set.hh"
#അന്തര്ഭാവ "hb-ot-map.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-depend-data.hh"


നാമാകാശ OT {


ഘടന hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, തർക്കം>
{
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { തിരികെ obj.intersects (ഇത്->glyphs); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }

  സ്ഥിരം hb_set_t *glyphs;

  hb_intersects_context_t (സ്ഥിരം hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

ഘടന hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, തർക്കം>
{
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { തിരികെ obj.may_have_non_1to1 (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }
};

ഘടന hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  തരനിർവചനം return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { obj.closure (ഇത്); തിരികെ hb_empty_t (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }
  ശൂന്യം recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    എങ്കിൽ (unlikely (nesting_level_ഇടത് == 0 || !recurse_func))
      തിരികെ;

    nesting_level_ഇടത്--;
    recurse_func (ഇത്, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_ഇടത്++;
  }

  ശൂന്യം reset_lookup_visit_എണ്ണം ()
  { lookup_എണ്ണം = 0; }

  തർക്കം lookup_limit_exceeded ()
  { തിരികെ lookup_എണ്ണം > HB_MAX_LOOKUP_VISIT_എണ്ണം; }

  തർക്കം should_visit_lookup (unsigned പൂർണ്ണസംഖ്യ lookup_index)
  {
    എങ്കിൽ (lookup_എണ്ണം++ > HB_MAX_LOOKUP_VISIT_എണ്ണം)
      തിരികെ അസത്യം;

    എങ്കിൽ (is_lookup_done (lookup_index))
      തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  തർക്കം is_lookup_done (unsigned പൂർണ്ണസംഖ്യ lookup_index)
  {
    എങ്കിൽ (unlikely (done_lookups_glyph_എണ്ണം->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      തിരികെ സത്യം;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned എടുക്കുക = glyphs->get_population ();
    എങ്കിൽ (unlikely (എടുക്കുക == HB_MAP_മൂല്യം_INVALID)) // Avoid sentinel interference.
      എടുക്കുക--;
    എങ്കിൽ (done_lookups_glyph_എണ്ണം->get (lookup_index) != എടുക്കുക)
    {
      done_lookups_glyph_എണ്ണം->set (lookup_index, എടുക്കുക);

      എങ്കിൽ (!done_lookups_glyph_set->has (lookup_index))
      {
	എങ്കിൽ (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  തിരികെ സത്യം;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    എങ്കിൽ (unlikely (covered_glyph_set->in_error ()))
      തിരികെ സത്യം;
    എങ്കിൽ (parent_active_glyphs ().is_subset (*covered_glyph_set))
      തിരികെ സത്യം;

    covered_glyph_set->union_ (parent_active_glyphs ());
    തിരികെ അസത്യം;
  }

  സ്ഥിരം hb_set_t& previous_parent_active_glyphs () {
    എങ്കിൽ (active_glyphs_അട്ടി.length <= 1)
      തിരികെ *glyphs;

    തിരികെ active_glyphs_അട്ടി[active_glyphs_അട്ടി.length - 2];
  }

  സ്ഥിരം hb_set_t& parent_active_glyphs ()
  {
    എങ്കിൽ (!active_glyphs_അട്ടി)
      തിരികെ *glyphs;

    തിരികെ active_glyphs_അട്ടി.tail ();
  }

  hb_set_t* തള്ളുക_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_അട്ടി.push ();
    എങ്കിൽ (unlikely (active_glyphs_അട്ടി.in_error ()))
      തിരികെ nullptr;
    തിരികെ s;
  }

  തർക്കം എടുക്കുക_cur_done_glyphs ()
  {
    എങ്കിൽ (!active_glyphs_അട്ടി)
      തിരികെ അസത്യം;

    active_glyphs_അട്ടി.എടുക്കുക ();
    തിരികെ സത്യം;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_അട്ടി;
  recurse_func_t recurse_func = nullptr;
  unsigned പൂർണ്ണസംഖ്യ nesting_level_ഇടത്;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_എണ്ണം_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned പൂർണ്ണസംഖ്യ nesting_level_ഇടത്_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_ഇടത് (nesting_level_ഇടത്_2),
			  done_lookups_glyph_എണ്ണം (done_lookups_glyph_എണ്ണം_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  ശൂന്യം set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ശൂന്യം flush ()
  {
    output->del_range (face->get_അക്കം_glyphs (), HB_SET_മൂല്യം_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_അട്ടി.എടുക്കുക ();
    active_glyphs_അട്ടി.reset ();
  }

  സ്വകാര്യ:
  hb_map_t *done_lookups_glyph_എണ്ണം;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം = 0;
};



ഘടന hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  തരനിർവചനം return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { obj.closure_lookups (ഇത്); തിരികെ hb_empty_t (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }
  ശൂന്യം recurse (unsigned lookup_index)
  {
    എങ്കിൽ (unlikely (nesting_level_ഇടത് == 0 || !recurse_func))
      തിരികെ;

    /* Return if new lookup was recursed to before. */
    എങ്കിൽ (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      തിരികെ;

    nesting_level_ഇടത്--;
    recurse_func (ഇത്, lookup_index);
    nesting_level_ഇടത്++;
  }

  ശൂന്യം set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  ശൂന്യം set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  തർക്കം lookup_limit_exceeded ()
  {
    തർക്കം ret = lookup_എണ്ണം > HB_MAX_LOOKUP_VISIT_എണ്ണം;
    എങ്കിൽ (ret)
      DEBUG_സന്ദേശം (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    തിരികെ ret; }

  തർക്കം is_lookup_visited (unsigned lookup_index)
  {
    എങ്കിൽ (unlikely (lookup_എണ്ണം++ > HB_MAX_LOOKUP_VISIT_എണ്ണം))
    {
      DEBUG_സന്ദേശം (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_എണ്ണം, lookup_index);
      തിരികെ സത്യം;
    }

    എങ്കിൽ (unlikely (visited_lookups->in_error ()))
      തിരികെ സത്യം;

    തിരികെ visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  സ്ഥിരം hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned പൂർണ്ണസംഖ്യ nesting_level_ഇടത്;

  hb_closure_lookups_context_t (hb_face_t *face_,
				സ്ഥിരം hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_ഇടത്_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_ഇടത് (nesting_level_ഇടത്_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_എണ്ണം (0) {}

  ശൂന്യം set_recurse_func (recurse_func_t func) { recurse_func = func; }

  സ്വകാര്യ:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം;
};

ഘടന hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, തർക്കം>
{
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { തിരികെ obj.would_apply (ഇത്); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }

  hb_face_t *face;
  സ്ഥിരം hb_codepoint_t *glyphs;
  unsigned പൂർണ്ണസംഖ്യ len;
  തർക്കം zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    സ്ഥിരം hb_codepoint_t *glyphs_,
			    unsigned പൂർണ്ണസംഖ്യ len_,
			    തർക്കം zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

ഘടന hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  തരനിർവചനം return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_index);
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { obj.collect_glyphs (ഇത്); തിരികെ hb_empty_t (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }
  ശൂന്യം recurse (unsigned പൂർണ്ണസംഖ്യ lookup_index)
  {
    എങ്കിൽ (unlikely (nesting_level_ഇടത് == 0 || !recurse_func))
      തിരികെ;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    എങ്കിൽ (output == hb_set_get_empty ())
      തിരികെ;

    /* Return if new lookup was recursed to before. */
    എങ്കിൽ (recursed_lookups->has (lookup_index))
      തിരികെ;

    hb_set_t *old_before = before;
    hb_set_t *old_നിവേശം  = നിവേശം;
    hb_set_t *old_after  = after;
    before = നിവേശം = after = hb_set_get_empty ();

    nesting_level_ഇടത്--;
    recurse_func (ഇത്, lookup_index);
    nesting_level_ഇടത്++;

    before = old_before;
    നിവേശം  = old_നിവേശം;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *നിവേശം;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned പൂർണ്ണസംഖ്യ nesting_level_ഇടത്;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_നിവേശം,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned പൂർണ്ണസംഖ്യ nesting_level_ഇടത്_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      നിവേശം  (glyphs_നിവേശം  ? glyphs_നിവേശം  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_ഇടത് (nesting_level_ഇടത്_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  ശൂന്യം set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ഘടന hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  എണ്ണൽ glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  സ്ഥിതം constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  ഘടന glyph_set_cache_താക്കോൽ_t
  {
    glyph_set_cache_താക്കോൽ_t () = സ്ഥിരസ്ഥിതി;
    glyph_set_cache_താക്കോൽ_t (സ്ഥിരം ശൂന്യം *data_, unsigned മൂല്യം_3, glyph_set_type_t type_)
      : data (data_), value (മൂല്യം_3), type (type_) {}

    തർക്കം operator == (സ്ഥിരം glyph_set_cache_താക്കോൽ_t &o) സ്ഥിരം
    { തിരികെ data == o.data && value == o.value && type == o.type; }

    uint32_t hash () സ്ഥിരം
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      തിരികെ current * 31 + hb_hash ((unsigned) type);
    }

    സ്ഥിരം ശൂന്യം *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  ഘടന context_set_cache_താക്കോൽ_t
  {
    context_set_cache_താക്കോൽ_t () = സ്ഥിരസ്ഥിതി;
    context_set_cache_താക്കോൽ_t (സ്ഥിരം ശൂന്യം *rule_,
			     സ്ഥിരം ശൂന്യം *record_,
			     സ്ഥിരം ശൂന്യം *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned മൂല്യം_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (മൂല്യം_3), format (format_) {}

    തർക്കം operator == (സ്ഥിരം context_set_cache_താക്കോൽ_t &o) സ്ഥിരം
    {
      തിരികെ rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () സ്ഥിരം
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      തിരികെ current * 31 + hb_hash (format);
    }

    സ്ഥിരം ശൂന്യം *rule = nullptr;
    സ്ഥിരം ശൂന്യം *record = nullptr;
    സ്ഥിരം ശൂന്യം *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  ഘടന recurse_താക്കോൽ_t
  {
    recurse_താക്കോൽ_t () = സ്ഥിരസ്ഥിതി;
    recurse_താക്കോൽ_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    തർക്കം operator == (സ്ഥിരം recurse_താക്കോൽ_t &o) സ്ഥിരം
    {
      തിരികെ lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () സ്ഥിരം
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      തിരികെ current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  തരനിർവചനം return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { obj.depend (ഇത്); തിരികെ hb_empty_t (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }

  ശൂന്യം recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    എങ്കിൽ (lookups_seen.has (lookup_idx)) തിരികെ;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_താക്കോൽ = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    എങ്കിൽ (!recurse_path_to_index.has (path_താക്കോൽ, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      എങ്കിൽ (unlikely (!recurse_path_to_index.set (path_താക്കോൽ, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	തിരികെ;
      }
    }
    അല്ലെങ്കിൽ
      path_index = *path;
    recurse_path = path_index;
    recurse_func (ഇത്, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  ഘടന active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    തർക്കം use_വേര് = അസത്യം;
  };

  സ്ഥിരം hb_set_t& parent_active_glyphs ()
  {
    എങ്കിൽ (!active_glyphs_അട്ടി_depth)
      തിരികെ *glyphs;

    സ്ഥിരം active_glyphs_frame_t &frame = active_glyphs_അട്ടി[active_glyphs_അട്ടി_depth - 1];
    തിരികെ frame.use_വേര് ? *glyphs : frame.glyphs;
  }

  hb_set_t* തള്ളുക_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    എങ്കിൽ (active_glyphs_അട്ടി_depth == active_glyphs_അട്ടി.length &&
	unlikely (!active_glyphs_അട്ടി.തള്ളുക_or_fail ()))
      തിരികെ nullptr;

    active_glyphs_frame_t &frame = active_glyphs_അട്ടി[active_glyphs_അട്ടി_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_വേര് = അസത്യം;
    തിരികെ &frame.glyphs;
  }

  തർക്കം തള്ളുക_വേര്_active_glyphs ()
  {
    എങ്കിൽ (active_glyphs_അട്ടി_depth == active_glyphs_അട്ടി.length &&
	unlikely (!active_glyphs_അട്ടി.തള്ളുക_or_fail ()))
      തിരികെ അസത്യം;

    active_glyphs_അട്ടി[active_glyphs_അട്ടി_depth++].use_വേര് = സത്യം;
    തിരികെ സത്യം;
  }

  തർക്കം എടുക്കുക_cur_done_glyphs ()
  {
    എങ്കിൽ (!active_glyphs_അട്ടി_depth)
      തിരികെ അസത്യം;

    active_glyphs_അട്ടി_depth--;
    തിരികെ സത്യം;
  }

  തർക്കം get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    എങ്കിൽ (!active_glyphs_അട്ടി_depth ||
	active_glyphs_അട്ടി[active_glyphs_അട്ടി_depth - 1].use_വേര്)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      തിരികെ സത്യം;
    }

    active_glyphs_frame_t &frame = active_glyphs_അട്ടി[active_glyphs_അട്ടി_depth - 1];
    *active_idx = frame.active_idx;
    എങ്കിൽ (*active_idx != HB_CODEPOINT_INVALID)
      തിരികെ സത്യം;

    hb_codepoint_t glyph;
    എങ്കിൽ (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    അല്ലെങ്കിൽ എങ്കിൽ (!find_or_create_recurse_set (frame.glyphs, active_idx))
      തിരികെ അസത്യം;

    frame.active_idx = *active_idx;
    തിരികെ സത്യം;
  }

  തർക്കം get_context_set_cache_താക്കോൽ (സ്ഥിരം ശൂന്യം *rule,
				  സ്ഥിരം ശൂന്യം *record,
				  സ്ഥിരം ശൂന്യം *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_താക്കോൽ_t *key)
  {
    hb_codepoint_t active_idx;
    എങ്കിൽ (!get_parent_active_glyphs_index (&active_idx))
      തിരികെ അസത്യം;
    *key = context_set_cache_താക്കോൽ_t (rule, record, data, active_idx, value, format);
    തിരികെ സത്യം;
  }

  തർക്കം get_recurse_താക്കോൽ (unsigned lookup_idx, recurse_താക്കോൽ_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    എങ്കിൽ (!get_parent_active_glyphs_index (&active_idx))
      തിരികെ അസത്യം;

    *key = recurse_താക്കോൽ_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    തിരികെ !completed_recursions.has (*key);
  }

  ശൂന്യം finish_recurse (സ്ഥിരം recurse_താക്കോൽ_t &key)
  { depend_data->check_success (completed_recursions.set (key, സത്യം)); }

  ശൂന്യം reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  തർക്കം find_or_create_recurse_set (സ്ഥിരം hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    എങ്കിൽ (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      തിരികെ സത്യം;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    എങ്കിൽ (unlikely (!copy))
      തിരികെ depend_data->fail ();
    copy->set (set);
    എങ്കിൽ (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      തിരികെ depend_data->fail ();
    }

    *index = recurse_sets.length;
    എങ്കിൽ (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      തിരികെ depend_data->fail ();
    }
    എങ്കിൽ (unlikely (!recurse_sets.തള്ളുക_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      തിരികെ depend_data->fail ();
    എങ്കിൽ (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      തിരികെ depend_data->fail ();
    തിരികെ സത്യം;
  }

  സ്ഥിരം hb_set_t *find_or_create_glyph_set (സ്ഥിരം ശൂന്യം *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (സ്ഥിരം hb_set_t *set);

  ഘടന depend_scratch_t
  {
    ശൂന്യം clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      നിവേശം_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<സ്ഥിരം hb_set_t *> backtrack_sets;
    hb_vector_t<സ്ഥിരം hb_set_t *> lookahead_sets;
    hb_vector_t<സ്ഥിരം hb_set_t *> നിവേശം_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    എങ്കിൽ (scratch)
      depend_scratch_pool = scratch->next;
    അല്ലെങ്കിൽ
    {
      scratch = (depend_scratch_t *) hb_malloc (വലുപ്പം (depend_scratch_t));
      എങ്കിൽ (unlikely (!scratch))
      {
        depend_data->fail ();
        തിരികെ nullptr;
      }
      പുതിയ (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    തിരികെ scratch;
  }

  ശൂന്യം release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_അട്ടി;
  unsigned active_glyphs_അട്ടി_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<സ്ഥിരം hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_താക്കോൽ_t, തർക്കം> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_താക്കോൽ_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_താക്കോൽ_t, hb_codepoint_t> context_set_cache;
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
    ആകുമ്പോൾ (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  ശൂന്യം set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
ഘടന hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, സ്ഥിരം Coverage &>
{
  തരനിർവചനം സ്ഥിരം Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { തിരികെ obj.get_coverage (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ Null (Coverage); }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം
  {
    r.collect_coverage (set);
    തിരികെ അസത്യം;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

ഘടന matcher_t
{
  തരനിർവചനം തർക്കം (*match_func_t) (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data);

  template <typename context_t>
  ശൂന്യം init (സ്ഥിരം context_t *c, തർക്കം context_match = അസത്യം)
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

  ശൂന്യം set_match_func (match_func_t match_func_,
		       സ്ഥിരം ശൂന്യം *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  എണ്ണൽ may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) സ്ഥിരം
  {
    എങ്കിൽ (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      തിരികെ MATCH_NO;

    എങ്കിൽ (match_func)
      തിരികെ match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    തിരികെ MATCH_MAYBE;
  }

  എണ്ണൽ may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (സ്ഥിരം context_t *c,
		       സ്ഥിരം hb_glyph_info_t &info) സ്ഥിരം
  {
    എങ്കിൽ (!c->check_glyph_property (&info, lookup_props))
      തിരികെ SKIP_YES;

    എങ്കിൽ (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      തിരികെ SKIP_MAYBE;

    തിരികെ SKIP_NO;
  }

  പൊതു:
  unsigned പൂർണ്ണസംഖ്യ lookup_props = 0;
  hb_mask_t mask = -1;
  തർക്കം ignore_zwnj = അസത്യം;
  തർക്കം ignore_zwj = അസത്യം;
  തർക്കം ignore_hidden = അസത്യം;
  തർക്കം per_syllable = അസത്യം;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  സ്ഥിരം ശൂന്യം *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
ഘടന glyph_data_t
{
  template <typename T>
  glyph_data_t (സ്ഥിരം T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (വലുപ്പം (T) == 3)
#endif
  {
    static_assert (വലുപ്പം (T) == 2 || വലുപ്പം (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (wide)
      തിരികെ ((സ്ഥിരം HBUINT24 *) data)[i];
#endif
    തിരികെ ((സ്ഥിരം HBUINT16 *) data)[i];
  }

  സ്ഥിരം ശൂന്യം *data;
#ifndef HB_NO_BEYOND_64K
  തർക്കം wide;
#endif
};

template <typename context_t>
ഘടന skipping_iterator_t
{
  ശൂന്യം init (context_t *c_, തർക്കം context_match = അസത്യം)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  ശൂന്യം set_lookup_props (unsigned പൂർണ്ണസംഖ്യ lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  ശൂന്യം set_match_func (matcher_t::match_func_t match_func_,
		       സ്ഥിരം ശൂന്യം *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  ശൂന്യം set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (സ്ഥിരം HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (സ്ഥിരം HBUINT24 *) glyph_data.data : nullptr;
#അല്ലെങ്കിൽ
    match_glyph_data16 = (സ്ഥിരം HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ശൂന്യം reset (unsigned പൂർണ്ണസംഖ്യ start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  ശൂന്യം reset_back (unsigned പൂർണ്ണസംഖ്യ start_index_, തർക്കം from_out_buffer = അസത്യം)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ശൂന്യം reset_fast (unsigned പൂർണ്ണസംഖ്യ start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (സ്ഥിരം hb_glyph_info_t &info) സ്ഥിരം
  { തിരികെ matcher.may_skip (c, info); }

  എണ്ണൽ match_t {
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
    എങ്കിൽ (unlikely (skip == matcher_t::SKIP_YES))
      തിരികെ SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    എങ്കിൽ (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      തിരികെ MATCH;

    എങ്കിൽ (skip == matcher_t::SKIP_NO)
      തിരികെ NOT_MATCH;

    തിരികെ SKIP;
  }

  തർക്കം next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    സ്ഥിരം signed stop = (signed) end - 1;
    ആകുമ്പോൾ ((signed) idx < stop)
    {
      idx++;
      തിരഞ്ഞെടുപ്പ് (match (info[idx]))
      {
	അവസ്ഥ MATCH:
	{
	  advance_glyph_data ();
	  തിരികെ സത്യം;
	}
	അവസ്ഥ NOT_MATCH:
	{
	  എങ്കിൽ (unsafe_to)
	    *unsafe_to = idx + 1;
	  തിരികെ അസത്യം;
	}
	അവസ്ഥ SKIP:
	  തുടരുക;
      }
    }
    എങ്കിൽ (unsafe_to)
      *unsafe_to = end;
    തിരികെ അസത്യം;
  }
  തർക്കം prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    സ്ഥിരം unsigned stop = 0;
    ആകുമ്പോൾ (idx > stop)
    {
      idx--;
      തിരഞ്ഞെടുപ്പ് (match (out_info[idx]))
      {
	അവസ്ഥ MATCH:
	{
	  advance_glyph_data ();
	  തിരികെ സത്യം;
	}
	അവസ്ഥ NOT_MATCH:
	{
	  എങ്കിൽ (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  തിരികെ അസത്യം;
	}
	അവസ്ഥ SKIP:
	  തുടരുക;
      }
    }
    എങ്കിൽ (unsafe_from)
      *unsafe_from = 0;
    തിരികെ അസത്യം;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    എങ്കിൽ (match_glyph_data16) തിരികെ *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    അല്ലെങ്കിൽ
    എങ്കിൽ (match_glyph_data24) തിരികെ *match_glyph_data24;
#endif
    തിരികെ 0;
  }
  HB_ALWAYS_INLINE
  ശൂന്യം
  advance_glyph_data ()
  {
    എങ്കിൽ (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    അല്ലെങ്കിൽ
    എങ്കിൽ (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned പൂർണ്ണസംഖ്യ idx;
  സംരക്ഷിത:
  context_t *c;
  matcher_t matcher;
  സ്ഥിരം HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  സ്ഥിരം HBUINT24 *match_glyph_data24;
#endif

  unsigned പൂർണ്ണസംഖ്യ end;
};

ഘടന hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, തർക്കം, HB_DEBUG_APPLY>
{
  സ്ഥിരം അക്ഷരം *get_name () { തിരികെ "APPLY"; }
  തരനിർവചനം return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_index);

  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_ (സ്ഥിരം T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_ (സ്ഥിരം T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { തിരികെ apply_(obj, ഇത്, hb_prioritize); }

  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }
  return_t recurse (unsigned പൂർണ്ണസംഖ്യ sub_lookup_index)
  {
    പ്രതിജ്ഞാ (recurse_func);
    എങ്കിൽ (unlikely (nesting_level_ഇടത് == 0))
    {
      buffer->successful = അസത്യം;
      തിരികെ default_return_മൂല്യം ();
    }

    buffer->max_ops--;
    എങ്കിൽ (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = അസത്യം;
      തിരികെ default_return_മൂല്യം ();
    }

    nesting_level_ഇടത്--;
    തർക്കം ret = recurse_func (ഇത്, sub_lookup_index);
    nesting_level_ഇടത്++;
    തിരികെ ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_നിവേശം, iter_context;

  unsigned പൂർണ്ണസംഖ്യ table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  സ്ഥിരം GDEF &gdef;
  സ്ഥിരം GDEF::accelerator_t &gdef_accel;
  സ്ഥിരം ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned പൂർണ്ണസംഖ്യ lookup_index = (unsigned) -1;
  unsigned പൂർണ്ണസംഖ്യ lookup_props = 0;
  unsigned പൂർണ്ണസംഖ്യ nesting_level_ഇടത് = HB_MAX_NESTING_LEVEL;

  തർക്കം has_glyph_classes;
  തർക്കം auto_zwnj = സത്യം;
  തർക്കം auto_zwj = സത്യം;
  തർക്കം per_syllable = അസത്യം;
  തർക്കം random = അസത്യം;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t അട്ടി_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  ശൂന്യം *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned പൂർണ്ണസംഖ്യ table_index_,
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
#അല്ലെങ്കിൽ
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#അല്ലെങ്കിൽ
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (അട്ടി_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    എങ്കിൽ (buffer->messaging ())
    {
      പ്രതിജ്ഞാ (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = ഇത്;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    എങ്കിൽ (buffer->messaging ())
    {
      പ്രതിജ്ഞാ (buffer->changed_func == buffer_changed_trampoline);
      പ്രതിജ്ഞാ (buffer->changed_data == ഇത്);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  ശൂന്യം init_iters ()
  {
    iter_നിവേശം.init (ഇത്, അസത്യം);
    iter_context.init (ഇത്, സത്യം);
  }

  ശൂന്യം set_lookup_mask (hb_mask_t mask, തർക്കം init = സത്യം) { lookup_mask = mask; last_base = -1; last_base_until = 0; എങ്കിൽ (init) init_iters (); }
  ശൂന്യം set_auto_zwj (തർക്കം auto_zwj_, തർക്കം init = സത്യം) { auto_zwj = auto_zwj_; എങ്കിൽ (init) init_iters (); }
  ശൂന്യം set_auto_zwnj (തർക്കം auto_zwnj_, തർക്കം init = സത്യം) { auto_zwnj = auto_zwnj_; എങ്കിൽ (init) init_iters (); }
  ശൂന്യം set_per_syllable (തർക്കം per_syllable_, തർക്കം init = സത്യം) { per_syllable = per_syllable_; എങ്കിൽ (init) init_iters (); }
  ശൂന്യം set_random (തർക്കം random_) { random = random_; }
  ശൂന്യം set_recurse_func (recurse_func_t func) { recurse_func = func; }
  ശൂന്യം set_lookup_index (unsigned പൂർണ്ണസംഖ്യ lookup_index_) { lookup_index = lookup_index_; }
  ശൂന്യം set_lookup_props (unsigned പൂർണ്ണസംഖ്യ lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_സംഖ്യ ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    തിരികെ buffer->random_state;
  }

  സ്ഥിതം ശൂന്യം buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, ശൂന്യം *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  ശൂന്യം buffer_changed ()
  {
    buffer->update_digest ();
    എങ്കിൽ (likely (has_glyph_classes))
      വേണ്ടി (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  തർക്കം match_properties_mark (സ്ഥിരം hb_glyph_info_t *info,
			      unsigned പൂർണ്ണസംഖ്യ    glyph_props,
			      unsigned പൂർണ്ണസംഖ്യ    match_props) സ്ഥിരം
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    എങ്കിൽ (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      തിരികെ gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    എങ്കിൽ (match_props & LookupFlag::MarkAttachmentType)
      തിരികെ (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    തിരികെ സത്യം;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം check_glyph_property (സ്ഥിരം hb_glyph_info_t *info,
			     unsigned match_props) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    എങ്കിൽ (glyph_props & match_props & LookupFlag::IgnoreFlags)
      തിരികെ അസത്യം;

    എങ്കിൽ (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      തിരികെ match_properties_mark (info, glyph_props, match_props);

    തിരികെ സത്യം;
  }

  ശൂന്യം _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned പൂർണ്ണസംഖ്യ class_guess = 0,
			  തർക്കം ligature = അസത്യം,
			  തർക്കം component = അസത്യം)
  {
    buffer->digest.add (glyph_index);

    എങ്കിൽ (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned പൂർണ്ണസംഖ്യ props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    എങ്കിൽ (ligature)
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
    എങ്കിൽ (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    എങ്കിൽ (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    അല്ലെങ്കിൽ
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  ശൂന്യം _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned പൂർണ്ണസംഖ്യ props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  ശൂന്യം _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  ശൂന്യം replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (ശൂന്യം) buffer->replace_glyph (glyph_index);
  }
  ശൂന്യം replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  ശൂന്യം replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned പൂർണ്ണസംഖ്യ class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, സത്യം);
    (ശൂന്യം) buffer->replace_glyph (glyph_index);
  }
  ശൂന്യം output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned പൂർണ്ണസംഖ്യ class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, അസത്യം, സത്യം);
    (ശൂന്യം) buffer->output_glyph (glyph_index);
  }
};

എണ്ണൽ വർഗ്ഗം hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

ഘടന hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  സ്ഥിതം വരിയിൽ auto collect_second_glyphs_ (സ്ഥിരം T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (ശൂന്യം, obj.collect_second_glyphs (digest) )
  template <typename T>
  സ്ഥിതം വരിയിൽ ശൂന്യം collect_second_glyphs_ (സ്ഥിരം T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_ (സ്ഥിരം T *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache, hb_priority<1>) HB_RETURN (തർക്കം, obj->apply (c, external_cache) )
  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_ (സ്ഥിരം T *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache, hb_priority<0>) HB_RETURN (തർക്കം, obj->apply (c) )
  template <typename T>
  സ്ഥിതം വരിയിൽ തർക്കം apply_to (സ്ഥിരം ശൂന്യം *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache)
  {
    സ്ഥിരം T *typed_obj = (സ്ഥിരം T *) obj;
    തിരികെ apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_cached_ (സ്ഥിരം T *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache, hb_priority<2>) HB_RETURN (തർക്കം, obj->apply_cached (c, external_cache) )
  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_cached_ (സ്ഥിരം T *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache, hb_priority<1>) HB_RETURN (തർക്കം, obj->apply (c, external_cache) )
  template <typename T>
  സ്ഥിതം വരിയിൽ auto apply_cached_ (സ്ഥിരം T *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache, hb_priority<0>) HB_RETURN (തർക്കം, obj->apply (c) )
  template <typename T>
  സ്ഥിതം വരിയിൽ തർക്കം apply_cached_to (സ്ഥിരം ശൂന്യം *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache)
  {
    സ്ഥിരം T *typed_obj = (സ്ഥിരം T *) obj;
    തിരികെ apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  സ്ഥിതം വരിയിൽ auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (തർക്കം, T::cache_func (c, op) )
  template <typename T=ശൂന്യം>
  സ്ഥിതം വരിയിൽ തർക്കം cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { തിരികെ അസത്യം; }
  template <typename Type>
  സ്ഥിതം വരിയിൽ തർക്കം cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    തിരികെ cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  തരനിർവചനം തർക്കം (*hb_apply_func_t) (സ്ഥിരം ശൂന്യം *obj, hb_ot_apply_context_t *c, ശൂന്യം *external_cache);
  തരനിർവചനം തർക്കം (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  ഘടന hb_applicable_t
  {
    friend ഘടന hb_accelerate_subtables_context_t;
    friend ഘടന hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    ശൂന്യം init (സ്ഥിരം T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , ശൂന്യം *external_cache_
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
    തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    തർക്കം apply_no_digest (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ apply_func (obj, c, nullptr);
    }
#അല്ലെങ്കിൽ
    തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    തർക്കം apply_cached (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    തർക്കം apply_no_digest (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ apply_func (obj, c, external_cache);
    }
    തർക്കം apply_cached_no_digest (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ apply_cached_func (obj, c, external_cache);
    }

    തർക്കം cache_enter (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    ശൂന്യം cache_leave (hb_ot_apply_context_t *c) സ്ഥിരം
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    സ്വകാര്യ:
    സ്ഥിരം ശൂന്യം *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    ശൂന്യം *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (സ്ഥിരം T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (സ്ഥിരം T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (സ്ഥിരം T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (സ്ഥിരം T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ശൂന്യം *external_cache = nullptr;
    എങ്കിൽ (i < 8)
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
    എങ്കിൽ (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    തിരികെ hb_empty_t ();
  }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }

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


തരനിർവചനം തർക്കം (*intersects_func_t) (സ്ഥിരം hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data, ശൂന്യം *cache);
തരനിർവചനം ശൂന്യം (*intersected_glyphs_func_t) (സ്ഥിരം hb_set_t *glyphs, സ്ഥിരം ശൂന്യം *data, unsigned value, hb_set_t *intersected_glyphs, ശൂന്യം *cache);
തരനിർവചനം ശൂന്യം (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data);
തരനിർവചനം തർക്കം (*match_func_t) (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data);
തരനിർവചനം unsigned (*get_മൂല്യം_func_t) (hb_glyph_info_t &info, സ്ഥിരം ശൂന്യം *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
ഘടന hb_ot_layout_ruleset_digest_t
{
  ശൂന്യം init () { mask = 0; }
  ശൂന്യം set_full () { mask = (uint64_t) -1; }
  ശൂന്യം add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  തർക്കം may_have (unsigned value) സ്ഥിരം { തിരികെ mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

ഘടന ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
ഘടന ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
ഘടന ContextApplyFuncs
{
  match_func_t match;
};
ഘടന ChainContextApplyFuncs
{
  match_func_t match[3];
};


സ്ഥിതം വരിയിൽ തർക്കം intersects_glyph (സ്ഥിരം hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data HB_UNUSED, ശൂന്യം *cache HB_UNUSED)
{
  തിരികെ glyphs->has (value);
}
സ്ഥിതം വരിയിൽ തർക്കം intersects_class (സ്ഥിരം hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data, ശൂന്യം *cache)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  എങ്കിൽ (map->has (value, &cached_v))
    തിരികെ *cached_v;

  തർക്കം v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  തിരികെ v;
}
സ്ഥിതം വരിയിൽ തർക്കം intersects_coverage (സ്ഥിരം hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data, ശൂന്യം *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  തിരികെ (data+coverage).intersects (glyphs);
}


സ്ഥിതം വരിയിൽ ശൂന്യം intersected_glyph (സ്ഥിരം hb_set_t *glyphs HB_UNUSED, സ്ഥിരം ശൂന്യം *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED ശൂന്യം *cache)
{
  unsigned g = reinterpret_cast<സ്ഥിരം HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

സ്ഥിതം വരിയിൽ ശൂന്യം intersected_class_glyphs (സ്ഥിരം hb_set_t *glyphs, സ്ഥിരം ശൂന്യം *data, unsigned value, hb_set_t *intersected_glyphs, ശൂന്യം *cache)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  എങ്കിൽ (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    തിരികെ;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

സ്ഥിതം വരിയിൽ ശൂന്യം intersected_coverage_glyphs (സ്ഥിരം hb_set_t *glyphs, സ്ഥിരം ശൂന്യം *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED ശൂന്യം *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


സ്ഥിതം വരിയിൽ തർക്കം array_is_subset_of (സ്ഥിരം hb_set_t *glyphs,
				       unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       സ്ഥിരം ശൂന്യം *intersects_data,
				       ശൂന്യം *cache)
{
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    എങ്കിൽ (!intersects_func (glyphs, values[i], intersects_data, cache)) തിരികെ അസത്യം;
  തിരികെ സത്യം;
}


സ്ഥിതം വരിയിൽ ശൂന്യം collect_glyph (hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data HB_UNUSED)
{
  glyphs->add (value);
}
സ്ഥിതം വരിയിൽ ശൂന്യം collect_class (hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
സ്ഥിതം വരിയിൽ ശൂന്യം collect_coverage (hb_set_t *glyphs, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

വരിയിൽ സ്ഥിരം hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (സ്ഥിരം ശൂന്യം *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_താക്കോൽ_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  എങ്കിൽ (glyph_set_to_index.has (key, &existing))
    തിരികെ glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  എങ്കിൽ (unlikely (!set))
  {
    depend_data->fail ();
    തിരികെ nullptr;
  }

  എങ്കിൽ (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  അല്ലെങ്കിൽ എങ്കിൽ (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  അല്ലെങ്കിൽ
    set->add (value);
  എങ്കിൽ (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    തിരികെ nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  എങ്കിൽ (unlikely (!glyph_sets.തള്ളുക_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    തിരികെ nullptr;
  }
  എങ്കിൽ (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    തിരികെ nullptr;
  }
  തിരികെ glyph_sets[index].get ();
}

വരിയിൽ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (സ്ഥിരം hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  എങ്കിൽ (cached_context_set_indices.has (key, &existing))
    തിരികെ *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  എങ്കിൽ (unlikely (index == HB_CODEPOINT_INVALID))
    തിരികെ HB_CODEPOINT_INVALID;
  എങ്കിൽ (unlikely (!cached_context_set_indices.set (key, index)))
    തിരികെ depend_data->fail_invalid ();
  തിരികെ index;
}

സ്ഥിതം വരിയിൽ ശൂന്യം collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  സ്ഥിരം ശൂന്യം *collect_data)
{
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    collect_func (glyphs, values[i], collect_data);
}


സ്ഥിതം വരിയിൽ തർക്കം match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, സ്ഥിരം ശൂന്യം *data HB_UNUSED)
{
  തിരികെ സത്യം;
}
സ്ഥിതം വരിയിൽ തർക്കം match_glyph (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data HB_UNUSED)
{
  തിരികെ info.codepoint == value;
}
സ്ഥിതം വരിയിൽ തർക്കം match_class (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);
  തിരികെ class_def.get_class (info.codepoint) == value;
}
സ്ഥിതം വരിയിൽ unsigned get_class_മൂല്യം (hb_glyph_info_t &info, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *> (data);
  തിരികെ class_def.get_class (info.codepoint);
}
ഘടന match_class_cache_data_t
{
  സ്ഥിരം ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
സ്ഥിതം വരിയിൽ തർക്കം match_class_cache (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം auto &cache_data = *reinterpret_cast<സ്ഥിരം match_class_cache_data_t *> (data);
  തിരികെ cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
സ്ഥിതം വരിയിൽ unsigned get_class_cache_മൂല്യം (hb_glyph_info_t &info, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം auto &cache_data = *reinterpret_cast<സ്ഥിരം match_class_cache_data_t *> (data);
  തിരികെ cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
സ്ഥിതം വരിയിൽ unsigned get_class_cached (സ്ഥിരം ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  എങ്കിൽ (klass < 255)
    തിരികെ klass;
  klass = class_def.get_class (info.codepoint);
  എങ്കിൽ (likely (klass < 255))
    info.syllable() = klass;
  തിരികെ klass;
}
സ്ഥിതം വരിയിൽ തർക്കം match_class_cached (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);
  തിരികെ get_class_cached (class_def, info) == value;
}
സ്ഥിതം വരിയിൽ unsigned get_class_cached_മൂല്യം (hb_glyph_info_t &info, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *> (data);
  തിരികെ get_class_cached (class_def, info);
}
സ്ഥിതം വരിയിൽ unsigned get_class_cached1 (സ്ഥിരം ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  എങ്കിൽ (klass < 15)
    തിരികെ klass;
  klass = class_def.get_class (info.codepoint);
  എങ്കിൽ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  തിരികെ klass;
}
സ്ഥിതം വരിയിൽ തർക്കം match_class_cached1 (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);
  തിരികെ get_class_cached1 (class_def, info) == value;
}
സ്ഥിതം വരിയിൽ unsigned get_class_cached2 (സ്ഥിരം ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  എങ്കിൽ (klass < 15)
    തിരികെ klass;
  klass = class_def.get_class (info.codepoint);
  എങ്കിൽ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  തിരികെ klass;
}
സ്ഥിതം വരിയിൽ തർക്കം match_class_cached2 (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *>(data);
  തിരികെ get_class_cached2 (class_def, info) == value;
}
സ്ഥിതം വരിയിൽ unsigned get_class_cached2_മൂല്യം (hb_glyph_info_t &info, സ്ഥിരം ശൂന്യം *data)
{
  സ്ഥിരം ClassDef &class_def = *reinterpret_cast<സ്ഥിരം ClassDef *> (data);
  തിരികെ get_class_cached2 (class_def, info);
}
സ്ഥിതം വരിയിൽ തർക്കം match_coverage (hb_glyph_info_t &info, unsigned value, സ്ഥിരം ശൂന്യം *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  തിരികെ (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

സ്ഥിതം വരിയിൽ തർക്കം would_match_നിവേശം (hb_would_apply_context_t *c,
				      unsigned പൂർണ്ണസംഖ്യ എണ്ണം, /* Including the first glyph (not matched) */
				      glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      സ്ഥിരം ശൂന്യം *match_data)
{
  എങ്കിൽ (എണ്ണം != c->len)
    തിരികെ അസത്യം;

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    എങ്കിൽ (likely (!match_func (info, നിവേശം[i - 1], match_data)))
      തിരികെ അസത്യം;
  }

  തിരികെ സത്യം;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#അല്ലെങ്കിൽ
വരിയിൽ
#endif
സ്ഥിതം തർക്കം match_നിവേശം (hb_ot_apply_context_t *c,
			 unsigned പൂർണ്ണസംഖ്യ എണ്ണം, /* Including the first glyph (not matched) */
			 glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 സ്ഥിരം ശൂന്യം *match_data,
			 unsigned പൂർണ്ണസംഖ്യ *end_position,
			 unsigned പൂർണ്ണസംഖ്യ *p_total_component_എണ്ണം = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  എങ്കിൽ (എണ്ണം == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    എങ്കിൽ (p_total_component_എണ്ണം)
      *p_total_component_എണ്ണം = hb_glyph_info_get_lig_അക്കം_comps (&buffer->cur());
    return_trace (സത്യം);
  }

  എങ്കിൽ (unlikely (എണ്ണം > HB_MAX_CONTEXT_LENGTH)) return_trace (അസത്യം);

  auto &skippy_iter = c->iter_നിവേശം;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (നിവേശം);

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

  unsigned പൂർണ്ണസംഖ്യ total_component_എണ്ണം = 0;

  unsigned പൂർണ്ണസംഖ്യ first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned പൂർണ്ണസംഖ്യ first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  എണ്ണൽ {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
  {
    unsigned unsafe_to;
    എങ്കിൽ (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (അസത്യം);
    }

    എങ്കിൽ (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (അസത്യം);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned പൂർണ്ണസംഖ്യ this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned പൂർണ്ണസംഖ്യ this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    എങ്കിൽ (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      എങ്കിൽ (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	എങ്കിൽ (ligbase == LIGBASE_NOT_CHECKED)
	{
	  തർക്കം found = അസത്യം;
	  സ്ഥിരം auto *out = buffer->out_info;
	  unsigned പൂർണ്ണസംഖ്യ j = buffer->out_len;
	  ആകുമ്പോൾ (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    എങ്കിൽ (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = സത്യം;
	      നിർത്തുക;
	    }
	    j--;
	  }

	  എങ്കിൽ (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  അല്ലെങ്കിൽ
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	എങ്കിൽ (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (അസത്യം);
      }
    }
    അല്ലെങ്കിൽ
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      എങ്കിൽ (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (അസത്യം);
    }

    total_component_എണ്ണം += hb_glyph_info_get_lig_അക്കം_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  എങ്കിൽ (p_total_component_എണ്ണം)
  {
    total_component_എണ്ണം += hb_glyph_info_get_lig_അക്കം_comps (&buffer->cur());
    *p_total_component_എണ്ണം = total_component_എണ്ണം;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (സത്യം);
}
സ്ഥിതം വരിയിൽ തർക്കം ligate_നിവേശം (hb_ot_apply_context_t *c,
				 unsigned പൂർണ്ണസംഖ്യ എണ്ണം, /* Including the first glyph */
				 unsigned പൂർണ്ണസംഖ്യ match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned പൂർണ്ണസംഖ്യ total_component_എണ്ണം)
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

  തർക്കം is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  തർക്കം is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
    എങ്കിൽ (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = അസത്യം;
      is_mark_ligature = അസത്യം;
      നിർത്തുക;
    }
  തർക്കം is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned പൂർണ്ണസംഖ്യ klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned പൂർണ്ണസംഖ്യ lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned പൂർണ്ണസംഖ്യ last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned പൂർണ്ണസംഖ്യ last_അക്കം_components = hb_glyph_info_get_lig_അക്കം_comps (&buffer->cur());
  unsigned പൂർണ്ണസംഖ്യ components_so_far = last_അക്കം_components;

  എങ്കിൽ (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_എണ്ണം);
    എങ്കിൽ (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
  {
    ആകുമ്പോൾ (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      എങ്കിൽ (is_ligature)
      {
	unsigned പൂർണ്ണസംഖ്യ this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	എങ്കിൽ (this_comp == 0)
	  this_comp = last_അക്കം_components;
	പ്രതിജ്ഞാ (components_so_far >= last_അക്കം_components);
	unsigned പൂർണ്ണസംഖ്യ new_lig_comp = components_so_far - last_അക്കം_components +
				    hb_min (this_comp, last_അക്കം_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (ശൂന്യം) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_അക്കം_components = hb_glyph_info_get_lig_അക്കം_comps_in_ligation (&buffer->cur());
    components_so_far += last_അക്കം_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  എങ്കിൽ (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    വേണ്ടി (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      എങ്കിൽ (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) നിർത്തുക;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      എങ്കിൽ (!this_comp) നിർത്തുക;

      പ്രതിജ്ഞാ (components_so_far >= last_അക്കം_components);
      unsigned new_lig_comp = components_so_far - last_അക്കം_components +
			      hb_min (this_comp, last_അക്കം_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (സത്യം);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#അല്ലെങ്കിൽ
വരിയിൽ
#endif
സ്ഥിതം തർക്കം match_backtrack (hb_ot_apply_context_t *c,
			     unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     സ്ഥിരം ശൂന്യം *match_data,
			     unsigned പൂർണ്ണസംഖ്യ *match_start)
{
  TRACE_APPLY (nullptr);

  എങ്കിൽ (!എണ്ണം)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (സത്യം);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    unsigned unsafe_from;
    എങ്കിൽ (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (അസത്യം);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (സത്യം);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#അല്ലെങ്കിൽ
വരിയിൽ
#endif
സ്ഥിതം തർക്കം match_lookahead (hb_ot_apply_context_t *c,
			     unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     സ്ഥിരം ശൂന്യം *match_data,
			     unsigned പൂർണ്ണസംഖ്യ start_index,
			     unsigned പൂർണ്ണസംഖ്യ *end_index)
{
  TRACE_APPLY (nullptr);

  എങ്കിൽ (!എണ്ണം)
  {
    *end_index = start_index;
    return_trace (സത്യം);
  }

  auto &skippy_iter = c->iter_context;
  പ്രതിജ്ഞാ (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    unsigned unsafe_to;
    എങ്കിൽ (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (അസത്യം);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (സത്യം);
}



ഘടന LookupRecord
{
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം hb_map_t         *lookup_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

സ്ഥിതം വരിയിൽ unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     സ്ഥിരം hb_array_t<സ്ഥിരം LookupRecord> lookupRecords,
						     സ്ഥിരം hb_map_t *lookup_map)
{
  unsigned എണ്ണം = 0;
  വേണ്ടി (സ്ഥിരം LookupRecord& r : lookupRecords)
  {
    എങ്കിൽ (!lookup_map->has (r.lookupListIndex))
      തുടരുക;

    എങ്കിൽ (!r.serialize (c, lookup_map))
      തിരികെ 0;

    എണ്ണം++;
  }
  തിരികെ എണ്ണം;
}

എണ്ണൽ ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

സ്ഥിതം വരിയിൽ സ്ഥിരം hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			സ്ഥിരം ശൂന്യം *data,
			unsigned value,
			തർക്കം filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  തിരഞ്ഞെടുപ്പ് (context_format)
  {
    അവസ്ഥ ContextFormat::SimpleContext:
      തിരികെ c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    അവസ്ഥ ContextFormat::ClassBasedContext:
    {
      സ്ഥിരം hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      എങ്കിൽ (unlikely (!class_set))
	തിരികെ nullptr;
      എങ്കിൽ (!filter_class_by_parent)
	തിരികെ class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      നിർത്തുക;
    }
    അവസ്ഥ ContextFormat::CoverageBasedContext:
      തിരികെ c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  എങ്കിൽ (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    തിരികെ nullptr;
  }
  തിരികെ scratch;
}

സ്ഥിതം വരിയിൽ ശൂന്യം context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned നിവേശം_എണ്ണം, glyph_data_t നിവേശം,
					     unsigned lookup_എണ്ണം_2,
					     സ്ഥിരം LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     സ്ഥിരം ശൂന്യം *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ശൂന്യം *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < lookup_എണ്ണം_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    എങ്കിൽ (seqIndex >= നിവേശം_എണ്ണം) തുടരുക;

    തർക്കം has_pos_glyphs = അസത്യം;

    എങ്കിൽ (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = സത്യം;
      pos_glyphs.clear ();
      എങ്കിൽ (seqIndex == 0)
      {
        തിരഞ്ഞെടുപ്പ് (context_format) {
        അവസ്ഥ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          നിർത്തുക;
        അവസ്ഥ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          നിർത്തുക;
        അവസ്ഥ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          നിർത്തുക;
        }
      }
      അല്ലെങ്കിൽ
      {
        സ്ഥിരം ശൂന്യം *നിവേശം_data = നിവേശം.data;
        unsigned നിവേശം_മൂല്യം = seqIndex - 1;
        എങ്കിൽ (context_format != ContextFormat::SimpleContext)
        {
          നിവേശം_data = data;
          നിവേശം_മൂല്യം = നിവേശം[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, നിവേശം_data, നിവേശം_മൂല്യം, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs))
      തിരികെ;
    എങ്കിൽ (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } അല്ലെങ്കിൽ {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = നിവേശം_എണ്ണം;
    എങ്കിൽ (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->എടുക്കുക_cur_done_glyphs ();
  }
}

സ്ഥിതം തർക്കം
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned നിവേശം_എണ്ണം_2,
				unsigned lookup_എണ്ണം,
				സ്ഥിരം LookupRecord lookup_records[],
				സ്ഥിരം ശൂന്യം *rule,
				unsigned value,
				ContextFormat context_format,
				സ്ഥിരം ശൂന്യം *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  തർക്കം have_active_idx = അസത്യം;
  വേണ്ടി (unsigned i = 0; i < lookup_എണ്ണം; i++)
  {
    എങ്കിൽ (lookup_records[i].sequenceIndex >= നിവേശം_എണ്ണം_2)
      തുടരുക;

    എങ്കിൽ (!have_active_idx)
    {
      എങ്കിൽ (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	തിരികെ അസത്യം;
      have_active_idx = സത്യം;
    }

    hb_depend_context_t::context_set_cache_താക്കോൽ_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    എങ്കിൽ (!c->context_set_cache.has (key, &cached_context_set))
      തിരികെ അസത്യം;
    എങ്കിൽ (unlikely (!cached_context_sets->തള്ളുക_or_fail (*cached_context_set)))
      തിരികെ c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  തിരികെ have_active_idx;
}

സ്ഥിതം വരിയിൽ ശൂന്യം context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned നിവേശം_എണ്ണം, glyph_data_t നിവേശം,
					     unsigned lookup_എണ്ണം_2,
					     സ്ഥിരം LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     സ്ഥിരം ശൂന്യം *rule,
					     unsigned value,
					     ContextFormat context_format,
					     സ്ഥിരം ശൂന്യം *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ശൂന്യം *cache,
					     സ്ഥിരം hb_set_t &preliminary_context,
					     സ്ഥിരം hb_vector_t<സ്ഥിരം hb_set_t *> *നിവേശം_position_glyphs,
					     സ്ഥിരം hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  എങ്കിൽ (unlikely (നിവേശം_എണ്ണം > HB_MAX_CONTEXT_LENGTH))
    തിരികെ;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < lookup_എണ്ണം_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    എങ്കിൽ (seqIndex >= നിവേശം_എണ്ണം)
      തുടരുക;

    hb_depend_context_t::context_set_cache_താക്കോൽ_t context_cache_താക്കോൽ;
    എങ്കിൽ (unlikely (!c->get_context_set_cache_താക്കോൽ (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_താക്കോൽ)))
      തിരികെ;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    എങ്കിൽ (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    അല്ലെങ്കിൽ എങ്കിൽ (c->context_set_cache.has (context_cache_താക്കോൽ, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (!നിവേശം_position_glyphs))
      {
	c->depend_data->fail ();
	തിരികെ;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_മൂല്യം_INVALID;
      ആകുമ്പോൾ (preliminary_context.next (&elem)) {
	എങ്കിൽ (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} അല്ലെങ്കിൽ {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      വേണ്ടി (unsigned j = 0; j < നിവേശം_position_glyphs->length; j++) {
	എങ്കിൽ (j != seqIndex && (*നിവേശം_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*നിവേശം_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_മൂല്യം_INVALID;
      ആകുമ്പോൾ (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	സ്ഥിരം hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	എങ്കിൽ (unlikely (!original_set)) തുടരുക;

	/* Dependency context sets are never inverted. */
	എങ്കിൽ (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      വേണ്ടി (unsigned j = 0; j < നിവേശം_position_glyphs->length; j++) {
	എങ്കിൽ (j != seqIndex && (*നിവേശം_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  എങ്കിൽ (!(*നിവേശം_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    തർക്കം cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*നിവേശം_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*നിവേശം_position_glyphs)[j]);
	    എങ്കിൽ (unlikely (idx == HB_CODEPOINT_INVALID))
	      തിരികെ;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      എങ്കിൽ (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	തിരികെ;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      എങ്കിൽ (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	തിരികെ;

      എങ്കിൽ (unlikely (!c->context_set_cache.set (context_cache_താക്കോൽ, context_set_idx)))
      {
	c->depend_data->fail ();
	തിരികെ;
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
    തർക്കം nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    എങ്കിൽ (നിവേശം_എണ്ണം > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    എങ്കിൽ (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    തർക്കം has_pos_glyphs = അസത്യം;

    എങ്കിൽ (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = സത്യം;
      pos_glyphs.clear ();
      എങ്കിൽ (seqIndex == 0)
      {
        തിരഞ്ഞെടുപ്പ് (context_format) {
        അവസ്ഥ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          നിർത്തുക;
        അവസ്ഥ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          നിർത്തുക;
        അവസ്ഥ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          നിർത്തുക;
        }
      }
      അല്ലെങ്കിൽ
      {
        സ്ഥിരം ശൂന്യം *നിവേശം_data = നിവേശം.data;
        unsigned നിവേശം_മൂല്യം = seqIndex - 1;
        എങ്കിൽ (context_format != ContextFormat::SimpleContext)
        {
          നിവേശം_data = data;
          നിവേശം_മൂല്യം = നിവേശം[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, നിവേശം_data, നിവേശം_മൂല്യം, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    എങ്കിൽ (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      എങ്കിൽ (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	എങ്കിൽ (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->തള്ളുക_cur_active_glyphs (active_idx);
      എങ്കിൽ (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	തിരികെ;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!c->തള്ളുക_വേര്_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      തിരികെ;
    }

    unsigned endIndex = നിവേശം_എണ്ണം;
    എങ്കിൽ (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->എടുക്കുക_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
സ്ഥിതം വരിയിൽ ശൂന്യം recurse_lookups (context_t *c,
                                    unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
                                    സ്ഥിരം LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < lookup_എണ്ണം_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

സ്ഥിതം വരിയിൽ ശൂന്യം apply_lookup (hb_ot_apply_context_t *c,
				 unsigned പൂർണ്ണസംഖ്യ എണ്ണം, /* Including the first glyph */
				 unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
				 സ്ഥിരം LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned പൂർണ്ണസംഖ്യ match_end)
{
  hb_buffer_t *buffer = c->buffer;
  പൂർണ്ണസംഖ്യ end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned പൂർണ്ണസംഖ്യ bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    പൂർണ്ണസംഖ്യ delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < എണ്ണം; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < lookup_എണ്ണം_2 && buffer->successful; i++)
  {
    unsigned പൂർണ്ണസംഖ്യ idx = lookupRecord[i].sequenceIndex;
    എങ്കിൽ (idx >= എണ്ണം)
      തുടരുക;

    unsigned പൂർണ്ണസംഖ്യ orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    എങ്കിൽ (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      തുടരുക;

    എങ്കിൽ (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      നിർത്തുക;

    എങ്കിൽ (unlikely (buffer->max_ops <= 0))
      നിർത്തുക;

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      എങ്കിൽ (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    തർക്കം ret = c->recurse (lookupRecord[i].lookupListIndex);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      എങ്കിൽ (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    എങ്കിൽ (!ret)
      തുടരുക;

    unsigned പൂർണ്ണസംഖ്യ new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    പൂർണ്ണസംഖ്യ delta = new_len - orig_len;

    എങ്കിൽ (!delta)
      തുടരുക;

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
    എങ്കിൽ (end < പൂർണ്ണസംഖ്യ (c->match_positions.arrayZ[idx]))
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

    unsigned പൂർണ്ണസംഖ്യ next = idx + 1; /* next now is the position after the recursed lookup. */

    എങ്കിൽ (delta > 0)
    {
      എങ്കിൽ (unlikely (delta + എണ്ണം > HB_MAX_CONTEXT_LENGTH))
	നിർത്തുക;
      എങ്കിൽ (unlikely (എണ്ണം + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (എണ്ണം + delta)))
        തിരികെ;
    }
    അല്ലെങ്കിൽ
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (പൂർണ്ണസംഖ്യ) next - (പൂർണ്ണസംഖ്യ) എണ്ണം);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (എണ്ണം - next) * വലുപ്പം (c->match_positions.arrayZ[0]));
    next += delta;
    എണ്ണം += delta;

    /* Fill in new entries. */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    വേണ്ടി (; next < എണ്ണം; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  പ്രതിജ്ഞാ (end >= 0);
  (ശൂന്യം) buffer->move_to (end);
}



/* Contextual lookups */

ഘടന ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  സ്ഥിരം ശൂന്യം *intersects_data;
  ശൂന്യം *intersects_cache;
  ശൂന്യം *intersected_glyphs_cache;
};

തരനിർവചനം ContextClosureLookupContext ContextDependLookupContext;

ഘടന ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  സ്ഥിരം ശൂന്യം *collect_data;
};

ഘടന ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  സ്ഥിരം ശൂന്യം *match_data;
  get_മൂല്യം_func_t get_മൂല്യം;
};

സ്ഥിതം വരിയിൽ തർക്കം context_intersects (സ്ഥിരം hb_set_t *glyphs,
				       unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
				       glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  തിരികെ array_is_subset_of (glyphs,
			     നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0, നിവേശം,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

സ്ഥിതം വരിയിൽ ശൂന്യം context_closure_lookup (hb_closure_context_t *c,
					   unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
					   glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
					   unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
					   സ്ഥിരം LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  എങ്കിൽ (context_intersects (c->glyphs,
			  നിവേശം_എണ്ണം, നിവേശം,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     നിവേശം_എണ്ണം, നിവേശം,
				     lookup_എണ്ണം_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

സ്ഥിതം വരിയിൽ ശൂന്യം context_depend_lookup (hb_depend_context_t *c,
					  unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
					  glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
					  unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
					  സ്ഥിരം LookupRecord lookupRecord[],
					  സ്ഥിരം ശൂന്യം *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  എങ്കിൽ (!context_intersects (c->glyphs,
			   നിവേശം_എണ്ണം, നിവേശം,
			   lookup_context))
    തിരികെ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  എങ്കിൽ (unlikely (!scratch))
    തിരികെ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  തർക്കം context_sets_cached = context_depend_sets_are_cached (
    c, നിവേശം_എണ്ണം, lookup_എണ്ണം_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  എങ്കിൽ (unlikely (!c->depend_data->successful))
    തിരികെ;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<സ്ഥിരം hb_set_t *> &നിവേശം_position_glyphs = scratch->നിവേശം_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  എങ്കിൽ (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    സ്ഥിരം hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      സത്യം, &position_scratch, &scratch->compact_workspace);
    എങ്കിൽ (unlikely (!pos0_glyphs ||
		  !നിവേശം_position_glyphs.തള്ളുക_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      തിരികെ;
    }

    /* Positions 1+ (Input array) */
    വേണ്ടി (unsigned i = 0; i + 1 < നിവേശം_എണ്ണം; i++)
    {
      സ്ഥിരം hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	നിവേശം[i], അസത്യം, &position_scratch);
      എങ്കിൽ (unlikely (!pos_glyphs ||
		    !നിവേശം_position_glyphs.തള്ളുക_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	തിരികെ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  നിവേശം_എണ്ണം, നിവേശം,
				  lookup_എണ്ണം_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &നിവേശം_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

സ്ഥിതം വരിയിൽ ശൂന്യം context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
						  glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
						  unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
						  സ്ഥിരം LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->നിവേശം,
		 നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0, നിവേശം,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_എണ്ണം_2, lookupRecord);
}

സ്ഥിതം വരിയിൽ തർക്കം context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
					       glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
					       unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2 HB_UNUSED,
					       സ്ഥിരം LookupRecord lookupRecord[] HB_UNUSED,
					       സ്ഥിരം ContextApplyLookupContext &lookup_context)
{
  തിരികെ would_match_നിവേശം (c,
			    നിവേശം_എണ്ണം, നിവേശം,
			    lookup_context.funcs.match, lookup_context.match_data);
}

സ്ഥിതം വരിയിൽ തർക്കം context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
					 glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
					 unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
					 സ്ഥിരം LookupRecord lookupRecord[],
					 സ്ഥിരം ContextApplyLookupContext &lookup_context)
{
  എങ്കിൽ (unlikely (നിവേശം_എണ്ണം > HB_MAX_CONTEXT_LENGTH)) തിരികെ അസത്യം;

  unsigned match_end = 0;
  തർക്കം ret = അസത്യം;
  എങ്കിൽ (match_നിവേശം (c,
		   നിവേശം_എണ്ണം, നിവേശം,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  നിവേശം_എണ്ണം,
		  lookup_എണ്ണം_2, lookupRecord,
		  match_end);
    ret = സത്യം;
  }
  അല്ലെങ്കിൽ
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = അസത്യം;
  }

  തിരികെ ret;
}

സ്ഥിതം വരിയിൽ തർക്കം context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  തിരഞ്ഞെടുപ്പ് (op)
  {
    അവസ്ഥ hb_ot_subtable_cache_op_t::ENTER:
    {
      എങ്കിൽ (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	തിരികെ അസത്യം;
      auto &info = c->buffer->info;
      unsigned എണ്ണം = c->buffer->len;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      തിരികെ സത്യം;
    }
    അവസ്ഥ hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      നിർത്തുക;
    }
  }
  തിരികെ അസത്യം;
}

template <typename Types>
ഘടന Rule
{
  template <typename SetTypes, typename RuleType>
  friend ഘടന RuleSetOf;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ context_intersects (glyphs,
			       നിവേശം_എണ്ണം, നിവേശം_Z.arrayZ,
			       lookup_context);
  }

  ശൂന്യം depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array ((നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0)));
    context_depend_lookup (c,
			   നിവേശം_എണ്ണം, നിവേശം_Z.arrayZ,
			   lookup_എണ്ണം_2, lookupRecord.arrayZ,
			   ഇത്,
			   value, lookup_context);
  }

  ശൂന്യം closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;

    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array ((നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0)));
    context_closure_lookup (c,
			    നിവേശം_എണ്ണം, നിവേശം_Z.arrayZ,
			    lookup_എണ്ണം_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;
    എങ്കിൽ (!intersects (c->glyphs, lookup_context)) തിരികെ;

    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array (നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0));
    recurse_lookups (c, lookup_എണ്ണം_2, lookupRecord.arrayZ);
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array (നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0));
    context_collect_glyphs_lookup (c,
				   നിവേശം_എണ്ണം, നിവേശം_Z.arrayZ,
				   lookup_എണ്ണം_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest,
			      സ്ഥിരം ClassDef *class_def = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (നിവേശം_എണ്ണം <= 1)
    {
      *digest = hb_set_digest_t::full ();
      തിരികെ;
    }

    unsigned second = നിവേശം_Z.arrayZ[0];
    എങ്കിൽ (!class_def)
      digest->add (second);
    അല്ലെങ്കിൽ എങ്കിൽ (!second)
      *digest = hb_set_digest_t::full ();
    അല്ലെങ്കിൽ
      class_def->collect_class (digest, second);
  }

  തർക്കം would_apply (hb_would_apply_context_t *c,
		    സ്ഥിരം ContextApplyLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array (നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0));
    തിരികെ context_would_apply_lookup (c,
				       നിവേശം_എണ്ണം, നിവേശം_Z.arrayZ,
				       lookup_എണ്ണം_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  തർക്കം apply (hb_ot_apply_context_t *c,
	      സ്ഥിരം ContextApplyLookupContext &lookup_context) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array (നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0));
    return_trace (context_apply_lookup (c, നിവേശം_എണ്ണം, നിവേശം_Z.arrayZ, lookup_എണ്ണം_2, lookupRecord.arrayZ, lookup_context));
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം hb_map_t *നിവേശം_mapping, /* old->new glyphid or class mapping */
		  സ്ഥിരം hb_map_t *lookup_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (out))) return_trace (അസത്യം);

    out->നിവേശം_എണ്ണം = നിവേശം_എണ്ണം;
    സ്ഥിരം auto നിവേശം = നിവേശം_Z.as_array (നിവേശം_എണ്ണം - 1);
    വേണ്ടി (സ്ഥിരം auto org : നിവേശം)
    {
      typename Types::HBUINT d;
      d = നിവേശം_mapping->get (org);
      c->copy (d);
    }

    സ്ഥിരം auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (നിവേശം_Z.as_array ((നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0)));

    unsigned എണ്ണം = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_എണ്ണം_2), lookup_map);
    return_trace (c->check_assign (out->lookup_എണ്ണം_2, എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം subset (hb_subset_context_t *c,
	       സ്ഥിരം hb_map_t *lookup_map,
	       സ്ഥിരം hb_map_t *klass_map = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (unlikely (!നിവേശം_എണ്ണം)) return_trace (അസത്യം);
    സ്ഥിരം auto നിവേശം = നിവേശം_Z.as_array (നിവേശം_എണ്ണം - 1);

    സ്ഥിരം hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    എങ്കിൽ (!hb_all (നിവേശം, mapping)) return_trace (അസത്യം);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  c->check_range (നിവേശം_Z.arrayZ,
				  നിവേശം_Z.item_size * (നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0) +
				  LookupRecord::static_size * lookup_എണ്ണം_2));
  }

  സംരക്ഷിത:
  HBUINT16	നിവേശം_എണ്ണം;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_എണ്ണം_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		നിവേശം_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, നിവേശം_Z);
};

template <typename Types, typename RuleType>
ഘടന RuleSetOf
{
  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_map ([&] (സ്ഥിരം RuleType &_) { തിരികെ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ശൂന്യം collect_first_നിവേശം_classes (hb_ot_layout_ruleset_digest_t *digest) സ്ഥിരം
  {
    digest->init ();
    വേണ്ടി (unsigned i = 0; i < rule.len; i++)
    {
      സ്ഥിരം RuleType &r = ഇത്+rule.arrayZ[i];
      എങ്കിൽ (r.നിവേശം_എണ്ണം <= 1)
      {
	digest->set_full ();
	തിരികെ;
      }
      digest->add (r.നിവേശം_Z.arrayZ[0]);
    }
  }

  ശൂന്യം depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) സ്ഥിരം
  {
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  ശൂന്യം closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;

    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest,
			      സ്ഥിരം ClassDef *class_def = nullptr) സ്ഥിരം
  {
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([=] (സ്ഥിരം RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c,
		    സ്ഥിരം ContextApplyLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_map ([&] (സ്ഥിരം RuleType &_) { തിരികെ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  തർക്കം apply (hb_ot_apply_context_t *c,
	      സ്ഥിരം ContextApplyLookupContext &lookup_context,
	      സ്ഥിരം hb_ot_layout_ruleset_digest_t *digest = nullptr) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    unsigned അക്കം_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    എങ്കിൽ (HB_OPTIMIZE_SIZE_VAL || അക്കം_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (ഇത്))
      | hb_map ([&] (സ്ഥിരം RuleType &_) { തിരികെ _.apply (c, lookup_context); })
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
    തർക്കം matched = skippy_iter.next (&unsafe_to1);
    എങ്കിൽ (likely (matched))
    {
      എങ്കിൽ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        പോകുക slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      എങ്കിൽ (digest && lookup_context.get_മൂല്യം &&
	  !digest->may_have (lookup_context.get_മൂല്യം (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (അസത്യം);
	}
    }
    അല്ലെങ്കിൽ
    {
      തർക്കം unsafe_to_concat = അസത്യം;
      തർക്കം ret =
      + hb_iter (rule)
      | hb_map (hb_add (ഇത്))
      | hb_filter ([&] (സ്ഥിരം RuleType &_)
		   {
		     എങ്കിൽ (_.നിവേശം_എണ്ണം <= 1) തിരികെ സത്യം;
		     unsafe_to_concat = സത്യം;
		     തിരികെ അസത്യം;
		   })
      | hb_map ([&] (സ്ഥിരം RuleType &_) { തിരികെ _.apply (c, lookup_context); })
      | hb_any
      ;
      എങ്കിൽ (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    എങ്കിൽ (likely (matched))
    {
      എങ്കിൽ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        പോകുക slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_നിവേശം = lookup_context.funcs.match;
    auto *നിവേശം_data = lookup_context.match_data;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_rules; i++)
    {
      സ്ഥിരം auto &r = ഇത്+rule.arrayZ[i];

      സ്ഥിരം auto &നിവേശം = r.നിവേശം_Z;

      എങ്കിൽ (r.നിവേശം_എണ്ണം <= 1 ||
	  (!match_നിവേശം ||
	   match_നിവേശം (*first, നിവേശം.arrayZ[0], നിവേശം_data)))
      {
        എങ്കിൽ (!second ||
	    (r.നിവേശം_എണ്ണം <= 2 ||
	     (!match_നിവേശം ||
	      match_നിവേശം (*second, നിവേശം.arrayZ[1], നിവേശം_data)))
	   )
	{
	  എങ്കിൽ (r.apply (c, lookup_context))
	  {
	    എങ്കിൽ (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (സത്യം);
	  }
	}
	അല്ലെങ്കിൽ
	  unsafe_to = unsafe_to2;
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	വേണ്ടി (; i + 1 < അക്കം_rules; i++)
	{
	  സ്ഥിരം auto &r2 = ഇത്+rule.arrayZ[i + 1];
	  സ്ഥിരം auto &input2 = r2.നിവേശം_Z;
	  എങ്കിൽ (r2.നിവേശം_എണ്ണം <= 1 || input2.arrayZ[0] != നിവേശം.arrayZ[0])
	    നിർത്തുക;
	}
      }
    }
    എങ്കിൽ (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (അസത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c,
	       സ്ഥിരം hb_map_t *lookup_map,
	       സ്ഥിരം hb_map_t *klass_map = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    വേണ്ടി (സ്ഥിരം auto& _ : rule)
    {
      എങ്കിൽ (!_) തുടരുക;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o)) തുടരുക;

      എങ്കിൽ (!o->serialize_subset (c, _, ഇത്, lookup_map, klass_map))
      {
	out->rule.എടുക്കുക ();
	c->serializer->revert (o_snap);
      }
    }

    തർക്കം ret = തർക്കം (out->rule);
    എങ്കിൽ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (rule.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  പൊതു:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ഘടന RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
ഘടന ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
ഘടന ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    തിരികെ
    + hb_zip (ഇത്+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_map ([&] (സ്ഥിരം RuleSet &_) { തിരികെ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (ഇത്+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      തിരികെ c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](സ്ഥിരം hb_pair_t<hb_codepoint_t, unsigned> _) { തിരികെ hb_pair_t<unsigned, സ്ഥിരം RuleSet&> (_.first, ഇത്+ruleSet[_.second]); })
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (ഇത്+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      തിരികെ c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](സ്ഥിരം hb_pair_t<hb_codepoint_t, unsigned> _) { തിരികെ hb_pair_t<unsigned, സ്ഥിരം RuleSet&> (_.first, ഇത്+ruleSet[_.second]); })
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം
  {
    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (ഇത്+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    (ഇത്+coverage).collect_coverage (c->നിവേശം);

    ഘടന ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([digest] (സ്ഥിരം RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം RuleSet &rule_set = ഇത്+ruleSet[(ഇത്+coverage).get_coverage (c->glyphs[0])];
    ഘടന ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    തിരികെ rule_set.would_apply (c, lookup_context);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (likely (index == NOT_COVERED))
      return_trace (അസത്യം);

    സ്ഥിരം RuleSet &rule_set = ഇത്+ruleSet[index];
    ഘടന ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    സ്ഥിരം hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, ഇത്, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (തർക്കം (new_coverage));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && ruleSet.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ഘടന ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (glyphs))
      തിരികെ അസത്യം;

    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;

    hb_map_t cache;
    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (ഇത്+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    തിരികെ
    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_enumerate
    | hb_map ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം RuleSet &> p)
	      { തിരികെ class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;

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
    { തിരികെ class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം typename Types::template OffsetTo<RuleSet>&> _)
                {
                  സ്ഥിരം RuleSet& rule_set = ഇത്+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { തിരികെ class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം typename Types::template OffsetTo<RuleSet>&> _)
                {
                  സ്ഥിരം RuleSet& rule_set = ഇത്+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (c->glyphs))
      തിരികെ;

    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;

    hb_map_t cache;
    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_enumerate
    | hb_filter ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം RuleSet &> p)
    { തിരികെ class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (സ്ഥിരം RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    (ഇത്+coverage).collect_coverage (c->നിവേശം);

    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;
    ഘടന ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([=, &class_def] (സ്ഥിരം RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;
    unsigned പൂർണ്ണസംഖ്യ index = class_def.get_class (c->glyphs[0]);
    സ്ഥിരം RuleSet &rule_set = ഇത്+ruleSet[index];
    ഘടന ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    തിരികെ rule_set.would_apply (c, lookup_context);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  unsigned cache_cost () സ്ഥിരം
  {
    തിരികെ (ഇത്+classDef).cost ();
  }
  സ്ഥിതം തർക്കം cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    തിരികെ context_cache_func (c, op);
  }

  ഘടന external_cache_t
  {
    hb_ot_layout_ദ്വിമാന_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  ശൂന്യം *external_cache_create () സ്ഥിരം
  {
    unsigned എണ്ണം = ruleSet.len;
    unsigned size = വലുപ്പം (external_cache_t) -
		    HB_VAR_ARRAY * വലുപ്പം (hb_ot_layout_ruleset_digest_t) +
		    എണ്ണം * വലുപ്പം (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    എങ്കിൽ (likely (cache))
    {
      cache->coverage.clear ();
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	(ഇത്+ruleSet[i]).collect_first_നിവേശം_classes (&cache->rule_sets[i]);
    }
    തിരികെ cache;
  }
  തർക്കം apply_cached (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം { തിരികെ _apply (c, സത്യം, external_cache); }
  തർക്കം apply (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം { തിരികെ _apply (c, അസത്യം, external_cache); }
  തർക്കം _apply (hb_ot_apply_context_t *c, തർക്കം cached, ശൂന്യം *external_cache) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage_ദ്വിമാന (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#അല്ലെങ്കിൽ
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    സ്ഥിരം ClassDef &class_def = ഇത്+classDef;

    ഘടന ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_മൂല്യം : get_class_മൂല്യം
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    സ്ഥിരം RuleSet &rule_set = ഇത്+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    സ്ഥിരം hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#അല്ലെങ്കിൽ
    സ്ഥിരം hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;
    എങ്കിൽ (unlikely (!out->coverage.serialize_subset (c, coverage, ഇത്)))
      return_trace (അസത്യം);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, ഇത്, &klass_map);

    സ്ഥിരം hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (ഇത്+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (ഇത്+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    സ്ഥിരം hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    തർക്കം ret = സത്യം;
    പൂർണ്ണസംഖ്യ non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    വേണ്ടി (സ്ഥിരം auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o))
      {
	ret = അസത്യം;
	നിർത്തുക;
      }

      എങ്കിൽ (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, ഇത്, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    എങ്കിൽ (!ret || non_zero_index == -1) return_trace (അസത്യം);

    //prune empty trailing ruleSets
    --index;
    ആകുമ്പോൾ (index > non_zero_index)
    {
      out->ruleSet.എടുക്കുക ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (തർക്കം (out->ruleSet));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && classDef.sanitize (c, ഇത്) && ruleSet.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ഘടന ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverageZ[0]).intersects (glyphs))
      തിരികെ അസത്യം;

    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      ഇത്
    };
    തിരികെ context_intersects (glyphs,
			       glyph_എണ്ണം_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverageZ[0]).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ഇത്,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_എണ്ണം_2, (സ്ഥിരം HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_എണ്ണം_2, lookupRecord,
			   ഇത്,
			   coverageZ[0], lookup_context);

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverageZ[0]).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    ഘടന ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ഇത്
    };
    context_closure_lookup (c,
			    glyph_എണ്ണം_2, coverageZ.arrayZ + 1,
			    lookup_എണ്ണം_2, lookupRecord,
			    0, lookup_context);

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!intersects (c->glyphs))
      തിരികെ;
    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    recurse_lookups (c, lookup_എണ്ണം_2, lookupRecord);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    (ഇത്+coverageZ[0]).collect_coverage (c->നിവേശം);

    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    ഘടന ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      ഇത്
    };

    context_collect_glyphs_lookup (c,
				   glyph_എണ്ണം_2, coverageZ.arrayZ + 1,
				   lookup_എണ്ണം_2, lookupRecord,
				   lookup_context);
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    എങ്കിൽ (glyph_എണ്ണം_2 <= 1)
      *digest = hb_set_digest_t::full ();
    അല്ലെങ്കിൽ
      (ഇത്+coverageZ[1]).collect_coverage (digest);
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    ഘടന ContextApplyLookupContext lookup_context = {
      {match_coverage},
      ഇത്
    };
    തിരികെ context_would_apply_lookup (c,
				       glyph_എണ്ണം_2, coverageZ.arrayZ + 1,
				       lookup_എണ്ണം_2, lookupRecord,
				       lookup_context);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverageZ[0]; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    ഘടന ContextApplyLookupContext lookup_context = {
      {match_coverage},
      ഇത്
    };
    return_trace (context_apply_lookup (c,
					glyph_എണ്ണം_2,
					coverageZ.arrayZ + 1,
					lookup_എണ്ണം_2, lookupRecord,
					lookup_context));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->format = format;
    out->glyph_എണ്ണം_2 = glyph_എണ്ണം_2;

    auto coverages = coverageZ.as_array (glyph_എണ്ണം_2);

    വേണ്ടി (സ്ഥിരം auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      എങ്കിൽ (unlikely (!o)) return_trace (അസത്യം);
      എങ്കിൽ (!o->serialize_subset (c, offset, ഇത്)) return_trace (അസത്യം);
    }

    സ്ഥിരം auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_എണ്ണം_2));
    സ്ഥിരം hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned എണ്ണം = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_എണ്ണം_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_എണ്ണം_2, എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = glyph_എണ്ണം_2;
    എങ്കിൽ (unlikely (!എണ്ണം)) return_trace (അസത്യം); /* We want to access coverageZ[0] freely. */
    എങ്കിൽ (unlikely (!c->check_array (coverageZ.arrayZ, എണ്ണം))) return_trace (അസത്യം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!coverageZ[i].sanitize (c, ഇത്))) return_trace (അസത്യം);
    സ്ഥിരം LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_എണ്ണം_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_എണ്ണം_2)));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_എണ്ണം_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_എണ്ണം_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  പൊതു:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

ഘടന Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    അവസ്ഥ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    അവസ്ഥ 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
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

ഘടന ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  സ്ഥിരം ശൂന്യം *intersects_data[3];
  ശൂന്യം *intersects_cache[3];
  ശൂന്യം *intersected_glyphs_cache;
};

തരനിർവചനം ChainContextClosureLookupContext ChainContextDependLookupContext;

ഘടന ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  സ്ഥിരം ശൂന്യം *collect_data[3];
};

ഘടന ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  സ്ഥിരം ശൂന്യം *match_data[3];
  get_മൂല്യം_func_t get_നിവേശം_മൂല്യം;
};

സ്ഥിതം വരിയിൽ തർക്കം chain_context_intersects (സ്ഥിരം hb_set_t *glyphs,
					     unsigned പൂർണ്ണസംഖ്യ backtrack_എണ്ണം,
					     glyph_data_t backtrack,
					     unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
					     glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
					     unsigned പൂർണ്ണസംഖ്യ lookahead_എണ്ണം,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  തർക്കം bt_ok = array_is_subset_of (glyphs,
			     backtrack_എണ്ണം, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  തർക്കം in_ok = array_is_subset_of (glyphs,
			     നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0, നിവേശം,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  തർക്കം la_ok = array_is_subset_of (glyphs,
			     lookahead_എണ്ണം, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  തിരികെ bt_ok && in_ok && la_ok;
}

സ്ഥിതം വരിയിൽ ശൂന്യം chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned പൂർണ്ണസംഖ്യ backtrack_എണ്ണം,
						 glyph_data_t backtrack,
						 unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
						 glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
						 unsigned പൂർണ്ണസംഖ്യ lookahead_എണ്ണം,
						 glyph_data_t lookahead,
						 unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
						 സ്ഥിരം LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  എങ്കിൽ (chain_context_intersects (c->glyphs,
				backtrack_എണ്ണം, backtrack,
				നിവേശം_എണ്ണം, നിവേശം,
				lookahead_എണ്ണം, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     നിവേശം_എണ്ണം, നിവേശം,
		     lookup_എണ്ണം_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

സ്ഥിതം വരിയിൽ ശൂന്യം chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned പൂർണ്ണസംഖ്യ backtrack_എണ്ണം,
						glyph_data_t backtrack,
						unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
						glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
						unsigned പൂർണ്ണസംഖ്യ lookahead_എണ്ണം,
						glyph_data_t lookahead,
						unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
						സ്ഥിരം LookupRecord lookupRecord[],
						സ്ഥിരം ശൂന്യം *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  എങ്കിൽ (!chain_context_intersects (c->glyphs,
				 backtrack_എണ്ണം, backtrack,
				 നിവേശം_എണ്ണം, നിവേശം,
				 lookahead_എണ്ണം, lookahead,
				 lookup_context))
    തിരികെ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  എങ്കിൽ (unlikely (!scratch))
    തിരികെ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  തർക്കം context_sets_cached = context_depend_sets_are_cached (
    c, നിവേശം_എണ്ണം, lookup_എണ്ണം_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  എങ്കിൽ (unlikely (!c->depend_data->successful))
    തിരികെ;
  എങ്കിൽ (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    നിവേശം_എണ്ണം, നിവേശം,
				    lookup_എണ്ണം_2, lookupRecord,
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
    തിരികെ;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<സ്ഥിരം hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<സ്ഥിരം hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  വേണ്ടി (unsigned i = 0; i < backtrack_എണ്ണം; i++)
  {
    സ്ഥിരം hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], അസത്യം, &position_scratch);
    എങ്കിൽ (unlikely (!pos_glyphs))
      തിരികെ;
    എങ്കിൽ (!pos_glyphs->is_empty ())
    {
      എങ്കിൽ (unlikely (!backtrack_sets.തള്ളുക_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        തിരികെ;
      }
    }
  }

  /* Extract lookahead glyphs */
  വേണ്ടി (unsigned i = 0; i < lookahead_എണ്ണം; i++)
  {
    സ്ഥിരം hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], അസത്യം, &position_scratch);
    എങ്കിൽ (unlikely (!pos_glyphs))
      തിരികെ;
    എങ്കിൽ (!pos_glyphs->is_empty ())
    {
      എങ്കിൽ (unlikely (!lookahead_sets.തള്ളുക_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        തിരികെ;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  വേണ്ടി (സ്ഥിരം hb_set_t *back_set : backtrack_sets)
  {
    എങ്കിൽ (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } അല്ലെങ്കിൽ എങ്കിൽ (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      എങ്കിൽ (unlikely (set_idx == HB_CODEPOINT_INVALID))
        തിരികെ;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  വേണ്ടി (സ്ഥിരം hb_set_t *look_set : lookahead_sets)
  {
    എങ്കിൽ (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } അല്ലെങ്കിൽ എങ്കിൽ (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      എങ്കിൽ (unlikely (set_idx == HB_CODEPOINT_INVALID))
        തിരികെ;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  എങ്കിൽ (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    തിരികെ;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<സ്ഥിരം hb_set_t *> &നിവേശം_position_glyphs = scratch->നിവേശം_position_glyphs;

  എങ്കിൽ (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    സ്ഥിരം hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      സത്യം, &position_scratch, &scratch->compact_workspace);
    എങ്കിൽ (unlikely (!pos0_glyphs ||
		  !നിവേശം_position_glyphs.തള്ളുക_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      തിരികെ;
    }

    /* Positions 1+ (Input array) */
    വേണ്ടി (unsigned i = 0; i + 1 < നിവേശം_എണ്ണം; i++)
    {
      സ്ഥിരം hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	നിവേശം[i], അസത്യം, &position_scratch);
      എങ്കിൽ (unlikely (!pos_glyphs ||
		    !നിവേശം_position_glyphs.തള്ളുക_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	തിരികെ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  നിവേശം_എണ്ണം, നിവേശം,
				  lookup_എണ്ണം_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &നിവേശം_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

സ്ഥിതം വരിയിൽ ശൂന്യം chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned പൂർണ്ണസംഖ്യ backtrack_എണ്ണം,
							glyph_data_t backtrack,
							unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
							glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
							unsigned പൂർണ്ണസംഖ്യ lookahead_എണ്ണം,
							glyph_data_t lookahead,
							unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
							സ്ഥിരം LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_എണ്ണം, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->നിവേശം,
		 നിവേശം_എണ്ണം ? നിവേശം_എണ്ണം - 1 : 0, നിവേശം,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_എണ്ണം, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_എണ്ണം_2, lookupRecord);
}

സ്ഥിതം വരിയിൽ തർക്കം chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned പൂർണ്ണസംഖ്യ backtrack_എണ്ണം,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
						     glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
						     unsigned പൂർണ്ണസംഖ്യ lookahead_എണ്ണം,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2 HB_UNUSED,
						     സ്ഥിരം LookupRecord lookupRecord[] HB_UNUSED,
						     സ്ഥിരം ChainContextApplyLookupContext &lookup_context)
{
  തിരികെ (c->zero_context ? !backtrack_എണ്ണം && !lookahead_എണ്ണം : സത്യം)
      && would_match_നിവേശം (c,
			    നിവേശം_എണ്ണം, നിവേശം,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

സ്ഥിതം വരിയിൽ തർക്കം chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned പൂർണ്ണസംഖ്യ backtrack_എണ്ണം,
					       glyph_data_t backtrack,
					       unsigned പൂർണ്ണസംഖ്യ നിവേശം_എണ്ണം, /* Including the first glyph (not matched) */
					       glyph_data_t നിവേശം, /* Array of input values--start with second glyph */
					       unsigned പൂർണ്ണസംഖ്യ lookahead_എണ്ണം,
					       glyph_data_t lookahead,
					       unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം_2,
					       സ്ഥിരം LookupRecord lookupRecord[],
					       സ്ഥിരം ChainContextApplyLookupContext &lookup_context)
{
  എങ്കിൽ (unlikely (നിവേശം_എണ്ണം > HB_MAX_CONTEXT_LENGTH)) തിരികെ അസത്യം;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  തർക്കം ret = സത്യം;
  എങ്കിൽ (!(match_നിവേശം (c,
		     നിവേശം_എണ്ണം, നിവേശം,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_എണ്ണം, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    തിരികെ അസത്യം;
  }

  എങ്കിൽ (!match_backtrack (c,
			backtrack_എണ്ണം, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    തിരികെ അസത്യം;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		നിവേശം_എണ്ണം,
		lookup_എണ്ണം_2, lookupRecord,
		match_end);

  തിരികെ ret;
}

template <typename Types>
ഘടന ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend ഘടന ChainRuleSetOf;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    തിരികെ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     നിവേശം.lenP1, നിവേശം.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ശൂന്യം depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 നിവേശം.lenP1, നിവേശം.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ഇത്,
				 value,
				 lookup_context);
  }

  ശൂന്യം closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;

    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  നിവേശം.lenP1, നിവേശം.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;
    എങ്കിൽ (!intersects (c->glyphs, lookup_context)) തിരികെ;

    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 നിവേശം.lenP1, നിവേശം.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest,
			      സ്ഥിരം ClassDef *നിവേശം_class_def = nullptr,
			      സ്ഥിരം ClassDef *lookahead_class_def = nullptr) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);

    unsigned second;
    സ്ഥിരം ClassDef *class_def;
    എങ്കിൽ (നിവേശം.lenP1 > 1)
    {
      second = നിവേശം.arrayZ[0];
      class_def = നിവേശം_class_def;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    അല്ലെങ്കിൽ
    {
      *digest = hb_set_digest_t::full ();
      തിരികെ;
    }

    എങ്കിൽ (!class_def)
      digest->add (second);
    അല്ലെങ്കിൽ എങ്കിൽ (!second)
      *digest = hb_set_digest_t::full ();
    അല്ലെങ്കിൽ
      class_def->collect_class (digest, second);
  }

  തർക്കം would_apply (hb_would_apply_context_t *c,
		    സ്ഥിരം ChainContextApplyLookupContext &lookup_context) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    തിരികെ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     നിവേശം.lenP1, നിവേശം.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  തർക്കം apply (hb_ot_apply_context_t *c,
	      സ്ഥിരം ChainContextApplyLookupContext &lookup_context) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      നിവേശം.lenP1, നിവേശം.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) സ്ഥിരം
  {
    c->copy (len);
    വേണ്ടി (സ്ഥിരം auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം hb_map_t *lookup_map,
		  സ്ഥിരം hb_map_t *backtrack_map,
		  സ്ഥിരം hb_map_t *നിവേശം_map = nullptr,
		  സ്ഥിരം hb_map_t *lookahead_map = nullptr) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    സ്ഥിരം hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    എങ്കിൽ (നിവേശം_map) mapping = നിവേശം_map;
    serialize_array (c, നിവേശം.lenP1, + നിവേശം.iter ()
				     | hb_map (mapping));

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    എങ്കിൽ (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_എണ്ണം_2 = c->embed (&(lookup.len));
    എങ്കിൽ (!lookup_എണ്ണം_2) return_trace (അസത്യം);

    unsigned എണ്ണം = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_എണ്ണം_2, എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം subset (hb_subset_context_t *c,
	       സ്ഥിരം hb_map_t *lookup_map,
	       സ്ഥിരം hb_map_t *backtrack_map = nullptr,
	       സ്ഥിരം hb_map_t *നിവേശം_map = nullptr,
	       സ്ഥിരം hb_map_t *lookahead_map = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);

    എങ്കിൽ (!backtrack_map)
    {
      സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      എങ്കിൽ (!hb_all (backtrack, glyphset) ||
	  !hb_all (നിവേശം, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (അസത്യം);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (നിവേശം, നിവേശം_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (അസത്യം);

      serialize (c->serializer, lookup_map, backtrack_map, നിവേശം_map, lookahead_map);
    }

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* Hyper-optimized sanitized because this is really hot. */
    എങ്കിൽ (unlikely (!backtrack.len.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    എങ്കിൽ (unlikely (!നിവേശം.lenP1.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    എങ്കിൽ (unlikely (!lookahead.len.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  സംരക്ഷിത:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		നിവേശം_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  പൊതു:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
ഘടന ChainRuleSetOf
{
  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_map ([&] (സ്ഥിരം ChainRuleType &_) { തിരികെ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ശൂന്യം collect_first_നിവേശം_classes (hb_ot_layout_ruleset_digest_t *digest) സ്ഥിരം
  {
    digest->init ();
    വേണ്ടി (unsigned i = 0; i < rule.len; i++)
    {
      സ്ഥിരം ChainRuleType &r = ഇത്+rule.arrayZ[i];
      സ്ഥിരം auto &നിവേശം = StructAfter<decltype (r.നിവേശം_X)> (r.backtrack);
      എങ്കിൽ (നിവേശം.lenP1 <= 1)
      {
	digest->set_full ();
	തിരികെ;
      }
      digest->add (നിവേശം.arrayZ[0]);
    }
  }
  ശൂന്യം depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) സ്ഥിരം
  {
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  ശൂന്യം closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;

    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (c->lookup_limit_exceeded ())) തിരികെ;

    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest,
			      സ്ഥിരം ClassDef *നിവേശം_class_def = nullptr,
			      സ്ഥിരം ClassDef *lookahead_class_def = nullptr) സ്ഥിരം
  {
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([=] (സ്ഥിരം ChainRuleType &_)
		{ _.collect_second_glyphs (digest, നിവേശം_class_def, lookahead_class_def); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c,
		    സ്ഥിരം ChainContextApplyLookupContext &lookup_context) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (rule)
    | hb_map (hb_add (ഇത്))
    | hb_map ([&] (സ്ഥിരം ChainRuleType &_) { തിരികെ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  തർക്കം apply (hb_ot_apply_context_t *c,
	      സ്ഥിരം ChainContextApplyLookupContext &lookup_context,
	      സ്ഥിരം hb_ot_layout_ruleset_digest_t *digest = nullptr) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    unsigned അക്കം_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    എങ്കിൽ (HB_OPTIMIZE_SIZE_VAL || അക്കം_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (ഇത്))
      | hb_map ([&] (സ്ഥിരം ChainRuleType &_) { തിരികെ _.apply (c, lookup_context); })
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
    തർക്കം matched = skippy_iter.next (&unsafe_to1);
    എങ്കിൽ (likely (matched))
    {
      എങ്കിൽ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        പോകുക slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      എങ്കിൽ (digest && lookup_context.get_നിവേശം_മൂല്യം &&
	  !digest->may_have (lookup_context.get_നിവേശം_മൂല്യം (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (അസത്യം);
	}
    }
    അല്ലെങ്കിൽ
    {
      തർക്കം unsafe_to_concat = അസത്യം;
      തർക്കം ret =
      + hb_iter (rule)
      | hb_map (hb_add (ഇത്))
      | hb_filter ([&] (സ്ഥിരം ChainRuleType &_)
		   {
		     സ്ഥിരം auto &നിവേശം = StructAfter<decltype (_.നിവേശം_X)> (_.backtrack);
		     സ്ഥിരം auto &lookahead = StructAfter<decltype (_.lookaheadX)> (നിവേശം);
		     എങ്കിൽ (നിവേശം.lenP1 <= 1 && lookahead.len == 0) തിരികെ സത്യം;
		     unsafe_to_concat = സത്യം;
		     തിരികെ അസത്യം;
		   })
      | hb_map ([&] (സ്ഥിരം ChainRuleType &_) { തിരികെ _.apply (c, lookup_context); })
      | hb_any
      ;
      എങ്കിൽ (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    എങ്കിൽ (likely (matched))
    {
      എങ്കിൽ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        പോകുക slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_നിവേശം = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *നിവേശം_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_rules; i++)
    {
      സ്ഥിരം auto &r = ഇത്+rule.arrayZ[i];

      സ്ഥിരം auto &നിവേശം = StructAfter<decltype (r.നിവേശം_X)> (r.backtrack);
      സ്ഥിരം auto &lookahead = StructAfter<decltype (r.lookaheadX)> (നിവേശം);

      unsigned lenP1 = നിവേശം.lenP1;
      എങ്കിൽ (lenP1 > 1 ?
	   (!match_നിവേശം ||
	    match_നിവേശം (*first, നിവേശം.arrayZ[0], നിവേശം_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        എങ്കിൽ (!second ||
	    (lenP1 > 2 ?
	     (!match_നിവേശം ||
	      match_നിവേശം (*second, നിവേശം.arrayZ[1], നിവേശം_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  എങ്കിൽ (r.apply (c, lookup_context))
	  {
	    എങ്കിൽ (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (സത്യം);
	  }
	}
	അല്ലെങ്കിൽ
	  unsafe_to = unsafe_to2;
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	എങ്കിൽ (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  വേണ്ടി (; i + 1 < അക്കം_rules; i++)
	  {
	    സ്ഥിരം auto &r2 = ഇത്+rule.arrayZ[i + 1];
	    സ്ഥിരം auto &input2 = StructAfter<decltype (r2.നിവേശം_X)> (r2.backtrack);
	    എങ്കിൽ (input2.lenP1 <= 1 || input2.arrayZ[0] != നിവേശം.arrayZ[0])
	      നിർത്തുക;
	  }
	}
      }
    }
    എങ്കിൽ (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (അസത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c,
	       സ്ഥിരം hb_map_t *lookup_map,
	       സ്ഥിരം hb_map_t *backtrack_klass_map = nullptr,
	       സ്ഥിരം hb_map_t *നിവേശം_klass_map = nullptr,
	       സ്ഥിരം hb_map_t *lookahead_klass_map = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    വേണ്ടി (സ്ഥിരം auto& _ : rule)
    {
      എങ്കിൽ (!_) തുടരുക;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o)) തുടരുക;

      എങ്കിൽ (!o->serialize_subset (c, _, ഇത്,
				lookup_map,
				backtrack_klass_map,
				നിവേശം_klass_map,
				lookahead_klass_map))
      {
	out->rule.എടുക്കുക ();
	c->serializer->revert (o_snap);
      }
    }

    തർക്കം ret = തർക്കം (out->rule);
    എങ്കിൽ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (rule.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  പൊതു:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ഘടന ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (വലുപ്പം (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
ഘടന ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
ഘടന ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    തിരികെ
    + hb_zip (ഇത്+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_map ([&] (സ്ഥിരം ChainRuleSet &_) { തിരികെ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (ഇത്+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      തിരികെ c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](സ്ഥിരം hb_pair_t<hb_codepoint_t, unsigned> _) { തിരികെ hb_pair_t<unsigned, സ്ഥിരം ChainRuleSet&> (_.first, ഇത്+ruleSet[_.second]); })
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (ഇത്+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      തിരികെ c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](സ്ഥിരം hb_pair_t<hb_codepoint_t, unsigned> _) { തിരികെ hb_pair_t<unsigned, സ്ഥിരം ChainRuleSet&> (_.first, ഇത്+ruleSet[_.second]); })
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം
  {
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (ഇത്+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    (ഇത്+coverage).collect_coverage (c->നിവേശം);

    ഘടന ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([digest] (സ്ഥിരം ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം ChainRuleSet &rule_set = ഇത്+ruleSet[(ഇത്+coverage).get_coverage (c->glyphs[0])];
    ഘടന ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    തിരികെ rule_set.would_apply (c, lookup_context);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    സ്ഥിരം ChainRuleSet &rule_set = ഇത്+ruleSet[index];
    ഘടന ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    സ്ഥിരം hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, ഇത്, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (തർക്കം (new_coverage));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && ruleSet.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
ഘടന ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (glyphs))
      തിരികെ അസത്യം;

    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (ഇത്+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    നിവേശം_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    തിരികെ
    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_enumerate
    | hb_map ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം ChainRuleSet &> p)
	      { തിരികെ നിവേശം_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { തിരികെ നിവേശം_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  സ്ഥിരം ChainRuleSet& chainrule_set = ഇത്+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs)) തിരികെ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { തിരികെ നിവേശം_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  സ്ഥിരം ChainRuleSet& chainrule_set = ഇത്+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (c->glyphs))
      തിരികെ;

    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { തിരികെ നിവേശം_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (സ്ഥിരം ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    (ഇത്+coverage).collect_coverage (c->നിവേശം);

    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

    ഘടന ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([digest, &നിവേശം_class_def, &lookahead_class_def] (സ്ഥിരം ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &നിവേശം_class_def, &lookahead_class_def); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

    unsigned പൂർണ്ണസംഖ്യ index = നിവേശം_class_def.get_class (c->glyphs[0]);
    സ്ഥിരം ChainRuleSet &rule_set = ഇത്+ruleSet[index];
    ഘടന ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def}
    };
    തിരികെ rule_set.would_apply (c, lookup_context);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  unsigned cache_cost () സ്ഥിരം
  {
    തിരികെ (ഇത്+നിവേശം_Class_Def).cost () + (ഇത്+lookaheadClassDef).cost ();
  }
  സ്ഥിതം തർക്കം cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    തിരികെ context_cache_func (c, op);
  }

  ഘടന external_cache_t
  {
    hb_ot_layout_ദ്വിമാന_cache_t coverage;
    hb_ot_layout_mapping_cache_t നിവേശം_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  ശൂന്യം *external_cache_create () സ്ഥിരം
  {
    unsigned എണ്ണം = ruleSet.len;
    unsigned size = വലുപ്പം (external_cache_t) -
		    HB_VAR_ARRAY * വലുപ്പം (hb_ot_layout_ruleset_digest_t) +
		    എണ്ണം * വലുപ്പം (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    എങ്കിൽ (likely (cache))
    {
      cache->coverage.clear ();
      cache->നിവേശം_class.clear ();
      cache->lookahead_class.clear ();
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	(ഇത്+ruleSet[i]).collect_first_നിവേശം_classes (&cache->rule_sets[i]);
    }
    തിരികെ cache;
  }
  തർക്കം apply_cached (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം { തിരികെ _apply (c, സത്യം, external_cache); }
  തർക്കം apply (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം { തിരികെ _apply (c, അസത്യം, external_cache); }
  തർക്കം _apply (hb_ot_apply_context_t *c, തർക്കം cached, ശൂന്യം *external_cache) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage_ദ്വിമാന (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#അല്ലെങ്കിൽ
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    സ്ഥിരം ClassDef &backtrack_class_def = ഇത്+backtrackClassDef;
    സ്ഥിരം ClassDef &നിവേശം_class_def = ഇത്+നിവേശം_Class_Def;
    സ്ഥിരം ClassDef &lookahead_class_def = ഇത്+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t നിവേശം_cache_data = {
      &നിവേശം_class_def,
      cache ? &cache->നിവേശം_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    സ്ഥിരം ശൂന്യം *നിവേശം_match_data = cached
      ? static_cast<സ്ഥിരം ശൂന്യം *> (&നിവേശം_class_def)
      : static_cast<സ്ഥിരം ശൂന്യം *> (&നിവേശം_cache_data);
    സ്ഥിരം ശൂന്യം *lookahead_match_data = cached
      ? static_cast<സ്ഥിരം ശൂന്യം *> (&lookahead_class_def)
      : static_cast<സ്ഥിരം ശൂന്യം *> (&lookahead_cache_data);
    തർക്കം backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    സ്ഥിരം ശൂന്യം *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<സ്ഥിരം ശൂന്യം *> (&lookahead_cache_data)
      : static_cast<സ്ഥിരം ശൂന്യം *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    ഘടന ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       നിവേശം_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_മൂല്യം : get_class_cache_മൂല്യം
#അല്ലെങ്കിൽ
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &നിവേശം_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_മൂല്യം : get_class_മൂല്യം
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (നിവേശം_class_def, c->buffer->cur())
         : നിവേശം_class_def.get_class (c->buffer->cur().codepoint, നിവേശം_cache_data.cache);
#അല്ലെങ്കിൽ
    index = cached
         ? get_class_cached2 (നിവേശം_class_def, c->buffer->cur())
          : നിവേശം_class_def.get_class (c->buffer->cur().codepoint);
#endif
    സ്ഥിരം ChainRuleSet &rule_set = ഇത്+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    സ്ഥിരം hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#അല്ലെങ്കിൽ
    സ്ഥിരം hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, ഇത്);

    hb_map_t backtrack_klass_map;
    hb_map_t നിവേശം_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, ഇത്, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->നിവേശം_Class_Def.serialize_subset (c, നിവേശം_Class_Def, ഇത്, &നിവേശം_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, ഇത്, &lookahead_klass_map);

    എങ്കിൽ (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   നിവേശം_klass_map,
						   lookahead_klass_map)))
      return_trace (അസത്യം);

    സ്ഥിരം hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (ഇത്+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (ഇത്+നിവേശം_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    പൂർണ്ണസംഖ്യ non_zero_index = -1, index = 0;
    തർക്കം ret = സത്യം;
    സ്ഥിരം hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    വേണ്ടി (സ്ഥിരം auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (നിവേശം_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o))
      {
	ret = അസത്യം;
	നിർത്തുക;
      }
      എങ്കിൽ (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, ഇത്,
			       lookup_map,
			       &backtrack_klass_map,
			       &നിവേശം_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    എങ്കിൽ (!ret || non_zero_index == -1) return_trace (അസത്യം);

    // prune empty trailing ruleSets
    എങ്കിൽ (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (തർക്കം (out->ruleSet));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) &&
		  backtrackClassDef.sanitize (c, ഇത്) &&
		  നിവേശം_Class_Def.sanitize (c, ഇത്) &&
		  lookaheadClassDef.sanitize (c, ഇത്) &&
		  ruleSet.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		നിവേശം_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  പൊതു:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
ഘടന ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);

    എങ്കിൽ (!(ഇത്+നിവേശം[0]).intersects (glyphs))
      തിരികെ അസത്യം;

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {ഇത്, ഇത്, ഇത്}
    };
    തിരികെ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     നിവേശം.len, നിവേശം.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);

    എങ്കിൽ (!(ഇത്+നിവേശം[0]).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs))
      തിരികെ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ഇത്, ഇത്, ഇത്},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (സ്ഥിരം HBUINT16 *) backtrack.arrayZ,
				 നിവേശം.len, (സ്ഥിരം HBUINT16 *) നിവേശം.arrayZ + 1,
				 lookahead.len, (സ്ഥിരം HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ഇത്,
				 നിവേശം[0], lookup_context);

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);

    എങ്കിൽ (!(ഇത്+നിവേശം[0]).intersects (c->glyphs))
      തിരികെ;

    hb_set_t* cur_active_glyphs = c->തള്ളുക_cur_active_glyphs ();
    എങ്കിൽ (unlikely (!cur_active_glyphs))
      തിരികെ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ഘടന ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ഇത്, ഇത്, ഇത്}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  നിവേശം.len, നിവേശം.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->എടുക്കുക_cur_done_glyphs ();
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!intersects (c->glyphs))
      തിരികെ;

    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);

    (ഇത്+നിവേശം[0]).collect_coverage (c->നിവേശം);

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    ഘടന ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {ഇത്, ഇത്, ഇത്}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 നിവേശം.len, നിവേശം.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    എങ്കിൽ (നിവേശം.len > 1)
      (ഇത്+നിവേശം[1]).collect_coverage (digest);
    അല്ലെങ്കിൽ എങ്കിൽ (lookahead.len)
      (ഇത്+lookahead[0]).collect_coverage (digest);
    അല്ലെങ്കിൽ
      *digest = hb_set_digest_t::full ();
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ഘടന ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {ഇത്, ഇത്, ഇത്}
    };
    തിരികെ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     നിവേശം.len, നിവേശം.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം
  {
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    തിരികെ ഇത്+നിവേശം[0];
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);

    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+നിവേശം[0]).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ഘടന ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {ഇത്, ഇത്, ഇത്}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      നിവേശം.len, നിവേശം.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, സ്ഥിരം ശൂന്യം* base) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    എങ്കിൽ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (അസത്യം);

    വേണ്ടി (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    എങ്കിൽ (unlikely (!c->serializer->embed (ഇത്->format))) return_trace (അസത്യം);

    എങ്കിൽ (!serialize_coverage_offsets (c, backtrack.iter (), ഇത്))
      return_trace (അസത്യം);

    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    എങ്കിൽ (!serialize_coverage_offsets (c, നിവേശം.iter (), ഇത്))
      return_trace (അസത്യം);

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    എങ്കിൽ (!serialize_coverage_offsets (c, lookahead.iter (), ഇത്))
      return_trace (അസത്യം);

    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    സ്ഥിരം hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_എണ്ണം_2 = c->serializer->copy<HBUINT16> (lookup.len);
    എങ്കിൽ (!lookup_എണ്ണം_2) return_trace (അസത്യം);

    unsigned എണ്ണം = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_എണ്ണം_2, എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!backtrack.sanitize (c, ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &നിവേശം = StructAfter<decltype (നിവേശം_X)> (backtrack);
    എങ്കിൽ (unlikely (!നിവേശം.sanitize (c, ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    എങ്കിൽ (unlikely (!നിവേശം.len)) return_trace (അസത്യം); /* To be consistent with Context. */
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (നിവേശം);
    എങ്കിൽ (unlikely (!lookahead.sanitize (c, ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		നിവേശം_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  പൊതു:
  DEFINE_SIZE_MIN (10);
};

ഘടന ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    അവസ്ഥ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }		format;	/* Format identifier */
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
ഘടന ExtensionFormat1
{
  unsigned പൂർണ്ണസംഖ്യ get_type () സ്ഥിരം { തിരികെ extensionLookupType; }

  template <typename X>
  സ്ഥിരം X& get_subtable () സ്ഥിരം
  { തിരികെ ഇത് + reinterpret_cast<സ്ഥിരം Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, ഇത്))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    സ്ഥിരം auto& src_offset =
        reinterpret_cast<സ്ഥിരം Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, ഇത്, get_type ()));
  }

  സംരക്ഷിത:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
ഘടന Extension
{
  unsigned പൂർണ്ണസംഖ്യ get_type () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_type ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }
  template <typename X>
  സ്ഥിരം X& get_subtable () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.template get_subtable<typename T::SubTable> ();
    സ്ഥിരസ്ഥിതി:തിരികെ Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.subset (c);
    സ്ഥിരസ്ഥിതി: തിരികെ c->default_return_മൂല്യം ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

ഘടന hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  സ്ഥിതം hb_ot_layout_lookup_accelerator_t *create (സ്ഥിരം TLookup &lookup)
  {
    unsigned എണ്ണം = lookup.get_subtable_എണ്ണം ();

    unsigned size = വലുപ്പം (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * വലുപ്പം (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    എണ്ണം * വലുപ്പം (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    എങ്കിൽ (unlikely (!thiz))
      തിരികെ nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    ആകുമ്പോൾ (എണ്ണം && !thiz->subtables[എണ്ണം - 1].apply_func)
      എണ്ണം--;

    thiz->digest.init ();
    വേണ്ടി (auto& subtable : hb_iter (thiz->subtables, എണ്ണം))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->എണ്ണം = എണ്ണം;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    തിരികെ thiz;
  }

  ശൂന്യം fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം
  { തിരികെ digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം apply (hb_ot_apply_context_t *c, തർക്കം use_cache) സ്ഥിരം
  {
    എങ്കിൽ (എണ്ണം == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      എങ്കിൽ (use_cache)
	തിരികെ subtables[0].apply_cached_no_digest (c);
#endif
      തിരികെ subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    എങ്കിൽ (use_cache)
    {
      തിരികെ
      + hb_iter (hb_iter (subtables, എണ്ണം))
      | hb_map ([&c] (സ്ഥിരം hb_accelerate_subtables_context_t::hb_applicable_t &_) { തിരികെ _.apply_cached (c); })
      | hb_any
      ;
    }
    അല്ലെങ്കിൽ
#endif
    {
      തിരികെ
      + hb_iter (hb_iter (subtables, എണ്ണം))
      | hb_map ([&c] (സ്ഥിരം hb_accelerate_subtables_context_t::hb_applicable_t &_) { തിരികെ _.apply (c); })
      | hb_any
      ;
    }
    തിരികെ അസത്യം;
  }

  തർക്കം cache_enter (hb_ot_apply_context_t *c) സ്ഥിരം
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    തിരികെ subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
  }
  ശൂന്യം cache_leave (hb_ot_apply_context_t *c) സ്ഥിരം
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  സ്വകാര്യ:
  unsigned എണ്ണം = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ഘടന GSUBGPOS
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

  size_t get_size () സ്ഥിരം
  {
    എങ്കിൽ (version.major != 1) തിരികെ version.static_size;

    തിരികെ min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#അല്ലെങ്കിൽ
	   0
#endif
	   ;
  }

  സ്ഥിരം ScriptList &get_script_list () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u && scriptList2) തിരികെ ഇത് + scriptList2;
#endif
    തിരികെ ഇത് + scriptList;
  }
  സ്ഥിരം FeatureList &get_feature_list () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u && featureList2) തിരികെ ഇത് + featureList2;
#endif
    തിരികെ ഇത് + featureList;
  }
  template <typename TLookup>
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (ഇത്, get_size ())))
      return_trace (അസത്യം);
    hb_barrier ();

    തർക്കം script_list_ok = scriptList.sanitize (c, ഇത്);
    തർക്കം feature_list_ok = featureList.sanitize (c, ഇത്);
    തർക്കം lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, ഇത്);

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u)
    {
      എങ്കിൽ (scriptList2) script_list_ok = scriptList2.sanitize (c, ഇത്);
      എങ്കിൽ (featureList2) feature_list_ok = featureList2.sanitize (c, ഇത്);
      എങ്കിൽ (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, ഇത്);
    }
#endif

    എങ്കിൽ (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (അസത്യം);

#ifndef HB_NO_VAR
    എങ്കിൽ (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, ഇത്, &get_feature_list ()))))
      return_trace (അസത്യം);
#endif

    return_trace (സത്യം);
  }

  template <typename TLookup>
  തർക്കം subset (hb_subset_layout_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (version.major != 1) return_trace (അസത്യം);

    auto *out = c->subset_context->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (അസത്യം);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u)
    {
      എങ്കിൽ (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (അസത്യം);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      എങ്കിൽ (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 ഇത്, c);
      അല്ലെങ്കിൽ
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 ഇത്, c);

      എങ്കിൽ (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 ഇത്, c);
      അല്ലെങ്കിൽ
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 ഇത്, c);

      എങ്കിൽ (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, ഇത്, c);
      അല്ലെങ്കിൽ
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, ഇത്, c);
    }
    അല്ലെങ്കിൽ
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     ഇത്,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     ഇത്,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					ഇത്,
					c);
    }

#ifndef HB_NO_VAR
    എങ്കിൽ (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      എങ്കിൽ (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (അസത്യം);

      // if all axes are pinned all feature vars are dropped.
      തർക്കം ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, ഇത്, c);
      എങ്കിൽ (!ret)
      {
        out->featureVars = 0;
	എങ്കിൽ (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (സത്യം);
  }

  unsigned പൂർണ്ണസംഖ്യ get_lookup_എണ്ണം () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u && lookupList2)
      തിരികെ (ഇത് + lookupList2).len;
#endif
    തിരികെ (ഇത് + lookupList).len;
  }
  സ്ഥിരം Lookup& get_lookup (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u && lookupList2)
      തിരികെ (ഇത് + lookupList2)[i];
#endif
    തിരികെ (ഇത് + lookupList)[i];
  }
  സ്ഥിരം FeatureVariations &get_feature_variations () സ്ഥിരം
  {
    തിരികെ (version.to_int () >= 0x00010001u && hb_barrier ()) ? ഇത്+featureVars : Null (FeatureVariations);
  }

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }
  unsigned പൂർണ്ണസംഖ്യ get_script_എണ്ണം () സ്ഥിരം
  { തിരികെ get_script_list ().len; }
  സ്ഥിരം Tag& get_script_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ get_script_list ().get_tag (i); }
  unsigned പൂർണ്ണസംഖ്യ get_script_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
				unsigned പൂർണ്ണസംഖ്യ *script_എണ്ണം /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) സ്ഥിരം
  { തിരികെ get_script_list ().get_tags (start_offset, script_എണ്ണം, script_tags); }
  സ്ഥിരം Script& get_script (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ get_script_list ()[i]; }
  തർക്കം find_script_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  { തിരികെ get_script_list ().find_index (tag, index); }

  unsigned പൂർണ്ണസംഖ്യ get_feature_എണ്ണം () സ്ഥിരം
  { തിരികെ get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned പൂർണ്ണസംഖ്യ get_feature_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
				 unsigned പൂർണ്ണസംഖ്യ *feature_എണ്ണം_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) സ്ഥിരം
  { തിരികെ get_feature_list ().get_tags (start_offset, feature_എണ്ണം_2, feature_tags); }
  സ്ഥിരം Feature& get_feature (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ get_feature_list ()[i]; }
  തർക്കം find_feature_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  { തിരികെ get_feature_list ().find_index (tag, index); }

  തർക്കം find_variations_index (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ അക്കം_coords,
			      unsigned പൂർണ്ണസംഖ്യ *index,
			      ItemVarStoreInstancer *instancer) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    തിരികെ അസത്യം;
#endif
    തിരികെ get_feature_variations ().find_index (coords, അക്കം_coords, index, instancer);
  }
  തർക്കം get_feature_variations_state (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
				     unsigned പൂർണ്ണസംഖ്യ അക്കം_coords,
				     unsigned പൂർണ്ണസംഖ്യ *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) സ്ഥിരം
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    തിരികെ സത്യം;
#അല്ലെങ്കിൽ
    find_variations_index (coords, അക്കം_coords, index, instancer);
    തിരികെ get_feature_variations ().collect_lookup_conditions (
	coords,
	അക്കം_coords,
	instancer,
	lookup_conditions);
#endif
  }
  തർക്കം get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<സ്ഥിരം unsigned> conditions,
                              hb_set_t *lookup_indexes) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ അസത്യം;
#അല്ലെങ്കിൽ
    തിരികെ get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  സ്ഥിരം Feature& get_feature_variation (unsigned പൂർണ്ണസംഖ്യ feature_index,
					unsigned പൂർണ്ണസംഖ്യ variations_index) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    എങ്കിൽ (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      സ്ഥിരം Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      എങ്കിൽ (feature)
	തിരികെ *feature;
    }
#endif
    തിരികെ get_feature (feature_index);
  }

  ശൂന്യം feature_variation_collect_lookups (സ്ഥിരം hb_set_t *feature_indexes,
					  സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  ശൂന്യം collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  ശൂന്യം collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) സ്ഥിരം
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  ശൂന്യം closure_lookups (hb_face_t      *face,
			സ്ഥിരം hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) സ്ഥിരം
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    വേണ്ടി (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<സ്ഥിരം TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  ശൂന്യം prune_langsys (സ്ഥിരം hb_map_t *duplicate_feature_map,
                      സ്ഥിരം hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) സ്ഥിരം
  {
    hb_prune_langsys_context_t c (ഇത്, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned എണ്ണം = get_script_എണ്ണം ();
    വേണ്ടി (unsigned script_index = 0; script_index < എണ്ണം; script_index++)
    {
      സ്ഥിരം Tag& tag = get_script_tag (script_index);
      എങ്കിൽ (!layout_scripts->has (tag)) തുടരുക;
      സ്ഥിരം Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  ശൂന്യം prune_features (സ്ഥിരം hb_map_t *lookup_indices, /* IN */
		       സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       സ്ഥിരം hb_hashmap_t<unsigned, സ്ഥിരം Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    എങ്കിൽ (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      തിരികെ;
    }
#endif

    വേണ്ടി (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      എങ്കിൽ (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        തുടരുക;


      സ്ഥിരം Feature *f = &(get_feature (i));
      സ്ഥിരം Feature** p = nullptr;
      എങ്കിൽ (feature_substitutes_map->has (i, &p))
        f = *p;

      എങ്കിൽ (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        തുടരുക;

      എങ്കിൽ (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  ശൂന്യം collect_name_ids (സ്ഥിരം hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         സ്ഥിരം hb_hashmap_t<unsigned, സ്ഥിരം Feature*> *feature_substitutes_map,
                         സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         സ്ഥിരം hb_set_t *catch_all_features,
                         തർക്കം all_axes_pinned HB_UNUSED) സ്ഥിരം
  {
    unsigned എണ്ണം = get_feature_എണ്ണം ();
    വേണ്ടി (unsigned i = 0 ; i < എണ്ണം; i++)
    {
      എങ്കിൽ (!feature_index_map->has (i)) തുടരുക;
      hb_tag_t tag = get_feature_tag (i);
      സ്ഥിരം Feature *feature = &get_feature (i);
      സ്ഥിരം Feature **substitute;
      എങ്കിൽ (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        എങ്കിൽ (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    എങ്കിൽ (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [ഇത്] (unsigned i) { തിരികെ get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = സത്യം;
      ഇത്->table = sc.reference_table<T> (face);

      എങ്കിൽ (unlikely (ഇത്->table->is_blocklisted (ഇത്->table.get_blob (), face)))
      {
	hb_blob_destroy (ഇത്->table.get_blob ());
	ഇത്->table = hb_blob_get_empty ();
      }

      ഇത്->lookup_എണ്ണം = table->get_lookup_എണ്ണം ();

      ഇത്->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (ഇത്->lookup_എണ്ണം, വലുപ്പം (*accels));
      എങ്കിൽ (unlikely (!ഇത്->accels))
      {
	ഇത്->lookup_എണ്ണം = 0;
	ഇത്->table.destroy ();
	ഇത്->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ഇത്->lookup_എണ്ണം; i++)
      {
	auto *accel = ഇത്->accels[i].get_relaxed ();
	എങ്കിൽ (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (ഇത്->accels);
      ഇത്->table.destroy ();
    }

    hb_blob_t *get_blob () സ്ഥിരം { തിരികെ table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (lookup_index >= lookup_എണ്ണം)) തിരികെ nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      എങ്കിൽ (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	എങ്കിൽ (unlikely (!accel))
	  തിരികെ nullptr;

	എങ്കിൽ (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  പോകുക retry;
	}
      }

      തിരികെ accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned പൂർണ്ണസംഖ്യ lookup_എണ്ണം;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  സംരക്ഷിത:
  template <typename TLookup>
  സ്ഥിരം Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  സ്ഥിരം Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  സ്ഥിരം Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { തിരികെ reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  സ്ഥിരം Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  സ്ഥിരം Offset16To<RecordListOfFeature>& subset_feature_list_offset () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { തിരികെ reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  സ്ഥിരം Offset32To<RecordListOfFeature>& subset_feature_list2_offset () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  സ്ഥിരം ശൂന്യം* get_lookup_list_field_offset () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.to_int () >= 0x00010002u && lookupList2) തിരികെ &lookupList2;
#endif
    തിരികെ &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  സ്ഥിതം തർക്കം serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       സ്ഥിരം OffsetIn& in,
				       സ്ഥിരം Base *base,
				       Ts&&... ds)
  {
    out = 0;
    എങ്കിൽ (in.is_null ()) തിരികെ അസത്യം;

    auto *s = c->serializer;
    s->push ();
    തർക്കം ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    എങ്കിൽ (ret) s->add_link (out, s->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ s->എടുക്കുക_discard ();
    തിരികെ ret;
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
  പൊതു:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
