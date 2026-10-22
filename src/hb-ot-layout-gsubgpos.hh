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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_GSUBGPOS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_GSUBGPOS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-depend-data.hh"


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, ਤਾਰਕਿਕ>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { ਵਾਪਸ obj.intersects (ਇਹ->glyphs); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }

  ਅਚਲ hb_ਸੈੱਟ_t *glyphs;

  hb_intersects_context_t (ਅਚਲ hb_ਸੈੱਟ_t *glyphs_) :
                            glyphs (glyphs_) {}
};

ਬਣਤਰ hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, ਤਾਰਕਿਕ>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { ਵਾਪਸ obj.may_have_non_1to1 (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }
};

ਬਣਤਰ hb_ਘੇਰਾ_context_t :
       hb_dispatch_context_t<hb_ਘੇਰਾ_context_t>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ return_t (*recurse_func_t) (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *covered_seq_indicies, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { obj.ਘੇਰਾ (ਇਹ); ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਖਾਲੀ recurse (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *covered_seq_indicies, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (unlikely (nesting_level_ਖੱਬਾ == 0 || !recurse_func))
      ਵਾਪਸ;

    nesting_level_ਖੱਬਾ--;
    recurse_func (ਇਹ, lookup_ਸੂਚਕਾਂਕ, covered_seq_indicies, seq_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
    nesting_level_ਖੱਬਾ++;
  }

  ਖਾਲੀ reset_lookup_ਦੇਖੋ_ਗਿਣਤੀ ()
  { lookup_ਗਿਣਤੀ = 0; }

  ਤਾਰਕਿਕ lookup_ਹੱਦਬੰਦੀ_exceeded ()
  { ਵਾਪਸ lookup_ਗਿਣਤੀ > HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ; }

  ਤਾਰਕਿਕ should_ਦੇਖੋ_lookup (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (lookup_ਗਿਣਤੀ++ > HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ)
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (is_lookup_ਪੂਰਾ (lookup_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ is_lookup_ਪੂਰਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (unlikely (ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ->ਵਿੱਚ_error () ||
		  ਪੂਰਾ_lookups_glyph_ਸੈੱਟ->ਵਿੱਚ_error ()))
      ਵਾਪਸ ਸੱਚ;

    /* Have we visited this lookup with the current set of glyphs? */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੱਢੋ = glyphs->get_population ();
    ਜੇ (unlikely (ਕੱਢੋ == HB_MAP_ਮੁੱਲ_INVALID)) // Avoid sentinel interference.
      ਕੱਢੋ--;
    ਜੇ (ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ->get (lookup_ਸੂਚਕਾਂਕ) != ਕੱਢੋ)
    {
      ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ->ਸੈੱਟ (lookup_ਸੂਚਕਾਂਕ, ਕੱਢੋ);

      ਜੇ (!ਪੂਰਾ_lookups_glyph_ਸੈੱਟ->has (lookup_ਸੂਚਕਾਂਕ))
      {
	ਜੇ (unlikely (!ਪੂਰਾ_lookups_glyph_ਸੈੱਟ->ਸੈੱਟ (lookup_ਸੂਚਕਾਂਕ, hb::unique_ptr<hb_ਸੈੱਟ_t> {hb_ਸੈੱਟ_create ()})))
	  ਵਾਪਸ ਸੱਚ;
      }

      ਪੂਰਾ_lookups_glyph_ਸੈੱਟ->get (lookup_ਸੂਚਕਾਂਕ)->clear ();
    }

    hb_ਸੈੱਟ_t *covered_glyph_ਸੈੱਟ = ਪੂਰਾ_lookups_glyph_ਸੈੱਟ->get (lookup_ਸੂਚਕਾਂਕ);
    ਜੇ (unlikely (covered_glyph_ਸੈੱਟ->ਵਿੱਚ_error ()))
      ਵਾਪਸ ਸੱਚ;
    ਜੇ (ਮਾਪਾ_ਸਰਗਰਮ_glyphs ().is_subset (*covered_glyph_ਸੈੱਟ))
      ਵਾਪਸ ਸੱਚ;

    covered_glyph_ਸੈੱਟ->union_ (ਮਾਪਾ_ਸਰਗਰਮ_glyphs ());
    ਵਾਪਸ ਝੂਠ;
  }

  ਅਚਲ hb_ਸੈੱਟ_t& ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs () {
    ਜੇ (ਸਰਗਰਮ_glyphs_ਢੇਰ.ਲੰਮਾਈ <= 1)
      ਵਾਪਸ *glyphs;

    ਵਾਪਸ ਸਰਗਰਮ_glyphs_ਢੇਰ[ਸਰਗਰਮ_glyphs_ਢੇਰ.ਲੰਮਾਈ - 2];
  }

  ਅਚਲ hb_ਸੈੱਟ_t& ਮਾਪਾ_ਸਰਗਰਮ_glyphs ()
  {
    ਜੇ (!ਸਰਗਰਮ_glyphs_ਢੇਰ)
      ਵਾਪਸ *glyphs;

    ਵਾਪਸ ਸਰਗਰਮ_glyphs_ਢੇਰ.ਪੂਛ ();
  }

  hb_ਸੈੱਟ_t* ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ()
  {
    hb_ਸੈੱਟ_t *s = ਸਰਗਰਮ_glyphs_ਢੇਰ.push ();
    ਜੇ (unlikely (ਸਰਗਰਮ_glyphs_ਢੇਰ.ਵਿੱਚ_error ()))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ s;
  }

  ਤਾਰਕਿਕ ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ()
  {
    ਜੇ (!ਸਰਗਰਮ_glyphs_ਢੇਰ)
      ਵਾਪਸ ਝੂਠ;

    ਸਰਗਰਮ_glyphs_ਢੇਰ.ਕੱਢੋ ();
    ਵਾਪਸ ਸੱਚ;
  }

  hb_face_t *face;
  hb_ਸੈੱਟ_t *glyphs;
  hb_ਸੈੱਟ_t output[1];
  hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t> ਸਰਗਰਮ_glyphs_ਢੇਰ;
  recurse_func_t recurse_func = ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nesting_level_ਖੱਬਾ;

  hb_ਘੇਰਾ_context_t (hb_face_t *face_,
			hb_ਸੈੱਟ_t *glyphs_,
			hb_map_t *ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ_2,
			hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> *ਪੂਰਾ_lookups_glyph_ਸੈੱਟ_2,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nesting_level_ਖੱਬਾ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_ਖੱਬਾ (nesting_level_ਖੱਬਾ_2),
			  ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ (ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ_2),
			  ਪੂਰਾ_lookups_glyph_ਸੈੱਟ (ਪੂਰਾ_lookups_glyph_ਸੈੱਟ_2)
  {}

  ~hb_ਘੇਰਾ_context_t () { flush (); }

  ਖਾਲੀ ਸੈੱਟ_recurse_func (recurse_func_t func) { recurse_func = func; }

  ਖਾਲੀ flush ()
  {
    output->del_range (face->get_ਅੰਕ_glyphs (), HB_ਸੈੱਟ_ਮੁੱਲ_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    ਸਰਗਰਮ_glyphs_ਢੇਰ.ਕੱਢੋ ();
    ਸਰਗਰਮ_glyphs_ਢੇਰ.reset ();
  }

  ਨਿਜੀ:
  hb_map_t *ਪੂਰਾ_lookups_glyph_ਗਿਣਤੀ;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> *ਪੂਰਾ_lookups_glyph_ਸੈੱਟ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ = 0;
};



ਬਣਤਰ hb_ਘੇਰਾ_lookups_context_t :
       hb_dispatch_context_t<hb_ਘੇਰਾ_lookups_context_t>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ return_t (*recurse_func_t) (hb_ਘੇਰਾ_lookups_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { obj.ਘੇਰਾ_lookups (ਇਹ); ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਖਾਲੀ recurse (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (unlikely (nesting_level_ਖੱਬਾ == 0 || !recurse_func))
      ਵਾਪਸ;

    /* Return if new lookup was recursed to before. */
    ਜੇ (lookup_ਹੱਦਬੰਦੀ_exceeded ()
        || ਦੇਖਿਆ_lookups->ਵਿੱਚ_error ()
        || ਦੇਖਿਆ_lookups->has (lookup_ਸੂਚਕਾਂਕ))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      ਵਾਪਸ;

    nesting_level_ਖੱਬਾ--;
    recurse_func (ਇਹ, lookup_ਸੂਚਕਾਂਕ);
    nesting_level_ਖੱਬਾ++;
  }

  ਖਾਲੀ ਸੈੱਟ_lookup_ਦੇਖਿਆ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ)
  { ਦੇਖਿਆ_lookups->ਜੋੜੋ (lookup_ਸੂਚਕਾਂਕ); }

  ਖਾਲੀ ਸੈੱਟ_lookup_inactive (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ)
  { inactive_lookups->ਜੋੜੋ (lookup_ਸੂਚਕਾਂਕ); }

  ਤਾਰਕਿਕ lookup_ਹੱਦਬੰਦੀ_exceeded ()
  {
    ਤਾਰਕਿਕ ret = lookup_ਗਿਣਤੀ > HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ;
    ਜੇ (ret)
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "lookup visit count limit exceeded in lookup closure!");
    ਵਾਪਸ ret; }

  ਤਾਰਕਿਕ is_lookup_ਦੇਖਿਆ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (unlikely (lookup_ਗਿਣਤੀ++ > HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ))
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_ਗਿਣਤੀ, lookup_ਸੂਚਕਾਂਕ);
      ਵਾਪਸ ਸੱਚ;
    }

    ਜੇ (unlikely (ਦੇਖਿਆ_lookups->ਵਿੱਚ_error ()))
      ਵਾਪਸ ਸੱਚ;

    ਵਾਪਸ ਦੇਖਿਆ_lookups->has (lookup_ਸੂਚਕਾਂਕ);
  }

  hb_face_t *face;
  ਅਚਲ hb_ਸੈੱਟ_t *glyphs;
  recurse_func_t recurse_func;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nesting_level_ਖੱਬਾ;

  hb_ਘੇਰਾ_lookups_context_t (hb_face_t *face_,
				ਅਚਲ hb_ਸੈੱਟ_t *glyphs_,
				hb_ਸੈੱਟ_t *ਦੇਖਿਆ_lookups_2,
				hb_ਸੈੱਟ_t *inactive_lookups_,
				ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level_ਖੱਬਾ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (ਖਾਲੀ_ਸੂਚਕ),
				nesting_level_ਖੱਬਾ (nesting_level_ਖੱਬਾ_2),
				ਦੇਖਿਆ_lookups (ਦੇਖਿਆ_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_ਗਿਣਤੀ (0) {}

  ਖਾਲੀ ਸੈੱਟ_recurse_func (recurse_func_t func) { recurse_func = func; }

  ਨਿਜੀ:
  hb_ਸੈੱਟ_t *ਦੇਖਿਆ_lookups;
  hb_ਸੈੱਟ_t *inactive_lookups;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ;
};

ਬਣਤਰ hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, ਤਾਰਕਿਕ>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { ਵਾਪਸ obj.would_apply (ਇਹ); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }

  hb_face_t *face;
  ਅਚਲ hb_codepoint_t *glyphs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len;
  ਤਾਰਕਿਕ zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ਅਚਲ hb_codepoint_t *glyphs_,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len_,
			    ਤਾਰਕਿਕ zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

ਬਣਤਰ hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ);
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { obj.collect_glyphs (ਇਹ); ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਖਾਲੀ recurse (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (unlikely (nesting_level_ਖੱਬਾ == 0 || !recurse_func))
      ਵਾਪਸ;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    ਜੇ (output == hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ())
      ਵਾਪਸ;

    /* Return if new lookup was recursed to before. */
    ਜੇ (recursed_lookups->has (lookup_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t *old_before = before;
    hb_ਸੈੱਟ_t *old_ਨਿਵੇਸ਼  = ਨਿਵੇਸ਼;
    hb_ਸੈੱਟ_t *old_after  = after;
    before = ਨਿਵੇਸ਼ = after = hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ();

    nesting_level_ਖੱਬਾ--;
    recurse_func (ਇਹ, lookup_ਸੂਚਕਾਂਕ);
    nesting_level_ਖੱਬਾ++;

    before = old_before;
    ਨਿਵੇਸ਼  = old_ਨਿਵੇਸ਼;
    after  = old_after;

    recursed_lookups->ਜੋੜੋ (lookup_ਸੂਚਕਾਂਕ);
  }

  hb_face_t *face;
  hb_ਸੈੱਟ_t *before;
  hb_ਸੈੱਟ_t *ਨਿਵੇਸ਼;
  hb_ਸੈੱਟ_t *after;
  hb_ਸੈੱਟ_t *output;
  recurse_func_t recurse_func;
  hb_ਸੈੱਟ_t *recursed_lookups;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nesting_level_ਖੱਬਾ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_ਸੈੱਟ_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_ਸੈੱਟ_t  *glyphs_ਨਿਵੇਸ਼,  /* OUT.  May be NULL */
			       hb_ਸੈੱਟ_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_ਸੈੱਟ_t  *glyphs_output, /* OUT.  May be NULL */
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nesting_level_ਖੱਬਾ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()),
			      ਨਿਵੇਸ਼  (glyphs_ਨਿਵੇਸ਼  ? glyphs_ਨਿਵੇਸ਼  : hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()),
			      after  (glyphs_after  ? glyphs_after  : hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()),
			      output (glyphs_output ? glyphs_output : hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ ()),
			      recurse_func (ਖਾਲੀ_ਸੂਚਕ),
			      recursed_lookups (hb_ਸੈੱਟ_create ()),
			      nesting_level_ਖੱਬਾ (nesting_level_ਖੱਬਾ_2) {}
  ~hb_collect_glyphs_context_t () { hb_ਸੈੱਟ_destroy (recursed_lookups); }

  ਖਾਲੀ ਸੈੱਟ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ਬਣਤਰ hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  ਗਿਣਤੀ glyph_ਸੈੱਟ_type_t
  {
    GLYPH_ਸੈੱਟ_CLASS,
    GLYPH_ਸੈੱਟ_COVERAGE,
    GLYPH_ਸੈੱਟ_SINGLE
  };

  ਸਥਿਰ constexpr hb_codepoint_t SINGLE_GLYPH_ਸਰਗਰਮ_FLAG = 0x80000000u;

  ਬਣਤਰ glyph_ਸੈੱਟ_cache_ਕੁੰਜੀ_t
  {
    glyph_ਸੈੱਟ_cache_ਕੁੰਜੀ_t () = ਮੂਲ;
    glyph_ਸੈੱਟ_cache_ਕੁੰਜੀ_t (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_3, glyph_ਸੈੱਟ_type_t type_)
      : ਅੰਕੜੇ (ਅੰਕੜੇ_2), value (ਮੁੱਲ_3), type (type_) {}

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ glyph_ਸੈੱਟ_cache_ਕੁੰਜੀ_t &o) ਅਚਲ
    { ਵਾਪਸ ਅੰਕੜੇ == o.ਅੰਕੜੇ && value == o.value && type == o.type; }

    uint32_t hash () ਅਚਲ
    {
      uint32_t current = hb_hash ((uintptr_t) ਅੰਕੜੇ);
      current = current * 31 + hb_hash (value);
      ਵਾਪਸ current * 31 + hb_hash ((ਬਿਨਾਨਿਸ਼ਾਨੀ) type);
    }

    ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ value = 0;
    glyph_ਸੈੱਟ_type_t type = GLYPH_ਸੈੱਟ_CLASS;
  };

  ਬਣਤਰ context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t
  {
    context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t () = ਮੂਲ;
    context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t (ਅਚਲ ਖਾਲੀ *ਨਿਯਮ,
			     ਅਚਲ ਖਾਲੀ *ਰਿਕਾਰਡ_3,
			     ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_2,
			     hb_codepoint_t ਸਰਗਰਮ_glyphs,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_3,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ format_)
      : ਨਿਯਮ_2 (ਨਿਯਮ), ਰਿਕਾਰਡ (ਰਿਕਾਰਡ_3), ਅੰਕੜੇ (ਅੰਕੜੇ_2), ਸਰਗਰਮ_glyphs_2 (ਸਰਗਰਮ_glyphs),
	value (ਮੁੱਲ_3), format (format_) {}

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t &o) ਅਚਲ
    {
      ਵਾਪਸ ਨਿਯਮ_2 == o.ਨਿਯਮ_2 &&
	     ਰਿਕਾਰਡ == o.ਰਿਕਾਰਡ &&
	     ਅੰਕੜੇ == o.ਅੰਕੜੇ &&
	     ਸਰਗਰਮ_glyphs_2 == o.ਸਰਗਰਮ_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ਅਚਲ
    {
      uint32_t current = hb_hash ((uintptr_t) ਨਿਯਮ_2);
      current = current * 31 + hb_hash ((uintptr_t) ਰਿਕਾਰਡ);
      current = current * 31 + hb_hash ((uintptr_t) ਅੰਕੜੇ);
      current = current * 31 + hb_hash (ਸਰਗਰਮ_glyphs_2);
      current = current * 31 + hb_hash (value);
      ਵਾਪਸ current * 31 + hb_hash (format);
    }

    ਅਚਲ ਖਾਲੀ *ਨਿਯਮ_2 = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਖਾਲੀ *ਰਿਕਾਰਡ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    hb_codepoint_t ਸਰਗਰਮ_glyphs_2 = HB_CODEPOINT_INVALID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ value = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ format = 0;
  };

  ਬਣਤਰ recurse_ਕੁੰਜੀ_t
  {
    recurse_ਕੁੰਜੀ_t () = ਮੂਲ;
    recurse_ਕੁੰਜੀ_t (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ_3,
                   hb_codepoint_t ਸਰਗਰਮ_glyphs,
                   hb_codepoint_t recurse_ਰਸਤਾ,
                   hb_codepoint_t context_ਸੈੱਟ_2)
      : lookup_ਸੂਚਕਾਂਕ (lookup_ਸੂਚਕਾਂਕ_3),
        ਸਰਗਰਮ_glyphs_2 (ਸਰਗਰਮ_glyphs),
        recurse_ਰਸਤਾ_2 (recurse_ਰਸਤਾ),
        context_ਸੈੱਟ (context_ਸੈੱਟ_2) {}

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ recurse_ਕੁੰਜੀ_t &o) ਅਚਲ
    {
      ਵਾਪਸ lookup_ਸੂਚਕਾਂਕ == o.lookup_ਸੂਚਕਾਂਕ &&
             ਸਰਗਰਮ_glyphs_2 == o.ਸਰਗਰਮ_glyphs_2 &&
             recurse_ਰਸਤਾ_2 == o.recurse_ਰਸਤਾ_2 &&
             context_ਸੈੱਟ == o.context_ਸੈੱਟ;
    }

    uint32_t hash () ਅਚਲ
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_ਸੂਚਕਾਂਕ)) * 16777619;
      current = (current ^ hb_hash (ਸਰਗਰਮ_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_ਰਸਤਾ_2)) * 16777619;
      current = (current ^ hb_hash (context_ਸੈੱਟ)) * 16777619;
      ਵਾਪਸ current;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ = 0;
    hb_codepoint_t ਸਰਗਰਮ_glyphs_2 = 0;
    hb_codepoint_t recurse_ਰਸਤਾ_2 = 0;
    hb_codepoint_t context_ਸੈੱਟ = HB_CODEPOINT_INVALID;
  };

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ return_t (*recurse_func_t) (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { obj.depend (ਇਹ); ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  ਖਾਲੀ recurse (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_idx, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ)
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
    ਜੇ (lookups_ਵੇਖਿਆ.has (lookup_idx)) ਵਾਪਸ;
    lookups_ਵੇਖਿਆ.ਜੋੜੋ (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t ਮਾਪਾ_ਰਸਤਾ = recurse_ਰਸਤਾ_2;
    uint64_t ਰਸਤਾ_ਕੁੰਜੀ = (uint64_t) ਮਾਪਾ_ਰਸਤਾ << 32 | lookup_idx;
    hb_codepoint_t *ਰਸਤਾ = ਖਾਲੀ_ਸੂਚਕ;
    hb_codepoint_t ਰਸਤਾ_ਸੂਚਕਾਂਕ;
    ਜੇ (!recurse_ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.has (ਰਸਤਾ_ਕੁੰਜੀ, &ਰਸਤਾ))
    {
      ਰਸਤਾ_ਸੂਚਕਾਂਕ = recurse_ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.get_population () + 1;
      ਜੇ (unlikely (!recurse_ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.ਸੈੱਟ (ਰਸਤਾ_ਕੁੰਜੀ, ਰਸਤਾ_ਸੂਚਕਾਂਕ)))
      {
	depend_ਅੰਕੜੇ->fail ();
	lookups_ਵੇਖਿਆ.del (lookup_idx);
	ਵਾਪਸ;
      }
    }
    ਨਹੀਂਤਾਂ
      ਰਸਤਾ_ਸੂਚਕਾਂਕ = *ਰਸਤਾ;
    recurse_ਰਸਤਾ_2 = ਰਸਤਾ_ਸੂਚਕਾਂਕ;
    recurse_func (ਇਹ, lookup_idx, covered_seq_indices, seq_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
    recurse_ਰਸਤਾ_2 = ਮਾਪਾ_ਰਸਤਾ;
    lookups_ਵੇਖਿਆ.del (lookup_idx);
  }

  ਬਣਤਰ ਸਰਗਰਮ_glyphs_ਚੌਖਟਾ_t
  {
    hb_ਸੈੱਟ_t glyphs;
    hb_codepoint_t ਸਰਗਰਮ_idx = HB_CODEPOINT_INVALID;
    ਤਾਰਕਿਕ use_ਜੜ੍ਹ = ਝੂਠ;
  };

  ਅਚਲ hb_ਸੈੱਟ_t& ਮਾਪਾ_ਸਰਗਰਮ_glyphs ()
  {
    ਜੇ (!ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ)
      ਵਾਪਸ *glyphs;

    ਅਚਲ ਸਰਗਰਮ_glyphs_ਚੌਖਟਾ_t &ਚੌਖਟਾ = ਸਰਗਰਮ_glyphs_ਢੇਰ[ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ - 1];
    ਵਾਪਸ ਚੌਖਟਾ.use_ਜੜ੍ਹ ? *glyphs : ਚੌਖਟਾ.glyphs;
  }

  hb_ਸੈੱਟ_t* ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs (hb_codepoint_t ਸਰਗਰਮ_idx = HB_CODEPOINT_INVALID)
  {
    ਜੇ (ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ == ਸਰਗਰਮ_glyphs_ਢੇਰ.ਲੰਮਾਈ &&
	unlikely (!ਸਰਗਰਮ_glyphs_ਢੇਰ.ਧੱਕੋ_or_fail ()))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਸਰਗਰਮ_glyphs_ਚੌਖਟਾ_t &ਚੌਖਟਾ = ਸਰਗਰਮ_glyphs_ਢੇਰ[ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ++];
    ਚੌਖਟਾ.glyphs.clear ();
    ਚੌਖਟਾ.ਸਰਗਰਮ_idx = ਸਰਗਰਮ_idx;
    ਚੌਖਟਾ.use_ਜੜ੍ਹ = ਝੂਠ;
    ਵਾਪਸ &ਚੌਖਟਾ.glyphs;
  }

  ਤਾਰਕਿਕ ਧੱਕੋ_ਜੜ੍ਹ_ਸਰਗਰਮ_glyphs ()
  {
    ਜੇ (ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ == ਸਰਗਰਮ_glyphs_ਢੇਰ.ਲੰਮਾਈ &&
	unlikely (!ਸਰਗਰਮ_glyphs_ਢੇਰ.ਧੱਕੋ_or_fail ()))
      ਵਾਪਸ ਝੂਠ;

    ਸਰਗਰਮ_glyphs_ਢੇਰ[ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ++].use_ਜੜ੍ਹ = ਸੱਚ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ()
  {
    ਜੇ (!ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ)
      ਵਾਪਸ ਝੂਠ;

    ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ--;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_ਮਾਪਾ_ਸਰਗਰਮ_glyphs_ਸੂਚਕਾਂਕ (hb_codepoint_t *ਸਰਗਰਮ_idx)
  {
    ਜੇ (!ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ ||
	ਸਰਗਰਮ_glyphs_ਢੇਰ[ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ - 1].use_ਜੜ੍ਹ)
    {
      *ਸਰਗਰਮ_idx = HB_CODEPOINT_INVALID;
      ਵਾਪਸ ਸੱਚ;
    }

    ਸਰਗਰਮ_glyphs_ਚੌਖਟਾ_t &ਚੌਖਟਾ = ਸਰਗਰਮ_glyphs_ਢੇਰ[ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ - 1];
    *ਸਰਗਰਮ_idx = ਚੌਖਟਾ.ਸਰਗਰਮ_idx;
    ਜੇ (*ਸਰਗਰਮ_idx != HB_CODEPOINT_INVALID)
      ਵਾਪਸ ਸੱਚ;

    hb_codepoint_t glyph;
    ਜੇ (ਚੌਖਟਾ.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ਸਰਗਰਮ_FLAG))
      *ਸਰਗਰਮ_idx = glyph | SINGLE_GLYPH_ਸਰਗਰਮ_FLAG;
    ਨਹੀਂਤਾਂ ਜੇ (!ਲੱਭੋ_or_create_recurse_ਸੈੱਟ (ਚੌਖਟਾ.glyphs, ਸਰਗਰਮ_idx))
      ਵਾਪਸ ਝੂਠ;

    ਚੌਖਟਾ.ਸਰਗਰਮ_idx = *ਸਰਗਰਮ_idx;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_context_ਸੈੱਟ_cache_ਕੁੰਜੀ (ਅਚਲ ਖਾਲੀ *ਨਿਯਮ_2,
				  ਅਚਲ ਖਾਲੀ *ਰਿਕਾਰਡ,
				  ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ value,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ format,
				  context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t *key)
  {
    hb_codepoint_t ਸਰਗਰਮ_idx;
    ਜੇ (!get_ਮਾਪਾ_ਸਰਗਰਮ_glyphs_ਸੂਚਕਾਂਕ (&ਸਰਗਰਮ_idx))
      ਵਾਪਸ ਝੂਠ;
    *key = context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t (ਨਿਯਮ_2, ਰਿਕਾਰਡ, ਅੰਕੜੇ, ਸਰਗਰਮ_idx, value, format);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_recurse_ਕੁੰਜੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_idx, recurse_ਕੁੰਜੀ_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t ਸਰਗਰਮ_idx;
    ਜੇ (!get_ਮਾਪਾ_ਸਰਗਰਮ_glyphs_ਸੂਚਕਾਂਕ (&ਸਰਗਰਮ_idx))
      ਵਾਪਸ ਝੂਠ;

    *key = recurse_ਕੁੰਜੀ_t (lookup_idx,
                          ਸਰਗਰਮ_idx,
                          recurse_ਰਸਤਾ_2,
                          depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ !completed_recursions.has (*key);
  }

  ਖਾਲੀ ਅੰਤ_recurse (ਅਚਲ recurse_ਕੁੰਜੀ_t &key)
  { depend_ਅੰਕੜੇ->check_success (completed_recursions.ਸੈੱਟ (key, ਸੱਚ)); }

  ਖਾਲੀ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.clear ();
    recurse_ਰਸਤਾ_2 = 0;
  }

  ਤਾਰਕਿਕ ਲੱਭੋ_or_create_recurse_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &ਸੈੱਟ, hb_codepoint_t *ਸੂਚਕਾਂਕ_2)
  {
    hb_codepoint_t *existing = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (recurse_ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.has (&ਸੈੱਟ, &existing))
    {
      *ਸੂਚਕਾਂਕ_2 = *existing;
      ਵਾਪਸ ਸੱਚ;
    }

    hb_ਸੈੱਟ_t *copy = hb_object_create<hb_ਸੈੱਟ_t> ();
    ਜੇ (unlikely (!copy))
      ਵਾਪਸ depend_ਅੰਕੜੇ->fail ();
    copy->ਸੈੱਟ (ਸੈੱਟ);
    ਜੇ (unlikely (copy->ਵਿੱਚ_error ()))
    {
      hb_ਸੈੱਟ_destroy (copy);
      ਵਾਪਸ depend_ਅੰਕੜੇ->fail ();
    }

    *ਸੂਚਕਾਂਕ_2 = recurse_sets.ਲੰਮਾਈ;
    ਜੇ (unlikely (*ਸੂਚਕਾਂਕ_2 & SINGLE_GLYPH_ਸਰਗਰਮ_FLAG))
    {
      hb_ਸੈੱਟ_destroy (copy);
      ਵਾਪਸ depend_ਅੰਕੜੇ->fail ();
    }
    ਜੇ (unlikely (!recurse_sets.ਧੱਕੋ_or_fail (hb::unique_ptr<hb_ਸੈੱਟ_t> {copy})))
      ਵਾਪਸ depend_ਅੰਕੜੇ->fail ();
    ਜੇ (unlikely (!recurse_ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.ਸੈੱਟ (recurse_sets[*ਸੂਚਕਾਂਕ_2].get (), *ਸੂਚਕਾਂਕ_2)))
      ਵਾਪਸ depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਸੱਚ;
  }

  ਅਚਲ hb_ਸੈੱਟ_t *ਲੱਭੋ_or_create_glyph_ਸੈੱਟ (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ value,
                                             glyph_ਸੈੱਟ_type_t type);
  hb_codepoint_t ਲੱਭੋ_or_create_cached_context_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

  ਬਣਤਰ depend_scratch_t
  {
    ਖਾਲੀ clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      ਨਿਵੇਸ਼_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_ਸੈੱਟ_t preliminary_context;
    hb_ਸੈੱਟ_t position_scratch;
    hb_ਸੈੱਟ_t pos_glyphs;
    hb_ਸੈੱਟ_t position_context;
    hb_ਸੈੱਟ_t disjunctive_indices;
    hb_ਸੈੱਟ_t filtered_disjunctive_indices;
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> backtrack_sets;
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> lookahead_sets;
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> ਨਿਵੇਸ਼_position_glyphs;
    hb_ਸਦਿਸ਼_t<hb_codepoint_t> cached_context_sets;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> compact_workspace;
    depend_scratch_t *next = ਖਾਲੀ_ਸੂਚਕ;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    ਜੇ (scratch)
      depend_scratch_pool = scratch->next;
    ਨਹੀਂਤਾਂ
    {
      scratch = (depend_scratch_t *) hb_malloc (ਆਕਾਰ (depend_scratch_t));
      ਜੇ (unlikely (!scratch))
      {
        depend_ਅੰਕੜੇ->fail ();
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      }
      ਨਵਾਂ (scratch) depend_scratch_t ();
    }

    scratch->next = ਖਾਲੀ_ਸੂਚਕ;
    scratch->clear ();
    ਵਾਪਸ scratch;
  }

  ਖਾਲੀ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ;
  hb_face_t *face;
  hb_ਸੈੱਟ_t *glyphs;
  hb_ਸਦਿਸ਼_t<ਸਰਗਰਮ_glyphs_ਚੌਖਟਾ_t> ਸਰਗਰਮ_glyphs_ਢੇਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰਗਰਮ_glyphs_ਢੇਰ_ਡੂੰਘਾਈ = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_ਸੂਚਕਾਂਕ = HB_CODEPOINT_INVALID;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t lookups_ਵੇਖਿਆ;
  hb_ਸਦਿਸ਼_t<hb::unique_ptr<hb_ਸੈੱਟ_t>> recurse_sets;
  hb_hashmap_t<ਅਚਲ hb_ਸੈੱਟ_t *, hb_codepoint_t> recurse_ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ;
  hb_codepoint_t recurse_ਰਸਤਾ_2 = 0;
  hb_hashmap_t<recurse_ਕੁੰਜੀ_t, ਤਾਰਕਿਕ> completed_recursions;
  hb_ਸਦਿਸ਼_t<hb::unique_ptr<hb_ਸੈੱਟ_t>> glyph_sets;
  hb_hashmap_t<glyph_ਸੈੱਟ_cache_ਕੁੰਜੀ_t, hb_codepoint_t> glyph_ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_ਸੈੱਟ_indices;
  hb_hashmap_t<context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t, hb_codepoint_t> context_ਸੈੱਟ_cache;
  depend_scratch_t *depend_scratch_pool = ਖਾਲੀ_ਸੂਚਕ;

  hb_depend_context_t (hb_depend_ਅੰਕੜੇ_builder_t *depend_ਅੰਕੜੇ_2,
                       hb_face_t *face_,
                       hb_ਸੈੱਟ_t *glyphs_)
                      : depend_ਅੰਕੜੇ (depend_ਅੰਕੜੇ_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (ਖਾਲੀ_ਸੂਚਕ) {}
  ~hb_depend_context_t ()
  {
    ਜਦੋਂ (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  ਖਾਲੀ ਸੈੱਟ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
ਬਣਤਰ hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<ਸੈੱਟ_t>, ਅਚਲ Coverage &>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ Coverage &return_t; // Stoopid that we have to dupe this here.
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { ਵਾਪਸ obj.get_coverage (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ Null (Coverage); }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ
  {
    r.collect_coverage (ਸੈੱਟ);
    ਵਾਪਸ ਝੂਠ;
  }

  hb_collect_coverage_context_t (ਸੈੱਟ_t *ਸੈੱਟ_2) :
				   ਸੈੱਟ (ਸੈੱਟ_2) {}

  ਸੈੱਟ_t *ਸੈੱਟ;
};

ਬਣਤਰ matcher_t
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*ਮੇਲ_func_t) (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
  ਖਾਲੀ init (ਅਚਲ context_t *c, ਤਾਰਕਿਕ context_ਮੇਲ = ਝੂਠ)
  {
    ਸੈੱਟ_ਮੇਲ_func (ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->ਸਾਰਣੀ_ਸੂਚਕਾਂਕ == 1 || (context_ਮੇਲ && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_ਮੇਲ || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->ਸਾਰਣੀ_ਸੂਚਕਾਂਕ == 1;
    ਨਕਾਬ = context_ਮੇਲ ? -1 : c->lookup_ਨਕਾਬ;
    /* Per syllable matching is only for GSUB. */
    ਪ੍ਰਤੀ_syllable = c->ਸਾਰਣੀ_ਸੂਚਕਾਂਕ == 0 && c->ਪ੍ਰਤੀ_syllable;
    syllable = 0;
  }

  ਖਾਲੀ ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_func_t ਮੇਲ_func,
		       ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ)
  { ਮੇਲ_func_2 = ਮੇਲ_func; ਮੇਲ_ਅੰਕੜੇ_2 = ਮੇਲ_ਅੰਕੜੇ; }

  ਗਿਣਤੀ may_ਮੇਲ_t {
    ਮੇਲ_NO,
    ਮੇਲ_YES,
    ਮੇਲ_MAYBE
  };

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  may_ਮੇਲ_t may_ਮੇਲ (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_ਅੰਕੜੇ) ਅਚਲ
  {
    ਜੇ (!(info.ਨਕਾਬ & ਨਕਾਬ) ||
	(ਪ੍ਰਤੀ_syllable && syllable && syllable != info.syllable ()))
      ਵਾਪਸ ਮੇਲ_NO;

    ਜੇ (ਮੇਲ_func_2)
      ਵਾਪਸ ਮੇਲ_func_2 (info, glyph_ਅੰਕੜੇ, ਮੇਲ_ਅੰਕੜੇ_2) ? ਮੇਲ_YES : ਮੇਲ_NO;

    ਵਾਪਸ ਮੇਲ_MAYBE;
  }

  ਗਿਣਤੀ may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  may_skip_t may_skip (ਅਚਲ context_t *c,
		       ਅਚਲ hb_glyph_info_t &info) ਅਚਲ
  {
    ਜੇ (!c->check_glyph_property (&info, lookup_props))
      ਵਾਪਸ SKIP_YES;

    ਜੇ (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      ਵਾਪਸ SKIP_MAYBE;

    ਵਾਪਸ SKIP_NO;
  }

  ਜਨਤਕ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_props = 0;
  hb_ਨਕਾਬ_t ਨਕਾਬ = -1;
  ਤਾਰਕਿਕ ignore_zwnj = ਝੂਠ;
  ਤਾਰਕਿਕ ignore_zwj = ਝੂਠ;
  ਤਾਰਕਿਕ ignore_hidden = ਝੂਠ;
  ਤਾਰਕਿਕ ਪ੍ਰਤੀ_syllable = ਝੂਠ;
  uint8_t syllable = 0;
  ਮੇਲ_func_t ਮੇਲ_func_2 = ਖਾਲੀ_ਸੂਚਕ;
  ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2 = ਖਾਲੀ_ਸੂਚਕ;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
ਬਣਤਰ glyph_ਅੰਕੜੇ_t
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  glyph_ਅੰਕੜੇ_t (ਅਚਲ T *ਅੰਕੜੇ_2) : ਅੰਕੜੇ (ਅੰਕੜੇ_2)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
                              , wide (ਆਕਾਰ (T) == 3)
#ਜੇ_ਅੰਤ
  {
    static_assert (ਆਕਾਰ (T) == 2 || ਆਕਾਰ (T) == 3, "invalid glyph data size");
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (wide)
      ਵਾਪਸ ((ਅਚਲ HBUINT24 *) ਅੰਕੜੇ)[i];
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ((ਅਚਲ HBUINT16 *) ਅੰਕੜੇ)[i];
  }

  ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਤਾਰਕਿਕ wide;
#ਜੇ_ਅੰਤ
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
ਬਣਤਰ skipping_iterator_t
{
  ਖਾਲੀ init (context_t *c_, ਤਾਰਕਿਕ context_ਮੇਲ = ਝੂਠ)
  {
    c = c_;
    ਸਿਰਾ = c->ਬਫਰ->len;
    ਮੇਲ_glyph_data16 = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਮੇਲ_glyph_data24 = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ
    matcher.init (c, context_ਮੇਲ);
  }
  ਖਾਲੀ ਸੈੱਟ_lookup_props (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  ਖਾਲੀ ਸੈੱਟ_ਮੇਲ_func (matcher_t::ਮੇਲ_func_t ਮੇਲ_func,
		       ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ)
  {
    matcher.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_func, ਮੇਲ_ਅੰਕੜੇ);
  }
  ਖਾਲੀ ਸੈੱਟ_glyph_ਅੰਕੜੇ (glyph_ਅੰਕੜੇ_t glyph_ਅੰਕੜੇ)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਮੇਲ_glyph_data16 = glyph_ਅੰਕੜੇ.wide ? ਖਾਲੀ_ਸੂਚਕ : (ਅਚਲ HBUINT16 *) glyph_ਅੰਕੜੇ.ਅੰਕੜੇ;
    ਮੇਲ_glyph_data24 = glyph_ਅੰਕੜੇ.wide ? (ਅਚਲ HBUINT24 *) glyph_ਅੰਕੜੇ.ਅੰਕੜੇ : ਖਾਲੀ_ਸੂਚਕ;
#ਨਹੀਂਤਾਂ
    ਮੇਲ_glyph_data16 = (ਅਚਲ HBUINT16 *) glyph_ਅੰਕੜੇ.ਅੰਕੜੇ;
#ਜੇ_ਅੰਤ
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਖਾਲੀ reset (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ_2)
  {
    // For GSUB forward iterator
    idx = ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ_2;
    ਸਿਰਾ = c->ਬਫਰ->len;
    matcher.syllable = c->ਬਫਰ->cur().syllable();
  }
  ਖਾਲੀ reset_ਪਿੱਠ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ_2, ਤਾਰਕਿਕ from_ਬਾਹਰਲਾ_ਬਫਰ = ਝੂਠ)
  {
    // For GSUB backward iterator
    idx = ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ_2;
    matcher.syllable = c->ਬਫਰ->cur().syllable();
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਖਾਲੀ reset_fast (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ_2;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  matcher_t::may_skip_t may_skip (ਅਚਲ hb_glyph_info_t &info) ਅਚਲ
  { ਵਾਪਸ matcher.may_skip (c, info); }

  ਗਿਣਤੀ ਮੇਲ_t {
    ਮੇਲ_2,
    NOT_ਮੇਲ,
    SKIP
  };

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਮੇਲ_t ਮੇਲ (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    ਜੇ (unlikely (skip == matcher_t::SKIP_YES))
      ਵਾਪਸ SKIP;

    matcher_t::may_ਮੇਲ_t ਮੇਲ = matcher.may_ਮੇਲ (info, get_glyph_ਅੰਕੜੇ ());
    ਜੇ (ਮੇਲ == matcher_t::ਮੇਲ_YES ||
	(ਮੇਲ == matcher_t::ਮੇਲ_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      ਵਾਪਸ ਮੇਲ_2;

    ਜੇ (skip == matcher_t::SKIP_NO)
      ਵਾਪਸ NOT_ਮੇਲ;

    ਵਾਪਸ SKIP;
  }

  ਤਾਰਕਿਕ next (ਬਿਨਾਨਿਸ਼ਾਨੀ *unsafe_ਮੰਜ਼ਿਲ = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਸਵੈਚਾਲਿਤ *info = c->ਬਫਰ->info;
    ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ stop = (ਨਿਸ਼ਾਨੀਵਾਲਾ) ਸਿਰਾ - 1;
    ਜਦੋਂ ((ਨਿਸ਼ਾਨੀਵਾਲਾ) idx < stop)
    {
      idx++;
      ਚੋਣ (ਮੇਲ (info[idx]))
      {
	ਹਾਲਤ ਮੇਲ_2:
	{
	  advance_glyph_ਅੰਕੜੇ ();
	  ਵਾਪਸ ਸੱਚ;
	}
	ਹਾਲਤ NOT_ਮੇਲ:
	{
	  ਜੇ (unsafe_ਮੰਜ਼ਿਲ)
	    *unsafe_ਮੰਜ਼ਿਲ = idx + 1;
	  ਵਾਪਸ ਝੂਠ;
	}
	ਹਾਲਤ SKIP:
	  ਜਾਰੀ;
      }
    }
    ਜੇ (unsafe_ਮੰਜ਼ਿਲ)
      *unsafe_ਮੰਜ਼ਿਲ = ਸਿਰਾ;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ prev (ਬਿਨਾਨਿਸ਼ਾਨੀ *unsafe_from = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ_info = c->ਬਫਰ->ਬਾਹਰਲਾ_info;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ stop = 0;
    ਜਦੋਂ (idx > stop)
    {
      idx--;
      ਚੋਣ (ਮੇਲ (ਬਾਹਰਲਾ_info[idx]))
      {
	ਹਾਲਤ ਮੇਲ_2:
	{
	  advance_glyph_ਅੰਕੜੇ ();
	  ਵਾਪਸ ਸੱਚ;
	}
	ਹਾਲਤ NOT_ਮੇਲ:
	{
	  ਜੇ (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  ਵਾਪਸ ਝੂਠ;
	}
	ਹਾਲਤ SKIP:
	  ਜਾਰੀ;
      }
    }
    ਜੇ (unsafe_from)
      *unsafe_from = 0;
    ਵਾਪਸ ਝੂਠ;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_ਅੰਕੜੇ ()
  {
    ਜੇ (ਮੇਲ_glyph_data16) ਵਾਪਸ *ਮੇਲ_glyph_data16;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਨਹੀਂਤਾਂ
    ਜੇ (ਮੇਲ_glyph_data24) ਵਾਪਸ *ਮੇਲ_glyph_data24;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ 0;
  }
  HB_ALWAYS_INLINE
  ਖਾਲੀ
  advance_glyph_ਅੰਕੜੇ ()
  {
    ਜੇ (ਮੇਲ_glyph_data16) ਮੇਲ_glyph_data16++;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਨਹੀਂਤਾਂ
    ਜੇ (ਮੇਲ_glyph_data24) ਮੇਲ_glyph_data24++;
#ਜੇ_ਅੰਤ
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx;
  ਸੁਰੱਖਿਅਤ:
  context_t *c;
  matcher_t matcher;
  ਅਚਲ HBUINT16 *ਮੇਲ_glyph_data16;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਅਚਲ HBUINT24 *ਮੇਲ_glyph_data24;
#ਜੇ_ਅੰਤ

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ;
};

ਬਣਤਰ hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, ਤਾਰਕਿਕ, HB_DEBUG_APPLY>
{
  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "APPLY"; }
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ return_t (*recurse_func_t) (hb_ot_apply_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ);

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_ (ਅਚਲ T &obj, hb_ot_apply_context_t *c, hb_ਤਰਜੀਹ<1>) HB_RETURN (return_t, obj.apply (c, ਖਾਲੀ_ਸੂਚਕ) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_ (ਅਚਲ T &obj, hb_ot_apply_context_t *c, hb_ਤਰਜੀਹ<0>) HB_RETURN (return_t, obj.apply (c) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { ਵਾਪਸ apply_(obj, ਇਹ, hb_prioritize); }

  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }
  return_t recurse (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sub_lookup_ਸੂਚਕਾਂਕ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (recurse_func);
    ਜੇ (unlikely (nesting_level_ਖੱਬਾ == 0))
    {
      ਬਫਰ->successful = ਝੂਠ;
      ਵਾਪਸ default_return_ਮੁੱਲ ();
    }

    ਬਫਰ->max_ops--;
    ਜੇ (unlikely (ਬਫਰ->max_ops < 0))
    {
      ਬਫਰ->successful = ਝੂਠ;
      ਵਾਪਸ default_return_ਮੁੱਲ ();
    }

    nesting_level_ਖੱਬਾ--;
    ਤਾਰਕਿਕ ret = recurse_func (ਇਹ, sub_lookup_ਸੂਚਕਾਂਕ);
    nesting_level_ਖੱਬਾ++;
    ਵਾਪਸ ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_ਨਿਵੇਸ਼, iter_context;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_ਬਫਰ_t *ਬਫਰ;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = ਖਾਲੀ_ਸੂਚਕ;
  ਅਚਲ GDEF &gdef;
  ਅਚਲ GDEF::accelerator_t &gdef_accel;
  ਅਚਲ ਵਸਤੂ_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_ਨਕਾਬ_t lookup_ਨਕਾਬ = 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_props = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nesting_level_ਖੱਬਾ = HB_MAX_NESTING_LEVEL;

  ਤਾਰਕਿਕ has_glyph_classes;
  ਤਾਰਕਿਕ auto_zwnj = ਸੱਚ;
  ਤਾਰਕਿਕ auto_zwj = ਸੱਚ;
  ਤਾਰਕਿਕ ਪ੍ਰਤੀ_syllable = ਝੂਠ;
  ਤਾਰਕਿਕ random = ਝੂਠ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ new_syllables = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;

  ਨਿਸ਼ਾਨੀਵਾਲਾ ਆਖਰੀ_ਅਧਾਰ = -1; // GPOS uses
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_ਅਧਾਰ_until = 0; // GPOS uses

  hb_ਸਦਿਸ਼_t<uint32_t> ਮੇਲ_positions;
  uint32_t ਢੇਰ_ਮੇਲ_positions[8];
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
  hb_ਬਫਰ_t::ਬਦਲਿਆ_func_t orig_ਬਦਲਿਆ_func = ਖਾਲੀ_ਸੂਚਕ;
  ਖਾਲੀ *orig_ਬਦਲਿਆ_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ

  hb_ot_apply_context_t (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ_2,
			 hb_font_t *font_,
			 hb_ਬਫਰ_t *ਬਫਰ_2,
			 hb_blob_t *ਸਾਰਣੀ_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = ਖਾਲੀ_ਸੂਚਕ) :
			ਸਾਰਣੀ_ਸੂਚਕਾਂਕ (ਸਾਰਣੀ_ਸੂਚਕਾਂਕ_2),
			font (font_), face (font->face), ਬਫਰ (ਬਫਰ_2),
			sanitizer (ਸਾਰਣੀ_blob_2),
			gdef (
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
			      *face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ
#ਨਹੀਂਤਾਂ
			      Null (GDEF)
#ਜੇ_ਅੰਤ
			     ),
			gdef_accel (
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
			      *face->ਸਾਰਣੀ.GDEF
#ਨਹੀਂਤਾਂ
			      Null (GDEF::accelerator_t)
#ਜੇ_ਅੰਤ
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (ਬਫਰ_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    ਮੇਲ_positions.ਸੈੱਟ_storage (ਢੇਰ_ਮੇਲ_positions);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
    ਜੇ (ਬਫਰ->messaging ())
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->ਬਦਲਿਆ_func == ਖਾਲੀ_ਸੂਚਕ ||
	      ਬਫਰ->ਬਦਲਿਆ_func == ਬਫਰ_ਬਦਲਿਆ_trampoline);
      orig_ਬਦਲਿਆ_func = ਬਫਰ->ਬਦਲਿਆ_func;
      orig_ਬਦਲਿਆ_ਅੰਕੜੇ = ਬਫਰ->ਬਦਲਿਆ_ਅੰਕੜੇ;
      ਬਫਰ->ਬਦਲਿਆ_func = ਬਫਰ_ਬਦਲਿਆ_trampoline;
      ਬਫਰ->ਬਦਲਿਆ_ਅੰਕੜੇ = ਇਹ;
    }
#ਜੇ_ਅੰਤ
  }
  ~hb_ot_apply_context_t ()
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
    ਜੇ (ਬਫਰ->messaging ())
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->ਬਦਲਿਆ_func == ਬਫਰ_ਬਦਲਿਆ_trampoline);
      ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->ਬਦਲਿਆ_ਅੰਕੜੇ == ਇਹ);
      ਬਫਰ->ਬਦਲਿਆ_func = orig_ਬਦਲਿਆ_func;
      ਬਫਰ->ਬਦਲਿਆ_ਅੰਕੜੇ = orig_ਬਦਲਿਆ_ਅੰਕੜੇ;
    }
#ਜੇ_ਅੰਤ
  }

  ਖਾਲੀ init_iters ()
  {
    iter_ਨਿਵੇਸ਼.init (ਇਹ, ਝੂਠ);
    iter_context.init (ਇਹ, ਸੱਚ);
  }

  ਖਾਲੀ ਸੈੱਟ_lookup_ਨਕਾਬ (hb_ਨਕਾਬ_t ਨਕਾਬ, ਤਾਰਕਿਕ init = ਸੱਚ) { lookup_ਨਕਾਬ = ਨਕਾਬ; ਆਖਰੀ_ਅਧਾਰ = -1; ਆਖਰੀ_ਅਧਾਰ_until = 0; ਜੇ (init) init_iters (); }
  ਖਾਲੀ ਸੈੱਟ_auto_zwj (ਤਾਰਕਿਕ auto_zwj_, ਤਾਰਕਿਕ init = ਸੱਚ) { auto_zwj = auto_zwj_; ਜੇ (init) init_iters (); }
  ਖਾਲੀ ਸੈੱਟ_auto_zwnj (ਤਾਰਕਿਕ auto_zwnj_, ਤਾਰਕਿਕ init = ਸੱਚ) { auto_zwnj = auto_zwnj_; ਜੇ (init) init_iters (); }
  ਖਾਲੀ ਸੈੱਟ_ਪ੍ਰਤੀ_syllable (ਤਾਰਕਿਕ ਪ੍ਰਤੀ_syllable_2, ਤਾਰਕਿਕ init = ਸੱਚ) { ਪ੍ਰਤੀ_syllable = ਪ੍ਰਤੀ_syllable_2; ਜੇ (init) init_iters (); }
  ਖਾਲੀ ਸੈੱਟ_random (ਤਾਰਕਿਕ random_) { random = random_; }
  ਖਾਲੀ ਸੈੱਟ_recurse_func (recurse_func_t func) { recurse_func = func; }
  ਖਾਲੀ ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ_3) { lookup_ਸੂਚਕਾਂਕ = lookup_ਸੂਚਕਾਂਕ_3; }
  ਖਾਲੀ ਸੈੱਟ_lookup_props (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_ਸੰਖਿਆ ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    ਬਫਰ->random_state = ਬਫਰ->random_state * 48271 % 2147483647;
    ਵਾਪਸ ਬਫਰ->random_state;
  }

  ਸਥਿਰ ਖਾਲੀ ਬਫਰ_ਬਦਲਿਆ_trampoline (hb_ਬਫਰ_t *ਬਫਰ HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *user_ਅੰਕੜੇ)
  {
    ((hb_ot_apply_context_t *) user_ਅੰਕੜੇ)->ਬਫਰ_ਬਦਲਿਆ ();
  }
  ਖਾਲੀ ਬਫਰ_ਬਦਲਿਆ ()
  {
    ਬਫਰ->update_digest ();
    ਜੇ (likely (has_glyph_classes))
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਫਰ->len; i++)
	ਸੈੱਟ_glyph_class_props (ਬਫਰ->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  ਤਾਰਕਿਕ ਮੇਲ_properties_mark (ਅਚਲ hb_glyph_info_t *info,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    glyph_props,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮੇਲ_props) ਅਚਲ
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    ਜੇ (ਮੇਲ_props & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੈੱਟ_ਸੂਚਕਾਂਕ = ਮੇਲ_props >> 16;
      ਵਾਪਸ gdef_accel.mark_ਸੈੱਟ_covers (ਸੈੱਟ_ਸੂਚਕਾਂਕ, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    ਜੇ (ਮੇਲ_props & LookupFlag::MarkAttachmentType)
      ਵਾਪਸ (ਮੇਲ_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    ਵਾਪਸ ਸੱਚ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ check_glyph_property (ਅਚਲ hb_glyph_info_t *info,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੇਲ_props) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    ਜੇ (glyph_props & ਮੇਲ_props & LookupFlag::IgnoreFlags)
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      ਵਾਪਸ ਮੇਲ_properties_mark (info, glyph_props, ਮੇਲ_props);

    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ ਸੈੱਟ_glyph_class (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ class_guess = 0,
			  ਤਾਰਕਿਕ ligature = ਝੂਠ,
			  ਤਾਰਕਿਕ component = ਝੂਠ)
  {
    ਬਫਰ->digest.ਜੋੜੋ (glyph_ਸੂਚਕਾਂਕ);

    ਜੇ (new_syllables != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
      ਬਫਰ->cur().syllable() = new_syllables;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ props = _hb_glyph_info_get_glyph_props (&ਬਫਰ->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    ਜੇ (ligature)
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
    ਜੇ (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    ਜੇ (likely (has_glyph_classes))
    {
      ਸੈੱਟ_glyph_class_props (ਬਫਰ->cur(), props, glyph_ਸੂਚਕਾਂਕ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_ਸੈੱਟ_glyph_props (&ਬਫਰ->cur(), props | class_guess);
    }
    ਨਹੀਂਤਾਂ
      hb_glyph_info_ਸੈੱਟ_glyph_props (&ਬਫਰ->cur(), props);
  }
  ਖਾਲੀ ਸੈੱਟ_glyph_class_props (hb_glyph_info_t &info,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ props,
			       hb_codepoint_t glyph_ਸੂਚਕਾਂਕ)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_ਸੈੱਟ_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_ਸੂਚਕਾਂਕ));
  }
  ਖਾਲੀ ਸੈੱਟ_glyph_class_props (hb_glyph_info_t &info)
  {
    ਸੈੱਟ_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  ਖਾਲੀ replace_glyph (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ)
  {
    ਸੈੱਟ_glyph_class (glyph_ਸੂਚਕਾਂਕ);
    (ਖਾਲੀ) ਬਫਰ->replace_glyph (glyph_ਸੂਚਕਾਂਕ);
  }
  ਖਾਲੀ replace_glyph_inplace (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ)
  {
    ਸੈੱਟ_glyph_class (glyph_ਸੂਚਕਾਂਕ);
    ਬਫਰ->cur().codepoint = glyph_ਸੂਚਕਾਂਕ;
  }
  ਖਾਲੀ replace_glyph_with_ligature (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ class_guess)
  {
    ਸੈੱਟ_glyph_class (glyph_ਸੂਚਕਾਂਕ, class_guess, ਸੱਚ);
    (ਖਾਲੀ) ਬਫਰ->replace_glyph (glyph_ਸੂਚਕਾਂਕ);
  }
  ਖਾਲੀ output_glyph_for_component (hb_codepoint_t glyph_ਸੂਚਕਾਂਕ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ class_guess)
  {
    ਸੈੱਟ_glyph_class (glyph_ਸੂਚਕਾਂਕ, class_guess, ਝੂਠ, ਸੱਚ);
    (ਖਾਲੀ) ਬਫਰ->output_glyph (glyph_ਸੂਚਕਾਂਕ);
  }
};

ਗਿਣਤੀ ਵਰਗ hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

ਬਣਤਰ hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ collect_ਦੂਜਾ_glyphs_2 (ਅਚਲ T &obj,
					     hb_ਸੈੱਟ_digest_t *digest,
					     hb_ਤਰਜੀਹ<1>) HB_RETURN (ਖਾਲੀ, obj.collect_ਦੂਜਾ_glyphs (digest) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ collect_ਦੂਜਾ_glyphs_2 (ਅਚਲ T &obj HB_ਅਣਵਰਤਿਆ,
					     hb_ਸੈੱਟ_digest_t *digest,
					     hb_ਤਰਜੀਹ<0>)
  { *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ (); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_ (ਅਚਲ T *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache, hb_ਤਰਜੀਹ<1>) HB_RETURN (ਤਾਰਕਿਕ, obj->apply (c, external_cache) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_ (ਅਚਲ T *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache, hb_ਤਰਜੀਹ<0>) HB_RETURN (ਤਾਰਕਿਕ, obj->apply (c) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ apply_ਮੰਜ਼ਿਲ (ਅਚਲ ਖਾਲੀ *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache)
  {
    ਅਚਲ T *typed_obj = (ਅਚਲ T *) obj;
    ਵਾਪਸ apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_cached_ (ਅਚਲ T *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache, hb_ਤਰਜੀਹ<2>) HB_RETURN (ਤਾਰਕਿਕ, obj->apply_cached (c, external_cache) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_cached_ (ਅਚਲ T *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache, hb_ਤਰਜੀਹ<1>) HB_RETURN (ਤਾਰਕਿਕ, obj->apply (c, external_cache) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ apply_cached_ (ਅਚਲ T *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache, hb_ਤਰਜੀਹ<0>) HB_RETURN (ਤਾਰਕਿਕ, obj->apply (c) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ apply_cached_ਮੰਜ਼ਿਲ (ਅਚਲ ਖਾਲੀ *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache)
  {
    ਅਚਲ T *typed_obj = (ਅਚਲ T *) obj;
    ਵਾਪਸ apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_ਤਰਜੀਹ<1>) HB_RETURN (ਤਾਰਕਿਕ, T::cache_func (c, op) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T=ਖਾਲੀ>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_ਅਣਵਰਤਿਆ,
				  hb_ਤਰਜੀਹ<0>) { ਵਾਪਸ ਝੂਠ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cache_func_ਮੰਜ਼ਿਲ (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    ਵਾਪਸ cache_func_<Type> (c, op, hb_prioritize);
  }
#ਜੇ_ਅੰਤ

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*hb_apply_func_t) (ਅਚਲ ਖਾਲੀ *obj, hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache);
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  ਬਣਤਰ hb_applicable_t
  {
    ਮਿੱਤਰ ਬਣਤਰ hb_accelerate_subtables_context_t;
    ਮਿੱਤਰ ਬਣਤਰ hb_ot_layout_lookup_accelerator_t;

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
    ਖਾਲੀ init (ਅਚਲ T &obj_,
	       hb_apply_func_t apply_func_
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , ਖਾਲੀ *external_cache_
#ਜੇ_ਅੰਤ
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#ਜੇ_ਅੰਤ
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ digest.may_have (c->ਬਫਰ->cur().codepoint) && apply_func (obj, c, ਖਾਲੀ_ਸੂਚਕ);
    }
    ਤਾਰਕਿਕ apply_no_digest (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ apply_func (obj, c, ਖਾਲੀ_ਸੂਚਕ);
    }
#ਨਹੀਂਤਾਂ
    ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ digest.may_have (c->ਬਫਰ->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    ਤਾਰਕਿਕ apply_cached (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ digest.may_have (c->ਬਫਰ->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    ਤਾਰਕਿਕ apply_no_digest (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ apply_func (obj, c, external_cache);
    }
    ਤਾਰਕਿਕ apply_cached_no_digest (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ apply_cached_func (obj, c, external_cache);
    }

    ਤਾਰਕਿਕ cache_enter (hb_ot_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    ਖਾਲੀ cache_leave (hb_ot_apply_context_t *c) ਅਚਲ
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#ਜੇ_ਅੰਤ

    ਨਿਜੀ:
    ਅਚਲ ਖਾਲੀ *obj;
    hb_apply_func_t apply_func;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    ਖਾਲੀ *external_cache;
#ਜੇ_ਅੰਤ
    hb_ਸੈੱਟ_digest_t digest;
  };

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਵੈਚਾਲਿਤ cache_ਲਾਗਤ (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>) HB_AUTO_RETURN ( obj.cache_ਲਾਗਤ () )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਵੈਚਾਲਿਤ cache_ਲਾਗਤ (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>) HB_AUTO_RETURN ( 0u )

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਵੈਚਾਲਿਤ external_cache_create (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਵੈਚਾਲਿਤ external_cache_create (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>) HB_AUTO_RETURN ( ਖਾਲੀ_ਸੂਚਕ )
#ਜੇ_ਅੰਤ

  /* Dispatch interface. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj)
  {
    collect_ਦੂਜਾ_glyphs_2 (obj, &digest_ਦੂਜਾ, hb_prioritize);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਖਾਲੀ *external_cache = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#ਜੇ_ਅੰਤ

    hb_applicable_t *ਇੰਦਰਾਜ = &array[i++];

    ਇੰਦਰਾਜ->init (obj,
		 apply_ਮੰਜ਼ਿਲ<T>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_ਮੰਜ਼ਿਲ<T>
		 , cache_func_ਮੰਜ਼ਿਲ<T>
		 , external_cache
#ਜੇ_ਅੰਤ
		 );

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ = cache_ਲਾਗਤ (obj, hb_prioritize);
    ਜੇ (ਲਾਗਤ > subtable_cache_user_ਲਾਗਤ)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_ਲਾਗਤ = ਲਾਗਤ;
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();
  }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
  hb_ਸੈੱਟ_digest_t digest_ਦੂਜਾ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_cache_user_idx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_cache_user_ਲਾਗਤ = 0;
#ਜੇ_ਅੰਤ
};


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*intersects_func_t) (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਖਾਲੀ *cache);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*intersected_glyphs_func_t) (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ value, hb_ਸੈੱਟ_t *intersected_glyphs, ਖਾਲੀ *cache);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*collect_glyphs_func_t) (hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*ਮੇਲ_func_t) (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ (*get_ਮੁੱਲ_func_t) (hb_glyph_info_t &info, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
ਬਣਤਰ hb_ot_layout_ruleset_digest_t
{
  ਖਾਲੀ init () { ਨਕਾਬ = 0; }
  ਖਾਲੀ ਸੈੱਟ_ਭਰਿਆ () { ਨਕਾਬ = (uint64_t) -1; }
  ਖਾਲੀ ਜੋੜੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ value) { ਨਕਾਬ |= (uint64_t) 1 << (value & 63); }
  ਤਾਰਕਿਕ may_have (ਬਿਨਾਨਿਸ਼ਾਨੀ value) ਅਚਲ { ਵਾਪਸ ਨਕਾਬ & ((uint64_t) 1 << (value & 63)); }

  uint64_t ਨਕਾਬ;
};

ਬਣਤਰ Context_ਘੇਰਾ_Funcs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
ਬਣਤਰ ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
ਬਣਤਰ ContextApplyFuncs
{
  ਮੇਲ_func_t ਮੇਲ;
};
ਬਣਤਰ ChainContextApplyFuncs
{
  ਮੇਲ_func_t ਮੇਲ[3];
};


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ intersects_glyph (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *cache HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ glyphs->has (value);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ intersects_class (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਖਾਲੀ *cache)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  ਜੇ (map->has (value, &cached_v))
    ਵਾਪਸ *cached_v;

  ਤਾਰਕਿਕ v = class_def.intersects_class (glyphs, value);
  map->ਸੈੱਟ (value, v);

  ਵਾਪਸ v;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ intersects_coverage (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਖਾਲੀ *cache HB_ਅਣਵਰਤਿਆ)
{
  Offset32_ਮੰਜ਼ਿਲ<Coverage> coverage;
  coverage = value;
  ਵਾਪਸ (ਅੰਕੜੇ+coverage).intersects (glyphs);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ intersected_glyph (ਅਚਲ hb_ਸੈੱਟ_t *glyphs HB_ਅਣਵਰਤਿਆ, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ value, hb_ਸੈੱਟ_t *intersected_glyphs, HB_ਅਣਵਰਤਿਆ ਖਾਲੀ *cache)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ g = reinterpret_cast<ਅਚਲ HBUINT16 *>(ਅੰਕੜੇ)[value];
  intersected_glyphs->ਜੋੜੋ (g);
}

ਵਰਤੋਂ intersected_class_cache_t = hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸੈੱਟ_t>;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ intersected_class_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ value, hb_ਸੈੱਟ_t *intersected_glyphs, ਖਾਲੀ *cache)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_ਸੈੱਟ_t *cached_v;
  ਜੇ (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    ਵਾਪਸ;
  }

  hb_ਸੈੱਟ_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->ਸੈੱਟ (value, std::move (v));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ intersected_coverage_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ value, hb_ਸੈੱਟ_t *intersected_glyphs, HB_ਅਣਵਰਤਿਆ ਖਾਲੀ *cache)
{
  Offset32_ਮੰਜ਼ਿਲ<Coverage> coverage;
  coverage = value;
  (ਅੰਕੜੇ+coverage).intersect_ਸੈੱਟ (*glyphs, *intersected_glyphs);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ array_is_subset_ਦਾ (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				       glyph_ਅੰਕੜੇ_t values,
				       intersects_func_t intersects_func,
				       ਅਚਲ ਖਾਲੀ *intersects_ਅੰਕੜੇ,
				       ਖਾਲੀ *cache)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (!intersects_func (glyphs, values[i], intersects_ਅੰਕੜੇ, cache)) ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ ਸੱਚ;
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ collect_glyph (hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  glyphs->ਜੋੜੋ (value);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ collect_class (hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);
  class_def.collect_class (glyphs, value);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ collect_coverage (hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  Offset32_ਮੰਜ਼ਿਲ<Coverage> coverage;
  coverage = value;
  (ਅੰਕੜੇ+coverage).collect_coverage (glyphs);
}

ਸਤਰਅੰਦਰ ਅਚਲ hb_ਸੈੱਟ_t *
hb_depend_context_t::ਲੱਭੋ_or_create_glyph_ਸੈੱਟ (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
                                                ਬਿਨਾਨਿਸ਼ਾਨੀ value,
                                                glyph_ਸੈੱਟ_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_ਸੈੱਟ_cache_ਕੁੰਜੀ_t key (ਅੰਕੜੇ, value, type);
  hb_codepoint_t *existing = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (glyph_ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.has (key, &existing))
    ਵਾਪਸ glyph_sets[*existing].get ();

  hb_ਸੈੱਟ_t *ਸੈੱਟ = hb_object_create<hb_ਸੈੱਟ_t> ();
  ਜੇ (unlikely (!ਸੈੱਟ))
  {
    depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (type == GLYPH_ਸੈੱਟ_COVERAGE)
    collect_coverage (ਸੈੱਟ, value, ਅੰਕੜੇ);
  ਨਹੀਂਤਾਂ ਜੇ (type == GLYPH_ਸੈੱਟ_CLASS)
    collect_class (ਸੈੱਟ, value, ਅੰਕੜੇ);
  ਨਹੀਂਤਾਂ
    ਸੈੱਟ->ਜੋੜੋ (value);
  ਜੇ (unlikely (ਸੈੱਟ->ਵਿੱਚ_error ()))
  {
    hb_ਸੈੱਟ_destroy (ਸੈੱਟ);
    depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_codepoint_t ਸੂਚਕਾਂਕ_2 = glyph_sets.ਲੰਮਾਈ;
  ਜੇ (unlikely (!glyph_sets.ਧੱਕੋ_or_fail (hb::unique_ptr<hb_ਸੈੱਟ_t> {ਸੈੱਟ})))
  {
    depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਜੇ (unlikely (!glyph_ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.ਸੈੱਟ (key, ਸੂਚਕਾਂਕ_2)))
  {
    depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਵਾਪਸ glyph_sets[ਸੂਚਕਾਂਕ_2].get ();
}

ਸਤਰਅੰਦਰ hb_codepoint_t
hb_depend_context_t::ਲੱਭੋ_or_create_cached_context_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
{
  uintptr_t key = (uintptr_t) ਸੈੱਟ;
  hb_codepoint_t *existing = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (cached_context_ਸੈੱਟ_indices.has (key, &existing))
    ਵਾਪਸ *existing;

  hb_codepoint_t ਸੂਚਕਾਂਕ_2 = depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (*ਸੈੱਟ);
  ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 == HB_CODEPOINT_INVALID))
    ਵਾਪਸ HB_CODEPOINT_INVALID;
  ਜੇ (unlikely (!cached_context_ਸੈੱਟ_indices.ਸੈੱਟ (key, ਸੂਚਕਾਂਕ_2)))
    ਵਾਪਸ depend_ਅੰਕੜੇ->fail_invalid ();
  ਵਾਪਸ ਸੂਚਕਾਂਕ_2;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ collect_array (hb_collect_glyphs_context_t *c HB_ਅਣਵਰਤਿਆ,
				  hb_ਸੈੱਟ_t *glyphs,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				  glyph_ਅੰਕੜੇ_t values,
				  collect_glyphs_func_t collect_func,
				  ਅਚਲ ਖਾਲੀ *collect_ਅੰਕੜੇ)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    collect_func (glyphs, values[i], collect_ਅੰਕੜੇ);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_always (hb_glyph_info_t &info HB_ਅਣਵਰਤਿਆ, ਬਿਨਾਨਿਸ਼ਾਨੀ value HB_ਅਣਵਰਤਿਆ, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ਸੱਚ;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_glyph (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ info.codepoint == value;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_class (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);
  ਵਾਪਸ class_def.get_class (info.codepoint) == value;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_ਮੁੱਲ (hb_glyph_info_t &info, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *> (ਅੰਕੜੇ);
  ਵਾਪਸ class_def.get_class (info.codepoint);
}
ਬਣਤਰ ਮੇਲ_class_cache_ਅੰਕੜੇ_t
{
  ਅਚਲ ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_class_cache (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ਸਵੈਚਾਲਿਤ &cache_ਅੰਕੜੇ = *reinterpret_cast<ਅਚਲ ਮੇਲ_class_cache_ਅੰਕੜੇ_t *> (ਅੰਕੜੇ);
  ਵਾਪਸ cache_ਅੰਕੜੇ.class_def->get_class (info.codepoint, cache_ਅੰਕੜੇ.cache) == value;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_cache_ਮੁੱਲ (hb_glyph_info_t &info, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ਸਵੈਚਾਲਿਤ &cache_ਅੰਕੜੇ = *reinterpret_cast<ਅਚਲ ਮੇਲ_class_cache_ਅੰਕੜੇ_t *> (ਅੰਕੜੇ);
  ਵਾਪਸ cache_ਅੰਕੜੇ.class_def->get_class (info.codepoint, cache_ਅੰਕੜੇ.cache);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_cached (ਅਚਲ ClassDef &class_def, hb_glyph_info_t &info)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ klass = info.syllable();
  ਜੇ (klass < 255)
    ਵਾਪਸ klass;
  klass = class_def.get_class (info.codepoint);
  ਜੇ (likely (klass < 255))
    info.syllable() = klass;
  ਵਾਪਸ klass;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_class_cached (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);
  ਵਾਪਸ get_class_cached (class_def, info) == value;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_cached_ਮੁੱਲ (hb_glyph_info_t &info, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *> (ਅੰਕੜੇ);
  ਵਾਪਸ get_class_cached (class_def, info);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_cached1 (ਅਚਲ ClassDef &class_def, hb_glyph_info_t &info)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ klass = info.syllable() & 0x0F;
  ਜੇ (klass < 15)
    ਵਾਪਸ klass;
  klass = class_def.get_class (info.codepoint);
  ਜੇ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  ਵਾਪਸ klass;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_class_cached1 (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);
  ਵਾਪਸ get_class_cached1 (class_def, info) == value;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_cached2 (ਅਚਲ ClassDef &class_def, hb_glyph_info_t &info)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ klass = (info.syllable() & 0xF0) >> 4;
  ਜੇ (klass < 15)
    ਵਾਪਸ klass;
  klass = class_def.get_class (info.codepoint);
  ਜੇ (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  ਵਾਪਸ klass;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_class_cached2 (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *>(ਅੰਕੜੇ);
  ਵਾਪਸ get_class_cached2 (class_def, info) == value;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_class_cached2_ਮੁੱਲ (hb_glyph_info_t &info, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਅਚਲ ClassDef &class_def = *reinterpret_cast<ਅਚਲ ClassDef *> (ਅੰਕੜੇ);
  ਵਾਪਸ get_class_cached2 (class_def, info);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਮੇਲ_coverage (hb_glyph_info_t &info, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ)
{
  Offset32_ਮੰਜ਼ਿਲ<Coverage> coverage;
  coverage = value;
  ਵਾਪਸ (ਅੰਕੜੇ+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ would_ਮੇਲ_ਨਿਵੇਸ਼ (hb_would_apply_context_t *c,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, /* Including the first glyph (not matched) */
				      glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
				      ਮੇਲ_func_t ਮੇਲ_func_2,
				      ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2)
{
  ਜੇ (ਗਿਣਤੀ_2 != c->len)
    ਵਾਪਸ ਝੂਠ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    ਜੇ (likely (!ਮੇਲ_func_2 (info, ਨਿਵੇਸ਼[i - 1], ਮੇਲ_ਅੰਕੜੇ_2)))
      ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
HB_ALWAYS_INLINE
#ਨਹੀਂਤਾਂ
ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਤਾਰਕਿਕ ਮੇਲ_ਨਿਵੇਸ਼ (hb_ot_apply_context_t *c,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, /* Including the first glyph (not matched) */
			 glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
			 ਮੇਲ_func_t ਮੇਲ_func_2,
			 ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਿਰਾ_position,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *p_ਕੁੱਲ_component_ਗਿਣਤੀ = ਖਾਲੀ_ਸੂਚਕ)
{
  TRACE_APPLY (ਖਾਲੀ_ਸੂਚਕ);

  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

  ਜੇ (ਗਿਣਤੀ_2 == 1)
  {
    *ਸਿਰਾ_position = ਬਫਰ->idx + 1;
    c->ਮੇਲ_positions[0] = ਬਫਰ->idx;
    ਜੇ (p_ਕੁੱਲ_component_ਗਿਣਤੀ)
      *p_ਕੁੱਲ_component_ਗਿਣਤੀ = hb_glyph_info_get_lig_ਅੰਕ_comps (&ਬਫਰ->cur());
    return_trace (ਸੱਚ);
  }

  ਜੇ (unlikely (ਗਿਣਤੀ_2 > HB_MAX_CONTEXT_ਲੰਮਾਈ)) return_trace (ਝੂਠ);

  ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_ਨਿਵੇਸ਼;
  skippy_iter.reset (ਬਫਰ->idx);
  skippy_iter.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_func_2, ਮੇਲ_ਅੰਕੜੇ_2);
  skippy_iter.ਸੈੱਟ_glyph_ਅੰਕੜੇ (ਨਿਵੇਸ਼);

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

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੱਲ_component_ਗਿਣਤੀ = 0;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_lig_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->cur());
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_lig_comp = _hb_glyph_info_get_lig_comp (&ਬਫਰ->cur());

  ਗਿਣਤੀ {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ;
    ਜੇ (!skippy_iter.next (&unsafe_ਮੰਜ਼ਿਲ))
    {
      *ਸਿਰਾ_position = unsafe_ਮੰਜ਼ਿਲ;
      return_trace (ਝੂਠ);
    }

    ਜੇ (unlikely (i + 1 > c->ਮੇਲ_positions.ਲੰਮਾਈ &&
		  !c->ਮੇਲ_positions.resize_dirty  (i + 1)))
      return_trace (ਝੂਠ);
    c->ਮੇਲ_positions.arrayZ[i] = skippy_iter.idx;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_lig_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->info[skippy_iter.idx]);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_lig_comp = _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[skippy_iter.idx]);

    ਜੇ (ਪਹਿਲਾ_lig_id && ਪਹਿਲਾ_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      ਜੇ (ਪਹਿਲਾ_lig_id != this_lig_id || ਪਹਿਲਾ_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	ਜੇ (ligbase == LIGBASE_NOT_CHECKED)
	{
	  ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
	  ਅਚਲ ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = ਬਫਰ->ਬਾਹਰਲਾ_info;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਬਫਰ->ਬਾਹਰਲਾ_len;
	  ਜਦੋਂ (j && _hb_glyph_info_get_lig_id (&ਬਾਹਰਲਾ[j - 1]) == ਪਹਿਲਾ_lig_id)
	  {
	    ਜੇ (_hb_glyph_info_get_lig_comp (&ਬਾਹਰਲਾ[j - 1]) == 0)
	    {
	      j--;
	      ਮਿਲਿਆ = ਸੱਚ;
	      ਰੋਕੋ;
	    }
	    j--;
	  }

	  ਜੇ (ਮਿਲਿਆ && skippy_iter.may_skip (ਬਾਹਰਲਾ[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  ਨਹੀਂਤਾਂ
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	ਜੇ (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (ਝੂਠ);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      ਜੇ (this_lig_id && this_lig_comp && (this_lig_id != ਪਹਿਲਾ_lig_id))
	return_trace (ਝੂਠ);
    }

    ਕੁੱਲ_component_ਗਿਣਤੀ += hb_glyph_info_get_lig_ਅੰਕ_comps_ਵਿੱਚ_ligation (&ਬਫਰ->info[skippy_iter.idx]);
  }

  *ਸਿਰਾ_position = skippy_iter.idx + 1;

  ਜੇ (p_ਕੁੱਲ_component_ਗਿਣਤੀ)
  {
    ਕੁੱਲ_component_ਗਿਣਤੀ += hb_glyph_info_get_lig_ਅੰਕ_comps (&ਬਫਰ->cur());
    *p_ਕੁੱਲ_component_ਗਿਣਤੀ = ਕੁੱਲ_component_ਗਿਣਤੀ;
  }

  c->ਮੇਲ_positions.arrayZ[0] = ਬਫਰ->idx;

  return_trace (ਸੱਚ);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ligate_ਨਿਵੇਸ਼ (hb_ot_apply_context_t *c,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, /* Including the first glyph */
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_ਸਿਰਾ,
				 hb_codepoint_t lig_glyph,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੱਲ_component_ਗਿਣਤੀ)
{
  TRACE_APPLY (ਖਾਲੀ_ਸੂਚਕ);

  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

  ਬਫਰ->ਮਿਲਾਓ_clusters (ਬਫਰ->idx, ਮੇਲ_ਸਿਰਾ);

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

  ਤਾਰਕਿਕ is_ਅਧਾਰ_ligature = hb_glyph_info_is_ਅਧਾਰ_glyph (&ਬਫਰ->info[c->ਮੇਲ_positions.arrayZ[0]]);
  ਤਾਰਕਿਕ is_mark_ligature = _hb_glyph_info_is_mark (&ਬਫਰ->info[c->ਮੇਲ_positions.arrayZ[0]]);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (!_hb_glyph_info_is_mark (&ਬਫਰ->info[c->ਮੇਲ_positions.arrayZ[i]]))
    {
      is_ਅਧਾਰ_ligature = ਝੂਠ;
      is_mark_ligature = ਝੂਠ;
      ਰੋਕੋ;
    }
  ਤਾਰਕਿਕ is_ligature = !is_ਅਧਾਰ_ligature && !is_mark_ligature;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_id = is_ligature ? _hb_allocate_lig_id (ਬਫਰ) : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_lig_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->cur());
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਅੰਕ_components = hb_glyph_info_get_lig_ਅੰਕ_comps (&ਬਫਰ->cur());
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ components_so_far = ਆਖਰੀ_ਅੰਕ_components;

  ਜੇ (is_ligature)
  {
    hb_glyph_info_ਸੈੱਟ_lig_props_for_ligature (&ਬਫਰ->cur(), lig_id, ਕੁੱਲ_component_ਗਿਣਤੀ);
    ਜੇ (_hb_glyph_info_get_general_category (&ਬਫਰ->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_ਸੈੱਟ_general_category (&ਬਫਰ->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
  {
    ਜਦੋਂ (ਬਫਰ->idx < c->ਮੇਲ_positions.arrayZ[i] && ਬਫਰ->successful)
    {
      ਜੇ (is_ligature)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_comp = _hb_glyph_info_get_lig_comp (&ਬਫਰ->cur());
	ਜੇ (this_comp == 0)
	  this_comp = ਆਖਰੀ_ਅੰਕ_components;
	ਪ੍ਰਤਿਜ੍ਞਾ (components_so_far >= ਆਖਰੀ_ਅੰਕ_components);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_lig_comp = components_so_far - ਆਖਰੀ_ਅੰਕ_components +
				    hb_min (this_comp, ਆਖਰੀ_ਅੰਕ_components);
	  hb_glyph_info_ਸੈੱਟ_lig_props_for_mark (&ਬਫਰ->cur(), lig_id, new_lig_comp);
      }
      (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
    }

    ਆਖਰੀ_lig_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->cur());
    ਆਖਰੀ_ਅੰਕ_components = hb_glyph_info_get_lig_ਅੰਕ_comps_ਵਿੱਚ_ligation (&ਬਫਰ->cur());
    components_so_far += ਆਖਰੀ_ਅੰਕ_components;

    /* Skip the base glyph */
    ਬਫਰ->idx++;
  }

  ਜੇ (!is_mark_ligature && ਆਖਰੀ_lig_id)
  {
    /* Re-adjust components for any marks following. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਬਫਰ->idx; i < ਬਫਰ->len; ++i)
    {
      ਜੇ (ਆਖਰੀ_lig_id != _hb_glyph_info_get_lig_id (&ਬਫਰ->info[i])) ਰੋਕੋ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ this_comp = _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[i]);
      ਜੇ (!this_comp) ਰੋਕੋ;

      ਪ੍ਰਤਿਜ੍ਞਾ (components_so_far >= ਆਖਰੀ_ਅੰਕ_components);
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_lig_comp = components_so_far - ਆਖਰੀ_ਅੰਕ_components +
			      hb_min (this_comp, ਆਖਰੀ_ਅੰਕ_components);
      hb_glyph_info_ਸੈੱਟ_lig_props_for_mark (&ਬਫਰ->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (ਸੱਚ);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
HB_ALWAYS_INLINE
#ਨਹੀਂਤਾਂ
ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਤਾਰਕਿਕ ਮੇਲ_backtrack (hb_ot_apply_context_t *c,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			     glyph_ਅੰਕੜੇ_t backtrack,
			     ਮੇਲ_func_t ਮੇਲ_func_2,
			     ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮੇਲ_ਸ਼ੁਰੂ)
{
  TRACE_APPLY (ਖਾਲੀ_ਸੂਚਕ);

  ਜੇ (!ਗਿਣਤੀ_2)
  {
    *ਮੇਲ_ਸ਼ੁਰੂ = c->ਬਫਰ->backtrack_len ();
    return_trace (ਸੱਚ);
  }

  ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_context;
  skippy_iter.reset_ਪਿੱਠ (c->ਬਫਰ->backtrack_len ());
  skippy_iter.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_func_2, ਮੇਲ_ਅੰਕੜੇ_2);
  skippy_iter.ਸੈੱਟ_glyph_ਅੰਕੜੇ (backtrack);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_from;
    ਜੇ (!skippy_iter.prev (&unsafe_from))
    {
      *ਮੇਲ_ਸ਼ੁਰੂ = unsafe_from;
      return_trace (ਝੂਠ);
    }
  }

  *ਮੇਲ_ਸ਼ੁਰੂ = skippy_iter.idx;
  return_trace (ਸੱਚ);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
HB_ALWAYS_INLINE
#ਨਹੀਂਤਾਂ
ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਤਾਰਕਿਕ ਮੇਲ_lookahead (hb_ot_apply_context_t *c,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			     glyph_ਅੰਕੜੇ_t lookahead,
			     ਮੇਲ_func_t ਮੇਲ_func_2,
			     ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਿਰਾ_ਸੂਚਕਾਂਕ)
{
  TRACE_APPLY (ਖਾਲੀ_ਸੂਚਕ);

  ਜੇ (!ਗਿਣਤੀ_2)
  {
    *ਸਿਰਾ_ਸੂਚਕਾਂਕ = ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ;
    return_trace (ਸੱਚ);
  }

  ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_context;
  ਪ੍ਰਤਿਜ੍ਞਾ (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ >= 1);
  skippy_iter.reset (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ - 1);
  skippy_iter.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_func_2, ਮੇਲ_ਅੰਕੜੇ_2);
  skippy_iter.ਸੈੱਟ_glyph_ਅੰਕੜੇ (lookahead);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ;
    ਜੇ (!skippy_iter.next (&unsafe_ਮੰਜ਼ਿਲ))
    {
      *ਸਿਰਾ_ਸੂਚਕਾਂਕ = unsafe_ਮੰਜ਼ਿਲ;
      return_trace (ਝੂਠ);
    }
  }

  *ਸਿਰਾ_ਸੂਚਕਾਂਕ = skippy_iter.idx + 1;
  return_trace (ਸੱਚ);
}



ਬਣਤਰ Lookup_ਰਿਕਾਰਡ
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ hb_map_t         *lookup_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    return_trace (c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->lookup_List_ਸੂਚਕਾਂਕ, lookup_map->get (lookup_List_ਸੂਚਕਾਂਕ), HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT16	sequence_ਸੂਚਕਾਂਕ;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_ਸੂਚਕਾਂਕ;	/* Lookup to apply to that
					 * position--zero--based */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ਅਚਲ hb_array_t<ਅਚਲ Lookup_ਰਿਕਾਰਡ> lookup_ਰਿਕਾਰਡਾਂ,
						     ਅਚਲ hb_map_t *lookup_map)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
  ਲਈ (ਅਚਲ Lookup_ਰਿਕਾਰਡ& r : lookup_ਰਿਕਾਰਡਾਂ)
  {
    ਜੇ (!lookup_map->has (r.lookup_List_ਸੂਚਕਾਂਕ))
      ਜਾਰੀ;

    ਜੇ (!r.serialize (c, lookup_map))
      ਵਾਪਸ 0;

    ਗਿਣਤੀ_2++;
  }
  ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਗਿਣਤੀ ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ hb_ਸੈੱਟ_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ value,
			ਤਾਰਕਿਕ filter_class_by_ਮਾਪਾ,
			hb_ਸੈੱਟ_t *scratch,
			hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *compact_workspace = ਖਾਲੀ_ਸੂਚਕ)
{
  ਚੋਣ (context_format)
  {
    ਹਾਲਤ ContextFormat::SimpleContext:
      ਵਾਪਸ c->ਲੱਭੋ_or_create_glyph_ਸੈੱਟ (ਖਾਲੀ_ਸੂਚਕ, value,
                                           hb_depend_context_t::GLYPH_ਸੈੱਟ_SINGLE);
    ਹਾਲਤ ContextFormat::ClassBasedContext:
    {
      ਅਚਲ hb_ਸੈੱਟ_t *class_ਸੈੱਟ = c->ਲੱਭੋ_or_create_glyph_ਸੈੱਟ (
        ਅੰਕੜੇ, value, hb_depend_context_t::GLYPH_ਸੈੱਟ_CLASS);
      ਜੇ (unlikely (!class_ਸੈੱਟ))
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (!filter_class_by_ਮਾਪਾ)
	ਵਾਪਸ class_ਸੈੱਟ;
      scratch->ਸੈੱਟ (*class_ਸੈੱਟ);
      scratch->intersect (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), compact_workspace);
      ਰੋਕੋ;
    }
    ਹਾਲਤ ContextFormat::CoverageBasedContext:
      ਵਾਪਸ c->ਲੱਭੋ_or_create_glyph_ਸੈੱਟ (
        ਅੰਕੜੇ, value, hb_depend_context_t::GLYPH_ਸੈੱਟ_COVERAGE);
  }

  ਜੇ (unlikely (scratch->ਵਿੱਚ_error ()))
  {
    c->depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਵਾਪਸ scratch;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ context_ਘੇਰਾ_recurse_lookups (hb_ਘੇਰਾ_context_t *c,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਵੇਸ਼_ਗਿਣਤੀ, glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਗਿਣਤੀ_2,
					     ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[] /* Array of LookupRecords--in design order */,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ value,
					     ContextFormat context_format,
					     ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ਖਾਲੀ *cache)
{
  hb_ਸੈੱਟ_t covered_seq_indicies;
  hb_ਸੈੱਟ_t pos_glyphs;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < lookup_ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ_2 = lookup_ਰਿਕਾਰਡ[i].sequence_ਸੂਚਕਾਂਕ;
    ਜੇ (seq_ਸੂਚਕਾਂਕ_2 >= ਨਿਵੇਸ਼_ਗਿਣਤੀ) ਜਾਰੀ;

    ਤਾਰਕਿਕ has_pos_glyphs = ਝੂਠ;

    ਜੇ (!covered_seq_indicies.has (seq_ਸੂਚਕਾਂਕ_2))
    {
      has_pos_glyphs = ਸੱਚ;
      pos_glyphs.clear ();
      ਜੇ (seq_ਸੂਚਕਾਂਕ_2 == 0)
      {
        ਚੋਣ (context_format) {
        ਹਾਲਤ ContextFormat::SimpleContext:
          pos_glyphs.ਜੋੜੋ (value);
          ਰੋਕੋ;
        ਹਾਲਤ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), ਅੰਕੜੇ, value, &pos_glyphs, cache);
          ਰੋਕੋ;
        ਹਾਲਤ ContextFormat::CoverageBasedContext:
          pos_glyphs.ਸੈੱਟ (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs ());
          ਰੋਕੋ;
        }
      }
      ਨਹੀਂਤਾਂ
      {
        ਅਚਲ ਖਾਲੀ *ਨਿਵੇਸ਼_ਅੰਕੜੇ = ਨਿਵੇਸ਼.ਅੰਕੜੇ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਵੇਸ਼_ਮੁੱਲ = seq_ਸੂਚਕਾਂਕ_2 - 1;
        ਜੇ (context_format != ContextFormat::SimpleContext)
        {
          ਨਿਵੇਸ਼_ਅੰਕੜੇ = ਅੰਕੜੇ;
          ਨਿਵੇਸ਼_ਮੁੱਲ = ਨਿਵੇਸ਼[seq_ਸੂਚਕਾਂਕ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, ਨਿਵੇਸ਼_ਅੰਕੜੇ, ਨਿਵੇਸ਼_ਮੁੱਲ, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.ਜੋੜੋ (seq_ਸੂਚਕਾਂਕ_2);
    hb_ਸੈੱਟ_t *cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs))
      ਵਾਪਸ;
    ਜੇ (has_pos_glyphs) {
      *cur_ਸਰਗਰਮ_glyphs = std::move (pos_glyphs);
    } ਨਹੀਂਤਾਂ {
      *cur_ਸਰਗਰਮ_glyphs = *c->glyphs;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ_2 = ਨਿਵੇਸ਼_ਗਿਣਤੀ;
    ਜੇ (context_format == ContextFormat::CoverageBasedContext)
      ਸਿਰਾ_ਸੂਚਕਾਂਕ_2 += 1;

    c->recurse (lookup_ਰਿਕਾਰਡ[i].lookup_List_ਸੂਚਕਾਂਕ, &covered_seq_indicies, seq_ਸੂਚਕਾਂਕ_2, ਸਿਰਾ_ਸੂਚਕਾਂਕ_2);

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
context_depend_sets_are_cached (hb_depend_context_t *c,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਵੇਸ਼_ਗਿਣਤੀ_2,
				ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਗਿਣਤੀ,
				ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡਾਂ_2[],
				ਅਚਲ ਖਾਲੀ *ਨਿਯਮ_2,
				ਬਿਨਾਨਿਸ਼ਾਨੀ value,
				ContextFormat context_format,
				ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
				hb_ਸਦਿਸ਼_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t ਸਰਗਰਮ_idx = HB_CODEPOINT_INVALID;
  ਤਾਰਕਿਕ have_ਸਰਗਰਮ_idx = ਝੂਠ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < lookup_ਗਿਣਤੀ; i++)
  {
    ਜੇ (lookup_ਰਿਕਾਰਡਾਂ_2[i].sequence_ਸੂਚਕਾਂਕ >= ਨਿਵੇਸ਼_ਗਿਣਤੀ_2)
      ਜਾਰੀ;

    ਜੇ (!have_ਸਰਗਰਮ_idx)
    {
      ਜੇ (unlikely (!c->get_ਮਾਪਾ_ਸਰਗਰਮ_glyphs_ਸੂਚਕਾਂਕ (&ਸਰਗਰਮ_idx)))
	ਵਾਪਸ ਝੂਠ;
      have_ਸਰਗਰਮ_idx = ਸੱਚ;
    }

    hb_depend_context_t::context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t key (
      ਨਿਯਮ_2, &lookup_ਰਿਕਾਰਡਾਂ_2[i], ਅੰਕੜੇ, ਸਰਗਰਮ_idx, value, context_format);
    hb_codepoint_t *cached_context_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (!c->context_ਸੈੱਟ_cache.has (key, &cached_context_ਸੈੱਟ))
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely (!cached_context_sets->ਧੱਕੋ_or_fail (*cached_context_ਸੈੱਟ)))
      ਵਾਪਸ c->depend_ਅੰਕੜੇ->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  ਵਾਪਸ have_ਸਰਗਰਮ_idx;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ context_depend_recurse_lookups (hb_depend_context_t *c,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਵੇਸ਼_ਗਿਣਤੀ, glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਗਿਣਤੀ_2,
					     ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[] /* Array of LookupRecords--in design order */,
					     ਅਚਲ ਖਾਲੀ *ਨਿਯਮ_2,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ value,
					     ContextFormat context_format,
					     ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     ਖਾਲੀ *cache,
					     ਅਚਲ hb_ਸੈੱਟ_t &preliminary_context,
					     ਅਚਲ hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> *ਨਿਵੇਸ਼_position_glyphs,
					     ਅਚਲ hb_ਸਦਿਸ਼_t<hb_codepoint_t> *cached_context_sets,
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
  ਜੇ (unlikely (ਨਿਵੇਸ਼_ਗਿਣਤੀ > HB_MAX_CONTEXT_ਲੰਮਾਈ))
    ਵਾਪਸ;

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t covered_seq_indices;
  hb_ਸੈੱਟ_t &pos_glyphs = scratch->pos_glyphs;
  hb_ਸੈੱਟ_t &position_context = scratch->position_context;
  hb_ਸੈੱਟ_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_ਸੈੱਟ_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  ਬਿਨਾਨਿਸ਼ਾਨੀ cached_context_ਸੂਚਕਾਂਕ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < lookup_ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ_2 = lookup_ਰਿਕਾਰਡ[i].sequence_ਸੂਚਕਾਂਕ;
    ਜੇ (seq_ਸੂਚਕਾਂਕ_2 >= ਨਿਵੇਸ਼_ਗਿਣਤੀ)
      ਜਾਰੀ;

    hb_depend_context_t::context_ਸੈੱਟ_cache_ਕੁੰਜੀ_t context_cache_ਕੁੰਜੀ;
    ਜੇ (unlikely (!c->get_context_ਸੈੱਟ_cache_ਕੁੰਜੀ (ਨਿਯਮ_2, &lookup_ਰਿਕਾਰਡ[i], ਅੰਕੜੇ, value,
						 context_format, &context_cache_ਕੁੰਜੀ)))
      ਵਾਪਸ;

    hb_codepoint_t context_ਸੈੱਟ_idx;
    hb_codepoint_t *cached_context_ਸੈੱਟ_idx;
    ਜੇ (cached_context_sets)
      context_ਸੈੱਟ_idx = (*cached_context_sets)[cached_context_ਸੂਚਕਾਂਕ++];
    ਨਹੀਂਤਾਂ ਜੇ (c->context_ਸੈੱਟ_cache.has (context_cache_ਕੁੰਜੀ, &cached_context_ਸੈੱਟ_idx))
      context_ਸੈੱਟ_idx = *cached_context_ਸੈੱਟ_idx;
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (!ਨਿਵੇਸ਼_position_glyphs))
      {
	c->depend_ਅੰਕੜੇ->fail ();
	ਵਾਪਸ;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਜਦੋਂ (preliminary_context.next (&elem)) {
	ਜੇ (!(elem & HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG)) {
	  /* Direct glyph */
	  position_context.ਜੋੜੋ (elem);
	} ਨਹੀਂਤਾਂ {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.ਜੋੜੋ (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਨਿਵੇਸ਼_position_glyphs->ਲੰਮਾਈ; j++) {
	ਜੇ (j != seq_ਸੂਚਕਾਂਕ_2 && (*ਨਿਵੇਸ਼_position_glyphs)[j]->get_population () == 1) {
	  position_context.ਜੋੜੋ ((*ਨਿਵੇਸ਼_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਜਦੋਂ (disjunctive_indices.next (&elem)) {
	hb_codepoint_t ਸੈੱਟ_idx = elem & 0x7FFFFFFF;
	ਅਚਲ hb_ਸੈੱਟ_t *original_ਸੈੱਟ = c->depend_ਅੰਕੜੇ->get_ਸੈੱਟ_from_ਸੂਚਕਾਂਕ (ਸੈੱਟ_idx);
	ਜੇ (unlikely (!original_ਸੈੱਟ)) ਜਾਰੀ;

	/* Dependency context sets are never inverted. */
	ਜੇ (!original_ਸੈੱਟ->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.ਜੋੜੋ (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਨਿਵੇਸ਼_position_glyphs->ਲੰਮਾਈ; j++) {
	ਜੇ (j != seq_ਸੂਚਕਾਂਕ_2 && (*ਨਿਵੇਸ਼_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  ਜੇ (!(*ਨਿਵੇਸ਼_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    ਤਾਰਕਿਕ cached_ਸੈੱਟ = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_ਸੈੱਟ
	      ? c->ਲੱਭੋ_or_create_cached_context_ਸੈੱਟ ((*ਨਿਵੇਸ਼_position_glyphs)[j])
	      : c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (*(*ਨਿਵੇਸ਼_position_glyphs)[j]);
	    ਜੇ (unlikely (idx == HB_CODEPOINT_INVALID))
	      ਵਾਪਸ;
	    filtered_disjunctive_indices.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      ਜੇ (unlikely (position_context.ਵਿੱਚ_error () ||
		    filtered_disjunctive_indices.ਵਿੱਚ_error ()))
      {
	c->depend_ਅੰਕੜੇ->fail ();
	ਵਾਪਸ;
      }

      context_ਸੈੱਟ_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (position_context);
      ਜੇ (unlikely (!position_context.is_empty () &&
		    context_ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
	ਵਾਪਸ;

      ਜੇ (unlikely (!c->context_ਸੈੱਟ_cache.ਸੈੱਟ (context_cache_ਕੁੰਜੀ, context_ਸੈੱਟ_idx)))
      {
	c->depend_ਅੰਕੜੇ->fail ();
	ਵਾਪਸ;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
    hb_subset_depend_ਕਿਨਾਰਾ_flags_t saved_ਕਿਨਾਰਾ_flags = c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    ਤਾਰਕਿਕ nested_contextual = (saved_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = context_ਸੈੱਟ_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags = HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE;
    ਜੇ (ਨਿਵੇਸ਼_ਗਿਣਤੀ > 1) {
      c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags |= HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_FROM_CONTEXT_POSITION;
    }
    ਜੇ (nested_contextual) {
      c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags |= HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_FROM_NESTED_CONTEXT;
    }

    ਤਾਰਕਿਕ has_pos_glyphs = ਝੂਠ;

    ਜੇ (!covered_seq_indices.has (seq_ਸੂਚਕਾਂਕ_2))
    {
      has_pos_glyphs = ਸੱਚ;
      pos_glyphs.clear ();
      ਜੇ (seq_ਸੂਚਕਾਂਕ_2 == 0)
      {
        ਚੋਣ (context_format) {
        ਹਾਲਤ ContextFormat::SimpleContext:
          pos_glyphs.ਜੋੜੋ (value);
          ਰੋਕੋ;
        ਹਾਲਤ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), ਅੰਕੜੇ, value, &pos_glyphs, cache);
          ਰੋਕੋ;
        ਹਾਲਤ ContextFormat::CoverageBasedContext:
          pos_glyphs.ਸੈੱਟ (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs ());
          ਰੋਕੋ;
        }
      }
      ਨਹੀਂਤਾਂ
      {
        ਅਚਲ ਖਾਲੀ *ਨਿਵੇਸ਼_ਅੰਕੜੇ = ਨਿਵੇਸ਼.ਅੰਕੜੇ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਿਵੇਸ਼_ਮੁੱਲ = seq_ਸੂਚਕਾਂਕ_2 - 1;
        ਜੇ (context_format != ContextFormat::SimpleContext)
        {
          ਨਿਵੇਸ਼_ਅੰਕੜੇ = ਅੰਕੜੇ;
          ਨਿਵੇਸ਼_ਮੁੱਲ = ਨਿਵੇਸ਼[seq_ਸੂਚਕਾਂਕ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, ਨਿਵੇਸ਼_ਅੰਕੜੇ, ਨਿਵੇਸ਼_ਮੁੱਲ, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.ਜੋੜੋ (seq_ਸੂਚਕਾਂਕ_2);
    ਜੇ (has_pos_glyphs)
    {
      hb_codepoint_t ਸਰਗਰਮ_idx = HB_CODEPOINT_INVALID;
      ਜੇ (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_ਸੂਚਕਾਂਕ_2 ? pos_glyphs.get_min () : value;
	ਜੇ (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ਸਰਗਰਮ_FLAG))
	  ਸਰਗਰਮ_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ਸਰਗਰਮ_FLAG;
      }
      hb_ਸੈੱਟ_t *cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs (ਸਰਗਰਮ_idx);
      ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs))
      {
	c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = saved_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
	c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags = saved_ਕਿਨਾਰਾ_flags;
	ਵਾਪਸ;
      }
      *cur_ਸਰਗਰਮ_glyphs = std::move (pos_glyphs);
    }
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!c->ਧੱਕੋ_ਜੜ੍ਹ_ਸਰਗਰਮ_glyphs ()))
    {
      c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = saved_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
      c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags = saved_ਕਿਨਾਰਾ_flags;
      ਵਾਪਸ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ_2 = ਨਿਵੇਸ਼_ਗਿਣਤੀ;
    ਜੇ (context_format == ContextFormat::CoverageBasedContext)
      ਸਿਰਾ_ਸੂਚਕਾਂਕ_2 += 1;

    c->recurse (lookup_ਰਿਕਾਰਡ[i].lookup_List_ਸੂਚਕਾਂਕ, &covered_seq_indices, seq_ਸੂਚਕਾਂਕ_2, ਸਿਰਾ_ਸੂਚਕਾਂਕ_2);
    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = saved_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
    c->depend_ਅੰਕੜੇ->current_ਕਿਨਾਰਾ_flags = saved_ਕਿਨਾਰਾ_flags;
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ recurse_lookups (context_t *c,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
                                    ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[] /* Array of LookupRecords--in design order */)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < lookup_ਗਿਣਤੀ_2; i++)
    c->recurse (lookup_ਰਿਕਾਰਡ[i].lookup_List_ਸੂਚਕਾਂਕ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ apply_lookup (hb_ot_apply_context_t *c,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, /* Including the first glyph */
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
				 ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[], /* Array of LookupRecords--in design order */
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_ਸਿਰਾ)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
  ਪੂਰਨ ਸਿਰਾ;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bl = ਬਫਰ->backtrack_len ();
    ਸਿਰਾ = bl + ਮੇਲ_ਸਿਰਾ - ਬਫਰ->idx;

    ਪੂਰਨ delta = bl - ਬਫਰ->idx;
    /* Convert positions to new indexing. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਗਿਣਤੀ_2; j++)
      c->ਮੇਲ_positions.arrayZ[j] += delta;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < lookup_ਗਿਣਤੀ_2 && ਬਫਰ->successful; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = lookup_ਰਿਕਾਰਡ[i].sequence_ਸੂਚਕਾਂਕ;
    ਜੇ (idx >= ਗਿਣਤੀ_2)
      ਜਾਰੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ orig_len = ਬਫਰ->backtrack_len () + ਬਫਰ->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    ਜੇ (unlikely (c->ਮੇਲ_positions.arrayZ[idx] >= orig_len))
      ਜਾਰੀ;

    ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (c->ਮੇਲ_positions.arrayZ[idx])))
      ਰੋਕੋ;

    ਜੇ (unlikely (ਬਫਰ->max_ops <= 0))
      ਰੋਕੋ;

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      ਜੇ (ਬਫਰ->have_output)
        c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "start recursing to lookup %u at %u",
			  (ਬਿਨਾਨਿਸ਼ਾਨੀ) lookup_ਰਿਕਾਰਡ[i].lookup_List_ਸੂਚਕਾਂਕ,
			  ਬਫਰ->idx);
    }

    ਤਾਰਕਿਕ ret = c->recurse (lookup_ਰਿਕਾਰਡ[i].lookup_List_ਸੂਚਕਾਂਕ);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      ਜੇ (ਬਫਰ->have_output)
        c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "end recursing to lookup %u",
			  (ਬਿਨਾਨਿਸ਼ਾਨੀ) lookup_ਰਿਕਾਰਡ[i].lookup_List_ਸੂਚਕਾਂਕ);
    }

    ਜੇ (!ret)
      ਜਾਰੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_len = ਬਫਰ->backtrack_len () + ਬਫਰ->lookahead_len ();
    ਪੂਰਨ delta = new_len - orig_len;

    ਜੇ (!delta)
      ਜਾਰੀ;

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

    ਸਿਰਾ += delta;
    ਜੇ (ਸਿਰਾ < ਪੂਰਨ (c->ਮੇਲ_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->ਮੇਲ_positions.arrayZ[idx] - ਸਿਰਾ;
      ਸਿਰਾ = c->ਮੇਲ_positions.arrayZ[idx];
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ next = idx + 1; /* next now is the position after the recursed lookup. */

    ਜੇ (delta > 0)
    {
      ਜੇ (unlikely (delta + ਗਿਣਤੀ_2 > HB_MAX_CONTEXT_ਲੰਮਾਈ))
	ਰੋਕੋ;
      ਜੇ (unlikely (ਗਿਣਤੀ_2 + delta > c->ਮੇਲ_positions.ਲੰਮਾਈ &&
		    !c->ਮੇਲ_positions.resize_dirty  (ਗਿਣਤੀ_2 + delta)))
        ਵਾਪਸ;
    }
    ਨਹੀਂਤਾਂ
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (ਪੂਰਨ) next - (ਪੂਰਨ) ਗਿਣਤੀ_2);
      next -= delta;
    }

    /* Shift! */
    memmove (c->ਮੇਲ_positions + next + delta, c->ਮੇਲ_positions + next,
	     (ਗਿਣਤੀ_2 - next) * ਆਕਾਰ (c->ਮੇਲ_positions.arrayZ[0]));
    next += delta;
    ਗਿਣਤੀ_2 += delta;

    /* Fill in new entries. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = idx + 1; j < next; j++)
      c->ਮੇਲ_positions.arrayZ[j] = c->ਮੇਲ_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    ਲਈ (; next < ਗਿਣਤੀ_2; next++)
      c->ਮੇਲ_positions.arrayZ[next] += delta;
  }

  ਪ੍ਰਤਿਜ੍ਞਾ (ਸਿਰਾ >= 0);
  (ਖਾਲੀ) ਬਫਰ->move_ਮੰਜ਼ਿਲ (ਸਿਰਾ);
}



/* Contextual lookups */

ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context
{
  Context_ਘੇਰਾ_Funcs funcs;
  ContextFormat context_format;
  ਅਚਲ ਖਾਲੀ *intersects_ਅੰਕੜੇ;
  ਖਾਲੀ *intersects_cache;
  ਖਾਲੀ *intersected_glyphs_cache;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Context_ਘੇਰਾ_Lookup_Context ContextDependLookupContext;

ਬਣਤਰ ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ਅਚਲ ਖਾਲੀ *collect_ਅੰਕੜੇ;
};

ਬਣਤਰ ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2;
  get_ਮੁੱਲ_func_t get_ਮੁੱਲ;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ context_intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
				       glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
				       Context_ਘੇਰਾ_Lookup_Context &lookup_context)
{
  ਵਾਪਸ array_is_subset_ਦਾ (glyphs,
			     ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0, ਨਿਵੇਸ਼,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ਅੰਕੜੇ,
			     lookup_context.intersects_cache);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ context_ਘੇਰਾ_lookup (hb_ਘੇਰਾ_context_t *c,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
					   glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
					   ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
					   ਬਿਨਾਨਿਸ਼ਾਨੀ value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_ਘੇਰਾ_Lookup_Context &lookup_context)
{
  ਜੇ (context_intersects (c->glyphs,
			  ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
			  lookup_context))
    context_ਘੇਰਾ_recurse_lookups (c,
				     ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
				     lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_ਅੰਕੜੇ,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ context_depend_lookup (hb_depend_context_t *c,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
					  glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
					  ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
					  ਅਚਲ ਖਾਲੀ *ਨਿਯਮ_2,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  ਜੇ (!context_intersects (c->glyphs,
			   ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
			   lookup_context))
    ਵਾਪਸ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ਜੇ (unlikely (!scratch))
    ਵਾਪਸ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  ਤਾਰਕਿਕ context_sets_cached = context_depend_sets_are_cached (
    c, ਨਿਵੇਸ਼_ਗਿਣਤੀ, lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ, ਨਿਯਮ_2, value,
    lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ,
    &scratch->cached_context_sets);
  ਜੇ (unlikely (!c->depend_ਅੰਕੜੇ->successful))
    ਵਾਪਸ;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_ਸੈੱਟ_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_ਸੈੱਟ_t &position_scratch = scratch->position_scratch;
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> &ਨਿਵੇਸ਼_position_glyphs = scratch->ਨਿਵੇਸ਼_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  ਜੇ (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ਅਚਲ hb_ਸੈੱਟ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ, value,
      ਸੱਚ, &position_scratch, &scratch->compact_workspace);
    ਜੇ (unlikely (!pos0_glyphs ||
		  !ਨਿਵੇਸ਼_position_glyphs.ਧੱਕੋ_or_fail (pos0_glyphs)))
    {
      c->depend_ਅੰਕੜੇ->fail ();
      ਵਾਪਸ;
    }

    /* Positions 1+ (Input array) */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਨਿਵੇਸ਼_ਗਿਣਤੀ; i++)
    {
      ਅਚਲ hb_ਸੈੱਟ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ,
	ਨਿਵੇਸ਼[i], ਝੂਠ, &position_scratch);
      ਜੇ (unlikely (!pos_glyphs ||
		    !ਨਿਵੇਸ਼_position_glyphs.ਧੱਕੋ_or_fail (pos_glyphs)))
      {
	c->depend_ਅੰਕੜੇ->fail ();
	ਵਾਪਸ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
				  lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
				  ਨਿਯਮ_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_ਅੰਕੜੇ,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? ਖਾਲੀ_ਸੂਚਕ : &ਨਿਵੇਸ਼_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : ਖਾਲੀ_ਸੂਚਕ,
				  scratch);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
						  glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
						  ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->ਨਿਵੇਸ਼,
		 ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0, ਨਿਵੇਸ਼,
		 lookup_context.funcs.collect, lookup_context.collect_ਅੰਕੜੇ);
  recurse_lookups (c,
		   lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ context_would_apply_lookup (hb_would_apply_context_t *c,
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
					       glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2 HB_ਅਣਵਰਤਿਆ,
					       ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[] HB_ਅਣਵਰਤਿਆ,
					       ਅਚਲ ContextApplyLookupContext &lookup_context)
{
  ਵਾਪਸ would_ਮੇਲ_ਨਿਵੇਸ਼ (c,
			    ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
			    lookup_context.funcs.ਮੇਲ, lookup_context.ਮੇਲ_ਅੰਕੜੇ_2);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ context_apply_lookup (hb_ot_apply_context_t *c,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
					 glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
					 ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
					 ਅਚਲ ContextApplyLookupContext &lookup_context)
{
  ਜੇ (unlikely (ਨਿਵੇਸ਼_ਗਿਣਤੀ > HB_MAX_CONTEXT_ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੇਲ_ਸਿਰਾ = 0;
  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਜੇ (ਮੇਲ_ਨਿਵੇਸ਼ (c,
		   ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
		   lookup_context.funcs.ਮੇਲ, lookup_context.ਮੇਲ_ਅੰਕੜੇ_2,
		   &ਮੇਲ_ਸਿਰਾ))
  {
    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (c->ਬਫਰ->idx, ਮੇਲ_ਸਿਰਾ);
    apply_lookup (c,
		  ਨਿਵੇਸ਼_ਗਿਣਤੀ,
		  lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
		  ਮੇਲ_ਸਿਰਾ);
    ret = ਸੱਚ;
  }
  ਨਹੀਂਤਾਂ
  {
    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, ਮੇਲ_ਸਿਰਾ);
    ret = ਝੂਠ;
  }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  ਚੋਣ (op)
  {
    ਹਾਲਤ hb_ot_subtable_cache_op_t::ENTER:
    {
      ਜੇ (!HB_ਬਫਰ_TRY_ALLOCATE_VAR (c->ਬਫਰ, syllable))
	ਵਾਪਸ ਝੂਠ;
      ਸਵੈਚਾਲਿਤ &info = c->ਬਫਰ->info;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = c->ਬਫਰ->len;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      ਵਾਪਸ ਸੱਚ;
    }
    ਹਾਲਤ hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
      HB_ਬਫਰ_DEALLOCATE_VAR (c->ਬਫਰ, syllable);
      ਰੋਕੋ;
    }
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ਨਿਯਮ_3
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_Types, ਕਿਸਮ_ਨਾਂ ਨਿਯਮ_Type>
  ਮਿੱਤਰ ਬਣਤਰ ਨਿਯਮ_ਸੈੱਟ_ਦਾ;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ context_intersects (glyphs,
			       ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼_Z.arrayZ,
			       lookup_context);
  }

  ਖਾਲੀ depend (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ContextDependLookupContext &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array ((ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0)));
    context_depend_lookup (c,
			   ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼_Z.arrayZ,
			   lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ.arrayZ,
			   ਇਹ,
			   value, lookup_context);
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value, Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array ((ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0)));
    context_ਘੇਰਾ_lookup (c,
			    ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼_Z.arrayZ,
			    lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ.arrayZ,
			    value, lookup_context);
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c,
                        Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;
    ਜੇ (!intersects (c->glyphs, lookup_context)) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array (ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0));
    recurse_lookups (c, lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ.arrayZ);
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array (ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0));
    context_collect_glyphs_lookup (c,
				   ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼_Z.arrayZ,
				   lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ.arrayZ,
				   lookup_context);
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest,
			      ਅਚਲ ClassDef *class_def = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (ਨਿਵੇਸ਼_ਗਿਣਤੀ <= 1)
    {
      *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ ();
      ਵਾਪਸ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ second = ਨਿਵੇਸ਼_Z.arrayZ[0];
    ਜੇ (!class_def)
      digest->ਜੋੜੋ (second);
    ਨਹੀਂਤਾਂ ਜੇ (!second)
      *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ ();
    ਨਹੀਂਤਾਂ
      class_def->collect_class (digest, second);
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c,
		    ਅਚਲ ContextApplyLookupContext &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array (ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0));
    ਵਾਪਸ context_would_apply_lookup (c,
				       ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼_Z.arrayZ,
				       lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ.arrayZ,
				       lookup_context);
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c,
	      ਅਚਲ ContextApplyLookupContext &lookup_context) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array (ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0));
    return_trace (context_apply_lookup (c, ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼_Z.arrayZ, lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ.arrayZ, lookup_context));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ hb_map_t *ਨਿਵੇਸ਼_mapping, /* old->new glyphid or class mapping */
		  ਅਚਲ hb_map_t *lookup_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਨਿਵੇਸ਼_ਗਿਣਤੀ = ਨਿਵੇਸ਼_ਗਿਣਤੀ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਨਿਵੇਸ਼ = ਨਿਵੇਸ਼_Z.as_array (ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1);
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ org : ਨਿਵੇਸ਼)
    {
      ਕਿਸਮ_ਨਾਂ Types::HBUINT d;
      d = ਨਿਵੇਸ਼_mapping->get (org);
      c->copy (d);
    }

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>>
					   (ਨਿਵੇਸ਼_Z.as_array ((ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0)));

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = serialize_lookuprecord_array (c, lookup_ਰਿਕਾਰਡ.as_array (lookup_ਗਿਣਤੀ_2), lookup_map);
    return_trace (c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->lookup_ਗਿਣਤੀ_2, ਗਿਣਤੀ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       ਅਚਲ hb_map_t *lookup_map,
	       ਅਚਲ hb_map_t *klass_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (unlikely (!ਨਿਵੇਸ਼_ਗਿਣਤੀ)) return_trace (ਝੂਠ);
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਨਿਵੇਸ਼ = ਨਿਵੇਸ਼_Z.as_array (ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1);

    ਅਚਲ hb_map_t *mapping = klass_map == ਖਾਲੀ_ਸੂਚਕ ? c->plan->glyph_map : klass_map;
    ਜੇ (!hb_all (ਨਿਵੇਸ਼, mapping)) return_trace (ਝੂਠ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  c->check_range (ਨਿਵੇਸ਼_Z.arrayZ,
				  ਨਿਵੇਸ਼_Z.ਵਸਤੂ_ਮਾਪ * (ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0) +
				  Lookup_ਰਿਕਾਰਡ::static_ਮਾਪ * lookup_ਗਿਣਤੀ_2));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਨਿਵੇਸ਼_ਗਿਣਤੀ;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_ਗਿਣਤੀ_2;		/* Number of LookupRecords */
  Unsized_Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBUINT>
		ਨਿਵੇਸ਼_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, ਨਿਵੇਸ਼_Z);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types, ਕਿਸਮ_ਨਾਂ ਨਿਯਮ_Type>
ਬਣਤਰ ਨਿਯਮ_ਸੈੱਟ_ਦਾ
{
  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
		   Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { ਵਾਪਸ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ਖਾਲੀ collect_ਪਹਿਲਾ_ਨਿਵੇਸ਼_classes (hb_ot_layout_ruleset_digest_t *digest) ਅਚਲ
  {
    digest->init ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਨਿਯਮ_2.len; i++)
    {
      ਅਚਲ ਨਿਯਮ_Type &r = ਇਹ+ਨਿਯਮ_2.arrayZ[i];
      ਜੇ (r.ਨਿਵੇਸ਼_ਗਿਣਤੀ <= 1)
      {
	digest->ਸੈੱਟ_ਭਰਿਆ ();
	ਵਾਪਸ;
      }
      digest->ਜੋੜੋ (r.ਨਿਵੇਸ਼_Z.arrayZ[0]);
    }
  }

  ਖਾਲੀ depend (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ContextDependLookupContext &lookup_context) ਅਚਲ
  {
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value,
		Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;

    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { _.ਘੇਰਾ (c, value, lookup_context); })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c,
                        Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { _.ਘੇਰਾ_lookups (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest,
			      ਅਚਲ ClassDef *class_def = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([=] (ਅਚਲ ਨਿਯਮ_Type &_) { _.collect_ਦੂਜਾ_glyphs (digest, class_def); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c,
		    ਅਚਲ ContextApplyLookupContext &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { ਵਾਪਸ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c,
	      ਅਚਲ ContextApplyLookupContext &lookup_context,
	      ਅਚਲ hb_ot_layout_ruleset_digest_t *digest = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਨਿਯਮਾਂ = ਨਿਯਮ_2.len;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_RULESETS_FAST_ਰਸਤਾ
    ਜੇ (HB_OPTIMIZE_ਮਾਪ_VAL || ਅੰਕ_ਨਿਯਮਾਂ <= 4)
#ਜੇ_ਅੰਤ
    {
    slow:
      return_trace (
      + hb_iter (ਨਿਯਮ_2)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_map ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { ਵਾਪਸ _.apply (c, lookup_context); })
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
    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_context;
    skippy_iter.reset (c->ਬਫਰ->idx);
    skippy_iter.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_always, ਖਾਲੀ_ਸੂਚਕ);
    skippy_iter.ਸੈੱਟ_glyph_ਅੰਕੜੇ ((HBUINT16 *) ਖਾਲੀ_ਸੂਚਕ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *ਪਹਿਲਾ = ਖਾਲੀ_ਸੂਚਕ, *second = ਖਾਲੀ_ਸੂਚਕ;
    ਤਾਰਕਿਕ matched = skippy_iter.next (&unsafe_to1);
    ਜੇ (likely (matched))
    {
      ਜੇ (skippy_iter.may_skip (c->ਬਫਰ->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ਜਾਓ slow;
      }

      ਪਹਿਲਾ = &c->ਬਫਰ->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ਜੇ (digest && lookup_context.get_ਮੁੱਲ &&
	  !digest->may_have (lookup_context.get_ਮੁੱਲ (*ਪਹਿਲਾ, lookup_context.ਮੇਲ_ਅੰਕੜੇ_2)))
	{
	  c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_to1);
	  return_trace (ਝੂਠ);
	}
    }
    ਨਹੀਂਤਾਂ
    {
      ਤਾਰਕਿਕ unsafe_ਮੰਜ਼ਿਲ_concat = ਝੂਠ;
      ਤਾਰਕਿਕ ret =
      + hb_iter (ਨਿਯਮ_2)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_filter ([&] (ਅਚਲ ਨਿਯਮ_Type &_)
		   {
		     ਜੇ (_.ਨਿਵੇਸ਼_ਗਿਣਤੀ <= 1) ਵਾਪਸ ਸੱਚ;
		     unsafe_ਮੰਜ਼ਿਲ_concat = ਸੱਚ;
		     ਵਾਪਸ ਝੂਠ;
		   })
      | hb_map ([&] (ਅਚਲ ਨਿਯਮ_Type &_) { ਵਾਪਸ _.apply (c, lookup_context); })
      | hb_any
      ;
      ਜੇ (unsafe_ਮੰਜ਼ਿਲ_concat)
	c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ਜੇ (likely (matched))
    {
      ਜੇ (skippy_iter.may_skip (c->ਬਫਰ->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ਜਾਓ slow;
      }

      second = &c->ਬਫਰ->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    ਸਵੈਚਾਲਿਤ ਮੇਲ_ਨਿਵੇਸ਼ = lookup_context.funcs.ਮੇਲ;
    ਸਵੈਚਾਲਿਤ *ਨਿਵੇਸ਼_ਅੰਕੜੇ = lookup_context.ਮੇਲ_ਅੰਕੜੇ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ਨਿਯਮਾਂ; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &r = ਇਹ+ਨਿਯਮ_2.arrayZ[i];

      ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = r.ਨਿਵੇਸ਼_Z;

      ਜੇ (r.ਨਿਵੇਸ਼_ਗਿਣਤੀ <= 1 ||
	  (!ਮੇਲ_ਨਿਵੇਸ਼ ||
	   ਮੇਲ_ਨਿਵੇਸ਼ (*ਪਹਿਲਾ, ਨਿਵੇਸ਼.arrayZ[0], ਨਿਵੇਸ਼_ਅੰਕੜੇ)))
      {
        ਜੇ (!second ||
	    (r.ਨਿਵੇਸ਼_ਗਿਣਤੀ <= 2 ||
	     (!ਮੇਲ_ਨਿਵੇਸ਼ ||
	      ਮੇਲ_ਨਿਵੇਸ਼ (*second, ਨਿਵੇਸ਼.arrayZ[1], ਨਿਵੇਸ਼_ਅੰਕੜੇ)))
	   )
	{
	  ਜੇ (r.apply (c, lookup_context))
	  {
	    ਜੇ (unsafe_ਮੰਜ਼ਿਲ != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
	      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);
	    return_trace (ਸੱਚ);
	  }
	}
	ਨਹੀਂਤਾਂ
	  unsafe_ਮੰਜ਼ਿਲ = unsafe_to2;
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (unsafe_ਮੰਜ਼ਿਲ == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
	  unsafe_ਮੰਜ਼ਿਲ = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	ਲਈ (; i + 1 < ਅੰਕ_ਨਿਯਮਾਂ; i++)
	{
	  ਅਚਲ ਸਵੈਚਾਲਿਤ &r2 = ਇਹ+ਨਿਯਮ_2.arrayZ[i + 1];
	  ਅਚਲ ਸਵੈਚਾਲਿਤ &input2 = r2.ਨਿਵੇਸ਼_Z;
	  ਜੇ (r2.ਨਿਵੇਸ਼_ਗਿਣਤੀ <= 1 || input2.arrayZ[0] != ਨਿਵੇਸ਼.arrayZ[0])
	    ਰੋਕੋ;
	}
      }
    }
    ਜੇ (likely (unsafe_ਮੰਜ਼ਿਲ != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1))
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);

    return_trace (ਝੂਠ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       ਅਚਲ hb_map_t *lookup_map,
	       ਅਚਲ hb_map_t *klass_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਨਿਯਮ_2)
    {
      ਜੇ (!_) ਜਾਰੀ;
      ਸਵੈਚਾਲਿਤ o_snap = c->serializer->snapshot ();
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->ਨਿਯਮ_2.serialize_append (c->serializer);
      ਜੇ (unlikely (!o)) ਜਾਰੀ;

      ਜੇ (!o->serialize_subset (c, _, ਇਹ, lookup_map, klass_map))
      {
	ਬਾਹਰਲਾ->ਨਿਯਮ_2.ਕੱਢੋ ();
	c->serializer->revert (o_snap);
      }
    }

    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->ਨਿਯਮ_2);
    ਜੇ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਨਿਯਮ_2.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<ਨਿਯਮ_Type>>
		ਨਿਯਮ_2;			/* Array of Rule tables
					 * ordered by preference */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, ਨਿਯਮ_2);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ਨਿਯਮ_ਸੈੱਟ : ਨਿਯਮ_ਸੈੱਟ_ਦਾ<SmallTypes, ਨਿਯਮ_3<Types>> {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Class_ਨਿਯਮ_ਸੈੱਟ : ਨਿਯਮ_ਸੈੱਟ_ਦਾ<Types, ਨਿਯਮ_3<SmallTypes>> {};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ContextFormat1_4
{
  ਵਰਤੋਂ ਨਿਯਮ_ਸੈੱਟ = OT::ਨਿਯਮ_ਸੈੱਟ<Types>;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      ਖਾਲੀ_ਸੂਚਕ
    };

    ਵਾਪਸ
    + hb_zip (ਇਹ+coverage, ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter (*glyphs, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([&] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &_) { ਵਾਪਸ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (*c->glyphs, *cur_ਸਰਗਰਮ_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      ਖਾਲੀ_ਸੂਚਕ,
      ਖਾਲੀ_ਸੂਚਕ,  /* intersects_cache */
      ਖਾਲੀ_ਸੂਚਕ   /* intersected_glyphs_cache */
    };

    + hb_zip (ਇਹ+coverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਨਿਯਮ_ਸੈੱਟ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ਵਾਪਸ c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs ().has (_);
    }, hb_ਪਹਿਲਾ)
    | hb_map ([&](ਅਚਲ hb_pair_t<hb_codepoint_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> _) { ਵਾਪਸ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਨਿਯਮ_ਸੈੱਟ&> (_.ਪਹਿਲਾ, ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[_.second]); })
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਨਿਯਮ_ਸੈੱਟ&>& _) { _.second.depend (c, _.ਪਹਿਲਾ, lookup_context); })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), *cur_ਸਰਗਰਮ_glyphs);

    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      ਖਾਲੀ_ਸੂਚਕ
    };

    + hb_zip (ਇਹ+coverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਨਿਯਮ_ਸੈੱਟ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ਵਾਪਸ c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs ().has (_);
    }, hb_ਪਹਿਲਾ)
    | hb_map ([&](ਅਚਲ hb_pair_t<hb_codepoint_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> _) { ਵਾਪਸ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਨਿਯਮ_ਸੈੱਟ&> (_.ਪਹਿਲਾ, ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[_.second]); })
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਨਿਯਮ_ਸੈੱਟ&>& _) { _.second.ਘੇਰਾ (c, _.ਪਹਿਲਾ, lookup_context); })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ
  {
    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_glyph, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::SimpleContext,
      ਖਾਲੀ_ਸੂਚਕ
    };

    + hb_zip (ਇਹ+coverage, ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter (*c->glyphs, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &_) { _.ਘੇਰਾ_lookups (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    (ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼);

    ਬਣਤਰ ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      ਖਾਲੀ_ਸੂਚਕ
    };

    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([digest] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_ਦੂਜਾ_glyphs (digest); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[(ਇਹ+coverage).get_coverage (c->glyphs[0])];
    ਬਣਤਰ ContextApplyLookupContext lookup_context = {
      {ਮੇਲ_glyph},
      ਖਾਲੀ_ਸੂਚਕ
    };
    ਵਾਪਸ ਨਿਯਮ_ਸੈੱਟ_3.would_apply (c, lookup_context);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (likely (ਸੂਚਕਾਂਕ_2 == NOT_COVERED))
      return_trace (ਝੂਠ);

    ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[ਸੂਚਕਾਂਕ_2];
    ਬਣਤਰ ContextApplyLookupContext lookup_context = {
      {ਮੇਲ_glyph},
      ਖਾਲੀ_ਸੂਚਕ
    };
    return_trace (ਨਿਯਮ_ਸੈੱਟ_3.apply (c, lookup_context));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    ਅਚਲ hb_map_t *lookup_map = c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+coverage, ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2, ਇਹ, lookup_map), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਤਾਰਕਿਕ (new_coverage));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && ਨਿਯਮ_ਸੈੱਟ_2.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<ਨਿਯਮ_ਸੈੱਟ>>
		ਨਿਯਮ_ਸੈੱਟ_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, ਨਿਯਮ_ਸੈੱਟ_2);
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ContextFormat2_5
{
  ਵਰਤੋਂ ਨਿਯਮ_ਸੈੱਟ = OT::Class_ਨਿਯਮ_ਸੈੱਟ<Types>;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (glyphs))
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;

    hb_map_t cache;
    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_class, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_ਸੈੱਟ_t retained_coverage_glyphs;
    (ਇਹ+coverage).intersect_ਸੈੱਟ (*glyphs, retained_coverage_glyphs);

    hb_ਸੈੱਟ_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_enumerate
    | hb_map ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &> p)
	      { ਵਾਪਸ class_def.intersects_class (glyphs, p.ਪਹਿਲਾ) &&
		       coverage_glyph_classes.has (p.ਪਹਿਲਾ) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (*c->glyphs, *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter ([&] (ਬਿਨਾਨਿਸ਼ਾਨੀ _)
    { ਵਾਪਸ class_def.intersects_class (&c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), _); },
		 hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<ਨਿਯਮ_ਸੈੱਟ>&> _)
                {
                  ਅਚਲ ਨਿਯਮ_ਸੈੱਟ& ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+_.second;
                  ਨਿਯਮ_ਸੈੱਟ_3.depend (c, _.ਪਹਿਲਾ, lookup_context);
                })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs (),
				   *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter ([&] (ਬਿਨਾਨਿਸ਼ਾਨੀ _)
    { ਵਾਪਸ class_def.intersects_class (&c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), _); },
		 hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<ਨਿਯਮ_ਸੈੱਟ>&> _)
                {
                  ਅਚਲ ਨਿਯਮ_ਸੈੱਟ& ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+_.second;
                  ਨਿਯਮ_ਸੈੱਟ_3.ਘੇਰਾ (c, _.ਪਹਿਲਾ, lookup_context);
                })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (c->glyphs))
      ਵਾਪਸ;

    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;

    hb_map_t cache;
    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_class, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_enumerate
    | hb_filter ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &> p)
    { ਵਾਪਸ class_def.intersects_class (c->glyphs, p.ਪਹਿਲਾ); })
    | hb_map (hb_ਦੂਜਾ)
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ & _)
    { _.ਘੇਰਾ_lookups (c, lookup_context); });
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    (ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼);

    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;
    ਬਣਤਰ ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;
    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([=, &class_def] (ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_ਦੂਜਾ_glyphs (digest, &class_def); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = class_def.get_class (c->glyphs[0]);
    ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[ਸੂਚਕਾਂਕ_2];
    ਬਣਤਰ ContextApplyLookupContext lookup_context = {
      {ਮੇਲ_class},
      &class_def
    };
    ਵਾਪਸ ਨਿਯਮ_ਸੈੱਟ_3.would_apply (c, lookup_context);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ cache_ਲਾਗਤ () ਅਚਲ
  {
    ਵਾਪਸ (ਇਹ+classDef).ਲਾਗਤ ();
  }
  ਸਥਿਰ ਤਾਰਕਿਕ cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ਵਾਪਸ context_cache_func (c, op);
  }

  ਬਣਤਰ external_cache_t
  {
    hb_ot_layout_ਦੁਆਧਾਰੀ_cache_t coverage;
    hb_ot_layout_ruleset_digest_t ਨਿਯਮ_sets[HB_VAR_ARRAY];
  };
  ਖਾਲੀ *external_cache_create () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਨਿਯਮ_ਸੈੱਟ_2.len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = ਆਕਾਰ (external_cache_t) -
		    HB_VAR_ARRAY * ਆਕਾਰ (hb_ot_layout_ruleset_digest_t) +
		    ਗਿਣਤੀ_2 * ਆਕਾਰ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (ਮਾਪ);
    ਜੇ (likely (cache))
    {
      cache->coverage.clear ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	(ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[i]).collect_ਪਹਿਲਾ_ਨਿਵੇਸ਼_classes (&cache->ਨਿਯਮ_sets[i]);
    }
    ਵਾਪਸ cache;
  }
  ਤਾਰਕਿਕ apply_cached (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ { ਵਾਪਸ _apply (c, ਸੱਚ, external_cache); }
  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ { ਵਾਪਸ _apply (c, ਝੂਠ, external_cache); }
  ਤਾਰਕਿਕ _apply (hb_ot_apply_context_t *c, ਤਾਰਕਿਕ cached, ਖਾਲੀ *external_cache) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage_ਦੁਆਧਾਰੀ (c->ਬਫਰ->cur().codepoint, cache ? &cache->coverage : ਖਾਲੀ_ਸੂਚਕ);
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
#ਜੇ_ਅੰਤ
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਅਚਲ ClassDef &class_def = ਇਹ+classDef;

    ਬਣਤਰ ContextApplyLookupContext lookup_context = {
      {cached ? ਮੇਲ_class_cached : ਮੇਲ_class},
      &class_def,
      cached ? get_class_cached_ਮੁੱਲ : get_class_ਮੁੱਲ
    };

    ਸੂਚਕਾਂਕ_2 = cached ? get_class_cached (class_def, c->ਬਫਰ->cur()) : class_def.get_class (c->ਬਫਰ->cur().codepoint);
    ਅਚਲ ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[ਸੂਚਕਾਂਕ_2];
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਅਚਲ hb_ot_layout_ruleset_digest_t *digest = cache && ਸੂਚਕਾਂਕ_2 < ਨਿਯਮ_ਸੈੱਟ_2.len ? &cache->ਨਿਯਮ_sets[ਸੂਚਕਾਂਕ_2] : ਖਾਲੀ_ਸੂਚਕ;
#ਨਹੀਂਤਾਂ
    ਅਚਲ hb_ot_layout_ruleset_digest_t *digest = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ
    return_trace (ਨਿਯਮ_ਸੈੱਟ_3.apply (c, lookup_context, digest));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;
    ਜੇ (unlikely (!ਬਾਹਰਲਾ->coverage.serialize_subset (c, coverage, ਇਹ)))
      return_trace (ਝੂਠ);

    hb_map_t klass_map;
    ਬਾਹਰਲਾ->classDef.serialize_subset (c, classDef, ਇਹ, &klass_map);

    ਅਚਲ hb_ਸੈੱਟ_t* glyphset = c->plan->glyphset_gsub ();
    hb_ਸੈੱਟ_t retained_coverage_glyphs;
    (ਇਹ+coverage).intersect_ਸੈੱਟ (*glyphset, retained_coverage_glyphs);

    hb_ਸੈੱਟ_t coverage_glyph_classes;
    (ਇਹ+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ਅਚਲ hb_map_t *lookup_map = c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    ਤਾਰਕਿਕ ret = ਸੱਚ;
    ਪੂਰਨ non_zero_ਸੂਚਕਾਂਕ = -1, ਸੂਚਕਾਂਕ_2 = 0;
    ਸਵੈਚਾਲਿਤ snapshot = c->serializer->snapshot();
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (ਨਿਯਮ_ਸੈੱਟ_2)
			 | hb_filter (klass_map, hb_ਪਹਿਲਾ))
    {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2.serialize_append (c->serializer);
      ਜੇ (unlikely (!o))
      {
	ret = ਝੂਠ;
	ਰੋਕੋ;
      }

      ਜੇ (coverage_glyph_classes.has (_.ਪਹਿਲਾ) &&
	  o->serialize_subset (c, _.second, ਇਹ, lookup_map, &klass_map)) {
	non_zero_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2;
        snapshot = c->serializer->snapshot();
      }

      ਸੂਚਕਾਂਕ_2++;
    }

    ਜੇ (!ret || non_zero_ਸੂਚਕਾਂਕ == -1) return_trace (ਝੂਠ);

    //prune empty trailing ruleSets
    --ਸੂਚਕਾਂਕ_2;
    ਜਦੋਂ (ਸੂਚਕਾਂਕ_2 > non_zero_ਸੂਚਕਾਂਕ)
    {
      ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2.ਕੱਢੋ ();
      ਸੂਚਕਾਂਕ_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && classDef.sanitize (c, ਇਹ) && ਨਿਯਮ_ਸੈੱਟ_2.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<ਨਿਯਮ_ਸੈੱਟ>>
		ਨਿਯਮ_ਸੈੱਟ_2;		/* Array of RuleSet tables
					 * ordered by class */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + 2 * Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, ਨਿਯਮ_ਸੈੱਟ_2);
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ContextFormat3_6
{
  ਵਰਤੋਂ ਨਿਯਮ_ਸੈੱਟ = OT::ਨਿਯਮ_ਸੈੱਟ<SmallTypes>;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverageZ[0]).intersects (glyphs))
      ਵਾਪਸ ਝੂਠ;

    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_coverage, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::CoverageBasedContext,
      ਇਹ
    };
    ਵਾਪਸ context_intersects (glyphs,
			       glyph_ਗਿਣਤੀ_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverageZ[0]).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (*c->glyphs, *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ਇਹ,
      ਖਾਲੀ_ਸੂਚਕ,  /* intersects_cache */
      ਖਾਲੀ_ਸੂਚਕ   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_ਗਿਣਤੀ_2, (ਅਚਲ HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
			   ਇਹ,
			   coverageZ[0], lookup_context);

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverageZ[0]).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs (),
				   *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    ਬਣਤਰ Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      ਇਹ
    };
    context_ਘੇਰਾ_lookup (c,
			    glyph_ਗਿਣਤੀ_2, coverageZ.arrayZ + 1,
			    lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
			    0, lookup_context);

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ
  {
    ਜੇ (!intersects (c->glyphs))
      ਵਾਪਸ;
    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    recurse_lookups (c, lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    (ਇਹ+coverageZ[0]).collect_coverage (c->ਨਿਵੇਸ਼);

    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    ਬਣਤਰ ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      ਇਹ
    };

    context_collect_glyphs_lookup (c,
				   glyph_ਗਿਣਤੀ_2, coverageZ.arrayZ + 1,
				   lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
				   lookup_context);
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    ਜੇ (glyph_ਗਿਣਤੀ_2 <= 1)
      *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ ();
    ਨਹੀਂਤਾਂ
      (ਇਹ+coverageZ[1]).collect_coverage (digest);
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    ਬਣਤਰ ContextApplyLookupContext lookup_context = {
      {ਮੇਲ_coverage},
      ਇਹ
    };
    ਵਾਪਸ context_would_apply_lookup (c,
				       glyph_ਗਿਣਤੀ_2, coverageZ.arrayZ + 1,
				       lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
				       lookup_context);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverageZ[0]; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverageZ[0]).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    ਬਣਤਰ ContextApplyLookupContext lookup_context = {
      {ਮੇਲ_coverage},
      ਇਹ
    };
    return_trace (context_apply_lookup (c,
					glyph_ਗਿਣਤੀ_2,
					coverageZ.arrayZ + 1,
					lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
					lookup_context));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->format = format;
    ਬਾਹਰਲਾ->glyph_ਗਿਣਤੀ_2 = glyph_ਗਿਣਤੀ_2;

    ਸਵੈਚਾਲਿਤ coverages = coverageZ.as_array (glyph_ਗਿਣਤੀ_2);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਵਿੱਥ : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      ਸਵੈਚਾਲਿਤ *o = c->serializer->allocate_ਮਾਪ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>> (Types::ਵਿੱਥ_2::static_ਮਾਪ);
      ਜੇ (unlikely (!o)) return_trace (ਝੂਠ);
      ਜੇ (!o->serialize_subset (c, ਵਿੱਥ, ਇਹ)) return_trace (ਝੂਠ);
    }

    ਅਚਲ ਸਵੈਚਾਲਿਤ& lookup_ਰਿਕਾਰਡ = StructAfter<Unsized_Array_ਦਾ<Lookup_ਰਿਕਾਰਡ>> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    ਅਚਲ hb_map_t *lookup_map = c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = serialize_lookuprecord_array (c->serializer, lookup_ਰਿਕਾਰਡ.as_array (lookup_ਗਿਣਤੀ_2), lookup_map);
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->lookup_ਗਿਣਤੀ_2, ਗਿਣਤੀ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = glyph_ਗਿਣਤੀ_2;
    ਜੇ (unlikely (!ਗਿਣਤੀ_2)) return_trace (ਝੂਠ); /* We want to access coverageZ[0] freely. */
    ਜੇ (unlikely (!c->check_array (coverageZ.arrayZ, ਗਿਣਤੀ_2))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!coverageZ[i].sanitize (c, ਇਹ))) return_trace (ਝੂਠ);
    ਅਚਲ Lookup_ਰਿਕਾਰਡ *lookup_ਰਿਕਾਰਡ = &StructAfter<Lookup_ਰਿਕਾਰਡ> (coverageZ.as_array (glyph_ਗਿਣਤੀ_2));
    return_trace (likely (c->check_array (lookup_ਰਿਕਾਰਡ, lookup_ਗਿਣਤੀ_2)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_ਗਿਣਤੀ_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_ਗਿਣਤੀ_2;		/* Number of LookupRecords */
  Unsized_Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, coverageZ);
};

ਬਣਤਰ Context
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ਹਾਲਤ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ਹਾਲਤ 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#ਜੇ_ਅੰਤ
  } u;
};


/* Chaining Contextual lookups */

ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context
{
  Context_ਘੇਰਾ_Funcs funcs;
  ContextFormat context_format;
  ਅਚਲ ਖਾਲੀ *intersects_ਅੰਕੜੇ[3];
  ਖਾਲੀ *intersects_cache[3];
  ਖਾਲੀ *intersected_glyphs_cache;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Chain_Context_ਘੇਰਾ_Lookup_Context ChainContextDependLookupContext;

ਬਣਤਰ ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ਅਚਲ ਖਾਲੀ *collect_ਅੰਕੜੇ[3];
};

ਬਣਤਰ ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ਅਚਲ ਖਾਲੀ *ਮੇਲ_ਅੰਕੜੇ_2[3];
  get_ਮੁੱਲ_func_t get_ਨਿਵੇਸ਼_ਮੁੱਲ;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ chain_context_intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_ਗਿਣਤੀ,
					     glyph_ਅੰਕੜੇ_t backtrack,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
					     glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_ਗਿਣਤੀ,
					     glyph_ਅੰਕੜੇ_t lookahead,
					     Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context)
{
  ਤਾਰਕਿਕ bt_ok = array_is_subset_ਦਾ (glyphs,
			     backtrack_ਗਿਣਤੀ, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ਅੰਕੜੇ[0],
			     lookup_context.intersects_cache[0]);
  ਤਾਰਕਿਕ ਵਿੱਚ_ok = array_is_subset_ਦਾ (glyphs,
			     ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0, ਨਿਵੇਸ਼,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ਅੰਕੜੇ[1],
			     lookup_context.intersects_cache[1]);
  ਤਾਰਕਿਕ la_ok = array_is_subset_ਦਾ (glyphs,
			     lookahead_ਗਿਣਤੀ, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ਅੰਕੜੇ[2],
			     lookup_context.intersects_cache[2]);

  ਵਾਪਸ bt_ok && ਵਿੱਚ_ok && la_ok;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ chain_context_ਘੇਰਾ_lookup (hb_ਘੇਰਾ_context_t *c,
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_ਗਿਣਤੀ,
						 glyph_ਅੰਕੜੇ_t backtrack,
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
						 glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_ਗਿਣਤੀ,
						 glyph_ਅੰਕੜੇ_t lookahead,
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
						 ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
						 ਬਿਨਾਨਿਸ਼ਾਨੀ value,
						 Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context)
{
  ਜੇ (chain_context_intersects (c->glyphs,
				backtrack_ਗਿਣਤੀ, backtrack,
				ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
				lookahead_ਗਿਣਤੀ, lookahead,
				lookup_context))
    context_ਘੇਰਾ_recurse_lookups (c,
		     ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
		     lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_ਅੰਕੜੇ[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ chain_context_depend_lookup (hb_depend_context_t *c,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_ਗਿਣਤੀ,
						glyph_ਅੰਕੜੇ_t backtrack,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
						glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_ਗਿਣਤੀ,
						glyph_ਅੰਕੜੇ_t lookahead,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
						ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
						ਅਚਲ ਖਾਲੀ *ਨਿਯਮ_2,
						ਬਿਨਾਨਿਸ਼ਾਨੀ value,
						ChainContextDependLookupContext &lookup_context)
{
  ਜੇ (!chain_context_intersects (c->glyphs,
				 backtrack_ਗਿਣਤੀ, backtrack,
				 ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
				 lookahead_ਗਿਣਤੀ, lookahead,
				 lookup_context))
    ਵਾਪਸ;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  ਜੇ (unlikely (!scratch))
    ਵਾਪਸ;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  ਤਾਰਕਿਕ context_sets_cached = context_depend_sets_are_cached (
    c, ਨਿਵੇਸ਼_ਗਿਣਤੀ, lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ, ਨਿਯਮ_2, value,
    lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ[1],
    &scratch->cached_context_sets);
  ਜੇ (unlikely (!c->depend_ਅੰਕੜੇ->successful))
    ਵਾਪਸ;
  ਜੇ (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
				    lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
				    ਨਿਯਮ_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_ਅੰਕੜੇ[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    ਖਾਲੀ_ਸੂਚਕ,
				    &scratch->cached_context_sets,
				    scratch);
    ਵਾਪਸ;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_ਸੈੱਟ_t &position_scratch = scratch->position_scratch;
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < backtrack_ਗਿਣਤੀ; i++)
  {
    ਅਚਲ hb_ਸੈੱਟ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ[0],
      backtrack[i], ਝੂਠ, &position_scratch);
    ਜੇ (unlikely (!pos_glyphs))
      ਵਾਪਸ;
    ਜੇ (!pos_glyphs->is_empty ())
    {
      ਜੇ (unlikely (!backtrack_sets.ਧੱਕੋ_or_fail (pos_glyphs)))
      {
        c->depend_ਅੰਕੜੇ->fail ();
        ਵਾਪਸ;
      }
    }
  }

  /* Extract lookahead glyphs */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < lookahead_ਗਿਣਤੀ; i++)
  {
    ਅਚਲ hb_ਸੈੱਟ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ[2],
      lookahead[i], ਝੂਠ, &position_scratch);
    ਜੇ (unlikely (!pos_glyphs))
      ਵਾਪਸ;
    ਜੇ (!pos_glyphs->is_empty ())
    {
      ਜੇ (unlikely (!lookahead_sets.ਧੱਕੋ_or_fail (pos_glyphs)))
      {
        c->depend_ਅੰਕੜੇ->fail ();
        ਵਾਪਸ;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_ਸੈੱਟ_t &preliminary_context = scratch->preliminary_context;
  ਲਈ (ਅਚਲ hb_ਸੈੱਟ_t *ਪਿੱਠ_ਸੈੱਟ : backtrack_sets)
  {
    ਜੇ (ਪਿੱਠ_ਸੈੱਟ->get_population () == 1) {
      preliminary_context.ਜੋੜੋ (ਪਿੱਠ_ਸੈੱਟ->get_min ());  // Direct glyph
    } ਨਹੀਂਤਾਂ ਜੇ (ਪਿੱਠ_ਸੈੱਟ->get_population () > 1) {
      hb_codepoint_t ਸੈੱਟ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (*ਪਿੱਠ_ਸੈੱਟ)
        : c->ਲੱਭੋ_or_create_cached_context_ਸੈੱਟ (ਪਿੱਠ_ਸੈੱਟ);
      ਜੇ (unlikely (ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
        ਵਾਪਸ;
      preliminary_context.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | ਸੈੱਟ_idx);  // Encoded set reference
    }
  }
  ਲਈ (ਅਚਲ hb_ਸੈੱਟ_t *look_ਸੈੱਟ : lookahead_sets)
  {
    ਜੇ (look_ਸੈੱਟ->get_population () == 1) {
      preliminary_context.ਜੋੜੋ (look_ਸੈੱਟ->get_min ());  // Direct glyph
    } ਨਹੀਂਤਾਂ ਜੇ (look_ਸੈੱਟ->get_population () > 1) {
      hb_codepoint_t ਸੈੱਟ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (*look_ਸੈੱਟ)
        : c->ਲੱਭੋ_or_create_cached_context_ਸੈੱਟ (look_ਸੈੱਟ);
      ਜੇ (unlikely (ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
        ਵਾਪਸ;
      preliminary_context.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | ਸੈੱਟ_idx);  // Encoded set reference
    }
  }
  ਜੇ (unlikely (preliminary_context.ਵਿੱਚ_error ()))
  {
    c->depend_ਅੰਕੜੇ->fail ();
    ਵਾਪਸ;
  }

  /* Build glyph sets for ALL input positions */
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸੈੱਟ_t *> &ਨਿਵੇਸ਼_position_glyphs = scratch->ਨਿਵੇਸ਼_position_glyphs;

  ਜੇ (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ਅਚਲ hb_ਸੈੱਟ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ[1], value,
      ਸੱਚ, &position_scratch, &scratch->compact_workspace);
    ਜੇ (unlikely (!pos0_glyphs ||
		  !ਨਿਵੇਸ਼_position_glyphs.ਧੱਕੋ_or_fail (pos0_glyphs)))
    {
      c->depend_ਅੰਕੜੇ->fail ();
      ਵਾਪਸ;
    }

    /* Positions 1+ (Input array) */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਨਿਵੇਸ਼_ਗਿਣਤੀ; i++)
    {
      ਅਚਲ hb_ਸੈੱਟ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_ਅੰਕੜੇ[1],
	ਨਿਵੇਸ਼[i], ਝੂਠ, &position_scratch);
      ਜੇ (unlikely (!pos_glyphs ||
		    !ਨਿਵੇਸ਼_position_glyphs.ਧੱਕੋ_or_fail (pos_glyphs)))
      {
	c->depend_ਅੰਕੜੇ->fail ();
	ਵਾਪਸ;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
				  lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
				  ਨਿਯਮ_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_ਅੰਕੜੇ[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? ਖਾਲੀ_ਸੂਚਕ : &ਨਿਵੇਸ਼_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : ਖਾਲੀ_ਸੂਚਕ,
				  scratch);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_ਗਿਣਤੀ,
							glyph_ਅੰਕੜੇ_t backtrack,
							ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
							glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
							ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_ਗਿਣਤੀ,
							glyph_ਅੰਕੜੇ_t lookahead,
							ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
							ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_ਗਿਣਤੀ, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_ਅੰਕੜੇ[0]);
  collect_array (c, c->ਨਿਵੇਸ਼,
		 ਨਿਵੇਸ਼_ਗਿਣਤੀ ? ਨਿਵੇਸ਼_ਗਿਣਤੀ - 1 : 0, ਨਿਵੇਸ਼,
		 lookup_context.funcs.collect, lookup_context.collect_ਅੰਕੜੇ[1]);
  collect_array (c, c->after,
		 lookahead_ਗਿਣਤੀ, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_ਅੰਕੜੇ[2]);
  recurse_lookups (c,
		   lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_ਗਿਣਤੀ,
						     glyph_ਅੰਕੜੇ_t backtrack HB_ਅਣਵਰਤਿਆ,
						     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
						     glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
						     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_ਗਿਣਤੀ,
						     glyph_ਅੰਕੜੇ_t lookahead HB_ਅਣਵਰਤਿਆ,
						     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2 HB_ਅਣਵਰਤਿਆ,
						     ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[] HB_ਅਣਵਰਤਿਆ,
						     ਅਚਲ ChainContextApplyLookupContext &lookup_context)
{
  ਵਾਪਸ (c->zero_context ? !backtrack_ਗਿਣਤੀ && !lookahead_ਗਿਣਤੀ : ਸੱਚ)
      && would_ਮੇਲ_ਨਿਵੇਸ਼ (c,
			    ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
			    lookup_context.funcs.ਮੇਲ[1], lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[1]);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backtrack_ਗਿਣਤੀ,
					       glyph_ਅੰਕੜੇ_t backtrack,
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਿਵੇਸ਼_ਗਿਣਤੀ, /* Including the first glyph (not matched) */
					       glyph_ਅੰਕੜੇ_t ਨਿਵੇਸ਼, /* Array of input values--start with second glyph */
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookahead_ਗਿਣਤੀ,
					       glyph_ਅੰਕੜੇ_t lookahead,
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ_2,
					       ਅਚਲ Lookup_ਰਿਕਾਰਡ lookup_ਰਿਕਾਰਡ[],
					       ਅਚਲ ChainContextApplyLookupContext &lookup_context)
{
  ਜੇ (unlikely (ਨਿਵੇਸ਼_ਗਿਣਤੀ > HB_MAX_CONTEXT_ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ = c->ਬਫਰ->ਬਾਹਰਲਾ_len;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ = c->ਬਫਰ->idx;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੇਲ_ਸਿਰਾ = 0;
  ਤਾਰਕਿਕ ret = ਸੱਚ;
  ਜੇ (!(ਮੇਲ_ਨਿਵੇਸ਼ (c,
		     ਨਿਵੇਸ਼_ਗਿਣਤੀ, ਨਿਵੇਸ਼,
		     lookup_context.funcs.ਮੇਲ[1], lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[1],
		     &ਮੇਲ_ਸਿਰਾ) && (ਸਿਰਾ_ਸੂਚਕਾਂਕ = ਮੇਲ_ਸਿਰਾ)
       && ਮੇਲ_lookahead (c,
			   lookahead_ਗਿਣਤੀ, lookahead,
			   lookup_context.funcs.ਮੇਲ[2], lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[2],
			   ਮੇਲ_ਸਿਰਾ, &ਸਿਰਾ_ਸੂਚਕਾਂਕ)))
  {
    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (!ਮੇਲ_backtrack (c,
			backtrack_ਗਿਣਤੀ, backtrack,
			lookup_context.funcs.ਮੇਲ[0], lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[0],
			&ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ))
  {
    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ ਝੂਠ;
  }

  c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break_from_outbuffer (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
  apply_lookup (c,
		ਨਿਵੇਸ਼_ਗਿਣਤੀ,
		lookup_ਗਿਣਤੀ_2, lookup_ਰਿਕਾਰਡ,
		ਮੇਲ_ਸਿਰਾ);

  ਵਾਪਸ ret;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Chain_ਨਿਯਮ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_Types, ਕਿਸਮ_ਨਾਂ Chain_ਨਿਯਮ_Type>
  ਮਿੱਤਰ ਬਣਤਰ Chain_ਨਿਯਮ_ਸੈੱਟ_ਦਾ;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਵਾਪਸ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ਨਿਵੇਸ਼.lenP1, ਨਿਵੇਸ਼.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ਖਾਲੀ depend (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ChainContextDependLookupContext &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 ਨਿਵੇਸ਼.lenP1, ਨਿਵੇਸ਼.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ਇਹ,
				 value,
				 lookup_context);
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value,
		Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_ਘੇਰਾ_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ਨਿਵੇਸ਼.lenP1, ਨਿਵੇਸ਼.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c,
                        Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;
    ਜੇ (!intersects (c->glyphs, lookup_context)) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ਨਿਵੇਸ਼.lenP1, ਨਿਵੇਸ਼.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest,
			      ਅਚਲ ClassDef *ਨਿਵੇਸ਼_class_def = ਖਾਲੀ_ਸੂਚਕ,
			      ਅਚਲ ClassDef *lookahead_class_def = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);

    ਬਿਨਾਨਿਸ਼ਾਨੀ second;
    ਅਚਲ ClassDef *class_def;
    ਜੇ (ਨਿਵੇਸ਼.lenP1 > 1)
    {
      second = ਨਿਵੇਸ਼.arrayZ[0];
      class_def = ਨਿਵੇਸ਼_class_def;
    }
    ਨਹੀਂਤਾਂ ਜੇ (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    ਨਹੀਂਤਾਂ
    {
      *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ ();
      ਵਾਪਸ;
    }

    ਜੇ (!class_def)
      digest->ਜੋੜੋ (second);
    ਨਹੀਂਤਾਂ ਜੇ (!second)
      *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ ();
    ਨਹੀਂਤਾਂ
      class_def->collect_class (digest, second);
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c,
		    ਅਚਲ ChainContextApplyLookupContext &lookup_context) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ਵਾਪਸ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ਨਿਵੇਸ਼.lenP1, ਨਿਵੇਸ਼.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c,
	      ਅਚਲ ChainContextApplyLookupContext &lookup_context) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ਨਿਵੇਸ਼.lenP1, ਨਿਵੇਸ਼.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ਅਚਲ
  {
    c->copy (len);
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ g : it)
    {
      ਕਿਸਮ_ਨਾਂ Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ hb_map_t *lookup_map,
		  ਅਚਲ hb_map_t *backtrack_map,
		  ਅਚਲ hb_map_t *ਨਿਵੇਸ਼_map = ਖਾਲੀ_ਸੂਚਕ,
		  ਅਚਲ hb_map_t *lookahead_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਅਚਲ hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਜੇ (ਨਿਵੇਸ਼_map) mapping = ਨਿਵੇਸ਼_map;
    serialize_array (c, ਨਿਵੇਸ਼.lenP1, + ਨਿਵੇਸ਼.iter ()
				     | hb_map (mapping));

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਜੇ (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_ਗਿਣਤੀ_2 = c->embed (&(lookup.len));
    ਜੇ (!lookup_ਗਿਣਤੀ_2) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_ਸੌਂਪੋ (*lookup_ਗਿਣਤੀ_2, ਗਿਣਤੀ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       ਅਚਲ hb_map_t *lookup_map,
	       ਅਚਲ hb_map_t *backtrack_map = ਖਾਲੀ_ਸੂਚਕ,
	       ਅਚਲ hb_map_t *ਨਿਵੇਸ਼_map = ਖਾਲੀ_ਸੂਚਕ,
	       ਅਚਲ hb_map_t *lookahead_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);

    ਜੇ (!backtrack_map)
    {
      ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
      ਜੇ (!hb_all (backtrack, glyphset) ||
	  !hb_all (ਨਿਵੇਸ਼, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (ਝੂਠ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (ਨਿਵੇਸ਼, ਨਿਵੇਸ਼_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (ਝੂਠ);

      serialize (c->serializer, lookup_map, backtrack_map, ਨਿਵੇਸ਼_map, lookahead_map);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* Hyper-optimized sanitized because this is really hot. */
    ਜੇ (unlikely (!backtrack.len.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਜੇ (unlikely (!ਨਿਵੇਸ਼.lenP1.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਜੇ (unlikely (!lookahead.len.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBUINT>
		ਨਿਵੇਸ਼_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_ਦਾ<Lookup_ਰਿਕਾਰਡ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types, ਕਿਸਮ_ਨਾਂ Chain_ਨਿਯਮ_Type>
ਬਣਤਰ Chain_ਨਿਯਮ_ਸੈੱਟ_ਦਾ
{
  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { ਵਾਪਸ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ਖਾਲੀ collect_ਪਹਿਲਾ_ਨਿਵੇਸ਼_classes (hb_ot_layout_ruleset_digest_t *digest) ਅਚਲ
  {
    digest->init ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਨਿਯਮ_2.len; i++)
    {
      ਅਚਲ Chain_ਨਿਯਮ_Type &r = ਇਹ+ਨਿਯਮ_2.arrayZ[i];
      ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (r.ਨਿਵੇਸ਼_X)> (r.backtrack);
      ਜੇ (ਨਿਵੇਸ਼.lenP1 <= 1)
      {
	digest->ਸੈੱਟ_ਭਰਿਆ ();
	ਵਾਪਸ;
      }
      digest->ਜੋੜੋ (ਨਿਵੇਸ਼.arrayZ[0]);
    }
  }
  ਖਾਲੀ depend (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value, ChainContextDependLookupContext &lookup_context) ਅਚਲ
  {
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ value, Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;

    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { _.ਘੇਰਾ (c, value, lookup_context); })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c,
                        Chain_Context_ਘੇਰਾ_Lookup_Context &lookup_context) ਅਚਲ
  {
    ਜੇ (unlikely (c->lookup_ਹੱਦਬੰਦੀ_exceeded ())) ਵਾਪਸ;

    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { _.ਘੇਰਾ_lookups (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest,
			      ਅਚਲ ClassDef *ਨਿਵੇਸ਼_class_def = ਖਾਲੀ_ਸੂਚਕ,
			      ਅਚਲ ClassDef *lookahead_class_def = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([=] (ਅਚਲ Chain_ਨਿਯਮ_Type &_)
		{ _.collect_ਦੂਜਾ_glyphs (digest, ਨਿਵੇਸ਼_class_def, lookahead_class_def); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c,
		    ਅਚਲ ChainContextApplyLookupContext &lookup_context) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { ਵਾਪਸ _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c,
	      ਅਚਲ ChainContextApplyLookupContext &lookup_context,
	      ਅਚਲ hb_ot_layout_ruleset_digest_t *digest = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਨਿਯਮਾਂ = ਨਿਯਮ_2.len;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_RULESETS_FAST_ਰਸਤਾ
    ਜੇ (HB_OPTIMIZE_ਮਾਪ_VAL || ਅੰਕ_ਨਿਯਮਾਂ <= 4)
#ਜੇ_ਅੰਤ
    {
    slow:
      return_trace (
      + hb_iter (ਨਿਯਮ_2)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_map ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { ਵਾਪਸ _.apply (c, lookup_context); })
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
    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_context;
    skippy_iter.reset (c->ਬਫਰ->idx);
    skippy_iter.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_always, ਖਾਲੀ_ਸੂਚਕ);
    skippy_iter.ਸੈੱਟ_glyph_ਅੰਕੜੇ ((HBUINT16 *) ਖਾਲੀ_ਸੂਚਕ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *ਪਹਿਲਾ = ਖਾਲੀ_ਸੂਚਕ, *second = ਖਾਲੀ_ਸੂਚਕ;
    ਤਾਰਕਿਕ matched = skippy_iter.next (&unsafe_to1);
    ਜੇ (likely (matched))
    {
      ਜੇ (skippy_iter.may_skip (c->ਬਫਰ->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ਜਾਓ slow;
      }

      ਪਹਿਲਾ = &c->ਬਫਰ->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      ਜੇ (digest && lookup_context.get_ਨਿਵੇਸ਼_ਮੁੱਲ &&
	  !digest->may_have (lookup_context.get_ਨਿਵੇਸ਼_ਮੁੱਲ (*ਪਹਿਲਾ, lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[1])))
	{
	  c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_to1);
	  return_trace (ਝੂਠ);
	}
    }
    ਨਹੀਂਤਾਂ
    {
      ਤਾਰਕਿਕ unsafe_ਮੰਜ਼ਿਲ_concat = ਝੂਠ;
      ਤਾਰਕਿਕ ret =
      + hb_iter (ਨਿਯਮ_2)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_filter ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_)
		   {
		     ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (_.ਨਿਵੇਸ਼_X)> (_.backtrack);
		     ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (_.lookaheadX)> (ਨਿਵੇਸ਼);
		     ਜੇ (ਨਿਵੇਸ਼.lenP1 <= 1 && lookahead.len == 0) ਵਾਪਸ ਸੱਚ;
		     unsafe_ਮੰਜ਼ਿਲ_concat = ਸੱਚ;
		     ਵਾਪਸ ਝੂਠ;
		   })
      | hb_map ([&] (ਅਚਲ Chain_ਨਿਯਮ_Type &_) { ਵਾਪਸ _.apply (c, lookup_context); })
      | hb_any
      ;
      ਜੇ (unsafe_ਮੰਜ਼ਿਲ_concat)
	c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    ਜੇ (likely (matched))
    {
      ਜੇ (skippy_iter.may_skip (c->ਬਫਰ->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ਜਾਓ slow;
      }

      second = &c->ਬਫਰ->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    ਸਵੈਚਾਲਿਤ ਮੇਲ_ਨਿਵੇਸ਼ = lookup_context.funcs.ਮੇਲ[1];
    ਸਵੈਚਾਲਿਤ ਮੇਲ_lookahead = lookup_context.funcs.ਮੇਲ[2];
    ਸਵੈਚਾਲਿਤ *ਨਿਵੇਸ਼_ਅੰਕੜੇ = lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[1];
    ਸਵੈਚਾਲਿਤ *lookahead_ਅੰਕੜੇ = lookup_context.ਮੇਲ_ਅੰਕੜੇ_2[2];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ਨਿਯਮਾਂ; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &r = ਇਹ+ਨਿਯਮ_2.arrayZ[i];

      ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (r.ਨਿਵੇਸ਼_X)> (r.backtrack);
      ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (r.lookaheadX)> (ਨਿਵੇਸ਼);

      ਬਿਨਾਨਿਸ਼ਾਨੀ lenP1 = ਨਿਵੇਸ਼.lenP1;
      ਜੇ (lenP1 > 1 ?
	   (!ਮੇਲ_ਨਿਵੇਸ਼ ||
	    ਮੇਲ_ਨਿਵੇਸ਼ (*ਪਹਿਲਾ, ਨਿਵੇਸ਼.arrayZ[0], ਨਿਵੇਸ਼_ਅੰਕੜੇ))
	  :
	   (!lookahead.len || !ਮੇਲ_lookahead ||
	    ਮੇਲ_lookahead (*ਪਹਿਲਾ, lookahead.arrayZ[0], lookahead_ਅੰਕੜੇ)))
      {
	lenP1 = hb_max (lenP1, 1u);
        ਜੇ (!second ||
	    (lenP1 > 2 ?
	     (!ਮੇਲ_ਨਿਵੇਸ਼ ||
	      ਮੇਲ_ਨਿਵੇਸ਼ (*second, ਨਿਵੇਸ਼.arrayZ[1], ਨਿਵੇਸ਼_ਅੰਕੜੇ))
	     :
	     (lookahead.len <= 2 - lenP1 || !ਮੇਲ_lookahead ||
	      ਮੇਲ_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_ਅੰਕੜੇ))))
	{
	  ਜੇ (r.apply (c, lookup_context))
	  {
	    ਜੇ (unsafe_ਮੰਜ਼ਿਲ != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
	      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);
	    return_trace (ਸੱਚ);
	  }
	}
	ਨਹੀਂਤਾਂ
	  unsafe_ਮੰਜ਼ਿਲ = unsafe_to2;
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (unsafe_ਮੰਜ਼ਿਲ == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
	  unsafe_ਮੰਜ਼ਿਲ = unsafe_to1;

	ਜੇ (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  ਲਈ (; i + 1 < ਅੰਕ_ਨਿਯਮਾਂ; i++)
	  {
	    ਅਚਲ ਸਵੈਚਾਲਿਤ &r2 = ਇਹ+ਨਿਯਮ_2.arrayZ[i + 1];
	    ਅਚਲ ਸਵੈਚਾਲਿਤ &input2 = StructAfter<decltype (r2.ਨਿਵੇਸ਼_X)> (r2.backtrack);
	    ਜੇ (input2.lenP1 <= 1 || input2.arrayZ[0] != ਨਿਵੇਸ਼.arrayZ[0])
	      ਰੋਕੋ;
	  }
	}
      }
    }
    ਜੇ (likely (unsafe_ਮੰਜ਼ਿਲ != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1))
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);

    return_trace (ਝੂਠ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       ਅਚਲ hb_map_t *lookup_map,
	       ਅਚਲ hb_map_t *backtrack_klass_map = ਖਾਲੀ_ਸੂਚਕ,
	       ਅਚਲ hb_map_t *ਨਿਵੇਸ਼_klass_map = ਖਾਲੀ_ਸੂਚਕ,
	       ਅਚਲ hb_map_t *lookahead_klass_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਨਿਯਮ_2)
    {
      ਜੇ (!_) ਜਾਰੀ;
      ਸਵੈਚਾਲਿਤ o_snap = c->serializer->snapshot ();
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->ਨਿਯਮ_2.serialize_append (c->serializer);
      ਜੇ (unlikely (!o)) ਜਾਰੀ;

      ਜੇ (!o->serialize_subset (c, _, ਇਹ,
				lookup_map,
				backtrack_klass_map,
				ਨਿਵੇਸ਼_klass_map,
				lookahead_klass_map))
      {
	ਬਾਹਰਲਾ->ਨਿਯਮ_2.ਕੱਢੋ ();
	c->serializer->revert (o_snap);
      }
    }

    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->ਨਿਯਮ_2);
    ਜੇ (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਨਿਯਮ_2.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Chain_ਨਿਯਮ_Type>>
		ਨਿਯਮ_2;			/* Array of ChainRule tables
					 * ordered by preference */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, ਨਿਯਮ_2);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Chain_ਨਿਯਮ_ਸੈੱਟ : Chain_ਨਿਯਮ_ਸੈੱਟ_ਦਾ<Types, Chain_ਨਿਯਮ<Types>> {};

static_assert (ਆਕਾਰ (Chain_ਨਿਯਮ_ਸੈੱਟ<MediumTypes>) ==
	       HBUINT16::static_ਮਾਪ + Offset24::static_ਮਾਪ, "");

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Chain_Class_ਨਿਯਮ_ਸੈੱਟ : Chain_ਨਿਯਮ_ਸੈੱਟ_ਦਾ<Types, Chain_ਨਿਯਮ<SmallTypes>> {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ChainContextFormat1_4
{
  ਵਰਤੋਂ Chain_ਨਿਯਮ_ਸੈੱਟ = OT::Chain_ਨਿਯਮ_ਸੈੱਟ<Types>;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ}
    };

    ਵਾਪਸ
    + hb_zip (ਇਹ+coverage, ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter (*glyphs, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([&] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_) { ਵਾਪਸ _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (*c->glyphs, *cur_ਸਰਗਰਮ_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ},
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ},  /* intersects_cache */
      ਖਾਲੀ_ਸੂਚਕ                        /* intersected_glyphs_cache */
    };

    + hb_zip (ਇਹ+coverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਨਿਯਮ_ਸੈੱਟ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ਵਾਪਸ c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs ().has (_);
    }, hb_ਪਹਿਲਾ)
    | hb_map ([&](ਅਚਲ hb_pair_t<hb_codepoint_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> _) { ਵਾਪਸ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ&> (_.ਪਹਿਲਾ, ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[_.second]); })
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ&>& _) { _.second.depend (c, _.ਪਹਿਲਾ, lookup_context); })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs (),
				   *cur_ਸਰਗਰਮ_glyphs);

    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ}
    };

    + hb_zip (ਇਹ+coverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਨਿਯਮ_ਸੈੱਟ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ਵਾਪਸ c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs ().has (_);
    }, hb_ਪਹਿਲਾ)
    | hb_map ([&](ਅਚਲ hb_pair_t<hb_codepoint_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> _) { ਵਾਪਸ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ&> (_.ਪਹਿਲਾ, ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[_.second]); })
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ&>& _) { _.second.ਘੇਰਾ (c, _.ਪਹਿਲਾ, lookup_context); })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ
  {
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_glyph, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::SimpleContext,
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ}
    };

    + hb_zip (ਇਹ+coverage, ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter (*c->glyphs, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_) { _.ਘੇਰਾ_lookups (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    (ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼);

    ਬਣਤਰ ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ}
    };

    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([digest] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_ਦੂਜਾ_glyphs (digest); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[(ਇਹ+coverage).get_coverage (c->glyphs[0])];
    ਬਣਤਰ ChainContextApplyLookupContext lookup_context = {
      {{ਮੇਲ_glyph, ਮੇਲ_glyph, ਮੇਲ_glyph}},
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ}
    };
    ਵਾਪਸ ਨਿਯਮ_ਸੈੱਟ_3.would_apply (c, lookup_context);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[ਸੂਚਕਾਂਕ_2];
    ਬਣਤਰ ChainContextApplyLookupContext lookup_context = {
      {{ਮੇਲ_glyph, ਮੇਲ_glyph, ਮੇਲ_glyph}},
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ}
    };
    return_trace (ਨਿਯਮ_ਸੈੱਟ_3.apply (c, lookup_context));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    ਅਚਲ hb_map_t *lookup_map = c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+coverage, ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2, ਇਹ, lookup_map), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਤਾਰਕਿਕ (new_coverage));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && ਨਿਯਮ_ਸੈੱਟ_2.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Chain_ਨਿਯਮ_ਸੈੱਟ>>
		ਨਿਯਮ_ਸੈੱਟ_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, ਨਿਯਮ_ਸੈੱਟ_2);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ChainContextFormat2_5
{
  ਵਰਤੋਂ Chain_ਨਿਯਮ_ਸੈੱਟ = OT::Chain_Class_ਨਿਯਮ_ਸੈੱਟ<Types>;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (glyphs))
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_class, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_ਸੈੱਟ_t retained_coverage_glyphs;
    (ਇਹ+coverage).intersect_ਸੈੱਟ (*glyphs, retained_coverage_glyphs);

    hb_ਸੈੱਟ_t coverage_glyph_classes;
    ਨਿਵੇਸ਼_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ਵਾਪਸ
    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_enumerate
    | hb_map ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &> p)
	      { ਵਾਪਸ ਨਿਵੇਸ਼_class_def.intersects_class (glyphs, p.ਪਹਿਲਾ) &&
		       coverage_glyph_classes.has (p.ਪਹਿਲਾ) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (*c->glyphs, *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter ([&] (ਬਿਨਾਨਿਸ਼ਾਨੀ _)
    { ਵਾਪਸ ਨਿਵੇਸ਼_class_def.intersects_class (&c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), _); },
		 hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Chain_ਨਿਯਮ_ਸੈੱਟ>&> _)
                {
                  ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ& chainrule_ਸੈੱਟ = ਇਹ+_.second;
                  chainrule_ਸੈੱਟ.depend (c, _.ਪਹਿਲਾ, lookup_context);
                })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs)) ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs (),
				   *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_filter ([&] (ਬਿਨਾਨਿਸ਼ਾਨੀ _)
    { ਵਾਪਸ ਨਿਵੇਸ਼_class_def.intersects_class (&c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), _); },
		 hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Chain_ਨਿਯਮ_ਸੈੱਟ>&> _)
                {
                  ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ& chainrule_ਸੈੱਟ = ਇਹ+_.second;
                  chainrule_ਸੈੱਟ.ਘੇਰਾ (c, _.ਪਹਿਲਾ, lookup_context);
                })
    ;

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (c->glyphs))
      ਵਾਪਸ;

    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_class, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_enumerate
    | hb_filter([&] (ਬਿਨਾਨਿਸ਼ਾਨੀ klass)
    { ਵਾਪਸ ਨਿਵੇਸ਼_class_def.intersects_class (c->glyphs, klass); }, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_)
    { _.ਘੇਰਾ_lookups (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    (ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼);

    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

    ਬਣਤਰ ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;
    + hb_iter (ਨਿਯਮ_ਸੈੱਟ_2)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([digest, &ਨਿਵੇਸ਼_class_def, &lookahead_class_def] (ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &_)
		{ _.collect_ਦੂਜਾ_glyphs (digest, &ਨਿਵੇਸ਼_class_def, &lookahead_class_def); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = ਨਿਵੇਸ਼_class_def.get_class (c->glyphs[0]);
    ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[ਸੂਚਕਾਂਕ_2];
    ਬਣਤਰ ChainContextApplyLookupContext lookup_context = {
      {{ਮੇਲ_class, ਮੇਲ_class, ਮੇਲ_class}},
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def}
    };
    ਵਾਪਸ ਨਿਯਮ_ਸੈੱਟ_3.would_apply (c, lookup_context);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ cache_ਲਾਗਤ () ਅਚਲ
  {
    ਵਾਪਸ (ਇਹ+ਨਿਵੇਸ਼_Class_Def).ਲਾਗਤ () + (ਇਹ+lookaheadClassDef).ਲਾਗਤ ();
  }
  ਸਥਿਰ ਤਾਰਕਿਕ cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ਵਾਪਸ context_cache_func (c, op);
  }

  ਬਣਤਰ external_cache_t
  {
    hb_ot_layout_ਦੁਆਧਾਰੀ_cache_t coverage;
    hb_ot_layout_mapping_cache_t ਨਿਵੇਸ਼_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t ਨਿਯਮ_sets[HB_VAR_ARRAY];
  };
  ਖਾਲੀ *external_cache_create () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਨਿਯਮ_ਸੈੱਟ_2.len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = ਆਕਾਰ (external_cache_t) -
		    HB_VAR_ARRAY * ਆਕਾਰ (hb_ot_layout_ruleset_digest_t) +
		    ਗਿਣਤੀ_2 * ਆਕਾਰ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (ਮਾਪ);
    ਜੇ (likely (cache))
    {
      cache->coverage.clear ();
      cache->ਨਿਵੇਸ਼_class.clear ();
      cache->lookahead_class.clear ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	(ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[i]).collect_ਪਹਿਲਾ_ਨਿਵੇਸ਼_classes (&cache->ਨਿਯਮ_sets[i]);
    }
    ਵਾਪਸ cache;
  }
  ਤਾਰਕਿਕ apply_cached (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ { ਵਾਪਸ _apply (c, ਸੱਚ, external_cache); }
  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ { ਵਾਪਸ _apply (c, ਝੂਠ, external_cache); }
  ਤਾਰਕਿਕ _apply (hb_ot_apply_context_t *c, ਤਾਰਕਿਕ cached, ਖਾਲੀ *external_cache) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage_ਦੁਆਧਾਰੀ (c->ਬਫਰ->cur().codepoint, cache ? &cache->coverage : ਖਾਲੀ_ਸੂਚਕ);
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
#ਜੇ_ਅੰਤ
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਅਚਲ ClassDef &backtrack_class_def = ਇਹ+backtrackClassDef;
    ਅਚਲ ClassDef &ਨਿਵੇਸ਼_class_def = ਇਹ+ਨਿਵੇਸ਼_Class_Def;
    ਅਚਲ ClassDef &lookahead_class_def = ਇਹ+lookaheadClassDef;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਮੇਲ_class_cache_ਅੰਕੜੇ_t ਨਿਵੇਸ਼_cache_ਅੰਕੜੇ = {
      &ਨਿਵੇਸ਼_class_def,
      cache ? &cache->ਨਿਵੇਸ਼_class : ਖਾਲੀ_ਸੂਚਕ
    };
    ਮੇਲ_class_cache_ਅੰਕੜੇ_t lookahead_cache_ਅੰਕੜੇ = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : ਖਾਲੀ_ਸੂਚਕ
    };
    ਅਚਲ ਖਾਲੀ *ਨਿਵੇਸ਼_ਮੇਲ_ਅੰਕੜੇ = cached
      ? static_cast<ਅਚਲ ਖਾਲੀ *> (&ਨਿਵੇਸ਼_class_def)
      : static_cast<ਅਚਲ ਖਾਲੀ *> (&ਨਿਵੇਸ਼_cache_ਅੰਕੜੇ);
    ਅਚਲ ਖਾਲੀ *lookahead_ਮੇਲ_ਅੰਕੜੇ = cached
      ? static_cast<ਅਚਲ ਖਾਲੀ *> (&lookahead_class_def)
      : static_cast<ਅਚਲ ਖਾਲੀ *> (&lookahead_cache_ਅੰਕੜੇ);
    ਤਾਰਕਿਕ backtrack_ਵਰਤਦਾ_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ਅਚਲ ਖਾਲੀ *backtrack_ਮੇਲ_ਅੰਕੜੇ = backtrack_ਵਰਤਦਾ_lookahead_cache
      ? static_cast<ਅਚਲ ਖਾਲੀ *> (&lookahead_cache_ਅੰਕੜੇ)
      : static_cast<ਅਚਲ ਖਾਲੀ *> (&backtrack_class_def);
#ਜੇ_ਅੰਤ

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    ਬਣਤਰ ChainContextApplyLookupContext lookup_context = {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? ਮੇਲ_class_cached1 :
        backtrack_ਵਰਤਦਾ_lookahead_cache ? ਮੇਲ_class_cache : ਮੇਲ_class,
        cached ? ਮੇਲ_class_cached2 : ਮੇਲ_class_cache,
        cached ? ਮੇਲ_class_cached1 : ਮੇਲ_class_cache}},
      {backtrack_ਮੇਲ_ਅੰਕੜੇ,
       ਨਿਵੇਸ਼_ਮੇਲ_ਅੰਕੜੇ,
       lookahead_ਮੇਲ_ਅੰਕੜੇ},
      cached ? get_class_cached2_ਮੁੱਲ : get_class_cache_ਮੁੱਲ
#ਨਹੀਂਤਾਂ
      {{cached && &backtrack_class_def == &lookahead_class_def ? ਮੇਲ_class_cached1 : ਮੇਲ_class,
        cached ? ਮੇਲ_class_cached2 : ਮੇਲ_class,
        cached ? ਮੇਲ_class_cached1 : ਮੇਲ_class}},
      {&backtrack_class_def,
       &ਨਿਵੇਸ਼_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_ਮੁੱਲ : get_class_ਮੁੱਲ
#ਜੇ_ਅੰਤ
    };

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਸੂਚਕਾਂਕ_2 = cached
         ? get_class_cached2 (ਨਿਵੇਸ਼_class_def, c->ਬਫਰ->cur())
         : ਨਿਵੇਸ਼_class_def.get_class (c->ਬਫਰ->cur().codepoint, ਨਿਵੇਸ਼_cache_ਅੰਕੜੇ.cache);
#ਨਹੀਂਤਾਂ
    ਸੂਚਕਾਂਕ_2 = cached
         ? get_class_cached2 (ਨਿਵੇਸ਼_class_def, c->ਬਫਰ->cur())
          : ਨਿਵੇਸ਼_class_def.get_class (c->ਬਫਰ->cur().codepoint);
#ਜੇ_ਅੰਤ
    ਅਚਲ Chain_ਨਿਯਮ_ਸੈੱਟ &ਨਿਯਮ_ਸੈੱਟ_3 = ਇਹ+ਨਿਯਮ_ਸੈੱਟ_2[ਸੂਚਕਾਂਕ_2];
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਅਚਲ hb_ot_layout_ruleset_digest_t *digest = cache && ਸੂਚਕਾਂਕ_2 < ਨਿਯਮ_ਸੈੱਟ_2.len ? &cache->ਨਿਯਮ_sets[ਸੂਚਕਾਂਕ_2] : ਖਾਲੀ_ਸੂਚਕ;
#ਨਹੀਂਤਾਂ
    ਅਚਲ hb_ot_layout_ruleset_digest_t *digest = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ
    return_trace (ਨਿਯਮ_ਸੈੱਟ_3.apply (c, lookup_context, digest));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;
    ਬਾਹਰਲਾ->coverage.serialize_subset (c, coverage, ਇਹ);

    hb_map_t backtrack_klass_map;
    hb_map_t ਨਿਵੇਸ਼_klass_map;
    hb_map_t lookahead_klass_map;

    ਬਾਹਰਲਾ->backtrackClassDef.serialize_subset (c, backtrackClassDef, ਇਹ, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    ਬਾਹਰਲਾ->ਨਿਵੇਸ਼_Class_Def.serialize_subset (c, ਨਿਵੇਸ਼_Class_Def, ਇਹ, &ਨਿਵੇਸ਼_klass_map);
    ਬਾਹਰਲਾ->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, ਇਹ, &lookahead_klass_map);

    ਜੇ (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   ਨਿਵੇਸ਼_klass_map,
						   lookahead_klass_map)))
      return_trace (ਝੂਠ);

    ਅਚਲ hb_ਸੈੱਟ_t* glyphset = c->plan->glyphset_gsub ();
    hb_ਸੈੱਟ_t retained_coverage_glyphs;
    (ਇਹ+coverage).intersect_ਸੈੱਟ (*glyphset, retained_coverage_glyphs);

    hb_ਸੈੱਟ_t coverage_glyph_classes;
    (ਇਹ+ਨਿਵੇਸ਼_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ਪੂਰਨ non_zero_ਸੂਚਕਾਂਕ = -1, ਸੂਚਕਾਂਕ_2 = 0;
    ਤਾਰਕਿਕ ret = ਸੱਚ;
    ਅਚਲ hb_map_t *lookup_map = c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    ਸਵੈਚਾਲਿਤ ਆਖਰੀ_non_zero = c->serializer->snapshot ();
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (ਨਿਯਮ_ਸੈੱਟ_2)
			 | hb_filter (ਨਿਵੇਸ਼_klass_map, hb_ਪਹਿਲਾ))
    {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2.serialize_append (c->serializer);
      ਜੇ (unlikely (!o))
      {
	ret = ਝੂਠ;
	ਰੋਕੋ;
      }
      ਜੇ (coverage_glyph_classes.has (_.ਪਹਿਲਾ) &&
          o->serialize_subset (c, _.second, ਇਹ,
			       lookup_map,
			       &backtrack_klass_map,
			       &ਨਿਵੇਸ਼_klass_map,
			       &lookahead_klass_map))
      {
        ਆਖਰੀ_non_zero = c->serializer->snapshot ();
	non_zero_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2;
      }

      ਸੂਚਕਾਂਕ_2++;
    }

    ਜੇ (!ret || non_zero_ਸੂਚਕਾਂਕ == -1) return_trace (ਝੂਠ);

    // prune empty trailing ruleSets
    ਜੇ (ਸੂਚਕਾਂਕ_2 > non_zero_ਸੂਚਕਾਂਕ) {
      c->serializer->revert (ਆਖਰੀ_non_zero);
      ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2.len = non_zero_ਸੂਚਕਾਂਕ + 1;
    }

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->ਨਿਯਮ_ਸੈੱਟ_2));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) &&
		  backtrackClassDef.sanitize (c, ਇਹ) &&
		  ਨਿਵੇਸ਼_Class_Def.sanitize (c, ਇਹ) &&
		  lookaheadClassDef.sanitize (c, ਇਹ) &&
		  ਨਿਯਮ_ਸੈੱਟ_2.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ClassDef>
		ਨਿਵੇਸ਼_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Chain_ਨਿਯਮ_ਸੈੱਟ>>
		ਨਿਯਮ_ਸੈੱਟ_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4 + 4 * Types::LOffset::static_ਮਾਪ, ਨਿਯਮ_ਸੈੱਟ_2);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ChainContextFormat3
{
  ਵਰਤੋਂ ਨਿਯਮ_ਸੈੱਟ = OT::ਨਿਯਮ_ਸੈੱਟ<SmallTypes>;

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);

    ਜੇ (!(ਇਹ+ਨਿਵੇਸ਼[0]).intersects (glyphs))
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_coverage, ਖਾਲੀ_ਸੂਚਕ},
      ContextFormat::CoverageBasedContext,
      {ਇਹ, ਇਹ, ਇਹ}
    };
    ਵਾਪਸ chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ਨਿਵੇਸ਼.len, ਨਿਵੇਸ਼.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);

    ਜੇ (!(ਇਹ+ਨਿਵੇਸ਼[0]).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs))
      ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (*c->glyphs, *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ਇਹ, ਇਹ, ਇਹ},
      {ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ},  /* intersects_cache */
      ਖਾਲੀ_ਸੂਚਕ                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ਅਚਲ HBUINT16 *) backtrack.arrayZ,
				 ਨਿਵੇਸ਼.len, (ਅਚਲ HBUINT16 *) ਨਿਵੇਸ਼.arrayZ + 1,
				 lookahead.len, (ਅਚਲ HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 ਇਹ,
				 ਨਿਵੇਸ਼[0], lookup_context);

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);

    ਜੇ (!(ਇਹ+ਨਿਵੇਸ਼[0]).intersects (c->glyphs))
      ਵਾਪਸ;

    hb_ਸੈੱਟ_t* cur_ਸਰਗਰਮ_glyphs = c->ਧੱਕੋ_cur_ਸਰਗਰਮ_glyphs ();
    ਜੇ (unlikely (!cur_ਸਰਗਰਮ_glyphs))
      ਵਾਪਸ;
    get_coverage ().intersect_ਸੈੱਟ (c->ਪਿਛਲਾ_ਮਾਪਾ_ਸਰਗਰਮ_glyphs (),
				   *cur_ਸਰਗਰਮ_glyphs);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ਬਣਤਰ Chain_Context_ਘੇਰਾ_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {ਇਹ, ਇਹ, ਇਹ}
    };
    chain_context_ਘੇਰਾ_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ਨਿਵੇਸ਼.len, ਨਿਵੇਸ਼.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->ਕੱਢੋ_cur_ਪੂਰਾ_glyphs ();
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ
  {
    ਜੇ (!intersects (c->glyphs))
      ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);

    (ਇਹ+ਨਿਵੇਸ਼[0]).collect_coverage (c->ਨਿਵੇਸ਼);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);

    ਬਣਤਰ ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {ਇਹ, ਇਹ, ਇਹ}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ਨਿਵੇਸ਼.len, ਨਿਵੇਸ਼.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਜੇ (ਨਿਵੇਸ਼.len > 1)
      (ਇਹ+ਨਿਵੇਸ਼[1]).collect_coverage (digest);
    ਨਹੀਂਤਾਂ ਜੇ (lookahead.len)
      (ਇਹ+lookahead[0]).collect_coverage (digest);
    ਨਹੀਂਤਾਂ
      *digest = hb_ਸੈੱਟ_digest_t::ਭਰਿਆ ();
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ਬਣਤਰ ChainContextApplyLookupContext lookup_context = {
      {{ਮੇਲ_coverage, ਮੇਲ_coverage, ਮੇਲ_coverage}},
      {ਇਹ, ਇਹ, ਇਹ}
    };
    ਵਾਪਸ chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ਨਿਵੇਸ਼.len, ਨਿਵੇਸ਼.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਵਾਪਸ ਇਹ+ਨਿਵੇਸ਼[0];
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+ਨਿਵੇਸ਼[0]).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ਬਣਤਰ ChainContextApplyLookupContext lookup_context = {
      {{ਮੇਲ_coverage, ਮੇਲ_coverage, ਮੇਲ_coverage}},
      {ਇਹ, ਇਹ, ਇਹ}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ਨਿਵੇਸ਼.len, ਨਿਵੇਸ਼.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ਅਚਲ ਖਾਲੀ* ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ->serialize (c->serializer, 0u))) return_trace (ਝੂਠ);

    ਲਈ (ਸਵੈਚਾਲਿਤ& ਵਿੱਥ : it) {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->serialize_append (c->serializer);
      ਜੇ (unlikely (!o) || !o->serialize_subset (c, ਵਿੱਥ, ਅਧਾਰ))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਜੇ (unlikely (!c->serializer->embed (ਇਹ->format))) return_trace (ਝੂਠ);

    ਜੇ (!serialize_coverage_offsets (c, backtrack.iter (), ਇਹ))
      return_trace (ਝੂਠ);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਜੇ (!serialize_coverage_offsets (c, ਨਿਵੇਸ਼.iter (), ਇਹ))
      return_trace (ਝੂਠ);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਜੇ (!serialize_coverage_offsets (c, lookahead.iter (), ਇਹ))
      return_trace (ਝੂਠ);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ਅਚਲ hb_map_t *lookup_map = c->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_ਗਿਣਤੀ_2 = c->serializer->copy<HBUINT16> (lookup.len);
    ਜੇ (!lookup_ਗਿਣਤੀ_2) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_ਸੌਂਪੋ (*lookup_ਗਿਣਤੀ_2, ਗਿਣਤੀ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!backtrack.sanitize (c, ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਵੇਸ਼ = StructAfter<decltype (ਨਿਵੇਸ਼_X)> (backtrack);
    ਜੇ (unlikely (!ਨਿਵੇਸ਼.sanitize (c, ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਜੇ (unlikely (!ਨਿਵੇਸ਼.len)) return_trace (ਝੂਠ); /* To be consistent with Context. */
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (ਨਿਵੇਸ਼);
    ਜੇ (unlikely (!lookahead.sanitize (c, ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>
		ਨਿਵੇਸ਼_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_ਦਾ<Lookup_ਰਿਕਾਰਡ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (10);
};

ਬਣਤਰ ChainContext
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ਹਾਲਤ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#ਜੇ_ਅੰਤ
  } u;
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ ExtensionFormat1
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type () ਅਚਲ { ਵਾਪਸ extensionLookupType; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ X>
  ਅਚਲ X& get_subtable () ਅਚਲ
  { ਵਾਪਸ ਇਹ + reinterpret_cast<ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ> &> (extension_ਵਿੱਥ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, ਇਹ))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, format);
    return_trace (get_subtable<ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  extensionLookupType != T::Sub_ਸਾਰਣੀ::Extension);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->format = format;
    ਬਾਹਰਲਾ->extensionLookupType = extensionLookupType;

    ਅਚਲ ਸਵੈਚਾਲਿਤ& src_ਵਿੱਥ =
        reinterpret_cast<ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ> &> (extension_ਵਿੱਥ);
    ਸਵੈਚਾਲਿਤ& dest_ਵਿੱਥ =
        reinterpret_cast<Offset32_ਮੰਜ਼ਿਲ<ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ> &> (ਬਾਹਰਲਾ->extension_ਵਿੱਥ);

    return_trace (dest_ਵਿੱਥ.serialize_subset (c, src_ਵਿੱਥ, ਇਹ, get_type ()));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_ਵਿੱਥ;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ Extension
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_type ();
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ X>
  ਅਚਲ X& get_subtable () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.ਸਾਂਚਾ get_subtable<ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ> ();
    ਮੂਲ:ਵਾਪਸ Null (ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.subset (c);
    ਮੂਲ: ਵਾਪਸ c->default_return_ਮੁੱਲ ();
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

ਬਣਤਰ hb_ot_layout_lookup_accelerator_t
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਸਥਿਰ hb_ot_layout_lookup_accelerator_t *create (ਅਚਲ TLookup &lookup)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = lookup.get_subtable_ਗਿਣਤੀ ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = ਆਕਾਰ (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * ਆਕਾਰ (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    ਗਿਣਤੀ_2 * ਆਕਾਰ (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    ਸਵੈਚਾਲਿਤ *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, ਮਾਪ);
    ਜੇ (unlikely (!thiz))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    ਜਦੋਂ (ਗਿਣਤੀ_2 && !thiz->subtables[ਗਿਣਤੀ_2 - 1].apply_func)
      ਗਿਣਤੀ_2--;

    thiz->digest.init ();
    ਲਈ (ਸਵੈਚਾਲਿਤ& subtable : hb_iter (thiz->subtables, ਗਿਣਤੀ_2))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_ਦੂਜਾ = c_accelerate_subtables.digest_ਦੂਜਾ;

    thiz->ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ thiz;
  }

  ਖਾਲੀ fini ()
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      hb_free (subtables[i].external_cache);
#ਜੇ_ਅੰਤ
  }

  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ digest.may_have (g); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਤਾਰਕਿਕ use_cache) ਅਚਲ
  {
    ਜੇ (ਗਿਣਤੀ_2 == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
      ਜੇ (use_cache)
	ਵਾਪਸ subtables[0].apply_cached_no_digest (c);
#ਜੇ_ਅੰਤ
      ਵਾਪਸ subtables[0].apply_no_digest (c);
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਜੇ (use_cache)
    {
      ਵਾਪਸ
      + hb_iter (hb_iter (subtables, ਗਿਣਤੀ_2))
      | hb_map ([&c] (ਅਚਲ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ਵਾਪਸ _.apply_cached (c); })
      | hb_any
      ;
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
      ਵਾਪਸ
      + hb_iter (hb_iter (subtables, ਗਿਣਤੀ_2))
      | hb_map ([&c] (ਅਚਲ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ਵਾਪਸ _.apply (c); })
      | hb_any
      ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ cache_enter (hb_ot_apply_context_t *c) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਵਾਪਸ subtable_cache_user_idx != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  }
  ਖਾਲੀ cache_leave (hb_ot_apply_context_t *c) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#ਜੇ_ਅੰਤ
  }


  hb_ਸੈੱਟ_digest_t digest;
  hb_ਸੈੱਟ_digest_t digest_ਦੂਜਾ;
  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0; /* Number of subtables in the array. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_cache_user_idx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
#ਜੇ_ਅੰਤ
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ਬਣਤਰ GSUBGPOS
{
  ਵਰਤੋਂ LookupListT = LookupList<SmallTypes>;
  ਵਰਤੋਂ LookupList2T = LookupList<MediumTypes>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਵਰਤੋਂ TLookupList = List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<TLookup, ਕਿਸਮ_ਨਾਂ SmallTypes::HBLUINT>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਵਰਤੋਂ TLookupList2 = List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<TLookup, ਕਿਸਮ_ਨਾਂ MediumTypes::HBLUINT>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਵਰਤੋਂ TLookup_ਵਿੱਥ_List = Lookup_ਵਿੱਥ_List<TLookup, ਕਿਸਮ_ਨਾਂ SmallTypes::HBLUINT>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਵਰਤੋਂ TLookup_ਵਿੱਥ_List2 = Lookup_ਵਿੱਥ_List<TLookup, ਕਿਸਮ_ਨਾਂ MediumTypes::HBLUINT>;

  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ.major != 1) ਵਾਪਸ ਸੰਸਕਰਣ.static_ਮਾਪ;

    ਵਾਪਸ min_ਮਾਪ +
	   (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010001u ? featureVars.static_ਮਾਪ : 0) +
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
	   (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u ? 3 * Offset32::static_ਮਾਪ : 0)
#ਨਹੀਂਤਾਂ
	   0
#ਜੇ_ਅੰਤ
	   ;
  }

  ਅਚਲ ScriptList &get_script_list () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && scriptList2) ਵਾਪਸ ਇਹ + scriptList2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਇਹ + scriptList;
  }
  ਅਚਲ FeatureList &get_feature_list () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && featureList2) ਵਾਪਸ ਇਹ + featureList2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਇਹ + featureList;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!ਸੰਸਕਰਣ.sanitize (c) ||
		  ਸੰਸਕਰਣ.major != 1 ||
		  !c->check_range (ਇਹ, get_ਮਾਪ ())))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਤਾਰਕਿਕ script_list_ok = scriptList.sanitize (c, ਇਹ);
    ਤਾਰਕਿਕ feature_list_ok = featureList.sanitize (c, ਇਹ);
    ਤਾਰਕਿਕ lookup_list_ok = typed_lookup_list_ਵਿੱਥ<TLookup> ().sanitize (c, ਇਹ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u)
    {
      ਜੇ (scriptList2) script_list_ok = scriptList2.sanitize (c, ਇਹ);
      ਜੇ (featureList2) feature_list_ok = featureList2.sanitize (c, ਇਹ);
      ਜੇ (lookupList2) lookup_list_ok = typed_lookup_list2_ਵਿੱਥ<TLookup> ().sanitize (c, ਇਹ);
    }
#ਜੇ_ਅੰਤ

    ਜੇ (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (ਝੂਠ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (unlikely (!(ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010001u ||
		    featureVars.sanitize (c, ਇਹ, &get_feature_list ()))))
      return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਤਾਰਕਿਕ subset (hb_subset_layout_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (ਸੰਸਕਰਣ.major != 1) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->subset_context->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->subset_context->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u)
    {
      ਜੇ (unlikely (!c->subset_context->serializer->extend_ਮਾਪ (ਬਾਹਰਲਾ, get_ਮਾਪ ())))
        return_trace (ਝੂਠ);

      ਬਾਹਰਲਾ->scriptList = 0;
      ਬਾਹਰਲਾ->featureList = 0;
      ਬਾਹਰਲਾ->lookupList = 0;

      ਜੇ (lookupList2)
	serialize_subset_ਵਿੱਥ (c->subset_context, ਬਾਹਰਲਾ->lookupList2,
				 subset_lookup_list2_ਵਿੱਥ<TLookup> (),
				 ਇਹ, c);
      ਨਹੀਂਤਾਂ
	serialize_subset_ਵਿੱਥ (c->subset_context, ਬਾਹਰਲਾ->lookupList2,
				 subset_lookup_list_ਵਿੱਥ<TLookup> (),
				 ਇਹ, c);

      ਜੇ (featureList2)
	serialize_subset_ਵਿੱਥ (c->subset_context, ਬਾਹਰਲਾ->featureList2,
				 subset_feature_list2_ਵਿੱਥ (),
				 ਇਹ, c);
      ਨਹੀਂਤਾਂ
	serialize_subset_ਵਿੱਥ (c->subset_context, ਬਾਹਰਲਾ->featureList2,
				 subset_feature_list_ਵਿੱਥ (),
				 ਇਹ, c);

      ਜੇ (scriptList2)
	serialize_subset_ਵਿੱਥ (c->subset_context, ਬਾਹਰਲਾ->scriptList2, scriptList2, ਇਹ, c);
      ਨਹੀਂਤਾਂ
	serialize_subset_ਵਿੱਥ (c->subset_context, ਬਾਹਰਲਾ->scriptList2, scriptList, ਇਹ, c);
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
      ਬਾਹਰਲਾ->subset_lookup_list_ਵਿੱਥ<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_ਵਿੱਥ<TLookup> (),
			     ਇਹ,
			     c);

      ਬਾਹਰਲਾ->subset_feature_list_ਵਿੱਥ ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_ਵਿੱਥ (),
			     ਇਹ,
			     c);

      ਬਾਹਰਲਾ->scriptList.serialize_subset (c->subset_context,
					scriptList,
					ਇਹ,
					c);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010001u)
    {
      ਸਵੈਚਾਲਿਤ snapshot = c->subset_context->serializer->snapshot ();
      ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&ਬਾਹਰਲਾ->featureVars))
        return_trace (ਝੂਠ);

      // if all axes are pinned all feature vars are dropped.
      ਤਾਰਕਿਕ ret = !c->subset_context->plan->all_axes_pinned
                 && ਬਾਹਰਲਾ->featureVars.serialize_subset (c->subset_context, featureVars, ਇਹ, c);
      ਜੇ (!ret)
      {
        ਬਾਹਰਲਾ->featureVars = 0;
	ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = 1;
	  ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 0;
	}
      }
    }
#ਜੇ_ਅੰਤ

    return_trace (ਸੱਚ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lookup_ਗਿਣਤੀ () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && lookupList2)
      ਵਾਪਸ (ਇਹ + lookupList2).len;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ (ਇਹ + lookupList).len;
  }
  ਅਚਲ Lookup& get_lookup (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && lookupList2)
      ਵਾਪਸ (ਇਹ + lookupList2)[i];
#ਜੇ_ਅੰਤ
    ਵਾਪਸ (ਇਹ + lookupList)[i];
  }
  ਅਚਲ FeatureVariations &get_feature_variations () ਅਚਲ
  {
    ਵਾਪਸ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010001u && hb_barrier ()) ? ਇਹ+featureVars : Null (FeatureVariations);
  }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_script_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ get_script_list ().len; }
  ਅਚਲ ਟੈਗ& get_script_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ get_script_list ().get_ਟੈਗ (i); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_script_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
				hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ get_script_list ().get_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ); }
  ਅਚਲ Script& get_script (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ get_script_list ()[i]; }
  ਤਾਰਕਿਕ ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  { ਵਾਪਸ get_script_list ().ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ get_feature_list ().len; }
  hb_ਟੈਗ_t get_feature_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ i == ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ ? HB_ਟੈਗ_NONE : get_feature_list ().get_ਟੈਗ (i); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
				 hb_ਟੈਗ_t     *feature_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ get_feature_list ().get_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, feature_ਗਿਣਤੀ_2, feature_ਟੈਗ_ਸੂਚੀ); }
  ਅਚਲ Feature& get_feature (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ get_feature_list ()[i]; }
  ਤਾਰਕਿਕ ਲੱਭੋ_feature_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  { ਵਾਪਸ get_feature_list ().ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2); }

  ਤਾਰਕਿਕ ਲੱਭੋ_variations_ਸੂਚਕਾਂਕ (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2,
			      ਵਸਤੂ_Var_Store_Instancer *instancer) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    *ਸੂਚਕਾਂਕ_2 = FeatureVariations::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ;
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ get_feature_variations ().ਲੱਭੋ_ਸੂਚਕਾਂਕ (coords, ਅੰਕ_coords, ਸੂਚਕਾਂਕ_2, instancer);
  }
  ਤਾਰਕਿਕ get_feature_variations_state (ਅਚਲ ਪੂਰਨ *coords,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2,
				     ਵਸਤੂ_Var_Store_Instancer *instancer,
				     hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *lookup_conditions) ਅਚਲ
  {
    lookup_conditions->reset ();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    *ਸੂਚਕਾਂਕ_2 = FeatureVariations::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ;
    ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
    ਲੱਭੋ_variations_ਸੂਚਕਾਂਕ (coords, ਅੰਕ_coords, ਸੂਚਕਾਂਕ_2, instancer);
    ਵਾਪਸ get_feature_variations ().collect_lookup_conditions (
	coords,
	ਅੰਕ_coords,
	instancer,
	lookup_conditions);
#ਜੇ_ਅੰਤ
  }
  ਤਾਰਕਿਕ get_lookup_variations (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ variations_ਸੂਚਕਾਂਕ,
                              hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions,
                              hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ get_feature_variations ().get_lookup_variations (
	feature_ਸੂਚਕਾਂਕ, get_feature_variation (feature_ਸੂਚਕਾਂਕ, variations_ਸੂਚਕਾਂਕ),
	conditions, lookup_indexes);
#ਜੇ_ਅੰਤ
  }
  ਅਚਲ Feature& get_feature_variation (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ variations_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (FeatureVariations::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ != variations_ਸੂਚਕਾਂਕ &&
	ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010001u)
    {
      ਅਚਲ Feature *feature = get_feature_variations ().ਲੱਭੋ_substitute (variations_ਸੂਚਕਾਂਕ,
									  feature_ਸੂਚਕਾਂਕ);
      ਜੇ (feature)
	ਵਾਪਸ *feature;
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ get_feature (feature_ਸੂਚਕਾਂਕ);
  }

  ਖਾਲੀ feature_variation_collect_lookups (ਅਚਲ hb_ਸੈੱਟ_t *feature_indexes,
					  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map,
					  hb_ਸੈੱਟ_t       *lookup_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_ਰਿਕਾਰਡ_cond_idx_map, lookup_indexes);
#ਜੇ_ਅੰਤ
  }

  ਖਾਲੀ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#ਜੇ_ਅੰਤ
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਖਾਲੀ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ਅਚਲ
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#ਜੇ_ਅੰਤ

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਖਾਲੀ ਘੇਰਾ_lookups (hb_face_t      *face,
			ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
			hb_ਸੈੱਟ_t       *lookup_indexes /* IN/OUT */) ਅਚਲ
  {
    hb_ਸੈੱਟ_t ਦੇਖਿਆ_lookups, inactive_lookups;
    hb_ਘੇਰਾ_lookups_context_t c (face, glyphs, &ਦੇਖਿਆ_lookups, &inactive_lookups);

    c.ਸੈੱਟ_recurse_func (TLookup::ਸਾਂਚਾ dispatch_recurse_func<hb_ਘੇਰਾ_lookups_context_t>);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ : *lookup_indexes)
      reinterpret_cast<ਅਚਲ TLookup &> (get_lookup (lookup_ਸੂਚਕਾਂਕ)).ਘੇਰਾ_lookups (&c, lookup_ਸੂਚਕਾਂਕ);

    hb_ਸੈੱਟ_union (lookup_indexes, &ਦੇਖਿਆ_lookups);
    hb_ਸੈੱਟ_ਘਟਾਓ (lookup_indexes, &inactive_lookups);
  }

  ਖਾਲੀ prune_langsys (ਅਚਲ hb_map_t *duplicate_feature_map,
                      ਅਚਲ hb_ਸੈੱਟ_t *layout_scripts,
                      hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> *script_langsys_map,
                      hb_ਸੈੱਟ_t       *new_feature_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    hb_prune_langsys_context_t c (ਇਹ, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = get_script_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ script_ਸੂਚਕਾਂਕ = 0; script_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2; script_ਸੂਚਕਾਂਕ++)
    {
      ਅਚਲ ਟੈਗ& ਟੈਗ_2 = get_script_ਟੈਗ (script_ਸੂਚਕਾਂਕ);
      ਜੇ (!layout_scripts->has (ਟੈਗ_2)) ਜਾਰੀ;
      ਅਚਲ Script& s = get_script (script_ਸੂਚਕਾਂਕ);
      s.prune_langsys (&c, script_ਸੂਚਕਾਂਕ);
    }
  }

  ਖਾਲੀ prune_features (ਅਚਲ hb_map_t *lookup_indices, /*ਵਿੱਚ*/
		       ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map, /*ਵਿੱਚ*/
		       ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Feature*> *feature_substitutes_map, /*ਵਿੱਚ*/
		       hb_ਸੈੱਟ_t       *feature_indices /* IN/OUT */) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_ਸੈੱਟ_t alternate_feature_indices;
    get_feature_variations ().ਘੇਰਾ_features (lookup_indices, feature_ਰਿਕਾਰਡ_cond_idx_map, &alternate_feature_indices);
    ਜੇ (unlikely (alternate_feature_indices.ਵਿੱਚ_error()))
    {
      feature_indices->err ();
      ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : hb_iter (feature_indices))
    {
      hb_ਟੈਗ_t ਟੈਗ_2 =  get_feature_ਟੈਗ (i);
      ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        ਜਾਰੀ;


      ਅਚਲ Feature *f = &(get_feature (i));
      ਅਚਲ Feature** p = ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (feature_substitutes_map->has (i, &p))
        f = *p;

      ਜੇ (!f->featureParams.is_null () &&
          ਟੈਗ_2 == HB_ਟੈਗ ('s', 'i', 'z', 'e'))
        ਜਾਰੀ;

      ਜੇ (!f->intersects_lookup_indexes (lookup_indices)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
          && !alternate_feature_indices.has (i)
#ਜੇ_ਅੰਤ
	  )
	feature_indices->del (i);
    }
  }

  ਖਾਲੀ collect_name_ids (ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain, /*ਬਾਹਰਲਾ*/
                         ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Feature*> *feature_substitutes_map,
                         ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map HB_ਅਣਵਰਤਿਆ,
                         ਅਚਲ hb_ਸੈੱਟ_t *catch_all_features,
                         ਤਾਰਕਿਕ all_axes_pinned HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = get_feature_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!feature_ਸੂਚਕਾਂਕ_map->has (i)) ਜਾਰੀ;
      hb_ਟੈਗ_t ਟੈਗ_2 = get_feature_ਟੈਗ (i);
      ਅਚਲ Feature *feature = &get_feature (i);
      ਅਚਲ Feature **substitute;
      ਜੇ (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        ਜੇ (catch_all_features->has (i))
          get_feature (i).collect_name_ids (ਟੈਗ_2, nameids_ਮੰਜ਼ਿਲ_retain);
      }
      feature->collect_name_ids (ਟੈਗ_2, nameids_ਮੰਜ਼ਿਲ_retain);
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_ਸੂਚਕਾਂਕ_map, feature_ਰਿਕਾਰਡ_cond_idx_map,
                                                  [ਇਹ] (ਬਿਨਾਨਿਸ਼ਾਨੀ i) { ਵਾਪਸ get_feature_ਟੈਗ (i); },
                                                  nameids_ਮੰਜ਼ਿਲ_retain);
#ਜੇ_ਅੰਤ
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = ਸੱਚ;
      ਇਹ->ਸਾਰਣੀ = sc.reference_ਸਾਰਣੀ<T> (face);

      ਜੇ (unlikely (ਇਹ->ਸਾਰਣੀ->is_blocklisted (ਇਹ->ਸਾਰਣੀ.get_blob (), face)))
      {
	hb_blob_destroy (ਇਹ->ਸਾਰਣੀ.get_blob ());
	ਇਹ->ਸਾਰਣੀ = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
      }

      ਇਹ->lookup_ਗਿਣਤੀ = ਸਾਰਣੀ->get_lookup_ਗਿਣਤੀ ();

      ਇਹ->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (ਇਹ->lookup_ਗਿਣਤੀ, ਆਕਾਰ (*accels));
      ਜੇ (unlikely (!ਇਹ->accels))
      {
	ਇਹ->lookup_ਗਿਣਤੀ = 0;
	ਇਹ->ਸਾਰਣੀ.destroy ();
	ਇਹ->ਸਾਰਣੀ = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
      }
    }
    ~accelerator_t ()
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਇਹ->lookup_ਗਿਣਤੀ; i++)
      {
	ਸਵੈਚਾਲਿਤ *accel = ਇਹ->accels[i].get_relaxed ();
	ਜੇ (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (ਇਹ->accels);
      ਇਹ->ਸਾਰਣੀ.destroy ();
    }

    hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ) ਅਚਲ
    {
      ਜੇ (unlikely (lookup_ਸੂਚਕਾਂਕ >= lookup_ਗਿਣਤੀ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    retry:
      ਸਵੈਚਾਲਿਤ *accel = accels[lookup_ਸੂਚਕਾਂਕ].get_acquire ();
      ਜੇ (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ));
	ਜੇ (unlikely (!accel))
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

	ਜੇ (unlikely (!accels[lookup_ਸੂਚਕਾਂਕ].cmpexch (ਖਾਲੀ_ਸੂਚਕ, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  ਜਾਓ retry;
	}
      }

      ਵਾਪਸ accel;
    }

    hb_blob_ptr_t<T> ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਗਿਣਤੀ;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  ਸੁਰੱਖਿਅਤ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<TLookupList<TLookup>>& typed_lookup_list_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<TLookupList<TLookup>>&> (lookupList); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<TLookupList2<TLookup>>& typed_lookup_list2_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<TLookupList2<TLookup>>&> (lookupList2); }
#ਜੇ_ਅੰਤ

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<TLookup_ਵਿੱਥ_List<TLookup>>& subset_lookup_list_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<TLookup_ਵਿੱਥ_List<TLookup>>&> (lookupList); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  Offset16_ਮੰਜ਼ਿਲ<TLookup_ਵਿੱਥ_List<TLookup>>& subset_lookup_list_ਵਿੱਥ ()
  { ਵਾਪਸ reinterpret_cast<Offset16_ਮੰਜ਼ਿਲ<TLookup_ਵਿੱਥ_List<TLookup>>&> (lookupList); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup>
  ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<TLookup_ਵਿੱਥ_List2<TLookup>>& subset_lookup_list2_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<TLookup_ਵਿੱਥ_List2<TLookup>>&> (lookupList2); }
#ਜੇ_ਅੰਤ

  ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<ਰਿਕਾਰਡ_List_ਦਾ_Feature>& subset_feature_list_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<ਰਿਕਾਰਡ_List_ਦਾ_Feature>&> (featureList); }
  Offset16_ਮੰਜ਼ਿਲ<ਰਿਕਾਰਡ_List_ਦਾ_Feature>& subset_feature_list_ਵਿੱਥ ()
  { ਵਾਪਸ reinterpret_cast<Offset16_ਮੰਜ਼ਿਲ<ਰਿਕਾਰਡ_List_ਦਾ_Feature>&> (featureList); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<ਰਿਕਾਰਡ_List_ਦਾ_Feature>& subset_feature_list2_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<ਰਿਕਾਰਡ_List_ਦਾ_Feature>&> (featureList2); }
#ਜੇ_ਅੰਤ

  ਅਚਲ ਖਾਲੀ* get_lookup_list_ਖੇਤਰ_ਵਿੱਥ () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && lookupList2) ਵਾਪਸ &lookupList2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ &lookupList;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_ਬਾਹਰਲਾ, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_ਵਿੱਚ, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_3, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_subset_ਵਿੱਥ (hb_subset_context_t *c,
				       ਵਿੱਥ_ਬਾਹਰਲਾ& ਬਾਹਰਲਾ,
				       ਅਚਲ ਵਿੱਥ_ਵਿੱਚ& ਵਿੱਚ,
				       ਅਚਲ ਅਧਾਰ_3 *ਅਧਾਰ,
				       Ts&&... ds)
  {
    ਬਾਹਰਲਾ = 0;
    ਜੇ (ਵਿੱਚ.is_null ()) ਵਾਪਸ ਝੂਠ;

    ਸਵੈਚਾਲਿਤ *s = c->serializer;
    s->push ();
    ਤਾਰਕਿਕ ret = c->dispatch (ਅਧਾਰ+ਵਿੱਚ, std::forward<Ts> (ds)...);
    ਜੇ (ret) s->ਜੋੜੋ_link (ਬਾਹਰਲਾ, s->ਕੱਢੋ_pack ());
    ਨਹੀਂਤਾਂ s->ਕੱਢੋ_discard ();
    ਵਾਪਸ ret;
  }

  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the GSUB/GPOS table--initially set
				 * to 0x00010000u */
  Offset16_ਮੰਜ਼ਿਲ<ScriptList>
		scriptList;	/* Offset to ScriptList table */
  Offset16_ਮੰਜ਼ਿਲ<FeatureList>
		featureList;	/* Offset to FeatureList table */
  Offset16_ਮੰਜ਼ਿਲ<LookupListT>
		lookupList;	/* Offset to LookupList table */
  Offset32_ਮੰਜ਼ਿਲ<FeatureVariations>
		featureVars;	/* Offset to Feature Variations table--from
				 * beginning of table (may be NULL).
				 * Introduced in version 0x00010001. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  Offset32_ਮੰਜ਼ਿਲ<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_ਮੰਜ਼ਿਲ<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_ਮੰਜ਼ਿਲ<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#ਜੇ_ਅੰਤ
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (10);
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_GSUBGPOS_HH */
