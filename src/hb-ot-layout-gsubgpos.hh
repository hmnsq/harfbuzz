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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-buffer.hh"
#અન્તર્ભાવ "hb-map.hh"
#અન્તર્ભાવ "hb-set.hh"
#અન્તર્ભાવ "hb-ot-map.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-ot-layout-gdef-table.hh"
#અન્તર્ભાવ "hb-depend-data.hh"


નામાકાશ OT {


સંરચના hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, તાર્કિક>
{
  template <typename T>
  return_t dispatch (અચલ T &obj) { પાછા obj.intersects (આ->glyphs); }
  સ્થિર return_t default_return_કિંમત () { પાછા ખોટું; }
  તાર્કિક stop_sublookup_iteration (return_t r) અચલ { પાછા r; }

  અચલ hb_set_t *glyphs;

  hb_intersects_context_t (અચલ hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

સંરચના hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, તાર્કિક>
{
  template <typename T>
  return_t dispatch (અચલ T &obj) { પાછા obj.may_have_non_1to1 (); }
  સ્થિર return_t default_return_કિંમત () { પાછા ખોટું; }
  તાર્કિક stop_sublookup_iteration (return_t r) અચલ { પાછા r; }
};

સંરચના hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  પ્રકારવ્યાખ્યા return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (અચલ T &obj) { obj.closure (આ); પાછા hb_empty_t (); }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }
  શૂન્ય recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    જો (unlikely (nesting_level_ડાબું == 0 || !recurse_func))
      પાછા;

    nesting_level_ડાબું--;
    recurse_func (આ, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_ડાબું++;
  }

  શૂન્ય reset_lookup_visit_ગણતરી ()
  { lookup_ગણતરી = 0; }

  તાર્કિક lookup_limit_exceeded ()
  { પાછા lookup_ગણતરી > HB_MAX_LOOKUP_VISIT_ગણતરી; }

  તાર્કિક should_visit_lookup (unsigned પૂર્ણાંક lookup_index)
  {
    જો (lookup_ગણતરી++ > HB_MAX_LOOKUP_VISIT_ગણતરી)
      પાછા ખોટું;

    જો (is_lookup_done (lookup_index))
      પાછા ખોટું;

    પાછા સાચું;
  }

  તાર્કિક is_lookup_done (unsigned પૂર્ણાંક lookup_index)
  {
    જો (unlikely (done_lookups_glyph_ગણતરી->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      પાછા સાચું;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned કાઢો = glyphs->get_population ();
    જો (unlikely (કાઢો == HB_MAP_કિંમત_INVALID)) // Avoid sentinel interference.
      કાઢો--;
    જો (done_lookups_glyph_ગણતરી->get (lookup_index) != કાઢો)
    {
      done_lookups_glyph_ગણતરી->set (lookup_index, કાઢો);

      જો (!done_lookups_glyph_set->has (lookup_index))
      {
	જો (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  પાછા સાચું;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    જો (unlikely (covered_glyph_set->in_error ()))
      પાછા સાચું;
    જો (parent_active_glyphs ().is_subset (*covered_glyph_set))
      પાછા સાચું;

    covered_glyph_set->union_ (parent_active_glyphs ());
    પાછા ખોટું;
  }

  અચલ hb_set_t& previous_parent_active_glyphs () {
    જો (active_glyphs_ઢગલો.length <= 1)
      પાછા *glyphs;

    પાછા active_glyphs_ઢગલો[active_glyphs_ઢગલો.length - 2];
  }

  અચલ hb_set_t& parent_active_glyphs ()
  {
    જો (!active_glyphs_ઢગલો)
      પાછા *glyphs;

    પાછા active_glyphs_ઢગલો.tail ();
  }

  hb_set_t* ધકેલો_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_ઢગલો.push ();
    જો (unlikely (active_glyphs_ઢગલો.in_error ()))
      પાછા nullptr;
    પાછા s;
  }

  તાર્કિક કાઢો_cur_done_glyphs ()
  {
    જો (!active_glyphs_ઢગલો)
      પાછા ખોટું;

    active_glyphs_ઢગલો.કાઢો ();
    પાછા સાચું;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_ઢગલો;
  recurse_func_t recurse_func = nullptr;
  unsigned પૂર્ણાંક nesting_level_ડાબું;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_ગણતરી_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned પૂર્ણાંક nesting_level_ડાબું_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_ડાબું (nesting_level_ડાબું_2),
			  done_lookups_glyph_ગણતરી (done_lookups_glyph_ગણતરી_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  શૂન્ય set_recurse_func (recurse_func_t func) { recurse_func = func; }

  શૂન્ય flush ()
  {
    output->del_range (face->get_અંક_glyphs (), HB_SET_કિંમત_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_ઢગલો.કાઢો ();
    active_glyphs_ઢગલો.reset ();
  }

  ખાનગી:
  hb_map_t *done_lookups_glyph_ગણતરી;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned પૂર્ણાંક lookup_ગણતરી = 0;
};



સંરચના hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  પ્રકારવ્યાખ્યા return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (અચલ T &obj) { obj.closure_lookups (આ); પાછા hb_empty_t (); }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }
  શૂન્ય recurse (unsigned lookup_index)
  {
    જો (unlikely (nesting_level_ડાબું == 0 || !recurse_func))
      પાછા;

    /* Return if new lookup was recursed to before. */
    જો (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      પાછા;

    nesting_level_ડાબું--;
    recurse_func (આ, lookup_index);
    nesting_level_ડાબું++;
  }

  શૂન્ય set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  શૂન્ય set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  તાર્કિક lookup_limit_exceeded ()
  {
    તાર્કિક ret = lookup_ગણતરી > HB_MAX_LOOKUP_VISIT_ગણતરી;
    જો (ret)
      DEBUG_સંદેશ (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    પાછા ret; }

  તાર્કિક is_lookup_visited (unsigned lookup_index)
  {
    જો (unlikely (lookup_ગણતરી++ > HB_MAX_LOOKUP_VISIT_ગણતરી))
    {
      DEBUG_સંદેશ (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_ગણતરી, lookup_index);
      પાછા સાચું;
    }

    જો (unlikely (visited_lookups->in_error ()))
      પાછા સાચું;

    પાછા visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  અચલ hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned પૂર્ણાંક nesting_level_ડાબું;

  hb_closure_lookups_context_t (hb_face_t *face_,
				અચલ hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_ડાબું_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_ડાબું (nesting_level_ડાબું_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_ગણતરી (0) {}

  શૂન્ય set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ખાનગી:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned પૂર્ણાંક lookup_ગણતરી;
};

સંરચના hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, તાર્કિક>
{
  template <typename T>
  return_t dispatch (અચલ T &obj) { પાછા obj.would_apply (આ); }
  સ્થિર return_t default_return_કિંમત () { પાછા ખોટું; }
  તાર્કિક stop_sublookup_iteration (return_t r) અચલ { પાછા r; }

  hb_face_t *face;
  અચલ hb_codepoint_t *glyphs;
  unsigned પૂર્ણાંક len;
  તાર્કિક zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    અચલ hb_codepoint_t *glyphs_,
			    unsigned પૂર્ણાંક len_,
			    તાર્કિક zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

સંરચના hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  પ્રકારવ્યાખ્યા return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned પૂર્ણાંક lookup_index);
  template <typename T>
  return_t dispatch (અચલ T &obj) { obj.collect_glyphs (આ); પાછા hb_empty_t (); }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }
  શૂન્ય recurse (unsigned પૂર્ણાંક lookup_index)
  {
    જો (unlikely (nesting_level_ડાબું == 0 || !recurse_func))
      પાછા;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    જો (output == hb_set_get_empty ())
      પાછા;

    /* Return if new lookup was recursed to before. */
    જો (recursed_lookups->has (lookup_index))
      પાછા;

    hb_set_t *old_before = before;
    hb_set_t *old_આગત  = આગત;
    hb_set_t *old_after  = after;
    before = આગત = after = hb_set_get_empty ();

    nesting_level_ડાબું--;
    recurse_func (આ, lookup_index);
    nesting_level_ડાબું++;

    before = old_before;
    આગત  = old_આગત;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *આગત;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned પૂર્ણાંક nesting_level_ડાબું;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_આગત,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned પૂર્ણાંક nesting_level_ડાબું_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      આગત  (glyphs_આગત  ? glyphs_આગત  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_ડાબું (nesting_level_ડાબું_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  શૂન્ય set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

સંરચના hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  ગણના glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  સ્થિર constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  સંરચના glyph_set_cache_ચાવી_t
  {
    glyph_set_cache_ચાવી_t () = મૂળભૂત;
    glyph_set_cache_ચાવી_t (અચલ શૂન્ય *data_, unsigned કિંમત_3, glyph_set_type_t type_)
      : data (data_), value (કિંમત_3), type (type_) {}

    તાર્કિક operator == (અચલ glyph_set_cache_ચાવી_t &o) અચલ
    { પાછા data == o.data && value == o.value && type == o.type; }

    uint32_t hash () અચલ
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      પાછા current * 31 + hb_hash ((unsigned) type);
    }

    અચલ શૂન્ય *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  સંરચના context_set_cache_ચાવી_t
  {
    context_set_cache_ચાવી_t () = મૂળભૂત;
    context_set_cache_ચાવી_t (અચલ શૂન્ય *rule_,
			     અચલ શૂન્ય *record_,
			     અચલ શૂન્ય *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned કિંમત_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (કિંમત_3), format (format_) {}

    તાર્કિક operator == (અચલ context_set_cache_ચાવી_t &o) અચલ
    {
      પાછા rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () અચલ
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      પાછા current * 31 + hb_hash (format);
    }

    અચલ શૂન્ય *rule = nullptr;
    અચલ શૂન્ય *record = nullptr;
    અચલ શૂન્ય *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  સંરચના recurse_ચાવી_t
  {
    recurse_ચાવી_t () = મૂળભૂત;
    recurse_ચાવી_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    તાર્કિક operator == (અચલ recurse_ચાવી_t &o) અચલ
    {
      પાછા lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () અચલ
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      પાછા current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  પ્રકારવ્યાખ્યા return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (અચલ T &obj) { obj.depend (આ); પાછા hb_empty_t (); }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  શૂન્ય recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    જો (lookups_seen.has (lookup_idx)) પાછા;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_ચાવી = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    જો (!recurse_path_to_index.has (path_ચાવી, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      જો (unlikely (!recurse_path_to_index.set (path_ચાવી, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	પાછા;
      }
    }
    નહીંતર
      path_index = *path;
    recurse_path = path_index;
    recurse_func (આ, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  સંરચના active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    તાર્કિક use_મૂળ = ખોટું;
  };

  અચલ hb_set_t& parent_active_glyphs ()
  {
    જો (!active_glyphs_ઢગલો_depth)
      પાછા *glyphs;

    અચલ active_glyphs_frame_t &frame = active_glyphs_ઢગલો[active_glyphs_ઢગલો_depth - 1];
    પાછા frame.use_મૂળ ? *glyphs : frame.glyphs;
  }

  hb_set_t* ધકેલો_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    જો (active_glyphs_ઢગલો_depth == active_glyphs_ઢગલો.length &&
	unlikely (!active_glyphs_ઢગલો.ધકેલો_or_fail ()))
      પાછા nullptr;

    active_glyphs_frame_t &frame = active_glyphs_ઢગલો[active_glyphs_ઢગલો_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_મૂળ = ખોટું;
    પાછા &frame.glyphs;
  }

  તાર્કિક ધકેલો_મૂળ_active_glyphs ()
  {
    જો (active_glyphs_ઢગલો_depth == active_glyphs_ઢગલો.length &&
	unlikely (!active_glyphs_ઢગલો.ધકેલો_or_fail ()))
      પાછા ખોટું;

    active_glyphs_ઢગલો[active_glyphs_ઢગલો_depth++].use_મૂળ = સાચું;
    પાછા સાચું;
  }

  તાર્કિક કાઢો_cur_done_glyphs ()
  {
    જો (!active_glyphs_ઢગલો_depth)
      પાછા ખોટું;

    active_glyphs_ઢગલો_depth--;
    પાછા સાચું;
  }

  તાર્કિક get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    જો (!active_glyphs_ઢગલો_depth ||
	active_glyphs_ઢગલો[active_glyphs_ઢગલો_depth - 1].use_મૂળ)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      પાછા સાચું;
    }

    active_glyphs_frame_t &frame = active_glyphs_ઢગલો[active_glyphs_ઢગલો_depth - 1];
    *active_idx = frame.active_idx;
    જો (*active_idx != HB_CODEPOINT_INVALID)
      પાછા સાચું;

    hb_codepoint_t glyph;
    જો (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    નહીંતર જો (!find_or_create_recurse_set (frame.glyphs, active_idx))
      પાછા ખોટું;

    frame.active_idx = *active_idx;
    પાછા સાચું;
  }

  તાર્કિક get_context_set_cache_ચાવી (અચલ શૂન્ય *rule,
				  અચલ શૂન્ય *record,
				  અચલ શૂન્ય *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_ચાવી_t *key)
  {
    hb_codepoint_t active_idx;
    જો (!get_parent_active_glyphs_index (&active_idx))
      પાછા ખોટું;
    *key = context_set_cache_ચાવી_t (rule, record, data, active_idx, value, format);
    પાછા સાચું;
  }

  તાર્કિક get_recurse_ચાવી (unsigned lookup_idx, recurse_ચાવી_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    જો (!get_parent_active_glyphs_index (&active_idx))
      પાછા ખોટું;

    *key = recurse_ચાવી_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    પાછા !completed_recursions.has (*key);
  }

  શૂન્ય finish_recurse (અચલ recurse_ચાવી_t &key)
  { depend_data->check_success (completed_recursions.set (key, સાચું)); }

  શૂન્ય reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  તાર્કિક find_or_create_recurse_set (અચલ hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    જો (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      પાછા સાચું;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    જો (unlikely (!copy))
      પાછા depend_data->fail ();
    copy->set (set);
    જો (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      પાછા depend_data->fail ();
    }

    *index = recurse_sets.length;
    જો (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      પાછા depend_data->fail ();
    }
    જો (unlikely (!recurse_sets.ધકેલો_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      પાછા depend_data->fail ();
    જો (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      પાછા depend_data->fail ();
    પાછા સાચું;
  }

  અચલ hb_set_t *find_or_create_glyph_set (અચલ શૂન્ય *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (અચલ hb_set_t *set);

  સંરચના depend_scratch_t
  {
    શૂન્ય clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      આગત_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<અચલ hb_set_t *> backtrack_sets;
    hb_vector_t<અચલ hb_set_t *> lookahead_sets;
    hb_vector_t<અચલ hb_set_t *> આગત_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    જો (scratch)
      depend_scratch_pool = scratch->next;
    નહીંતર
    {
      scratch = (depend_scratch_t *) hb_malloc (કદ (depend_scratch_t));
      જો (unlikely (!scratch))
      {
        depend_data->fail ();
        પાછા nullptr;
      }
      નવું (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    પાછા scratch;
  }

  શૂન્ય release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_ઢગલો;
  unsigned active_glyphs_ઢગલો_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<અચલ hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_ચાવી_t, તાર્કિક> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_ચાવી_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_ચાવી_t, hb_codepoint_t> context_set_cache;
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
    જ્યારે (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  શૂન્ય set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
સંરચના hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, અચલ Coverage &>
{
  પ્રકારવ્યાખ્યા અચલ Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (અચલ T &obj) { પાછા obj.get_coverage (); }
  સ્થિર return_t default_return_કિંમત () { પાછા Null (Coverage); }
  તાર્કિક stop_sublookup_iteration (return_t r) અચલ
  {
    r.collect_coverage (set);
    પાછા ખોટું;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

સંરચના matcher_t
{
  પ્રકારવ્યાખ્યા તાર્કિક (*match_func_t) (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data);

  template <typename context_t>
  શૂન્ય init (અચલ context_t *c, તાર્કિક context_match = ખોટું)
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

  શૂન્ય set_match_func (match_func_t match_func_,
		       અચલ શૂન્ય *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  ગણના may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) અચલ
  {
    જો (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      પાછા MATCH_NO;

    જો (match_func)
      પાછા match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    પાછા MATCH_MAYBE;
  }

  ગણના may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (અચલ context_t *c,
		       અચલ hb_glyph_info_t &info) અચલ
  {
    જો (!c->check_glyph_property (&info, lookup_props))
      પાછા SKIP_YES;

    જો (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      પાછા SKIP_MAYBE;

    પાછા SKIP_NO;
  }

  જાહેર:
  unsigned પૂર્ણાંક lookup_props = 0;
  hb_mask_t mask = -1;
  તાર્કિક ignore_zwnj = ખોટું;
  તાર્કિક ignore_zwj = ખોટું;
  તાર્કિક ignore_hidden = ખોટું;
  તાર્કિક per_syllable = ખોટું;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  અચલ શૂન્ય *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
સંરચના glyph_data_t
{
  template <typename T>
  glyph_data_t (અચલ T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (કદ (T) == 3)
#endif
  {
    static_assert (કદ (T) == 2 || કદ (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (wide)
      પાછા ((અચલ HBUINT24 *) data)[i];
#endif
    પાછા ((અચલ HBUINT16 *) data)[i];
  }

  અચલ શૂન્ય *data;
#ifndef HB_NO_BEYOND_64K
  તાર્કિક wide;
#endif
};

template <typename context_t>
સંરચના skipping_iterator_t
{
  શૂન્ય init (context_t *c_, તાર્કિક context_match = ખોટું)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  શૂન્ય set_lookup_props (unsigned પૂર્ણાંક lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  શૂન્ય set_match_func (matcher_t::match_func_t match_func_,
		       અચલ શૂન્ય *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  શૂન્ય set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (અચલ HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (અચલ HBUINT24 *) glyph_data.data : nullptr;
#નહીંતર
    match_glyph_data16 = (અચલ HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  શૂન્ય reset (unsigned પૂર્ણાંક start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  શૂન્ય reset_back (unsigned પૂર્ણાંક start_index_, તાર્કિક from_out_buffer = ખોટું)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  શૂન્ય reset_fast (unsigned પૂર્ણાંક start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (અચલ hb_glyph_info_t &info) અચલ
  { પાછા matcher.may_skip (c, info); }

  ગણના match_t {
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
    જો (unlikely (skip == matcher_t::SKIP_YES))
      પાછા SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    જો (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      પાછા MATCH;

    જો (skip == matcher_t::SKIP_NO)
      પાછા NOT_MATCH;

    પાછા SKIP;
  }

  તાર્કિક next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    અચલ signed stop = (signed) end - 1;
    જ્યારે ((signed) idx < stop)
    {
      idx++;
      પસંદગી (match (info[idx]))
      {
	સ્થિતિ MATCH:
	{
	  advance_glyph_data ();
	  પાછા સાચું;
	}
	સ્થિતિ NOT_MATCH:
	{
	  જો (unsafe_to)
	    *unsafe_to = idx + 1;
	  પાછા ખોટું;
	}
	સ્થિતિ SKIP:
	  ચાલુ;
      }
    }
    જો (unsafe_to)
      *unsafe_to = end;
    પાછા ખોટું;
  }
  તાર્કિક prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    અચલ unsigned stop = 0;
    જ્યારે (idx > stop)
    {
      idx--;
      પસંદગી (match (out_info[idx]))
      {
	સ્થિતિ MATCH:
	{
	  advance_glyph_data ();
	  પાછા સાચું;
	}
	સ્થિતિ NOT_MATCH:
	{
	  જો (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  પાછા ખોટું;
	}
	સ્થિતિ SKIP:
	  ચાલુ;
      }
    }
    જો (unsafe_from)
      *unsafe_from = 0;
    પાછા ખોટું;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    જો (match_glyph_data16) પાછા *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    નહીંતર
    જો (match_glyph_data24) પાછા *match_glyph_data24;
#endif
    પાછા 0;
  }
  HB_ALWAYS_INLINE
  શૂન્ય
  advance_glyph_data ()
  {
    જો (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    નહીંતર
    જો (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned પૂર્ણાંક idx;
  સુરક્ષિત:
  context_t *c;
  matcher_t matcher;
  અચલ HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  અચલ HBUINT24 *match_glyph_data24;
#endif

  unsigned પૂર્ણાંક end;
};

સંરચના hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, તાર્કિક, HB_DEBUG_APPLY>
{
  અચલ અક્ષર *get_name () { પાછા "APPLY"; }
  પ્રકારવ્યાખ્યા return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned પૂર્ણાંક lookup_index);

  template <typename T>
  સ્થિર અંતઃરેખ auto apply_ (અચલ T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  સ્થિર અંતઃરેખ auto apply_ (અચલ T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (અચલ T &obj) { પાછા apply_(obj, આ, hb_prioritize); }

  સ્થિર return_t default_return_કિંમત () { પાછા ખોટું; }
  તાર્કિક stop_sublookup_iteration (return_t r) અચલ { પાછા r; }
  return_t recurse (unsigned પૂર્ણાંક sub_lookup_index)
  {
    પ્રતિજ્ઞા (recurse_func);
    જો (unlikely (nesting_level_ડાબું == 0))
    {
      buffer->successful = ખોટું;
      પાછા default_return_કિંમત ();
    }

    buffer->max_ops--;
    જો (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = ખોટું;
      પાછા default_return_કિંમત ();
    }

    nesting_level_ડાબું--;
    તાર્કિક ret = recurse_func (આ, sub_lookup_index);
    nesting_level_ડાબું++;
    પાછા ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_આગત, iter_context;

  unsigned પૂર્ણાંક table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  અચલ GDEF &gdef;
  અચલ GDEF::accelerator_t &gdef_accel;
  અચલ ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned પૂર્ણાંક lookup_index = (unsigned) -1;
  unsigned પૂર્ણાંક lookup_props = 0;
  unsigned પૂર્ણાંક nesting_level_ડાબું = HB_MAX_NESTING_LEVEL;

  તાર્કિક has_glyph_classes;
  તાર્કિક auto_zwnj = સાચું;
  તાર્કિક auto_zwj = સાચું;
  તાર્કિક per_syllable = ખોટું;
  તાર્કિક random = ખોટું;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t ઢગલો_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  શૂન્ય *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned પૂર્ણાંક table_index_,
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
#નહીંતર
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#નહીંતર
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (ઢગલો_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    જો (buffer->messaging ())
    {
      પ્રતિજ્ઞા (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = આ;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    જો (buffer->messaging ())
    {
      પ્રતિજ્ઞા (buffer->changed_func == buffer_changed_trampoline);
      પ્રતિજ્ઞા (buffer->changed_data == આ);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  શૂન્ય init_iters ()
  {
    iter_આગત.init (આ, ખોટું);
    iter_context.init (આ, સાચું);
  }

  શૂન્ય set_lookup_mask (hb_mask_t mask, તાર્કિક init = સાચું) { lookup_mask = mask; last_base = -1; last_base_until = 0; જો (init) init_iters (); }
  શૂન્ય set_auto_zwj (તાર્કિક auto_zwj_, તાર્કિક init = સાચું) { auto_zwj = auto_zwj_; જો (init) init_iters (); }
  શૂન્ય set_auto_zwnj (તાર્કિક auto_zwnj_, તાર્કિક init = સાચું) { auto_zwnj = auto_zwnj_; જો (init) init_iters (); }
  શૂન્ય set_per_syllable (તાર્કિક per_syllable_, તાર્કિક init = સાચું) { per_syllable = per_syllable_; જો (init) init_iters (); }
  શૂન્ય set_random (તાર્કિક random_) { random = random_; }
  શૂન્ય set_recurse_func (recurse_func_t func) { recurse_func = func; }
  શૂન્ય set_lookup_index (unsigned પૂર્ણાંક lookup_index_) { lookup_index = lookup_index_; }
  શૂન્ય set_lookup_props (unsigned પૂર્ણાંક lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_સંખ્યા ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    પાછા buffer->random_state;
  }

  સ્થિર શૂન્ય buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, શૂન્ય *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  શૂન્ય buffer_changed ()
  {
    buffer->update_digest ();
    જો (likely (has_glyph_classes))
      માટે (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  તાર્કિક match_properties_mark (અચલ hb_glyph_info_t *info,
			      unsigned પૂર્ણાંક    glyph_props,
			      unsigned પૂર્ણાંક    match_props) અચલ
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    જો (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      પાછા gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    જો (match_props & LookupFlag::MarkAttachmentType)
      પાછા (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    પાછા સાચું;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક check_glyph_property (અચલ hb_glyph_info_t *info,
			     unsigned match_props) અચલ
  {
    unsigned પૂર્ણાંક glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    જો (glyph_props & match_props & LookupFlag::IgnoreFlags)
      પાછા ખોટું;

    જો (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      પાછા match_properties_mark (info, glyph_props, match_props);

    પાછા સાચું;
  }

  શૂન્ય _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned પૂર્ણાંક class_guess = 0,
			  તાર્કિક ligature = ખોટું,
			  તાર્કિક component = ખોટું)
  {
    buffer->digest.add (glyph_index);

    જો (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned પૂર્ણાંક props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    જો (ligature)
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
    જો (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    જો (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    નહીંતર જો (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    નહીંતર
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  શૂન્ય _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned પૂર્ણાંક props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  શૂન્ય _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  શૂન્ય replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (શૂન્ય) buffer->replace_glyph (glyph_index);
  }
  શૂન્ય replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  શૂન્ય replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned પૂર્ણાંક class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, સાચું);
    (શૂન્ય) buffer->replace_glyph (glyph_index);
  }
  શૂન્ય output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned પૂર્ણાંક class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, ખોટું, સાચું);
    (શૂન્ય) buffer->output_glyph (glyph_index);
  }
};

ગણના વર્ગ hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

સંરચના hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  સ્થિર અંતઃરેખ auto collect_second_glyphs_ (અચલ T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (શૂન્ય, obj.collect_second_glyphs (digest) )
  template <typename T>
  સ્થિર અંતઃરેખ શૂન્ય collect_second_glyphs_ (અચલ T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  સ્થિર અંતઃરેખ auto apply_ (અચલ T *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache, hb_priority<1>) HB_RETURN (તાર્કિક, obj->apply (c, external_cache) )
  template <typename T>
  સ્થિર અંતઃરેખ auto apply_ (અચલ T *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache, hb_priority<0>) HB_RETURN (તાર્કિક, obj->apply (c) )
  template <typename T>
  સ્થિર અંતઃરેખ તાર્કિક apply_to (અચલ શૂન્ય *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache)
  {
    અચલ T *typed_obj = (અચલ T *) obj;
    પાછા apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  સ્થિર અંતઃરેખ auto apply_cached_ (અચલ T *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache, hb_priority<2>) HB_RETURN (તાર્કિક, obj->apply_cached (c, external_cache) )
  template <typename T>
  સ્થિર અંતઃરેખ auto apply_cached_ (અચલ T *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache, hb_priority<1>) HB_RETURN (તાર્કિક, obj->apply (c, external_cache) )
  template <typename T>
  સ્થિર અંતઃરેખ auto apply_cached_ (અચલ T *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache, hb_priority<0>) HB_RETURN (તાર્કિક, obj->apply (c) )
  template <typename T>
  સ્થિર અંતઃરેખ તાર્કિક apply_cached_to (અચલ શૂન્ય *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache)
  {
    અચલ T *typed_obj = (અચલ T *) obj;
    પાછા apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  સ્થિર અંતઃરેખ auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (તાર્કિક, T::cache_func (c, op) )
  template <typename T=શૂન્ય>
  સ્થિર અંતઃરેખ તાર્કિક cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { પાછા ખોટું; }
  template <typename Type>
  સ્થિર અંતઃરેખ તાર્કિક cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    પાછા cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  પ્રકારવ્યાખ્યા તાર્કિક (*hb_apply_func_t) (અચલ શૂન્ય *obj, hb_ot_apply_context_t *c, શૂન્ય *external_cache);
  પ્રકારવ્યાખ્યા તાર્કિક (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  સંરચના hb_applicable_t
  {
    friend સંરચના hb_accelerate_subtables_context_t;
    friend સંરચના hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    શૂન્ય init (અચલ T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , શૂન્ય *external_cache_
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
    તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
    {
      પાછા digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    તાર્કિક apply_no_digest (hb_ot_apply_context_t *c) અચલ
    {
      પાછા apply_func (obj, c, nullptr);
    }
#નહીંતર
    તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
    {
      પાછા digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    તાર્કિક apply_cached (hb_ot_apply_context_t *c) અચલ
    {
      પાછા digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    તાર્કિક apply_no_digest (hb_ot_apply_context_t *c) અચલ
    {
      પાછા apply_func (obj, c, external_cache);
    }
    તાર્કિક apply_cached_no_digest (hb_ot_apply_context_t *c) અચલ
    {
      પાછા apply_cached_func (obj, c, external_cache);
    }

    તાર્કિક cache_enter (hb_ot_apply_context_t *c) અચલ
    {
      પાછા cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    શૂન્ય cache_leave (hb_ot_apply_context_t *c) અચલ
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    ખાનગી:
    અચલ શૂન્ય *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    શૂન્ય *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (અચલ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (અચલ T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (અચલ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (અચલ T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (અચલ T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    શૂન્ય *external_cache = nullptr;
    જો (i < 8)
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
    જો (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    પાછા hb_empty_t ();
  }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

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


પ્રકારવ્યાખ્યા તાર્કિક (*intersects_func_t) (અચલ hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data, શૂન્ય *cache);
પ્રકારવ્યાખ્યા શૂન્ય (*intersected_glyphs_func_t) (અચલ hb_set_t *glyphs, અચલ શૂન્ય *data, unsigned value, hb_set_t *intersected_glyphs, શૂન્ય *cache);
પ્રકારવ્યાખ્યા શૂન્ય (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data);
પ્રકારવ્યાખ્યા તાર્કિક (*match_func_t) (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data);
પ્રકારવ્યાખ્યા unsigned (*get_કિંમત_func_t) (hb_glyph_info_t &info, અચલ શૂન્ય *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
સંરચના hb_ot_layout_ruleset_digest_t
{
  શૂન્ય init () { mask = 0; }
  શૂન્ય set_full () { mask = (uint64_t) -1; }
  શૂન્ય add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  તાર્કિક may_have (unsigned value) અચલ { પાછા mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

સંરચના ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
સંરચના ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
સંરચના ContextApplyFuncs
{
  match_func_t match;
};
સંરચના ChainContextApplyFuncs
{
  match_func_t match[3];
};


સ્થિર અંતઃરેખ તાર્કિક intersects_glyph (અચલ hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data HB_UNUSED, શૂન્ય *cache HB_UNUSED)
{
  પાછા glyphs->has (value);
}
સ્થિર અંતઃરેખ તાર્કિક intersects_class (અચલ hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data, શૂન્ય *cache)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  જો (map->has (value, &cached_v))
    પાછા *cached_v;

  તાર્કિક v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  પાછા v;
}
સ્થિર અંતઃરેખ તાર્કિક intersects_coverage (અચલ hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data, શૂન્ય *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  પાછા (data+coverage).intersects (glyphs);
}


સ્થિર અંતઃરેખ શૂન્ય intersected_glyph (અચલ hb_set_t *glyphs HB_UNUSED, અચલ શૂન્ય *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED શૂન્ય *cache)
{
  unsigned g = reinterpret_cast<અચલ HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

સ્થિર અંતઃરેખ શૂન્ય intersected_class_glyphs (અચલ hb_set_t *glyphs, અચલ શૂન્ય *data, unsigned value, hb_set_t *intersected_glyphs, શૂન્ય *cache)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  જો (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    પાછા;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

સ્થિર અંતઃરેખ શૂન્ય intersected_coverage_glyphs (અચલ hb_set_t *glyphs, અચલ શૂન્ય *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED શૂન્ય *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


સ્થિર અંતઃરેખ તાર્કિક array_is_subset_of (અચલ hb_set_t *glyphs,
				       unsigned પૂર્ણાંક ગણતરી,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       અચલ શૂન્ય *intersects_data,
				       શૂન્ય *cache)
{
  માટે (unsigned i = 0; i < ગણતરી; i++)
    જો (!intersects_func (glyphs, values[i], intersects_data, cache)) પાછા ખોટું;
  પાછા સાચું;
}


સ્થિર અંતઃરેખ શૂન્ય collect_glyph (hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data HB_UNUSED)
{
  glyphs->add (value);
}
સ્થિર અંતઃરેખ શૂન્ય collect_class (hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
સ્થિર અંતઃરેખ શૂન્ય collect_coverage (hb_set_t *glyphs, unsigned value, અચલ શૂન્ય *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

અંતઃરેખ અચલ hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (અચલ શૂન્ય *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_ચાવી_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  જો (glyph_set_to_index.has (key, &existing))
    પાછા glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  જો (unlikely (!set))
  {
    depend_data->fail ();
    પાછા nullptr;
  }

  જો (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  નહીંતર જો (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  નહીંતર
    set->add (value);
  જો (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    પાછા nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  જો (unlikely (!glyph_sets.ધકેલો_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    પાછા nullptr;
  }
  જો (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    પાછા nullptr;
  }
  પાછા glyph_sets[index].get ();
}

અંતઃરેખ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (અચલ hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  જો (cached_context_set_indices.has (key, &existing))
    પાછા *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  જો (unlikely (index == HB_CODEPOINT_INVALID))
    પાછા HB_CODEPOINT_INVALID;
  જો (unlikely (!cached_context_set_indices.set (key, index)))
    પાછા depend_data->fail_invalid ();
  પાછા index;
}

સ્થિર અંતઃરેખ શૂન્ય collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned પૂર્ણાંક ગણતરી,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  અચલ શૂન્ય *collect_data)
{
  માટે (unsigned i = 0; i < ગણતરી; i++)
    collect_func (glyphs, values[i], collect_data);
}


સ્થિર અંતઃરેખ તાર્કિક match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, અચલ શૂન્ય *data HB_UNUSED)
{
  પાછા સાચું;
}
સ્થિર અંતઃરેખ તાર્કિક match_glyph (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data HB_UNUSED)
{
  પાછા info.codepoint == value;
}
સ્થિર અંતઃરેખ તાર્કિક match_class (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);
  પાછા class_def.get_class (info.codepoint) == value;
}
સ્થિર અંતઃરેખ unsigned get_class_કિંમત (hb_glyph_info_t &info, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *> (data);
  પાછા class_def.get_class (info.codepoint);
}
સંરચના match_class_cache_data_t
{
  અચલ ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
સ્થિર અંતઃરેખ તાર્કિક match_class_cache (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data)
{
  અચલ auto &cache_data = *reinterpret_cast<અચલ match_class_cache_data_t *> (data);
  પાછા cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
સ્થિર અંતઃરેખ unsigned get_class_cache_કિંમત (hb_glyph_info_t &info, અચલ શૂન્ય *data)
{
  અચલ auto &cache_data = *reinterpret_cast<અચલ match_class_cache_data_t *> (data);
  પાછા cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
સ્થિર અંતઃરેખ unsigned get_class_cached (અચલ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  જો (klass < 255)
    પાછા klass;
  klass = class_def.get_class (info.codepoint);
  જો (likely (klass < 255))
    info.syllable() = klass;
  પાછા klass;
}
સ્થિર અંતઃરેખ તાર્કિક match_class_cached (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);
  પાછા get_class_cached (class_def, info) == value;
}
સ્થિર અંતઃરેખ unsigned get_class_cached_કિંમત (hb_glyph_info_t &info, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *> (data);
  પાછા get_class_cached (class_def, info);
}
સ્થિર અંતઃરેખ unsigned get_class_cached1 (અચલ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  જો (klass < 15)
    પાછા klass;
  klass = class_def.get_class (info.codepoint);
  જો (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  પાછા klass;
}
સ્થિર અંતઃરેખ તાર્કિક match_class_cached1 (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);
  પાછા get_class_cached1 (class_def, info) == value;
}
સ્થિર અંતઃરેખ unsigned get_class_cached2 (અચલ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  જો (klass < 15)
    પાછા klass;
  klass = class_def.get_class (info.codepoint);
  જો (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  પાછા klass;
}
સ્થિર અંતઃરેખ તાર્કિક match_class_cached2 (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *>(data);
  પાછા get_class_cached2 (class_def, info) == value;
}
સ્થિર અંતઃરેખ unsigned get_class_cached2_કિંમત (hb_glyph_info_t &info, અચલ શૂન્ય *data)
{
  અચલ ClassDef &class_def = *reinterpret_cast<અચલ ClassDef *> (data);
  પાછા get_class_cached2 (class_def, info);
}
સ્થિર અંતઃરેખ તાર્કિક match_coverage (hb_glyph_info_t &info, unsigned value, અચલ શૂન્ય *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  પાછા (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

સ્થિર અંતઃરેખ તાર્કિક would_match_આગત (hb_would_apply_context_t *c,
				      unsigned પૂર્ણાંક ગણતરી, /* Including the first glyph (not matched) */
				      glyph_data_t આગત, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      અચલ શૂન્ય *match_data)
{
  જો (ગણતરી != c->len)
    પાછા ખોટું;

  માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    જો (likely (!match_func (info, આગત[i - 1], match_data)))
      પાછા ખોટું;
  }

  પાછા સાચું;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#નહીંતર
અંતઃરેખ
#endif
સ્થિર તાર્કિક match_આગત (hb_ot_apply_context_t *c,
			 unsigned પૂર્ણાંક ગણતરી, /* Including the first glyph (not matched) */
			 glyph_data_t આગત, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 અચલ શૂન્ય *match_data,
			 unsigned પૂર્ણાંક *end_position,
			 unsigned પૂર્ણાંક *p_total_component_ગણતરી = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  જો (ગણતરી == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    જો (p_total_component_ગણતરી)
      *p_total_component_ગણતરી = hb_glyph_info_get_lig_અંક_comps (&buffer->cur());
    return_trace (સાચું);
  }

  જો (unlikely (ગણતરી > HB_MAX_CONTEXT_LENGTH)) return_trace (ખોટું);

  auto &skippy_iter = c->iter_આગત;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (આગત);

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

  unsigned પૂર્ણાંક total_component_ગણતરી = 0;

  unsigned પૂર્ણાંક first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned પૂર્ણાંક first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  ગણના {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
  {
    unsigned unsafe_to;
    જો (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (ખોટું);
    }

    જો (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (ખોટું);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned પૂર્ણાંક this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned પૂર્ણાંક this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    જો (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      જો (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	જો (ligbase == LIGBASE_NOT_CHECKED)
	{
	  તાર્કિક found = ખોટું;
	  અચલ auto *out = buffer->out_info;
	  unsigned પૂર્ણાંક j = buffer->out_len;
	  જ્યારે (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    જો (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = સાચું;
	      તોડો;
	    }
	    j--;
	  }

	  જો (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  નહીંતર
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	જો (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (ખોટું);
      }
    }
    નહીંતર
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      જો (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (ખોટું);
    }

    total_component_ગણતરી += hb_glyph_info_get_lig_અંક_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  જો (p_total_component_ગણતરી)
  {
    total_component_ગણતરી += hb_glyph_info_get_lig_અંક_comps (&buffer->cur());
    *p_total_component_ગણતરી = total_component_ગણતરી;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (સાચું);
}
સ્થિર અંતઃરેખ તાર્કિક ligate_આગત (hb_ot_apply_context_t *c,
				 unsigned પૂર્ણાંક ગણતરી, /* Including the first glyph */
				 unsigned પૂર્ણાંક match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned પૂર્ણાંક total_component_ગણતરી)
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

  તાર્કિક is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  તાર્કિક is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
    જો (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = ખોટું;
      is_mark_ligature = ખોટું;
      તોડો;
    }
  તાર્કિક is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned પૂર્ણાંક klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned પૂર્ણાંક lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned પૂર્ણાંક last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned પૂર્ણાંક last_અંક_components = hb_glyph_info_get_lig_અંક_comps (&buffer->cur());
  unsigned પૂર્ણાંક components_so_far = last_અંક_components;

  જો (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_ગણતરી);
    જો (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
  {
    જ્યારે (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      જો (is_ligature)
      {
	unsigned પૂર્ણાંક this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	જો (this_comp == 0)
	  this_comp = last_અંક_components;
	પ્રતિજ્ઞા (components_so_far >= last_અંક_components);
	unsigned પૂર્ણાંક new_lig_comp = components_so_far - last_અંક_components +
				    hb_min (this_comp, last_અંક_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (શૂન્ય) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_અંક_components = hb_glyph_info_get_lig_અંક_comps_in_ligation (&buffer->cur());
    components_so_far += last_અંક_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  જો (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    માટે (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      જો (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) તોડો;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      જો (!this_comp) તોડો;

      પ્રતિજ્ઞા (components_so_far >= last_અંક_components);
      unsigned new_lig_comp = components_so_far - last_અંક_components +
			      hb_min (this_comp, last_અંક_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (સાચું);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#નહીંતર
અંતઃરેખ
#endif
સ્થિર તાર્કિક match_backtrack (hb_ot_apply_context_t *c,
			     unsigned પૂર્ણાંક ગણતરી,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     અચલ શૂન્ય *match_data,
			     unsigned પૂર્ણાંક *match_start)
{
  TRACE_APPLY (nullptr);

  જો (!ગણતરી)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (સાચું);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
  {
    unsigned unsafe_from;
    જો (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (ખોટું);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (સાચું);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#નહીંતર
અંતઃરેખ
#endif
સ્થિર તાર્કિક match_lookahead (hb_ot_apply_context_t *c,
			     unsigned પૂર્ણાંક ગણતરી,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     અચલ શૂન્ય *match_data,
			     unsigned પૂર્ણાંક start_index,
			     unsigned પૂર્ણાંક *end_index)
{
  TRACE_APPLY (nullptr);

  જો (!ગણતરી)
  {
    *end_index = start_index;
    return_trace (સાચું);
  }

  auto &skippy_iter = c->iter_context;
  પ્રતિજ્ઞા (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
  {
    unsigned unsafe_to;
    જો (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (ખોટું);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (સાચું);
}



સંરચના LookupRecord
{
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ hb_map_t         *lookup_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સ્થિર અંતઃરેખ unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     અચલ hb_array_t<અચલ LookupRecord> lookupRecords,
						     અચલ hb_map_t *lookup_map)
{
  unsigned ગણતરી = 0;
  માટે (અચલ LookupRecord& r : lookupRecords)
  {
    જો (!lookup_map->has (r.lookupListIndex))
      ચાલુ;

    જો (!r.serialize (c, lookup_map))
      પાછા 0;

    ગણતરી++;
  }
  પાછા ગણતરી;
}

ગણના ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

સ્થિર અંતઃરેખ અચલ hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			અચલ શૂન્ય *data,
			unsigned value,
			તાર્કિક filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  પસંદગી (context_format)
  {
    સ્થિતિ ContextFormat::SimpleContext:
      પાછા c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    સ્થિતિ ContextFormat::ClassBasedContext:
    {
      અચલ hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      જો (unlikely (!class_set))
	પાછા nullptr;
      જો (!filter_class_by_parent)
	પાછા class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      તોડો;
    }
    સ્થિતિ ContextFormat::CoverageBasedContext:
      પાછા c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  જો (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    પાછા nullptr;
  }
  પાછા scratch;
}

સ્થિર અંતઃરેખ શૂન્ય context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned આગત_ગણતરી, glyph_data_t આગત,
					     unsigned lookup_ગણતરી_2,
					     અચલ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     અચલ શૂન્ય *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     શૂન્ય *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  માટે (unsigned પૂર્ણાંક i = 0; i < lookup_ગણતરી_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    જો (seqIndex >= આગત_ગણતરી) ચાલુ;

    તાર્કિક has_pos_glyphs = ખોટું;

    જો (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = સાચું;
      pos_glyphs.clear ();
      જો (seqIndex == 0)
      {
        પસંદગી (context_format) {
        સ્થિતિ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          તોડો;
        સ્થિતિ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          તોડો;
        સ્થિતિ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          તોડો;
        }
      }
      નહીંતર
      {
        અચલ શૂન્ય *આગત_data = આગત.data;
        unsigned આગત_કિંમત = seqIndex - 1;
        જો (context_format != ContextFormat::SimpleContext)
        {
          આગત_data = data;
          આગત_કિંમત = આગત[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, આગત_data, આગત_કિંમત, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs))
      પાછા;
    જો (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } નહીંતર {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = આગત_ગણતરી;
    જો (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->કાઢો_cur_done_glyphs ();
  }
}

સ્થિર તાર્કિક
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned આગત_ગણતરી_2,
				unsigned lookup_ગણતરી,
				અચલ LookupRecord lookup_records[],
				અચલ શૂન્ય *rule,
				unsigned value,
				ContextFormat context_format,
				અચલ શૂન્ય *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  તાર્કિક have_active_idx = ખોટું;
  માટે (unsigned i = 0; i < lookup_ગણતરી; i++)
  {
    જો (lookup_records[i].sequenceIndex >= આગત_ગણતરી_2)
      ચાલુ;

    જો (!have_active_idx)
    {
      જો (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	પાછા ખોટું;
      have_active_idx = સાચું;
    }

    hb_depend_context_t::context_set_cache_ચાવી_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    જો (!c->context_set_cache.has (key, &cached_context_set))
      પાછા ખોટું;
    જો (unlikely (!cached_context_sets->ધકેલો_or_fail (*cached_context_set)))
      પાછા c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  પાછા have_active_idx;
}

સ્થિર અંતઃરેખ શૂન્ય context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned આગત_ગણતરી, glyph_data_t આગત,
					     unsigned lookup_ગણતરી_2,
					     અચલ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     અચલ શૂન્ય *rule,
					     unsigned value,
					     ContextFormat context_format,
					     અચલ શૂન્ય *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     શૂન્ય *cache,
					     અચલ hb_set_t &preliminary_context,
					     અચલ hb_vector_t<અચલ hb_set_t *> *આગત_position_glyphs,
					     અચલ hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  જો (unlikely (આગત_ગણતરી > HB_MAX_CONTEXT_LENGTH))
    પાછા;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < lookup_ગણતરી_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    જો (seqIndex >= આગત_ગણતરી)
      ચાલુ;

    hb_depend_context_t::context_set_cache_ચાવી_t context_cache_ચાવી;
    જો (unlikely (!c->get_context_set_cache_ચાવી (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_ચાવી)))
      પાછા;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    જો (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    નહીંતર જો (c->context_set_cache.has (context_cache_ચાવી, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    નહીંતર
    {
      જો (unlikely (!આગત_position_glyphs))
      {
	c->depend_data->fail ();
	પાછા;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_કિંમત_INVALID;
      જ્યારે (preliminary_context.next (&elem)) {
	જો (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} નહીંતર {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      માટે (unsigned j = 0; j < આગત_position_glyphs->length; j++) {
	જો (j != seqIndex && (*આગત_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*આગત_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_કિંમત_INVALID;
      જ્યારે (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	અચલ hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	જો (unlikely (!original_set)) ચાલુ;

	/* Dependency context sets are never inverted. */
	જો (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      માટે (unsigned j = 0; j < આગત_position_glyphs->length; j++) {
	જો (j != seqIndex && (*આગત_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  જો (!(*આગત_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    તાર્કિક cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*આગત_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*આગત_position_glyphs)[j]);
	    જો (unlikely (idx == HB_CODEPOINT_INVALID))
	      પાછા;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      જો (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	પાછા;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      જો (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	પાછા;

      જો (unlikely (!c->context_set_cache.set (context_cache_ચાવી, context_set_idx)))
      {
	c->depend_data->fail ();
	પાછા;
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
    તાર્કિક nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    જો (આગત_ગણતરી > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    જો (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    તાર્કિક has_pos_glyphs = ખોટું;

    જો (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = સાચું;
      pos_glyphs.clear ();
      જો (seqIndex == 0)
      {
        પસંદગી (context_format) {
        સ્થિતિ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          તોડો;
        સ્થિતિ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          તોડો;
        સ્થિતિ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          તોડો;
        }
      }
      નહીંતર
      {
        અચલ શૂન્ય *આગત_data = આગત.data;
        unsigned આગત_કિંમત = seqIndex - 1;
        જો (context_format != ContextFormat::SimpleContext)
        {
          આગત_data = data;
          આગત_કિંમત = આગત[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, આગત_data, આગત_કિંમત, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    જો (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      જો (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	જો (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ધકેલો_cur_active_glyphs (active_idx);
      જો (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	પાછા;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    નહીંતર જો (unlikely (!c->ધકેલો_મૂળ_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      પાછા;
    }

    unsigned endIndex = આગત_ગણતરી;
    જો (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->કાઢો_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
સ્થિર અંતઃરેખ શૂન્ય recurse_lookups (context_t *c,
                                    unsigned પૂર્ણાંક lookup_ગણતરી_2,
                                    અચલ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  માટે (unsigned પૂર્ણાંક i = 0; i < lookup_ગણતરી_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

સ્થિર અંતઃરેખ શૂન્ય apply_lookup (hb_ot_apply_context_t *c,
				 unsigned પૂર્ણાંક ગણતરી, /* Including the first glyph */
				 unsigned પૂર્ણાંક lookup_ગણતરી_2,
				 અચલ LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned પૂર્ણાંક match_end)
{
  hb_buffer_t *buffer = c->buffer;
  પૂર્ણાંક end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned પૂર્ણાંક bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    પૂર્ણાંક delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    માટે (unsigned પૂર્ણાંક j = 0; j < ગણતરી; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  માટે (unsigned પૂર્ણાંક i = 0; i < lookup_ગણતરી_2 && buffer->successful; i++)
  {
    unsigned પૂર્ણાંક idx = lookupRecord[i].sequenceIndex;
    જો (idx >= ગણતરી)
      ચાલુ;

    unsigned પૂર્ણાંક orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    જો (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      ચાલુ;

    જો (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      તોડો;

    જો (unlikely (buffer->max_ops <= 0))
      તોડો;

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      જો (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    તાર્કિક ret = c->recurse (lookupRecord[i].lookupListIndex);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      જો (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    જો (!ret)
      ચાલુ;

    unsigned પૂર્ણાંક new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    પૂર્ણાંક delta = new_len - orig_len;

    જો (!delta)
      ચાલુ;

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
    જો (end < પૂર્ણાંક (c->match_positions.arrayZ[idx]))
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

    unsigned પૂર્ણાંક next = idx + 1; /* next now is the position after the recursed lookup. */

    જો (delta > 0)
    {
      જો (unlikely (delta + ગણતરી > HB_MAX_CONTEXT_LENGTH))
	તોડો;
      જો (unlikely (ગણતરી + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (ગણતરી + delta)))
        પાછા;
    }
    નહીંતર
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (પૂર્ણાંક) next - (પૂર્ણાંક) ગણતરી);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (ગણતરી - next) * કદ (c->match_positions.arrayZ[0]));
    next += delta;
    ગણતરી += delta;

    /* Fill in new entries. */
    માટે (unsigned પૂર્ણાંક j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    માટે (; next < ગણતરી; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  પ્રતિજ્ઞા (end >= 0);
  (શૂન્ય) buffer->move_to (end);
}



/* Contextual lookups */

સંરચના ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  અચલ શૂન્ય *intersects_data;
  શૂન્ય *intersects_cache;
  શૂન્ય *intersected_glyphs_cache;
};

પ્રકારવ્યાખ્યા ContextClosureLookupContext ContextDependLookupContext;

સંરચના ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  અચલ શૂન્ય *collect_data;
};

સંરચના ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  અચલ શૂન્ય *match_data;
  get_કિંમત_func_t get_કિંમત;
};

સ્થિર અંતઃરેખ તાર્કિક context_intersects (અચલ hb_set_t *glyphs,
				       unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
				       glyph_data_t આગત, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  પાછા array_is_subset_of (glyphs,
			     આગત_ગણતરી ? આગત_ગણતરી - 1 : 0, આગત,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

સ્થિર અંતઃરેખ શૂન્ય context_closure_lookup (hb_closure_context_t *c,
					   unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
					   glyph_data_t આગત, /* Array of input values--start with second glyph */
					   unsigned પૂર્ણાંક lookup_ગણતરી_2,
					   અચલ LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  જો (context_intersects (c->glyphs,
			  આગત_ગણતરી, આગત,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     આગત_ગણતરી, આગત,
				     lookup_ગણતરી_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

સ્થિર અંતઃરેખ શૂન્ય context_depend_lookup (hb_depend_context_t *c,
					  unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
					  glyph_data_t આગત, /* Array of input values--start with second glyph */
					  unsigned પૂર્ણાંક lookup_ગણતરી_2,
					  અચલ LookupRecord lookupRecord[],
					  અચલ શૂન્ય *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  જો (!context_intersects (c->glyphs,
			   આગત_ગણતરી, આગત,
			   lookup_context))
    પાછા;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  જો (unlikely (!scratch))
    પાછા;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  તાર્કિક context_sets_cached = context_depend_sets_are_cached (
    c, આગત_ગણતરી, lookup_ગણતરી_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  જો (unlikely (!c->depend_data->successful))
    પાછા;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<અચલ hb_set_t *> &આગત_position_glyphs = scratch->આગત_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  જો (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    અચલ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      સાચું, &position_scratch, &scratch->compact_workspace);
    જો (unlikely (!pos0_glyphs ||
		  !આગત_position_glyphs.ધકેલો_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      પાછા;
    }

    /* Positions 1+ (Input array) */
    માટે (unsigned i = 0; i + 1 < આગત_ગણતરી; i++)
    {
      અચલ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	આગત[i], ખોટું, &position_scratch);
      જો (unlikely (!pos_glyphs ||
		    !આગત_position_glyphs.ધકેલો_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	પાછા;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  આગત_ગણતરી, આગત,
				  lookup_ગણતરી_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &આગત_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

સ્થિર અંતઃરેખ શૂન્ય context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
						  glyph_data_t આગત, /* Array of input values--start with second glyph */
						  unsigned પૂર્ણાંક lookup_ગણતરી_2,
						  અચલ LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->આગત,
		 આગત_ગણતરી ? આગત_ગણતરી - 1 : 0, આગત,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_ગણતરી_2, lookupRecord);
}

સ્થિર અંતઃરેખ તાર્કિક context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
					       glyph_data_t આગત, /* Array of input values--start with second glyph */
					       unsigned પૂર્ણાંક lookup_ગણતરી_2 HB_UNUSED,
					       અચલ LookupRecord lookupRecord[] HB_UNUSED,
					       અચલ ContextApplyLookupContext &lookup_context)
{
  પાછા would_match_આગત (c,
			    આગત_ગણતરી, આગત,
			    lookup_context.funcs.match, lookup_context.match_data);
}

સ્થિર અંતઃરેખ તાર્કિક context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
					 glyph_data_t આગત, /* Array of input values--start with second glyph */
					 unsigned પૂર્ણાંક lookup_ગણતરી_2,
					 અચલ LookupRecord lookupRecord[],
					 અચલ ContextApplyLookupContext &lookup_context)
{
  જો (unlikely (આગત_ગણતરી > HB_MAX_CONTEXT_LENGTH)) પાછા ખોટું;

  unsigned match_end = 0;
  તાર્કિક ret = ખોટું;
  જો (match_આગત (c,
		   આગત_ગણતરી, આગત,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  આગત_ગણતરી,
		  lookup_ગણતરી_2, lookupRecord,
		  match_end);
    ret = સાચું;
  }
  નહીંતર
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = ખોટું;
  }

  પાછા ret;
}

સ્થિર અંતઃરેખ તાર્કિક context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  પસંદગી (op)
  {
    સ્થિતિ hb_ot_subtable_cache_op_t::ENTER:
    {
      જો (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	પાછા ખોટું;
      auto &info = c->buffer->info;
      unsigned ગણતરી = c->buffer->len;
      માટે (unsigned i = 0; i < ગણતરી; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      પાછા સાચું;
    }
    સ્થિતિ hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      તોડો;
    }
  }
  પાછા ખોટું;
}

template <typename Types>
સંરચના Rule
{
  template <typename SetTypes, typename RuleType>
  friend સંરચના RuleSetOf;

  તાર્કિક intersects (અચલ hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) અચલ
  {
    પાછા context_intersects (glyphs,
			       આગત_ગણતરી, આગત_Z.arrayZ,
			       lookup_context);
  }

  શૂન્ય depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) અચલ
  {
    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array ((આગત_ગણતરી ? આગત_ગણતરી - 1 : 0)));
    context_depend_lookup (c,
			   આગત_ગણતરી, આગત_Z.arrayZ,
			   lookup_ગણતરી_2, lookupRecord.arrayZ,
			   આ,
			   value, lookup_context);
  }

  શૂન્ય closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;

    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array ((આગત_ગણતરી ? આગત_ગણતરી - 1 : 0)));
    context_closure_lookup (c,
			    આગત_ગણતરી, આગત_Z.arrayZ,
			    lookup_ગણતરી_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;
    જો (!intersects (c->glyphs, lookup_context)) પાછા;

    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array (આગત_ગણતરી ? આગત_ગણતરી - 1 : 0));
    recurse_lookups (c, lookup_ગણતરી_2, lookupRecord.arrayZ);
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) અચલ
  {
    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array (આગત_ગણતરી ? આગત_ગણતરી - 1 : 0));
    context_collect_glyphs_lookup (c,
				   આગત_ગણતરી, આગત_Z.arrayZ,
				   lookup_ગણતરી_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest,
			      અચલ ClassDef *class_def = nullptr) અચલ
  {
    જો (આગત_ગણતરી <= 1)
    {
      *digest = hb_set_digest_t::full ();
      પાછા;
    }

    unsigned second = આગત_Z.arrayZ[0];
    જો (!class_def)
      digest->add (second);
    નહીંતર જો (!second)
      *digest = hb_set_digest_t::full ();
    નહીંતર
      class_def->collect_class (digest, second);
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c,
		    અચલ ContextApplyLookupContext &lookup_context) અચલ
  {
    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array (આગત_ગણતરી ? આગત_ગણતરી - 1 : 0));
    પાછા context_would_apply_lookup (c,
				       આગત_ગણતરી, આગત_Z.arrayZ,
				       lookup_ગણતરી_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  તાર્કિક apply (hb_ot_apply_context_t *c,
	      અચલ ContextApplyLookupContext &lookup_context) અચલ
  {
    TRACE_APPLY (આ);
    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array (આગત_ગણતરી ? આગત_ગણતરી - 1 : 0));
    return_trace (context_apply_lookup (c, આગત_ગણતરી, આગત_Z.arrayZ, lookup_ગણતરી_2, lookupRecord.arrayZ, lookup_context));
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ hb_map_t *આગત_mapping, /* old->new glyphid or class mapping */
		  અચલ hb_map_t *lookup_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->start_embed (આ);
    જો (unlikely (!c->extend_min (out))) return_trace (ખોટું);

    out->આગત_ગણતરી = આગત_ગણતરી;
    અચલ auto આગત = આગત_Z.as_array (આગત_ગણતરી - 1);
    માટે (અચલ auto org : આગત)
    {
      typename Types::HBUINT d;
      d = આગત_mapping->get (org);
      c->copy (d);
    }

    અચલ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (આગત_Z.as_array ((આગત_ગણતરી ? આગત_ગણતરી - 1 : 0)));

    unsigned ગણતરી = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_ગણતરી_2), lookup_map);
    return_trace (c->check_assign (out->lookup_ગણતરી_2, ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       અચલ hb_map_t *lookup_map,
	       અચલ hb_map_t *klass_map = nullptr) અચલ
  {
    TRACE_SUBSET (આ);
    જો (unlikely (!આગત_ગણતરી)) return_trace (ખોટું);
    અચલ auto આગત = આગત_Z.as_array (આગત_ગણતરી - 1);

    અચલ hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    જો (!hb_all (આગત, mapping)) return_trace (ખોટું);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  c->check_range (આગત_Z.arrayZ,
				  આગત_Z.item_size * (આગત_ગણતરી ? આગત_ગણતરી - 1 : 0) +
				  LookupRecord::static_size * lookup_ગણતરી_2));
  }

  સુરક્ષિત:
  HBUINT16	આગત_ગણતરી;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_ગણતરી_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		આગત_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  જાહેર:
  DEFINE_SIZE_ARRAY (4, આગત_Z);
};

template <typename Types, typename RuleType>
સંરચના RuleSetOf
{
  તાર્કિક intersects (અચલ hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) અચલ
  {
    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_map ([&] (અચલ RuleType &_) { પાછા _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  શૂન્ય collect_first_આગત_classes (hb_ot_layout_ruleset_digest_t *digest) અચલ
  {
    digest->init ();
    માટે (unsigned i = 0; i < rule.len; i++)
    {
      અચલ RuleType &r = આ+rule.arrayZ[i];
      જો (r.આગત_ગણતરી <= 1)
      {
	digest->set_full ();
	પાછા;
      }
      digest->add (r.આગત_Z.arrayZ[0]);
    }
  }

  શૂન્ય depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) અચલ
  {
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  શૂન્ય closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;

    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) અચલ
  {
    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest,
			      અચલ ClassDef *class_def = nullptr) અચલ
  {
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([=] (અચલ RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c,
		    અચલ ContextApplyLookupContext &lookup_context) અચલ
  {
    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_map ([&] (અચલ RuleType &_) { પાછા _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c,
	      અચલ ContextApplyLookupContext &lookup_context,
	      અચલ hb_ot_layout_ruleset_digest_t *digest = nullptr) અચલ
  {
    TRACE_APPLY (આ);

    unsigned અંક_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    જો (HB_OPTIMIZE_SIZE_VAL || અંક_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (આ))
      | hb_map ([&] (અચલ RuleType &_) { પાછા _.apply (c, lookup_context); })
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
    તાર્કિક matched = skippy_iter.next (&unsafe_to1);
    જો (likely (matched))
    {
      જો (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        જાઓ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      જો (digest && lookup_context.get_કિંમત &&
	  !digest->may_have (lookup_context.get_કિંમત (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (ખોટું);
	}
    }
    નહીંતર
    {
      તાર્કિક unsafe_to_concat = ખોટું;
      તાર્કિક ret =
      + hb_iter (rule)
      | hb_map (hb_add (આ))
      | hb_filter ([&] (અચલ RuleType &_)
		   {
		     જો (_.આગત_ગણતરી <= 1) પાછા સાચું;
		     unsafe_to_concat = સાચું;
		     પાછા ખોટું;
		   })
      | hb_map ([&] (અચલ RuleType &_) { પાછા _.apply (c, lookup_context); })
      | hb_any
      ;
      જો (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    જો (likely (matched))
    {
      જો (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        જાઓ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_આગત = lookup_context.funcs.match;
    auto *આગત_data = lookup_context.match_data;
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_rules; i++)
    {
      અચલ auto &r = આ+rule.arrayZ[i];

      અચલ auto &આગત = r.આગત_Z;

      જો (r.આગત_ગણતરી <= 1 ||
	  (!match_આગત ||
	   match_આગત (*first, આગત.arrayZ[0], આગત_data)))
      {
        જો (!second ||
	    (r.આગત_ગણતરી <= 2 ||
	     (!match_આગત ||
	      match_આગત (*second, આગત.arrayZ[1], આગત_data)))
	   )
	{
	  જો (r.apply (c, lookup_context))
	  {
	    જો (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (સાચું);
	  }
	}
	નહીંતર
	  unsafe_to = unsafe_to2;
      }
      નહીંતર
      {
	જો (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	માટે (; i + 1 < અંક_rules; i++)
	{
	  અચલ auto &r2 = આ+rule.arrayZ[i + 1];
	  અચલ auto &input2 = r2.આગત_Z;
	  જો (r2.આગત_ગણતરી <= 1 || input2.arrayZ[0] != આગત.arrayZ[0])
	    તોડો;
	}
      }
    }
    જો (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ખોટું);
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       અચલ hb_map_t *lookup_map,
	       અચલ hb_map_t *klass_map = nullptr) અચલ
  {
    TRACE_SUBSET (આ);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    માટે (અચલ auto& _ : rule)
    {
      જો (!_) ચાલુ;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      જો (unlikely (!o)) ચાલુ;

      જો (!o->serialize_subset (c, _, આ, lookup_map, klass_map))
      {
	out->rule.કાઢો ();
	c->serializer->revert (o_snap);
      }
    }

    તાર્કિક ret = તાર્કિક (out->rule);
    જો (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (rule.sanitize (c, આ));
  }

  સુરક્ષિત:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  જાહેર:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
સંરચના RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
સંરચના ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
સંરચના ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    પાછા
    + hb_zip (આ+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_map ([&] (અચલ RuleSet &_) { પાછા _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (આ+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      પાછા c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](અચલ hb_pair_t<hb_codepoint_t, unsigned> _) { પાછા hb_pair_t<unsigned, અચલ RuleSet&> (_.first, આ+ruleSet[_.second]); })
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (આ+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      પાછા c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](અચલ hb_pair_t<hb_codepoint_t, unsigned> _) { પાછા hb_pair_t<unsigned, અચલ RuleSet&> (_.first, આ+ruleSet[_.second]); })
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ
  {
    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (આ+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    (આ+coverage).collect_coverage (c->આગત);

    સંરચના ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([digest] (અચલ RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    અચલ RuleSet &rule_set = આ+ruleSet[(આ+coverage).get_coverage (c->glyphs[0])];
    સંરચના ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    પાછા rule_set.would_apply (c, lookup_context);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
    જો (likely (index == NOT_COVERED))
      return_trace (ખોટું);

    અચલ RuleSet &rule_set = આ+ruleSet[index];
    સંરચના ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    અચલ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, આ, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (તાર્કિક (new_coverage));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && ruleSet.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
સંરચના ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    જો (!(આ+coverage).intersects (glyphs))
      પાછા ખોટું;

    અચલ ClassDef &class_def = આ+classDef;

    hb_map_t cache;
    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (આ+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    પાછા
    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_enumerate
    | hb_map ([&] (અચલ hb_pair_t<unsigned, અચલ RuleSet &> p)
	      { પાછા class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    જો (!(આ+coverage).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    અચલ ClassDef &class_def = આ+classDef;

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
    { પાછા class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  અચલ RuleSet& rule_set = આ+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    જો (!(આ+coverage).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    અચલ ClassDef &class_def = આ+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { પાછા class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  અચલ RuleSet& rule_set = આ+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ
  {
    જો (!(આ+coverage).intersects (c->glyphs))
      પાછા;

    અચલ ClassDef &class_def = આ+classDef;

    hb_map_t cache;
    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_enumerate
    | hb_filter ([&] (અચલ hb_pair_t<unsigned, અચલ RuleSet &> p)
    { પાછા class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (અચલ RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    (આ+coverage).collect_coverage (c->આગત);

    અચલ ClassDef &class_def = આ+classDef;
    સંરચના ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    અચલ ClassDef &class_def = આ+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([=, &class_def] (અચલ RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    અચલ ClassDef &class_def = આ+classDef;
    unsigned પૂર્ણાંક index = class_def.get_class (c->glyphs[0]);
    અચલ RuleSet &rule_set = આ+ruleSet[index];
    સંરચના ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    પાછા rule_set.would_apply (c, lookup_context);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  unsigned cache_cost () અચલ
  {
    પાછા (આ+classDef).cost ();
  }
  સ્થિર તાર્કિક cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    પાછા context_cache_func (c, op);
  }

  સંરચના external_cache_t
  {
    hb_ot_layout_દ્વિઅંકી_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  શૂન્ય *external_cache_create () અચલ
  {
    unsigned ગણતરી = ruleSet.len;
    unsigned size = કદ (external_cache_t) -
		    HB_VAR_ARRAY * કદ (hb_ot_layout_ruleset_digest_t) +
		    ગણતરી * કદ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    જો (likely (cache))
    {
      cache->coverage.clear ();
      માટે (unsigned i = 0; i < ગણતરી; i++)
	(આ+ruleSet[i]).collect_first_આગત_classes (&cache->rule_sets[i]);
    }
    પાછા cache;
  }
  તાર્કિક apply_cached (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ { પાછા _apply (c, સાચું, external_cache); }
  તાર્કિક apply (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ { પાછા _apply (c, ખોટું, external_cache); }
  તાર્કિક _apply (hb_ot_apply_context_t *c, તાર્કિક cached, શૂન્ય *external_cache) અચલ
  {
    TRACE_APPLY (આ);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage_દ્વિઅંકી (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#નહીંતર
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    જો (index == NOT_COVERED) return_trace (ખોટું);

    અચલ ClassDef &class_def = આ+classDef;

    સંરચના ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_કિંમત : get_class_કિંમત
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    અચલ RuleSet &rule_set = આ+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    અચલ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#નહીંતર
    અચલ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;
    જો (unlikely (!out->coverage.serialize_subset (c, coverage, આ)))
      return_trace (ખોટું);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, આ, &klass_map);

    અચલ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (આ+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (આ+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    અચલ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    તાર્કિક ret = સાચું;
    પૂર્ણાંક non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    માટે (અચલ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      જો (unlikely (!o))
      {
	ret = ખોટું;
	તોડો;
      }

      જો (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, આ, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    જો (!ret || non_zero_index == -1) return_trace (ખોટું);

    //prune empty trailing ruleSets
    --index;
    જ્યારે (index > non_zero_index)
    {
      out->ruleSet.કાઢો ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (તાર્કિક (out->ruleSet));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && classDef.sanitize (c, આ) && ruleSet.sanitize (c, આ));
  }

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
સંરચના ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    જો (!(આ+coverageZ[0]).intersects (glyphs))
      પાછા ખોટું;

    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      આ
    };
    પાછા context_intersects (glyphs,
			       glyph_ગણતરી_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    જો (!(આ+coverageZ[0]).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      આ,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_ગણતરી_2, (અચલ HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_ગણતરી_2, lookupRecord,
			   આ,
			   coverageZ[0], lookup_context);

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    જો (!(આ+coverageZ[0]).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    સંરચના ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      આ
    };
    context_closure_lookup (c,
			    glyph_ગણતરી_2, coverageZ.arrayZ + 1,
			    lookup_ગણતરી_2, lookupRecord,
			    0, lookup_context);

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ
  {
    જો (!intersects (c->glyphs))
      પાછા;
    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    recurse_lookups (c, lookup_ગણતરી_2, lookupRecord);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    (આ+coverageZ[0]).collect_coverage (c->આગત);

    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    સંરચના ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      આ
    };

    context_collect_glyphs_lookup (c,
				   glyph_ગણતરી_2, coverageZ.arrayZ + 1,
				   lookup_ગણતરી_2, lookupRecord,
				   lookup_context);
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    જો (glyph_ગણતરી_2 <= 1)
      *digest = hb_set_digest_t::full ();
    નહીંતર
      (આ+coverageZ[1]).collect_coverage (digest);
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    સંરચના ContextApplyLookupContext lookup_context = {
      {match_coverage},
      આ
    };
    પાછા context_would_apply_lookup (c,
				       glyph_ગણતરી_2, coverageZ.arrayZ + 1,
				       lookup_ગણતરી_2, lookupRecord,
				       lookup_context);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverageZ[0]; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક index = (આ+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    સંરચના ContextApplyLookupContext lookup_context = {
      {match_coverage},
      આ
    };
    return_trace (context_apply_lookup (c,
					glyph_ગણતરી_2,
					coverageZ.arrayZ + 1,
					lookup_ગણતરી_2, lookupRecord,
					lookup_context));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    out->format = format;
    out->glyph_ગણતરી_2 = glyph_ગણતરી_2;

    auto coverages = coverageZ.as_array (glyph_ગણતરી_2);

    માટે (અચલ auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      જો (unlikely (!o)) return_trace (ખોટું);
      જો (!o->serialize_subset (c, offset, આ)) return_trace (ખોટું);
    }

    અચલ auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_ગણતરી_2));
    અચલ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned ગણતરી = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_ગણતરી_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_ગણતરી_2, ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ))) return_trace (ખોટું);
    hb_barrier ();
    unsigned પૂર્ણાંક ગણતરી = glyph_ગણતરી_2;
    જો (unlikely (!ગણતરી)) return_trace (ખોટું); /* We want to access coverageZ[0] freely. */
    જો (unlikely (!c->check_array (coverageZ.arrayZ, ગણતરી))) return_trace (ખોટું);
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!coverageZ[i].sanitize (c, આ))) return_trace (ખોટું);
    અચલ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ગણતરી_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_ગણતરી_2)));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_ગણતરી_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_ગણતરી_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  જાહેર:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

સંરચના Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    સ્થિતિ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    સ્થિતિ 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
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

સંરચના ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  અચલ શૂન્ય *intersects_data[3];
  શૂન્ય *intersects_cache[3];
  શૂન્ય *intersected_glyphs_cache;
};

પ્રકારવ્યાખ્યા ChainContextClosureLookupContext ChainContextDependLookupContext;

સંરચના ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  અચલ શૂન્ય *collect_data[3];
};

સંરચના ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  અચલ શૂન્ય *match_data[3];
  get_કિંમત_func_t get_આગત_કિંમત;
};

સ્થિર અંતઃરેખ તાર્કિક chain_context_intersects (અચલ hb_set_t *glyphs,
					     unsigned પૂર્ણાંક backtrack_ગણતરી,
					     glyph_data_t backtrack,
					     unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
					     glyph_data_t આગત, /* Array of input values--start with second glyph */
					     unsigned પૂર્ણાંક lookahead_ગણતરી,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  તાર્કિક bt_ok = array_is_subset_of (glyphs,
			     backtrack_ગણતરી, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  તાર્કિક in_ok = array_is_subset_of (glyphs,
			     આગત_ગણતરી ? આગત_ગણતરી - 1 : 0, આગત,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  તાર્કિક la_ok = array_is_subset_of (glyphs,
			     lookahead_ગણતરી, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  પાછા bt_ok && in_ok && la_ok;
}

સ્થિર અંતઃરેખ શૂન્ય chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned પૂર્ણાંક backtrack_ગણતરી,
						 glyph_data_t backtrack,
						 unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
						 glyph_data_t આગત, /* Array of input values--start with second glyph */
						 unsigned પૂર્ણાંક lookahead_ગણતરી,
						 glyph_data_t lookahead,
						 unsigned પૂર્ણાંક lookup_ગણતરી_2,
						 અચલ LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  જો (chain_context_intersects (c->glyphs,
				backtrack_ગણતરી, backtrack,
				આગત_ગણતરી, આગત,
				lookahead_ગણતરી, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     આગત_ગણતરી, આગત,
		     lookup_ગણતરી_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

સ્થિર અંતઃરેખ શૂન્ય chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned પૂર્ણાંક backtrack_ગણતરી,
						glyph_data_t backtrack,
						unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
						glyph_data_t આગત, /* Array of input values--start with second glyph */
						unsigned પૂર્ણાંક lookahead_ગણતરી,
						glyph_data_t lookahead,
						unsigned પૂર્ણાંક lookup_ગણતરી_2,
						અચલ LookupRecord lookupRecord[],
						અચલ શૂન્ય *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  જો (!chain_context_intersects (c->glyphs,
				 backtrack_ગણતરી, backtrack,
				 આગત_ગણતરી, આગત,
				 lookahead_ગણતરી, lookahead,
				 lookup_context))
    પાછા;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  જો (unlikely (!scratch))
    પાછા;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  તાર્કિક context_sets_cached = context_depend_sets_are_cached (
    c, આગત_ગણતરી, lookup_ગણતરી_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  જો (unlikely (!c->depend_data->successful))
    પાછા;
  જો (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    આગત_ગણતરી, આગત,
				    lookup_ગણતરી_2, lookupRecord,
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
    પાછા;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<અચલ hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<અચલ hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  માટે (unsigned i = 0; i < backtrack_ગણતરી; i++)
  {
    અચલ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], ખોટું, &position_scratch);
    જો (unlikely (!pos_glyphs))
      પાછા;
    જો (!pos_glyphs->is_empty ())
    {
      જો (unlikely (!backtrack_sets.ધકેલો_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        પાછા;
      }
    }
  }

  /* Extract lookahead glyphs */
  માટે (unsigned i = 0; i < lookahead_ગણતરી; i++)
  {
    અચલ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], ખોટું, &position_scratch);
    જો (unlikely (!pos_glyphs))
      પાછા;
    જો (!pos_glyphs->is_empty ())
    {
      જો (unlikely (!lookahead_sets.ધકેલો_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        પાછા;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  માટે (અચલ hb_set_t *back_set : backtrack_sets)
  {
    જો (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } નહીંતર જો (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      જો (unlikely (set_idx == HB_CODEPOINT_INVALID))
        પાછા;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  માટે (અચલ hb_set_t *look_set : lookahead_sets)
  {
    જો (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } નહીંતર જો (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      જો (unlikely (set_idx == HB_CODEPOINT_INVALID))
        પાછા;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  જો (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    પાછા;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<અચલ hb_set_t *> &આગત_position_glyphs = scratch->આગત_position_glyphs;

  જો (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    અચલ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      સાચું, &position_scratch, &scratch->compact_workspace);
    જો (unlikely (!pos0_glyphs ||
		  !આગત_position_glyphs.ધકેલો_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      પાછા;
    }

    /* Positions 1+ (Input array) */
    માટે (unsigned i = 0; i + 1 < આગત_ગણતરી; i++)
    {
      અચલ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	આગત[i], ખોટું, &position_scratch);
      જો (unlikely (!pos_glyphs ||
		    !આગત_position_glyphs.ધકેલો_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	પાછા;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  આગત_ગણતરી, આગત,
				  lookup_ગણતરી_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &આગત_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

સ્થિર અંતઃરેખ શૂન્ય chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned પૂર્ણાંક backtrack_ગણતરી,
							glyph_data_t backtrack,
							unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
							glyph_data_t આગત, /* Array of input values--start with second glyph */
							unsigned પૂર્ણાંક lookahead_ગણતરી,
							glyph_data_t lookahead,
							unsigned પૂર્ણાંક lookup_ગણતરી_2,
							અચલ LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_ગણતરી, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->આગત,
		 આગત_ગણતરી ? આગત_ગણતરી - 1 : 0, આગત,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_ગણતરી, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_ગણતરી_2, lookupRecord);
}

સ્થિર અંતઃરેખ તાર્કિક chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned પૂર્ણાંક backtrack_ગણતરી,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
						     glyph_data_t આગત, /* Array of input values--start with second glyph */
						     unsigned પૂર્ણાંક lookahead_ગણતરી,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned પૂર્ણાંક lookup_ગણતરી_2 HB_UNUSED,
						     અચલ LookupRecord lookupRecord[] HB_UNUSED,
						     અચલ ChainContextApplyLookupContext &lookup_context)
{
  પાછા (c->zero_context ? !backtrack_ગણતરી && !lookahead_ગણતરી : સાચું)
      && would_match_આગત (c,
			    આગત_ગણતરી, આગત,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

સ્થિર અંતઃરેખ તાર્કિક chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned પૂર્ણાંક backtrack_ગણતરી,
					       glyph_data_t backtrack,
					       unsigned પૂર્ણાંક આગત_ગણતરી, /* Including the first glyph (not matched) */
					       glyph_data_t આગત, /* Array of input values--start with second glyph */
					       unsigned પૂર્ણાંક lookahead_ગણતરી,
					       glyph_data_t lookahead,
					       unsigned પૂર્ણાંક lookup_ગણતરી_2,
					       અચલ LookupRecord lookupRecord[],
					       અચલ ChainContextApplyLookupContext &lookup_context)
{
  જો (unlikely (આગત_ગણતરી > HB_MAX_CONTEXT_LENGTH)) પાછા ખોટું;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  તાર્કિક ret = સાચું;
  જો (!(match_આગત (c,
		     આગત_ગણતરી, આગત,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_ગણતરી, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    પાછા ખોટું;
  }

  જો (!match_backtrack (c,
			backtrack_ગણતરી, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    પાછા ખોટું;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		આગત_ગણતરી,
		lookup_ગણતરી_2, lookupRecord,
		match_end);

  પાછા ret;
}

template <typename Types>
સંરચના ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend સંરચના ChainRuleSetOf;

  તાર્કિક intersects (અચલ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    પાછા chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     આગત.lenP1, આગત.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  શૂન્ય depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 આગત.lenP1, આગત.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 આ,
				 value,
				 lookup_context);
  }

  શૂન્ય closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;

    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  આગત.lenP1, આગત.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;
    જો (!intersects (c->glyphs, lookup_context)) પાછા;

    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 આગત.lenP1, આગત.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest,
			      અચલ ClassDef *આગત_class_def = nullptr,
			      અચલ ClassDef *lookahead_class_def = nullptr) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);

    unsigned second;
    અચલ ClassDef *class_def;
    જો (આગત.lenP1 > 1)
    {
      second = આગત.arrayZ[0];
      class_def = આગત_class_def;
    }
    નહીંતર જો (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    નહીંતર
    {
      *digest = hb_set_digest_t::full ();
      પાછા;
    }

    જો (!class_def)
      digest->add (second);
    નહીંતર જો (!second)
      *digest = hb_set_digest_t::full ();
    નહીંતર
      class_def->collect_class (digest, second);
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c,
		    અચલ ChainContextApplyLookupContext &lookup_context) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    પાછા chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     આગત.lenP1, આગત.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  તાર્કિક apply (hb_ot_apply_context_t *c,
	      અચલ ChainContextApplyLookupContext &lookup_context) અચલ
  {
    TRACE_APPLY (આ);
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      આગત.lenP1, આગત.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) અચલ
  {
    c->copy (len);
    માટે (અચલ auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ hb_map_t *lookup_map,
		  અચલ hb_map_t *backtrack_map,
		  અચલ hb_map_t *આગત_map = nullptr,
		  અચલ hb_map_t *lookahead_map = nullptr) અચલ
  {
    TRACE_SERIALIZE (આ);

    અચલ hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    જો (આગત_map) mapping = આગત_map;
    serialize_array (c, આગત.lenP1, + આગત.iter ()
				     | hb_map (mapping));

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    જો (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_ગણતરી_2 = c->embed (&(lookup.len));
    જો (!lookup_ગણતરી_2) return_trace (ખોટું);

    unsigned ગણતરી = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_ગણતરી_2, ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       અચલ hb_map_t *lookup_map,
	       અચલ hb_map_t *backtrack_map = nullptr,
	       અચલ hb_map_t *આગત_map = nullptr,
	       અચલ hb_map_t *lookahead_map = nullptr) અચલ
  {
    TRACE_SUBSET (આ);

    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);

    જો (!backtrack_map)
    {
      અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      જો (!hb_all (backtrack, glyphset) ||
	  !hb_all (આગત, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (ખોટું);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    નહીંતર
    {
      જો (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (આગત, આગત_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (ખોટું);

      serialize (c->serializer, lookup_map, backtrack_map, આગત_map, lookahead_map);
    }

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* Hyper-optimized sanitized because this is really hot. */
    જો (unlikely (!backtrack.len.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    જો (unlikely (!આગત.lenP1.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    જો (unlikely (!lookahead.len.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  સુરક્ષિત:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		આગત_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  જાહેર:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
સંરચના ChainRuleSetOf
{
  તાર્કિક intersects (અચલ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) અચલ
  {
    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_map ([&] (અચલ ChainRuleType &_) { પાછા _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  શૂન્ય collect_first_આગત_classes (hb_ot_layout_ruleset_digest_t *digest) અચલ
  {
    digest->init ();
    માટે (unsigned i = 0; i < rule.len; i++)
    {
      અચલ ChainRuleType &r = આ+rule.arrayZ[i];
      અચલ auto &આગત = StructAfter<decltype (r.આગત_X)> (r.backtrack);
      જો (આગત.lenP1 <= 1)
      {
	digest->set_full ();
	પાછા;
      }
      digest->add (આગત.arrayZ[0]);
    }
  }
  શૂન્ય depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) અચલ
  {
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  શૂન્ય closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;

    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) અચલ
  {
    જો (unlikely (c->lookup_limit_exceeded ())) પાછા;

    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) અચલ
  {
    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest,
			      અચલ ClassDef *આગત_class_def = nullptr,
			      અચલ ClassDef *lookahead_class_def = nullptr) અચલ
  {
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_apply ([=] (અચલ ChainRuleType &_)
		{ _.collect_second_glyphs (digest, આગત_class_def, lookahead_class_def); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c,
		    અચલ ChainContextApplyLookupContext &lookup_context) અચલ
  {
    પાછા
    + hb_iter (rule)
    | hb_map (hb_add (આ))
    | hb_map ([&] (અચલ ChainRuleType &_) { પાછા _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c,
	      અચલ ChainContextApplyLookupContext &lookup_context,
	      અચલ hb_ot_layout_ruleset_digest_t *digest = nullptr) અચલ
  {
    TRACE_APPLY (આ);

    unsigned અંક_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    જો (HB_OPTIMIZE_SIZE_VAL || અંક_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (આ))
      | hb_map ([&] (અચલ ChainRuleType &_) { પાછા _.apply (c, lookup_context); })
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
    તાર્કિક matched = skippy_iter.next (&unsafe_to1);
    જો (likely (matched))
    {
      જો (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        જાઓ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      જો (digest && lookup_context.get_આગત_કિંમત &&
	  !digest->may_have (lookup_context.get_આગત_કિંમત (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (ખોટું);
	}
    }
    નહીંતર
    {
      તાર્કિક unsafe_to_concat = ખોટું;
      તાર્કિક ret =
      + hb_iter (rule)
      | hb_map (hb_add (આ))
      | hb_filter ([&] (અચલ ChainRuleType &_)
		   {
		     અચલ auto &આગત = StructAfter<decltype (_.આગત_X)> (_.backtrack);
		     અચલ auto &lookahead = StructAfter<decltype (_.lookaheadX)> (આગત);
		     જો (આગત.lenP1 <= 1 && lookahead.len == 0) પાછા સાચું;
		     unsafe_to_concat = સાચું;
		     પાછા ખોટું;
		   })
      | hb_map ([&] (અચલ ChainRuleType &_) { પાછા _.apply (c, lookup_context); })
      | hb_any
      ;
      જો (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    જો (likely (matched))
    {
      જો (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        જાઓ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_આગત = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *આગત_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_rules; i++)
    {
      અચલ auto &r = આ+rule.arrayZ[i];

      અચલ auto &આગત = StructAfter<decltype (r.આગત_X)> (r.backtrack);
      અચલ auto &lookahead = StructAfter<decltype (r.lookaheadX)> (આગત);

      unsigned lenP1 = આગત.lenP1;
      જો (lenP1 > 1 ?
	   (!match_આગત ||
	    match_આગત (*first, આગત.arrayZ[0], આગત_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        જો (!second ||
	    (lenP1 > 2 ?
	     (!match_આગત ||
	      match_આગત (*second, આગત.arrayZ[1], આગત_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  જો (r.apply (c, lookup_context))
	  {
	    જો (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (સાચું);
	  }
	}
	નહીંતર
	  unsafe_to = unsafe_to2;
      }
      નહીંતર
      {
	જો (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	જો (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  માટે (; i + 1 < અંક_rules; i++)
	  {
	    અચલ auto &r2 = આ+rule.arrayZ[i + 1];
	    અચલ auto &input2 = StructAfter<decltype (r2.આગત_X)> (r2.backtrack);
	    જો (input2.lenP1 <= 1 || input2.arrayZ[0] != આગત.arrayZ[0])
	      તોડો;
	  }
	}
      }
    }
    જો (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ખોટું);
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       અચલ hb_map_t *lookup_map,
	       અચલ hb_map_t *backtrack_klass_map = nullptr,
	       અચલ hb_map_t *આગત_klass_map = nullptr,
	       અચલ hb_map_t *lookahead_klass_map = nullptr) અચલ
  {
    TRACE_SUBSET (આ);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    માટે (અચલ auto& _ : rule)
    {
      જો (!_) ચાલુ;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      જો (unlikely (!o)) ચાલુ;

      જો (!o->serialize_subset (c, _, આ,
				lookup_map,
				backtrack_klass_map,
				આગત_klass_map,
				lookahead_klass_map))
      {
	out->rule.કાઢો ();
	c->serializer->revert (o_snap);
      }
    }

    તાર્કિક ret = તાર્કિક (out->rule);
    જો (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (rule.sanitize (c, આ));
  }

  સુરક્ષિત:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  જાહેર:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
સંરચના ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (કદ (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
સંરચના ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
સંરચના ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    પાછા
    + hb_zip (આ+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_map ([&] (અચલ ChainRuleSet &_) { પાછા _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (આ+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      પાછા c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](અચલ hb_pair_t<hb_codepoint_t, unsigned> _) { પાછા hb_pair_t<unsigned, અચલ ChainRuleSet&> (_.first, આ+ruleSet[_.second]); })
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (આ+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      પાછા c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](અચલ hb_pair_t<hb_codepoint_t, unsigned> _) { પાછા hb_pair_t<unsigned, અચલ ChainRuleSet&> (_.first, આ+ruleSet[_.second]); })
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ
  {
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (આ+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    (આ+coverage).collect_coverage (c->આગત);

    સંરચના ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([digest] (અચલ ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    અચલ ChainRuleSet &rule_set = આ+ruleSet[(આ+coverage).get_coverage (c->glyphs[0])];
    સંરચના ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    પાછા rule_set.would_apply (c, lookup_context);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    અચલ ChainRuleSet &rule_set = આ+ruleSet[index];
    સંરચના ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    અચલ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, આ, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (તાર્કિક (new_coverage));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && ruleSet.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
સંરચના ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    જો (!(આ+coverage).intersects (glyphs))
      પાછા ખોટું;

    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (આ+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    આગત_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    પાછા
    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_enumerate
    | hb_map ([&] (અચલ hb_pair_t<unsigned, અચલ ChainRuleSet &> p)
	      { પાછા આગત_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    જો (!(આ+coverage).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { પાછા આગત_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  અચલ ChainRuleSet& chainrule_set = આ+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    જો (!(આ+coverage).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs)) પાછા;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { પાછા આગત_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<unsigned, અચલ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  અચલ ChainRuleSet& chainrule_set = આ+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ
  {
    જો (!(આ+coverage).intersects (c->glyphs))
      પાછા;

    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { પાછા આગત_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (અચલ ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    (આ+coverage).collect_coverage (c->આગત);

    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

    સંરચના ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (આ))
    | hb_apply ([digest, &આગત_class_def, &lookahead_class_def] (અચલ ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &આગત_class_def, &lookahead_class_def); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

    unsigned પૂર્ણાંક index = આગત_class_def.get_class (c->glyphs[0]);
    અચલ ChainRuleSet &rule_set = આ+ruleSet[index];
    સંરચના ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def}
    };
    પાછા rule_set.would_apply (c, lookup_context);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  unsigned cache_cost () અચલ
  {
    પાછા (આ+આગત_Class_Def).cost () + (આ+lookaheadClassDef).cost ();
  }
  સ્થિર તાર્કિક cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    પાછા context_cache_func (c, op);
  }

  સંરચના external_cache_t
  {
    hb_ot_layout_દ્વિઅંકી_cache_t coverage;
    hb_ot_layout_mapping_cache_t આગત_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  શૂન્ય *external_cache_create () અચલ
  {
    unsigned ગણતરી = ruleSet.len;
    unsigned size = કદ (external_cache_t) -
		    HB_VAR_ARRAY * કદ (hb_ot_layout_ruleset_digest_t) +
		    ગણતરી * કદ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    જો (likely (cache))
    {
      cache->coverage.clear ();
      cache->આગત_class.clear ();
      cache->lookahead_class.clear ();
      માટે (unsigned i = 0; i < ગણતરી; i++)
	(આ+ruleSet[i]).collect_first_આગત_classes (&cache->rule_sets[i]);
    }
    પાછા cache;
  }
  તાર્કિક apply_cached (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ { પાછા _apply (c, સાચું, external_cache); }
  તાર્કિક apply (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ { પાછા _apply (c, ખોટું, external_cache); }
  તાર્કિક _apply (hb_ot_apply_context_t *c, તાર્કિક cached, શૂન્ય *external_cache) અચલ
  {
    TRACE_APPLY (આ);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage_દ્વિઅંકી (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#નહીંતર
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    જો (index == NOT_COVERED) return_trace (ખોટું);

    અચલ ClassDef &backtrack_class_def = આ+backtrackClassDef;
    અચલ ClassDef &આગત_class_def = આ+આગત_Class_Def;
    અચલ ClassDef &lookahead_class_def = આ+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t આગત_cache_data = {
      &આગત_class_def,
      cache ? &cache->આગત_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    અચલ શૂન્ય *આગત_match_data = cached
      ? static_cast<અચલ શૂન્ય *> (&આગત_class_def)
      : static_cast<અચલ શૂન્ય *> (&આગત_cache_data);
    અચલ શૂન્ય *lookahead_match_data = cached
      ? static_cast<અચલ શૂન્ય *> (&lookahead_class_def)
      : static_cast<અચલ શૂન્ય *> (&lookahead_cache_data);
    તાર્કિક backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    અચલ શૂન્ય *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<અચલ શૂન્ય *> (&lookahead_cache_data)
      : static_cast<અચલ શૂન્ય *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    સંરચના ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       આગત_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_કિંમત : get_class_cache_કિંમત
#નહીંતર
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &આગત_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_કિંમત : get_class_કિંમત
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (આગત_class_def, c->buffer->cur())
         : આગત_class_def.get_class (c->buffer->cur().codepoint, આગત_cache_data.cache);
#નહીંતર
    index = cached
         ? get_class_cached2 (આગત_class_def, c->buffer->cur())
          : આગત_class_def.get_class (c->buffer->cur().codepoint);
#endif
    અચલ ChainRuleSet &rule_set = આ+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    અચલ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#નહીંતર
    અચલ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, આ);

    hb_map_t backtrack_klass_map;
    hb_map_t આગત_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, આ, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->આગત_Class_Def.serialize_subset (c, આગત_Class_Def, આ, &આગત_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, આ, &lookahead_klass_map);

    જો (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   આગત_klass_map,
						   lookahead_klass_map)))
      return_trace (ખોટું);

    અચલ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (આ+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (આ+આગત_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    પૂર્ણાંક non_zero_index = -1, index = 0;
    તાર્કિક ret = સાચું;
    અચલ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    માટે (અચલ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (આગત_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      જો (unlikely (!o))
      {
	ret = ખોટું;
	તોડો;
      }
      જો (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, આ,
			       lookup_map,
			       &backtrack_klass_map,
			       &આગત_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    જો (!ret || non_zero_index == -1) return_trace (ખોટું);

    // prune empty trailing ruleSets
    જો (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (તાર્કિક (out->ruleSet));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) &&
		  backtrackClassDef.sanitize (c, આ) &&
		  આગત_Class_Def.sanitize (c, આ) &&
		  lookaheadClassDef.sanitize (c, આ) &&
		  ruleSet.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		આગત_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  જાહેર:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
સંરચના ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);

    જો (!(આ+આગત[0]).intersects (glyphs))
      પાછા ખોટું;

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {આ, આ, આ}
    };
    પાછા chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     આગત.len, આગત.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);

    જો (!(આ+આગત[0]).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs))
      પાછા;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {આ, આ, આ},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (અચલ HBUINT16 *) backtrack.arrayZ,
				 આગત.len, (અચલ HBUINT16 *) આગત.arrayZ + 1,
				 lookahead.len, (અચલ HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 આ,
				 આગત[0], lookup_context);

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);

    જો (!(આ+આગત[0]).intersects (c->glyphs))
      પાછા;

    hb_set_t* cur_active_glyphs = c->ધકેલો_cur_active_glyphs ();
    જો (unlikely (!cur_active_glyphs))
      પાછા;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    સંરચના ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {આ, આ, આ}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  આગત.len, આગત.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->કાઢો_cur_done_glyphs ();
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ
  {
    જો (!intersects (c->glyphs))
      પાછા;

    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);

    (આ+આગત[0]).collect_coverage (c->આગત);

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    સંરચના ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {આ, આ, આ}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 આગત.len, આગત.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    જો (આગત.len > 1)
      (આ+આગત[1]).collect_coverage (digest);
    નહીંતર જો (lookahead.len)
      (આ+lookahead[0]).collect_coverage (digest);
    નહીંતર
      *digest = hb_set_digest_t::full ();
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    સંરચના ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {આ, આ, આ}
    };
    પાછા chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     આગત.len, આગત.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  અચલ Coverage &get_coverage () અચલ
  {
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    પાછા આ+આગત[0];
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);

    unsigned પૂર્ણાંક index = (આ+આગત[0]).get_coverage (c->buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    સંરચના ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {આ, આ, આ}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      આગત.len, આગત.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, અચલ શૂન્ય* base) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    જો (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ખોટું);

    માટે (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      જો (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    જો (unlikely (!c->serializer->embed (આ->format))) return_trace (ખોટું);

    જો (!serialize_coverage_offsets (c, backtrack.iter (), આ))
      return_trace (ખોટું);

    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    જો (!serialize_coverage_offsets (c, આગત.iter (), આ))
      return_trace (ખોટું);

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    જો (!serialize_coverage_offsets (c, lookahead.iter (), આ))
      return_trace (ખોટું);

    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    અચલ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_ગણતરી_2 = c->serializer->copy<HBUINT16> (lookup.len);
    જો (!lookup_ગણતરી_2) return_trace (ખોટું);

    unsigned ગણતરી = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_ગણતરી_2, ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!backtrack.sanitize (c, આ))) return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &આગત = StructAfter<decltype (આગત_X)> (backtrack);
    જો (unlikely (!આગત.sanitize (c, આ))) return_trace (ખોટું);
    hb_barrier ();
    જો (unlikely (!આગત.len)) return_trace (ખોટું); /* To be consistent with Context. */
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (આગત);
    જો (unlikely (!lookahead.sanitize (c, આ))) return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		આગત_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  જાહેર:
  DEFINE_SIZE_MIN (10);
};

સંરચના ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    સ્થિતિ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }		format;	/* Format identifier */
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
સંરચના ExtensionFormat1
{
  unsigned પૂર્ણાંક get_type () અચલ { પાછા extensionLookupType; }

  template <typename X>
  અચલ X& get_subtable () અચલ
  { પાછા આ + reinterpret_cast<અચલ Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, આ))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    અચલ auto& src_offset =
        reinterpret_cast<અચલ Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, આ, get_type ()));
  }

  સુરક્ષિત:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
સંરચના Extension
{
  unsigned પૂર્ણાંક get_type () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_type ();
    મૂળભૂત:પાછા 0;
    }
  }
  template <typename X>
  અચલ X& get_subtable () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.template get_subtable<typename T::SubTable> ();
    મૂળભૂત:પાછા Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.subset (c);
    મૂળભૂત: પાછા c->default_return_કિંમત ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

સંરચના hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  સ્થિર hb_ot_layout_lookup_accelerator_t *create (અચલ TLookup &lookup)
  {
    unsigned ગણતરી = lookup.get_subtable_ગણતરી ();

    unsigned size = કદ (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * કદ (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    ગણતરી * કદ (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    જો (unlikely (!thiz))
      પાછા nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    જ્યારે (ગણતરી && !thiz->subtables[ગણતરી - 1].apply_func)
      ગણતરી--;

    thiz->digest.init ();
    માટે (auto& subtable : hb_iter (thiz->subtables, ગણતરી))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->ગણતરી = ગણતરી;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    માટે (unsigned i = 0; i < ગણતરી; i++)
      જો (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    પાછા thiz;
  }

  શૂન્ય fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    માટે (unsigned i = 0; i < ગણતરી; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  તાર્કિક may_have (hb_codepoint_t g) અચલ
  { પાછા digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક apply (hb_ot_apply_context_t *c, તાર્કિક use_cache) અચલ
  {
    જો (ગણતરી == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      જો (use_cache)
	પાછા subtables[0].apply_cached_no_digest (c);
#endif
      પાછા subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    જો (use_cache)
    {
      પાછા
      + hb_iter (hb_iter (subtables, ગણતરી))
      | hb_map ([&c] (અચલ hb_accelerate_subtables_context_t::hb_applicable_t &_) { પાછા _.apply_cached (c); })
      | hb_any
      ;
    }
    નહીંતર
#endif
    {
      પાછા
      + hb_iter (hb_iter (subtables, ગણતરી))
      | hb_map ([&c] (અચલ hb_accelerate_subtables_context_t::hb_applicable_t &_) { પાછા _.apply (c); })
      | hb_any
      ;
    }
    પાછા ખોટું;
  }

  તાર્કિક cache_enter (hb_ot_apply_context_t *c) અચલ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    પાછા subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#નહીંતર
    પાછા ખોટું;
#endif
  }
  શૂન્ય cache_leave (hb_ot_apply_context_t *c) અચલ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  ખાનગી:
  unsigned ગણતરી = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

સંરચના GSUBGPOS
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

  size_t get_size () અચલ
  {
    જો (version.major != 1) પાછા version.static_size;

    પાછા min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#નહીંતર
	   0
#endif
	   ;
  }

  અચલ ScriptList &get_script_list () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u && scriptList2) પાછા આ + scriptList2;
#endif
    પાછા આ + scriptList;
  }
  અચલ FeatureList &get_feature_list () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u && featureList2) પાછા આ + featureList2;
#endif
    પાછા આ + featureList;
  }
  template <typename TLookup>
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (આ, get_size ())))
      return_trace (ખોટું);
    hb_barrier ();

    તાર્કિક script_list_ok = scriptList.sanitize (c, આ);
    તાર્કિક feature_list_ok = featureList.sanitize (c, આ);
    તાર્કિક lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, આ);

#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u)
    {
      જો (scriptList2) script_list_ok = scriptList2.sanitize (c, આ);
      જો (featureList2) feature_list_ok = featureList2.sanitize (c, આ);
      જો (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, આ);
    }
#endif

    જો (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (ખોટું);

#ifndef HB_NO_VAR
    જો (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, આ, &get_feature_list ()))))
      return_trace (ખોટું);
#endif

    return_trace (સાચું);
  }

  template <typename TLookup>
  તાર્કિક subset (hb_subset_layout_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    જો (version.major != 1) return_trace (ખોટું);

    auto *out = c->subset_context->serializer->start_embed (આ);
    જો (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (ખોટું);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u)
    {
      જો (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (ખોટું);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      જો (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 આ, c);
      નહીંતર
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 આ, c);

      જો (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 આ, c);
      નહીંતર
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 આ, c);

      જો (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, આ, c);
      નહીંતર
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, આ, c);
    }
    નહીંતર
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     આ,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     આ,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					આ,
					c);
    }

#ifndef HB_NO_VAR
    જો (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      જો (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (ખોટું);

      // if all axes are pinned all feature vars are dropped.
      તાર્કિક ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, આ, c);
      જો (!ret)
      {
        out->featureVars = 0;
	જો (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (સાચું);
  }

  unsigned પૂર્ણાંક get_lookup_ગણતરી () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u && lookupList2)
      પાછા (આ + lookupList2).len;
#endif
    પાછા (આ + lookupList).len;
  }
  અચલ Lookup& get_lookup (unsigned પૂર્ણાંક i) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u && lookupList2)
      પાછા (આ + lookupList2)[i];
#endif
    પાછા (આ + lookupList)[i];
  }
  અચલ FeatureVariations &get_feature_variations () અચલ
  {
    પાછા (version.to_int () >= 0x00010001u && hb_barrier ()) ? આ+featureVars : Null (FeatureVariations);
  }

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }
  unsigned પૂર્ણાંક get_script_ગણતરી () અચલ
  { પાછા get_script_list ().len; }
  અચલ Tag& get_script_tag (unsigned પૂર્ણાંક i) અચલ
  { પાછા get_script_list ().get_tag (i); }
  unsigned પૂર્ણાંક get_script_tags (unsigned પૂર્ણાંક start_offset,
				unsigned પૂર્ણાંક *script_ગણતરી /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) અચલ
  { પાછા get_script_list ().get_tags (start_offset, script_ગણતરી, script_tags); }
  અચલ Script& get_script (unsigned પૂર્ણાંક i) અચલ
  { પાછા get_script_list ()[i]; }
  તાર્કિક find_script_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  { પાછા get_script_list ().find_index (tag, index); }

  unsigned પૂર્ણાંક get_feature_ગણતરી () અચલ
  { પાછા get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned પૂર્ણાંક i) અચલ
  { પાછા i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned પૂર્ણાંક get_feature_tags (unsigned પૂર્ણાંક start_offset,
				 unsigned પૂર્ણાંક *feature_ગણતરી_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) અચલ
  { પાછા get_feature_list ().get_tags (start_offset, feature_ગણતરી_2, feature_tags); }
  અચલ Feature& get_feature (unsigned પૂર્ણાંક i) અચલ
  { પાછા get_feature_list ()[i]; }
  તાર્કિક find_feature_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  { પાછા get_feature_list ().find_index (tag, index); }

  તાર્કિક find_variations_index (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક અંક_coords,
			      unsigned પૂર્ણાંક *index,
			      ItemVarStoreInstancer *instancer) અચલ
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    પાછા ખોટું;
#endif
    પાછા get_feature_variations ().find_index (coords, અંક_coords, index, instancer);
  }
  તાર્કિક get_feature_variations_state (અચલ પૂર્ણાંક *coords,
				     unsigned પૂર્ણાંક અંક_coords,
				     unsigned પૂર્ણાંક *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) અચલ
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    પાછા સાચું;
#નહીંતર
    find_variations_index (coords, અંક_coords, index, instancer);
    પાછા get_feature_variations ().collect_lookup_conditions (
	coords,
	અંક_coords,
	instancer,
	lookup_conditions);
#endif
  }
  તાર્કિક get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<અચલ unsigned> conditions,
                              hb_set_t *lookup_indexes) અચલ
  {
#ifdef HB_NO_VAR
    પાછા ખોટું;
#નહીંતર
    પાછા get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  અચલ Feature& get_feature_variation (unsigned પૂર્ણાંક feature_index,
					unsigned પૂર્ણાંક variations_index) અચલ
  {
#ifndef HB_NO_VAR
    જો (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      અચલ Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      જો (feature)
	પાછા *feature;
    }
#endif
    પાછા get_feature (feature_index);
  }

  શૂન્ય feature_variation_collect_lookups (અચલ hb_set_t *feature_indexes,
					  અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) અચલ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  શૂન્ય collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  શૂન્ય collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) અચલ
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  શૂન્ય closure_lookups (hb_face_t      *face,
			અચલ hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) અચલ
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    માટે (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<અચલ TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  શૂન્ય prune_langsys (અચલ hb_map_t *duplicate_feature_map,
                      અચલ hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) અચલ
  {
    hb_prune_langsys_context_t c (આ, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned ગણતરી = get_script_ગણતરી ();
    માટે (unsigned script_index = 0; script_index < ગણતરી; script_index++)
    {
      અચલ Tag& tag = get_script_tag (script_index);
      જો (!layout_scripts->has (tag)) ચાલુ;
      અચલ Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  શૂન્ય prune_features (અચલ hb_map_t *lookup_indices, /* IN */
		       અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       અચલ hb_hashmap_t<unsigned, અચલ Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) અચલ
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    જો (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      પાછા;
    }
#endif

    માટે (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      જો (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        ચાલુ;


      અચલ Feature *f = &(get_feature (i));
      અચલ Feature** p = nullptr;
      જો (feature_substitutes_map->has (i, &p))
        f = *p;

      જો (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        ચાલુ;

      જો (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  શૂન્ય collect_name_ids (અચલ hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         અચલ hb_hashmap_t<unsigned, અચલ Feature*> *feature_substitutes_map,
                         અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         અચલ hb_set_t *catch_all_features,
                         તાર્કિક all_axes_pinned HB_UNUSED) અચલ
  {
    unsigned ગણતરી = get_feature_ગણતરી ();
    માટે (unsigned i = 0 ; i < ગણતરી; i++)
    {
      જો (!feature_index_map->has (i)) ચાલુ;
      hb_tag_t tag = get_feature_tag (i);
      અચલ Feature *feature = &get_feature (i);
      અચલ Feature **substitute;
      જો (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        જો (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    જો (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [આ] (unsigned i) { પાછા get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = સાચું;
      આ->table = sc.reference_table<T> (face);

      જો (unlikely (આ->table->is_blocklisted (આ->table.get_blob (), face)))
      {
	hb_blob_destroy (આ->table.get_blob ());
	આ->table = hb_blob_get_empty ();
      }

      આ->lookup_ગણતરી = table->get_lookup_ગણતરી ();

      આ->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (આ->lookup_ગણતરી, કદ (*accels));
      જો (unlikely (!આ->accels))
      {
	આ->lookup_ગણતરી = 0;
	આ->table.destroy ();
	આ->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      માટે (unsigned પૂર્ણાંક i = 0; i < આ->lookup_ગણતરી; i++)
      {
	auto *accel = આ->accels[i].get_relaxed ();
	જો (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (આ->accels);
      આ->table.destroy ();
    }

    hb_blob_t *get_blob () અચલ { પાછા table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) અચલ
    {
      જો (unlikely (lookup_index >= lookup_ગણતરી)) પાછા nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      જો (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	જો (unlikely (!accel))
	  પાછા nullptr;

	જો (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  જાઓ retry;
	}
      }

      પાછા accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned પૂર્ણાંક lookup_ગણતરી;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  સુરક્ષિત:
  template <typename TLookup>
  અચલ Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () અચલ
  { પાછા reinterpret_cast<અચલ Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  અચલ Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () અચલ
  { પાછા reinterpret_cast<અચલ Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  અચલ Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () અચલ
  { પાછા reinterpret_cast<અચલ Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { પાછા reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  અચલ Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () અચલ
  { પાછા reinterpret_cast<અચલ Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  અચલ Offset16To<RecordListOfFeature>& subset_feature_list_offset () અચલ
  { પાછા reinterpret_cast<અચલ Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { પાછા reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  અચલ Offset32To<RecordListOfFeature>& subset_feature_list2_offset () અચલ
  { પાછા reinterpret_cast<અચલ Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  અચલ શૂન્ય* get_lookup_list_field_offset () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.to_int () >= 0x00010002u && lookupList2) પાછા &lookupList2;
#endif
    પાછા &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  સ્થિર તાર્કિક serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       અચલ OffsetIn& in,
				       અચલ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    જો (in.is_null ()) પાછા ખોટું;

    auto *s = c->serializer;
    s->push ();
    તાર્કિક ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    જો (ret) s->add_link (out, s->કાઢો_pack ());
    નહીંતર s->કાઢો_discard ();
    પાછા ret;
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
  જાહેર:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
