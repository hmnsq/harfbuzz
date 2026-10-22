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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-buffer.hh"
#அந்தர்பாவ "hb-map.hh"
#அந்தர்பாவ "hb-set.hh"
#அந்தர்பாவ "hb-ot-map.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-ot-layout-gdef-table.hh"
#அந்தர்பாவ "hb-depend-data.hh"


நாமாகாஶ OT {


கட்டமைப்பு hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, தருக்க>
{
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { திரும்பு obj.intersects (இது->glyphs); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு பொய்; }
  தருக்க stop_sublookup_iteration (return_t r) மாறிலி { திரும்பு r; }

  மாறிலி hb_set_t *glyphs;

  hb_intersects_context_t (மாறிலி hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

கட்டமைப்பு hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, தருக்க>
{
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { திரும்பு obj.may_have_non_1to1 (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு பொய்; }
  தருக்க stop_sublookup_iteration (return_t r) மாறிலி { திரும்பு r; }
};

கட்டமைப்பு hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  வகைவரை return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { obj.closure (இது); திரும்பு hb_empty_t (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }
  வெறுமை recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    எனில் (unlikely (nesting_level_இடது == 0 || !recurse_func))
      திரும்பு;

    nesting_level_இடது--;
    recurse_func (இது, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_இடது++;
  }

  வெறுமை reset_lookup_visit_மொத்தம் ()
  { lookup_மொத்தம் = 0; }

  தருக்க lookup_limit_exceeded ()
  { திரும்பு lookup_மொத்தம் > HB_MAX_LOOKUP_VISIT_மொத்தம்; }

  தருக்க should_visit_lookup (unsigned முழு lookup_index)
  {
    எனில் (lookup_மொத்தம்++ > HB_MAX_LOOKUP_VISIT_மொத்தம்)
      திரும்பு பொய்;

    எனில் (is_lookup_done (lookup_index))
      திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தருக்க is_lookup_done (unsigned முழு lookup_index)
  {
    எனில் (unlikely (done_lookups_glyph_மொத்தம்->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      திரும்பு உண்மை;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned எடு = glyphs->get_population ();
    எனில் (unlikely (எடு == HB_MAP_மதிப்பு_INVALID)) // Avoid sentinel interference.
      எடு--;
    எனில் (done_lookups_glyph_மொத்தம்->get (lookup_index) != எடு)
    {
      done_lookups_glyph_மொத்தம்->set (lookup_index, எடு);

      எனில் (!done_lookups_glyph_set->has (lookup_index))
      {
	எனில் (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  திரும்பு உண்மை;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    எனில் (unlikely (covered_glyph_set->in_error ()))
      திரும்பு உண்மை;
    எனில் (parent_active_glyphs ().is_subset (*covered_glyph_set))
      திரும்பு உண்மை;

    covered_glyph_set->union_ (parent_active_glyphs ());
    திரும்பு பொய்;
  }

  மாறிலி hb_set_t& previous_parent_active_glyphs () {
    எனில் (active_glyphs_அடுக்கு.length <= 1)
      திரும்பு *glyphs;

    திரும்பு active_glyphs_அடுக்கு[active_glyphs_அடுக்கு.length - 2];
  }

  மாறிலி hb_set_t& parent_active_glyphs ()
  {
    எனில் (!active_glyphs_அடுக்கு)
      திரும்பு *glyphs;

    திரும்பு active_glyphs_அடுக்கு.tail ();
  }

  hb_set_t* தள்ளு_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_அடுக்கு.push ();
    எனில் (unlikely (active_glyphs_அடுக்கு.in_error ()))
      திரும்பு nullptr;
    திரும்பு s;
  }

  தருக்க எடு_cur_done_glyphs ()
  {
    எனில் (!active_glyphs_அடுக்கு)
      திரும்பு பொய்;

    active_glyphs_அடுக்கு.எடு ();
    திரும்பு உண்மை;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_அடுக்கு;
  recurse_func_t recurse_func = nullptr;
  unsigned முழு nesting_level_இடது;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_மொத்தம்_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned முழு nesting_level_இடது_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_இடது (nesting_level_இடது_2),
			  done_lookups_glyph_மொத்தம் (done_lookups_glyph_மொத்தம்_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  வெறுமை set_recurse_func (recurse_func_t func) { recurse_func = func; }

  வெறுமை flush ()
  {
    output->del_range (face->get_இலக்கம்_glyphs (), HB_SET_மதிப்பு_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_அடுக்கு.எடு ();
    active_glyphs_அடுக்கு.reset ();
  }

  தனிப்பட்ட:
  hb_map_t *done_lookups_glyph_மொத்தம்;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned முழு lookup_மொத்தம் = 0;
};



கட்டமைப்பு hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  வகைவரை return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { obj.closure_lookups (இது); திரும்பு hb_empty_t (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }
  வெறுமை recurse (unsigned lookup_index)
  {
    எனில் (unlikely (nesting_level_இடது == 0 || !recurse_func))
      திரும்பு;

    /* Return if new lookup was recursed to before. */
    எனில் (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      திரும்பு;

    nesting_level_இடது--;
    recurse_func (இது, lookup_index);
    nesting_level_இடது++;
  }

  வெறுமை set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  வெறுமை set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  தருக்க lookup_limit_exceeded ()
  {
    தருக்க ret = lookup_மொத்தம் > HB_MAX_LOOKUP_VISIT_மொத்தம்;
    எனில் (ret)
      DEBUG_செய்தி (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    திரும்பு ret; }

  தருக்க is_lookup_visited (unsigned lookup_index)
  {
    எனில் (unlikely (lookup_மொத்தம்++ > HB_MAX_LOOKUP_VISIT_மொத்தம்))
    {
      DEBUG_செய்தி (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_மொத்தம், lookup_index);
      திரும்பு உண்மை;
    }

    எனில் (unlikely (visited_lookups->in_error ()))
      திரும்பு உண்மை;

    திரும்பு visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  மாறிலி hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned முழு nesting_level_இடது;

  hb_closure_lookups_context_t (hb_face_t *face_,
				மாறிலி hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_இடது_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_இடது (nesting_level_இடது_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_மொத்தம் (0) {}

  வெறுமை set_recurse_func (recurse_func_t func) { recurse_func = func; }

  தனிப்பட்ட:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned முழு lookup_மொத்தம்;
};

கட்டமைப்பு hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, தருக்க>
{
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { திரும்பு obj.would_apply (இது); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு பொய்; }
  தருக்க stop_sublookup_iteration (return_t r) மாறிலி { திரும்பு r; }

  hb_face_t *face;
  மாறிலி hb_codepoint_t *glyphs;
  unsigned முழு len;
  தருக்க zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    மாறிலி hb_codepoint_t *glyphs_,
			    unsigned முழு len_,
			    தருக்க zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

கட்டமைப்பு hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  வகைவரை return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned முழு lookup_index);
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { obj.collect_glyphs (இது); திரும்பு hb_empty_t (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }
  வெறுமை recurse (unsigned முழு lookup_index)
  {
    எனில் (unlikely (nesting_level_இடது == 0 || !recurse_func))
      திரும்பு;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    எனில் (output == hb_set_get_empty ())
      திரும்பு;

    /* Return if new lookup was recursed to before. */
    எனில் (recursed_lookups->has (lookup_index))
      திரும்பு;

    hb_set_t *old_before = before;
    hb_set_t *old_உள்ளீடு  = உள்ளீடு;
    hb_set_t *old_after  = after;
    before = உள்ளீடு = after = hb_set_get_empty ();

    nesting_level_இடது--;
    recurse_func (இது, lookup_index);
    nesting_level_இடது++;

    before = old_before;
    உள்ளீடு  = old_உள்ளீடு;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *உள்ளீடு;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned முழு nesting_level_இடது;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_உள்ளீடு,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned முழு nesting_level_இடது_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      உள்ளீடு  (glyphs_உள்ளீடு  ? glyphs_உள்ளீடு  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_இடது (nesting_level_இடது_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  வெறுமை set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

கட்டமைப்பு hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  எண்ணிக்கை glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  நிலையான constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  கட்டமைப்பு glyph_set_cache_சாவி_t
  {
    glyph_set_cache_சாவி_t () = இயல்பு;
    glyph_set_cache_சாவி_t (மாறிலி வெறுமை *data_, unsigned மதிப்பு_3, glyph_set_type_t type_)
      : data (data_), value (மதிப்பு_3), type (type_) {}

    தருக்க operator == (மாறிலி glyph_set_cache_சாவி_t &o) மாறிலி
    { திரும்பு data == o.data && value == o.value && type == o.type; }

    uint32_t hash () மாறிலி
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      திரும்பு current * 31 + hb_hash ((unsigned) type);
    }

    மாறிலி வெறுமை *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  கட்டமைப்பு context_set_cache_சாவி_t
  {
    context_set_cache_சாவி_t () = இயல்பு;
    context_set_cache_சாவி_t (மாறிலி வெறுமை *rule_,
			     மாறிலி வெறுமை *record_,
			     மாறிலி வெறுமை *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned மதிப்பு_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (மதிப்பு_3), format (format_) {}

    தருக்க operator == (மாறிலி context_set_cache_சாவி_t &o) மாறிலி
    {
      திரும்பு rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () மாறிலி
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      திரும்பு current * 31 + hb_hash (format);
    }

    மாறிலி வெறுமை *rule = nullptr;
    மாறிலி வெறுமை *record = nullptr;
    மாறிலி வெறுமை *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  கட்டமைப்பு recurse_சாவி_t
  {
    recurse_சாவி_t () = இயல்பு;
    recurse_சாவி_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    தருக்க operator == (மாறிலி recurse_சாவி_t &o) மாறிலி
    {
      திரும்பு lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () மாறிலி
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      திரும்பு current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  வகைவரை return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { obj.depend (இது); திரும்பு hb_empty_t (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  வெறுமை recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    எனில் (lookups_seen.has (lookup_idx)) திரும்பு;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_சாவி = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    எனில் (!recurse_path_to_index.has (path_சாவி, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      எனில் (unlikely (!recurse_path_to_index.set (path_சாவி, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	திரும்பு;
      }
    }
    இல்லையெனில்
      path_index = *path;
    recurse_path = path_index;
    recurse_func (இது, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  கட்டமைப்பு active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    தருக்க use_வேர் = பொய்;
  };

  மாறிலி hb_set_t& parent_active_glyphs ()
  {
    எனில் (!active_glyphs_அடுக்கு_depth)
      திரும்பு *glyphs;

    மாறிலி active_glyphs_frame_t &frame = active_glyphs_அடுக்கு[active_glyphs_அடுக்கு_depth - 1];
    திரும்பு frame.use_வேர் ? *glyphs : frame.glyphs;
  }

  hb_set_t* தள்ளு_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    எனில் (active_glyphs_அடுக்கு_depth == active_glyphs_அடுக்கு.length &&
	unlikely (!active_glyphs_அடுக்கு.தள்ளு_or_fail ()))
      திரும்பு nullptr;

    active_glyphs_frame_t &frame = active_glyphs_அடுக்கு[active_glyphs_அடுக்கு_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_வேர் = பொய்;
    திரும்பு &frame.glyphs;
  }

  தருக்க தள்ளு_வேர்_active_glyphs ()
  {
    எனில் (active_glyphs_அடுக்கு_depth == active_glyphs_அடுக்கு.length &&
	unlikely (!active_glyphs_அடுக்கு.தள்ளு_or_fail ()))
      திரும்பு பொய்;

    active_glyphs_அடுக்கு[active_glyphs_அடுக்கு_depth++].use_வேர் = உண்மை;
    திரும்பு உண்மை;
  }

  தருக்க எடு_cur_done_glyphs ()
  {
    எனில் (!active_glyphs_அடுக்கு_depth)
      திரும்பு பொய்;

    active_glyphs_அடுக்கு_depth--;
    திரும்பு உண்மை;
  }

  தருக்க get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    எனில் (!active_glyphs_அடுக்கு_depth ||
	active_glyphs_அடுக்கு[active_glyphs_அடுக்கு_depth - 1].use_வேர்)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      திரும்பு உண்மை;
    }

    active_glyphs_frame_t &frame = active_glyphs_அடுக்கு[active_glyphs_அடுக்கு_depth - 1];
    *active_idx = frame.active_idx;
    எனில் (*active_idx != HB_CODEPOINT_INVALID)
      திரும்பு உண்மை;

    hb_codepoint_t glyph;
    எனில் (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    இல்லையெனில் எனில் (!find_or_create_recurse_set (frame.glyphs, active_idx))
      திரும்பு பொய்;

    frame.active_idx = *active_idx;
    திரும்பு உண்மை;
  }

  தருக்க get_context_set_cache_சாவி (மாறிலி வெறுமை *rule,
				  மாறிலி வெறுமை *record,
				  மாறிலி வெறுமை *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_சாவி_t *key)
  {
    hb_codepoint_t active_idx;
    எனில் (!get_parent_active_glyphs_index (&active_idx))
      திரும்பு பொய்;
    *key = context_set_cache_சாவி_t (rule, record, data, active_idx, value, format);
    திரும்பு உண்மை;
  }

  தருக்க get_recurse_சாவி (unsigned lookup_idx, recurse_சாவி_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    எனில் (!get_parent_active_glyphs_index (&active_idx))
      திரும்பு பொய்;

    *key = recurse_சாவி_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    திரும்பு !completed_recursions.has (*key);
  }

  வெறுமை finish_recurse (மாறிலி recurse_சாவி_t &key)
  { depend_data->check_success (completed_recursions.set (key, உண்மை)); }

  வெறுமை reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  தருக்க find_or_create_recurse_set (மாறிலி hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    எனில் (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      திரும்பு உண்மை;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    எனில் (unlikely (!copy))
      திரும்பு depend_data->fail ();
    copy->set (set);
    எனில் (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      திரும்பு depend_data->fail ();
    }

    *index = recurse_sets.length;
    எனில் (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      திரும்பு depend_data->fail ();
    }
    எனில் (unlikely (!recurse_sets.தள்ளு_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      திரும்பு depend_data->fail ();
    எனில் (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      திரும்பு depend_data->fail ();
    திரும்பு உண்மை;
  }

  மாறிலி hb_set_t *find_or_create_glyph_set (மாறிலி வெறுமை *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (மாறிலி hb_set_t *set);

  கட்டமைப்பு depend_scratch_t
  {
    வெறுமை clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      உள்ளீடு_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<மாறிலி hb_set_t *> backtrack_sets;
    hb_vector_t<மாறிலி hb_set_t *> lookahead_sets;
    hb_vector_t<மாறிலி hb_set_t *> உள்ளீடு_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    எனில் (scratch)
      depend_scratch_pool = scratch->next;
    இல்லையெனில்
    {
      scratch = (depend_scratch_t *) hb_malloc (அளவு (depend_scratch_t));
      எனில் (unlikely (!scratch))
      {
        depend_data->fail ();
        திரும்பு nullptr;
      }
      புதிய (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    திரும்பு scratch;
  }

  வெறுமை release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_அடுக்கு;
  unsigned active_glyphs_அடுக்கு_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<மாறிலி hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_சாவி_t, தருக்க> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_சாவி_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_சாவி_t, hb_codepoint_t> context_set_cache;
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
    போது (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  வெறுமை set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
கட்டமைப்பு hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, மாறிலி Coverage &>
{
  வகைவரை மாறிலி Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { திரும்பு obj.get_coverage (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு Null (Coverage); }
  தருக்க stop_sublookup_iteration (return_t r) மாறிலி
  {
    r.collect_coverage (set);
    திரும்பு பொய்;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

கட்டமைப்பு matcher_t
{
  வகைவரை தருக்க (*match_func_t) (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data);

  template <typename context_t>
  வெறுமை init (மாறிலி context_t *c, தருக்க context_match = பொய்)
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

  வெறுமை set_match_func (match_func_t match_func_,
		       மாறிலி வெறுமை *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  எண்ணிக்கை may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) மாறிலி
  {
    எனில் (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      திரும்பு MATCH_NO;

    எனில் (match_func)
      திரும்பு match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    திரும்பு MATCH_MAYBE;
  }

  எண்ணிக்கை may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (மாறிலி context_t *c,
		       மாறிலி hb_glyph_info_t &info) மாறிலி
  {
    எனில் (!c->check_glyph_property (&info, lookup_props))
      திரும்பு SKIP_YES;

    எனில் (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      திரும்பு SKIP_MAYBE;

    திரும்பு SKIP_NO;
  }

  பொது:
  unsigned முழு lookup_props = 0;
  hb_mask_t mask = -1;
  தருக்க ignore_zwnj = பொய்;
  தருக்க ignore_zwj = பொய்;
  தருக்க ignore_hidden = பொய்;
  தருக்க per_syllable = பொய்;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  மாறிலி வெறுமை *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
கட்டமைப்பு glyph_data_t
{
  template <typename T>
  glyph_data_t (மாறிலி T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (அளவு (T) == 3)
#endif
  {
    static_assert (அளவு (T) == 2 || அளவு (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (wide)
      திரும்பு ((மாறிலி HBUINT24 *) data)[i];
#endif
    திரும்பு ((மாறிலி HBUINT16 *) data)[i];
  }

  மாறிலி வெறுமை *data;
#ifndef HB_NO_BEYOND_64K
  தருக்க wide;
#endif
};

template <typename context_t>
கட்டமைப்பு skipping_iterator_t
{
  வெறுமை init (context_t *c_, தருக்க context_match = பொய்)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  வெறுமை set_lookup_props (unsigned முழு lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  வெறுமை set_match_func (matcher_t::match_func_t match_func_,
		       மாறிலி வெறுமை *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  வெறுமை set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (மாறிலி HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (மாறிலி HBUINT24 *) glyph_data.data : nullptr;
#இல்லையெனில்
    match_glyph_data16 = (மாறிலி HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  வெறுமை reset (unsigned முழு start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  வெறுமை reset_back (unsigned முழு start_index_, தருக்க from_out_buffer = பொய்)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  வெறுமை reset_fast (unsigned முழு start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (மாறிலி hb_glyph_info_t &info) மாறிலி
  { திரும்பு matcher.may_skip (c, info); }

  எண்ணிக்கை match_t {
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
    எனில் (unlikely (skip == matcher_t::SKIP_YES))
      திரும்பு SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    எனில் (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      திரும்பு MATCH;

    எனில் (skip == matcher_t::SKIP_NO)
      திரும்பு NOT_MATCH;

    திரும்பு SKIP;
  }

  தருக்க next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    மாறிலி signed stop = (signed) end - 1;
    போது ((signed) idx < stop)
    {
      idx++;
      தேர்வு (match (info[idx]))
      {
	நிலை MATCH:
	{
	  advance_glyph_data ();
	  திரும்பு உண்மை;
	}
	நிலை NOT_MATCH:
	{
	  எனில் (unsafe_to)
	    *unsafe_to = idx + 1;
	  திரும்பு பொய்;
	}
	நிலை SKIP:
	  தொடர்;
      }
    }
    எனில் (unsafe_to)
      *unsafe_to = end;
    திரும்பு பொய்;
  }
  தருக்க prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    மாறிலி unsigned stop = 0;
    போது (idx > stop)
    {
      idx--;
      தேர்வு (match (out_info[idx]))
      {
	நிலை MATCH:
	{
	  advance_glyph_data ();
	  திரும்பு உண்மை;
	}
	நிலை NOT_MATCH:
	{
	  எனில் (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  திரும்பு பொய்;
	}
	நிலை SKIP:
	  தொடர்;
      }
    }
    எனில் (unsafe_from)
      *unsafe_from = 0;
    திரும்பு பொய்;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    எனில் (match_glyph_data16) திரும்பு *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    இல்லையெனில்
    எனில் (match_glyph_data24) திரும்பு *match_glyph_data24;
#endif
    திரும்பு 0;
  }
  HB_ALWAYS_INLINE
  வெறுமை
  advance_glyph_data ()
  {
    எனில் (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    இல்லையெனில்
    எனில் (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned முழு idx;
  பாதுகாக்கப்பட்ட:
  context_t *c;
  matcher_t matcher;
  மாறிலி HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  மாறிலி HBUINT24 *match_glyph_data24;
#endif

  unsigned முழு end;
};

கட்டமைப்பு hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, தருக்க, HB_DEBUG_APPLY>
{
  மாறிலி எழுத்து *get_name () { திரும்பு "APPLY"; }
  வகைவரை return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned முழு lookup_index);

  template <typename T>
  நிலையான உள்வரி auto apply_ (மாறிலி T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  நிலையான உள்வரி auto apply_ (மாறிலி T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { திரும்பு apply_(obj, இது, hb_prioritize); }

  நிலையான return_t default_return_மதிப்பு () { திரும்பு பொய்; }
  தருக்க stop_sublookup_iteration (return_t r) மாறிலி { திரும்பு r; }
  return_t recurse (unsigned முழு sub_lookup_index)
  {
    ப்ரதிஜ்ஞா (recurse_func);
    எனில் (unlikely (nesting_level_இடது == 0))
    {
      buffer->successful = பொய்;
      திரும்பு default_return_மதிப்பு ();
    }

    buffer->max_ops--;
    எனில் (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = பொய்;
      திரும்பு default_return_மதிப்பு ();
    }

    nesting_level_இடது--;
    தருக்க ret = recurse_func (இது, sub_lookup_index);
    nesting_level_இடது++;
    திரும்பு ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_உள்ளீடு, iter_context;

  unsigned முழு table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  மாறிலி GDEF &gdef;
  மாறிலி GDEF::accelerator_t &gdef_accel;
  மாறிலி ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned முழு lookup_index = (unsigned) -1;
  unsigned முழு lookup_props = 0;
  unsigned முழு nesting_level_இடது = HB_MAX_NESTING_LEVEL;

  தருக்க has_glyph_classes;
  தருக்க auto_zwnj = உண்மை;
  தருக்க auto_zwj = உண்மை;
  தருக்க per_syllable = பொய்;
  தருக்க random = பொய்;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t அடுக்கு_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  வெறுமை *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned முழு table_index_,
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
#இல்லையெனில்
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#இல்லையெனில்
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (அடுக்கு_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    எனில் (buffer->messaging ())
    {
      ப்ரதிஜ்ஞா (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = இது;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    எனில் (buffer->messaging ())
    {
      ப்ரதிஜ்ஞா (buffer->changed_func == buffer_changed_trampoline);
      ப்ரதிஜ்ஞா (buffer->changed_data == இது);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  வெறுமை init_iters ()
  {
    iter_உள்ளீடு.init (இது, பொய்);
    iter_context.init (இது, உண்மை);
  }

  வெறுமை set_lookup_mask (hb_mask_t mask, தருக்க init = உண்மை) { lookup_mask = mask; last_base = -1; last_base_until = 0; எனில் (init) init_iters (); }
  வெறுமை set_auto_zwj (தருக்க auto_zwj_, தருக்க init = உண்மை) { auto_zwj = auto_zwj_; எனில் (init) init_iters (); }
  வெறுமை set_auto_zwnj (தருக்க auto_zwnj_, தருக்க init = உண்மை) { auto_zwnj = auto_zwnj_; எனில் (init) init_iters (); }
  வெறுமை set_per_syllable (தருக்க per_syllable_, தருக்க init = உண்மை) { per_syllable = per_syllable_; எனில் (init) init_iters (); }
  வெறுமை set_random (தருக்க random_) { random = random_; }
  வெறுமை set_recurse_func (recurse_func_t func) { recurse_func = func; }
  வெறுமை set_lookup_index (unsigned முழு lookup_index_) { lookup_index = lookup_index_; }
  வெறுமை set_lookup_props (unsigned முழு lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_எண் ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    திரும்பு buffer->random_state;
  }

  நிலையான வெறுமை buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, வெறுமை *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  வெறுமை buffer_changed ()
  {
    buffer->update_digest ();
    எனில் (likely (has_glyph_classes))
      க்கு (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  தருக்க match_properties_mark (மாறிலி hb_glyph_info_t *info,
			      unsigned முழு    glyph_props,
			      unsigned முழு    match_props) மாறிலி
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    எனில் (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      திரும்பு gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    எனில் (match_props & LookupFlag::MarkAttachmentType)
      திரும்பு (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    திரும்பு உண்மை;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க check_glyph_property (மாறிலி hb_glyph_info_t *info,
			     unsigned match_props) மாறிலி
  {
    unsigned முழு glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    எனில் (glyph_props & match_props & LookupFlag::IgnoreFlags)
      திரும்பு பொய்;

    எனில் (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      திரும்பு match_properties_mark (info, glyph_props, match_props);

    திரும்பு உண்மை;
  }

  வெறுமை _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned முழு class_guess = 0,
			  தருக்க ligature = பொய்,
			  தருக்க component = பொய்)
  {
    buffer->digest.add (glyph_index);

    எனில் (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned முழு props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    எனில் (ligature)
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
    எனில் (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    எனில் (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    இல்லையெனில் எனில் (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    இல்லையெனில்
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  வெறுமை _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned முழு props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  வெறுமை _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  வெறுமை replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (வெறுமை) buffer->replace_glyph (glyph_index);
  }
  வெறுமை replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  வெறுமை replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned முழு class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, உண்மை);
    (வெறுமை) buffer->replace_glyph (glyph_index);
  }
  வெறுமை output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned முழு class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, பொய், உண்மை);
    (வெறுமை) buffer->output_glyph (glyph_index);
  }
};

எண்ணிக்கை வகுப்பு hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

கட்டமைப்பு hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  நிலையான உள்வரி auto collect_second_glyphs_ (மாறிலி T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (வெறுமை, obj.collect_second_glyphs (digest) )
  template <typename T>
  நிலையான உள்வரி வெறுமை collect_second_glyphs_ (மாறிலி T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  நிலையான உள்வரி auto apply_ (மாறிலி T *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache, hb_priority<1>) HB_RETURN (தருக்க, obj->apply (c, external_cache) )
  template <typename T>
  நிலையான உள்வரி auto apply_ (மாறிலி T *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache, hb_priority<0>) HB_RETURN (தருக்க, obj->apply (c) )
  template <typename T>
  நிலையான உள்வரி தருக்க apply_to (மாறிலி வெறுமை *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache)
  {
    மாறிலி T *typed_obj = (மாறிலி T *) obj;
    திரும்பு apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  நிலையான உள்வரி auto apply_cached_ (மாறிலி T *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache, hb_priority<2>) HB_RETURN (தருக்க, obj->apply_cached (c, external_cache) )
  template <typename T>
  நிலையான உள்வரி auto apply_cached_ (மாறிலி T *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache, hb_priority<1>) HB_RETURN (தருக்க, obj->apply (c, external_cache) )
  template <typename T>
  நிலையான உள்வரி auto apply_cached_ (மாறிலி T *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache, hb_priority<0>) HB_RETURN (தருக்க, obj->apply (c) )
  template <typename T>
  நிலையான உள்வரி தருக்க apply_cached_to (மாறிலி வெறுமை *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache)
  {
    மாறிலி T *typed_obj = (மாறிலி T *) obj;
    திரும்பு apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  நிலையான உள்வரி auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (தருக்க, T::cache_func (c, op) )
  template <typename T=வெறுமை>
  நிலையான உள்வரி தருக்க cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { திரும்பு பொய்; }
  template <typename Type>
  நிலையான உள்வரி தருக்க cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    திரும்பு cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  வகைவரை தருக்க (*hb_apply_func_t) (மாறிலி வெறுமை *obj, hb_ot_apply_context_t *c, வெறுமை *external_cache);
  வகைவரை தருக்க (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  கட்டமைப்பு hb_applicable_t
  {
    friend கட்டமைப்பு hb_accelerate_subtables_context_t;
    friend கட்டமைப்பு hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    வெறுமை init (மாறிலி T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , வெறுமை *external_cache_
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
    தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    தருக்க apply_no_digest (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு apply_func (obj, c, nullptr);
    }
#இல்லையெனில்
    தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    தருக்க apply_cached (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    தருக்க apply_no_digest (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு apply_func (obj, c, external_cache);
    }
    தருக்க apply_cached_no_digest (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு apply_cached_func (obj, c, external_cache);
    }

    தருக்க cache_enter (hb_ot_apply_context_t *c) மாறிலி
    {
      திரும்பு cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    வெறுமை cache_leave (hb_ot_apply_context_t *c) மாறிலி
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    தனிப்பட்ட:
    மாறிலி வெறுமை *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    வெறுமை *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (மாறிலி T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (மாறிலி T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (மாறிலி T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (மாறிலி T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (மாறிலி T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    வெறுமை *external_cache = nullptr;
    எனில் (i < 8)
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
    எனில் (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    திரும்பு hb_empty_t ();
  }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

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


வகைவரை தருக்க (*intersects_func_t) (மாறிலி hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data, வெறுமை *cache);
வகைவரை வெறுமை (*intersected_glyphs_func_t) (மாறிலி hb_set_t *glyphs, மாறிலி வெறுமை *data, unsigned value, hb_set_t *intersected_glyphs, வெறுமை *cache);
வகைவரை வெறுமை (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data);
வகைவரை தருக்க (*match_func_t) (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data);
வகைவரை unsigned (*get_மதிப்பு_func_t) (hb_glyph_info_t &info, மாறிலி வெறுமை *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
கட்டமைப்பு hb_ot_layout_ruleset_digest_t
{
  வெறுமை init () { mask = 0; }
  வெறுமை set_full () { mask = (uint64_t) -1; }
  வெறுமை add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  தருக்க may_have (unsigned value) மாறிலி { திரும்பு mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

கட்டமைப்பு ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
கட்டமைப்பு ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
கட்டமைப்பு ContextApplyFuncs
{
  match_func_t match;
};
கட்டமைப்பு ChainContextApplyFuncs
{
  match_func_t match[3];
};


நிலையான உள்வரி தருக்க intersects_glyph (மாறிலி hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data HB_UNUSED, வெறுமை *cache HB_UNUSED)
{
  திரும்பு glyphs->has (value);
}
நிலையான உள்வரி தருக்க intersects_class (மாறிலி hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data, வெறுமை *cache)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  எனில் (map->has (value, &cached_v))
    திரும்பு *cached_v;

  தருக்க v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  திரும்பு v;
}
நிலையான உள்வரி தருக்க intersects_coverage (மாறிலி hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data, வெறுமை *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  திரும்பு (data+coverage).intersects (glyphs);
}


நிலையான உள்வரி வெறுமை intersected_glyph (மாறிலி hb_set_t *glyphs HB_UNUSED, மாறிலி வெறுமை *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED வெறுமை *cache)
{
  unsigned g = reinterpret_cast<மாறிலி HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

நிலையான உள்வரி வெறுமை intersected_class_glyphs (மாறிலி hb_set_t *glyphs, மாறிலி வெறுமை *data, unsigned value, hb_set_t *intersected_glyphs, வெறுமை *cache)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  எனில் (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    திரும்பு;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

நிலையான உள்வரி வெறுமை intersected_coverage_glyphs (மாறிலி hb_set_t *glyphs, மாறிலி வெறுமை *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED வெறுமை *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


நிலையான உள்வரி தருக்க array_is_subset_of (மாறிலி hb_set_t *glyphs,
				       unsigned முழு மொத்தம்,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       மாறிலி வெறுமை *intersects_data,
				       வெறுமை *cache)
{
  க்கு (unsigned i = 0; i < மொத்தம்; i++)
    எனில் (!intersects_func (glyphs, values[i], intersects_data, cache)) திரும்பு பொய்;
  திரும்பு உண்மை;
}


நிலையான உள்வரி வெறுமை collect_glyph (hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data HB_UNUSED)
{
  glyphs->add (value);
}
நிலையான உள்வரி வெறுமை collect_class (hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
நிலையான உள்வரி வெறுமை collect_coverage (hb_set_t *glyphs, unsigned value, மாறிலி வெறுமை *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

உள்வரி மாறிலி hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (மாறிலி வெறுமை *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_சாவி_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  எனில் (glyph_set_to_index.has (key, &existing))
    திரும்பு glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  எனில் (unlikely (!set))
  {
    depend_data->fail ();
    திரும்பு nullptr;
  }

  எனில் (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  இல்லையெனில் எனில் (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  இல்லையெனில்
    set->add (value);
  எனில் (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    திரும்பு nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  எனில் (unlikely (!glyph_sets.தள்ளு_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    திரும்பு nullptr;
  }
  எனில் (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    திரும்பு nullptr;
  }
  திரும்பு glyph_sets[index].get ();
}

உள்வரி hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (மாறிலி hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  எனில் (cached_context_set_indices.has (key, &existing))
    திரும்பு *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  எனில் (unlikely (index == HB_CODEPOINT_INVALID))
    திரும்பு HB_CODEPOINT_INVALID;
  எனில் (unlikely (!cached_context_set_indices.set (key, index)))
    திரும்பு depend_data->fail_invalid ();
  திரும்பு index;
}

நிலையான உள்வரி வெறுமை collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned முழு மொத்தம்,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  மாறிலி வெறுமை *collect_data)
{
  க்கு (unsigned i = 0; i < மொத்தம்; i++)
    collect_func (glyphs, values[i], collect_data);
}


நிலையான உள்வரி தருக்க match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, மாறிலி வெறுமை *data HB_UNUSED)
{
  திரும்பு உண்மை;
}
நிலையான உள்வரி தருக்க match_glyph (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data HB_UNUSED)
{
  திரும்பு info.codepoint == value;
}
நிலையான உள்வரி தருக்க match_class (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);
  திரும்பு class_def.get_class (info.codepoint) == value;
}
நிலையான உள்வரி unsigned get_class_மதிப்பு (hb_glyph_info_t &info, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *> (data);
  திரும்பு class_def.get_class (info.codepoint);
}
கட்டமைப்பு match_class_cache_data_t
{
  மாறிலி ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
நிலையான உள்வரி தருக்க match_class_cache (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data)
{
  மாறிலி auto &cache_data = *reinterpret_cast<மாறிலி match_class_cache_data_t *> (data);
  திரும்பு cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
நிலையான உள்வரி unsigned get_class_cache_மதிப்பு (hb_glyph_info_t &info, மாறிலி வெறுமை *data)
{
  மாறிலி auto &cache_data = *reinterpret_cast<மாறிலி match_class_cache_data_t *> (data);
  திரும்பு cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
நிலையான உள்வரி unsigned get_class_cached (மாறிலி ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  எனில் (klass < 255)
    திரும்பு klass;
  klass = class_def.get_class (info.codepoint);
  எனில் (likely (klass < 255))
    info.syllable() = klass;
  திரும்பு klass;
}
நிலையான உள்வரி தருக்க match_class_cached (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);
  திரும்பு get_class_cached (class_def, info) == value;
}
நிலையான உள்வரி unsigned get_class_cached_மதிப்பு (hb_glyph_info_t &info, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *> (data);
  திரும்பு get_class_cached (class_def, info);
}
நிலையான உள்வரி unsigned get_class_cached1 (மாறிலி ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  எனில் (klass < 15)
    திரும்பு klass;
  klass = class_def.get_class (info.codepoint);
  எனில் (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  திரும்பு klass;
}
நிலையான உள்வரி தருக்க match_class_cached1 (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);
  திரும்பு get_class_cached1 (class_def, info) == value;
}
நிலையான உள்வரி unsigned get_class_cached2 (மாறிலி ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  எனில் (klass < 15)
    திரும்பு klass;
  klass = class_def.get_class (info.codepoint);
  எனில் (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  திரும்பு klass;
}
நிலையான உள்வரி தருக்க match_class_cached2 (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *>(data);
  திரும்பு get_class_cached2 (class_def, info) == value;
}
நிலையான உள்வரி unsigned get_class_cached2_மதிப்பு (hb_glyph_info_t &info, மாறிலி வெறுமை *data)
{
  மாறிலி ClassDef &class_def = *reinterpret_cast<மாறிலி ClassDef *> (data);
  திரும்பு get_class_cached2 (class_def, info);
}
நிலையான உள்வரி தருக்க match_coverage (hb_glyph_info_t &info, unsigned value, மாறிலி வெறுமை *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  திரும்பு (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

நிலையான உள்வரி தருக்க would_match_உள்ளீடு (hb_would_apply_context_t *c,
				      unsigned முழு மொத்தம், /* Including the first glyph (not matched) */
				      glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      மாறிலி வெறுமை *match_data)
{
  எனில் (மொத்தம் != c->len)
    திரும்பு பொய்;

  க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    எனில் (likely (!match_func (info, உள்ளீடு[i - 1], match_data)))
      திரும்பு பொய்;
  }

  திரும்பு உண்மை;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#இல்லையெனில்
உள்வரி
#endif
நிலையான தருக்க match_உள்ளீடு (hb_ot_apply_context_t *c,
			 unsigned முழு மொத்தம், /* Including the first glyph (not matched) */
			 glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 மாறிலி வெறுமை *match_data,
			 unsigned முழு *end_position,
			 unsigned முழு *p_total_component_மொத்தம் = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  எனில் (மொத்தம் == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    எனில் (p_total_component_மொத்தம்)
      *p_total_component_மொத்தம் = hb_glyph_info_get_lig_இலக்கம்_comps (&buffer->cur());
    return_trace (உண்மை);
  }

  எனில் (unlikely (மொத்தம் > HB_MAX_CONTEXT_LENGTH)) return_trace (பொய்);

  auto &skippy_iter = c->iter_உள்ளீடு;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (உள்ளீடு);

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

  unsigned முழு total_component_மொத்தம் = 0;

  unsigned முழு first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned முழு first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  எண்ணிக்கை {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
  {
    unsigned unsafe_to;
    எனில் (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (பொய்);
    }

    எனில் (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (பொய்);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned முழு this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned முழு this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    எனில் (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      எனில் (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	எனில் (ligbase == LIGBASE_NOT_CHECKED)
	{
	  தருக்க found = பொய்;
	  மாறிலி auto *out = buffer->out_info;
	  unsigned முழு j = buffer->out_len;
	  போது (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    எனில் (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = உண்மை;
	      நிறுத்து;
	    }
	    j--;
	  }

	  எனில் (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  இல்லையெனில்
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	எனில் (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (பொய்);
      }
    }
    இல்லையெனில்
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      எனில் (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (பொய்);
    }

    total_component_மொத்தம் += hb_glyph_info_get_lig_இலக்கம்_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  எனில் (p_total_component_மொத்தம்)
  {
    total_component_மொத்தம் += hb_glyph_info_get_lig_இலக்கம்_comps (&buffer->cur());
    *p_total_component_மொத்தம் = total_component_மொத்தம்;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (உண்மை);
}
நிலையான உள்வரி தருக்க ligate_உள்ளீடு (hb_ot_apply_context_t *c,
				 unsigned முழு மொத்தம், /* Including the first glyph */
				 unsigned முழு match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned முழு total_component_மொத்தம்)
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

  தருக்க is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  தருக்க is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
    எனில் (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = பொய்;
      is_mark_ligature = பொய்;
      நிறுத்து;
    }
  தருக்க is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned முழு klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned முழு lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned முழு last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned முழு last_இலக்கம்_components = hb_glyph_info_get_lig_இலக்கம்_comps (&buffer->cur());
  unsigned முழு components_so_far = last_இலக்கம்_components;

  எனில் (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_மொத்தம்);
    எனில் (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
  {
    போது (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      எனில் (is_ligature)
      {
	unsigned முழு this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	எனில் (this_comp == 0)
	  this_comp = last_இலக்கம்_components;
	ப்ரதிஜ்ஞா (components_so_far >= last_இலக்கம்_components);
	unsigned முழு new_lig_comp = components_so_far - last_இலக்கம்_components +
				    hb_min (this_comp, last_இலக்கம்_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (வெறுமை) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_இலக்கம்_components = hb_glyph_info_get_lig_இலக்கம்_comps_in_ligation (&buffer->cur());
    components_so_far += last_இலக்கம்_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  எனில் (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    க்கு (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      எனில் (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) நிறுத்து;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      எனில் (!this_comp) நிறுத்து;

      ப்ரதிஜ்ஞா (components_so_far >= last_இலக்கம்_components);
      unsigned new_lig_comp = components_so_far - last_இலக்கம்_components +
			      hb_min (this_comp, last_இலக்கம்_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (உண்மை);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#இல்லையெனில்
உள்வரி
#endif
நிலையான தருக்க match_backtrack (hb_ot_apply_context_t *c,
			     unsigned முழு மொத்தம்,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     மாறிலி வெறுமை *match_data,
			     unsigned முழு *match_start)
{
  TRACE_APPLY (nullptr);

  எனில் (!மொத்தம்)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (உண்மை);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
  {
    unsigned unsafe_from;
    எனில் (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (பொய்);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (உண்மை);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#இல்லையெனில்
உள்வரி
#endif
நிலையான தருக்க match_lookahead (hb_ot_apply_context_t *c,
			     unsigned முழு மொத்தம்,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     மாறிலி வெறுமை *match_data,
			     unsigned முழு start_index,
			     unsigned முழு *end_index)
{
  TRACE_APPLY (nullptr);

  எனில் (!மொத்தம்)
  {
    *end_index = start_index;
    return_trace (உண்மை);
  }

  auto &skippy_iter = c->iter_context;
  ப்ரதிஜ்ஞா (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
  {
    unsigned unsafe_to;
    எனில் (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (பொய்);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (உண்மை);
}



கட்டமைப்பு LookupRecord
{
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி hb_map_t         *lookup_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  பொது:
  DEFINE_SIZE_STATIC (4);
};

நிலையான உள்வரி unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     மாறிலி hb_array_t<மாறிலி LookupRecord> lookupRecords,
						     மாறிலி hb_map_t *lookup_map)
{
  unsigned மொத்தம் = 0;
  க்கு (மாறிலி LookupRecord& r : lookupRecords)
  {
    எனில் (!lookup_map->has (r.lookupListIndex))
      தொடர்;

    எனில் (!r.serialize (c, lookup_map))
      திரும்பு 0;

    மொத்தம்++;
  }
  திரும்பு மொத்தம்;
}

எண்ணிக்கை ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

நிலையான உள்வரி மாறிலி hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			மாறிலி வெறுமை *data,
			unsigned value,
			தருக்க filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  தேர்வு (context_format)
  {
    நிலை ContextFormat::SimpleContext:
      திரும்பு c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    நிலை ContextFormat::ClassBasedContext:
    {
      மாறிலி hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      எனில் (unlikely (!class_set))
	திரும்பு nullptr;
      எனில் (!filter_class_by_parent)
	திரும்பு class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      நிறுத்து;
    }
    நிலை ContextFormat::CoverageBasedContext:
      திரும்பு c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  எனில் (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    திரும்பு nullptr;
  }
  திரும்பு scratch;
}

நிலையான உள்வரி வெறுமை context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned உள்ளீடு_மொத்தம், glyph_data_t உள்ளீடு,
					     unsigned lookup_மொத்தம்_2,
					     மாறிலி LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     மாறிலி வெறுமை *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     வெறுமை *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  க்கு (unsigned முழு i = 0; i < lookup_மொத்தம்_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    எனில் (seqIndex >= உள்ளீடு_மொத்தம்) தொடர்;

    தருக்க has_pos_glyphs = பொய்;

    எனில் (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = உண்மை;
      pos_glyphs.clear ();
      எனில் (seqIndex == 0)
      {
        தேர்வு (context_format) {
        நிலை ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          நிறுத்து;
        நிலை ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          நிறுத்து;
        நிலை ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          நிறுத்து;
        }
      }
      இல்லையெனில்
      {
        மாறிலி வெறுமை *உள்ளீடு_data = உள்ளீடு.data;
        unsigned உள்ளீடு_மதிப்பு = seqIndex - 1;
        எனில் (context_format != ContextFormat::SimpleContext)
        {
          உள்ளீடு_data = data;
          உள்ளீடு_மதிப்பு = உள்ளீடு[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, உள்ளீடு_data, உள்ளீடு_மதிப்பு, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs))
      திரும்பு;
    எனில் (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } இல்லையெனில் {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = உள்ளீடு_மொத்தம்;
    எனில் (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->எடு_cur_done_glyphs ();
  }
}

நிலையான தருக்க
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned உள்ளீடு_மொத்தம்_2,
				unsigned lookup_மொத்தம்,
				மாறிலி LookupRecord lookup_records[],
				மாறிலி வெறுமை *rule,
				unsigned value,
				ContextFormat context_format,
				மாறிலி வெறுமை *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  தருக்க have_active_idx = பொய்;
  க்கு (unsigned i = 0; i < lookup_மொத்தம்; i++)
  {
    எனில் (lookup_records[i].sequenceIndex >= உள்ளீடு_மொத்தம்_2)
      தொடர்;

    எனில் (!have_active_idx)
    {
      எனில் (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	திரும்பு பொய்;
      have_active_idx = உண்மை;
    }

    hb_depend_context_t::context_set_cache_சாவி_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    எனில் (!c->context_set_cache.has (key, &cached_context_set))
      திரும்பு பொய்;
    எனில் (unlikely (!cached_context_sets->தள்ளு_or_fail (*cached_context_set)))
      திரும்பு c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  திரும்பு have_active_idx;
}

நிலையான உள்வரி வெறுமை context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned உள்ளீடு_மொத்தம், glyph_data_t உள்ளீடு,
					     unsigned lookup_மொத்தம்_2,
					     மாறிலி LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     மாறிலி வெறுமை *rule,
					     unsigned value,
					     ContextFormat context_format,
					     மாறிலி வெறுமை *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     வெறுமை *cache,
					     மாறிலி hb_set_t &preliminary_context,
					     மாறிலி hb_vector_t<மாறிலி hb_set_t *> *உள்ளீடு_position_glyphs,
					     மாறிலி hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  எனில் (unlikely (உள்ளீடு_மொத்தம் > HB_MAX_CONTEXT_LENGTH))
    திரும்பு;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  க்கு (unsigned முழு i = 0; i < lookup_மொத்தம்_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    எனில் (seqIndex >= உள்ளீடு_மொத்தம்)
      தொடர்;

    hb_depend_context_t::context_set_cache_சாவி_t context_cache_சாவி;
    எனில் (unlikely (!c->get_context_set_cache_சாவி (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_சாவி)))
      திரும்பு;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    எனில் (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    இல்லையெனில் எனில் (c->context_set_cache.has (context_cache_சாவி, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    இல்லையெனில்
    {
      எனில் (unlikely (!உள்ளீடு_position_glyphs))
      {
	c->depend_data->fail ();
	திரும்பு;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_மதிப்பு_INVALID;
      போது (preliminary_context.next (&elem)) {
	எனில் (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} இல்லையெனில் {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      க்கு (unsigned j = 0; j < உள்ளீடு_position_glyphs->length; j++) {
	எனில் (j != seqIndex && (*உள்ளீடு_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*உள்ளீடு_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_மதிப்பு_INVALID;
      போது (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	மாறிலி hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	எனில் (unlikely (!original_set)) தொடர்;

	/* Dependency context sets are never inverted. */
	எனில் (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      க்கு (unsigned j = 0; j < உள்ளீடு_position_glyphs->length; j++) {
	எனில் (j != seqIndex && (*உள்ளீடு_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  எனில் (!(*உள்ளீடு_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    தருக்க cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*உள்ளீடு_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*உள்ளீடு_position_glyphs)[j]);
	    எனில் (unlikely (idx == HB_CODEPOINT_INVALID))
	      திரும்பு;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      எனில் (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	திரும்பு;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      எனில் (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	திரும்பு;

      எனில் (unlikely (!c->context_set_cache.set (context_cache_சாவி, context_set_idx)))
      {
	c->depend_data->fail ();
	திரும்பு;
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
    தருக்க nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    எனில் (உள்ளீடு_மொத்தம் > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    எனில் (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    தருக்க has_pos_glyphs = பொய்;

    எனில் (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = உண்மை;
      pos_glyphs.clear ();
      எனில் (seqIndex == 0)
      {
        தேர்வு (context_format) {
        நிலை ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          நிறுத்து;
        நிலை ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          நிறுத்து;
        நிலை ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          நிறுத்து;
        }
      }
      இல்லையெனில்
      {
        மாறிலி வெறுமை *உள்ளீடு_data = உள்ளீடு.data;
        unsigned உள்ளீடு_மதிப்பு = seqIndex - 1;
        எனில் (context_format != ContextFormat::SimpleContext)
        {
          உள்ளீடு_data = data;
          உள்ளீடு_மதிப்பு = உள்ளீடு[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, உள்ளீடு_data, உள்ளீடு_மதிப்பு, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    எனில் (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      எனில் (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	எனில் (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->தள்ளு_cur_active_glyphs (active_idx);
      எனில் (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	திரும்பு;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    இல்லையெனில் எனில் (unlikely (!c->தள்ளு_வேர்_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      திரும்பு;
    }

    unsigned endIndex = உள்ளீடு_மொத்தம்;
    எனில் (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->எடு_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
நிலையான உள்வரி வெறுமை recurse_lookups (context_t *c,
                                    unsigned முழு lookup_மொத்தம்_2,
                                    மாறிலி LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  க்கு (unsigned முழு i = 0; i < lookup_மொத்தம்_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

நிலையான உள்வரி வெறுமை apply_lookup (hb_ot_apply_context_t *c,
				 unsigned முழு மொத்தம், /* Including the first glyph */
				 unsigned முழு lookup_மொத்தம்_2,
				 மாறிலி LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned முழு match_end)
{
  hb_buffer_t *buffer = c->buffer;
  முழு end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned முழு bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    முழு delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    க்கு (unsigned முழு j = 0; j < மொத்தம்; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  க்கு (unsigned முழு i = 0; i < lookup_மொத்தம்_2 && buffer->successful; i++)
  {
    unsigned முழு idx = lookupRecord[i].sequenceIndex;
    எனில் (idx >= மொத்தம்)
      தொடர்;

    unsigned முழு orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    எனில் (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      தொடர்;

    எனில் (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      நிறுத்து;

    எனில் (unlikely (buffer->max_ops <= 0))
      நிறுத்து;

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      எனில் (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    தருக்க ret = c->recurse (lookupRecord[i].lookupListIndex);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      எனில் (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    எனில் (!ret)
      தொடர்;

    unsigned முழு new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    முழு delta = new_len - orig_len;

    எனில் (!delta)
      தொடர்;

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
    எனில் (end < முழு (c->match_positions.arrayZ[idx]))
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

    unsigned முழு next = idx + 1; /* next now is the position after the recursed lookup. */

    எனில் (delta > 0)
    {
      எனில் (unlikely (delta + மொத்தம் > HB_MAX_CONTEXT_LENGTH))
	நிறுத்து;
      எனில் (unlikely (மொத்தம் + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (மொத்தம் + delta)))
        திரும்பு;
    }
    இல்லையெனில்
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (முழு) next - (முழு) மொத்தம்);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (மொத்தம் - next) * அளவு (c->match_positions.arrayZ[0]));
    next += delta;
    மொத்தம் += delta;

    /* Fill in new entries. */
    க்கு (unsigned முழு j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    க்கு (; next < மொத்தம்; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  ப்ரதிஜ்ஞா (end >= 0);
  (வெறுமை) buffer->move_to (end);
}



/* Contextual lookups */

கட்டமைப்பு ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  மாறிலி வெறுமை *intersects_data;
  வெறுமை *intersects_cache;
  வெறுமை *intersected_glyphs_cache;
};

வகைவரை ContextClosureLookupContext ContextDependLookupContext;

கட்டமைப்பு ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  மாறிலி வெறுமை *collect_data;
};

கட்டமைப்பு ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  மாறிலி வெறுமை *match_data;
  get_மதிப்பு_func_t get_மதிப்பு;
};

நிலையான உள்வரி தருக்க context_intersects (மாறிலி hb_set_t *glyphs,
				       unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
				       glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  திரும்பு array_is_subset_of (glyphs,
			     உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0, உள்ளீடு,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

நிலையான உள்வரி வெறுமை context_closure_lookup (hb_closure_context_t *c,
					   unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
					   glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
					   unsigned முழு lookup_மொத்தம்_2,
					   மாறிலி LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  எனில் (context_intersects (c->glyphs,
			  உள்ளீடு_மொத்தம், உள்ளீடு,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     உள்ளீடு_மொத்தம், உள்ளீடு,
				     lookup_மொத்தம்_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

நிலையான உள்வரி வெறுமை context_depend_lookup (hb_depend_context_t *c,
					  unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
					  glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
					  unsigned முழு lookup_மொத்தம்_2,
					  மாறிலி LookupRecord lookupRecord[],
					  மாறிலி வெறுமை *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  எனில் (!context_intersects (c->glyphs,
			   உள்ளீடு_மொத்தம், உள்ளீடு,
			   lookup_context))
    திரும்பு;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  எனில் (unlikely (!scratch))
    திரும்பு;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  தருக்க context_sets_cached = context_depend_sets_are_cached (
    c, உள்ளீடு_மொத்தம், lookup_மொத்தம்_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  எனில் (unlikely (!c->depend_data->successful))
    திரும்பு;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<மாறிலி hb_set_t *> &உள்ளீடு_position_glyphs = scratch->உள்ளீடு_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  எனில் (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    மாறிலி hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      உண்மை, &position_scratch, &scratch->compact_workspace);
    எனில் (unlikely (!pos0_glyphs ||
		  !உள்ளீடு_position_glyphs.தள்ளு_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      திரும்பு;
    }

    /* Positions 1+ (Input array) */
    க்கு (unsigned i = 0; i + 1 < உள்ளீடு_மொத்தம்; i++)
    {
      மாறிலி hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	உள்ளீடு[i], பொய், &position_scratch);
      எனில் (unlikely (!pos_glyphs ||
		    !உள்ளீடு_position_glyphs.தள்ளு_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	திரும்பு;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  உள்ளீடு_மொத்தம், உள்ளீடு,
				  lookup_மொத்தம்_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &உள்ளீடு_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

நிலையான உள்வரி வெறுமை context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
						  glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
						  unsigned முழு lookup_மொத்தம்_2,
						  மாறிலி LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->உள்ளீடு,
		 உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0, உள்ளீடு,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_மொத்தம்_2, lookupRecord);
}

நிலையான உள்வரி தருக்க context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
					       glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
					       unsigned முழு lookup_மொத்தம்_2 HB_UNUSED,
					       மாறிலி LookupRecord lookupRecord[] HB_UNUSED,
					       மாறிலி ContextApplyLookupContext &lookup_context)
{
  திரும்பு would_match_உள்ளீடு (c,
			    உள்ளீடு_மொத்தம், உள்ளீடு,
			    lookup_context.funcs.match, lookup_context.match_data);
}

நிலையான உள்வரி தருக்க context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
					 glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
					 unsigned முழு lookup_மொத்தம்_2,
					 மாறிலி LookupRecord lookupRecord[],
					 மாறிலி ContextApplyLookupContext &lookup_context)
{
  எனில் (unlikely (உள்ளீடு_மொத்தம் > HB_MAX_CONTEXT_LENGTH)) திரும்பு பொய்;

  unsigned match_end = 0;
  தருக்க ret = பொய்;
  எனில் (match_உள்ளீடு (c,
		   உள்ளீடு_மொத்தம், உள்ளீடு,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  உள்ளீடு_மொத்தம்,
		  lookup_மொத்தம்_2, lookupRecord,
		  match_end);
    ret = உண்மை;
  }
  இல்லையெனில்
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = பொய்;
  }

  திரும்பு ret;
}

நிலையான உள்வரி தருக்க context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  தேர்வு (op)
  {
    நிலை hb_ot_subtable_cache_op_t::ENTER:
    {
      எனில் (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	திரும்பு பொய்;
      auto &info = c->buffer->info;
      unsigned மொத்தம் = c->buffer->len;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      திரும்பு உண்மை;
    }
    நிலை hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      நிறுத்து;
    }
  }
  திரும்பு பொய்;
}

template <typename Types>
கட்டமைப்பு Rule
{
  template <typename SetTypes, typename RuleType>
  friend கட்டமைப்பு RuleSetOf;

  தருக்க intersects (மாறிலி hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) மாறிலி
  {
    திரும்பு context_intersects (glyphs,
			       உள்ளீடு_மொத்தம், உள்ளீடு_Z.arrayZ,
			       lookup_context);
  }

  வெறுமை depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array ((உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0)));
    context_depend_lookup (c,
			   உள்ளீடு_மொத்தம், உள்ளீடு_Z.arrayZ,
			   lookup_மொத்தம்_2, lookupRecord.arrayZ,
			   இது,
			   value, lookup_context);
  }

  வெறுமை closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;

    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array ((உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0)));
    context_closure_lookup (c,
			    உள்ளீடு_மொத்தம், உள்ளீடு_Z.arrayZ,
			    lookup_மொத்தம்_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;
    எனில் (!intersects (c->glyphs, lookup_context)) திரும்பு;

    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array (உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0));
    recurse_lookups (c, lookup_மொத்தம்_2, lookupRecord.arrayZ);
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array (உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0));
    context_collect_glyphs_lookup (c,
				   உள்ளீடு_மொத்தம், உள்ளீடு_Z.arrayZ,
				   lookup_மொத்தம்_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest,
			      மாறிலி ClassDef *class_def = nullptr) மாறிலி
  {
    எனில் (உள்ளீடு_மொத்தம் <= 1)
    {
      *digest = hb_set_digest_t::full ();
      திரும்பு;
    }

    unsigned second = உள்ளீடு_Z.arrayZ[0];
    எனில் (!class_def)
      digest->add (second);
    இல்லையெனில் எனில் (!second)
      *digest = hb_set_digest_t::full ();
    இல்லையெனில்
      class_def->collect_class (digest, second);
  }

  தருக்க would_apply (hb_would_apply_context_t *c,
		    மாறிலி ContextApplyLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array (உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0));
    திரும்பு context_would_apply_lookup (c,
				       உள்ளீடு_மொத்தம், உள்ளீடு_Z.arrayZ,
				       lookup_மொத்தம்_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  தருக்க apply (hb_ot_apply_context_t *c,
	      மாறிலி ContextApplyLookupContext &lookup_context) மாறிலி
  {
    TRACE_APPLY (இது);
    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array (உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0));
    return_trace (context_apply_lookup (c, உள்ளீடு_மொத்தம், உள்ளீடு_Z.arrayZ, lookup_மொத்தம்_2, lookupRecord.arrayZ, lookup_context));
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி hb_map_t *உள்ளீடு_mapping, /* old->new glyphid or class mapping */
		  மாறிலி hb_map_t *lookup_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->start_embed (இது);
    எனில் (unlikely (!c->extend_min (out))) return_trace (பொய்);

    out->உள்ளீடு_மொத்தம் = உள்ளீடு_மொத்தம்;
    மாறிலி auto உள்ளீடு = உள்ளீடு_Z.as_array (உள்ளீடு_மொத்தம் - 1);
    க்கு (மாறிலி auto org : உள்ளீடு)
    {
      typename Types::HBUINT d;
      d = உள்ளீடு_mapping->get (org);
      c->copy (d);
    }

    மாறிலி auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (உள்ளீடு_Z.as_array ((உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0)));

    unsigned மொத்தம் = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_மொத்தம்_2), lookup_map);
    return_trace (c->check_assign (out->lookup_மொத்தம்_2, மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க subset (hb_subset_context_t *c,
	       மாறிலி hb_map_t *lookup_map,
	       மாறிலி hb_map_t *klass_map = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (unlikely (!உள்ளீடு_மொத்தம்)) return_trace (பொய்);
    மாறிலி auto உள்ளீடு = உள்ளீடு_Z.as_array (உள்ளீடு_மொத்தம் - 1);

    மாறிலி hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    எனில் (!hb_all (உள்ளீடு, mapping)) return_trace (பொய்);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  பொது:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  c->check_range (உள்ளீடு_Z.arrayZ,
				  உள்ளீடு_Z.item_size * (உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0) +
				  LookupRecord::static_size * lookup_மொத்தம்_2));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	உள்ளீடு_மொத்தம்;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_மொத்தம்_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		உள்ளீடு_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  பொது:
  DEFINE_SIZE_ARRAY (4, உள்ளீடு_Z);
};

template <typename Types, typename RuleType>
கட்டமைப்பு RuleSetOf
{
  தருக்க intersects (மாறிலி hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) மாறிலி
  {
    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_map ([&] (மாறிலி RuleType &_) { திரும்பு _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  வெறுமை collect_first_உள்ளீடு_classes (hb_ot_layout_ruleset_digest_t *digest) மாறிலி
  {
    digest->init ();
    க்கு (unsigned i = 0; i < rule.len; i++)
    {
      மாறிலி RuleType &r = இது+rule.arrayZ[i];
      எனில் (r.உள்ளீடு_மொத்தம் <= 1)
      {
	digest->set_full ();
	திரும்பு;
      }
      digest->add (r.உள்ளீடு_Z.arrayZ[0]);
    }
  }

  வெறுமை depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) மாறிலி
  {
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  வெறுமை closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;

    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) மாறிலி
  {
    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest,
			      மாறிலி ClassDef *class_def = nullptr) மாறிலி
  {
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([=] (மாறிலி RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c,
		    மாறிலி ContextApplyLookupContext &lookup_context) மாறிலி
  {
    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_map ([&] (மாறிலி RuleType &_) { திரும்பு _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  தருக்க apply (hb_ot_apply_context_t *c,
	      மாறிலி ContextApplyLookupContext &lookup_context,
	      மாறிலி hb_ot_layout_ruleset_digest_t *digest = nullptr) மாறிலி
  {
    TRACE_APPLY (இது);

    unsigned இலக்கம்_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    எனில் (HB_OPTIMIZE_SIZE_VAL || இலக்கம்_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (இது))
      | hb_map ([&] (மாறிலி RuleType &_) { திரும்பு _.apply (c, lookup_context); })
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
    தருக்க matched = skippy_iter.next (&unsafe_to1);
    எனில் (likely (matched))
    {
      எனில் (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        செல் slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      எனில் (digest && lookup_context.get_மதிப்பு &&
	  !digest->may_have (lookup_context.get_மதிப்பு (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (பொய்);
	}
    }
    இல்லையெனில்
    {
      தருக்க unsafe_to_concat = பொய்;
      தருக்க ret =
      + hb_iter (rule)
      | hb_map (hb_add (இது))
      | hb_filter ([&] (மாறிலி RuleType &_)
		   {
		     எனில் (_.உள்ளீடு_மொத்தம் <= 1) திரும்பு உண்மை;
		     unsafe_to_concat = உண்மை;
		     திரும்பு பொய்;
		   })
      | hb_map ([&] (மாறிலி RuleType &_) { திரும்பு _.apply (c, lookup_context); })
      | hb_any
      ;
      எனில் (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    எனில் (likely (matched))
    {
      எனில் (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        செல் slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_உள்ளீடு = lookup_context.funcs.match;
    auto *உள்ளீடு_data = lookup_context.match_data;
    க்கு (unsigned முழு i = 0; i < இலக்கம்_rules; i++)
    {
      மாறிலி auto &r = இது+rule.arrayZ[i];

      மாறிலி auto &உள்ளீடு = r.உள்ளீடு_Z;

      எனில் (r.உள்ளீடு_மொத்தம் <= 1 ||
	  (!match_உள்ளீடு ||
	   match_உள்ளீடு (*first, உள்ளீடு.arrayZ[0], உள்ளீடு_data)))
      {
        எனில் (!second ||
	    (r.உள்ளீடு_மொத்தம் <= 2 ||
	     (!match_உள்ளீடு ||
	      match_உள்ளீடு (*second, உள்ளீடு.arrayZ[1], உள்ளீடு_data)))
	   )
	{
	  எனில் (r.apply (c, lookup_context))
	  {
	    எனில் (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (உண்மை);
	  }
	}
	இல்லையெனில்
	  unsafe_to = unsafe_to2;
      }
      இல்லையெனில்
      {
	எனில் (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	க்கு (; i + 1 < இலக்கம்_rules; i++)
	{
	  மாறிலி auto &r2 = இது+rule.arrayZ[i + 1];
	  மாறிலி auto &input2 = r2.உள்ளீடு_Z;
	  எனில் (r2.உள்ளீடு_மொத்தம் <= 1 || input2.arrayZ[0] != உள்ளீடு.arrayZ[0])
	    நிறுத்து;
	}
      }
    }
    எனில் (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (பொய்);
  }

  தருக்க subset (hb_subset_context_t *c,
	       மாறிலி hb_map_t *lookup_map,
	       மாறிலி hb_map_t *klass_map = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    க்கு (மாறிலி auto& _ : rule)
    {
      எனில் (!_) தொடர்;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      எனில் (unlikely (!o)) தொடர்;

      எனில் (!o->serialize_subset (c, _, இது, lookup_map, klass_map))
      {
	out->rule.எடு ();
	c->serializer->revert (o_snap);
      }
    }

    தருக்க ret = தருக்க (out->rule);
    எனில் (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (rule.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  பொது:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
கட்டமைப்பு RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
கட்டமைப்பு ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
கட்டமைப்பு ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    திரும்பு
    + hb_zip (இது+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_map ([&] (மாறிலி RuleSet &_) { திரும்பு _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (இது+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      திரும்பு c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](மாறிலி hb_pair_t<hb_codepoint_t, unsigned> _) { திரும்பு hb_pair_t<unsigned, மாறிலி RuleSet&> (_.first, இது+ruleSet[_.second]); })
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (இது+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      திரும்பு c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](மாறிலி hb_pair_t<hb_codepoint_t, unsigned> _) { திரும்பு hb_pair_t<unsigned, மாறிலி RuleSet&> (_.first, இது+ruleSet[_.second]); })
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி
  {
    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (இது+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    (இது+coverage).collect_coverage (c->உள்ளீடு);

    கட்டமைப்பு ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([digest] (மாறிலி RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    மாறிலி RuleSet &rule_set = இது+ruleSet[(இது+coverage).get_coverage (c->glyphs[0])];
    கட்டமைப்பு ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    திரும்பு rule_set.would_apply (c, lookup_context);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur().codepoint);
    எனில் (likely (index == NOT_COVERED))
      return_trace (பொய்);

    மாறிலி RuleSet &rule_set = இது+ruleSet[index];
    கட்டமைப்பு ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    மாறிலி hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, இது, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (தருக்க (new_coverage));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && ruleSet.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
கட்டமைப்பு ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (glyphs))
      திரும்பு பொய்;

    மாறிலி ClassDef &class_def = இது+classDef;

    hb_map_t cache;
    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (இது+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    திரும்பு
    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_enumerate
    | hb_map ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி RuleSet &> p)
	      { திரும்பு class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    மாறிலி ClassDef &class_def = இது+classDef;

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
    { திரும்பு class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி typename Types::template OffsetTo<RuleSet>&> _)
                {
                  மாறிலி RuleSet& rule_set = இது+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    மாறிலி ClassDef &class_def = இது+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { திரும்பு class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி typename Types::template OffsetTo<RuleSet>&> _)
                {
                  மாறிலி RuleSet& rule_set = இது+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (c->glyphs))
      திரும்பு;

    மாறிலி ClassDef &class_def = இது+classDef;

    hb_map_t cache;
    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_enumerate
    | hb_filter ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி RuleSet &> p)
    { திரும்பு class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (மாறிலி RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    (இது+coverage).collect_coverage (c->உள்ளீடு);

    மாறிலி ClassDef &class_def = இது+classDef;
    கட்டமைப்பு ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    மாறிலி ClassDef &class_def = இது+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([=, &class_def] (மாறிலி RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    மாறிலி ClassDef &class_def = இது+classDef;
    unsigned முழு index = class_def.get_class (c->glyphs[0]);
    மாறிலி RuleSet &rule_set = இது+ruleSet[index];
    கட்டமைப்பு ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    திரும்பு rule_set.would_apply (c, lookup_context);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  unsigned cache_cost () மாறிலி
  {
    திரும்பு (இது+classDef).cost ();
  }
  நிலையான தருக்க cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    திரும்பு context_cache_func (c, op);
  }

  கட்டமைப்பு external_cache_t
  {
    hb_ot_layout_இருமம்_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  வெறுமை *external_cache_create () மாறிலி
  {
    unsigned மொத்தம் = ruleSet.len;
    unsigned size = அளவு (external_cache_t) -
		    HB_VAR_ARRAY * அளவு (hb_ot_layout_ruleset_digest_t) +
		    மொத்தம் * அளவு (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    எனில் (likely (cache))
    {
      cache->coverage.clear ();
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
	(இது+ruleSet[i]).collect_first_உள்ளீடு_classes (&cache->rule_sets[i]);
    }
    திரும்பு cache;
  }
  தருக்க apply_cached (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி { திரும்பு _apply (c, உண்மை, external_cache); }
  தருக்க apply (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி { திரும்பு _apply (c, பொய், external_cache); }
  தருக்க _apply (hb_ot_apply_context_t *c, தருக்க cached, வெறுமை *external_cache) மாறிலி
  {
    TRACE_APPLY (இது);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned முழு index = (இது+coverage).get_coverage_இருமம் (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#இல்லையெனில்
    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    மாறிலி ClassDef &class_def = இது+classDef;

    கட்டமைப்பு ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_மதிப்பு : get_class_மதிப்பு
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    மாறிலி RuleSet &rule_set = இது+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    மாறிலி hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#இல்லையெனில்
    மாறிலி hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;
    எனில் (unlikely (!out->coverage.serialize_subset (c, coverage, இது)))
      return_trace (பொய்);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, இது, &klass_map);

    மாறிலி hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (இது+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (இது+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    மாறிலி hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    தருக்க ret = உண்மை;
    முழு non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    க்கு (மாறிலி auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      எனில் (unlikely (!o))
      {
	ret = பொய்;
	நிறுத்து;
      }

      எனில் (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, இது, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    எனில் (!ret || non_zero_index == -1) return_trace (பொய்);

    //prune empty trailing ruleSets
    --index;
    போது (index > non_zero_index)
    {
      out->ruleSet.எடு ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (தருக்க (out->ruleSet));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && classDef.sanitize (c, இது) && ruleSet.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
கட்டமைப்பு ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (!(இது+coverageZ[0]).intersects (glyphs))
      திரும்பு பொய்;

    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      இது
    };
    திரும்பு context_intersects (glyphs,
			       glyph_மொத்தம்_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverageZ[0]).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      இது,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_மொத்தம்_2, (மாறிலி HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_மொத்தம்_2, lookupRecord,
			   இது,
			   coverageZ[0], lookup_context);

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverageZ[0]).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    கட்டமைப்பு ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      இது
    };
    context_closure_lookup (c,
			    glyph_மொத்தம்_2, coverageZ.arrayZ + 1,
			    lookup_மொத்தம்_2, lookupRecord,
			    0, lookup_context);

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி
  {
    எனில் (!intersects (c->glyphs))
      திரும்பு;
    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    recurse_lookups (c, lookup_மொத்தம்_2, lookupRecord);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    (இது+coverageZ[0]).collect_coverage (c->உள்ளீடு);

    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    கட்டமைப்பு ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      இது
    };

    context_collect_glyphs_lookup (c,
				   glyph_மொத்தம்_2, coverageZ.arrayZ + 1,
				   lookup_மொத்தம்_2, lookupRecord,
				   lookup_context);
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    எனில் (glyph_மொத்தம்_2 <= 1)
      *digest = hb_set_digest_t::full ();
    இல்லையெனில்
      (இது+coverageZ[1]).collect_coverage (digest);
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    கட்டமைப்பு ContextApplyLookupContext lookup_context = {
      {match_coverage},
      இது
    };
    திரும்பு context_would_apply_lookup (c,
				       glyph_மொத்தம்_2, coverageZ.arrayZ + 1,
				       lookup_மொத்தம்_2, lookupRecord,
				       lookup_context);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverageZ[0]; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு index = (இது+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    கட்டமைப்பு ContextApplyLookupContext lookup_context = {
      {match_coverage},
      இது
    };
    return_trace (context_apply_lookup (c,
					glyph_மொத்தம்_2,
					coverageZ.arrayZ + 1,
					lookup_மொத்தம்_2, lookupRecord,
					lookup_context));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    out->format = format;
    out->glyph_மொத்தம்_2 = glyph_மொத்தம்_2;

    auto coverages = coverageZ.as_array (glyph_மொத்தம்_2);

    க்கு (மாறிலி auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      எனில் (unlikely (!o)) return_trace (பொய்);
      எனில் (!o->serialize_subset (c, offset, இது)) return_trace (பொய்);
    }

    மாறிலி auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_மொத்தம்_2));
    மாறிலி hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned மொத்தம் = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_மொத்தம்_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_மொத்தம்_2, மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது))) return_trace (பொய்);
    hb_barrier ();
    unsigned முழு மொத்தம் = glyph_மொத்தம்_2;
    எனில் (unlikely (!மொத்தம்)) return_trace (பொய்); /* We want to access coverageZ[0] freely. */
    எனில் (unlikely (!c->check_array (coverageZ.arrayZ, மொத்தம்))) return_trace (பொய்);
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!coverageZ[i].sanitize (c, இது))) return_trace (பொய்);
    மாறிலி LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_மொத்தம்_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_மொத்தம்_2)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_மொத்தம்_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_மொத்தம்_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  பொது:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

கட்டமைப்பு Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    நிலை 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    நிலை 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
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

கட்டமைப்பு ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  மாறிலி வெறுமை *intersects_data[3];
  வெறுமை *intersects_cache[3];
  வெறுமை *intersected_glyphs_cache;
};

வகைவரை ChainContextClosureLookupContext ChainContextDependLookupContext;

கட்டமைப்பு ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  மாறிலி வெறுமை *collect_data[3];
};

கட்டமைப்பு ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  மாறிலி வெறுமை *match_data[3];
  get_மதிப்பு_func_t get_உள்ளீடு_மதிப்பு;
};

நிலையான உள்வரி தருக்க chain_context_intersects (மாறிலி hb_set_t *glyphs,
					     unsigned முழு backtrack_மொத்தம்,
					     glyph_data_t backtrack,
					     unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
					     glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
					     unsigned முழு lookahead_மொத்தம்,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  தருக்க bt_ok = array_is_subset_of (glyphs,
			     backtrack_மொத்தம், backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  தருக்க in_ok = array_is_subset_of (glyphs,
			     உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0, உள்ளீடு,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  தருக்க la_ok = array_is_subset_of (glyphs,
			     lookahead_மொத்தம், lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  திரும்பு bt_ok && in_ok && la_ok;
}

நிலையான உள்வரி வெறுமை chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned முழு backtrack_மொத்தம்,
						 glyph_data_t backtrack,
						 unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
						 glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
						 unsigned முழு lookahead_மொத்தம்,
						 glyph_data_t lookahead,
						 unsigned முழு lookup_மொத்தம்_2,
						 மாறிலி LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  எனில் (chain_context_intersects (c->glyphs,
				backtrack_மொத்தம், backtrack,
				உள்ளீடு_மொத்தம், உள்ளீடு,
				lookahead_மொத்தம், lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     உள்ளீடு_மொத்தம், உள்ளீடு,
		     lookup_மொத்தம்_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

நிலையான உள்வரி வெறுமை chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned முழு backtrack_மொத்தம்,
						glyph_data_t backtrack,
						unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
						glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
						unsigned முழு lookahead_மொத்தம்,
						glyph_data_t lookahead,
						unsigned முழு lookup_மொத்தம்_2,
						மாறிலி LookupRecord lookupRecord[],
						மாறிலி வெறுமை *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  எனில் (!chain_context_intersects (c->glyphs,
				 backtrack_மொத்தம், backtrack,
				 உள்ளீடு_மொத்தம், உள்ளீடு,
				 lookahead_மொத்தம், lookahead,
				 lookup_context))
    திரும்பு;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  எனில் (unlikely (!scratch))
    திரும்பு;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  தருக்க context_sets_cached = context_depend_sets_are_cached (
    c, உள்ளீடு_மொத்தம், lookup_மொத்தம்_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  எனில் (unlikely (!c->depend_data->successful))
    திரும்பு;
  எனில் (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    உள்ளீடு_மொத்தம், உள்ளீடு,
				    lookup_மொத்தம்_2, lookupRecord,
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
    திரும்பு;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<மாறிலி hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<மாறிலி hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  க்கு (unsigned i = 0; i < backtrack_மொத்தம்; i++)
  {
    மாறிலி hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], பொய், &position_scratch);
    எனில் (unlikely (!pos_glyphs))
      திரும்பு;
    எனில் (!pos_glyphs->is_empty ())
    {
      எனில் (unlikely (!backtrack_sets.தள்ளு_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        திரும்பு;
      }
    }
  }

  /* Extract lookahead glyphs */
  க்கு (unsigned i = 0; i < lookahead_மொத்தம்; i++)
  {
    மாறிலி hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], பொய், &position_scratch);
    எனில் (unlikely (!pos_glyphs))
      திரும்பு;
    எனில் (!pos_glyphs->is_empty ())
    {
      எனில் (unlikely (!lookahead_sets.தள்ளு_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        திரும்பு;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  க்கு (மாறிலி hb_set_t *back_set : backtrack_sets)
  {
    எனில் (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } இல்லையெனில் எனில் (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      எனில் (unlikely (set_idx == HB_CODEPOINT_INVALID))
        திரும்பு;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  க்கு (மாறிலி hb_set_t *look_set : lookahead_sets)
  {
    எனில் (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } இல்லையெனில் எனில் (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      எனில் (unlikely (set_idx == HB_CODEPOINT_INVALID))
        திரும்பு;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  எனில் (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    திரும்பு;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<மாறிலி hb_set_t *> &உள்ளீடு_position_glyphs = scratch->உள்ளீடு_position_glyphs;

  எனில் (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    மாறிலி hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      உண்மை, &position_scratch, &scratch->compact_workspace);
    எனில் (unlikely (!pos0_glyphs ||
		  !உள்ளீடு_position_glyphs.தள்ளு_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      திரும்பு;
    }

    /* Positions 1+ (Input array) */
    க்கு (unsigned i = 0; i + 1 < உள்ளீடு_மொத்தம்; i++)
    {
      மாறிலி hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	உள்ளீடு[i], பொய், &position_scratch);
      எனில் (unlikely (!pos_glyphs ||
		    !உள்ளீடு_position_glyphs.தள்ளு_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	திரும்பு;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  உள்ளீடு_மொத்தம், உள்ளீடு,
				  lookup_மொத்தம்_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &உள்ளீடு_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

நிலையான உள்வரி வெறுமை chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned முழு backtrack_மொத்தம்,
							glyph_data_t backtrack,
							unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
							glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
							unsigned முழு lookahead_மொத்தம்,
							glyph_data_t lookahead,
							unsigned முழு lookup_மொத்தம்_2,
							மாறிலி LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_மொத்தம், backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->உள்ளீடு,
		 உள்ளீடு_மொத்தம் ? உள்ளீடு_மொத்தம் - 1 : 0, உள்ளீடு,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_மொத்தம், lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_மொத்தம்_2, lookupRecord);
}

நிலையான உள்வரி தருக்க chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned முழு backtrack_மொத்தம்,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
						     glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
						     unsigned முழு lookahead_மொத்தம்,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned முழு lookup_மொத்தம்_2 HB_UNUSED,
						     மாறிலி LookupRecord lookupRecord[] HB_UNUSED,
						     மாறிலி ChainContextApplyLookupContext &lookup_context)
{
  திரும்பு (c->zero_context ? !backtrack_மொத்தம் && !lookahead_மொத்தம் : உண்மை)
      && would_match_உள்ளீடு (c,
			    உள்ளீடு_மொத்தம், உள்ளீடு,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

நிலையான உள்வரி தருக்க chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned முழு backtrack_மொத்தம்,
					       glyph_data_t backtrack,
					       unsigned முழு உள்ளீடு_மொத்தம், /* Including the first glyph (not matched) */
					       glyph_data_t உள்ளீடு, /* Array of input values--start with second glyph */
					       unsigned முழு lookahead_மொத்தம்,
					       glyph_data_t lookahead,
					       unsigned முழு lookup_மொத்தம்_2,
					       மாறிலி LookupRecord lookupRecord[],
					       மாறிலி ChainContextApplyLookupContext &lookup_context)
{
  எனில் (unlikely (உள்ளீடு_மொத்தம் > HB_MAX_CONTEXT_LENGTH)) திரும்பு பொய்;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  தருக்க ret = உண்மை;
  எனில் (!(match_உள்ளீடு (c,
		     உள்ளீடு_மொத்தம், உள்ளீடு,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_மொத்தம், lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    திரும்பு பொய்;
  }

  எனில் (!match_backtrack (c,
			backtrack_மொத்தம், backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    திரும்பு பொய்;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		உள்ளீடு_மொத்தம்,
		lookup_மொத்தம்_2, lookupRecord,
		match_end);

  திரும்பு ret;
}

template <typename Types>
கட்டமைப்பு ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend கட்டமைப்பு ChainRuleSetOf;

  தருக்க intersects (மாறிலி hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    திரும்பு chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     உள்ளீடு.lenP1, உள்ளீடு.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  வெறுமை depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 உள்ளீடு.lenP1, உள்ளீடு.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 இது,
				 value,
				 lookup_context);
  }

  வெறுமை closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;

    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  உள்ளீடு.lenP1, உள்ளீடு.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;
    எனில் (!intersects (c->glyphs, lookup_context)) திரும்பு;

    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 உள்ளீடு.lenP1, உள்ளீடு.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest,
			      மாறிலி ClassDef *உள்ளீடு_class_def = nullptr,
			      மாறிலி ClassDef *lookahead_class_def = nullptr) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);

    unsigned second;
    மாறிலி ClassDef *class_def;
    எனில் (உள்ளீடு.lenP1 > 1)
    {
      second = உள்ளீடு.arrayZ[0];
      class_def = உள்ளீடு_class_def;
    }
    இல்லையெனில் எனில் (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    இல்லையெனில்
    {
      *digest = hb_set_digest_t::full ();
      திரும்பு;
    }

    எனில் (!class_def)
      digest->add (second);
    இல்லையெனில் எனில் (!second)
      *digest = hb_set_digest_t::full ();
    இல்லையெனில்
      class_def->collect_class (digest, second);
  }

  தருக்க would_apply (hb_would_apply_context_t *c,
		    மாறிலி ChainContextApplyLookupContext &lookup_context) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    திரும்பு chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     உள்ளீடு.lenP1, உள்ளீடு.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  தருக்க apply (hb_ot_apply_context_t *c,
	      மாறிலி ChainContextApplyLookupContext &lookup_context) மாறிலி
  {
    TRACE_APPLY (இது);
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      உள்ளீடு.lenP1, உள்ளீடு.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) மாறிலி
  {
    c->copy (len);
    க்கு (மாறிலி auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி hb_map_t *lookup_map,
		  மாறிலி hb_map_t *backtrack_map,
		  மாறிலி hb_map_t *உள்ளீடு_map = nullptr,
		  மாறிலி hb_map_t *lookahead_map = nullptr) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    மாறிலி hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    எனில் (உள்ளீடு_map) mapping = உள்ளீடு_map;
    serialize_array (c, உள்ளீடு.lenP1, + உள்ளீடு.iter ()
				     | hb_map (mapping));

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    எனில் (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_மொத்தம்_2 = c->embed (&(lookup.len));
    எனில் (!lookup_மொத்தம்_2) return_trace (பொய்);

    unsigned மொத்தம் = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_மொத்தம்_2, மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க subset (hb_subset_context_t *c,
	       மாறிலி hb_map_t *lookup_map,
	       மாறிலி hb_map_t *backtrack_map = nullptr,
	       மாறிலி hb_map_t *உள்ளீடு_map = nullptr,
	       மாறிலி hb_map_t *lookahead_map = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);

    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);

    எனில் (!backtrack_map)
    {
      மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      எனில் (!hb_all (backtrack, glyphset) ||
	  !hb_all (உள்ளீடு, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (பொய்);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    இல்லையெனில்
    {
      எனில் (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (உள்ளீடு, உள்ளீடு_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (பொய்);

      serialize (c->serializer, lookup_map, backtrack_map, உள்ளீடு_map, lookahead_map);
    }

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* Hyper-optimized sanitized because this is really hot. */
    எனில் (unlikely (!backtrack.len.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    எனில் (unlikely (!உள்ளீடு.lenP1.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    எனில் (unlikely (!lookahead.len.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  பாதுகாக்கப்பட்ட:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		உள்ளீடு_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  பொது:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
கட்டமைப்பு ChainRuleSetOf
{
  தருக்க intersects (மாறிலி hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) மாறிலி
  {
    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_map ([&] (மாறிலி ChainRuleType &_) { திரும்பு _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  வெறுமை collect_first_உள்ளீடு_classes (hb_ot_layout_ruleset_digest_t *digest) மாறிலி
  {
    digest->init ();
    க்கு (unsigned i = 0; i < rule.len; i++)
    {
      மாறிலி ChainRuleType &r = இது+rule.arrayZ[i];
      மாறிலி auto &உள்ளீடு = StructAfter<decltype (r.உள்ளீடு_X)> (r.backtrack);
      எனில் (உள்ளீடு.lenP1 <= 1)
      {
	digest->set_full ();
	திரும்பு;
      }
      digest->add (உள்ளீடு.arrayZ[0]);
    }
  }
  வெறுமை depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) மாறிலி
  {
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  வெறுமை closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;

    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) மாறிலி
  {
    எனில் (unlikely (c->lookup_limit_exceeded ())) திரும்பு;

    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) மாறிலி
  {
    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest,
			      மாறிலி ClassDef *உள்ளீடு_class_def = nullptr,
			      மாறிலி ClassDef *lookahead_class_def = nullptr) மாறிலி
  {
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_apply ([=] (மாறிலி ChainRuleType &_)
		{ _.collect_second_glyphs (digest, உள்ளீடு_class_def, lookahead_class_def); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c,
		    மாறிலி ChainContextApplyLookupContext &lookup_context) மாறிலி
  {
    திரும்பு
    + hb_iter (rule)
    | hb_map (hb_add (இது))
    | hb_map ([&] (மாறிலி ChainRuleType &_) { திரும்பு _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  தருக்க apply (hb_ot_apply_context_t *c,
	      மாறிலி ChainContextApplyLookupContext &lookup_context,
	      மாறிலி hb_ot_layout_ruleset_digest_t *digest = nullptr) மாறிலி
  {
    TRACE_APPLY (இது);

    unsigned இலக்கம்_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    எனில் (HB_OPTIMIZE_SIZE_VAL || இலக்கம்_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (இது))
      | hb_map ([&] (மாறிலி ChainRuleType &_) { திரும்பு _.apply (c, lookup_context); })
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
    தருக்க matched = skippy_iter.next (&unsafe_to1);
    எனில் (likely (matched))
    {
      எனில் (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        செல் slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      எனில் (digest && lookup_context.get_உள்ளீடு_மதிப்பு &&
	  !digest->may_have (lookup_context.get_உள்ளீடு_மதிப்பு (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (பொய்);
	}
    }
    இல்லையெனில்
    {
      தருக்க unsafe_to_concat = பொய்;
      தருக்க ret =
      + hb_iter (rule)
      | hb_map (hb_add (இது))
      | hb_filter ([&] (மாறிலி ChainRuleType &_)
		   {
		     மாறிலி auto &உள்ளீடு = StructAfter<decltype (_.உள்ளீடு_X)> (_.backtrack);
		     மாறிலி auto &lookahead = StructAfter<decltype (_.lookaheadX)> (உள்ளீடு);
		     எனில் (உள்ளீடு.lenP1 <= 1 && lookahead.len == 0) திரும்பு உண்மை;
		     unsafe_to_concat = உண்மை;
		     திரும்பு பொய்;
		   })
      | hb_map ([&] (மாறிலி ChainRuleType &_) { திரும்பு _.apply (c, lookup_context); })
      | hb_any
      ;
      எனில் (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    எனில் (likely (matched))
    {
      எனில் (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        செல் slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_உள்ளீடு = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *உள்ளீடு_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    க்கு (unsigned முழு i = 0; i < இலக்கம்_rules; i++)
    {
      மாறிலி auto &r = இது+rule.arrayZ[i];

      மாறிலி auto &உள்ளீடு = StructAfter<decltype (r.உள்ளீடு_X)> (r.backtrack);
      மாறிலி auto &lookahead = StructAfter<decltype (r.lookaheadX)> (உள்ளீடு);

      unsigned lenP1 = உள்ளீடு.lenP1;
      எனில் (lenP1 > 1 ?
	   (!match_உள்ளீடு ||
	    match_உள்ளீடு (*first, உள்ளீடு.arrayZ[0], உள்ளீடு_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        எனில் (!second ||
	    (lenP1 > 2 ?
	     (!match_உள்ளீடு ||
	      match_உள்ளீடு (*second, உள்ளீடு.arrayZ[1], உள்ளீடு_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  எனில் (r.apply (c, lookup_context))
	  {
	    எனில் (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (உண்மை);
	  }
	}
	இல்லையெனில்
	  unsafe_to = unsafe_to2;
      }
      இல்லையெனில்
      {
	எனில் (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	எனில் (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  க்கு (; i + 1 < இலக்கம்_rules; i++)
	  {
	    மாறிலி auto &r2 = இது+rule.arrayZ[i + 1];
	    மாறிலி auto &input2 = StructAfter<decltype (r2.உள்ளீடு_X)> (r2.backtrack);
	    எனில் (input2.lenP1 <= 1 || input2.arrayZ[0] != உள்ளீடு.arrayZ[0])
	      நிறுத்து;
	  }
	}
      }
    }
    எனில் (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (பொய்);
  }

  தருக்க subset (hb_subset_context_t *c,
	       மாறிலி hb_map_t *lookup_map,
	       மாறிலி hb_map_t *backtrack_klass_map = nullptr,
	       மாறிலி hb_map_t *உள்ளீடு_klass_map = nullptr,
	       மாறிலி hb_map_t *lookahead_klass_map = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    க்கு (மாறிலி auto& _ : rule)
    {
      எனில் (!_) தொடர்;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      எனில் (unlikely (!o)) தொடர்;

      எனில் (!o->serialize_subset (c, _, இது,
				lookup_map,
				backtrack_klass_map,
				உள்ளீடு_klass_map,
				lookahead_klass_map))
      {
	out->rule.எடு ();
	c->serializer->revert (o_snap);
      }
    }

    தருக்க ret = தருக்க (out->rule);
    எனில் (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (rule.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  பொது:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
கட்டமைப்பு ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (அளவு (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
கட்டமைப்பு ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
கட்டமைப்பு ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    திரும்பு
    + hb_zip (இது+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_map ([&] (மாறிலி ChainRuleSet &_) { திரும்பு _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (இது+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      திரும்பு c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](மாறிலி hb_pair_t<hb_codepoint_t, unsigned> _) { திரும்பு hb_pair_t<unsigned, மாறிலி ChainRuleSet&> (_.first, இது+ruleSet[_.second]); })
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (இது+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      திரும்பு c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](மாறிலி hb_pair_t<hb_codepoint_t, unsigned> _) { திரும்பு hb_pair_t<unsigned, மாறிலி ChainRuleSet&> (_.first, இது+ruleSet[_.second]); })
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி
  {
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (இது+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    (இது+coverage).collect_coverage (c->உள்ளீடு);

    கட்டமைப்பு ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([digest] (மாறிலி ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    மாறிலி ChainRuleSet &rule_set = இது+ruleSet[(இது+coverage).get_coverage (c->glyphs[0])];
    கட்டமைப்பு ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    திரும்பு rule_set.would_apply (c, lookup_context);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    மாறிலி ChainRuleSet &rule_set = இது+ruleSet[index];
    கட்டமைப்பு ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    மாறிலி hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, இது, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (தருக்க (new_coverage));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && ruleSet.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
கட்டமைப்பு ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (glyphs))
      திரும்பு பொய்;

    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

    hb_map_t caches[3] = {};
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (இது+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    உள்ளீடு_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    திரும்பு
    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_enumerate
    | hb_map ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி ChainRuleSet &> p)
	      { திரும்பு உள்ளீடு_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { திரும்பு உள்ளீடு_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  மாறிலி ChainRuleSet& chainrule_set = இது+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs)) திரும்பு;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { திரும்பு உள்ளீடு_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (மாறிலி hb_pair_t<unsigned, மாறிலி typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  மாறிலி ChainRuleSet& chainrule_set = இது+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (c->glyphs))
      திரும்பு;

    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

    hb_map_t caches[3] = {};
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { திரும்பு உள்ளீடு_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (மாறிலி ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    (இது+coverage).collect_coverage (c->உள்ளீடு);

    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

    கட்டமைப்பு ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (இது))
    | hb_apply ([digest, &உள்ளீடு_class_def, &lookahead_class_def] (மாறிலி ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &உள்ளீடு_class_def, &lookahead_class_def); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

    unsigned முழு index = உள்ளீடு_class_def.get_class (c->glyphs[0]);
    மாறிலி ChainRuleSet &rule_set = இது+ruleSet[index];
    கட்டமைப்பு ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def}
    };
    திரும்பு rule_set.would_apply (c, lookup_context);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  unsigned cache_cost () மாறிலி
  {
    திரும்பு (இது+உள்ளீடு_Class_Def).cost () + (இது+lookaheadClassDef).cost ();
  }
  நிலையான தருக்க cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    திரும்பு context_cache_func (c, op);
  }

  கட்டமைப்பு external_cache_t
  {
    hb_ot_layout_இருமம்_cache_t coverage;
    hb_ot_layout_mapping_cache_t உள்ளீடு_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  வெறுமை *external_cache_create () மாறிலி
  {
    unsigned மொத்தம் = ruleSet.len;
    unsigned size = அளவு (external_cache_t) -
		    HB_VAR_ARRAY * அளவு (hb_ot_layout_ruleset_digest_t) +
		    மொத்தம் * அளவு (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    எனில் (likely (cache))
    {
      cache->coverage.clear ();
      cache->உள்ளீடு_class.clear ();
      cache->lookahead_class.clear ();
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
	(இது+ruleSet[i]).collect_first_உள்ளீடு_classes (&cache->rule_sets[i]);
    }
    திரும்பு cache;
  }
  தருக்க apply_cached (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி { திரும்பு _apply (c, உண்மை, external_cache); }
  தருக்க apply (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி { திரும்பு _apply (c, பொய், external_cache); }
  தருக்க _apply (hb_ot_apply_context_t *c, தருக்க cached, வெறுமை *external_cache) மாறிலி
  {
    TRACE_APPLY (இது);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned முழு index = (இது+coverage).get_coverage_இருமம் (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#இல்லையெனில்
    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    மாறிலி ClassDef &backtrack_class_def = இது+backtrackClassDef;
    மாறிலி ClassDef &உள்ளீடு_class_def = இது+உள்ளீடு_Class_Def;
    மாறிலி ClassDef &lookahead_class_def = இது+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t உள்ளீடு_cache_data = {
      &உள்ளீடு_class_def,
      cache ? &cache->உள்ளீடு_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    மாறிலி வெறுமை *உள்ளீடு_match_data = cached
      ? static_cast<மாறிலி வெறுமை *> (&உள்ளீடு_class_def)
      : static_cast<மாறிலி வெறுமை *> (&உள்ளீடு_cache_data);
    மாறிலி வெறுமை *lookahead_match_data = cached
      ? static_cast<மாறிலி வெறுமை *> (&lookahead_class_def)
      : static_cast<மாறிலி வெறுமை *> (&lookahead_cache_data);
    தருக்க backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    மாறிலி வெறுமை *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<மாறிலி வெறுமை *> (&lookahead_cache_data)
      : static_cast<மாறிலி வெறுமை *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    கட்டமைப்பு ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       உள்ளீடு_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_மதிப்பு : get_class_cache_மதிப்பு
#இல்லையெனில்
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &உள்ளீடு_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_மதிப்பு : get_class_மதிப்பு
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (உள்ளீடு_class_def, c->buffer->cur())
         : உள்ளீடு_class_def.get_class (c->buffer->cur().codepoint, உள்ளீடு_cache_data.cache);
#இல்லையெனில்
    index = cached
         ? get_class_cached2 (உள்ளீடு_class_def, c->buffer->cur())
          : உள்ளீடு_class_def.get_class (c->buffer->cur().codepoint);
#endif
    மாறிலி ChainRuleSet &rule_set = இது+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    மாறிலி hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#இல்லையெனில்
    மாறிலி hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, இது);

    hb_map_t backtrack_klass_map;
    hb_map_t உள்ளீடு_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, இது, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->உள்ளீடு_Class_Def.serialize_subset (c, உள்ளீடு_Class_Def, இது, &உள்ளீடு_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, இது, &lookahead_klass_map);

    எனில் (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   உள்ளீடு_klass_map,
						   lookahead_klass_map)))
      return_trace (பொய்);

    மாறிலி hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (இது+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (இது+உள்ளீடு_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    முழு non_zero_index = -1, index = 0;
    தருக்க ret = உண்மை;
    மாறிலி hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    க்கு (மாறிலி auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (உள்ளீடு_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      எனில் (unlikely (!o))
      {
	ret = பொய்;
	நிறுத்து;
      }
      எனில் (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, இது,
			       lookup_map,
			       &backtrack_klass_map,
			       &உள்ளீடு_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    எனில் (!ret || non_zero_index == -1) return_trace (பொய்);

    // prune empty trailing ruleSets
    எனில் (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (தருக்க (out->ruleSet));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) &&
		  backtrackClassDef.sanitize (c, இது) &&
		  உள்ளீடு_Class_Def.sanitize (c, இது) &&
		  lookaheadClassDef.sanitize (c, இது) &&
		  ruleSet.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		உள்ளீடு_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  பொது:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
கட்டமைப்பு ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);

    எனில் (!(இது+உள்ளீடு[0]).intersects (glyphs))
      திரும்பு பொய்;

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {இது, இது, இது}
    };
    திரும்பு chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     உள்ளீடு.len, உள்ளீடு.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);

    எனில் (!(இது+உள்ளீடு[0]).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs))
      திரும்பு;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {இது, இது, இது},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (மாறிலி HBUINT16 *) backtrack.arrayZ,
				 உள்ளீடு.len, (மாறிலி HBUINT16 *) உள்ளீடு.arrayZ + 1,
				 lookahead.len, (மாறிலி HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 இது,
				 உள்ளீடு[0], lookup_context);

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);

    எனில் (!(இது+உள்ளீடு[0]).intersects (c->glyphs))
      திரும்பு;

    hb_set_t* cur_active_glyphs = c->தள்ளு_cur_active_glyphs ();
    எனில் (unlikely (!cur_active_glyphs))
      திரும்பு;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    கட்டமைப்பு ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {இது, இது, இது}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  உள்ளீடு.len, உள்ளீடு.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->எடு_cur_done_glyphs ();
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி
  {
    எனில் (!intersects (c->glyphs))
      திரும்பு;

    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);

    (இது+உள்ளீடு[0]).collect_coverage (c->உள்ளீடு);

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    கட்டமைப்பு ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {இது, இது, இது}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 உள்ளீடு.len, உள்ளீடு.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    எனில் (உள்ளீடு.len > 1)
      (இது+உள்ளீடு[1]).collect_coverage (digest);
    இல்லையெனில் எனில் (lookahead.len)
      (இது+lookahead[0]).collect_coverage (digest);
    இல்லையெனில்
      *digest = hb_set_digest_t::full ();
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    கட்டமைப்பு ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {இது, இது, இது}
    };
    திரும்பு chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     உள்ளீடு.len, உள்ளீடு.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  மாறிலி Coverage &get_coverage () மாறிலி
  {
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    திரும்பு இது+உள்ளீடு[0];
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);

    unsigned முழு index = (இது+உள்ளீடு[0]).get_coverage (c->buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    கட்டமைப்பு ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {இது, இது, இது}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      உள்ளீடு.len, உள்ளீடு.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, மாறிலி வெறுமை* base) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    எனில் (unlikely (!out->serialize (c->serializer, 0u))) return_trace (பொய்);

    க்கு (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      எனில் (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    எனில் (unlikely (!c->serializer->embed (இது->format))) return_trace (பொய்);

    எனில் (!serialize_coverage_offsets (c, backtrack.iter (), இது))
      return_trace (பொய்);

    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    எனில் (!serialize_coverage_offsets (c, உள்ளீடு.iter (), இது))
      return_trace (பொய்);

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    எனில் (!serialize_coverage_offsets (c, lookahead.iter (), இது))
      return_trace (பொய்);

    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    மாறிலி hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_மொத்தம்_2 = c->serializer->copy<HBUINT16> (lookup.len);
    எனில் (!lookup_மொத்தம்_2) return_trace (பொய்);

    unsigned மொத்தம் = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_மொத்தம்_2, மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!backtrack.sanitize (c, இது))) return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &உள்ளீடு = StructAfter<decltype (உள்ளீடு_X)> (backtrack);
    எனில் (unlikely (!உள்ளீடு.sanitize (c, இது))) return_trace (பொய்);
    hb_barrier ();
    எனில் (unlikely (!உள்ளீடு.len)) return_trace (பொய்); /* To be consistent with Context. */
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (உள்ளீடு);
    எனில் (unlikely (!lookahead.sanitize (c, இது))) return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		உள்ளீடு_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  பொது:
  DEFINE_SIZE_MIN (10);
};

கட்டமைப்பு ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    நிலை 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }		format;	/* Format identifier */
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
கட்டமைப்பு ExtensionFormat1
{
  unsigned முழு get_type () மாறிலி { திரும்பு extensionLookupType; }

  template <typename X>
  மாறிலி X& get_subtable () மாறிலி
  { திரும்பு இது + reinterpret_cast<மாறிலி Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, இது))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    மாறிலி auto& src_offset =
        reinterpret_cast<மாறிலி Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, இது, get_type ()));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  பொது:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
கட்டமைப்பு Extension
{
  unsigned முழு get_type () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_type ();
    இயல்பு:திரும்பு 0;
    }
  }
  template <typename X>
  மாறிலி X& get_subtable () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.template get_subtable<typename T::SubTable> ();
    இயல்பு:திரும்பு Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.subset (c);
    இயல்பு: திரும்பு c->default_return_மதிப்பு ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

கட்டமைப்பு hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  நிலையான hb_ot_layout_lookup_accelerator_t *create (மாறிலி TLookup &lookup)
  {
    unsigned மொத்தம் = lookup.get_subtable_மொத்தம் ();

    unsigned size = அளவு (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * அளவு (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    மொத்தம் * அளவு (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    எனில் (unlikely (!thiz))
      திரும்பு nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    போது (மொத்தம் && !thiz->subtables[மொத்தம் - 1].apply_func)
      மொத்தம்--;

    thiz->digest.init ();
    க்கு (auto& subtable : hb_iter (thiz->subtables, மொத்தம்))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->மொத்தம் = மொத்தம்;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      எனில் (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    திரும்பு thiz;
  }

  வெறுமை fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  தருக்க may_have (hb_codepoint_t g) மாறிலி
  { திரும்பு digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க apply (hb_ot_apply_context_t *c, தருக்க use_cache) மாறிலி
  {
    எனில் (மொத்தம் == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      எனில் (use_cache)
	திரும்பு subtables[0].apply_cached_no_digest (c);
#endif
      திரும்பு subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    எனில் (use_cache)
    {
      திரும்பு
      + hb_iter (hb_iter (subtables, மொத்தம்))
      | hb_map ([&c] (மாறிலி hb_accelerate_subtables_context_t::hb_applicable_t &_) { திரும்பு _.apply_cached (c); })
      | hb_any
      ;
    }
    இல்லையெனில்
#endif
    {
      திரும்பு
      + hb_iter (hb_iter (subtables, மொத்தம்))
      | hb_map ([&c] (மாறிலி hb_accelerate_subtables_context_t::hb_applicable_t &_) { திரும்பு _.apply (c); })
      | hb_any
      ;
    }
    திரும்பு பொய்;
  }

  தருக்க cache_enter (hb_ot_apply_context_t *c) மாறிலி
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    திரும்பு subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#இல்லையெனில்
    திரும்பு பொய்;
#endif
  }
  வெறுமை cache_leave (hb_ot_apply_context_t *c) மாறிலி
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  தனிப்பட்ட:
  unsigned மொத்தம் = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

கட்டமைப்பு GSUBGPOS
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

  size_t get_size () மாறிலி
  {
    எனில் (version.major != 1) திரும்பு version.static_size;

    திரும்பு min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#இல்லையெனில்
	   0
#endif
	   ;
  }

  மாறிலி ScriptList &get_script_list () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u && scriptList2) திரும்பு இது + scriptList2;
#endif
    திரும்பு இது + scriptList;
  }
  மாறிலி FeatureList &get_feature_list () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u && featureList2) திரும்பு இது + featureList2;
#endif
    திரும்பு இது + featureList;
  }
  template <typename TLookup>
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (இது, get_size ())))
      return_trace (பொய்);
    hb_barrier ();

    தருக்க script_list_ok = scriptList.sanitize (c, இது);
    தருக்க feature_list_ok = featureList.sanitize (c, இது);
    தருக்க lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, இது);

#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u)
    {
      எனில் (scriptList2) script_list_ok = scriptList2.sanitize (c, இது);
      எனில் (featureList2) feature_list_ok = featureList2.sanitize (c, இது);
      எனில் (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, இது);
    }
#endif

    எனில் (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (பொய்);

#ifndef HB_NO_VAR
    எனில் (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, இது, &get_feature_list ()))))
      return_trace (பொய்);
#endif

    return_trace (உண்மை);
  }

  template <typename TLookup>
  தருக்க subset (hb_subset_layout_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (version.major != 1) return_trace (பொய்);

    auto *out = c->subset_context->serializer->start_embed (இது);
    எனில் (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (பொய்);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u)
    {
      எனில் (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (பொய்);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      எனில் (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 இது, c);
      இல்லையெனில்
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 இது, c);

      எனில் (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 இது, c);
      இல்லையெனில்
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 இது, c);

      எனில் (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, இது, c);
      இல்லையெனில்
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, இது, c);
    }
    இல்லையெனில்
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     இது,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     இது,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					இது,
					c);
    }

#ifndef HB_NO_VAR
    எனில் (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      எனில் (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (பொய்);

      // if all axes are pinned all feature vars are dropped.
      தருக்க ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, இது, c);
      எனில் (!ret)
      {
        out->featureVars = 0;
	எனில் (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (உண்மை);
  }

  unsigned முழு get_lookup_மொத்தம் () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u && lookupList2)
      திரும்பு (இது + lookupList2).len;
#endif
    திரும்பு (இது + lookupList).len;
  }
  மாறிலி Lookup& get_lookup (unsigned முழு i) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u && lookupList2)
      திரும்பு (இது + lookupList2)[i];
#endif
    திரும்பு (இது + lookupList)[i];
  }
  மாறிலி FeatureVariations &get_feature_variations () மாறிலி
  {
    திரும்பு (version.to_int () >= 0x00010001u && hb_barrier ()) ? இது+featureVars : Null (FeatureVariations);
  }

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }
  unsigned முழு get_script_மொத்தம் () மாறிலி
  { திரும்பு get_script_list ().len; }
  மாறிலி Tag& get_script_tag (unsigned முழு i) மாறிலி
  { திரும்பு get_script_list ().get_tag (i); }
  unsigned முழு get_script_tags (unsigned முழு start_offset,
				unsigned முழு *script_மொத்தம் /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) மாறிலி
  { திரும்பு get_script_list ().get_tags (start_offset, script_மொத்தம், script_tags); }
  மாறிலி Script& get_script (unsigned முழு i) மாறிலி
  { திரும்பு get_script_list ()[i]; }
  தருக்க find_script_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  { திரும்பு get_script_list ().find_index (tag, index); }

  unsigned முழு get_feature_மொத்தம் () மாறிலி
  { திரும்பு get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned முழு i) மாறிலி
  { திரும்பு i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned முழு get_feature_tags (unsigned முழு start_offset,
				 unsigned முழு *feature_மொத்தம்_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) மாறிலி
  { திரும்பு get_feature_list ().get_tags (start_offset, feature_மொத்தம்_2, feature_tags); }
  மாறிலி Feature& get_feature (unsigned முழு i) மாறிலி
  { திரும்பு get_feature_list ()[i]; }
  தருக்க find_feature_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  { திரும்பு get_feature_list ().find_index (tag, index); }

  தருக்க find_variations_index (மாறிலி முழு *coords, unsigned முழு இலக்கம்_coords,
			      unsigned முழு *index,
			      ItemVarStoreInstancer *instancer) மாறிலி
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    திரும்பு பொய்;
#endif
    திரும்பு get_feature_variations ().find_index (coords, இலக்கம்_coords, index, instancer);
  }
  தருக்க get_feature_variations_state (மாறிலி முழு *coords,
				     unsigned முழு இலக்கம்_coords,
				     unsigned முழு *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) மாறிலி
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    திரும்பு உண்மை;
#இல்லையெனில்
    find_variations_index (coords, இலக்கம்_coords, index, instancer);
    திரும்பு get_feature_variations ().collect_lookup_conditions (
	coords,
	இலக்கம்_coords,
	instancer,
	lookup_conditions);
#endif
  }
  தருக்க get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<மாறிலி unsigned> conditions,
                              hb_set_t *lookup_indexes) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு பொய்;
#இல்லையெனில்
    திரும்பு get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  மாறிலி Feature& get_feature_variation (unsigned முழு feature_index,
					unsigned முழு variations_index) மாறிலி
  {
#ifndef HB_NO_VAR
    எனில் (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      மாறிலி Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      எனில் (feature)
	திரும்பு *feature;
    }
#endif
    திரும்பு get_feature (feature_index);
  }

  வெறுமை feature_variation_collect_lookups (மாறிலி hb_set_t *feature_indexes,
					  மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) மாறிலி
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  வெறுமை collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  வெறுமை collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) மாறிலி
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  வெறுமை closure_lookups (hb_face_t      *face,
			மாறிலி hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) மாறிலி
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    க்கு (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<மாறிலி TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  வெறுமை prune_langsys (மாறிலி hb_map_t *duplicate_feature_map,
                      மாறிலி hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) மாறிலி
  {
    hb_prune_langsys_context_t c (இது, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned மொத்தம் = get_script_மொத்தம் ();
    க்கு (unsigned script_index = 0; script_index < மொத்தம்; script_index++)
    {
      மாறிலி Tag& tag = get_script_tag (script_index);
      எனில் (!layout_scripts->has (tag)) தொடர்;
      மாறிலி Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  வெறுமை prune_features (மாறிலி hb_map_t *lookup_indices, /* IN */
		       மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       மாறிலி hb_hashmap_t<unsigned, மாறிலி Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) மாறிலி
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    எனில் (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      திரும்பு;
    }
#endif

    க்கு (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      எனில் (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        தொடர்;


      மாறிலி Feature *f = &(get_feature (i));
      மாறிலி Feature** p = nullptr;
      எனில் (feature_substitutes_map->has (i, &p))
        f = *p;

      எனில் (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        தொடர்;

      எனில் (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  வெறுமை collect_name_ids (மாறிலி hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         மாறிலி hb_hashmap_t<unsigned, மாறிலி Feature*> *feature_substitutes_map,
                         மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         மாறிலி hb_set_t *catch_all_features,
                         தருக்க all_axes_pinned HB_UNUSED) மாறிலி
  {
    unsigned மொத்தம் = get_feature_மொத்தம் ();
    க்கு (unsigned i = 0 ; i < மொத்தம்; i++)
    {
      எனில் (!feature_index_map->has (i)) தொடர்;
      hb_tag_t tag = get_feature_tag (i);
      மாறிலி Feature *feature = &get_feature (i);
      மாறிலி Feature **substitute;
      எனில் (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        எனில் (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    எனில் (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [இது] (unsigned i) { திரும்பு get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = உண்மை;
      இது->table = sc.reference_table<T> (face);

      எனில் (unlikely (இது->table->is_blocklisted (இது->table.get_blob (), face)))
      {
	hb_blob_destroy (இது->table.get_blob ());
	இது->table = hb_blob_get_empty ();
      }

      இது->lookup_மொத்தம் = table->get_lookup_மொத்தம் ();

      இது->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (இது->lookup_மொத்தம், அளவு (*accels));
      எனில் (unlikely (!இது->accels))
      {
	இது->lookup_மொத்தம் = 0;
	இது->table.destroy ();
	இது->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      க்கு (unsigned முழு i = 0; i < இது->lookup_மொத்தம்; i++)
      {
	auto *accel = இது->accels[i].get_relaxed ();
	எனில் (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (இது->accels);
      இது->table.destroy ();
    }

    hb_blob_t *get_blob () மாறிலி { திரும்பு table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) மாறிலி
    {
      எனில் (unlikely (lookup_index >= lookup_மொத்தம்)) திரும்பு nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      எனில் (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	எனில் (unlikely (!accel))
	  திரும்பு nullptr;

	எனில் (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  செல் retry;
	}
      }

      திரும்பு accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned முழு lookup_மொத்தம்;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  பாதுகாக்கப்பட்ட:
  template <typename TLookup>
  மாறிலி Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  மாறிலி Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  மாறிலி Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { திரும்பு reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  மாறிலி Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  மாறிலி Offset16To<RecordListOfFeature>& subset_feature_list_offset () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { திரும்பு reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  மாறிலி Offset32To<RecordListOfFeature>& subset_feature_list2_offset () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  மாறிலி வெறுமை* get_lookup_list_field_offset () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.to_int () >= 0x00010002u && lookupList2) திரும்பு &lookupList2;
#endif
    திரும்பு &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  நிலையான தருக்க serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       மாறிலி OffsetIn& in,
				       மாறிலி Base *base,
				       Ts&&... ds)
  {
    out = 0;
    எனில் (in.is_null ()) திரும்பு பொய்;

    auto *s = c->serializer;
    s->push ();
    தருக்க ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    எனில் (ret) s->add_link (out, s->எடு_pack ());
    இல்லையெனில் s->எடு_discard ();
    திரும்பு ret;
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
  பொது:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
