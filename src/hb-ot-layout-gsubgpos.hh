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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-buffer.hh"
#ಅನ್ತರ್ಭಾವ "hb-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gdef-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-depend-data.hh"


ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, ತಾರ್ಕಿಕ>
{
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { ಪ್ರತ್ಯಾಗಮ obj.intersects (ಏತತ್->glyphs); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ r; }

  ಧ್ರುವ hb_set_t *glyphs;

  hb_intersects_context_t (ಧ್ರುವ hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

ಸಂರಚನಾ hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, ತಾರ್ಕಿಕ>
{
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { ಪ್ರತ್ಯಾಗಮ obj.may_have_non_1to1 (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ r; }
};

ಸಂರಚನಾ hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { obj.closure (ಏತತ್); ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಶೂನ್ಯ recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    ಯದಿ (unlikely (nesting_level_ಎಡ == 0 || !recurse_func))
      ಪ್ರತ್ಯಾಗಮ;

    nesting_level_ಎಡ--;
    recurse_func (ಏತತ್, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_ಎಡ++;
  }

  ಶೂನ್ಯ reset_lookup_visit_ಎಣಿಕೆ ()
  { lookup_ಎಣಿಕೆ = 0; }

  ತಾರ್ಕಿಕ lookup_limit_exceeded ()
  { ಪ್ರತ್ಯಾಗಮ lookup_ಎಣಿಕೆ > HB_MAX_LOOKUP_VISIT_ಎಣಿಕೆ; }

  ತಾರ್ಕಿಕ should_visit_lookup (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index)
  {
    ಯದಿ (lookup_ಎಣಿಕೆ++ > HB_MAX_LOOKUP_VISIT_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (is_lookup_done (lookup_index))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ is_lookup_done (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index)
  {
    ಯದಿ (unlikely (done_lookups_glyph_ಎಣಿಕೆ->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned ತೆಗೆ = glyphs->get_population ();
    ಯದಿ (unlikely (ತೆಗೆ == HB_MAP_ಮೌಲ್ಯ_INVALID)) // Avoid sentinel interference.
      ತೆಗೆ--;
    ಯದಿ (done_lookups_glyph_ಎಣಿಕೆ->get (lookup_index) != ತೆಗೆ)
    {
      done_lookups_glyph_ಎಣಿಕೆ->set (lookup_index, ತೆಗೆ);

      ಯದಿ (!done_lookups_glyph_set->has (lookup_index))
      {
	ಯದಿ (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    ಯದಿ (unlikely (covered_glyph_set->in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (parent_active_glyphs ().is_subset (*covered_glyph_set))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    covered_glyph_set->union_ (parent_active_glyphs ());
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಧ್ರುವ hb_set_t& previous_parent_active_glyphs () {
    ಯದಿ (active_glyphs_ರಾಶಿ.length <= 1)
      ಪ್ರತ್ಯಾಗಮ *glyphs;

    ಪ್ರತ್ಯಾಗಮ active_glyphs_ರಾಶಿ[active_glyphs_ರಾಶಿ.length - 2];
  }

  ಧ್ರುವ hb_set_t& parent_active_glyphs ()
  {
    ಯದಿ (!active_glyphs_ರಾಶಿ)
      ಪ್ರತ್ಯಾಗಮ *glyphs;

    ಪ್ರತ್ಯಾಗಮ active_glyphs_ರಾಶಿ.tail ();
  }

  hb_set_t* ತಳ್ಳು_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_ರಾಶಿ.push ();
    ಯದಿ (unlikely (active_glyphs_ರಾಶಿ.in_error ()))
      ಪ್ರತ್ಯಾಗಮ nullptr;
    ಪ್ರತ್ಯಾಗಮ s;
  }

  ತಾರ್ಕಿಕ ತೆಗೆ_cur_done_glyphs ()
  {
    ಯದಿ (!active_glyphs_ರಾಶಿ)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    active_glyphs_ರಾಶಿ.ತೆಗೆ ();
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_ರಾಶಿ;
  recurse_func_t recurse_func = nullptr;
  unsigned ಪೂರ್ಣಾಙ್ಕ nesting_level_ಎಡ;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_ಎಣಿಕೆ_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned ಪೂರ್ಣಾಙ್ಕ nesting_level_ಎಡ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_ಎಡ (nesting_level_ಎಡ_2),
			  done_lookups_glyph_ಎಣಿಕೆ (done_lookups_glyph_ಎಣಿಕೆ_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  ಶೂನ್ಯ set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ಶೂನ್ಯ flush ()
  {
    output->del_range (face->get_ಅಂಕೆ_glyphs (), HB_SET_ಮೌಲ್ಯ_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_ರಾಶಿ.ತೆಗೆ ();
    active_glyphs_ರಾಶಿ.reset ();
  }

  ಗುಪ್ತ:
  hb_map_t *done_lookups_glyph_ಎಣಿಕೆ;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ = 0;
};



ಸಂರಚನಾ hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { obj.closure_lookups (ಏತತ್); ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಶೂನ್ಯ recurse (unsigned lookup_index)
  {
    ಯದಿ (unlikely (nesting_level_ಎಡ == 0 || !recurse_func))
      ಪ್ರತ್ಯಾಗಮ;

    /* Return if new lookup was recursed to before. */
    ಯದಿ (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      ಪ್ರತ್ಯಾಗಮ;

    nesting_level_ಎಡ--;
    recurse_func (ಏತತ್, lookup_index);
    nesting_level_ಎಡ++;
  }

  ಶೂನ್ಯ set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  ಶೂನ್ಯ set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  ತಾರ್ಕಿಕ lookup_limit_exceeded ()
  {
    ತಾರ್ಕಿಕ ret = lookup_ಎಣಿಕೆ > HB_MAX_LOOKUP_VISIT_ಎಣಿಕೆ;
    ಯದಿ (ret)
      DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    ಪ್ರತ್ಯಾಗಮ ret; }

  ತಾರ್ಕಿಕ is_lookup_visited (unsigned lookup_index)
  {
    ಯದಿ (unlikely (lookup_ಎಣಿಕೆ++ > HB_MAX_LOOKUP_VISIT_ಎಣಿಕೆ))
    {
      DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_ಎಣಿಕೆ, lookup_index);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಯದಿ (unlikely (visited_lookups->in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  ಧ್ರುವ hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned ಪೂರ್ಣಾಙ್ಕ nesting_level_ಎಡ;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ಧ್ರುವ hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_ಎಡ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_ಎಡ (nesting_level_ಎಡ_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_ಎಣಿಕೆ (0) {}

  ಶೂನ್ಯ set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ಗುಪ್ತ:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ;
};

ಸಂರಚನಾ hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, ತಾರ್ಕಿಕ>
{
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { ಪ್ರತ್ಯಾಗಮ obj.would_apply (ಏತತ್); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ r; }

  hb_face_t *face;
  ಧ್ರುವ hb_codepoint_t *glyphs;
  unsigned ಪೂರ್ಣಾಙ್ಕ len;
  ತಾರ್ಕಿಕ zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ಧ್ರುವ hb_codepoint_t *glyphs_,
			    unsigned ಪೂರ್ಣಾಙ್ಕ len_,
			    ತಾರ್ಕಿಕ zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

ಸಂರಚನಾ hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index);
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { obj.collect_glyphs (ಏತತ್); ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಶೂನ್ಯ recurse (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index)
  {
    ಯದಿ (unlikely (nesting_level_ಎಡ == 0 || !recurse_func))
      ಪ್ರತ್ಯಾಗಮ;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    ಯದಿ (output == hb_set_get_empty ())
      ಪ್ರತ್ಯಾಗಮ;

    /* Return if new lookup was recursed to before. */
    ಯದಿ (recursed_lookups->has (lookup_index))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t *old_before = before;
    hb_set_t *old_ಒಳಹರಿವು  = ಒಳಹರಿವು;
    hb_set_t *old_after  = after;
    before = ಒಳಹರಿವು = after = hb_set_get_empty ();

    nesting_level_ಎಡ--;
    recurse_func (ಏತತ್, lookup_index);
    nesting_level_ಎಡ++;

    before = old_before;
    ಒಳಹರಿವು  = old_ಒಳಹರಿವು;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *ಒಳಹರಿವು;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned ಪೂರ್ಣಾಙ್ಕ nesting_level_ಎಡ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_ಒಳಹರಿವು,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned ಪೂರ್ಣಾಙ್ಕ nesting_level_ಎಡ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      ಒಳಹರಿವು  (glyphs_ಒಳಹರಿವು  ? glyphs_ಒಳಹರಿವು  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_ಎಡ (nesting_level_ಎಡ_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  ಶೂನ್ಯ set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ಸಂರಚನಾ hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  ಗಣನಾ glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  ಸ್ಥಿರ constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  ಸಂರಚನಾ glyph_set_cache_ಕೀಲಿ_t
  {
    glyph_set_cache_ಕೀಲಿ_t () = ಸ್ವಭಾವ;
    glyph_set_cache_ಕೀಲಿ_t (ಧ್ರುವ ಶೂನ್ಯ *data_, unsigned ಮೌಲ್ಯ_3, glyph_set_type_t type_)
      : data (data_), value (ಮೌಲ್ಯ_3), type (type_) {}

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ glyph_set_cache_ಕೀಲಿ_t &o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ data == o.data && value == o.value && type == o.type; }

    uint32_t hash () ಧ್ರುವ
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      ಪ್ರತ್ಯಾಗಮ current * 31 + hb_hash ((unsigned) type);
    }

    ಧ್ರುವ ಶೂನ್ಯ *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  ಸಂರಚನಾ context_set_cache_ಕೀಲಿ_t
  {
    context_set_cache_ಕೀಲಿ_t () = ಸ್ವಭಾವ;
    context_set_cache_ಕೀಲಿ_t (ಧ್ರುವ ಶೂನ್ಯ *rule_,
			     ಧ್ರುವ ಶೂನ್ಯ *record_,
			     ಧ್ರುವ ಶೂನ್ಯ *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned ಮೌಲ್ಯ_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (ಮೌಲ್ಯ_3), format (format_) {}

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ context_set_cache_ಕೀಲಿ_t &o) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ಧ್ರುವ
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      ಪ್ರತ್ಯಾಗಮ current * 31 + hb_hash (format);
    }

    ಧ್ರುವ ಶೂನ್ಯ *rule = nullptr;
    ಧ್ರುವ ಶೂನ್ಯ *record = nullptr;
    ಧ್ರುವ ಶೂನ್ಯ *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  ಸಂರಚನಾ recurse_ಕೀಲಿ_t
  {
    recurse_ಕೀಲಿ_t () = ಸ್ವಭಾವ;
    recurse_ಕೀಲಿ_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ recurse_ಕೀಲಿ_t &o) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () ಧ್ರುವ
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      ಪ್ರತ್ಯಾಗಮ current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  ಪ್ರಕಾರನಿರ್ದೇಶ return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { obj.depend (ಏತತ್); ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  ಶೂನ್ಯ recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    ಯದಿ (lookups_seen.has (lookup_idx)) ಪ್ರತ್ಯಾಗಮ;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_ಕೀಲಿ = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    ಯದಿ (!recurse_path_to_index.has (path_ಕೀಲಿ, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      ಯದಿ (unlikely (!recurse_path_to_index.set (path_ಕೀಲಿ, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	ಪ್ರತ್ಯಾಗಮ;
      }
    }
    ಅನ್ಯಥಾ
      path_index = *path;
    recurse_path = path_index;
    recurse_func (ಏತತ್, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  ಸಂರಚನಾ active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    ತಾರ್ಕಿಕ use_ಬೇರು = ಅಸತ್ಯ;
  };

  ಧ್ರುವ hb_set_t& parent_active_glyphs ()
  {
    ಯದಿ (!active_glyphs_ರಾಶಿ_depth)
      ಪ್ರತ್ಯಾಗಮ *glyphs;

    ಧ್ರುವ active_glyphs_frame_t &frame = active_glyphs_ರಾಶಿ[active_glyphs_ರಾಶಿ_depth - 1];
    ಪ್ರತ್ಯಾಗಮ frame.use_ಬೇರು ? *glyphs : frame.glyphs;
  }

  hb_set_t* ತಳ್ಳು_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    ಯದಿ (active_glyphs_ರಾಶಿ_depth == active_glyphs_ರಾಶಿ.length &&
	unlikely (!active_glyphs_ರಾಶಿ.ತಳ್ಳು_or_fail ()))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    active_glyphs_frame_t &frame = active_glyphs_ರಾಶಿ[active_glyphs_ರಾಶಿ_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_ಬೇರು = ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ &frame.glyphs;
  }

  ತಾರ್ಕಿಕ ತಳ್ಳು_ಬೇರು_active_glyphs ()
  {
    ಯದಿ (active_glyphs_ರಾಶಿ_depth == active_glyphs_ರಾಶಿ.length &&
	unlikely (!active_glyphs_ರಾಶಿ.ತಳ್ಳು_or_fail ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    active_glyphs_ರಾಶಿ[active_glyphs_ರಾಶಿ_depth++].use_ಬೇರು = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ ತೆಗೆ_cur_done_glyphs ()
  {
    ಯದಿ (!active_glyphs_ರಾಶಿ_depth)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    active_glyphs_ರಾಶಿ_depth--;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    ಯದಿ (!active_glyphs_ರಾಶಿ_depth ||
	active_glyphs_ರಾಶಿ[active_glyphs_ರಾಶಿ_depth - 1].use_ಬೇರು)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    active_glyphs_frame_t &frame = active_glyphs_ರಾಶಿ[active_glyphs_ರಾಶಿ_depth - 1];
    *active_idx = frame.active_idx;
    ಯದಿ (*active_idx != HB_CODEPOINT_INVALID)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    hb_codepoint_t glyph;
    ಯದಿ (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    ಅನ್ಯಥಾ ಯದಿ (!find_or_create_recurse_set (frame.glyphs, active_idx))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    frame.active_idx = *active_idx;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_context_set_cache_ಕೀಲಿ (ಧ್ರುವ ಶೂನ್ಯ *rule,
				  ಧ್ರುವ ಶೂನ್ಯ *record,
				  ಧ್ರುವ ಶೂನ್ಯ *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_ಕೀಲಿ_t *key)
  {
    hb_codepoint_t active_idx;
    ಯದಿ (!get_parent_active_glyphs_index (&active_idx))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *key = context_set_cache_ಕೀಲಿ_t (rule, record, data, active_idx, value, format);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_recurse_ಕೀಲಿ (unsigned lookup_idx, recurse_ಕೀಲಿ_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    ಯದಿ (!get_parent_active_glyphs_index (&active_idx))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    *key = recurse_ಕೀಲಿ_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    ಪ್ರತ್ಯಾಗಮ !completed_recursions.has (*key);
  }

  ಶೂನ್ಯ finish_recurse (ಧ್ರುವ recurse_ಕೀಲಿ_t &key)
  { depend_data->check_success (completed_recursions.set (key, ಸತ್ಯ)); }

  ಶೂನ್ಯ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  ತಾರ್ಕಿಕ find_or_create_recurse_set (ಧ್ರುವ hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    ಯದಿ (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    ಯದಿ (unlikely (!copy))
      ಪ್ರತ್ಯಾಗಮ depend_data->fail ();
    copy->set (set);
    ಯದಿ (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      ಪ್ರತ್ಯಾಗಮ depend_data->fail ();
    }

    *index = recurse_sets.length;
    ಯದಿ (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      ಪ್ರತ್ಯಾಗಮ depend_data->fail ();
    }
    ಯದಿ (unlikely (!recurse_sets.ತಳ್ಳು_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      ಪ್ರತ್ಯಾಗಮ depend_data->fail ();
    ಯದಿ (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      ಪ್ರತ್ಯಾಗಮ depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಧ್ರುವ hb_set_t *find_or_create_glyph_set (ಧ್ರುವ ಶೂನ್ಯ *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ಧ್ರುವ hb_set_t *set);

  ಸಂರಚನಾ depend_scratch_t
  {
    ಶೂನ್ಯ clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      ಒಳಹರಿವು_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ಧ್ರುವ hb_set_t *> backtrack_sets;
    hb_vector_t<ಧ್ರುವ hb_set_t *> lookahead_sets;
    hb_vector_t<ಧ್ರುವ hb_set_t *> ಒಳಹರಿವು_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    ಯದಿ (scratch)
      depend_scratch_pool = scratch->next;
    ಅನ್ಯಥಾ
    {
      scratch = (depend_scratch_t *) hb_malloc (ಪರಿಮಾಣ (depend_scratch_t));
      ಯದಿ (unlikely (!scratch))
      {
        depend_data->fail ();
        ಪ್ರತ್ಯಾಗಮ nullptr;
      }
      ನೂತನ (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    ಪ್ರತ್ಯಾಗಮ scratch;
  }

  ಶೂನ್ಯ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_ರಾಶಿ;
  unsigned active_glyphs_ರಾಶಿ_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ಧ್ರುವ hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_ಕೀಲಿ_t, ತಾರ್ಕಿಕ> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_ಕೀಲಿ_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_ಕೀಲಿ_t, hb_codepoint_t> context_set_cache;
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
    ಯಾವತ್ (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  ಶೂನ್ಯ set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
ಸಂರಚನಾ hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ಧ್ರುವ Coverage &>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಧ್ರುವ Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { ಪ್ರತ್ಯಾಗಮ obj.get_coverage (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ Null (Coverage); }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ
  {
    r.collect_coverage (set);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

ಸಂರಚನಾ matcher_t
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*match_func_t) (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data);

  template <typename context_t>
  ಶೂನ್ಯ init (ಧ್ರುವ context_t *c, ತಾರ್ಕಿಕ context_match = ಅಸತ್ಯ)
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

  ಶೂನ್ಯ set_match_func (match_func_t match_func_,
		       ಧ್ರುವ ಶೂನ್ಯ *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  ಗಣನಾ may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) ಧ್ರುವ
  {
    ಯದಿ (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      ಪ್ರತ್ಯಾಗಮ MATCH_NO;

    ಯದಿ (match_func)
      ಪ್ರತ್ಯಾಗಮ match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    ಪ್ರತ್ಯಾಗಮ MATCH_MAYBE;
  }

  ಗಣನಾ may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (ಧ್ರುವ context_t *c,
		       ಧ್ರುವ hb_glyph_info_t &info) ಧ್ರುವ
  {
    ಯದಿ (!c->check_glyph_property (&info, lookup_props))
      ಪ್ರತ್ಯಾಗಮ SKIP_YES;

    ಯದಿ (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      ಪ್ರತ್ಯಾಗಮ SKIP_MAYBE;

    ಪ್ರತ್ಯಾಗಮ SKIP_NO;
  }

  ಸಾರ್ವಜನಿಕ:
  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_props = 0;
  hb_mask_t mask = -1;
  ತಾರ್ಕಿಕ ignore_zwnj = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ ignore_zwj = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ ignore_hidden = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ per_syllable = ಅಸತ್ಯ;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ಧ್ರುವ ಶೂನ್ಯ *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
ಸಂರಚನಾ glyph_data_t
{
  template <typename T>
  glyph_data_t (ಧ್ರುವ T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (ಪರಿಮಾಣ (T) == 3)
#endif
  {
    static_assert (ಪರಿಮಾಣ (T) == 2 || ಪರಿಮಾಣ (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (wide)
      ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ HBUINT24 *) data)[i];
#endif
    ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ HBUINT16 *) data)[i];
  }

  ಧ್ರುವ ಶೂನ್ಯ *data;
#ifndef HB_NO_BEYOND_64K
  ತಾರ್ಕಿಕ wide;
#endif
};

template <typename context_t>
ಸಂರಚನಾ skipping_iterator_t
{
  ಶೂನ್ಯ init (context_t *c_, ತಾರ್ಕಿಕ context_match = ಅಸತ್ಯ)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  ಶೂನ್ಯ set_lookup_props (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  ಶೂನ್ಯ set_match_func (matcher_t::match_func_t match_func_,
		       ಧ್ರುವ ಶೂನ್ಯ *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  ಶೂನ್ಯ set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (ಧ್ರುವ HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (ಧ್ರುವ HBUINT24 *) glyph_data.data : nullptr;
#ಅನ್ಯಥಾ
    match_glyph_data16 = (ಧ್ರುವ HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ಶೂನ್ಯ reset (unsigned ಪೂರ್ಣಾಙ್ಕ start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  ಶೂನ್ಯ reset_back (unsigned ಪೂರ್ಣಾಙ್ಕ start_index_, ತಾರ್ಕಿಕ from_out_buffer = ಅಸತ್ಯ)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ಶೂನ್ಯ reset_fast (unsigned ಪೂರ್ಣಾಙ್ಕ start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ಧ್ರುವ hb_glyph_info_t &info) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ matcher.may_skip (c, info); }

  ಗಣನಾ match_t {
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
    ಯದಿ (unlikely (skip == matcher_t::SKIP_YES))
      ಪ್ರತ್ಯಾಗಮ SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    ಯದಿ (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      ಪ್ರತ್ಯಾಗಮ MATCH;

    ಯದಿ (skip == matcher_t::SKIP_NO)
      ಪ್ರತ್ಯಾಗಮ NOT_MATCH;

    ಪ್ರತ್ಯಾಗಮ SKIP;
  }

  ತಾರ್ಕಿಕ next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    ಧ್ರುವ signed stop = (signed) end - 1;
    ಯಾವತ್ ((signed) idx < stop)
    {
      idx++;
      ವಿಕಲ್ಪ (match (info[idx]))
      {
	ಪಕ್ಷ MATCH:
	{
	  advance_glyph_data ();
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
	}
	ಪಕ್ಷ NOT_MATCH:
	{
	  ಯದಿ (unsafe_to)
	    *unsafe_to = idx + 1;
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
	ಪಕ್ಷ SKIP:
	  ಅನುವರ್ತ;
      }
    }
    ಯದಿ (unsafe_to)
      *unsafe_to = end;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    ಧ್ರುವ unsigned stop = 0;
    ಯಾವತ್ (idx > stop)
    {
      idx--;
      ವಿಕಲ್ಪ (match (out_info[idx]))
      {
	ಪಕ್ಷ MATCH:
	{
	  advance_glyph_data ();
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
	}
	ಪಕ್ಷ NOT_MATCH:
	{
	  ಯದಿ (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
	ಪಕ್ಷ SKIP:
	  ಅನುವರ್ತ;
      }
    }
    ಯದಿ (unsafe_from)
      *unsafe_from = 0;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    ಯದಿ (match_glyph_data16) ಪ್ರತ್ಯಾಗಮ *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    ಅನ್ಯಥಾ
    ಯದಿ (match_glyph_data24) ಪ್ರತ್ಯಾಗಮ *match_glyph_data24;
#endif
    ಪ್ರತ್ಯಾಗಮ 0;
  }
  HB_ALWAYS_INLINE
  ಶೂನ್ಯ
  advance_glyph_data ()
  {
    ಯದಿ (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    ಅನ್ಯಥಾ
    ಯದಿ (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ idx;
  ರಕ್ಷಿತ:
  context_t *c;
  matcher_t matcher;
  ಧ್ರುವ HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ಧ್ರುವ HBUINT24 *match_glyph_data24;
#endif

  unsigned ಪೂರ್ಣಾಙ್ಕ end;
};

ಸಂರಚನಾ hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, ತಾರ್ಕಿಕ, HB_DEBUG_APPLY>
{
  ಧ್ರುವ ವರ್ಣ *get_name () { ಪ್ರತ್ಯಾಗಮ "APPLY"; }
  ಪ್ರಕಾರನಿರ್ದೇಶ return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index);

  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_ (ಧ್ರುವ T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_ (ಧ್ರುವ T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { ಪ್ರತ್ಯಾಗಮ apply_(obj, ಏತತ್, hb_prioritize); }

  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ r; }
  return_t recurse (unsigned ಪೂರ್ಣಾಙ್ಕ sub_lookup_index)
  {
    ಪ್ರತಿಜ್ಞಾ (recurse_func);
    ಯದಿ (unlikely (nesting_level_ಎಡ == 0))
    {
      buffer->successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ default_return_ಮೌಲ್ಯ ();
    }

    buffer->max_ops--;
    ಯದಿ (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ default_return_ಮೌಲ್ಯ ();
    }

    nesting_level_ಎಡ--;
    ತಾರ್ಕಿಕ ret = recurse_func (ಏತತ್, sub_lookup_index);
    nesting_level_ಎಡ++;
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_ಒಳಹರಿವು, iter_context;

  unsigned ಪೂರ್ಣಾಙ್ಕ table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  ಧ್ರುವ GDEF &gdef;
  ಧ್ರುವ GDEF::accelerator_t &gdef_accel;
  ಧ್ರುವ ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index = (unsigned) -1;
  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_props = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ nesting_level_ಎಡ = HB_MAX_NESTING_LEVEL;

  ತಾರ್ಕಿಕ has_glyph_classes;
  ತಾರ್ಕಿಕ auto_zwnj = ಸತ್ಯ;
  ತಾರ್ಕಿಕ auto_zwj = ಸತ್ಯ;
  ತಾರ್ಕಿಕ per_syllable = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ random = ಅಸತ್ಯ;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t ರಾಶಿ_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  ಶೂನ್ಯ *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned ಪೂರ್ಣಾಙ್ಕ table_index_,
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
#ಅನ್ಯಥಾ
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#ಅನ್ಯಥಾ
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (ರಾಶಿ_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    ಯದಿ (buffer->messaging ())
    {
      ಪ್ರತಿಜ್ಞಾ (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = ಏತತ್;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    ಯದಿ (buffer->messaging ())
    {
      ಪ್ರತಿಜ್ಞಾ (buffer->changed_func == buffer_changed_trampoline);
      ಪ್ರತಿಜ್ಞಾ (buffer->changed_data == ಏತತ್);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  ಶೂನ್ಯ init_iters ()
  {
    iter_ಒಳಹರಿವು.init (ಏತತ್, ಅಸತ್ಯ);
    iter_context.init (ಏತತ್, ಸತ್ಯ);
  }

  ಶೂನ್ಯ set_lookup_mask (hb_mask_t mask, ತಾರ್ಕಿಕ init = ಸತ್ಯ) { lookup_mask = mask; last_base = -1; last_base_until = 0; ಯದಿ (init) init_iters (); }
  ಶೂನ್ಯ set_auto_zwj (ತಾರ್ಕಿಕ auto_zwj_, ತಾರ್ಕಿಕ init = ಸತ್ಯ) { auto_zwj = auto_zwj_; ಯದಿ (init) init_iters (); }
  ಶೂನ್ಯ set_auto_zwnj (ತಾರ್ಕಿಕ auto_zwnj_, ತಾರ್ಕಿಕ init = ಸತ್ಯ) { auto_zwnj = auto_zwnj_; ಯದಿ (init) init_iters (); }
  ಶೂನ್ಯ set_per_syllable (ತಾರ್ಕಿಕ per_syllable_, ತಾರ್ಕಿಕ init = ಸತ್ಯ) { per_syllable = per_syllable_; ಯದಿ (init) init_iters (); }
  ಶೂನ್ಯ set_random (ತಾರ್ಕಿಕ random_) { random = random_; }
  ಶೂನ್ಯ set_recurse_func (recurse_func_t func) { recurse_func = func; }
  ಶೂನ್ಯ set_lookup_index (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index_) { lookup_index = lookup_index_; }
  ಶೂನ್ಯ set_lookup_props (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_ಸಂಖ್ಯೆ ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    ಪ್ರತ್ಯಾಗಮ buffer->random_state;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, ಶೂನ್ಯ *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  ಶೂನ್ಯ buffer_changed ()
  {
    buffer->update_digest ();
    ಯದಿ (likely (has_glyph_classes))
      ಪ್ರತಿ (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  ತಾರ್ಕಿಕ match_properties_mark (ಧ್ರುವ hb_glyph_info_t *info,
			      unsigned ಪೂರ್ಣಾಙ್ಕ    glyph_props,
			      unsigned ಪೂರ್ಣಾಙ್ಕ    match_props) ಧ್ರುವ
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    ಯದಿ (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      ಪ್ರತ್ಯಾಗಮ gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    ಯದಿ (match_props & LookupFlag::MarkAttachmentType)
      ಪ್ರತ್ಯಾಗಮ (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ check_glyph_property (ಧ್ರುವ hb_glyph_info_t *info,
			     unsigned match_props) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    ಯದಿ (glyph_props & match_props & LookupFlag::IgnoreFlags)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      ಪ್ರತ್ಯಾಗಮ match_properties_mark (info, glyph_props, match_props);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned ಪೂರ್ಣಾಙ್ಕ class_guess = 0,
			  ತಾರ್ಕಿಕ ligature = ಅಸತ್ಯ,
			  ತಾರ್ಕಿಕ component = ಅಸತ್ಯ)
  {
    buffer->digest.add (glyph_index);

    ಯದಿ (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned ಪೂರ್ಣಾಙ್ಕ props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    ಯದಿ (ligature)
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
    ಯದಿ (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    ಯದಿ (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    ಅನ್ಯಥಾ ಯದಿ (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    ಅನ್ಯಥಾ
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  ಶೂನ್ಯ _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned ಪೂರ್ಣಾಙ್ಕ props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  ಶೂನ್ಯ _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  ಶೂನ್ಯ replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (ಶೂನ್ಯ) buffer->replace_glyph (glyph_index);
  }
  ಶೂನ್ಯ replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  ಶೂನ್ಯ replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned ಪೂರ್ಣಾಙ್ಕ class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, ಸತ್ಯ);
    (ಶೂನ್ಯ) buffer->replace_glyph (glyph_index);
  }
  ಶೂನ್ಯ output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned ಪೂರ್ಣಾಙ್ಕ class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, ಅಸತ್ಯ, ಸತ್ಯ);
    (ಶೂನ್ಯ) buffer->output_glyph (glyph_index);
  }
};

ಗಣನಾ ವರ್ಗ hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

ಸಂರಚನಾ hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto collect_second_glyphs_ (ಧ್ರುವ T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (ಶೂನ್ಯ, obj.collect_second_glyphs (digest) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ collect_second_glyphs_ (ಧ್ರುವ T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_ (ಧ್ರುವ T *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache, hb_priority<1>) HB_RETURN (ತಾರ್ಕಿಕ, obj->apply (c, external_cache) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_ (ಧ್ರುವ T *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache, hb_priority<0>) HB_RETURN (ತಾರ್ಕಿಕ, obj->apply (c) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ apply_to (ಧ್ರುವ ಶೂನ್ಯ *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache)
  {
    ಧ್ರುವ T *typed_obj = (ಧ್ರುವ T *) obj;
    ಪ್ರತ್ಯಾಗಮ apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_cached_ (ಧ್ರುವ T *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache, hb_priority<2>) HB_RETURN (ತಾರ್ಕಿಕ, obj->apply_cached (c, external_cache) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_cached_ (ಧ್ರುವ T *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache, hb_priority<1>) HB_RETURN (ತಾರ್ಕಿಕ, obj->apply (c, external_cache) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto apply_cached_ (ಧ್ರುವ T *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache, hb_priority<0>) HB_RETURN (ತಾರ್ಕಿಕ, obj->apply (c) )
  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ apply_cached_to (ಧ್ರುವ ಶೂನ್ಯ *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache)
  {
    ಧ್ರುವ T *typed_obj = (ಧ್ರುವ T *) obj;
    ಪ್ರತ್ಯಾಗಮ apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (ತಾರ್ಕಿಕ, T::cache_func (c, op) )
  template <typename T=ಶೂನ್ಯ>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  template <typename Type>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    ಪ್ರತ್ಯಾಗಮ cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*hb_apply_func_t) (ಧ್ರುವ ಶೂನ್ಯ *obj, hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache);
  ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  ಸಂರಚನಾ hb_applicable_t
  {
    friend ಸಂರಚನಾ hb_accelerate_subtables_context_t;
    friend ಸಂರಚನಾ hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    ಶೂನ್ಯ init (ಧ್ರುವ T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , ಶೂನ್ಯ *external_cache_
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
    ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    ತಾರ್ಕಿಕ apply_no_digest (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ apply_func (obj, c, nullptr);
    }
#ಅನ್ಯಥಾ
    ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    ತಾರ್ಕಿಕ apply_cached (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    ತಾರ್ಕಿಕ apply_no_digest (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ apply_func (obj, c, external_cache);
    }
    ತಾರ್ಕಿಕ apply_cached_no_digest (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ apply_cached_func (obj, c, external_cache);
    }

    ತಾರ್ಕಿಕ cache_enter (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    ಶೂನ್ಯ cache_leave (hb_ot_apply_context_t *c) ಧ್ರುವ
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    ಗುಪ್ತ:
    ಧ್ರುವ ಶೂನ್ಯ *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    ಶೂನ್ಯ *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (ಧ್ರುವ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (ಧ್ರುವ T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (ಧ್ರುವ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (ಧ್ರುವ T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಶೂನ್ಯ *external_cache = nullptr;
    ಯದಿ (i < 8)
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
    ಯದಿ (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    ಪ್ರತ್ಯಾಗಮ hb_empty_t ();
  }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

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


ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*intersects_func_t) (ಧ್ರುವ hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data, ಶೂನ್ಯ *cache);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*intersected_glyphs_func_t) (ಧ್ರುವ hb_set_t *glyphs, ಧ್ರುವ ಶೂನ್ಯ *data, unsigned value, hb_set_t *intersected_glyphs, ಶೂನ್ಯ *cache);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data);
ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*match_func_t) (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data);
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned (*get_ಮೌಲ್ಯ_func_t) (hb_glyph_info_t &info, ಧ್ರುವ ಶೂನ್ಯ *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
ಸಂರಚನಾ hb_ot_layout_ruleset_digest_t
{
  ಶೂನ್ಯ init () { mask = 0; }
  ಶೂನ್ಯ set_full () { mask = (uint64_t) -1; }
  ಶೂನ್ಯ add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  ತಾರ್ಕಿಕ may_have (unsigned value) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

ಸಂರಚನಾ ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
ಸಂರಚನಾ ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
ಸಂರಚನಾ ContextApplyFuncs
{
  match_func_t match;
};
ಸಂರಚನಾ ChainContextApplyFuncs
{
  match_func_t match[3];
};


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ intersects_glyph (ಧ್ರುವ hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data HB_UNUSED, ಶೂನ್ಯ *cache HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ glyphs->has (value);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ intersects_class (ಧ್ರುವ hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data, ಶೂನ್ಯ *cache)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  ಯದಿ (map->has (value, &cached_v))
    ಪ್ರತ್ಯಾಗಮ *cached_v;

  ತಾರ್ಕಿಕ v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  ಪ್ರತ್ಯಾಗಮ v;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ intersects_coverage (ಧ್ರುವ hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data, ಶೂನ್ಯ *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ಪ್ರತ್ಯಾಗಮ (data+coverage).intersects (glyphs);
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ intersected_glyph (ಧ್ರುವ hb_set_t *glyphs HB_UNUSED, ಧ್ರುವ ಶೂನ್ಯ *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED ಶೂನ್ಯ *cache)
{
  unsigned g = reinterpret_cast<ಧ್ರುವ HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ intersected_class_glyphs (ಧ್ರುವ hb_set_t *glyphs, ಧ್ರುವ ಶೂನ್ಯ *data, unsigned value, hb_set_t *intersected_glyphs, ಶೂನ್ಯ *cache)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  ಯದಿ (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ intersected_coverage_glyphs (ಧ್ರುವ hb_set_t *glyphs, ಧ್ರುವ ಶೂನ್ಯ *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED ಶೂನ್ಯ *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ array_is_subset_of (ಧ್ರುವ hb_set_t *glyphs,
				       unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       ಧ್ರುವ ಶೂನ್ಯ *intersects_data,
				       ಶೂನ್ಯ *cache)
{
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    ಯದಿ (!intersects_func (glyphs, values[i], intersects_data, cache)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ collect_glyph (hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data HB_UNUSED)
{
  glyphs->add (value);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ collect_class (hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ collect_coverage (hb_set_t *glyphs, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

ಅನ್ತಃಸ್ಥ ಧ್ರುವ hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ಧ್ರುವ ಶೂನ್ಯ *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_ಕೀಲಿ_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  ಯದಿ (glyph_set_to_index.has (key, &existing))
    ಪ್ರತ್ಯಾಗಮ glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  ಯದಿ (unlikely (!set))
  {
    depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  ಅನ್ಯಥಾ ಯದಿ (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  ಅನ್ಯಥಾ
    set->add (value);
  ಯದಿ (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  ಯದಿ (unlikely (!glyph_sets.ತಳ್ಳು_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  ಯದಿ (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  ಪ್ರತ್ಯಾಗಮ glyph_sets[index].get ();
}

ಅನ್ತಃಸ್ಥ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ಧ್ರುವ hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  ಯದಿ (cached_context_set_indices.has (key, &existing))
    ಪ್ರತ್ಯಾಗಮ *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  ಯದಿ (unlikely (index == HB_CODEPOINT_INVALID))
    ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID;
  ಯದಿ (unlikely (!cached_context_set_indices.set (key, index)))
    ಪ್ರತ್ಯಾಗಮ depend_data->fail_invalid ();
  ಪ್ರತ್ಯಾಗಮ index;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  ಧ್ರುವ ಶೂನ್ಯ *collect_data)
{
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    collect_func (glyphs, values[i], collect_data);
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, ಧ್ರುವ ಶೂನ್ಯ *data HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_glyph (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ info.codepoint == value;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_class (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);
  ಪ್ರತ್ಯಾಗಮ class_def.get_class (info.codepoint) == value;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_ಮೌಲ್ಯ (hb_glyph_info_t &info, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *> (data);
  ಪ್ರತ್ಯಾಗಮ class_def.get_class (info.codepoint);
}
ಸಂರಚನಾ match_class_cache_data_t
{
  ಧ್ರುವ ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_class_cache (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ auto &cache_data = *reinterpret_cast<ಧ್ರುವ match_class_cache_data_t *> (data);
  ಪ್ರತ್ಯಾಗಮ cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_cache_ಮೌಲ್ಯ (hb_glyph_info_t &info, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ auto &cache_data = *reinterpret_cast<ಧ್ರುವ match_class_cache_data_t *> (data);
  ಪ್ರತ್ಯಾಗಮ cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_cached (ಧ್ರುವ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  ಯದಿ (klass < 255)
    ಪ್ರತ್ಯಾಗಮ klass;
  klass = class_def.get_class (info.codepoint);
  ಯದಿ (likely (klass < 255))
    info.syllable() = klass;
  ಪ್ರತ್ಯಾಗಮ klass;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_class_cached (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);
  ಪ್ರತ್ಯಾಗಮ get_class_cached (class_def, info) == value;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_cached_ಮೌಲ್ಯ (hb_glyph_info_t &info, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *> (data);
  ಪ್ರತ್ಯಾಗಮ get_class_cached (class_def, info);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_cached1 (ಧ್ರುವ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  ಯದಿ (klass < 15)
    ಪ್ರತ್ಯಾಗಮ klass;
  klass = class_def.get_class (info.codepoint);
  ಯದಿ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  ಪ್ರತ್ಯಾಗಮ klass;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_class_cached1 (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);
  ಪ್ರತ್ಯಾಗಮ get_class_cached1 (class_def, info) == value;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_cached2 (ಧ್ರುವ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  ಯದಿ (klass < 15)
    ಪ್ರತ್ಯಾಗಮ klass;
  klass = class_def.get_class (info.codepoint);
  ಯದಿ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  ಪ್ರತ್ಯಾಗಮ klass;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_class_cached2 (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *>(data);
  ಪ್ರತ್ಯಾಗಮ get_class_cached2 (class_def, info) == value;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_class_cached2_ಮೌಲ್ಯ (hb_glyph_info_t &info, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  ಧ್ರುವ ClassDef &class_def = *reinterpret_cast<ಧ್ರುವ ClassDef *> (data);
  ಪ್ರತ್ಯಾಗಮ get_class_cached2 (class_def, info);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ match_coverage (hb_glyph_info_t &info, unsigned value, ಧ್ರುವ ಶೂನ್ಯ *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ಪ್ರತ್ಯಾಗಮ (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ would_match_ಒಳಹರಿವು (hb_would_apply_context_t *c,
				      unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, /* Including the first glyph (not matched) */
				      glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ಧ್ರುವ ಶೂನ್ಯ *match_data)
{
  ಯದಿ (ಎಣಿಕೆ != c->len)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    ಯದಿ (likely (!match_func (info, ಒಳಹರಿವು[i - 1], match_data)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#ಅನ್ಯಥಾ
ಅನ್ತಃಸ್ಥ
#endif
ಸ್ಥಿರ ತಾರ್ಕಿಕ match_ಒಳಹರಿವು (hb_ot_apply_context_t *c,
			 unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, /* Including the first glyph (not matched) */
			 glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ಧ್ರುವ ಶೂನ್ಯ *match_data,
			 unsigned ಪೂರ್ಣಾಙ್ಕ *end_position,
			 unsigned ಪೂರ್ಣಾಙ್ಕ *p_total_component_ಎಣಿಕೆ = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  ಯದಿ (ಎಣಿಕೆ == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    ಯದಿ (p_total_component_ಎಣಿಕೆ)
      *p_total_component_ಎಣಿಕೆ = hb_glyph_info_get_lig_ಅಂಕೆ_comps (&buffer->cur());
    return_trace (ಸತ್ಯ);
  }

  ಯದಿ (unlikely (ಎಣಿಕೆ > HB_MAX_CONTEXT_LENGTH)) return_trace (ಅಸತ್ಯ);

  auto &skippy_iter = c->iter_ಒಳಹರಿವು;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (ಒಳಹರಿವು);

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

  unsigned ಪೂರ್ಣಾಙ್ಕ total_component_ಎಣಿಕೆ = 0;

  unsigned ಪೂರ್ಣಾಙ್ಕ first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned ಪೂರ್ಣಾಙ್ಕ first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  ಗಣನಾ {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
  {
    unsigned unsafe_to;
    ಯದಿ (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (ಅಸತ್ಯ);
    }

    ಯದಿ (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (ಅಸತ್ಯ);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned ಪೂರ್ಣಾಙ್ಕ this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned ಪೂರ್ಣಾಙ್ಕ this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    ಯದಿ (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      ಯದಿ (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	ಯದಿ (ligbase == LIGBASE_NOT_CHECKED)
	{
	  ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
	  ಧ್ರುವ auto *out = buffer->out_info;
	  unsigned ಪೂರ್ಣಾಙ್ಕ j = buffer->out_len;
	  ಯಾವತ್ (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    ಯದಿ (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = ಸತ್ಯ;
	      ಭಙ್ಗ;
	    }
	    j--;
	  }

	  ಯದಿ (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  ಅನ್ಯಥಾ
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	ಯದಿ (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (ಅಸತ್ಯ);
      }
    }
    ಅನ್ಯಥಾ
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      ಯದಿ (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (ಅಸತ್ಯ);
    }

    total_component_ಎಣಿಕೆ += hb_glyph_info_get_lig_ಅಂಕೆ_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  ಯದಿ (p_total_component_ಎಣಿಕೆ)
  {
    total_component_ಎಣಿಕೆ += hb_glyph_info_get_lig_ಅಂಕೆ_comps (&buffer->cur());
    *p_total_component_ಎಣಿಕೆ = total_component_ಎಣಿಕೆ;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (ಸತ್ಯ);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ligate_ಒಳಹರಿವು (hb_ot_apply_context_t *c,
				 unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, /* Including the first glyph */
				 unsigned ಪೂರ್ಣಾಙ್ಕ match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned ಪೂರ್ಣಾಙ್ಕ total_component_ಎಣಿಕೆ)
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

  ತಾರ್ಕಿಕ is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  ತಾರ್ಕಿಕ is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
    ಯದಿ (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = ಅಸತ್ಯ;
      is_mark_ligature = ಅಸತ್ಯ;
      ಭಙ್ಗ;
    }
  ತಾರ್ಕಿಕ is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned ಪೂರ್ಣಾಙ್ಕ klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned ಪೂರ್ಣಾಙ್ಕ last_ಅಂಕೆ_components = hb_glyph_info_get_lig_ಅಂಕೆ_comps (&buffer->cur());
  unsigned ಪೂರ್ಣಾಙ್ಕ components_so_far = last_ಅಂಕೆ_components;

  ಯದಿ (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_ಎಣಿಕೆ);
    ಯದಿ (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
  {
    ಯಾವತ್ (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      ಯದಿ (is_ligature)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	ಯದಿ (this_comp == 0)
	  this_comp = last_ಅಂಕೆ_components;
	ಪ್ರತಿಜ್ಞಾ (components_so_far >= last_ಅಂಕೆ_components);
	unsigned ಪೂರ್ಣಾಙ್ಕ new_lig_comp = components_so_far - last_ಅಂಕೆ_components +
				    hb_min (this_comp, last_ಅಂಕೆ_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (ಶೂನ್ಯ) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_ಅಂಕೆ_components = hb_glyph_info_get_lig_ಅಂಕೆ_comps_in_ligation (&buffer->cur());
    components_so_far += last_ಅಂಕೆ_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  ಯದಿ (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    ಪ್ರತಿ (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      ಯದಿ (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) ಭಙ್ಗ;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      ಯದಿ (!this_comp) ಭಙ್ಗ;

      ಪ್ರತಿಜ್ಞಾ (components_so_far >= last_ಅಂಕೆ_components);
      unsigned new_lig_comp = components_so_far - last_ಅಂಕೆ_components +
			      hb_min (this_comp, last_ಅಂಕೆ_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (ಸತ್ಯ);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#ಅನ್ಯಥಾ
ಅನ್ತಃಸ್ಥ
#endif
ಸ್ಥಿರ ತಾರ್ಕಿಕ match_backtrack (hb_ot_apply_context_t *c,
			     unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     ಧ್ರುವ ಶೂನ್ಯ *match_data,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *match_start)
{
  TRACE_APPLY (nullptr);

  ಯದಿ (!ಎಣಿಕೆ)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (ಸತ್ಯ);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
  {
    unsigned unsafe_from;
    ಯದಿ (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (ಅಸತ್ಯ);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (ಸತ್ಯ);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#ಅನ್ಯಥಾ
ಅನ್ತಃಸ್ಥ
#endif
ಸ್ಥಿರ ತಾರ್ಕಿಕ match_lookahead (hb_ot_apply_context_t *c,
			     unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     ಧ್ರುವ ಶೂನ್ಯ *match_data,
			     unsigned ಪೂರ್ಣಾಙ್ಕ start_index,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *end_index)
{
  TRACE_APPLY (nullptr);

  ಯದಿ (!ಎಣಿಕೆ)
  {
    *end_index = start_index;
    return_trace (ಸತ್ಯ);
  }

  auto &skippy_iter = c->iter_context;
  ಪ್ರತಿಜ್ಞಾ (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
  {
    unsigned unsafe_to;
    ಯದಿ (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (ಅಸತ್ಯ);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (ಸತ್ಯ);
}



ಸಂರಚನಾ LookupRecord
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ hb_map_t         *lookup_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ಧ್ರುವ hb_array_t<ಧ್ರುವ LookupRecord> lookupRecords,
						     ಧ್ರುವ hb_map_t *lookup_map)
{
  unsigned ಎಣಿಕೆ = 0;
  ಪ್ರತಿ (ಧ್ರುವ LookupRecord& r : lookupRecords)
  {
    ಯದಿ (!lookup_map->has (r.lookupListIndex))
      ಅನುವರ್ತ;

    ಯದಿ (!r.serialize (c, lookup_map))
      ಪ್ರತ್ಯಾಗಮ 0;

    ಎಣಿಕೆ++;
  }
  ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಗಣನಾ ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ಧ್ರುವ ಶೂನ್ಯ *data,
			unsigned value,
			ತಾರ್ಕಿಕ filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  ವಿಕಲ್ಪ (context_format)
  {
    ಪಕ್ಷ ContextFormat::SimpleContext:
      ಪ್ರತ್ಯಾಗಮ c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    ಪಕ್ಷ ContextFormat::ClassBasedContext:
    {
      ಧ್ರುವ hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      ಯದಿ (unlikely (!class_set))
	ಪ್ರತ್ಯಾಗಮ nullptr;
      ಯದಿ (!filter_class_by_parent)
	ಪ್ರತ್ಯಾಗಮ class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      ಭಙ್ಗ;
    }
    ಪಕ್ಷ ContextFormat::CoverageBasedContext:
      ಪ್ರತ್ಯಾಗಮ c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  ಯದಿ (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  ಪ್ರತ್ಯಾಗಮ scratch;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned ಒಳಹರಿವು_ಎಣಿಕೆ, glyph_data_t ಒಳಹರಿವು,
					     unsigned lookup_ಎಣಿಕೆ_2,
					     ಧ್ರುವ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     ಧ್ರುವ ಶೂನ್ಯ *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ಶೂನ್ಯ *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < lookup_ಎಣಿಕೆ_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    ಯದಿ (seqIndex >= ಒಳಹರಿವು_ಎಣಿಕೆ) ಅನುವರ್ತ;

    ತಾರ್ಕಿಕ has_pos_glyphs = ಅಸತ್ಯ;

    ಯದಿ (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = ಸತ್ಯ;
      pos_glyphs.clear ();
      ಯದಿ (seqIndex == 0)
      {
        ವಿಕಲ್ಪ (context_format) {
        ಪಕ್ಷ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          ಭಙ್ಗ;
        ಪಕ್ಷ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          ಭಙ್ಗ;
        ಪಕ್ಷ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          ಭಙ್ಗ;
        }
      }
      ಅನ್ಯಥಾ
      {
        ಧ್ರುವ ಶೂನ್ಯ *ಒಳಹರಿವು_data = ಒಳಹರಿವು.data;
        unsigned ಒಳಹರಿವು_ಮೌಲ್ಯ = seqIndex - 1;
        ಯದಿ (context_format != ContextFormat::SimpleContext)
        {
          ಒಳಹರಿವು_data = data;
          ಒಳಹರಿವು_ಮೌಲ್ಯ = ಒಳಹರಿವು[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, ಒಳಹರಿವು_data, ಒಳಹರಿವು_ಮೌಲ್ಯ, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs))
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } ಅನ್ಯಥಾ {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = ಒಳಹರಿವು_ಎಣಿಕೆ;
    ಯದಿ (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->ತೆಗೆ_cur_done_glyphs ();
  }
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned ಒಳಹರಿವು_ಎಣಿಕೆ_2,
				unsigned lookup_ಎಣಿಕೆ,
				ಧ್ರುವ LookupRecord lookup_records[],
				ಧ್ರುವ ಶೂನ್ಯ *rule,
				unsigned value,
				ContextFormat context_format,
				ಧ್ರುವ ಶೂನ್ಯ *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  ತಾರ್ಕಿಕ have_active_idx = ಅಸತ್ಯ;
  ಪ್ರತಿ (unsigned i = 0; i < lookup_ಎಣಿಕೆ; i++)
  {
    ಯದಿ (lookup_records[i].sequenceIndex >= ಒಳಹರಿವು_ಎಣಿಕೆ_2)
      ಅನುವರ್ತ;

    ಯದಿ (!have_active_idx)
    {
      ಯದಿ (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      have_active_idx = ಸತ್ಯ;
    }

    hb_depend_context_t::context_set_cache_ಕೀಲಿ_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    ಯದಿ (!c->context_set_cache.has (key, &cached_context_set))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely (!cached_context_sets->ತಳ್ಳು_or_fail (*cached_context_set)))
      ಪ್ರತ್ಯಾಗಮ c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  ಪ್ರತ್ಯಾಗಮ have_active_idx;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned ಒಳಹರಿವು_ಎಣಿಕೆ, glyph_data_t ಒಳಹರಿವು,
					     unsigned lookup_ಎಣಿಕೆ_2,
					     ಧ್ರುವ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ಧ್ರುವ ಶೂನ್ಯ *rule,
					     unsigned value,
					     ContextFormat context_format,
					     ಧ್ರುವ ಶೂನ್ಯ *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ಶೂನ್ಯ *cache,
					     ಧ್ರುವ hb_set_t &preliminary_context,
					     ಧ್ರುವ hb_vector_t<ಧ್ರುವ hb_set_t *> *ಒಳಹರಿವು_position_glyphs,
					     ಧ್ರುವ hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  ಯದಿ (unlikely (ಒಳಹರಿವು_ಎಣಿಕೆ > HB_MAX_CONTEXT_LENGTH))
    ಪ್ರತ್ಯಾಗಮ;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < lookup_ಎಣಿಕೆ_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    ಯದಿ (seqIndex >= ಒಳಹರಿವು_ಎಣಿಕೆ)
      ಅನುವರ್ತ;

    hb_depend_context_t::context_set_cache_ಕೀಲಿ_t context_cache_ಕೀಲಿ;
    ಯದಿ (unlikely (!c->get_context_set_cache_ಕೀಲಿ (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_ಕೀಲಿ)))
      ಪ್ರತ್ಯಾಗಮ;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    ಯದಿ (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    ಅನ್ಯಥಾ ಯದಿ (c->context_set_cache.has (context_cache_ಕೀಲಿ, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (!ಒಳಹರಿವು_position_glyphs))
      {
	c->depend_data->fail ();
	ಪ್ರತ್ಯಾಗಮ;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_ಮೌಲ್ಯ_INVALID;
      ಯಾವತ್ (preliminary_context.next (&elem)) {
	ಯದಿ (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} ಅನ್ಯಥಾ {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      ಪ್ರತಿ (unsigned j = 0; j < ಒಳಹರಿವು_position_glyphs->length; j++) {
	ಯದಿ (j != seqIndex && (*ಒಳಹರಿವು_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*ಒಳಹರಿವು_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_ಮೌಲ್ಯ_INVALID;
      ಯಾವತ್ (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ಧ್ರುವ hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	ಯದಿ (unlikely (!original_set)) ಅನುವರ್ತ;

	/* Dependency context sets are never inverted. */
	ಯದಿ (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      ಪ್ರತಿ (unsigned j = 0; j < ಒಳಹರಿವು_position_glyphs->length; j++) {
	ಯದಿ (j != seqIndex && (*ಒಳಹರಿವು_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  ಯದಿ (!(*ಒಳಹರಿವು_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    ತಾರ್ಕಿಕ cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*ಒಳಹರಿವು_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*ಒಳಹರಿವು_position_glyphs)[j]);
	    ಯದಿ (unlikely (idx == HB_CODEPOINT_INVALID))
	      ಪ್ರತ್ಯಾಗಮ;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      ಯದಿ (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	ಪ್ರತ್ಯಾಗಮ;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      ಯದಿ (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	ಪ್ರತ್ಯಾಗಮ;

      ಯದಿ (unlikely (!c->context_set_cache.set (context_cache_ಕೀಲಿ, context_set_idx)))
      {
	c->depend_data->fail ();
	ಪ್ರತ್ಯಾಗಮ;
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
    ತಾರ್ಕಿಕ nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    ಯದಿ (ಒಳಹರಿವು_ಎಣಿಕೆ > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    ಯದಿ (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    ತಾರ್ಕಿಕ has_pos_glyphs = ಅಸತ್ಯ;

    ಯದಿ (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = ಸತ್ಯ;
      pos_glyphs.clear ();
      ಯದಿ (seqIndex == 0)
      {
        ವಿಕಲ್ಪ (context_format) {
        ಪಕ್ಷ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          ಭಙ್ಗ;
        ಪಕ್ಷ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          ಭಙ್ಗ;
        ಪಕ್ಷ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          ಭಙ್ಗ;
        }
      }
      ಅನ್ಯಥಾ
      {
        ಧ್ರುವ ಶೂನ್ಯ *ಒಳಹರಿವು_data = ಒಳಹರಿವು.data;
        unsigned ಒಳಹರಿವು_ಮೌಲ್ಯ = seqIndex - 1;
        ಯದಿ (context_format != ContextFormat::SimpleContext)
        {
          ಒಳಹರಿವು_data = data;
          ಒಳಹರಿವು_ಮೌಲ್ಯ = ಒಳಹರಿವು[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, ಒಳಹರಿವು_data, ಒಳಹರಿವು_ಮೌಲ್ಯ, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    ಯದಿ (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      ಯದಿ (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	ಯದಿ (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs (active_idx);
      ಯದಿ (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	ಪ್ರತ್ಯಾಗಮ;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!c->ತಳ್ಳು_ಬೇರು_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      ಪ್ರತ್ಯಾಗಮ;
    }

    unsigned endIndex = ಒಳಹರಿವು_ಎಣಿಕೆ;
    ಯದಿ (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->ತೆಗೆ_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ recurse_lookups (context_t *c,
                                    unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
                                    ಧ್ರುವ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < lookup_ಎಣಿಕೆ_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ apply_lookup (hb_ot_apply_context_t *c,
				 unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, /* Including the first glyph */
				 unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
				 ಧ್ರುವ LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned ಪೂರ್ಣಾಙ್ಕ match_end)
{
  hb_buffer_t *buffer = c->buffer;
  ಪೂರ್ಣಾಙ್ಕ end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    ಪೂರ್ಣಾಙ್ಕ delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < ಎಣಿಕೆ; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < lookup_ಎಣಿಕೆ_2 && buffer->successful; i++)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ idx = lookupRecord[i].sequenceIndex;
    ಯದಿ (idx >= ಎಣಿಕೆ)
      ಅನುವರ್ತ;

    unsigned ಪೂರ್ಣಾಙ್ಕ orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    ಯದಿ (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      ಅನುವರ್ತ;

    ಯದಿ (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      ಭಙ್ಗ;

    ಯದಿ (unlikely (buffer->max_ops <= 0))
      ಭಙ್ಗ;

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      ಯದಿ (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    ತಾರ್ಕಿಕ ret = c->recurse (lookupRecord[i].lookupListIndex);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      ಯದಿ (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    ಯದಿ (!ret)
      ಅನುವರ್ತ;

    unsigned ಪೂರ್ಣಾಙ್ಕ new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    ಪೂರ್ಣಾಙ್ಕ delta = new_len - orig_len;

    ಯದಿ (!delta)
      ಅನುವರ್ತ;

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
    ಯದಿ (end < ಪೂರ್ಣಾಙ್ಕ (c->match_positions.arrayZ[idx]))
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

    unsigned ಪೂರ್ಣಾಙ್ಕ next = idx + 1; /* next now is the position after the recursed lookup. */

    ಯದಿ (delta > 0)
    {
      ಯದಿ (unlikely (delta + ಎಣಿಕೆ > HB_MAX_CONTEXT_LENGTH))
	ಭಙ್ಗ;
      ಯದಿ (unlikely (ಎಣಿಕೆ + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (ಎಣಿಕೆ + delta)))
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಅನ್ಯಥಾ
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (ಪೂರ್ಣಾಙ್ಕ) next - (ಪೂರ್ಣಾಙ್ಕ) ಎಣಿಕೆ);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (ಎಣಿಕೆ - next) * ಪರಿಮಾಣ (c->match_positions.arrayZ[0]));
    next += delta;
    ಎಣಿಕೆ += delta;

    /* Fill in new entries. */
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    ಪ್ರತಿ (; next < ಎಣಿಕೆ; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  ಪ್ರತಿಜ್ಞಾ (end >= 0);
  (ಶೂನ್ಯ) buffer->move_to (end);
}



/* Contextual lookups */

ಸಂರಚನಾ ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ಧ್ರುವ ಶೂನ್ಯ *intersects_data;
  ಶೂನ್ಯ *intersects_cache;
  ಶೂನ್ಯ *intersected_glyphs_cache;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ContextClosureLookupContext ContextDependLookupContext;

ಸಂರಚನಾ ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ಧ್ರುವ ಶೂನ್ಯ *collect_data;
};

ಸಂರಚನಾ ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ಧ್ರುವ ಶೂನ್ಯ *match_data;
  get_ಮೌಲ್ಯ_func_t get_ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ context_intersects (ಧ್ರುವ hb_set_t *glyphs,
				       unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
				       glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  ಪ್ರತ್ಯಾಗಮ array_is_subset_of (glyphs,
			     ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0, ಒಳಹರಿವು,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ context_closure_lookup (hb_closure_context_t *c,
					   unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
					   glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
					   unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
					   ಧ್ರುವ LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  ಯದಿ (context_intersects (c->glyphs,
			  ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
				     lookup_ಎಣಿಕೆ_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ context_depend_lookup (hb_depend_context_t *c,
					  unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
					  glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
					  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
					  ಧ್ರುವ LookupRecord lookupRecord[],
					  ಧ್ರುವ ಶೂನ್ಯ *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  ಯದಿ (!context_intersects (c->glyphs,
			   ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
			   lookup_context))
    ಪ್ರತ್ಯಾಗಮ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ಯದಿ (unlikely (!scratch))
    ಪ್ರತ್ಯಾಗಮ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  ತಾರ್ಕಿಕ context_sets_cached = context_depend_sets_are_cached (
    c, ಒಳಹರಿವು_ಎಣಿಕೆ, lookup_ಎಣಿಕೆ_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  ಯದಿ (unlikely (!c->depend_data->successful))
    ಪ್ರತ್ಯಾಗಮ;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ಧ್ರುವ hb_set_t *> &ಒಳಹರಿವು_position_glyphs = scratch->ಒಳಹರಿವು_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  ಯದಿ (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ಧ್ರುವ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      ಸತ್ಯ, &position_scratch, &scratch->compact_workspace);
    ಯದಿ (unlikely (!pos0_glyphs ||
		  !ಒಳಹರಿವು_position_glyphs.ತಳ್ಳು_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* Positions 1+ (Input array) */
    ಪ್ರತಿ (unsigned i = 0; i + 1 < ಒಳಹರಿವು_ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	ಒಳಹರಿವು[i], ಅಸತ್ಯ, &position_scratch);
      ಯದಿ (unlikely (!pos_glyphs ||
		    !ಒಳಹರಿವು_position_glyphs.ತಳ್ಳು_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	ಪ್ರತ್ಯಾಗಮ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
				  lookup_ಎಣಿಕೆ_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &ಒಳಹರಿವು_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
						  glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
						  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
						  ಧ್ರುವ LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->ಒಳಹರಿವು,
		 ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0, ಒಳಹರಿವು,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_ಎಣಿಕೆ_2, lookupRecord);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
					       glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
					       unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2 HB_UNUSED,
					       ಧ್ರುವ LookupRecord lookupRecord[] HB_UNUSED,
					       ಧ್ರುವ ContextApplyLookupContext &lookup_context)
{
  ಪ್ರತ್ಯಾಗಮ would_match_ಒಳಹರಿವು (c,
			    ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
			    lookup_context.funcs.match, lookup_context.match_data);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
					 glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
					 unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
					 ಧ್ರುವ LookupRecord lookupRecord[],
					 ಧ್ರುವ ContextApplyLookupContext &lookup_context)
{
  ಯದಿ (unlikely (ಒಳಹರಿವು_ಎಣಿಕೆ > HB_MAX_CONTEXT_LENGTH)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned match_end = 0;
  ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
  ಯದಿ (match_ಒಳಹರಿವು (c,
		   ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  ಒಳಹರಿವು_ಎಣಿಕೆ,
		  lookup_ಎಣಿಕೆ_2, lookupRecord,
		  match_end);
    ret = ಸತ್ಯ;
  }
  ಅನ್ಯಥಾ
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = ಅಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  ವಿಕಲ್ಪ (op)
  {
    ಪಕ್ಷ hb_ot_subtable_cache_op_t::ENTER:
    {
      ಯದಿ (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      auto &info = c->buffer->info;
      unsigned ಎಣಿಕೆ = c->buffer->len;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪಕ್ಷ hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      ಭಙ್ಗ;
    }
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

template <typename Types>
ಸಂರಚನಾ Rule
{
  template <typename SetTypes, typename RuleType>
  friend ಸಂರಚನಾ RuleSetOf;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ context_intersects (glyphs,
			       ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು_Z.arrayZ,
			       lookup_context);
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array ((ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0)));
    context_depend_lookup (c,
			   ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು_Z.arrayZ,
			   lookup_ಎಣಿಕೆ_2, lookupRecord.arrayZ,
			   ಏತತ್,
			   value, lookup_context);
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array ((ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0)));
    context_closure_lookup (c,
			    ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು_Z.arrayZ,
			    lookup_ಎಣಿಕೆ_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!intersects (c->glyphs, lookup_context)) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array (ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0));
    recurse_lookups (c, lookup_ಎಣಿಕೆ_2, lookupRecord.arrayZ);
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array (ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0));
    context_collect_glyphs_lookup (c,
				   ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು_Z.arrayZ,
				   lookup_ಎಣಿಕೆ_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest,
			      ಧ್ರುವ ClassDef *class_def = nullptr) ಧ್ರುವ
  {
    ಯದಿ (ಒಳಹರಿವು_ಎಣಿಕೆ <= 1)
    {
      *digest = hb_set_digest_t::full ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    unsigned second = ಒಳಹರಿವು_Z.arrayZ[0];
    ಯದಿ (!class_def)
      digest->add (second);
    ಅನ್ಯಥಾ ಯದಿ (!second)
      *digest = hb_set_digest_t::full ();
    ಅನ್ಯಥಾ
      class_def->collect_class (digest, second);
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c,
		    ಧ್ರುವ ContextApplyLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array (ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0));
    ಪ್ರತ್ಯಾಗಮ context_would_apply_lookup (c,
				       ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು_Z.arrayZ,
				       lookup_ಎಣಿಕೆ_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c,
	      ಧ್ರುವ ContextApplyLookupContext &lookup_context) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array (ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0));
    return_trace (context_apply_lookup (c, ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು_Z.arrayZ, lookup_ಎಣಿಕೆ_2, lookupRecord.arrayZ, lookup_context));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ hb_map_t *ಒಳಹರಿವು_mapping, /* old->new glyphid or class mapping */
		  ಧ್ರುವ hb_map_t *lookup_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->ಒಳಹರಿವು_ಎಣಿಕೆ = ಒಳಹರಿವು_ಎಣಿಕೆ;
    ಧ್ರುವ auto ಒಳಹರಿವು = ಒಳಹರಿವು_Z.as_array (ಒಳಹರಿವು_ಎಣಿಕೆ - 1);
    ಪ್ರತಿ (ಧ್ರುವ auto org : ಒಳಹರಿವು)
    {
      typename Types::HBUINT d;
      d = ಒಳಹರಿವು_mapping->get (org);
      c->copy (d);
    }

    ಧ್ರುವ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (ಒಳಹರಿವು_Z.as_array ((ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0)));

    unsigned ಎಣಿಕೆ = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_ಎಣಿಕೆ_2), lookup_map);
    return_trace (c->check_assign (out->lookup_ಎಣಿಕೆ_2, ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       ಧ್ರುವ hb_map_t *lookup_map,
	       ಧ್ರುವ hb_map_t *klass_map = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (unlikely (!ಒಳಹರಿವು_ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ);
    ಧ್ರುವ auto ಒಳಹರಿವು = ಒಳಹರಿವು_Z.as_array (ಒಳಹರಿವು_ಎಣಿಕೆ - 1);

    ಧ್ರುವ hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    ಯದಿ (!hb_all (ಒಳಹರಿವು, mapping)) return_trace (ಅಸತ್ಯ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  c->check_range (ಒಳಹರಿವು_Z.arrayZ,
				  ಒಳಹರಿವು_Z.item_size * (ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0) +
				  LookupRecord::static_size * lookup_ಎಣಿಕೆ_2));
  }

  ರಕ್ಷಿತ:
  HBUINT16	ಒಳಹರಿವು_ಎಣಿಕೆ;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_ಎಣಿಕೆ_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		ಒಳಹರಿವು_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, ಒಳಹರಿವು_Z);
};

template <typename Types, typename RuleType>
ಸಂರಚನಾ RuleSetOf
{
  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([&] (ಧ್ರುವ RuleType &_) { ಪ್ರತ್ಯಾಗಮ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ಶೂನ್ಯ collect_first_ಒಳಹರಿವು_classes (hb_ot_layout_ruleset_digest_t *digest) ಧ್ರುವ
  {
    digest->init ();
    ಪ್ರತಿ (unsigned i = 0; i < rule.len; i++)
    {
      ಧ್ರುವ RuleType &r = ಏತತ್+rule.arrayZ[i];
      ಯದಿ (r.ಒಳಹರಿವು_ಎಣಿಕೆ <= 1)
      {
	digest->set_full ();
	ಪ್ರತ್ಯಾಗಮ;
      }
      digest->add (r.ಒಳಹರಿವು_Z.arrayZ[0]);
    }
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ಧ್ರುವ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest,
			      ಧ್ರುವ ClassDef *class_def = nullptr) ಧ್ರುವ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([=] (ಧ್ರುವ RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c,
		    ಧ್ರುವ ContextApplyLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([&] (ಧ್ರುವ RuleType &_) { ಪ್ರತ್ಯಾಗಮ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c,
	      ಧ್ರುವ ContextApplyLookupContext &lookup_context,
	      ಧ್ರುವ hb_ot_layout_ruleset_digest_t *digest = nullptr) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    unsigned ಅಂಕೆ_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    ಯದಿ (HB_OPTIMIZE_SIZE_VAL || ಅಂಕೆ_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (ಏತತ್))
      | hb_map ([&] (ಧ್ರುವ RuleType &_) { ಪ್ರತ್ಯಾಗಮ _.apply (c, lookup_context); })
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
    ತಾರ್ಕಿಕ matched = skippy_iter.next (&unsafe_to1);
    ಯದಿ (likely (matched))
    {
      ಯದಿ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ಗಚ್ಛ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ಯದಿ (digest && lookup_context.get_ಮೌಲ್ಯ &&
	  !digest->may_have (lookup_context.get_ಮೌಲ್ಯ (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (ಅಸತ್ಯ);
	}
    }
    ಅನ್ಯಥಾ
    {
      ತಾರ್ಕಿಕ unsafe_to_concat = ಅಸತ್ಯ;
      ತಾರ್ಕಿಕ ret =
      + hb_iter (rule)
      | hb_map (hb_add (ಏತತ್))
      | hb_filter ([&] (ಧ್ರುವ RuleType &_)
		   {
		     ಯದಿ (_.ಒಳಹರಿವು_ಎಣಿಕೆ <= 1) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
		     unsafe_to_concat = ಸತ್ಯ;
		     ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
		   })
      | hb_map ([&] (ಧ್ರುವ RuleType &_) { ಪ್ರತ್ಯಾಗಮ _.apply (c, lookup_context); })
      | hb_any
      ;
      ಯದಿ (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ಯದಿ (likely (matched))
    {
      ಯದಿ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ಗಚ್ಛ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_ಒಳಹರಿವು = lookup_context.funcs.match;
    auto *ಒಳಹರಿವು_data = lookup_context.match_data;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_rules; i++)
    {
      ಧ್ರುವ auto &r = ಏತತ್+rule.arrayZ[i];

      ಧ್ರುವ auto &ಒಳಹರಿವು = r.ಒಳಹರಿವು_Z;

      ಯದಿ (r.ಒಳಹರಿವು_ಎಣಿಕೆ <= 1 ||
	  (!match_ಒಳಹರಿವು ||
	   match_ಒಳಹರಿವು (*first, ಒಳಹರಿವು.arrayZ[0], ಒಳಹರಿವು_data)))
      {
        ಯದಿ (!second ||
	    (r.ಒಳಹರಿವು_ಎಣಿಕೆ <= 2 ||
	     (!match_ಒಳಹರಿವು ||
	      match_ಒಳಹರಿವು (*second, ಒಳಹರಿವು.arrayZ[1], ಒಳಹರಿವು_data)))
	   )
	{
	  ಯದಿ (r.apply (c, lookup_context))
	  {
	    ಯದಿ (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (ಸತ್ಯ);
	  }
	}
	ಅನ್ಯಥಾ
	  unsafe_to = unsafe_to2;
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	ಪ್ರತಿ (; i + 1 < ಅಂಕೆ_rules; i++)
	{
	  ಧ್ರುವ auto &r2 = ಏತತ್+rule.arrayZ[i + 1];
	  ಧ್ರುವ auto &input2 = r2.ಒಳಹರಿವು_Z;
	  ಯದಿ (r2.ಒಳಹರಿವು_ಎಣಿಕೆ <= 1 || input2.arrayZ[0] != ಒಳಹರಿವು.arrayZ[0])
	    ಭಙ್ಗ;
	}
      }
    }
    ಯದಿ (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ಅಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       ಧ್ರುವ hb_map_t *lookup_map,
	       ಧ್ರುವ hb_map_t *klass_map = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : rule)
    {
      ಯದಿ (!_) ಅನುವರ್ತ;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      ಯದಿ (unlikely (!o)) ಅನುವರ್ತ;

      ಯದಿ (!o->serialize_subset (c, _, ಏತತ್, lookup_map, klass_map))
      {
	out->rule.ತೆಗೆ ();
	c->serializer->revert (o_snap);
      }
    }

    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (out->rule);
    ಯದಿ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (rule.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ಸಂರಚನಾ RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
ಸಂರಚನಾ ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
ಸಂರಚನಾ ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    ಪ್ರತ್ಯಾಗಮ
    + hb_zip (ಏತತ್+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([&] (ಧ್ರುವ RuleSet &_) { ಪ್ರತ್ಯಾಗಮ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (ಏತತ್+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ಪ್ರತ್ಯಾಗಮ c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ಧ್ರುವ hb_pair_t<hb_codepoint_t, unsigned> _) { ಪ್ರತ್ಯಾಗಮ hb_pair_t<unsigned, ಧ್ರುವ RuleSet&> (_.first, ಏತತ್+ruleSet[_.second]); })
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (ಏತತ್+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ಪ್ರತ್ಯಾಗಮ c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ಧ್ರುವ hb_pair_t<hb_codepoint_t, unsigned> _) { ಪ್ರತ್ಯಾಗಮ hb_pair_t<unsigned, ಧ್ರುವ RuleSet&> (_.first, ಏತತ್+ruleSet[_.second]); })
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ
  {
    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (ಏತತ್+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು);

    ಸಂರಚನಾ ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([digest] (ಧ್ರುವ RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ RuleSet &rule_set = ಏತತ್+ruleSet[(ಏತತ್+coverage).get_coverage (c->glyphs[0])];
    ಸಂರಚನಾ ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    ಪ್ರತ್ಯಾಗಮ rule_set.would_apply (c, lookup_context);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (likely (index == NOT_COVERED))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ RuleSet &rule_set = ಏತತ್+ruleSet[index];
    ಸಂರಚನಾ ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    ಧ್ರುವ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, ಏತತ್, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && ruleSet.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ಸಂರಚನಾ ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (glyphs))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;

    hb_map_t cache;
    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (ಏತತ್+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_enumerate
    | hb_map ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ RuleSet &> p)
	      { ಪ್ರತ್ಯಾಗಮ class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;

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
    { ಪ್ರತ್ಯಾಗಮ class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ಧ್ರುವ RuleSet& rule_set = ಏತತ್+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ಪ್ರತ್ಯಾಗಮ class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ಧ್ರುವ RuleSet& rule_set = ಏತತ್+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;

    hb_map_t cache;
    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_enumerate
    | hb_filter ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ RuleSet &> p)
    { ಪ್ರತ್ಯಾಗಮ class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ಧ್ರುವ RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು);

    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;
    ಸಂರಚನಾ ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([=, &class_def] (ಧ್ರುವ RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = class_def.get_class (c->glyphs[0]);
    ಧ್ರುವ RuleSet &rule_set = ಏತತ್+ruleSet[index];
    ಸಂರಚನಾ ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    ಪ್ರತ್ಯಾಗಮ rule_set.would_apply (c, lookup_context);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  unsigned cache_cost () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+classDef).cost ();
  }
  ಸ್ಥಿರ ತಾರ್ಕಿಕ cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ಪ್ರತ್ಯಾಗಮ context_cache_func (c, op);
  }

  ಸಂರಚನಾ external_cache_t
  {
    hb_ot_layout_ದ್ವಿಮಾನ_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  ಶೂನ್ಯ *external_cache_create () ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = ruleSet.len;
    unsigned size = ಪರಿಮಾಣ (external_cache_t) -
		    HB_VAR_ARRAY * ಪರಿಮಾಣ (hb_ot_layout_ruleset_digest_t) +
		    ಎಣಿಕೆ * ಪರಿಮಾಣ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ಯದಿ (likely (cache))
    {
      cache->coverage.clear ();
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	(ಏತತ್+ruleSet[i]).collect_first_ಒಳಹರಿವು_classes (&cache->rule_sets[i]);
    }
    ಪ್ರತ್ಯಾಗಮ cache;
  }
  ತಾರ್ಕಿಕ apply_cached (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _apply (c, ಸತ್ಯ, external_cache); }
  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _apply (c, ಅಸತ್ಯ, external_cache); }
  ತಾರ್ಕಿಕ _apply (hb_ot_apply_context_t *c, ತಾರ್ಕಿಕ cached, ಶೂನ್ಯ *external_cache) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage_ದ್ವಿಮಾನ (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ಅನ್ಯಥಾ
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ ClassDef &class_def = ಏತತ್+classDef;

    ಸಂರಚನಾ ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_ಮೌಲ್ಯ : get_class_ಮೌಲ್ಯ
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    ಧ್ರುವ RuleSet &rule_set = ಏತತ್+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಧ್ರುವ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#ಅನ್ಯಥಾ
    ಧ್ರುವ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;
    ಯದಿ (unlikely (!out->coverage.serialize_subset (c, coverage, ಏತತ್)))
      return_trace (ಅಸತ್ಯ);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, ಏತತ್, &klass_map);

    ಧ್ರುವ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (ಏತತ್+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (ಏತತ್+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ಧ್ರುವ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    ತಾರ್ಕಿಕ ret = ಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      ಯದಿ (unlikely (!o))
      {
	ret = ಅಸತ್ಯ;
	ಭಙ್ಗ;
      }

      ಯದಿ (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, ಏತತ್, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    ಯದಿ (!ret || non_zero_index == -1) return_trace (ಅಸತ್ಯ);

    //prune empty trailing ruleSets
    --index;
    ಯಾವತ್ (index > non_zero_index)
    {
      out->ruleSet.ತೆಗೆ ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (ತಾರ್ಕಿಕ (out->ruleSet));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && classDef.sanitize (c, ಏತತ್) && ruleSet.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ಸಂರಚನಾ ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverageZ[0]).intersects (glyphs))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      ಏತತ್
    };
    ಪ್ರತ್ಯಾಗಮ context_intersects (glyphs,
			       glyph_ಎಣಿಕೆ_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverageZ[0]).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ಏತತ್,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_ಎಣಿಕೆ_2, (ಧ್ರುವ HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_ಎಣಿಕೆ_2, lookupRecord,
			   ಏತತ್,
			   coverageZ[0], lookup_context);

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverageZ[0]).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    ಸಂರಚನಾ ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ಏತತ್
    };
    context_closure_lookup (c,
			    glyph_ಎಣಿಕೆ_2, coverageZ.arrayZ + 1,
			    lookup_ಎಣಿಕೆ_2, lookupRecord,
			    0, lookup_context);

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;
    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    recurse_lookups (c, lookup_ಎಣಿಕೆ_2, lookupRecord);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+coverageZ[0]).collect_coverage (c->ಒಳಹರಿವು);

    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    ಸಂರಚನಾ ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      ಏತತ್
    };

    context_collect_glyphs_lookup (c,
				   glyph_ಎಣಿಕೆ_2, coverageZ.arrayZ + 1,
				   lookup_ಎಣಿಕೆ_2, lookupRecord,
				   lookup_context);
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    ಯದಿ (glyph_ಎಣಿಕೆ_2 <= 1)
      *digest = hb_set_digest_t::full ();
    ಅನ್ಯಥಾ
      (ಏತತ್+coverageZ[1]).collect_coverage (digest);
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    ಸಂರಚನಾ ContextApplyLookupContext lookup_context = {
      {match_coverage},
      ಏತತ್
    };
    ಪ್ರತ್ಯಾಗಮ context_would_apply_lookup (c,
				       glyph_ಎಣಿಕೆ_2, coverageZ.arrayZ + 1,
				       lookup_ಎಣಿಕೆ_2, lookupRecord,
				       lookup_context);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverageZ[0]; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    ಸಂರಚನಾ ContextApplyLookupContext lookup_context = {
      {match_coverage},
      ಏತತ್
    };
    return_trace (context_apply_lookup (c,
					glyph_ಎಣಿಕೆ_2,
					coverageZ.arrayZ + 1,
					lookup_ಎಣಿಕೆ_2, lookupRecord,
					lookup_context));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->format = format;
    out->glyph_ಎಣಿಕೆ_2 = glyph_ಎಣಿಕೆ_2;

    auto coverages = coverageZ.as_array (glyph_ಎಣಿಕೆ_2);

    ಪ್ರತಿ (ಧ್ರುವ auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      ಯದಿ (unlikely (!o)) return_trace (ಅಸತ್ಯ);
      ಯದಿ (!o->serialize_subset (c, offset, ಏತತ್)) return_trace (ಅಸತ್ಯ);
    }

    ಧ್ರುವ auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    ಧ್ರುವ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned ಎಣಿಕೆ = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_ಎಣಿಕೆ_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_ಎಣಿಕೆ_2, ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = glyph_ಎಣಿಕೆ_2;
    ಯದಿ (unlikely (!ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ); /* We want to access coverageZ[0] freely. */
    ಯದಿ (unlikely (!c->check_array (coverageZ.arrayZ, ಎಣಿಕೆ))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!coverageZ[i].sanitize (c, ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಧ್ರುವ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_ಎಣಿಕೆ_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_ಎಣಿಕೆ_2)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_ಎಣಿಕೆ_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_ಎಣಿಕೆ_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

ಸಂರಚನಾ Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
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

ಸಂರಚನಾ ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ಧ್ರುವ ಶೂನ್ಯ *intersects_data[3];
  ಶೂನ್ಯ *intersects_cache[3];
  ಶೂನ್ಯ *intersected_glyphs_cache;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ChainContextClosureLookupContext ChainContextDependLookupContext;

ಸಂರಚನಾ ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ಧ್ರುವ ಶೂನ್ಯ *collect_data[3];
};

ಸಂರಚನಾ ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ಧ್ರುವ ಶೂನ್ಯ *match_data[3];
  get_ಮೌಲ್ಯ_func_t get_ಒಳಹರಿವು_ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ chain_context_intersects (ಧ್ರುವ hb_set_t *glyphs,
					     unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_ಎಣಿಕೆ,
					     glyph_data_t backtrack,
					     unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
					     glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
					     unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_ಎಣಿಕೆ,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  ತಾರ್ಕಿಕ bt_ok = array_is_subset_of (glyphs,
			     backtrack_ಎಣಿಕೆ, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  ತಾರ್ಕಿಕ in_ok = array_is_subset_of (glyphs,
			     ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0, ಒಳಹರಿವು,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  ತಾರ್ಕಿಕ la_ok = array_is_subset_of (glyphs,
			     lookahead_ಎಣಿಕೆ, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  ಪ್ರತ್ಯಾಗಮ bt_ok && in_ok && la_ok;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_ಎಣಿಕೆ,
						 glyph_data_t backtrack,
						 unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
						 glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
						 unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_ಎಣಿಕೆ,
						 glyph_data_t lookahead,
						 unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
						 ಧ್ರುವ LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  ಯದಿ (chain_context_intersects (c->glyphs,
				backtrack_ಎಣಿಕೆ, backtrack,
				ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
				lookahead_ಎಣಿಕೆ, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
		     lookup_ಎಣಿಕೆ_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_ಎಣಿಕೆ,
						glyph_data_t backtrack,
						unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
						glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
						unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_ಎಣಿಕೆ,
						glyph_data_t lookahead,
						unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
						ಧ್ರುವ LookupRecord lookupRecord[],
						ಧ್ರುವ ಶೂನ್ಯ *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  ಯದಿ (!chain_context_intersects (c->glyphs,
				 backtrack_ಎಣಿಕೆ, backtrack,
				 ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
				 lookahead_ಎಣಿಕೆ, lookahead,
				 lookup_context))
    ಪ್ರತ್ಯಾಗಮ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ಯದಿ (unlikely (!scratch))
    ಪ್ರತ್ಯಾಗಮ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  ತಾರ್ಕಿಕ context_sets_cached = context_depend_sets_are_cached (
    c, ಒಳಹರಿವು_ಎಣಿಕೆ, lookup_ಎಣಿಕೆ_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  ಯದಿ (unlikely (!c->depend_data->successful))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
				    lookup_ಎಣಿಕೆ_2, lookupRecord,
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
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ಧ್ರುವ hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ಧ್ರುವ hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  ಪ್ರತಿ (unsigned i = 0; i < backtrack_ಎಣಿಕೆ; i++)
  {
    ಧ್ರುವ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], ಅಸತ್ಯ, &position_scratch);
    ಯದಿ (unlikely (!pos_glyphs))
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!pos_glyphs->is_empty ())
    {
      ಯದಿ (unlikely (!backtrack_sets.ತಳ್ಳು_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        ಪ್ರತ್ಯಾಗಮ;
      }
    }
  }

  /* Extract lookahead glyphs */
  ಪ್ರತಿ (unsigned i = 0; i < lookahead_ಎಣಿಕೆ; i++)
  {
    ಧ್ರುವ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], ಅಸತ್ಯ, &position_scratch);
    ಯದಿ (unlikely (!pos_glyphs))
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!pos_glyphs->is_empty ())
    {
      ಯದಿ (unlikely (!lookahead_sets.ತಳ್ಳು_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        ಪ್ರತ್ಯಾಗಮ;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  ಪ್ರತಿ (ಧ್ರುವ hb_set_t *back_set : backtrack_sets)
  {
    ಯದಿ (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } ಅನ್ಯಥಾ ಯದಿ (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      ಯದಿ (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ಪ್ರತ್ಯಾಗಮ;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  ಪ್ರತಿ (ಧ್ರುವ hb_set_t *look_set : lookahead_sets)
  {
    ಯದಿ (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } ಅನ್ಯಥಾ ಯದಿ (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      ಯದಿ (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ಪ್ರತ್ಯಾಗಮ;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  ಯದಿ (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ಧ್ರುವ hb_set_t *> &ಒಳಹರಿವು_position_glyphs = scratch->ಒಳಹರಿವು_position_glyphs;

  ಯದಿ (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ಧ್ರುವ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      ಸತ್ಯ, &position_scratch, &scratch->compact_workspace);
    ಯದಿ (unlikely (!pos0_glyphs ||
		  !ಒಳಹರಿವು_position_glyphs.ತಳ್ಳು_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* Positions 1+ (Input array) */
    ಪ್ರತಿ (unsigned i = 0; i + 1 < ಒಳಹರಿವು_ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	ಒಳಹರಿವು[i], ಅಸತ್ಯ, &position_scratch);
      ಯದಿ (unlikely (!pos_glyphs ||
		    !ಒಳಹರಿವು_position_glyphs.ತಳ್ಳು_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	ಪ್ರತ್ಯಾಗಮ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
				  lookup_ಎಣಿಕೆ_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &ಒಳಹರಿವು_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_ಎಣಿಕೆ,
							glyph_data_t backtrack,
							unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
							glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
							unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_ಎಣಿಕೆ,
							glyph_data_t lookahead,
							unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
							ಧ್ರುವ LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_ಎಣಿಕೆ, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->ಒಳಹರಿವು,
		 ಒಳಹರಿವು_ಎಣಿಕೆ ? ಒಳಹರಿವು_ಎಣಿಕೆ - 1 : 0, ಒಳಹರಿವು,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_ಎಣಿಕೆ, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_ಎಣಿಕೆ_2, lookupRecord);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_ಎಣಿಕೆ,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
						     glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
						     unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_ಎಣಿಕೆ,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2 HB_UNUSED,
						     ಧ್ರುವ LookupRecord lookupRecord[] HB_UNUSED,
						     ಧ್ರುವ ChainContextApplyLookupContext &lookup_context)
{
  ಪ್ರತ್ಯಾಗಮ (c->zero_context ? !backtrack_ಎಣಿಕೆ && !lookahead_ಎಣಿಕೆ : ಸತ್ಯ)
      && would_match_ಒಳಹರಿವು (c,
			    ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned ಪೂರ್ಣಾಙ್ಕ backtrack_ಎಣಿಕೆ,
					       glyph_data_t backtrack,
					       unsigned ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_ಎಣಿಕೆ, /* Including the first glyph (not matched) */
					       glyph_data_t ಒಳಹರಿವು, /* Array of input values--start with second glyph */
					       unsigned ಪೂರ್ಣಾಙ್ಕ lookahead_ಎಣಿಕೆ,
					       glyph_data_t lookahead,
					       unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ_2,
					       ಧ್ರುವ LookupRecord lookupRecord[],
					       ಧ್ರುವ ChainContextApplyLookupContext &lookup_context)
{
  ಯದಿ (unlikely (ಒಳಹರಿವು_ಎಣಿಕೆ > HB_MAX_CONTEXT_LENGTH)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  ತಾರ್ಕಿಕ ret = ಸತ್ಯ;
  ಯದಿ (!(match_ಒಳಹರಿವು (c,
		     ಒಳಹರಿವು_ಎಣಿಕೆ, ಒಳಹರಿವು,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_ಎಣಿಕೆ, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಯದಿ (!match_backtrack (c,
			backtrack_ಎಣಿಕೆ, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		ಒಳಹರಿವು_ಎಣಿಕೆ,
		lookup_ಎಣಿಕೆ_2, lookupRecord,
		match_end);

  ಪ್ರತ್ಯಾಗಮ ret;
}

template <typename Types>
ಸಂರಚನಾ ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend ಸಂರಚನಾ ChainRuleSetOf;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಪ್ರತ್ಯಾಗಮ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ಒಳಹರಿವು.lenP1, ಒಳಹರಿವು.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 ಒಳಹರಿವು.lenP1, ಒಳಹರಿವು.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ಏತತ್,
				 value,
				 lookup_context);
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ಒಳಹರಿವು.lenP1, ಒಳಹರಿವು.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!intersects (c->glyphs, lookup_context)) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ಒಳಹರಿವು.lenP1, ಒಳಹರಿವು.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest,
			      ಧ್ರುವ ClassDef *ಒಳಹರಿವು_class_def = nullptr,
			      ಧ್ರುವ ClassDef *lookahead_class_def = nullptr) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);

    unsigned second;
    ಧ್ರುವ ClassDef *class_def;
    ಯದಿ (ಒಳಹರಿವು.lenP1 > 1)
    {
      second = ಒಳಹರಿವು.arrayZ[0];
      class_def = ಒಳಹರಿವು_class_def;
    }
    ಅನ್ಯಥಾ ಯದಿ (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    ಅನ್ಯಥಾ
    {
      *digest = hb_set_digest_t::full ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (!class_def)
      digest->add (second);
    ಅನ್ಯಥಾ ಯದಿ (!second)
      *digest = hb_set_digest_t::full ();
    ಅನ್ಯಥಾ
      class_def->collect_class (digest, second);
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c,
		    ಧ್ರುವ ChainContextApplyLookupContext &lookup_context) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ಪ್ರತ್ಯಾಗಮ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ಒಳಹರಿವು.lenP1, ಒಳಹರಿವು.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c,
	      ಧ್ರುವ ChainContextApplyLookupContext &lookup_context) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ಒಳಹರಿವು.lenP1, ಒಳಹರಿವು.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ಧ್ರುವ
  {
    c->copy (len);
    ಪ್ರತಿ (ಧ್ರುವ auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ hb_map_t *lookup_map,
		  ಧ್ರುವ hb_map_t *backtrack_map,
		  ಧ್ರುವ hb_map_t *ಒಳಹರಿವು_map = nullptr,
		  ಧ್ರುವ hb_map_t *lookahead_map = nullptr) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಧ್ರುವ hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಯದಿ (ಒಳಹರಿವು_map) mapping = ಒಳಹರಿವು_map;
    serialize_array (c, ಒಳಹರಿವು.lenP1, + ಒಳಹರಿವು.iter ()
				     | hb_map (mapping));

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಯದಿ (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_ಎಣಿಕೆ_2 = c->embed (&(lookup.len));
    ಯದಿ (!lookup_ಎಣಿಕೆ_2) return_trace (ಅಸತ್ಯ);

    unsigned ಎಣಿಕೆ = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_ಎಣಿಕೆ_2, ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       ಧ್ರುವ hb_map_t *lookup_map,
	       ಧ್ರುವ hb_map_t *backtrack_map = nullptr,
	       ಧ್ರುವ hb_map_t *ಒಳಹರಿವು_map = nullptr,
	       ಧ್ರುವ hb_map_t *lookahead_map = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);

    ಯದಿ (!backtrack_map)
    {
      ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      ಯದಿ (!hb_all (backtrack, glyphset) ||
	  !hb_all (ಒಳಹರಿವು, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (ಅಸತ್ಯ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (ಒಳಹರಿವು, ಒಳಹರಿವು_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (ಅಸತ್ಯ);

      serialize (c->serializer, lookup_map, backtrack_map, ಒಳಹರಿವು_map, lookahead_map);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* Hyper-optimized sanitized because this is really hot. */
    ಯದಿ (unlikely (!backtrack.len.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಯದಿ (unlikely (!ಒಳಹರಿವು.lenP1.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಯದಿ (unlikely (!lookahead.len.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  ರಕ್ಷಿತ:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		ಒಳಹರಿವು_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
ಸಂರಚನಾ ChainRuleSetOf
{
  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([&] (ಧ್ರುವ ChainRuleType &_) { ಪ್ರತ್ಯಾಗಮ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ಶೂನ್ಯ collect_first_ಒಳಹರಿವು_classes (hb_ot_layout_ruleset_digest_t *digest) ಧ್ರುವ
  {
    digest->init ();
    ಪ್ರತಿ (unsigned i = 0; i < rule.len; i++)
    {
      ಧ್ರುವ ChainRuleType &r = ಏತತ್+rule.arrayZ[i];
      ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (r.ಒಳಹರಿವು_X)> (r.backtrack);
      ಯದಿ (ಒಳಹರಿವು.lenP1 <= 1)
      {
	digest->set_full ();
	ಪ್ರತ್ಯಾಗಮ;
      }
      digest->add (ಒಳಹರಿವು.arrayZ[0]);
    }
  }
  ಶೂನ್ಯ depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ಧ್ರುವ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  ಶೂನ್ಯ closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ಧ್ರುವ
  {
    ಯದಿ (unlikely (c->lookup_limit_exceeded ())) ಪ್ರತ್ಯಾಗಮ;

    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest,
			      ಧ್ರುವ ClassDef *ಒಳಹರಿವು_class_def = nullptr,
			      ಧ್ರುವ ClassDef *lookahead_class_def = nullptr) ಧ್ರುವ
  {
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([=] (ಧ್ರುವ ChainRuleType &_)
		{ _.collect_second_glyphs (digest, ಒಳಹರಿವು_class_def, lookahead_class_def); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c,
		    ಧ್ರುವ ChainContextApplyLookupContext &lookup_context) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (rule)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([&] (ಧ್ರುವ ChainRuleType &_) { ಪ್ರತ್ಯಾಗಮ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c,
	      ಧ್ರುವ ChainContextApplyLookupContext &lookup_context,
	      ಧ್ರುವ hb_ot_layout_ruleset_digest_t *digest = nullptr) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    unsigned ಅಂಕೆ_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    ಯದಿ (HB_OPTIMIZE_SIZE_VAL || ಅಂಕೆ_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (ಏತತ್))
      | hb_map ([&] (ಧ್ರುವ ChainRuleType &_) { ಪ್ರತ್ಯಾಗಮ _.apply (c, lookup_context); })
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
    ತಾರ್ಕಿಕ matched = skippy_iter.next (&unsafe_to1);
    ಯದಿ (likely (matched))
    {
      ಯದಿ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ಗಚ್ಛ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ಯದಿ (digest && lookup_context.get_ಒಳಹರಿವು_ಮೌಲ್ಯ &&
	  !digest->may_have (lookup_context.get_ಒಳಹರಿವು_ಮೌಲ್ಯ (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (ಅಸತ್ಯ);
	}
    }
    ಅನ್ಯಥಾ
    {
      ತಾರ್ಕಿಕ unsafe_to_concat = ಅಸತ್ಯ;
      ತಾರ್ಕಿಕ ret =
      + hb_iter (rule)
      | hb_map (hb_add (ಏತತ್))
      | hb_filter ([&] (ಧ್ರುವ ChainRuleType &_)
		   {
		     ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (_.ಒಳಹರಿವು_X)> (_.backtrack);
		     ಧ್ರುವ auto &lookahead = StructAfter<decltype (_.lookaheadX)> (ಒಳಹರಿವು);
		     ಯದಿ (ಒಳಹರಿವು.lenP1 <= 1 && lookahead.len == 0) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
		     unsafe_to_concat = ಸತ್ಯ;
		     ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
		   })
      | hb_map ([&] (ಧ್ರುವ ChainRuleType &_) { ಪ್ರತ್ಯಾಗಮ _.apply (c, lookup_context); })
      | hb_any
      ;
      ಯದಿ (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ಯದಿ (likely (matched))
    {
      ಯದಿ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ಗಚ್ಛ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_ಒಳಹರಿವು = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *ಒಳಹರಿವು_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_rules; i++)
    {
      ಧ್ರುವ auto &r = ಏತತ್+rule.arrayZ[i];

      ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (r.ಒಳಹರಿವು_X)> (r.backtrack);
      ಧ್ರುವ auto &lookahead = StructAfter<decltype (r.lookaheadX)> (ಒಳಹರಿವು);

      unsigned lenP1 = ಒಳಹರಿವು.lenP1;
      ಯದಿ (lenP1 > 1 ?
	   (!match_ಒಳಹರಿವು ||
	    match_ಒಳಹರಿವು (*first, ಒಳಹರಿವು.arrayZ[0], ಒಳಹರಿವು_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        ಯದಿ (!second ||
	    (lenP1 > 2 ?
	     (!match_ಒಳಹರಿವು ||
	      match_ಒಳಹರಿವು (*second, ಒಳಹರಿವು.arrayZ[1], ಒಳಹರಿವು_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  ಯದಿ (r.apply (c, lookup_context))
	  {
	    ಯದಿ (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (ಸತ್ಯ);
	  }
	}
	ಅನ್ಯಥಾ
	  unsafe_to = unsafe_to2;
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	ಯದಿ (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  ಪ್ರತಿ (; i + 1 < ಅಂಕೆ_rules; i++)
	  {
	    ಧ್ರುವ auto &r2 = ಏತತ್+rule.arrayZ[i + 1];
	    ಧ್ರುವ auto &input2 = StructAfter<decltype (r2.ಒಳಹರಿವು_X)> (r2.backtrack);
	    ಯದಿ (input2.lenP1 <= 1 || input2.arrayZ[0] != ಒಳಹರಿವು.arrayZ[0])
	      ಭಙ್ಗ;
	  }
	}
      }
    }
    ಯದಿ (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ಅಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       ಧ್ರುವ hb_map_t *lookup_map,
	       ಧ್ರುವ hb_map_t *backtrack_klass_map = nullptr,
	       ಧ್ರುವ hb_map_t *ಒಳಹರಿವು_klass_map = nullptr,
	       ಧ್ರುವ hb_map_t *lookahead_klass_map = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : rule)
    {
      ಯದಿ (!_) ಅನುವರ್ತ;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      ಯದಿ (unlikely (!o)) ಅನುವರ್ತ;

      ಯದಿ (!o->serialize_subset (c, _, ಏತತ್,
				lookup_map,
				backtrack_klass_map,
				ಒಳಹರಿವು_klass_map,
				lookahead_klass_map))
      {
	out->rule.ತೆಗೆ ();
	c->serializer->revert (o_snap);
      }
    }

    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (out->rule);
    ಯದಿ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (rule.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ಸಂರಚನಾ ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (ಪರಿಮಾಣ (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
ಸಂರಚನಾ ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
ಸಂರಚನಾ ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    ಪ್ರತ್ಯಾಗಮ
    + hb_zip (ಏತತ್+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([&] (ಧ್ರುವ ChainRuleSet &_) { ಪ್ರತ್ಯಾಗಮ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (ಏತತ್+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ಪ್ರತ್ಯಾಗಮ c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ಧ್ರುವ hb_pair_t<hb_codepoint_t, unsigned> _) { ಪ್ರತ್ಯಾಗಮ hb_pair_t<unsigned, ಧ್ರುವ ChainRuleSet&> (_.first, ಏತತ್+ruleSet[_.second]); })
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (ಏತತ್+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ಪ್ರತ್ಯಾಗಮ c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ಧ್ರುವ hb_pair_t<hb_codepoint_t, unsigned> _) { ಪ್ರತ್ಯಾಗಮ hb_pair_t<unsigned, ಧ್ರುವ ChainRuleSet&> (_.first, ಏತತ್+ruleSet[_.second]); })
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ
  {
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (ಏತತ್+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು);

    ಸಂರಚನಾ ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([digest] (ಧ್ರುವ ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ ChainRuleSet &rule_set = ಏತತ್+ruleSet[(ಏತತ್+coverage).get_coverage (c->glyphs[0])];
    ಸಂರಚನಾ ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    ಪ್ರತ್ಯಾಗಮ rule_set.would_apply (c, lookup_context);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ ChainRuleSet &rule_set = ಏತತ್+ruleSet[index];
    ಸಂರಚನಾ ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    ಧ್ರುವ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, ಏತತ್, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && ruleSet.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
ಸಂರಚನಾ ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (glyphs))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (ಏತತ್+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    ಒಳಹರಿವು_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_enumerate
    | hb_map ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ ChainRuleSet &> p)
	      { ಪ್ರತ್ಯಾಗಮ ಒಳಹರಿವು_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ಪ್ರತ್ಯಾಗಮ ಒಳಹರಿವು_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ಧ್ರುವ ChainRuleSet& chainrule_set = ಏತತ್+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs)) ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ಪ್ರತ್ಯಾಗಮ ಒಳಹರಿವು_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ಧ್ರುವ ChainRuleSet& chainrule_set = ಏತತ್+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { ಪ್ರತ್ಯಾಗಮ ಒಳಹರಿವು_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ಧ್ರುವ ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು);

    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

    ಸಂರಚನಾ ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([digest, &ಒಳಹರಿವು_class_def, &lookahead_class_def] (ಧ್ರುವ ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &ಒಳಹರಿವು_class_def, &lookahead_class_def); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

    unsigned ಪೂರ್ಣಾಙ್ಕ index = ಒಳಹರಿವು_class_def.get_class (c->glyphs[0]);
    ಧ್ರುವ ChainRuleSet &rule_set = ಏತತ್+ruleSet[index];
    ಸಂರಚನಾ ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def}
    };
    ಪ್ರತ್ಯಾಗಮ rule_set.would_apply (c, lookup_context);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  unsigned cache_cost () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+ಒಳಹರಿವು_Class_Def).cost () + (ಏತತ್+lookaheadClassDef).cost ();
  }
  ಸ್ಥಿರ ತಾರ್ಕಿಕ cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ಪ್ರತ್ಯಾಗಮ context_cache_func (c, op);
  }

  ಸಂರಚನಾ external_cache_t
  {
    hb_ot_layout_ದ್ವಿಮಾನ_cache_t coverage;
    hb_ot_layout_mapping_cache_t ಒಳಹರಿವು_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  ಶೂನ್ಯ *external_cache_create () ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = ruleSet.len;
    unsigned size = ಪರಿಮಾಣ (external_cache_t) -
		    HB_VAR_ARRAY * ಪರಿಮಾಣ (hb_ot_layout_ruleset_digest_t) +
		    ಎಣಿಕೆ * ಪರಿಮಾಣ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ಯದಿ (likely (cache))
    {
      cache->coverage.clear ();
      cache->ಒಳಹರಿವು_class.clear ();
      cache->lookahead_class.clear ();
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	(ಏತತ್+ruleSet[i]).collect_first_ಒಳಹರಿವು_classes (&cache->rule_sets[i]);
    }
    ಪ್ರತ್ಯಾಗಮ cache;
  }
  ತಾರ್ಕಿಕ apply_cached (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _apply (c, ಸತ್ಯ, external_cache); }
  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ _apply (c, ಅಸತ್ಯ, external_cache); }
  ತಾರ್ಕಿಕ _apply (hb_ot_apply_context_t *c, ತಾರ್ಕಿಕ cached, ಶೂನ್ಯ *external_cache) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage_ದ್ವಿಮಾನ (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ಅನ್ಯಥಾ
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ ClassDef &backtrack_class_def = ಏತತ್+backtrackClassDef;
    ಧ್ರುವ ClassDef &ಒಳಹರಿವು_class_def = ಏತತ್+ಒಳಹರಿವು_Class_Def;
    ಧ್ರುವ ClassDef &lookahead_class_def = ಏತತ್+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t ಒಳಹರಿವು_cache_data = {
      &ಒಳಹರಿವು_class_def,
      cache ? &cache->ಒಳಹರಿವು_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ಧ್ರುವ ಶೂನ್ಯ *ಒಳಹರಿವು_match_data = cached
      ? static_cast<ಧ್ರುವ ಶೂನ್ಯ *> (&ಒಳಹರಿವು_class_def)
      : static_cast<ಧ್ರುವ ಶೂನ್ಯ *> (&ಒಳಹರಿವು_cache_data);
    ಧ್ರುವ ಶೂನ್ಯ *lookahead_match_data = cached
      ? static_cast<ಧ್ರುವ ಶೂನ್ಯ *> (&lookahead_class_def)
      : static_cast<ಧ್ರುವ ಶೂನ್ಯ *> (&lookahead_cache_data);
    ತಾರ್ಕಿಕ backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ಧ್ರುವ ಶೂನ್ಯ *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<ಧ್ರುವ ಶೂನ್ಯ *> (&lookahead_cache_data)
      : static_cast<ಧ್ರುವ ಶೂನ್ಯ *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    ಸಂರಚನಾ ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       ಒಳಹರಿವು_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_ಮೌಲ್ಯ : get_class_cache_ಮೌಲ್ಯ
#ಅನ್ಯಥಾ
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &ಒಳಹರಿವು_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_ಮೌಲ್ಯ : get_class_ಮೌಲ್ಯ
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (ಒಳಹರಿವು_class_def, c->buffer->cur())
         : ಒಳಹರಿವು_class_def.get_class (c->buffer->cur().codepoint, ಒಳಹರಿವು_cache_data.cache);
#ಅನ್ಯಥಾ
    index = cached
         ? get_class_cached2 (ಒಳಹರಿವು_class_def, c->buffer->cur())
          : ಒಳಹರಿವು_class_def.get_class (c->buffer->cur().codepoint);
#endif
    ಧ್ರುವ ChainRuleSet &rule_set = ಏತತ್+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಧ್ರುವ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#ಅನ್ಯಥಾ
    ಧ್ರುವ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, ಏತತ್);

    hb_map_t backtrack_klass_map;
    hb_map_t ಒಳಹರಿವು_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, ಏತತ್, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->ಒಳಹರಿವು_Class_Def.serialize_subset (c, ಒಳಹರಿವು_Class_Def, ಏತತ್, &ಒಳಹರಿವು_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, ಏತತ್, &lookahead_klass_map);

    ಯದಿ (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   ಒಳಹರಿವು_klass_map,
						   lookahead_klass_map)))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (ಏತತ್+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (ಏತತ್+ಒಳಹರಿವು_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ಪೂರ್ಣಾಙ್ಕ non_zero_index = -1, index = 0;
    ತಾರ್ಕಿಕ ret = ಸತ್ಯ;
    ಧ್ರುವ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (ಒಳಹರಿವು_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      ಯದಿ (unlikely (!o))
      {
	ret = ಅಸತ್ಯ;
	ಭಙ್ಗ;
      }
      ಯದಿ (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, ಏತತ್,
			       lookup_map,
			       &backtrack_klass_map,
			       &ಒಳಹರಿವು_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    ಯದಿ (!ret || non_zero_index == -1) return_trace (ಅಸತ್ಯ);

    // prune empty trailing ruleSets
    ಯದಿ (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (ತಾರ್ಕಿಕ (out->ruleSet));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) &&
		  backtrackClassDef.sanitize (c, ಏತತ್) &&
		  ಒಳಹರಿವು_Class_Def.sanitize (c, ಏತತ್) &&
		  lookaheadClassDef.sanitize (c, ಏತತ್) &&
		  ruleSet.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		ಒಳಹರಿವು_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
ಸಂರಚನಾ ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);

    ಯದಿ (!(ಏತತ್+ಒಳಹರಿವು[0]).intersects (glyphs))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {ಏತತ್, ಏತತ್, ಏತತ್}
    };
    ಪ್ರತ್ಯಾಗಮ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ಒಳಹರಿವು.len, ಒಳಹರಿವು.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);

    ಯದಿ (!(ಏತತ್+ಒಳಹರಿವು[0]).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs))
      ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ಏತತ್, ಏತತ್, ಏತತ್},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ಧ್ರುವ HBUINT16 *) backtrack.arrayZ,
				 ಒಳಹರಿವು.len, (ಧ್ರುವ HBUINT16 *) ಒಳಹರಿವು.arrayZ + 1,
				 lookahead.len, (ಧ್ರುವ HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ಏತತ್,
				 ಒಳಹರಿವು[0], lookup_context);

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);

    ಯದಿ (!(ಏತತ್+ಒಳಹರಿವು[0]).intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    hb_set_t* cur_active_glyphs = c->ತಳ್ಳು_cur_active_glyphs ();
    ಯದಿ (unlikely (!cur_active_glyphs))
      ಪ್ರತ್ಯಾಗಮ;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ಸಂರಚನಾ ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ಏತತ್, ಏತತ್, ಏತತ್}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ಒಳಹರಿವು.len, ಒಳಹರಿವು.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->ತೆಗೆ_cur_done_glyphs ();
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!intersects (c->glyphs))
      ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);

    (ಏತತ್+ಒಳಹರಿವು[0]).collect_coverage (c->ಒಳಹರಿವು);

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    ಸಂರಚನಾ ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {ಏತತ್, ಏತತ್, ಏತತ್}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ಒಳಹರಿವು.len, ಒಳಹರಿವು.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಯದಿ (ಒಳಹರಿವು.len > 1)
      (ಏತತ್+ಒಳಹರಿವು[1]).collect_coverage (digest);
    ಅನ್ಯಥಾ ಯದಿ (lookahead.len)
      (ಏತತ್+lookahead[0]).collect_coverage (digest);
    ಅನ್ಯಥಾ
      *digest = hb_set_digest_t::full ();
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ಸಂರಚನಾ ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {ಏತತ್, ಏತತ್, ಏತತ್}
    };
    ಪ್ರತ್ಯಾಗಮ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ಒಳಹರಿವು.len, ಒಳಹರಿವು.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ
  {
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+ಒಳಹರಿವು[0];
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);

    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+ಒಳಹರಿವು[0]).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ಸಂರಚನಾ ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {ಏತತ್, ಏತತ್, ಏತತ್}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ಒಳಹರಿವು.len, ಒಳಹರಿವು.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ಧ್ರುವ ಶೂನ್ಯ* base) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    ಯದಿ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      ಯದಿ (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    ಯದಿ (unlikely (!c->serializer->embed (ಏತತ್->format))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!serialize_coverage_offsets (c, backtrack.iter (), ಏತತ್))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಯದಿ (!serialize_coverage_offsets (c, ಒಳಹರಿವು.iter (), ಏತತ್))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಯದಿ (!serialize_coverage_offsets (c, lookahead.iter (), ಏತತ್))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ಧ್ರುವ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_ಎಣಿಕೆ_2 = c->serializer->copy<HBUINT16> (lookup.len);
    ಯದಿ (!lookup_ಎಣಿಕೆ_2) return_trace (ಅಸತ್ಯ);

    unsigned ಎಣಿಕೆ = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_ಎಣಿಕೆ_2, ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!backtrack.sanitize (c, ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &ಒಳಹರಿವು = StructAfter<decltype (ಒಳಹರಿವು_X)> (backtrack);
    ಯದಿ (unlikely (!ಒಳಹರಿವು.sanitize (c, ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಯದಿ (unlikely (!ಒಳಹರಿವು.len)) return_trace (ಅಸತ್ಯ); /* To be consistent with Context. */
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (ಒಳಹರಿವು);
    ಯದಿ (unlikely (!lookahead.sanitize (c, ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		ಒಳಹರಿವು_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (10);
};

ಸಂರಚನಾ ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }		format;	/* Format identifier */
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
ಸಂರಚನಾ ExtensionFormat1
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ extensionLookupType; }

  template <typename X>
  ಧ್ರುವ X& get_subtable () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್ + reinterpret_cast<ಧ್ರುವ Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, ಏತತ್))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ಧ್ರುವ auto& src_offset =
        reinterpret_cast<ಧ್ರುವ Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, ಏತತ್, get_type ()));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
ಸಂರಚನಾ Extension
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_type () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_type ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }
  template <typename X>
  ಧ್ರುವ X& get_subtable () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.template get_subtable<typename T::SubTable> ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.subset (c);
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ c->default_return_ಮೌಲ್ಯ ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

ಸಂರಚನಾ hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  ಸ್ಥಿರ hb_ot_layout_lookup_accelerator_t *create (ಧ್ರುವ TLookup &lookup)
  {
    unsigned ಎಣಿಕೆ = lookup.get_subtable_ಎಣಿಕೆ ();

    unsigned size = ಪರಿಮಾಣ (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * ಪರಿಮಾಣ (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    ಎಣಿಕೆ * ಪರಿಮಾಣ (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    ಯದಿ (unlikely (!thiz))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    ಯಾವತ್ (ಎಣಿಕೆ && !thiz->subtables[ಎಣಿಕೆ - 1].apply_func)
      ಎಣಿಕೆ--;

    thiz->digest.init ();
    ಪ್ರತಿ (auto& subtable : hb_iter (thiz->subtables, ಎಣಿಕೆ))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->ಎಣಿಕೆ = ಎಣಿಕೆ;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    ಪ್ರತ್ಯಾಗಮ thiz;
  }

  ಶೂನ್ಯ fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ತಾರ್ಕಿಕ use_cache) ಧ್ರುವ
  {
    ಯದಿ (ಎಣಿಕೆ == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      ಯದಿ (use_cache)
	ಪ್ರತ್ಯಾಗಮ subtables[0].apply_cached_no_digest (c);
#endif
      ಪ್ರತ್ಯಾಗಮ subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಯದಿ (use_cache)
    {
      ಪ್ರತ್ಯಾಗಮ
      + hb_iter (hb_iter (subtables, ಎಣಿಕೆ))
      | hb_map ([&c] (ಧ್ರುವ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ಪ್ರತ್ಯಾಗಮ _.apply_cached (c); })
      | hb_any
      ;
    }
    ಅನ್ಯಥಾ
#endif
    {
      ಪ್ರತ್ಯಾಗಮ
      + hb_iter (hb_iter (subtables, ಎಣಿಕೆ))
      | hb_map ([&c] (ಧ್ರುವ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ಪ್ರತ್ಯಾಗಮ _.apply (c); })
      | hb_any
      ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ cache_enter (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಪ್ರತ್ಯಾಗಮ subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
  }
  ಶೂನ್ಯ cache_leave (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  ಗುಪ್ತ:
  unsigned ಎಣಿಕೆ = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ಸಂರಚನಾ GSUBGPOS
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

  size_t get_size () ಧ್ರುವ
  {
    ಯದಿ (version.major != 1) ಪ್ರತ್ಯಾಗಮ version.static_size;

    ಪ್ರತ್ಯಾಗಮ min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#ಅನ್ಯಥಾ
	   0
#endif
	   ;
  }

  ಧ್ರುವ ScriptList &get_script_list () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u && scriptList2) ಪ್ರತ್ಯಾಗಮ ಏತತ್ + scriptList2;
#endif
    ಪ್ರತ್ಯಾಗಮ ಏತತ್ + scriptList;
  }
  ಧ್ರುವ FeatureList &get_feature_list () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u && featureList2) ಪ್ರತ್ಯಾಗಮ ಏತತ್ + featureList2;
#endif
    ಪ್ರತ್ಯಾಗಮ ಏತತ್ + featureList;
  }
  template <typename TLookup>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (ಏತತ್, get_size ())))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ತಾರ್ಕಿಕ script_list_ok = scriptList.sanitize (c, ಏತತ್);
    ತಾರ್ಕಿಕ feature_list_ok = featureList.sanitize (c, ಏತತ್);
    ತಾರ್ಕಿಕ lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, ಏತತ್);

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u)
    {
      ಯದಿ (scriptList2) script_list_ok = scriptList2.sanitize (c, ಏತತ್);
      ಯದಿ (featureList2) feature_list_ok = featureList2.sanitize (c, ಏತತ್);
      ಯದಿ (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, ಏತತ್);
    }
#endif

    ಯದಿ (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (ಅಸತ್ಯ);

#ifndef HB_NO_VAR
    ಯದಿ (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, ಏತತ್, &get_feature_list ()))))
      return_trace (ಅಸತ್ಯ);
#endif

    return_trace (ಸತ್ಯ);
  }

  template <typename TLookup>
  ತಾರ್ಕಿಕ subset (hb_subset_layout_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (version.major != 1) return_trace (ಅಸತ್ಯ);

    auto *out = c->subset_context->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u)
    {
      ಯದಿ (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (ಅಸತ್ಯ);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      ಯದಿ (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 ಏತತ್, c);
      ಅನ್ಯಥಾ
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 ಏತತ್, c);

      ಯದಿ (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 ಏತತ್, c);
      ಅನ್ಯಥಾ
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 ಏತತ್, c);

      ಯದಿ (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, ಏತತ್, c);
      ಅನ್ಯಥಾ
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, ಏತತ್, c);
    }
    ಅನ್ಯಥಾ
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     ಏತತ್,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     ಏತತ್,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					ಏತತ್,
					c);
    }

#ifndef HB_NO_VAR
    ಯದಿ (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      ಯದಿ (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (ಅಸತ್ಯ);

      // if all axes are pinned all feature vars are dropped.
      ತಾರ್ಕಿಕ ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, ಏತತ್, c);
      ಯದಿ (!ret)
      {
        out->featureVars = 0;
	ಯದಿ (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (ಸತ್ಯ);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_lookup_ಎಣಿಕೆ () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u && lookupList2)
      ಪ್ರತ್ಯಾಗಮ (ಏತತ್ + lookupList2).len;
#endif
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್ + lookupList).len;
  }
  ಧ್ರುವ Lookup& get_lookup (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u && lookupList2)
      ಪ್ರತ್ಯಾಗಮ (ಏತತ್ + lookupList2)[i];
#endif
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್ + lookupList)[i];
  }
  ಧ್ರುವ FeatureVariations &get_feature_variations () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (version.to_int () >= 0x00010001u && hb_barrier ()) ? ಏತತ್+featureVars : Null (FeatureVariations);
  }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_script_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_script_list ().len; }
  ಧ್ರುವ Tag& get_script_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_script_list ().get_tag (i); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_script_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				unsigned ಪೂರ್ಣಾಙ್ಕ *script_ಎಣಿಕೆ /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_script_list ().get_tags (start_offset, script_ಎಣಿಕೆ, script_tags); }
  ಧ್ರುವ Script& get_script (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_script_list ()[i]; }
  ತಾರ್ಕಿಕ find_script_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_script_list ().find_index (tag, index); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				 unsigned ಪೂರ್ಣಾಙ್ಕ *feature_ಎಣಿಕೆ_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_feature_list ().get_tags (start_offset, feature_ಎಣಿಕೆ_2, feature_tags); }
  ಧ್ರುವ Feature& get_feature (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_feature_list ()[i]; }
  ತಾರ್ಕಿಕ find_feature_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_feature_list ().find_index (tag, index); }

  ತಾರ್ಕಿಕ find_variations_index (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_coords,
			      unsigned ಪೂರ್ಣಾಙ್ಕ *index,
			      ItemVarStoreInstancer *instancer) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ get_feature_variations ().find_index (coords, ಅಂಕೆ_coords, index, instancer);
  }
  ತಾರ್ಕಿಕ get_feature_variations_state (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
				     unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_coords,
				     unsigned ಪೂರ್ಣಾಙ್ಕ *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) ಧ್ರುವ
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
    find_variations_index (coords, ಅಂಕೆ_coords, index, instancer);
    ಪ್ರತ್ಯಾಗಮ get_feature_variations ().collect_lookup_conditions (
	coords,
	ಅಂಕೆ_coords,
	instancer,
	lookup_conditions);
#endif
  }
  ತಾರ್ಕಿಕ get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<ಧ್ರುವ unsigned> conditions,
                              hb_set_t *lookup_indexes) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  ಧ್ರುವ Feature& get_feature_variation (unsigned ಪೂರ್ಣಾಙ್ಕ feature_index,
					unsigned ಪೂರ್ಣಾಙ್ಕ variations_index) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    ಯದಿ (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      ಧ್ರುವ Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      ಯದಿ (feature)
	ಪ್ರತ್ಯಾಗಮ *feature;
    }
#endif
    ಪ್ರತ್ಯಾಗಮ get_feature (feature_index);
  }

  ಶೂನ್ಯ feature_variation_collect_lookups (ಧ್ರುವ hb_set_t *feature_indexes,
					  ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  ಶೂನ್ಯ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  ಶೂನ್ಯ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ಧ್ರುವ
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  ಶೂನ್ಯ closure_lookups (hb_face_t      *face,
			ಧ್ರುವ hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ಧ್ರುವ
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    ಪ್ರತಿ (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<ಧ್ರುವ TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  ಶೂನ್ಯ prune_langsys (ಧ್ರುವ hb_map_t *duplicate_feature_map,
                      ಧ್ರುವ hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ಧ್ರುವ
  {
    hb_prune_langsys_context_t c (ಏತತ್, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned ಎಣಿಕೆ = get_script_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned script_index = 0; script_index < ಎಣಿಕೆ; script_index++)
    {
      ಧ್ರುವ Tag& tag = get_script_tag (script_index);
      ಯದಿ (!layout_scripts->has (tag)) ಅನುವರ್ತ;
      ಧ್ರುವ Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  ಶೂನ್ಯ prune_features (ಧ್ರುವ hb_map_t *lookup_indices, /* IN */
		       ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       ಧ್ರುವ hb_hashmap_t<unsigned, ಧ್ರುವ Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    ಯದಿ (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    ಪ್ರತಿ (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      ಯದಿ (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        ಅನುವರ್ತ;


      ಧ್ರುವ Feature *f = &(get_feature (i));
      ಧ್ರುವ Feature** p = nullptr;
      ಯದಿ (feature_substitutes_map->has (i, &p))
        f = *p;

      ಯದಿ (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        ಅನುವರ್ತ;

      ಯದಿ (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  ಶೂನ್ಯ collect_name_ids (ಧ್ರುವ hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ಧ್ರುವ hb_hashmap_t<unsigned, ಧ್ರುವ Feature*> *feature_substitutes_map,
                         ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ಧ್ರುವ hb_set_t *catch_all_features,
                         ತಾರ್ಕಿಕ all_axes_pinned HB_UNUSED) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = get_feature_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned i = 0 ; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!feature_index_map->has (i)) ಅನುವರ್ತ;
      hb_tag_t tag = get_feature_tag (i);
      ಧ್ರುವ Feature *feature = &get_feature (i);
      ಧ್ರುವ Feature **substitute;
      ಯದಿ (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        ಯದಿ (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    ಯದಿ (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [ಏತತ್] (unsigned i) { ಪ್ರತ್ಯಾಗಮ get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = ಸತ್ಯ;
      ಏತತ್->table = sc.reference_table<T> (face);

      ಯದಿ (unlikely (ಏತತ್->table->is_blocklisted (ಏತತ್->table.get_blob (), face)))
      {
	hb_blob_destroy (ಏತತ್->table.get_blob ());
	ಏತತ್->table = hb_blob_get_empty ();
      }

      ಏತತ್->lookup_ಎಣಿಕೆ = table->get_lookup_ಎಣಿಕೆ ();

      ಏತತ್->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (ಏತತ್->lookup_ಎಣಿಕೆ, ಪರಿಮಾಣ (*accels));
      ಯದಿ (unlikely (!ಏತತ್->accels))
      {
	ಏತತ್->lookup_ಎಣಿಕೆ = 0;
	ಏತತ್->table.destroy ();
	ಏತತ್->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಏತತ್->lookup_ಎಣಿಕೆ; i++)
      {
	auto *accel = ಏತತ್->accels[i].get_relaxed ();
	ಯದಿ (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (ಏತತ್->accels);
      ಏತತ್->table.destroy ();
    }

    hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) ಧ್ರುವ
    {
      ಯದಿ (unlikely (lookup_index >= lookup_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      ಯದಿ (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	ಯದಿ (unlikely (!accel))
	  ಪ್ರತ್ಯಾಗಮ nullptr;

	ಯದಿ (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  ಗಚ್ಛ retry;
	}
      }

      ಪ್ರತ್ಯಾಗಮ accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned ಪೂರ್ಣಾಙ್ಕ lookup_ಎಣಿಕೆ;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  ರಕ್ಷಿತ:
  template <typename TLookup>
  ಧ್ರುವ Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ಧ್ರುವ Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  ಧ್ರುವ Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ಧ್ರುವ Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ಧ್ರುವ Offset16To<RecordListOfFeature>& subset_feature_list_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ಧ್ರುವ Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ಧ್ರುವ ಶೂನ್ಯ* get_lookup_list_field_offset () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (version.to_int () >= 0x00010002u && lookupList2) ಪ್ರತ್ಯಾಗಮ &lookupList2;
#endif
    ಪ್ರತ್ಯಾಗಮ &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ಧ್ರುವ OffsetIn& in,
				       ಧ್ರುವ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    ಯದಿ (in.is_null ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto *s = c->serializer;
    s->push ();
    ತಾರ್ಕಿಕ ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    ಯದಿ (ret) s->add_link (out, s->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ s->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ret;
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
