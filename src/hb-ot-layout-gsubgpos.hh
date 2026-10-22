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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-buffer.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gdef-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-depend-data.hh"


ନାମାକାଶ OT {


ସଂରଚନା hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, ତାର୍କିକ>
{
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { ଫେରନ୍ତୁ obj.intersects (ଏହି->glyphs); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ { ଫେରନ୍ତୁ r; }

  ଅଚଳ hb_set_t *glyphs;

  hb_intersects_context_t (ଅଚଳ hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

ସଂରଚନା hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, ତାର୍କିକ>
{
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { ଫେରନ୍ତୁ obj.may_have_non_1to1 (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ { ଫେରନ୍ତୁ r; }
};

ସଂରଚନା hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { obj.closure (ଏହି); ଫେରନ୍ତୁ hb_empty_t (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }
  ଶୂନ୍ୟ recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    ଯଦି (unlikely (nesting_level_ବାମ == 0 || !recurse_func))
      ଫେରନ୍ତୁ;

    nesting_level_ବାମ--;
    recurse_func (ଏହି, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_ବାମ++;
  }

  ଶୂନ୍ୟ reset_lookup_visit_ଗଣନା ()
  { lookup_ଗଣନା = 0; }

  ତାର୍କିକ lookup_limit_exceeded ()
  { ଫେରନ୍ତୁ lookup_ଗଣନା > HB_MAX_LOOKUP_VISIT_ଗଣନା; }

  ତାର୍କିକ should_visit_lookup (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index)
  {
    ଯଦି (lookup_ଗଣନା++ > HB_MAX_LOOKUP_VISIT_ଗଣନା)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (is_lookup_done (lookup_index))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ is_lookup_done (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index)
  {
    ଯଦି (unlikely (done_lookups_glyph_ଗଣନା->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned କାଢ଼ = glyphs->get_population ();
    ଯଦି (unlikely (କାଢ଼ == HB_MAP_ମୂଲ୍ୟ_INVALID)) // Avoid sentinel interference.
      କାଢ଼--;
    ଯଦି (done_lookups_glyph_ଗଣନା->get (lookup_index) != କାଢ଼)
    {
      done_lookups_glyph_ଗଣନା->set (lookup_index, କାଢ଼);

      ଯଦି (!done_lookups_glyph_set->has (lookup_index))
      {
	ଯଦି (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  ଫେରନ୍ତୁ ସତ୍ୟ;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    ଯଦି (unlikely (covered_glyph_set->in_error ()))
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (parent_active_glyphs ().is_subset (*covered_glyph_set))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    covered_glyph_set->union_ (parent_active_glyphs ());
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଅଚଳ hb_set_t& previous_parent_active_glyphs () {
    ଯଦି (active_glyphs_ଗଦା.length <= 1)
      ଫେରନ୍ତୁ *glyphs;

    ଫେରନ୍ତୁ active_glyphs_ଗଦା[active_glyphs_ଗଦା.length - 2];
  }

  ଅଚଳ hb_set_t& parent_active_glyphs ()
  {
    ଯଦି (!active_glyphs_ଗଦା)
      ଫେରନ୍ତୁ *glyphs;

    ଫେରନ୍ତୁ active_glyphs_ଗଦା.tail ();
  }

  hb_set_t* ଠେଲ_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_ଗଦା.push ();
    ଯଦି (unlikely (active_glyphs_ଗଦା.in_error ()))
      ଫେରନ୍ତୁ nullptr;
    ଫେରନ୍ତୁ s;
  }

  ତାର୍କିକ କାଢ଼_cur_done_glyphs ()
  {
    ଯଦି (!active_glyphs_ଗଦା)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    active_glyphs_ଗଦା.କାଢ଼ ();
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_ଗଦା;
  recurse_func_t recurse_func = nullptr;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting_level_ବାମ;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_ଗଣନା_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting_level_ବାମ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_ବାମ (nesting_level_ବାମ_2),
			  done_lookups_glyph_ଗଣନା (done_lookups_glyph_ଗଣନା_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  ଶୂନ୍ୟ set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ଶୂନ୍ୟ flush ()
  {
    output->del_range (face->get_ଅଙ୍କ_glyphs (), HB_SET_ମୂଲ୍ୟ_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_ଗଦା.କାଢ଼ ();
    active_glyphs_ଗଦା.reset ();
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_map_t *done_lookups_glyph_ଗଣନା;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା = 0;
};



ସଂରଚନା hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { obj.closure_lookups (ଏହି); ଫେରନ୍ତୁ hb_empty_t (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }
  ଶୂନ୍ୟ recurse (unsigned lookup_index)
  {
    ଯଦି (unlikely (nesting_level_ବାମ == 0 || !recurse_func))
      ଫେରନ୍ତୁ;

    /* Return if new lookup was recursed to before. */
    ଯଦି (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      ଫେରନ୍ତୁ;

    nesting_level_ବାମ--;
    recurse_func (ଏହି, lookup_index);
    nesting_level_ବାମ++;
  }

  ଶୂନ୍ୟ set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  ଶୂନ୍ୟ set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  ତାର୍କିକ lookup_limit_exceeded ()
  {
    ତାର୍କିକ ret = lookup_ଗଣନା > HB_MAX_LOOKUP_VISIT_ଗଣନା;
    ଯଦି (ret)
      DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    ଫେରନ୍ତୁ ret; }

  ତାର୍କିକ is_lookup_visited (unsigned lookup_index)
  {
    ଯଦି (unlikely (lookup_ଗଣନା++ > HB_MAX_LOOKUP_VISIT_ଗଣନା))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_ଗଣନା, lookup_index);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଯଦି (unlikely (visited_lookups->in_error ()))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    ଫେରନ୍ତୁ visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  ଅଚଳ hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting_level_ବାମ;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ଅଚଳ hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_ବାମ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_ବାମ (nesting_level_ବାମ_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_ଗଣନା (0) {}

  ଶୂନ୍ୟ set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ବ୍ୟକ୍ତିଗତ:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା;
};

ସଂରଚନା hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, ତାର୍କିକ>
{
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { ଫେରନ୍ତୁ obj.would_apply (ଏହି); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ { ଫେରନ୍ତୁ r; }

  hb_face_t *face;
  ଅଚଳ hb_codepoint_t *glyphs;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ତାର୍କିକ zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ଅଚଳ hb_codepoint_t *glyphs_,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len_,
			    ତାର୍କିକ zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

ସଂରଚନା hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index);
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { obj.collect_glyphs (ଏହି); ଫେରନ୍ତୁ hb_empty_t (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }
  ଶୂନ୍ୟ recurse (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index)
  {
    ଯଦି (unlikely (nesting_level_ବାମ == 0 || !recurse_func))
      ଫେରନ୍ତୁ;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    ଯଦି (output == hb_set_get_empty ())
      ଫେରନ୍ତୁ;

    /* Return if new lookup was recursed to before. */
    ଯଦି (recursed_lookups->has (lookup_index))
      ଫେରନ୍ତୁ;

    hb_set_t *old_before = before;
    hb_set_t *old_ନିବେଶ  = ନିବେଶ;
    hb_set_t *old_after  = after;
    before = ନିବେଶ = after = hb_set_get_empty ();

    nesting_level_ବାମ--;
    recurse_func (ଏହି, lookup_index);
    nesting_level_ବାମ++;

    before = old_before;
    ନିବେଶ  = old_ନିବେଶ;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *ନିବେଶ;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting_level_ବାମ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_ନିବେଶ,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting_level_ବାମ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      ନିବେଶ  (glyphs_ନିବେଶ  ? glyphs_ନିବେଶ  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_ବାମ (nesting_level_ବାମ_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  ଶୂନ୍ୟ set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ସଂରଚନା hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  ଗଣନା glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  ସ୍ଥିର constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  ସଂରଚନା glyph_set_cache_ଚାବି_t
  {
    glyph_set_cache_ଚାବି_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    glyph_set_cache_ଚାବି_t (ଅଚଳ ଶୂନ୍ୟ *data_, unsigned ମୂଲ୍ୟ_3, glyph_set_type_t type_)
      : data (data_), value (ମୂଲ୍ୟ_3), type (type_) {}

    ତାର୍କିକ operator == (ଅଚଳ glyph_set_cache_ଚାବି_t &o) ଅଚଳ
    { ଫେରନ୍ତୁ data == o.data && value == o.value && type == o.type; }

    uint32_t hash () ଅଚଳ
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      ଫେରନ୍ତୁ current * 31 + hb_hash ((unsigned) type);
    }

    ଅଚଳ ଶୂନ୍ୟ *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  ସଂରଚନା context_set_cache_ଚାବି_t
  {
    context_set_cache_ଚାବି_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    context_set_cache_ଚାବି_t (ଅଚଳ ଶୂନ୍ୟ *rule_,
			     ଅଚଳ ଶୂନ୍ୟ *record_,
			     ଅଚଳ ଶୂନ୍ୟ *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned ମୂଲ୍ୟ_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (ମୂଲ୍ୟ_3), format (format_) {}

    ତାର୍କିକ operator == (ଅଚଳ context_set_cache_ଚାବି_t &o) ଅଚଳ
    {
      ଫେରନ୍ତୁ rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ଅଚଳ
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      ଫେରନ୍ତୁ current * 31 + hb_hash (format);
    }

    ଅଚଳ ଶୂନ୍ୟ *rule = nullptr;
    ଅଚଳ ଶୂନ୍ୟ *record = nullptr;
    ଅଚଳ ଶୂନ୍ୟ *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  ସଂରଚନା recurse_ଚାବି_t
  {
    recurse_ଚାବି_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    recurse_ଚାବି_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    ତାର୍କିକ operator == (ଅଚଳ recurse_ଚାବି_t &o) ଅଚଳ
    {
      ଫେରନ୍ତୁ lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () ଅଚଳ
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      ଫେରନ୍ତୁ current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  ପ୍ରକାରବ୍ୟାଖ୍ୟା return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { obj.depend (ଏହି); ଫେରନ୍ତୁ hb_empty_t (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  ଶୂନ୍ୟ recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    ଯଦି (lookups_seen.has (lookup_idx)) ଫେରନ୍ତୁ;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_ଚାବି = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    ଯଦି (!recurse_path_to_index.has (path_ଚାବି, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      ଯଦି (unlikely (!recurse_path_to_index.set (path_ଚାବି, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	ଫେରନ୍ତୁ;
      }
    }
    ଅନ୍ୟଥା
      path_index = *path;
    recurse_path = path_index;
    recurse_func (ଏହି, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  ସଂରଚନା active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    ତାର୍କିକ use_ମୂଳ = ମିଥ୍ୟା;
  };

  ଅଚଳ hb_set_t& parent_active_glyphs ()
  {
    ଯଦି (!active_glyphs_ଗଦା_depth)
      ଫେରନ୍ତୁ *glyphs;

    ଅଚଳ active_glyphs_frame_t &frame = active_glyphs_ଗଦା[active_glyphs_ଗଦା_depth - 1];
    ଫେରନ୍ତୁ frame.use_ମୂଳ ? *glyphs : frame.glyphs;
  }

  hb_set_t* ଠେଲ_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    ଯଦି (active_glyphs_ଗଦା_depth == active_glyphs_ଗଦା.length &&
	unlikely (!active_glyphs_ଗଦା.ଠେଲ_or_fail ()))
      ଫେରନ୍ତୁ nullptr;

    active_glyphs_frame_t &frame = active_glyphs_ଗଦା[active_glyphs_ଗଦା_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_ମୂଳ = ମିଥ୍ୟା;
    ଫେରନ୍ତୁ &frame.glyphs;
  }

  ତାର୍କିକ ଠେଲ_ମୂଳ_active_glyphs ()
  {
    ଯଦି (active_glyphs_ଗଦା_depth == active_glyphs_ଗଦା.length &&
	unlikely (!active_glyphs_ଗଦା.ଠେଲ_or_fail ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    active_glyphs_ଗଦା[active_glyphs_ଗଦା_depth++].use_ମୂଳ = ସତ୍ୟ;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ କାଢ଼_cur_done_glyphs ()
  {
    ଯଦି (!active_glyphs_ଗଦା_depth)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    active_glyphs_ଗଦା_depth--;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    ଯଦି (!active_glyphs_ଗଦା_depth ||
	active_glyphs_ଗଦା[active_glyphs_ଗଦା_depth - 1].use_ମୂଳ)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    active_glyphs_frame_t &frame = active_glyphs_ଗଦା[active_glyphs_ଗଦା_depth - 1];
    *active_idx = frame.active_idx;
    ଯଦି (*active_idx != HB_CODEPOINT_INVALID)
      ଫେରନ୍ତୁ ସତ୍ୟ;

    hb_codepoint_t glyph;
    ଯଦି (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    ଅନ୍ୟଥା ଯଦି (!find_or_create_recurse_set (frame.glyphs, active_idx))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    frame.active_idx = *active_idx;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_context_set_cache_ଚାବି (ଅଚଳ ଶୂନ୍ୟ *rule,
				  ଅଚଳ ଶୂନ୍ୟ *record,
				  ଅଚଳ ଶୂନ୍ୟ *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_ଚାବି_t *key)
  {
    hb_codepoint_t active_idx;
    ଯଦି (!get_parent_active_glyphs_index (&active_idx))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *key = context_set_cache_ଚାବି_t (rule, record, data, active_idx, value, format);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_recurse_ଚାବି (unsigned lookup_idx, recurse_ଚାବି_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    ଯଦି (!get_parent_active_glyphs_index (&active_idx))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    *key = recurse_ଚାବି_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    ଫେରନ୍ତୁ !completed_recursions.has (*key);
  }

  ଶୂନ୍ୟ finish_recurse (ଅଚଳ recurse_ଚାବି_t &key)
  { depend_data->check_success (completed_recursions.set (key, ସତ୍ୟ)); }

  ଶୂନ୍ୟ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  ତାର୍କିକ find_or_create_recurse_set (ଅଚଳ hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    ଯଦି (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    ଯଦି (unlikely (!copy))
      ଫେରନ୍ତୁ depend_data->fail ();
    copy->set (set);
    ଯଦି (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      ଫେରନ୍ତୁ depend_data->fail ();
    }

    *index = recurse_sets.length;
    ଯଦି (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      ଫେରନ୍ତୁ depend_data->fail ();
    }
    ଯଦି (unlikely (!recurse_sets.ଠେଲ_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      ଫେରନ୍ତୁ depend_data->fail ();
    ଯଦି (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      ଫେରନ୍ତୁ depend_data->fail ();
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଅଚଳ hb_set_t *find_or_create_glyph_set (ଅଚଳ ଶୂନ୍ୟ *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ଅଚଳ hb_set_t *set);

  ସଂରଚନା depend_scratch_t
  {
    ଶୂନ୍ୟ clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      ନିବେଶ_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ଅଚଳ hb_set_t *> backtrack_sets;
    hb_vector_t<ଅଚଳ hb_set_t *> lookahead_sets;
    hb_vector_t<ଅଚଳ hb_set_t *> ନିବେଶ_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    ଯଦି (scratch)
      depend_scratch_pool = scratch->next;
    ଅନ୍ୟଥା
    {
      scratch = (depend_scratch_t *) hb_malloc (ଆକାର (depend_scratch_t));
      ଯଦି (unlikely (!scratch))
      {
        depend_data->fail ();
        ଫେରନ୍ତୁ nullptr;
      }
      ନୂତନ (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    ଫେରନ୍ତୁ scratch;
  }

  ଶୂନ୍ୟ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_ଗଦା;
  unsigned active_glyphs_ଗଦା_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ଅଚଳ hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_ଚାବି_t, ତାର୍କିକ> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_ଚାବି_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_ଚାବି_t, hb_codepoint_t> context_set_cache;
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
    ଯେତେବେଳେ (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  ଶୂନ୍ୟ set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
ସଂରଚନା hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ଅଚଳ Coverage &>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଚଳ Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { ଫେରନ୍ତୁ obj.get_coverage (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ Null (Coverage); }
  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ
  {
    r.collect_coverage (set);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

ସଂରଚନା matcher_t
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*match_func_t) (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data);

  template <typename context_t>
  ଶୂନ୍ୟ init (ଅଚଳ context_t *c, ତାର୍କିକ context_match = ମିଥ୍ୟା)
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

  ଶୂନ୍ୟ set_match_func (match_func_t match_func_,
		       ଅଚଳ ଶୂନ୍ୟ *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  ଗଣନା may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) ଅଚଳ
  {
    ଯଦି (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      ଫେରନ୍ତୁ MATCH_NO;

    ଯଦି (match_func)
      ଫେରନ୍ତୁ match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    ଫେରନ୍ତୁ MATCH_MAYBE;
  }

  ଗଣନା may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (ଅଚଳ context_t *c,
		       ଅଚଳ hb_glyph_info_t &info) ଅଚଳ
  {
    ଯଦି (!c->check_glyph_property (&info, lookup_props))
      ଫେରନ୍ତୁ SKIP_YES;

    ଯଦି (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      ଫେରନ୍ତୁ SKIP_MAYBE;

    ଫେରନ୍ତୁ SKIP_NO;
  }

  ସର୍ବସାଧାରଣ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_props = 0;
  hb_mask_t mask = -1;
  ତାର୍କିକ ignore_zwnj = ମିଥ୍ୟା;
  ତାର୍କିକ ignore_zwj = ମିଥ୍ୟା;
  ତାର୍କିକ ignore_hidden = ମିଥ୍ୟା;
  ତାର୍କିକ per_syllable = ମିଥ୍ୟା;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ଅଚଳ ଶୂନ୍ୟ *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
ସଂରଚନା glyph_data_t
{
  template <typename T>
  glyph_data_t (ଅଚଳ T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (ଆକାର (T) == 3)
#endif
  {
    static_assert (ଆକାର (T) == 2 || ଆକାର (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (wide)
      ଫେରନ୍ତୁ ((ଅଚଳ HBUINT24 *) data)[i];
#endif
    ଫେରନ୍ତୁ ((ଅଚଳ HBUINT16 *) data)[i];
  }

  ଅଚଳ ଶୂନ୍ୟ *data;
#ifndef HB_NO_BEYOND_64K
  ତାର୍କିକ wide;
#endif
};

template <typename context_t>
ସଂରଚନା skipping_iterator_t
{
  ଶୂନ୍ୟ init (context_t *c_, ତାର୍କିକ context_match = ମିଥ୍ୟା)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  ଶୂନ୍ୟ set_lookup_props (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  ଶୂନ୍ୟ set_match_func (matcher_t::match_func_t match_func_,
		       ଅଚଳ ଶୂନ୍ୟ *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  ଶୂନ୍ୟ set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (ଅଚଳ HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (ଅଚଳ HBUINT24 *) glyph_data.data : nullptr;
#ଅନ୍ୟଥା
    match_glyph_data16 = (ଅଚଳ HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ଶୂନ୍ୟ reset (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  ଶୂନ୍ୟ reset_back (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index_, ତାର୍କିକ from_out_buffer = ମିଥ୍ୟା)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ଶୂନ୍ୟ reset_fast (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ଅଚଳ hb_glyph_info_t &info) ଅଚଳ
  { ଫେରନ୍ତୁ matcher.may_skip (c, info); }

  ଗଣନା match_t {
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
    ଯଦି (unlikely (skip == matcher_t::SKIP_YES))
      ଫେରନ୍ତୁ SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    ଯଦି (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      ଫେରନ୍ତୁ MATCH;

    ଯଦି (skip == matcher_t::SKIP_NO)
      ଫେରନ୍ତୁ NOT_MATCH;

    ଫେରନ୍ତୁ SKIP;
  }

  ତାର୍କିକ next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    ଅଚଳ signed stop = (signed) end - 1;
    ଯେତେବେଳେ ((signed) idx < stop)
    {
      idx++;
      ବାଛ (match (info[idx]))
      {
	ସ୍ଥିତି MATCH:
	{
	  advance_glyph_data ();
	  ଫେରନ୍ତୁ ସତ୍ୟ;
	}
	ସ୍ଥିତି NOT_MATCH:
	{
	  ଯଦି (unsafe_to)
	    *unsafe_to = idx + 1;
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
	ସ୍ଥିତି SKIP:
	  ଜାରି;
      }
    }
    ଯଦି (unsafe_to)
      *unsafe_to = end;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    ଅଚଳ unsigned stop = 0;
    ଯେତେବେଳେ (idx > stop)
    {
      idx--;
      ବାଛ (match (out_info[idx]))
      {
	ସ୍ଥିତି MATCH:
	{
	  advance_glyph_data ();
	  ଫେରନ୍ତୁ ସତ୍ୟ;
	}
	ସ୍ଥିତି NOT_MATCH:
	{
	  ଯଦି (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
	ସ୍ଥିତି SKIP:
	  ଜାରି;
      }
    }
    ଯଦି (unsafe_from)
      *unsafe_from = 0;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    ଯଦି (match_glyph_data16) ଫେରନ୍ତୁ *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    ଅନ୍ୟଥା
    ଯଦି (match_glyph_data24) ଫେରନ୍ତୁ *match_glyph_data24;
#endif
    ଫେରନ୍ତୁ 0;
  }
  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ
  advance_glyph_data ()
  {
    ଯଦି (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    ଅନ୍ୟଥା
    ଯଦି (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx;
  ସୁରକ୍ଷିତ:
  context_t *c;
  matcher_t matcher;
  ଅଚଳ HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ଅଚଳ HBUINT24 *match_glyph_data24;
#endif

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end;
};

ସଂରଚନା hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, ତାର୍କିକ, HB_DEBUG_APPLY>
{
  ଅଚଳ ଅକ୍ଷର *get_name () { ଫେରନ୍ତୁ "APPLY"; }
  ପ୍ରକାରବ୍ୟାଖ୍ୟା return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index);

  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_ (ଅଚଳ T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_ (ଅଚଳ T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { ଫେରନ୍ତୁ apply_(obj, ଏହି, hb_prioritize); }

  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ { ଫେରନ୍ତୁ r; }
  return_t recurse (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sub_lookup_index)
  {
    ପ୍ରତିଜ୍ଞା (recurse_func);
    ଯଦି (unlikely (nesting_level_ବାମ == 0))
    {
      buffer->successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ default_return_ମୂଲ୍ୟ ();
    }

    buffer->max_ops--;
    ଯଦି (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ default_return_ମୂଲ୍ୟ ();
    }

    nesting_level_ବାମ--;
    ତାର୍କିକ ret = recurse_func (ଏହି, sub_lookup_index);
    nesting_level_ବାମ++;
    ଫେରନ୍ତୁ ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_ନିବେଶ, iter_context;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  ଅଚଳ GDEF &gdef;
  ଅଚଳ GDEF::accelerator_t &gdef_accel;
  ଅଚଳ ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index = (unsigned) -1;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_props = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting_level_ବାମ = HB_MAX_NESTING_LEVEL;

  ତାର୍କିକ has_glyph_classes;
  ତାର୍କିକ auto_zwnj = ସତ୍ୟ;
  ତାର୍କିକ auto_zwj = ସତ୍ୟ;
  ତାର୍କିକ per_syllable = ମିଥ୍ୟା;
  ତାର୍କିକ random = ମିଥ୍ୟା;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t ଗଦା_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  ଶୂନ୍ୟ *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index_,
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
#ଅନ୍ୟଥା
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#ଅନ୍ୟଥା
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (ଗଦା_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    ଯଦି (buffer->messaging ())
    {
      ପ୍ରତିଜ୍ଞା (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = ଏହି;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    ଯଦି (buffer->messaging ())
    {
      ପ୍ରତିଜ୍ଞା (buffer->changed_func == buffer_changed_trampoline);
      ପ୍ରତିଜ୍ଞା (buffer->changed_data == ଏହି);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  ଶୂନ୍ୟ init_iters ()
  {
    iter_ନିବେଶ.init (ଏହି, ମିଥ୍ୟା);
    iter_context.init (ଏହି, ସତ୍ୟ);
  }

  ଶୂନ୍ୟ set_lookup_mask (hb_mask_t mask, ତାର୍କିକ init = ସତ୍ୟ) { lookup_mask = mask; last_base = -1; last_base_until = 0; ଯଦି (init) init_iters (); }
  ଶୂନ୍ୟ set_auto_zwj (ତାର୍କିକ auto_zwj_, ତାର୍କିକ init = ସତ୍ୟ) { auto_zwj = auto_zwj_; ଯଦି (init) init_iters (); }
  ଶୂନ୍ୟ set_auto_zwnj (ତାର୍କିକ auto_zwnj_, ତାର୍କିକ init = ସତ୍ୟ) { auto_zwnj = auto_zwnj_; ଯଦି (init) init_iters (); }
  ଶୂନ୍ୟ set_per_syllable (ତାର୍କିକ per_syllable_, ତାର୍କିକ init = ସତ୍ୟ) { per_syllable = per_syllable_; ଯଦି (init) init_iters (); }
  ଶୂନ୍ୟ set_random (ତାର୍କିକ random_) { random = random_; }
  ଶୂନ୍ୟ set_recurse_func (recurse_func_t func) { recurse_func = func; }
  ଶୂନ୍ୟ set_lookup_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index_) { lookup_index = lookup_index_; }
  ଶୂନ୍ୟ set_lookup_props (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_ସଂଖ୍ୟା ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    ଫେରନ୍ତୁ buffer->random_state;
  }

  ସ୍ଥିର ଶୂନ୍ୟ buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, ଶୂନ୍ୟ *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  ଶୂନ୍ୟ buffer_changed ()
  {
    buffer->update_digest ();
    ଯଦି (likely (has_glyph_classes))
      ପାଇଁ (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  ତାର୍କିକ match_properties_mark (ଅଚଳ hb_glyph_info_t *info,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    glyph_props,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    match_props) ଅଚଳ
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    ଯଦି (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      ଫେରନ୍ତୁ gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    ଯଦି (match_props & LookupFlag::MarkAttachmentType)
      ଫେରନ୍ତୁ (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ check_glyph_property (ଅଚଳ hb_glyph_info_t *info,
			     unsigned match_props) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    ଯଦି (glyph_props & match_props & LookupFlag::IgnoreFlags)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      ଫେରନ୍ତୁ match_properties_mark (info, glyph_props, match_props);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_guess = 0,
			  ତାର୍କିକ ligature = ମିଥ୍ୟା,
			  ତାର୍କିକ component = ମିଥ୍ୟା)
  {
    buffer->digest.add (glyph_index);

    ଯଦି (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    ଯଦି (ligature)
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
    ଯଦି (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    ଯଦି (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    ଅନ୍ୟଥା ଯଦି (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    ଅନ୍ୟଥା
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  ଶୂନ୍ୟ _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  ଶୂନ୍ୟ _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  ଶୂନ୍ୟ replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (ଶୂନ୍ୟ) buffer->replace_glyph (glyph_index);
  }
  ଶୂନ୍ୟ replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  ଶୂନ୍ୟ replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, ସତ୍ୟ);
    (ଶୂନ୍ୟ) buffer->replace_glyph (glyph_index);
  }
  ଶୂନ୍ୟ output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, ମିଥ୍ୟା, ସତ୍ୟ);
    (ଶୂନ୍ୟ) buffer->output_glyph (glyph_index);
  }
};

ଗଣନା ଶ୍ରେଣୀ hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

ସଂରଚନା hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto collect_second_glyphs_ (ଅଚଳ T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (ଶୂନ୍ୟ, obj.collect_second_glyphs (digest) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ collect_second_glyphs_ (ଅଚଳ T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_ (ଅଚଳ T *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache, hb_priority<1>) HB_RETURN (ତାର୍କିକ, obj->apply (c, external_cache) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_ (ଅଚଳ T *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache, hb_priority<0>) HB_RETURN (ତାର୍କିକ, obj->apply (c) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ apply_to (ଅଚଳ ଶୂନ୍ୟ *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache)
  {
    ଅଚଳ T *typed_obj = (ଅଚଳ T *) obj;
    ଫେରନ୍ତୁ apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_cached_ (ଅଚଳ T *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache, hb_priority<2>) HB_RETURN (ତାର୍କିକ, obj->apply_cached (c, external_cache) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_cached_ (ଅଚଳ T *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache, hb_priority<1>) HB_RETURN (ତାର୍କିକ, obj->apply (c, external_cache) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto apply_cached_ (ଅଚଳ T *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache, hb_priority<0>) HB_RETURN (ତାର୍କିକ, obj->apply (c) )
  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ apply_cached_to (ଅଚଳ ଶୂନ୍ୟ *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache)
  {
    ଅଚଳ T *typed_obj = (ଅଚଳ T *) obj;
    ଫେରନ୍ତୁ apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (ତାର୍କିକ, T::cache_func (c, op) )
  template <typename T=ଶୂନ୍ୟ>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  template <typename Type>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    ଫେରନ୍ତୁ cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*hb_apply_func_t) (ଅଚଳ ଶୂନ୍ୟ *obj, hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache);
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  ସଂରଚନା hb_applicable_t
  {
    friend ସଂରଚନା hb_accelerate_subtables_context_t;
    friend ସଂରଚନା hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    ଶୂନ୍ୟ init (ଅଚଳ T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , ଶୂନ୍ୟ *external_cache_
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
    ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    ତାର୍କିକ apply_no_digest (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ apply_func (obj, c, nullptr);
    }
#ଅନ୍ୟଥା
    ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    ତାର୍କିକ apply_cached (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    ତାର୍କିକ apply_no_digest (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ apply_func (obj, c, external_cache);
    }
    ତାର୍କିକ apply_cached_no_digest (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ apply_cached_func (obj, c, external_cache);
    }

    ତାର୍କିକ cache_enter (hb_ot_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    ଶୂନ୍ୟ cache_leave (hb_ot_apply_context_t *c) ଅଚଳ
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ ଶୂନ୍ୟ *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    ଶୂନ୍ୟ *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (ଅଚଳ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (ଅଚଳ T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (ଅଚଳ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (ଅଚଳ T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ଶୂନ୍ୟ *external_cache = nullptr;
    ଯଦି (i < 8)
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
    ଯଦି (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    ଫେରନ୍ତୁ hb_empty_t ();
  }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

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


ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*intersects_func_t) (ଅଚଳ hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data, ଶୂନ୍ୟ *cache);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*intersected_glyphs_func_t) (ଅଚଳ hb_set_t *glyphs, ଅଚଳ ଶୂନ୍ୟ *data, unsigned value, hb_set_t *intersected_glyphs, ଶୂନ୍ୟ *cache);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*match_func_t) (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data);
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned (*get_ମୂଲ୍ୟ_func_t) (hb_glyph_info_t &info, ଅଚଳ ଶୂନ୍ୟ *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
ସଂରଚନା hb_ot_layout_ruleset_digest_t
{
  ଶୂନ୍ୟ init () { mask = 0; }
  ଶୂନ୍ୟ set_full () { mask = (uint64_t) -1; }
  ଶୂନ୍ୟ add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  ତାର୍କିକ may_have (unsigned value) ଅଚଳ { ଫେରନ୍ତୁ mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

ସଂରଚନା ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
ସଂରଚନା ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
ସଂରଚନା ContextApplyFuncs
{
  match_func_t match;
};
ସଂରଚନା ChainContextApplyFuncs
{
  match_func_t match[3];
};


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ intersects_glyph (ଅଚଳ hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data HB_UNUSED, ଶୂନ୍ୟ *cache HB_UNUSED)
{
  ଫେରନ୍ତୁ glyphs->has (value);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ intersects_class (ଅଚଳ hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data, ଶୂନ୍ୟ *cache)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  ଯଦି (map->has (value, &cached_v))
    ଫେରନ୍ତୁ *cached_v;

  ତାର୍କିକ v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  ଫେରନ୍ତୁ v;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ intersects_coverage (ଅଚଳ hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data, ଶୂନ୍ୟ *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ଫେରନ୍ତୁ (data+coverage).intersects (glyphs);
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ intersected_glyph (ଅଚଳ hb_set_t *glyphs HB_UNUSED, ଅଚଳ ଶୂନ୍ୟ *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED ଶୂନ୍ୟ *cache)
{
  unsigned g = reinterpret_cast<ଅଚଳ HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ intersected_class_glyphs (ଅଚଳ hb_set_t *glyphs, ଅଚଳ ଶୂନ୍ୟ *data, unsigned value, hb_set_t *intersected_glyphs, ଶୂନ୍ୟ *cache)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  ଯଦି (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    ଫେରନ୍ତୁ;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ intersected_coverage_glyphs (ଅଚଳ hb_set_t *glyphs, ଅଚଳ ଶୂନ୍ୟ *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED ଶୂନ୍ୟ *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ array_is_subset_of (ଅଚଳ hb_set_t *glyphs,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       ଅଚଳ ଶୂନ୍ୟ *intersects_data,
				       ଶୂନ୍ୟ *cache)
{
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    ଯଦି (!intersects_func (glyphs, values[i], intersects_data, cache)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ collect_glyph (hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data HB_UNUSED)
{
  glyphs->add (value);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ collect_class (hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ collect_coverage (hb_set_t *glyphs, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

ଧାଡ଼ିଭିତରେ ଅଚଳ hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ଅଚଳ ଶୂନ୍ୟ *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_ଚାବି_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  ଯଦି (glyph_set_to_index.has (key, &existing))
    ଫେରନ୍ତୁ glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  ଯଦି (unlikely (!set))
  {
    depend_data->fail ();
    ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  ଅନ୍ୟଥା ଯଦି (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  ଅନ୍ୟଥା
    set->add (value);
  ଯଦି (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    ଫେରନ୍ତୁ nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  ଯଦି (unlikely (!glyph_sets.ଠେଲ_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    ଫେରନ୍ତୁ nullptr;
  }
  ଯଦି (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    ଫେରନ୍ତୁ nullptr;
  }
  ଫେରନ୍ତୁ glyph_sets[index].get ();
}

ଧାଡ଼ିଭିତରେ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ଅଚଳ hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  ଯଦି (cached_context_set_indices.has (key, &existing))
    ଫେରନ୍ତୁ *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  ଯଦି (unlikely (index == HB_CODEPOINT_INVALID))
    ଫେରନ୍ତୁ HB_CODEPOINT_INVALID;
  ଯଦି (unlikely (!cached_context_set_indices.set (key, index)))
    ଫେରନ୍ତୁ depend_data->fail_invalid ();
  ଫେରନ୍ତୁ index;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  ଅଚଳ ଶୂନ୍ୟ *collect_data)
{
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    collect_func (glyphs, values[i], collect_data);
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, ଅଚଳ ଶୂନ୍ୟ *data HB_UNUSED)
{
  ଫେରନ୍ତୁ ସତ୍ୟ;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_glyph (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data HB_UNUSED)
{
  ଫେରନ୍ତୁ info.codepoint == value;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_class (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);
  ଫେରନ୍ତୁ class_def.get_class (info.codepoint) == value;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_ମୂଲ୍ୟ (hb_glyph_info_t &info, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *> (data);
  ଫେରନ୍ତୁ class_def.get_class (info.codepoint);
}
ସଂରଚନା match_class_cache_data_t
{
  ଅଚଳ ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_class_cache (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ auto &cache_data = *reinterpret_cast<ଅଚଳ match_class_cache_data_t *> (data);
  ଫେରନ୍ତୁ cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_cache_ମୂଲ୍ୟ (hb_glyph_info_t &info, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ auto &cache_data = *reinterpret_cast<ଅଚଳ match_class_cache_data_t *> (data);
  ଫେରନ୍ତୁ cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_cached (ଅଚଳ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  ଯଦି (klass < 255)
    ଫେରନ୍ତୁ klass;
  klass = class_def.get_class (info.codepoint);
  ଯଦି (likely (klass < 255))
    info.syllable() = klass;
  ଫେରନ୍ତୁ klass;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_class_cached (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);
  ଫେରନ୍ତୁ get_class_cached (class_def, info) == value;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_cached_ମୂଲ୍ୟ (hb_glyph_info_t &info, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *> (data);
  ଫେରନ୍ତୁ get_class_cached (class_def, info);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_cached1 (ଅଚଳ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  ଯଦି (klass < 15)
    ଫେରନ୍ତୁ klass;
  klass = class_def.get_class (info.codepoint);
  ଯଦି (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  ଫେରନ୍ତୁ klass;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_class_cached1 (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);
  ଫେରନ୍ତୁ get_class_cached1 (class_def, info) == value;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_cached2 (ଅଚଳ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  ଯଦି (klass < 15)
    ଫେରନ୍ତୁ klass;
  klass = class_def.get_class (info.codepoint);
  ଯଦି (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  ଫେରନ୍ତୁ klass;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_class_cached2 (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *>(data);
  ଫେରନ୍ତୁ get_class_cached2 (class_def, info) == value;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_class_cached2_ମୂଲ୍ୟ (hb_glyph_info_t &info, ଅଚଳ ଶୂନ୍ୟ *data)
{
  ଅଚଳ ClassDef &class_def = *reinterpret_cast<ଅଚଳ ClassDef *> (data);
  ଫେରନ୍ତୁ get_class_cached2 (class_def, info);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ match_coverage (hb_glyph_info_t &info, unsigned value, ଅଚଳ ଶୂନ୍ୟ *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ଫେରନ୍ତୁ (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ would_match_ନିବେଶ (hb_would_apply_context_t *c,
				      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, /* Including the first glyph (not matched) */
				      glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ଅଚଳ ଶୂନ୍ୟ *match_data)
{
  ଯଦି (ଗଣନା_2 != c->len)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    ଯଦି (likely (!match_func (info, ନିବେଶ[i - 1], match_data)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#ଅନ୍ୟଥା
ଧାଡ଼ିଭିତରେ
#endif
ସ୍ଥିର ତାର୍କିକ match_ନିବେଶ (hb_ot_apply_context_t *c,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, /* Including the first glyph (not matched) */
			 glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ଅଚଳ ଶୂନ୍ୟ *match_data,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *end_position,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *p_total_component_ଗଣନା = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  ଯଦି (ଗଣନା_2 == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    ଯଦି (p_total_component_ଗଣନା)
      *p_total_component_ଗଣନା = hb_glyph_info_get_lig_ଅଙ୍କ_comps (&buffer->cur());
    return_trace (ସତ୍ୟ);
  }

  ଯଦି (unlikely (ଗଣନା_2 > HB_MAX_CONTEXT_LENGTH)) return_trace (ମିଥ୍ୟା);

  auto &skippy_iter = c->iter_ନିବେଶ;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (ନିବେଶ);

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

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_component_ଗଣନା = 0;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  ଗଣନା {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
  {
    unsigned unsafe_to;
    ଯଦି (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (ମିଥ୍ୟା);
    }

    ଯଦି (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (ମିଥ୍ୟା);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    ଯଦି (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      ଯଦି (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	ଯଦି (ligbase == LIGBASE_NOT_CHECKED)
	{
	  ତାର୍କିକ found = ମିଥ୍ୟା;
	  ଅଚଳ auto *out = buffer->out_info;
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = buffer->out_len;
	  ଯେତେବେଳେ (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    ଯଦି (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = ସତ୍ୟ;
	      ଭାଙ୍ଗ;
	    }
	    j--;
	  }

	  ଯଦି (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  ଅନ୍ୟଥା
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	ଯଦି (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (ମିଥ୍ୟା);
      }
    }
    ଅନ୍ୟଥା
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      ଯଦି (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (ମିଥ୍ୟା);
    }

    total_component_ଗଣନା += hb_glyph_info_get_lig_ଅଙ୍କ_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  ଯଦି (p_total_component_ଗଣନା)
  {
    total_component_ଗଣନା += hb_glyph_info_get_lig_ଅଙ୍କ_comps (&buffer->cur());
    *p_total_component_ଗଣନା = total_component_ଗଣନା;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (ସତ୍ୟ);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ligate_ନିବେଶ (hb_ot_apply_context_t *c,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, /* Including the first glyph */
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_component_ଗଣନା)
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

  ତାର୍କିକ is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  ତାର୍କିକ is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
    ଯଦି (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = ମିଥ୍ୟା;
      is_mark_ligature = ମିଥ୍ୟା;
      ଭାଙ୍ଗ;
    }
  ତାର୍କିକ is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_ଅଙ୍କ_components = hb_glyph_info_get_lig_ଅଙ୍କ_comps (&buffer->cur());
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା components_so_far = last_ଅଙ୍କ_components;

  ଯଦି (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_ଗଣନା);
    ଯଦି (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
  {
    ଯେତେବେଳେ (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      ଯଦି (is_ligature)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	ଯଦି (this_comp == 0)
	  this_comp = last_ଅଙ୍କ_components;
	ପ୍ରତିଜ୍ଞା (components_so_far >= last_ଅଙ୍କ_components);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_lig_comp = components_so_far - last_ଅଙ୍କ_components +
				    hb_min (this_comp, last_ଅଙ୍କ_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (ଶୂନ୍ୟ) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_ଅଙ୍କ_components = hb_glyph_info_get_lig_ଅଙ୍କ_comps_in_ligation (&buffer->cur());
    components_so_far += last_ଅଙ୍କ_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  ଯଦି (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    ପାଇଁ (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      ଯଦି (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) ଭାଙ୍ଗ;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      ଯଦି (!this_comp) ଭାଙ୍ଗ;

      ପ୍ରତିଜ୍ଞା (components_so_far >= last_ଅଙ୍କ_components);
      unsigned new_lig_comp = components_so_far - last_ଅଙ୍କ_components +
			      hb_min (this_comp, last_ଅଙ୍କ_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (ସତ୍ୟ);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#ଅନ୍ୟଥା
ଧାଡ଼ିଭିତରେ
#endif
ସ୍ଥିର ତାର୍କିକ match_backtrack (hb_ot_apply_context_t *c,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     ଅଚଳ ଶୂନ୍ୟ *match_data,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *match_start)
{
  TRACE_APPLY (nullptr);

  ଯଦି (!ଗଣନା_2)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (ସତ୍ୟ);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
  {
    unsigned unsafe_from;
    ଯଦି (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (ମିଥ୍ୟା);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (ସତ୍ୟ);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#ଅନ୍ୟଥା
ଧାଡ଼ିଭିତରେ
#endif
ସ୍ଥିର ତାର୍କିକ match_lookahead (hb_ot_apply_context_t *c,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     ଅଚଳ ଶୂନ୍ୟ *match_data,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *end_index)
{
  TRACE_APPLY (nullptr);

  ଯଦି (!ଗଣନା_2)
  {
    *end_index = start_index;
    return_trace (ସତ୍ୟ);
  }

  auto &skippy_iter = c->iter_context;
  ପ୍ରତିଜ୍ଞା (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
  {
    unsigned unsafe_to;
    ଯଦି (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (ମିଥ୍ୟା);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (ସତ୍ୟ);
}



ସଂରଚନା LookupRecord
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ hb_map_t         *lookup_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ଅଚଳ hb_array_t<ଅଚଳ LookupRecord> lookupRecords,
						     ଅଚଳ hb_map_t *lookup_map)
{
  unsigned ଗଣନା_2 = 0;
  ପାଇଁ (ଅଚଳ LookupRecord& r : lookupRecords)
  {
    ଯଦି (!lookup_map->has (r.lookupListIndex))
      ଜାରି;

    ଯଦି (!r.serialize (c, lookup_map))
      ଫେରନ୍ତୁ 0;

    ଗଣନା_2++;
  }
  ଫେରନ୍ତୁ ଗଣନା_2;
}

ଗଣନା ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ଅଚଳ ଶୂନ୍ୟ *data,
			unsigned value,
			ତାର୍କିକ filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  ବାଛ (context_format)
  {
    ସ୍ଥିତି ContextFormat::SimpleContext:
      ଫେରନ୍ତୁ c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    ସ୍ଥିତି ContextFormat::ClassBasedContext:
    {
      ଅଚଳ hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      ଯଦି (unlikely (!class_set))
	ଫେରନ୍ତୁ nullptr;
      ଯଦି (!filter_class_by_parent)
	ଫେରନ୍ତୁ class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      ଭାଙ୍ଗ;
    }
    ସ୍ଥିତି ContextFormat::CoverageBasedContext:
      ଫେରନ୍ତୁ c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  ଯଦି (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    ଫେରନ୍ତୁ nullptr;
  }
  ଫେରନ୍ତୁ scratch;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned ନିବେଶ_ଗଣନା, glyph_data_t ନିବେଶ,
					     unsigned lookup_ଗଣନା_2,
					     ଅଚଳ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     ଅଚଳ ଶୂନ୍ୟ *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ଶୂନ୍ୟ *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < lookup_ଗଣନା_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    ଯଦି (seqIndex >= ନିବେଶ_ଗଣନା) ଜାରି;

    ତାର୍କିକ has_pos_glyphs = ମିଥ୍ୟା;

    ଯଦି (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = ସତ୍ୟ;
      pos_glyphs.clear ();
      ଯଦି (seqIndex == 0)
      {
        ବାଛ (context_format) {
        ସ୍ଥିତି ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          ଭାଙ୍ଗ;
        }
      }
      ଅନ୍ୟଥା
      {
        ଅଚଳ ଶୂନ୍ୟ *ନିବେଶ_data = ନିବେଶ.data;
        unsigned ନିବେଶ_ମୂଲ୍ୟ = seqIndex - 1;
        ଯଦି (context_format != ContextFormat::SimpleContext)
        {
          ନିବେଶ_data = data;
          ନିବେଶ_ମୂଲ୍ୟ = ନିବେଶ[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, ନିବେଶ_data, ନିବେଶ_ମୂଲ୍ୟ, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs))
      ଫେରନ୍ତୁ;
    ଯଦି (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } ଅନ୍ୟଥା {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = ନିବେଶ_ଗଣନା;
    ଯଦି (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->କାଢ଼_cur_done_glyphs ();
  }
}

ସ୍ଥିର ତାର୍କିକ
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned ନିବେଶ_ଗଣନା_2,
				unsigned lookup_ଗଣନା,
				ଅଚଳ LookupRecord lookup_records[],
				ଅଚଳ ଶୂନ୍ୟ *rule,
				unsigned value,
				ContextFormat context_format,
				ଅଚଳ ଶୂନ୍ୟ *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  ତାର୍କିକ have_active_idx = ମିଥ୍ୟା;
  ପାଇଁ (unsigned i = 0; i < lookup_ଗଣନା; i++)
  {
    ଯଦି (lookup_records[i].sequenceIndex >= ନିବେଶ_ଗଣନା_2)
      ଜାରି;

    ଯଦି (!have_active_idx)
    {
      ଯଦି (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      have_active_idx = ସତ୍ୟ;
    }

    hb_depend_context_t::context_set_cache_ଚାବି_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    ଯଦି (!c->context_set_cache.has (key, &cached_context_set))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (!cached_context_sets->ଠେଲ_or_fail (*cached_context_set)))
      ଫେରନ୍ତୁ c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  ଫେରନ୍ତୁ have_active_idx;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned ନିବେଶ_ଗଣନା, glyph_data_t ନିବେଶ,
					     unsigned lookup_ଗଣନା_2,
					     ଅଚଳ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ଅଚଳ ଶୂନ୍ୟ *rule,
					     unsigned value,
					     ContextFormat context_format,
					     ଅଚଳ ଶୂନ୍ୟ *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ଶୂନ୍ୟ *cache,
					     ଅଚଳ hb_set_t &preliminary_context,
					     ଅଚଳ hb_vector_t<ଅଚଳ hb_set_t *> *ନିବେଶ_position_glyphs,
					     ଅଚଳ hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  ଯଦି (unlikely (ନିବେଶ_ଗଣନା > HB_MAX_CONTEXT_LENGTH))
    ଫେରନ୍ତୁ;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < lookup_ଗଣନା_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    ଯଦି (seqIndex >= ନିବେଶ_ଗଣନା)
      ଜାରି;

    hb_depend_context_t::context_set_cache_ଚାବି_t context_cache_ଚାବି;
    ଯଦି (unlikely (!c->get_context_set_cache_ଚାବି (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_ଚାବି)))
      ଫେରନ୍ତୁ;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    ଯଦି (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    ଅନ୍ୟଥା ଯଦି (c->context_set_cache.has (context_cache_ଚାବି, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (!ନିବେଶ_position_glyphs))
      {
	c->depend_data->fail ();
	ଫେରନ୍ତୁ;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_ମୂଲ୍ୟ_INVALID;
      ଯେତେବେଳେ (preliminary_context.next (&elem)) {
	ଯଦି (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} ଅନ୍ୟଥା {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      ପାଇଁ (unsigned j = 0; j < ନିବେଶ_position_glyphs->length; j++) {
	ଯଦି (j != seqIndex && (*ନିବେଶ_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*ନିବେଶ_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_ମୂଲ୍ୟ_INVALID;
      ଯେତେବେଳେ (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ଅଚଳ hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	ଯଦି (unlikely (!original_set)) ଜାରି;

	/* Dependency context sets are never inverted. */
	ଯଦି (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      ପାଇଁ (unsigned j = 0; j < ନିବେଶ_position_glyphs->length; j++) {
	ଯଦି (j != seqIndex && (*ନିବେଶ_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  ଯଦି (!(*ନିବେଶ_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    ତାର୍କିକ cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*ନିବେଶ_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*ନିବେଶ_position_glyphs)[j]);
	    ଯଦି (unlikely (idx == HB_CODEPOINT_INVALID))
	      ଫେରନ୍ତୁ;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      ଯଦି (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	ଫେରନ୍ତୁ;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      ଯଦି (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	ଫେରନ୍ତୁ;

      ଯଦି (unlikely (!c->context_set_cache.set (context_cache_ଚାବି, context_set_idx)))
      {
	c->depend_data->fail ();
	ଫେରନ୍ତୁ;
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
    ତାର୍କିକ nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    ଯଦି (ନିବେଶ_ଗଣନା > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    ଯଦି (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    ତାର୍କିକ has_pos_glyphs = ମିଥ୍ୟା;

    ଯଦି (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = ସତ୍ୟ;
      pos_glyphs.clear ();
      ଯଦି (seqIndex == 0)
      {
        ବାଛ (context_format) {
        ସ୍ଥିତି ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          ଭାଙ୍ଗ;
        }
      }
      ଅନ୍ୟଥା
      {
        ଅଚଳ ଶୂନ୍ୟ *ନିବେଶ_data = ନିବେଶ.data;
        unsigned ନିବେଶ_ମୂଲ୍ୟ = seqIndex - 1;
        ଯଦି (context_format != ContextFormat::SimpleContext)
        {
          ନିବେଶ_data = data;
          ନିବେଶ_ମୂଲ୍ୟ = ନିବେଶ[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, ନିବେଶ_data, ନିବେଶ_ମୂଲ୍ୟ, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    ଯଦି (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      ଯଦି (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	ଯଦି (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ଠେଲ_cur_active_glyphs (active_idx);
      ଯଦି (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	ଫେରନ୍ତୁ;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    ଅନ୍ୟଥା ଯଦି (unlikely (!c->ଠେଲ_ମୂଳ_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      ଫେରନ୍ତୁ;
    }

    unsigned endIndex = ନିବେଶ_ଗଣନା;
    ଯଦି (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->କାଢ଼_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ recurse_lookups (context_t *c,
                                    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
                                    ଅଚଳ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < lookup_ଗଣନା_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ apply_lookup (hb_ot_apply_context_t *c,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, /* Including the first glyph */
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
				 ଅଚଳ LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_end)
{
  hb_buffer_t *buffer = c->buffer;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ଗଣନା_2; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < lookup_ଗଣନା_2 && buffer->successful; i++)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = lookupRecord[i].sequenceIndex;
    ଯଦି (idx >= ଗଣନା_2)
      ଜାରି;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    ଯଦି (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      ଜାରି;

    ଯଦି (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      ଭାଙ୍ଗ;

    ଯଦି (unlikely (buffer->max_ops <= 0))
      ଭାଙ୍ଗ;

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      ଯଦି (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    ତାର୍କିକ ret = c->recurse (lookupRecord[i].lookupListIndex);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      ଯଦି (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    ଯଦି (!ret)
      ଜାରି;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = new_len - orig_len;

    ଯଦି (!delta)
      ଜାରି;

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
    ଯଦି (end < ପୂର୍ଣ୍ଣସଂଖ୍ୟା (c->match_positions.arrayZ[idx]))
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

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = idx + 1; /* next now is the position after the recursed lookup. */

    ଯଦି (delta > 0)
    {
      ଯଦି (unlikely (delta + ଗଣନା_2 > HB_MAX_CONTEXT_LENGTH))
	ଭାଙ୍ଗ;
      ଯଦି (unlikely (ଗଣନା_2 + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (ଗଣନା_2 + delta)))
        ଫେରନ୍ତୁ;
    }
    ଅନ୍ୟଥା
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) next - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଗଣନା_2);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (ଗଣନା_2 - next) * ଆକାର (c->match_positions.arrayZ[0]));
    next += delta;
    ଗଣନା_2 += delta;

    /* Fill in new entries. */
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    ପାଇଁ (; next < ଗଣନା_2; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  ପ୍ରତିଜ୍ଞା (end >= 0);
  (ଶୂନ୍ୟ) buffer->move_to (end);
}



/* Contextual lookups */

ସଂରଚନା ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ଅଚଳ ଶୂନ୍ୟ *intersects_data;
  ଶୂନ୍ୟ *intersects_cache;
  ଶୂନ୍ୟ *intersected_glyphs_cache;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ContextClosureLookupContext ContextDependLookupContext;

ସଂରଚନା ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ଅଚଳ ଶୂନ୍ୟ *collect_data;
};

ସଂରଚନା ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ଅଚଳ ଶୂନ୍ୟ *match_data;
  get_ମୂଲ୍ୟ_func_t get_ମୂଲ୍ୟ;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ context_intersects (ଅଚଳ hb_set_t *glyphs,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
				       glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  ଫେରନ୍ତୁ array_is_subset_of (glyphs,
			     ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0, ନିବେଶ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ context_closure_lookup (hb_closure_context_t *c,
					   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
					   glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
					   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
					   ଅଚଳ LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  ଯଦି (context_intersects (c->glyphs,
			  ନିବେଶ_ଗଣନା, ନିବେଶ,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     ନିବେଶ_ଗଣନା, ନିବେଶ,
				     lookup_ଗଣନା_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ context_depend_lookup (hb_depend_context_t *c,
					  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
					  glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
					  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
					  ଅଚଳ LookupRecord lookupRecord[],
					  ଅଚଳ ଶୂନ୍ୟ *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  ଯଦି (!context_intersects (c->glyphs,
			   ନିବେଶ_ଗଣନା, ନିବେଶ,
			   lookup_context))
    ଫେରନ୍ତୁ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ଯଦି (unlikely (!scratch))
    ଫେରନ୍ତୁ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  ତାର୍କିକ context_sets_cached = context_depend_sets_are_cached (
    c, ନିବେଶ_ଗଣନା, lookup_ଗଣନା_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  ଯଦି (unlikely (!c->depend_data->successful))
    ଫେରନ୍ତୁ;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ଅଚଳ hb_set_t *> &ନିବେଶ_position_glyphs = scratch->ନିବେଶ_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  ଯଦି (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ଅଚଳ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      ସତ୍ୟ, &position_scratch, &scratch->compact_workspace);
    ଯଦି (unlikely (!pos0_glyphs ||
		  !ନିବେଶ_position_glyphs.ଠେଲ_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      ଫେରନ୍ତୁ;
    }

    /* Positions 1+ (Input array) */
    ପାଇଁ (unsigned i = 0; i + 1 < ନିବେଶ_ଗଣନା; i++)
    {
      ଅଚଳ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	ନିବେଶ[i], ମିଥ୍ୟା, &position_scratch);
      ଯଦି (unlikely (!pos_glyphs ||
		    !ନିବେଶ_position_glyphs.ଠେଲ_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	ଫେରନ୍ତୁ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ନିବେଶ_ଗଣନା, ନିବେଶ,
				  lookup_ଗଣନା_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &ନିବେଶ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
						  glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
						  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
						  ଅଚଳ LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->ନିବେଶ,
		 ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0, ନିବେଶ,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_ଗଣନା_2, lookupRecord);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
					       glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
					       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2 HB_UNUSED,
					       ଅଚଳ LookupRecord lookupRecord[] HB_UNUSED,
					       ଅଚଳ ContextApplyLookupContext &lookup_context)
{
  ଫେରନ୍ତୁ would_match_ନିବେଶ (c,
			    ନିବେଶ_ଗଣନା, ନିବେଶ,
			    lookup_context.funcs.match, lookup_context.match_data);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
					 glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
					 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
					 ଅଚଳ LookupRecord lookupRecord[],
					 ଅଚଳ ContextApplyLookupContext &lookup_context)
{
  ଯଦି (unlikely (ନିବେଶ_ଗଣନା > HB_MAX_CONTEXT_LENGTH)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned match_end = 0;
  ତାର୍କିକ ret = ମିଥ୍ୟା;
  ଯଦି (match_ନିବେଶ (c,
		   ନିବେଶ_ଗଣନା, ନିବେଶ,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  ନିବେଶ_ଗଣନା,
		  lookup_ଗଣନା_2, lookupRecord,
		  match_end);
    ret = ସତ୍ୟ;
  }
  ଅନ୍ୟଥା
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = ମିଥ୍ୟା;
  }

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  ବାଛ (op)
  {
    ସ୍ଥିତି hb_ot_subtable_cache_op_t::ENTER:
    {
      ଯଦି (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      auto &info = c->buffer->info;
      unsigned ଗଣନା_2 = c->buffer->len;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ସ୍ଥିତି hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      ଭାଙ୍ଗ;
    }
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

template <typename Types>
ସଂରଚନା Rule
{
  template <typename SetTypes, typename RuleType>
  friend ସଂରଚନା RuleSetOf;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ context_intersects (glyphs,
			       ନିବେଶ_ଗଣନା, ନିବେଶ_Z.arrayZ,
			       lookup_context);
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array ((ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0)));
    context_depend_lookup (c,
			   ନିବେଶ_ଗଣନା, ନିବେଶ_Z.arrayZ,
			   lookup_ଗଣନା_2, lookupRecord.arrayZ,
			   ଏହି,
			   value, lookup_context);
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;

    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array ((ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0)));
    context_closure_lookup (c,
			    ନିବେଶ_ଗଣନା, ନିବେଶ_Z.arrayZ,
			    lookup_ଗଣନା_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;
    ଯଦି (!intersects (c->glyphs, lookup_context)) ଫେରନ୍ତୁ;

    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array (ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0));
    recurse_lookups (c, lookup_ଗଣନା_2, lookupRecord.arrayZ);
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array (ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0));
    context_collect_glyphs_lookup (c,
				   ନିବେଶ_ଗଣନା, ନିବେଶ_Z.arrayZ,
				   lookup_ଗଣନା_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest,
			      ଅଚଳ ClassDef *class_def = nullptr) ଅଚଳ
  {
    ଯଦି (ନିବେଶ_ଗଣନା <= 1)
    {
      *digest = hb_set_digest_t::full ();
      ଫେରନ୍ତୁ;
    }

    unsigned second = ନିବେଶ_Z.arrayZ[0];
    ଯଦି (!class_def)
      digest->add (second);
    ଅନ୍ୟଥା ଯଦି (!second)
      *digest = hb_set_digest_t::full ();
    ଅନ୍ୟଥା
      class_def->collect_class (digest, second);
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c,
		    ଅଚଳ ContextApplyLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array (ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0));
    ଫେରନ୍ତୁ context_would_apply_lookup (c,
				       ନିବେଶ_ଗଣନା, ନିବେଶ_Z.arrayZ,
				       lookup_ଗଣନା_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c,
	      ଅଚଳ ContextApplyLookupContext &lookup_context) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array (ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0));
    return_trace (context_apply_lookup (c, ନିବେଶ_ଗଣନା, ନିବେଶ_Z.arrayZ, lookup_ଗଣନା_2, lookupRecord.arrayZ, lookup_context));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ hb_map_t *ନିବେଶ_mapping, /* old->new glyphid or class mapping */
		  ଅଚଳ hb_map_t *lookup_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->start_embed (ଏହି);
    ଯଦି (unlikely (!c->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->ନିବେଶ_ଗଣନା = ନିବେଶ_ଗଣନା;
    ଅଚଳ auto ନିବେଶ = ନିବେଶ_Z.as_array (ନିବେଶ_ଗଣନା - 1);
    ପାଇଁ (ଅଚଳ auto org : ନିବେଶ)
    {
      typename Types::HBUINT d;
      d = ନିବେଶ_mapping->get (org);
      c->copy (d);
    }

    ଅଚଳ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ନିବେଶ_Z.as_array ((ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0)));

    unsigned ଗଣନା_2 = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_ଗଣନା_2), lookup_map);
    return_trace (c->check_assign (out->lookup_ଗଣନା_2, ଗଣନା_2, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       ଅଚଳ hb_map_t *lookup_map,
	       ଅଚଳ hb_map_t *klass_map = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (unlikely (!ନିବେଶ_ଗଣନା)) return_trace (ମିଥ୍ୟା);
    ଅଚଳ auto ନିବେଶ = ନିବେଶ_Z.as_array (ନିବେଶ_ଗଣନା - 1);

    ଅଚଳ hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    ଯଦି (!hb_all (ନିବେଶ, mapping)) return_trace (ମିଥ୍ୟା);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  c->check_range (ନିବେଶ_Z.arrayZ,
				  ନିବେଶ_Z.item_size * (ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0) +
				  LookupRecord::static_size * lookup_ଗଣନା_2));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	ନିବେଶ_ଗଣନା;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_ଗଣନା_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		ନିବେଶ_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, ନିବେଶ_Z);
};

template <typename Types, typename RuleType>
ସଂରଚନା RuleSetOf
{
  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_map ([&] (ଅଚଳ RuleType &_) { ଫେରନ୍ତୁ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ଶୂନ୍ୟ collect_first_ନିବେଶ_classes (hb_ot_layout_ruleset_digest_t *digest) ଅଚଳ
  {
    digest->init ();
    ପାଇଁ (unsigned i = 0; i < rule.len; i++)
    {
      ଅଚଳ RuleType &r = ଏହି+rule.arrayZ[i];
      ଯଦି (r.ନିବେଶ_ଗଣନା <= 1)
      {
	digest->set_full ();
	ଫେରନ୍ତୁ;
      }
      digest->add (r.ନିବେଶ_Z.arrayZ[0]);
    }
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ଅଚଳ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;

    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest,
			      ଅଚଳ ClassDef *class_def = nullptr) ଅଚଳ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([=] (ଅଚଳ RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c,
		    ଅଚଳ ContextApplyLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_map ([&] (ଅଚଳ RuleType &_) { ଫେରନ୍ତୁ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c,
	      ଅଚଳ ContextApplyLookupContext &lookup_context,
	      ଅଚଳ hb_ot_layout_ruleset_digest_t *digest = nullptr) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    unsigned ଅଙ୍କ_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    ଯଦି (HB_OPTIMIZE_SIZE_VAL || ଅଙ୍କ_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (ଏହି))
      | hb_map ([&] (ଅଚଳ RuleType &_) { ଫେରନ୍ତୁ _.apply (c, lookup_context); })
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
    ତାର୍କିକ matched = skippy_iter.next (&unsafe_to1);
    ଯଦି (likely (matched))
    {
      ଯଦି (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ଯାଅ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ଯଦି (digest && lookup_context.get_ମୂଲ୍ୟ &&
	  !digest->may_have (lookup_context.get_ମୂଲ୍ୟ (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (ମିଥ୍ୟା);
	}
    }
    ଅନ୍ୟଥା
    {
      ତାର୍କିକ unsafe_to_concat = ମିଥ୍ୟା;
      ତାର୍କିକ ret =
      + hb_iter (rule)
      | hb_map (hb_add (ଏହି))
      | hb_filter ([&] (ଅଚଳ RuleType &_)
		   {
		     ଯଦି (_.ନିବେଶ_ଗଣନା <= 1) ଫେରନ୍ତୁ ସତ୍ୟ;
		     unsafe_to_concat = ସତ୍ୟ;
		     ଫେରନ୍ତୁ ମିଥ୍ୟା;
		   })
      | hb_map ([&] (ଅଚଳ RuleType &_) { ଫେରନ୍ତୁ _.apply (c, lookup_context); })
      | hb_any
      ;
      ଯଦି (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ଯଦି (likely (matched))
    {
      ଯଦି (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ଯାଅ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_ନିବେଶ = lookup_context.funcs.match;
    auto *ନିବେଶ_data = lookup_context.match_data;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_rules; i++)
    {
      ଅଚଳ auto &r = ଏହି+rule.arrayZ[i];

      ଅଚଳ auto &ନିବେଶ = r.ନିବେଶ_Z;

      ଯଦି (r.ନିବେଶ_ଗଣନା <= 1 ||
	  (!match_ନିବେଶ ||
	   match_ନିବେଶ (*first, ନିବେଶ.arrayZ[0], ନିବେଶ_data)))
      {
        ଯଦି (!second ||
	    (r.ନିବେଶ_ଗଣନା <= 2 ||
	     (!match_ନିବେଶ ||
	      match_ନିବେଶ (*second, ନିବେଶ.arrayZ[1], ନିବେଶ_data)))
	   )
	{
	  ଯଦି (r.apply (c, lookup_context))
	  {
	    ଯଦି (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (ସତ୍ୟ);
	  }
	}
	ଅନ୍ୟଥା
	  unsafe_to = unsafe_to2;
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	ପାଇଁ (; i + 1 < ଅଙ୍କ_rules; i++)
	{
	  ଅଚଳ auto &r2 = ଏହି+rule.arrayZ[i + 1];
	  ଅଚଳ auto &input2 = r2.ନିବେଶ_Z;
	  ଯଦି (r2.ନିବେଶ_ଗଣନା <= 1 || input2.arrayZ[0] != ନିବେଶ.arrayZ[0])
	    ଭାଙ୍ଗ;
	}
      }
    }
    ଯଦି (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ମିଥ୍ୟା);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       ଅଚଳ hb_map_t *lookup_map,
	       ଅଚଳ hb_map_t *klass_map = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (ଅଚଳ auto& _ : rule)
    {
      ଯଦି (!_) ଜାରି;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      ଯଦି (unlikely (!o)) ଜାରି;

      ଯଦି (!o->serialize_subset (c, _, ଏହି, lookup_map, klass_map))
      {
	out->rule.କାଢ଼ ();
	c->serializer->revert (o_snap);
      }
    }

    ତାର୍କିକ ret = ତାର୍କିକ (out->rule);
    ଯଦି (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (rule.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ସଂରଚନା RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
ସଂରଚନା ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
ସଂରଚନା ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    ଫେରନ୍ତୁ
    + hb_zip (ଏହି+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_map ([&] (ଅଚଳ RuleSet &_) { ଫେରନ୍ତୁ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (ଏହି+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ଫେରନ୍ତୁ c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ଅଚଳ hb_pair_t<hb_codepoint_t, unsigned> _) { ଫେରନ୍ତୁ hb_pair_t<unsigned, ଅଚଳ RuleSet&> (_.first, ଏହି+ruleSet[_.second]); })
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (ଏହି+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ଫେରନ୍ତୁ c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ଅଚଳ hb_pair_t<hb_codepoint_t, unsigned> _) { ଫେରନ୍ତୁ hb_pair_t<unsigned, ଅଚଳ RuleSet&> (_.first, ଏହି+ruleSet[_.second]); })
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ
  {
    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (ଏହି+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    (ଏହି+coverage).collect_coverage (c->ନିବେଶ);

    ସଂରଚନା ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([digest] (ଅଚଳ RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଅଚଳ RuleSet &rule_set = ଏହି+ruleSet[(ଏହି+coverage).get_coverage (c->glyphs[0])];
    ସଂରଚନା ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    ଫେରନ୍ତୁ rule_set.would_apply (c, lookup_context);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur().codepoint);
    ଯଦି (likely (index == NOT_COVERED))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ RuleSet &rule_set = ଏହି+ruleSet[index];
    ସଂରଚନା ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    ଅଚଳ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, ଏହି, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ତାର୍କିକ (new_coverage));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && ruleSet.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ସଂରଚନା ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (glyphs))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ ClassDef &class_def = ଏହି+classDef;

    hb_map_t cache;
    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (ଏହି+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    ଫେରନ୍ତୁ
    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_enumerate
    | hb_map ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ RuleSet &> p)
	      { ଫେରନ୍ତୁ class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ଅଚଳ ClassDef &class_def = ଏହି+classDef;

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
    { ଫେରନ୍ତୁ class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ଅଚଳ RuleSet& rule_set = ଏହି+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ଅଚଳ ClassDef &class_def = ଏହି+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ଫେରନ୍ତୁ class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ଅଚଳ RuleSet& rule_set = ଏହି+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    ଅଚଳ ClassDef &class_def = ଏହି+classDef;

    hb_map_t cache;
    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_enumerate
    | hb_filter ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ RuleSet &> p)
    { ଫେରନ୍ତୁ class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ଅଚଳ RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    (ଏହି+coverage).collect_coverage (c->ନିବେଶ);

    ଅଚଳ ClassDef &class_def = ଏହି+classDef;
    ସଂରଚନା ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    ଅଚଳ ClassDef &class_def = ଏହି+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([=, &class_def] (ଅଚଳ RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଅଚଳ ClassDef &class_def = ଏହି+classDef;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = class_def.get_class (c->glyphs[0]);
    ଅଚଳ RuleSet &rule_set = ଏହି+ruleSet[index];
    ସଂରଚନା ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    ଫେରନ୍ତୁ rule_set.would_apply (c, lookup_context);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  unsigned cache_cost () ଅଚଳ
  {
    ଫେରନ୍ତୁ (ଏହି+classDef).cost ();
  }
  ସ୍ଥିର ତାର୍କିକ cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ଫେରନ୍ତୁ context_cache_func (c, op);
  }

  ସଂରଚନା external_cache_t
  {
    hb_ot_layout_ଦ୍ୱିମିକ_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  ଶୂନ୍ୟ *external_cache_create () ଅଚଳ
  {
    unsigned ଗଣନା_2 = ruleSet.len;
    unsigned size = ଆକାର (external_cache_t) -
		    HB_VAR_ARRAY * ଆକାର (hb_ot_layout_ruleset_digest_t) +
		    ଗଣନା_2 * ଆକାର (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ଯଦି (likely (cache))
    {
      cache->coverage.clear ();
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	(ଏହି+ruleSet[i]).collect_first_ନିବେଶ_classes (&cache->rule_sets[i]);
    }
    ଫେରନ୍ତୁ cache;
  }
  ତାର୍କିକ apply_cached (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ { ଫେରନ୍ତୁ _apply (c, ସତ୍ୟ, external_cache); }
  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ { ଫେରନ୍ତୁ _apply (c, ମିଥ୍ୟା, external_cache); }
  ତାର୍କିକ _apply (hb_ot_apply_context_t *c, ତାର୍କିକ cached, ଶୂନ୍ୟ *external_cache) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage_ଦ୍ୱିମିକ (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ଅନ୍ୟଥା
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଅଚଳ ClassDef &class_def = ଏହି+classDef;

    ସଂରଚନା ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_ମୂଲ୍ୟ : get_class_ମୂଲ୍ୟ
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    ଅଚଳ RuleSet &rule_set = ଏହି+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ଅଚଳ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#ଅନ୍ୟଥା
    ଅଚଳ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;
    ଯଦି (unlikely (!out->coverage.serialize_subset (c, coverage, ଏହି)))
      return_trace (ମିଥ୍ୟା);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, ଏହି, &klass_map);

    ଅଚଳ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (ଏହି+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (ଏହି+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ଅଚଳ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    ତାର୍କିକ ret = ସତ୍ୟ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      ଯଦି (unlikely (!o))
      {
	ret = ମିଥ୍ୟା;
	ଭାଙ୍ଗ;
      }

      ଯଦି (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, ଏହି, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    ଯଦି (!ret || non_zero_index == -1) return_trace (ମିଥ୍ୟା);

    //prune empty trailing ruleSets
    --index;
    ଯେତେବେଳେ (index > non_zero_index)
    {
      out->ruleSet.କାଢ଼ ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (ତାର୍କିକ (out->ruleSet));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && classDef.sanitize (c, ଏହି) && ruleSet.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ସଂରଚନା ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverageZ[0]).intersects (glyphs))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      ଏହି
    };
    ଫେରନ୍ତୁ context_intersects (glyphs,
			       glyph_ଗଣନା_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverageZ[0]).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ଏହି,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_ଗଣନା_2, (ଅଚଳ HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_ଗଣନା_2, lookupRecord,
			   ଏହି,
			   coverageZ[0], lookup_context);

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverageZ[0]).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    ସଂରଚନା ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ଏହି
    };
    context_closure_lookup (c,
			    glyph_ଗଣନା_2, coverageZ.arrayZ + 1,
			    lookup_ଗଣନା_2, lookupRecord,
			    0, lookup_context);

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ
  {
    ଯଦି (!intersects (c->glyphs))
      ଫେରନ୍ତୁ;
    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    recurse_lookups (c, lookup_ଗଣନା_2, lookupRecord);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    (ଏହି+coverageZ[0]).collect_coverage (c->ନିବେଶ);

    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    ସଂରଚନା ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      ଏହି
    };

    context_collect_glyphs_lookup (c,
				   glyph_ଗଣନା_2, coverageZ.arrayZ + 1,
				   lookup_ଗଣନା_2, lookupRecord,
				   lookup_context);
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    ଯଦି (glyph_ଗଣନା_2 <= 1)
      *digest = hb_set_digest_t::full ();
    ଅନ୍ୟଥା
      (ଏହି+coverageZ[1]).collect_coverage (digest);
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    ସଂରଚନା ContextApplyLookupContext lookup_context = {
      {match_coverage},
      ଏହି
    };
    ଫେରନ୍ତୁ context_would_apply_lookup (c,
				       glyph_ଗଣନା_2, coverageZ.arrayZ + 1,
				       lookup_ଗଣନା_2, lookupRecord,
				       lookup_context);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverageZ[0]; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    ସଂରଚନା ContextApplyLookupContext lookup_context = {
      {match_coverage},
      ଏହି
    };
    return_trace (context_apply_lookup (c,
					glyph_ଗଣନା_2,
					coverageZ.arrayZ + 1,
					lookup_ଗଣନା_2, lookupRecord,
					lookup_context));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->format = format;
    out->glyph_ଗଣନା_2 = glyph_ଗଣନା_2;

    auto coverages = coverageZ.as_array (glyph_ଗଣନା_2);

    ପାଇଁ (ଅଚଳ auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      ଯଦି (unlikely (!o)) return_trace (ମିଥ୍ୟା);
      ଯଦି (!o->serialize_subset (c, offset, ଏହି)) return_trace (ମିଥ୍ୟା);
    }

    ଅଚଳ auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_ଗଣନା_2));
    ଅଚଳ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned ଗଣନା_2 = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_ଗଣନା_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_ଗଣନା_2, ଗଣନା_2, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = glyph_ଗଣନା_2;
    ଯଦି (unlikely (!ଗଣନା_2)) return_trace (ମିଥ୍ୟା); /* We want to access coverageZ[0] freely. */
    ଯଦି (unlikely (!c->check_array (coverageZ.arrayZ, ଗଣନା_2))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!coverageZ[i].sanitize (c, ଏହି))) return_trace (ମିଥ୍ୟା);
    ଅଚଳ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ଗଣନା_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_ଗଣନା_2)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_ଗଣନା_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_ଗଣନା_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

ସଂରଚନା Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
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

ସଂରଚନା ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ଅଚଳ ଶୂନ୍ୟ *intersects_data[3];
  ଶୂନ୍ୟ *intersects_cache[3];
  ଶୂନ୍ୟ *intersected_glyphs_cache;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ChainContextClosureLookupContext ChainContextDependLookupContext;

ସଂରଚନା ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ଅଚଳ ଶୂନ୍ୟ *collect_data[3];
};

ସଂରଚନା ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ଅଚଳ ଶୂନ୍ୟ *match_data[3];
  get_ମୂଲ୍ୟ_func_t get_ନିବେଶ_ମୂଲ୍ୟ;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ chain_context_intersects (ଅଚଳ hb_set_t *glyphs,
					     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_ଗଣନା,
					     glyph_data_t backtrack,
					     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
					     glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
					     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_ଗଣନା,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  ତାର୍କିକ bt_ok = array_is_subset_of (glyphs,
			     backtrack_ଗଣନା, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  ତାର୍କିକ in_ok = array_is_subset_of (glyphs,
			     ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0, ନିବେଶ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  ତାର୍କିକ la_ok = array_is_subset_of (glyphs,
			     lookahead_ଗଣନା, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  ଫେରନ୍ତୁ bt_ok && in_ok && la_ok;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_ଗଣନା,
						 glyph_data_t backtrack,
						 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
						 glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
						 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_ଗଣନା,
						 glyph_data_t lookahead,
						 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
						 ଅଚଳ LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  ଯଦି (chain_context_intersects (c->glyphs,
				backtrack_ଗଣନା, backtrack,
				ନିବେଶ_ଗଣନା, ନିବେଶ,
				lookahead_ଗଣନା, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     ନିବେଶ_ଗଣନା, ନିବେଶ,
		     lookup_ଗଣନା_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_ଗଣନା,
						glyph_data_t backtrack,
						unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
						glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
						unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_ଗଣନା,
						glyph_data_t lookahead,
						unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
						ଅଚଳ LookupRecord lookupRecord[],
						ଅଚଳ ଶୂନ୍ୟ *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  ଯଦି (!chain_context_intersects (c->glyphs,
				 backtrack_ଗଣନା, backtrack,
				 ନିବେଶ_ଗଣନା, ନିବେଶ,
				 lookahead_ଗଣନା, lookahead,
				 lookup_context))
    ଫେରନ୍ତୁ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ଯଦି (unlikely (!scratch))
    ଫେରନ୍ତୁ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  ତାର୍କିକ context_sets_cached = context_depend_sets_are_cached (
    c, ନିବେଶ_ଗଣନା, lookup_ଗଣନା_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  ଯଦି (unlikely (!c->depend_data->successful))
    ଫେରନ୍ତୁ;
  ଯଦି (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    ନିବେଶ_ଗଣନା, ନିବେଶ,
				    lookup_ଗଣନା_2, lookupRecord,
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
    ଫେରନ୍ତୁ;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ଅଚଳ hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ଅଚଳ hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  ପାଇଁ (unsigned i = 0; i < backtrack_ଗଣନା; i++)
  {
    ଅଚଳ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], ମିଥ୍ୟା, &position_scratch);
    ଯଦି (unlikely (!pos_glyphs))
      ଫେରନ୍ତୁ;
    ଯଦି (!pos_glyphs->is_empty ())
    {
      ଯଦି (unlikely (!backtrack_sets.ଠେଲ_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        ଫେରନ୍ତୁ;
      }
    }
  }

  /* Extract lookahead glyphs */
  ପାଇଁ (unsigned i = 0; i < lookahead_ଗଣନା; i++)
  {
    ଅଚଳ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], ମିଥ୍ୟା, &position_scratch);
    ଯଦି (unlikely (!pos_glyphs))
      ଫେରନ୍ତୁ;
    ଯଦି (!pos_glyphs->is_empty ())
    {
      ଯଦି (unlikely (!lookahead_sets.ଠେଲ_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        ଫେରନ୍ତୁ;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  ପାଇଁ (ଅଚଳ hb_set_t *back_set : backtrack_sets)
  {
    ଯଦି (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } ଅନ୍ୟଥା ଯଦି (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      ଯଦି (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ଫେରନ୍ତୁ;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  ପାଇଁ (ଅଚଳ hb_set_t *look_set : lookahead_sets)
  {
    ଯଦି (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } ଅନ୍ୟଥା ଯଦି (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      ଯଦି (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ଫେରନ୍ତୁ;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  ଯଦି (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    ଫେରନ୍ତୁ;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ଅଚଳ hb_set_t *> &ନିବେଶ_position_glyphs = scratch->ନିବେଶ_position_glyphs;

  ଯଦି (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ଅଚଳ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      ସତ୍ୟ, &position_scratch, &scratch->compact_workspace);
    ଯଦି (unlikely (!pos0_glyphs ||
		  !ନିବେଶ_position_glyphs.ଠେଲ_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      ଫେରନ୍ତୁ;
    }

    /* Positions 1+ (Input array) */
    ପାଇଁ (unsigned i = 0; i + 1 < ନିବେଶ_ଗଣନା; i++)
    {
      ଅଚଳ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	ନିବେଶ[i], ମିଥ୍ୟା, &position_scratch);
      ଯଦି (unlikely (!pos_glyphs ||
		    !ନିବେଶ_position_glyphs.ଠେଲ_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	ଫେରନ୍ତୁ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ନିବେଶ_ଗଣନା, ନିବେଶ,
				  lookup_ଗଣନା_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &ନିବେଶ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_ଗଣନା,
							glyph_data_t backtrack,
							unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
							glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
							unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_ଗଣନା,
							glyph_data_t lookahead,
							unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
							ଅଚଳ LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_ଗଣନା, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->ନିବେଶ,
		 ନିବେଶ_ଗଣନା ? ନିବେଶ_ଗଣନା - 1 : 0, ନିବେଶ,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_ଗଣନା, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_ଗଣନା_2, lookupRecord);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_ଗଣନା,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
						     glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
						     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_ଗଣନା,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2 HB_UNUSED,
						     ଅଚଳ LookupRecord lookupRecord[] HB_UNUSED,
						     ଅଚଳ ChainContextApplyLookupContext &lookup_context)
{
  ଫେରନ୍ତୁ (c->zero_context ? !backtrack_ଗଣନା && !lookahead_ଗଣନା : ସତ୍ୟ)
      && would_match_ନିବେଶ (c,
			    ନିବେଶ_ଗଣନା, ନିବେଶ,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backtrack_ଗଣନା,
					       glyph_data_t backtrack,
					       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_ଗଣନା, /* Including the first glyph (not matched) */
					       glyph_data_t ନିବେଶ, /* Array of input values--start with second glyph */
					       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookahead_ଗଣନା,
					       glyph_data_t lookahead,
					       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା_2,
					       ଅଚଳ LookupRecord lookupRecord[],
					       ଅଚଳ ChainContextApplyLookupContext &lookup_context)
{
  ଯଦି (unlikely (ନିବେଶ_ଗଣନା > HB_MAX_CONTEXT_LENGTH)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  ତାର୍କିକ ret = ସତ୍ୟ;
  ଯଦି (!(match_ନିବେଶ (c,
		     ନିବେଶ_ଗଣନା, ନିବେଶ,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_ଗଣନା, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଯଦି (!match_backtrack (c,
			backtrack_ଗଣନା, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		ନିବେଶ_ଗଣନା,
		lookup_ଗଣନା_2, lookupRecord,
		match_end);

  ଫେରନ୍ତୁ ret;
}

template <typename Types>
ସଂରଚନା ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend ସଂରଚନା ChainRuleSetOf;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଫେରନ୍ତୁ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ନିବେଶ.lenP1, ନିବେଶ.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 ନିବେଶ.lenP1, ନିବେଶ.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ଏହି,
				 value,
				 lookup_context);
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;

    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ନିବେଶ.lenP1, ନିବେଶ.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;
    ଯଦି (!intersects (c->glyphs, lookup_context)) ଫେରନ୍ତୁ;

    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ନିବେଶ.lenP1, ନିବେଶ.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest,
			      ଅଚଳ ClassDef *ନିବେଶ_class_def = nullptr,
			      ଅଚଳ ClassDef *lookahead_class_def = nullptr) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);

    unsigned second;
    ଅଚଳ ClassDef *class_def;
    ଯଦି (ନିବେଶ.lenP1 > 1)
    {
      second = ନିବେଶ.arrayZ[0];
      class_def = ନିବେଶ_class_def;
    }
    ଅନ୍ୟଥା ଯଦି (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    ଅନ୍ୟଥା
    {
      *digest = hb_set_digest_t::full ();
      ଫେରନ୍ତୁ;
    }

    ଯଦି (!class_def)
      digest->add (second);
    ଅନ୍ୟଥା ଯଦି (!second)
      *digest = hb_set_digest_t::full ();
    ଅନ୍ୟଥା
      class_def->collect_class (digest, second);
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c,
		    ଅଚଳ ChainContextApplyLookupContext &lookup_context) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ଫେରନ୍ତୁ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ନିବେଶ.lenP1, ନିବେଶ.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c,
	      ଅଚଳ ChainContextApplyLookupContext &lookup_context) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ନିବେଶ.lenP1, ନିବେଶ.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ଅଚଳ
  {
    c->copy (len);
    ପାଇଁ (ଅଚଳ auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ hb_map_t *lookup_map,
		  ଅଚଳ hb_map_t *backtrack_map,
		  ଅଚଳ hb_map_t *ନିବେଶ_map = nullptr,
		  ଅଚଳ hb_map_t *lookahead_map = nullptr) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ଅଚଳ hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଯଦି (ନିବେଶ_map) mapping = ନିବେଶ_map;
    serialize_array (c, ନିବେଶ.lenP1, + ନିବେଶ.iter ()
				     | hb_map (mapping));

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଯଦି (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_ଗଣନା_2 = c->embed (&(lookup.len));
    ଯଦି (!lookup_ଗଣନା_2) return_trace (ମିଥ୍ୟା);

    unsigned ଗଣନା_2 = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_ଗଣନା_2, ଗଣନା_2, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       ଅଚଳ hb_map_t *lookup_map,
	       ଅଚଳ hb_map_t *backtrack_map = nullptr,
	       ଅଚଳ hb_map_t *ନିବେଶ_map = nullptr,
	       ଅଚଳ hb_map_t *lookahead_map = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);

    ଯଦି (!backtrack_map)
    {
      ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      ଯଦି (!hb_all (backtrack, glyphset) ||
	  !hb_all (ନିବେଶ, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (ମିଥ୍ୟା);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (ନିବେଶ, ନିବେଶ_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (ମିଥ୍ୟା);

      serialize (c->serializer, lookup_map, backtrack_map, ନିବେଶ_map, lookahead_map);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* Hyper-optimized sanitized because this is really hot. */
    ଯଦି (unlikely (!backtrack.len.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଯଦି (unlikely (!ନିବେଶ.lenP1.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଯଦି (unlikely (!lookahead.len.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  ସୁରକ୍ଷିତ:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		ନିବେଶ_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
ସଂରଚନା ChainRuleSetOf
{
  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_map ([&] (ଅଚଳ ChainRuleType &_) { ଫେରନ୍ତୁ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ଶୂନ୍ୟ collect_first_ନିବେଶ_classes (hb_ot_layout_ruleset_digest_t *digest) ଅଚଳ
  {
    digest->init ();
    ପାଇଁ (unsigned i = 0; i < rule.len; i++)
    {
      ଅଚଳ ChainRuleType &r = ଏହି+rule.arrayZ[i];
      ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (r.ନିବେଶ_X)> (r.backtrack);
      ଯଦି (ନିବେଶ.lenP1 <= 1)
      {
	digest->set_full ();
	ଫେରନ୍ତୁ;
      }
      digest->add (ନିବେଶ.arrayZ[0]);
    }
  }
  ଶୂନ୍ୟ depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ଅଚଳ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  ଶୂନ୍ୟ closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;

    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ଅଚଳ
  {
    ଯଦି (unlikely (c->lookup_limit_exceeded ())) ଫେରନ୍ତୁ;

    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest,
			      ଅଚଳ ClassDef *ନିବେଶ_class_def = nullptr,
			      ଅଚଳ ClassDef *lookahead_class_def = nullptr) ଅଚଳ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([=] (ଅଚଳ ChainRuleType &_)
		{ _.collect_second_glyphs (digest, ନିବେଶ_class_def, lookahead_class_def); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c,
		    ଅଚଳ ChainContextApplyLookupContext &lookup_context) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (rule)
    | hb_map (hb_add (ଏହି))
    | hb_map ([&] (ଅଚଳ ChainRuleType &_) { ଫେରନ୍ତୁ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c,
	      ଅଚଳ ChainContextApplyLookupContext &lookup_context,
	      ଅଚଳ hb_ot_layout_ruleset_digest_t *digest = nullptr) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    unsigned ଅଙ୍କ_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    ଯଦି (HB_OPTIMIZE_SIZE_VAL || ଅଙ୍କ_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (ଏହି))
      | hb_map ([&] (ଅଚଳ ChainRuleType &_) { ଫେରନ୍ତୁ _.apply (c, lookup_context); })
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
    ତାର୍କିକ matched = skippy_iter.next (&unsafe_to1);
    ଯଦି (likely (matched))
    {
      ଯଦି (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ଯାଅ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ଯଦି (digest && lookup_context.get_ନିବେଶ_ମୂଲ୍ୟ &&
	  !digest->may_have (lookup_context.get_ନିବେଶ_ମୂଲ୍ୟ (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (ମିଥ୍ୟା);
	}
    }
    ଅନ୍ୟଥା
    {
      ତାର୍କିକ unsafe_to_concat = ମିଥ୍ୟା;
      ତାର୍କିକ ret =
      + hb_iter (rule)
      | hb_map (hb_add (ଏହି))
      | hb_filter ([&] (ଅଚଳ ChainRuleType &_)
		   {
		     ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (_.ନିବେଶ_X)> (_.backtrack);
		     ଅଚଳ auto &lookahead = StructAfter<decltype (_.lookaheadX)> (ନିବେଶ);
		     ଯଦି (ନିବେଶ.lenP1 <= 1 && lookahead.len == 0) ଫେରନ୍ତୁ ସତ୍ୟ;
		     unsafe_to_concat = ସତ୍ୟ;
		     ଫେରନ୍ତୁ ମିଥ୍ୟା;
		   })
      | hb_map ([&] (ଅଚଳ ChainRuleType &_) { ଫେରନ୍ତୁ _.apply (c, lookup_context); })
      | hb_any
      ;
      ଯଦି (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ଯଦି (likely (matched))
    {
      ଯଦି (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ଯାଅ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_ନିବେଶ = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *ନିବେଶ_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_rules; i++)
    {
      ଅଚଳ auto &r = ଏହି+rule.arrayZ[i];

      ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (r.ନିବେଶ_X)> (r.backtrack);
      ଅଚଳ auto &lookahead = StructAfter<decltype (r.lookaheadX)> (ନିବେଶ);

      unsigned lenP1 = ନିବେଶ.lenP1;
      ଯଦି (lenP1 > 1 ?
	   (!match_ନିବେଶ ||
	    match_ନିବେଶ (*first, ନିବେଶ.arrayZ[0], ନିବେଶ_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        ଯଦି (!second ||
	    (lenP1 > 2 ?
	     (!match_ନିବେଶ ||
	      match_ନିବେଶ (*second, ନିବେଶ.arrayZ[1], ନିବେଶ_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  ଯଦି (r.apply (c, lookup_context))
	  {
	    ଯଦି (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (ସତ୍ୟ);
	  }
	}
	ଅନ୍ୟଥା
	  unsafe_to = unsafe_to2;
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	ଯଦି (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  ପାଇଁ (; i + 1 < ଅଙ୍କ_rules; i++)
	  {
	    ଅଚଳ auto &r2 = ଏହି+rule.arrayZ[i + 1];
	    ଅଚଳ auto &input2 = StructAfter<decltype (r2.ନିବେଶ_X)> (r2.backtrack);
	    ଯଦି (input2.lenP1 <= 1 || input2.arrayZ[0] != ନିବେଶ.arrayZ[0])
	      ଭାଙ୍ଗ;
	  }
	}
      }
    }
    ଯଦି (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ମିଥ୍ୟା);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       ଅଚଳ hb_map_t *lookup_map,
	       ଅଚଳ hb_map_t *backtrack_klass_map = nullptr,
	       ଅଚଳ hb_map_t *ନିବେଶ_klass_map = nullptr,
	       ଅଚଳ hb_map_t *lookahead_klass_map = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (ଅଚଳ auto& _ : rule)
    {
      ଯଦି (!_) ଜାରି;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      ଯଦି (unlikely (!o)) ଜାରି;

      ଯଦି (!o->serialize_subset (c, _, ଏହି,
				lookup_map,
				backtrack_klass_map,
				ନିବେଶ_klass_map,
				lookahead_klass_map))
      {
	out->rule.କାଢ଼ ();
	c->serializer->revert (o_snap);
      }
    }

    ତାର୍କିକ ret = ତାର୍କିକ (out->rule);
    ଯଦି (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (rule.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ସଂରଚନା ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (ଆକାର (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
ସଂରଚନା ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
ସଂରଚନା ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    ଫେରନ୍ତୁ
    + hb_zip (ଏହି+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_map ([&] (ଅଚଳ ChainRuleSet &_) { ଫେରନ୍ତୁ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (ଏହି+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ଫେରନ୍ତୁ c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ଅଚଳ hb_pair_t<hb_codepoint_t, unsigned> _) { ଫେରନ୍ତୁ hb_pair_t<unsigned, ଅଚଳ ChainRuleSet&> (_.first, ଏହି+ruleSet[_.second]); })
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (ଏହି+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ଫେରନ୍ତୁ c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ଅଚଳ hb_pair_t<hb_codepoint_t, unsigned> _) { ଫେରନ୍ତୁ hb_pair_t<unsigned, ଅଚଳ ChainRuleSet&> (_.first, ଏହି+ruleSet[_.second]); })
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ
  {
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (ଏହି+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    (ଏହି+coverage).collect_coverage (c->ନିବେଶ);

    ସଂରଚନା ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([digest] (ଅଚଳ ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଅଚଳ ChainRuleSet &rule_set = ଏହି+ruleSet[(ଏହି+coverage).get_coverage (c->glyphs[0])];
    ସଂରଚନା ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    ଫେରନ୍ତୁ rule_set.would_apply (c, lookup_context);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଅଚଳ ChainRuleSet &rule_set = ଏହି+ruleSet[index];
    ସଂରଚନା ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    ଅଚଳ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, ଏହି, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ତାର୍କିକ (new_coverage));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && ruleSet.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
ସଂରଚନା ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (glyphs))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (ଏହି+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    ନିବେଶ_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ଫେରନ୍ତୁ
    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_enumerate
    | hb_map ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ ChainRuleSet &> p)
	      { ଫେରନ୍ତୁ ନିବେଶ_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ଫେରନ୍ତୁ ନିବେଶ_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ଅଚଳ ChainRuleSet& chainrule_set = ଏହି+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs)) ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ଫେରନ୍ତୁ ନିବେଶ_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ଅଚଳ ChainRuleSet& chainrule_set = ଏହି+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { ଫେରନ୍ତୁ ନିବେଶ_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ଅଚଳ ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    (ଏହି+coverage).collect_coverage (c->ନିବେଶ);

    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

    ସଂରଚନା ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([digest, &ନିବେଶ_class_def, &lookahead_class_def] (ଅଚଳ ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &ନିବେଶ_class_def, &lookahead_class_def); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = ନିବେଶ_class_def.get_class (c->glyphs[0]);
    ଅଚଳ ChainRuleSet &rule_set = ଏହି+ruleSet[index];
    ସଂରଚନା ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def}
    };
    ଫେରନ୍ତୁ rule_set.would_apply (c, lookup_context);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  unsigned cache_cost () ଅଚଳ
  {
    ଫେରନ୍ତୁ (ଏହି+ନିବେଶ_Class_Def).cost () + (ଏହି+lookaheadClassDef).cost ();
  }
  ସ୍ଥିର ତାର୍କିକ cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ଫେରନ୍ତୁ context_cache_func (c, op);
  }

  ସଂରଚନା external_cache_t
  {
    hb_ot_layout_ଦ୍ୱିମିକ_cache_t coverage;
    hb_ot_layout_mapping_cache_t ନିବେଶ_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  ଶୂନ୍ୟ *external_cache_create () ଅଚଳ
  {
    unsigned ଗଣନା_2 = ruleSet.len;
    unsigned size = ଆକାର (external_cache_t) -
		    HB_VAR_ARRAY * ଆକାର (hb_ot_layout_ruleset_digest_t) +
		    ଗଣନା_2 * ଆକାର (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ଯଦି (likely (cache))
    {
      cache->coverage.clear ();
      cache->ନିବେଶ_class.clear ();
      cache->lookahead_class.clear ();
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	(ଏହି+ruleSet[i]).collect_first_ନିବେଶ_classes (&cache->rule_sets[i]);
    }
    ଫେରନ୍ତୁ cache;
  }
  ତାର୍କିକ apply_cached (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ { ଫେରନ୍ତୁ _apply (c, ସତ୍ୟ, external_cache); }
  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ { ଫେରନ୍ତୁ _apply (c, ମିଥ୍ୟା, external_cache); }
  ତାର୍କିକ _apply (hb_ot_apply_context_t *c, ତାର୍କିକ cached, ଶୂନ୍ୟ *external_cache) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage_ଦ୍ୱିମିକ (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ଅନ୍ୟଥା
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଅଚଳ ClassDef &backtrack_class_def = ଏହି+backtrackClassDef;
    ଅଚଳ ClassDef &ନିବେଶ_class_def = ଏହି+ନିବେଶ_Class_Def;
    ଅଚଳ ClassDef &lookahead_class_def = ଏହି+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t ନିବେଶ_cache_data = {
      &ନିବେଶ_class_def,
      cache ? &cache->ନିବେଶ_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ଅଚଳ ଶୂନ୍ୟ *ନିବେଶ_match_data = cached
      ? static_cast<ଅଚଳ ଶୂନ୍ୟ *> (&ନିବେଶ_class_def)
      : static_cast<ଅଚଳ ଶୂନ୍ୟ *> (&ନିବେଶ_cache_data);
    ଅଚଳ ଶୂନ୍ୟ *lookahead_match_data = cached
      ? static_cast<ଅଚଳ ଶୂନ୍ୟ *> (&lookahead_class_def)
      : static_cast<ଅଚଳ ଶୂନ୍ୟ *> (&lookahead_cache_data);
    ତାର୍କିକ backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ଅଚଳ ଶୂନ୍ୟ *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<ଅଚଳ ଶୂନ୍ୟ *> (&lookahead_cache_data)
      : static_cast<ଅଚଳ ଶୂନ୍ୟ *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    ସଂରଚନା ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       ନିବେଶ_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_ମୂଲ୍ୟ : get_class_cache_ମୂଲ୍ୟ
#ଅନ୍ୟଥା
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &ନିବେଶ_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_ମୂଲ୍ୟ : get_class_ମୂଲ୍ୟ
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (ନିବେଶ_class_def, c->buffer->cur())
         : ନିବେଶ_class_def.get_class (c->buffer->cur().codepoint, ନିବେଶ_cache_data.cache);
#ଅନ୍ୟଥା
    index = cached
         ? get_class_cached2 (ନିବେଶ_class_def, c->buffer->cur())
          : ନିବେଶ_class_def.get_class (c->buffer->cur().codepoint);
#endif
    ଅଚଳ ChainRuleSet &rule_set = ଏହି+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ଅଚଳ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#ଅନ୍ୟଥା
    ଅଚଳ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, ଏହି);

    hb_map_t backtrack_klass_map;
    hb_map_t ନିବେଶ_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, ଏହି, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->ନିବେଶ_Class_Def.serialize_subset (c, ନିବେଶ_Class_Def, ଏହି, &ନିବେଶ_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, ଏହି, &lookahead_klass_map);

    ଯଦି (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   ନିବେଶ_klass_map,
						   lookahead_klass_map)))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (ଏହି+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (ଏହି+ନିବେଶ_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା non_zero_index = -1, index = 0;
    ତାର୍କିକ ret = ସତ୍ୟ;
    ଅଚଳ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (ନିବେଶ_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      ଯଦି (unlikely (!o))
      {
	ret = ମିଥ୍ୟା;
	ଭାଙ୍ଗ;
      }
      ଯଦି (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, ଏହି,
			       lookup_map,
			       &backtrack_klass_map,
			       &ନିବେଶ_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    ଯଦି (!ret || non_zero_index == -1) return_trace (ମିଥ୍ୟା);

    // prune empty trailing ruleSets
    ଯଦି (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (ତାର୍କିକ (out->ruleSet));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) &&
		  backtrackClassDef.sanitize (c, ଏହି) &&
		  ନିବେଶ_Class_Def.sanitize (c, ଏହି) &&
		  lookaheadClassDef.sanitize (c, ଏହି) &&
		  ruleSet.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		ନିବେଶ_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
ସଂରଚନା ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);

    ଯଦି (!(ଏହି+ନିବେଶ[0]).intersects (glyphs))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {ଏହି, ଏହି, ଏହି}
    };
    ଫେରନ୍ତୁ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ନିବେଶ.len, ନିବେଶ.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);

    ଯଦି (!(ଏହି+ନିବେଶ[0]).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs))
      ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ଏହି, ଏହି, ଏହି},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ଅଚଳ HBUINT16 *) backtrack.arrayZ,
				 ନିବେଶ.len, (ଅଚଳ HBUINT16 *) ନିବେଶ.arrayZ + 1,
				 lookahead.len, (ଅଚଳ HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ଏହି,
				 ନିବେଶ[0], lookup_context);

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);

    ଯଦି (!(ଏହି+ନିବେଶ[0]).intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    hb_set_t* cur_active_glyphs = c->ଠେଲ_cur_active_glyphs ();
    ଯଦି (unlikely (!cur_active_glyphs))
      ଫେରନ୍ତୁ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ସଂରଚନା ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ଏହି, ଏହି, ଏହି}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ନିବେଶ.len, ନିବେଶ.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->କାଢ଼_cur_done_glyphs ();
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ
  {
    ଯଦି (!intersects (c->glyphs))
      ଫେରନ୍ତୁ;

    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);

    (ଏହି+ନିବେଶ[0]).collect_coverage (c->ନିବେଶ);

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    ସଂରଚନା ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {ଏହି, ଏହି, ଏହି}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ନିବେଶ.len, ନିବେଶ.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଯଦି (ନିବେଶ.len > 1)
      (ଏହି+ନିବେଶ[1]).collect_coverage (digest);
    ଅନ୍ୟଥା ଯଦି (lookahead.len)
      (ଏହି+lookahead[0]).collect_coverage (digest);
    ଅନ୍ୟଥା
      *digest = hb_set_digest_t::full ();
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ସଂରଚନା ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {ଏହି, ଏହି, ଏହି}
    };
    ଫେରନ୍ତୁ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ନିବେଶ.len, ନିବେଶ.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ
  {
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଫେରନ୍ତୁ ଏହି+ନିବେଶ[0];
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+ନିବେଶ[0]).get_coverage (c->buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ସଂରଚନା ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {ଏହି, ଏହି, ଏହି}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ନିବେଶ.len, ନିବେଶ.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ଅଚଳ ଶୂନ୍ୟ* base) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    ଯଦି (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      ଯଦି (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    ଯଦି (unlikely (!c->serializer->embed (ଏହି->format))) return_trace (ମିଥ୍ୟା);

    ଯଦି (!serialize_coverage_offsets (c, backtrack.iter (), ଏହି))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଯଦି (!serialize_coverage_offsets (c, ନିବେଶ.iter (), ଏହି))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଯଦି (!serialize_coverage_offsets (c, lookahead.iter (), ଏହି))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ଅଚଳ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_ଗଣନା_2 = c->serializer->copy<HBUINT16> (lookup.len);
    ଯଦି (!lookup_ଗଣନା_2) return_trace (ମିଥ୍ୟା);

    unsigned ଗଣନା_2 = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_ଗଣନା_2, ଗଣନା_2, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!backtrack.sanitize (c, ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &ନିବେଶ = StructAfter<decltype (ନିବେଶ_X)> (backtrack);
    ଯଦି (unlikely (!ନିବେଶ.sanitize (c, ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଯଦି (unlikely (!ନିବେଶ.len)) return_trace (ମିଥ୍ୟା); /* To be consistent with Context. */
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (ନିବେଶ);
    ଯଦି (unlikely (!lookahead.sanitize (c, ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		ନିବେଶ_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (10);
};

ସଂରଚନା ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }		format;	/* Format identifier */
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
ସଂରଚନା ExtensionFormat1
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type () ଅଚଳ { ଫେରନ୍ତୁ extensionLookupType; }

  template <typename X>
  ଅଚଳ X& get_subtable () ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି + reinterpret_cast<ଅଚଳ Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, ଏହି))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ଅଚଳ auto& src_offset =
        reinterpret_cast<ଅଚଳ Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, ଏହି, get_type ()));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
ସଂରଚନା Extension
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_type ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }
  template <typename X>
  ଅଚଳ X& get_subtable () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.template get_subtable<typename T::SubTable> ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.subset (c);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ c->default_return_ମୂଲ୍ୟ ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

ସଂରଚନା hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  ସ୍ଥିର hb_ot_layout_lookup_accelerator_t *create (ଅଚଳ TLookup &lookup)
  {
    unsigned ଗଣନା_2 = lookup.get_subtable_ଗଣନା ();

    unsigned size = ଆକାର (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * ଆକାର (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    ଗଣନା_2 * ଆକାର (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    ଯଦି (unlikely (!thiz))
      ଫେରନ୍ତୁ nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    ଯେତେବେଳେ (ଗଣନା_2 && !thiz->subtables[ଗଣନା_2 - 1].apply_func)
      ଗଣନା_2--;

    thiz->digest.init ();
    ପାଇଁ (auto& subtable : hb_iter (thiz->subtables, ଗଣନା_2))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->ଗଣନା_2 = ଗଣନା_2;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      ଯଦି (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    ଫେରନ୍ତୁ thiz;
  }

  ଶୂନ୍ୟ fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ତାର୍କିକ use_cache) ଅଚଳ
  {
    ଯଦି (ଗଣନା_2 == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      ଯଦି (use_cache)
	ଫେରନ୍ତୁ subtables[0].apply_cached_no_digest (c);
#endif
      ଫେରନ୍ତୁ subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ଯଦି (use_cache)
    {
      ଫେରନ୍ତୁ
      + hb_iter (hb_iter (subtables, ଗଣନା_2))
      | hb_map ([&c] (ଅଚଳ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ଫେରନ୍ତୁ _.apply_cached (c); })
      | hb_any
      ;
    }
    ଅନ୍ୟଥା
#endif
    {
      ଫେରନ୍ତୁ
      + hb_iter (hb_iter (subtables, ଗଣନା_2))
      | hb_map ([&c] (ଅଚଳ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ଫେରନ୍ତୁ _.apply (c); })
      | hb_any
      ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ cache_enter (hb_ot_apply_context_t *c) ଅଚଳ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ଫେରନ୍ତୁ subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
  }
  ଶୂନ୍ୟ cache_leave (hb_ot_apply_context_t *c) ଅଚଳ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  ବ୍ୟକ୍ତିଗତ:
  unsigned ଗଣନା_2 = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ସଂରଚନା GSUBGPOS
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

  size_t get_size () ଅଚଳ
  {
    ଯଦି (version.major != 1) ଫେରନ୍ତୁ version.static_size;

    ଫେରନ୍ତୁ min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#ଅନ୍ୟଥା
	   0
#endif
	   ;
  }

  ଅଚଳ ScriptList &get_script_list () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u && scriptList2) ଫେରନ୍ତୁ ଏହି + scriptList2;
#endif
    ଫେରନ୍ତୁ ଏହି + scriptList;
  }
  ଅଚଳ FeatureList &get_feature_list () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u && featureList2) ଫେରନ୍ତୁ ଏହି + featureList2;
#endif
    ଫେରନ୍ତୁ ଏହି + featureList;
  }
  template <typename TLookup>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (ଏହି, get_size ())))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ତାର୍କିକ script_list_ok = scriptList.sanitize (c, ଏହି);
    ତାର୍କିକ feature_list_ok = featureList.sanitize (c, ଏହି);
    ତାର୍କିକ lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, ଏହି);

#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u)
    {
      ଯଦି (scriptList2) script_list_ok = scriptList2.sanitize (c, ଏହି);
      ଯଦି (featureList2) feature_list_ok = featureList2.sanitize (c, ଏହି);
      ଯଦି (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, ଏହି);
    }
#endif

    ଯଦି (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (ମିଥ୍ୟା);

#ifndef HB_NO_VAR
    ଯଦି (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, ଏହି, &get_feature_list ()))))
      return_trace (ମିଥ୍ୟା);
#endif

    return_trace (ସତ୍ୟ);
  }

  template <typename TLookup>
  ତାର୍କିକ subset (hb_subset_layout_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (version.major != 1) return_trace (ମିଥ୍ୟା);

    auto *out = c->subset_context->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u)
    {
      ଯଦି (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (ମିଥ୍ୟା);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      ଯଦି (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 ଏହି, c);
      ଅନ୍ୟଥା
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 ଏହି, c);

      ଯଦି (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 ଏହି, c);
      ଅନ୍ୟଥା
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 ଏହି, c);

      ଯଦି (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, ଏହି, c);
      ଅନ୍ୟଥା
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, ଏହି, c);
    }
    ଅନ୍ୟଥା
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     ଏହି,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     ଏହି,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					ଏହି,
					c);
    }

#ifndef HB_NO_VAR
    ଯଦି (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      ଯଦି (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (ମିଥ୍ୟା);

      // if all axes are pinned all feature vars are dropped.
      ତାର୍କିକ ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, ଏହି, c);
      ଯଦି (!ret)
      {
        out->featureVars = 0;
	ଯଦି (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (ସତ୍ୟ);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lookup_ଗଣନା () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u && lookupList2)
      ଫେରନ୍ତୁ (ଏହି + lookupList2).len;
#endif
    ଫେରନ୍ତୁ (ଏହି + lookupList).len;
  }
  ଅଚଳ Lookup& get_lookup (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u && lookupList2)
      ଫେରନ୍ତୁ (ଏହି + lookupList2)[i];
#endif
    ଫେରନ୍ତୁ (ଏହି + lookupList)[i];
  }
  ଅଚଳ FeatureVariations &get_feature_variations () ଅଚଳ
  {
    ଫେରନ୍ତୁ (version.to_int () >= 0x00010001u && hb_barrier ()) ? ଏହି+featureVars : Null (FeatureVariations);
  }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_script_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ get_script_list ().len; }
  ଅଚଳ Tag& get_script_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ get_script_list ().get_tag (i); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_script_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *script_ଗଣନା /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ get_script_list ().get_tags (start_offset, script_ଗଣନା, script_tags); }
  ଅଚଳ Script& get_script (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ get_script_list ()[i]; }
  ତାର୍କିକ find_script_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  { ଫେରନ୍ତୁ get_script_list ().find_index (tag, index); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *feature_ଗଣନା_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ get_feature_list ().get_tags (start_offset, feature_ଗଣନା_2, feature_tags); }
  ଅଚଳ Feature& get_feature (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ get_feature_list ()[i]; }
  ତାର୍କିକ find_feature_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  { ଫେରନ୍ତୁ get_feature_list ().find_index (tag, index); }

  ତାର୍କିକ find_variations_index (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coords,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index,
			      ItemVarStoreInstancer *instancer) ଅଚଳ
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
    ଫେରନ୍ତୁ get_feature_variations ().find_index (coords, ଅଙ୍କ_coords, index, instancer);
  }
  ତାର୍କିକ get_feature_variations_state (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coords,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) ଅଚଳ
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    ଫେରନ୍ତୁ ସତ୍ୟ;
#ଅନ୍ୟଥା
    find_variations_index (coords, ଅଙ୍କ_coords, index, instancer);
    ଫେରନ୍ତୁ get_feature_variations ().collect_lookup_conditions (
	coords,
	ଅଙ୍କ_coords,
	instancer,
	lookup_conditions);
#endif
  }
  ତାର୍କିକ get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<ଅଚଳ unsigned> conditions,
                              hb_set_t *lookup_indexes) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  ଅଚଳ Feature& get_feature_variation (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_index,
					unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା variations_index) ଅଚଳ
  {
#ifndef HB_NO_VAR
    ଯଦି (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      ଅଚଳ Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      ଯଦି (feature)
	ଫେରନ୍ତୁ *feature;
    }
#endif
    ଫେରନ୍ତୁ get_feature (feature_index);
  }

  ଶୂନ୍ୟ feature_variation_collect_lookups (ଅଚଳ hb_set_t *feature_indexes,
					  ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ଅଚଳ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  ଶୂନ୍ୟ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  ଶୂନ୍ୟ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ଅଚଳ
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  ଶୂନ୍ୟ closure_lookups (hb_face_t      *face,
			ଅଚଳ hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ଅଚଳ
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    ପାଇଁ (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<ଅଚଳ TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  ଶୂନ୍ୟ prune_langsys (ଅଚଳ hb_map_t *duplicate_feature_map,
                      ଅଚଳ hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ଅଚଳ
  {
    hb_prune_langsys_context_t c (ଏହି, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned ଗଣନା_2 = get_script_ଗଣନା ();
    ପାଇଁ (unsigned script_index = 0; script_index < ଗଣନା_2; script_index++)
    {
      ଅଚଳ Tag& tag = get_script_tag (script_index);
      ଯଦି (!layout_scripts->has (tag)) ଜାରି;
      ଅଚଳ Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  ଶୂନ୍ୟ prune_features (ଅଚଳ hb_map_t *lookup_indices, /* IN */
		       ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       ଅଚଳ hb_hashmap_t<unsigned, ଅଚଳ Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) ଅଚଳ
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    ଯଦି (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      ଫେରନ୍ତୁ;
    }
#endif

    ପାଇଁ (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      ଯଦି (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        ଜାରି;


      ଅଚଳ Feature *f = &(get_feature (i));
      ଅଚଳ Feature** p = nullptr;
      ଯଦି (feature_substitutes_map->has (i, &p))
        f = *p;

      ଯଦି (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        ଜାରି;

      ଯଦି (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  ଶୂନ୍ୟ collect_name_ids (ଅଚଳ hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ଅଚଳ hb_hashmap_t<unsigned, ଅଚଳ Feature*> *feature_substitutes_map,
                         ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ଅଚଳ hb_set_t *catch_all_features,
                         ତାର୍କିକ all_axes_pinned HB_UNUSED) ଅଚଳ
  {
    unsigned ଗଣନା_2 = get_feature_ଗଣନା ();
    ପାଇଁ (unsigned i = 0 ; i < ଗଣନା_2; i++)
    {
      ଯଦି (!feature_index_map->has (i)) ଜାରି;
      hb_tag_t tag = get_feature_tag (i);
      ଅଚଳ Feature *feature = &get_feature (i);
      ଅଚଳ Feature **substitute;
      ଯଦି (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        ଯଦି (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    ଯଦି (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [ଏହି] (unsigned i) { ଫେରନ୍ତୁ get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = ସତ୍ୟ;
      ଏହି->table = sc.reference_table<T> (face);

      ଯଦି (unlikely (ଏହି->table->is_blocklisted (ଏହି->table.get_blob (), face)))
      {
	hb_blob_destroy (ଏହି->table.get_blob ());
	ଏହି->table = hb_blob_get_empty ();
      }

      ଏହି->lookup_ଗଣନା = table->get_lookup_ଗଣନା ();

      ଏହି->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (ଏହି->lookup_ଗଣନା, ଆକାର (*accels));
      ଯଦି (unlikely (!ଏହି->accels))
      {
	ଏହି->lookup_ଗଣନା = 0;
	ଏହି->table.destroy ();
	ଏହି->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଏହି->lookup_ଗଣନା; i++)
      {
	auto *accel = ଏହି->accels[i].get_relaxed ();
	ଯଦି (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (ଏହି->accels);
      ଏହି->table.destroy ();
    }

    hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) ଅଚଳ
    {
      ଯଦି (unlikely (lookup_index >= lookup_ଗଣନା)) ଫେରନ୍ତୁ nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      ଯଦି (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	ଯଦି (unlikely (!accel))
	  ଫେରନ୍ତୁ nullptr;

	ଯଦି (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  ଯାଅ retry;
	}
      }

      ଫେରନ୍ତୁ accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_ଗଣନା;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  ସୁରକ୍ଷିତ:
  template <typename TLookup>
  ଅଚଳ Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ଅଚଳ Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  ଅଚଳ Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { ଫେରନ୍ତୁ reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ଅଚଳ Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ଅଚଳ Offset16To<RecordListOfFeature>& subset_feature_list_offset () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { ଫେରନ୍ତୁ reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ଅଚଳ Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ଅଚଳ ଶୂନ୍ୟ* get_lookup_list_field_offset () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (version.to_int () >= 0x00010002u && lookupList2) ଫେରନ୍ତୁ &lookupList2;
#endif
    ଫେରନ୍ତୁ &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ସ୍ଥିର ତାର୍କିକ serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ଅଚଳ OffsetIn& in,
				       ଅଚଳ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    ଯଦି (in.is_null ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    auto *s = c->serializer;
    s->push ();
    ତାର୍କିକ ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    ଯଦି (ret) s->add_link (out, s->କାଢ଼_pack ());
    ଅନ୍ୟଥା s->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ret;
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
