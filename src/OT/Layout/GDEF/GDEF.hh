/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2011,2012  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GDEF_GDEF_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GDEF_GDEF_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-var-common.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-cache.hh"


ਨਾਮਾਕਾਸ਼ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
ਬਣਤਰ Attach_ਬਿੰਦੂ : Array16_ਦਾ<HBUINT16>
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    return_trace (ਬਾਹਰਲਾ->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
ਬਣਤਰ AttachList
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_attach_points (hb_codepoint_t glyph_id,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਬਿੰਦੂ_ਗਿਣਤੀ /* IN/OUT */,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਬਿੰਦੂ_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph_id);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED)
    {
      ਜੇ (ਬਿੰਦੂ_ਗਿਣਤੀ)
	*ਬਿੰਦੂ_ਗਿਣਤੀ = 0;
      ਵਾਪਸ 0;
    }

    ਅਚਲ Attach_ਬਿੰਦੂ &points = ਇਹ+attach_ਬਿੰਦੂ[ਸੂਚਕਾਂਕ_2];

    ਜੇ (ਬਿੰਦੂ_ਗਿਣਤੀ && ਬਿੰਦੂ_array)
    {
      + points.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿੰਦੂ_ਗਿਣਤੀ)
      | hb_sink (hb_array (ਬਿੰਦੂ_array, *ਬਿੰਦੂ_ਗਿਣਤੀ))
      ;
    }

    ਵਾਪਸ points.len;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+coverage, attach_ਬਿੰਦੂ)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->attach_ਬਿੰਦੂ, ਇਹ), hb_ਦੂਜਾ)
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
    return_trace (coverage.sanitize (c, ਇਹ) && attach_ਬਿੰਦੂ.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<Attach_ਬਿੰਦੂ>
		attach_ਬਿੰਦੂ;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, attach_ਬਿੰਦੂ);
};

/*
 * Ligature Caret Table
 */

ਬਣਤਰ Caret_ਮੁੱਲ_Format1
{
  ਮਿੱਤਰ ਬਣਤਰ Caret_ਮੁੱਲ;
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  hb_position_t get_caret_ਮੁੱਲ (hb_font_t *font, hb_direction_t direction) ਅਚਲ
  {
    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	caret_ਮੁੱਲ_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ Caret_ਮੁੱਲ_Format2
{
  ਮਿੱਤਰ ਬਣਤਰ Caret_ਮੁੱਲ;
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  hb_position_t get_caret_ਮੁੱਲ (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) ਅਚਲ
  {
    hb_position_t x, y;
    font->get_glyph_contour_ਬਿੰਦੂ_for_origin (glyph_id, caret_ਮੁੱਲ_ਬਿੰਦੂ, direction, &x, &y);
    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	caret_ਮੁੱਲ_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_ਮੁੱਲ_ਬਿੰਦੂ;	/* Contour point index on glyph */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ Caret_ਮੁੱਲ_Format3
{
  ਮਿੱਤਰ ਬਣਤਰ Caret_ਮੁੱਲ;

  hb_position_t get_caret_ਮੁੱਲ (hb_font_t *font, hb_direction_t direction,
				 ਅਚਲ ਵਸਤੂ_Variation_Store &var_store) ਅਚਲ
  {
    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_ਜੋੜੋ (font->em_scale_x (coordinate), (ਇਹ+device_ਸਾਰਣੀ).get_x_delta (font, var_store)) :
	   hb_saturate_ਜੋੜੋ (font->em_scale_y (coordinate), (ਇਹ+device_ਸਾਰਣੀ).get_y_delta (font, var_store));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (!c->serializer->embed (caret_ਮੁੱਲ_Format)) return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->embed (coordinate)) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ varidx = (ਇਹ+device_ਸਾਰਣੀ).get_variation_ਸੂਚਕਾਂਕ ();
    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
    ਜੇ (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_ਪਹਿਲਾ (*new_varidx_delta);
      ਪੂਰਨ delta = hb_ਦੂਜਾ (*new_varidx_delta);
      ਜੇ (delta != 0)
      {
        ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
          return_trace (ਝੂਠ);
      }

      ਜੇ (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
        return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->caret_ਮੁੱਲ_Format, 1, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
    }

    ਜੇ (!c->serializer->embed (device_ਸਾਰਣੀ))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->device_ਸਾਰਣੀ.serialize_copy (c->serializer, device_ਸਾਰਣੀ, ਇਹ, c->serializer->ਮੰਜ਼ਿਲ_bias (ਬਾਹਰਲਾ),
						   hb_serialize_context_t::ਸਿਰ, &c->plan->layout_variation_idx_delta_map));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  { (ਇਹ+device_ਸਾਰਣੀ).collect_variation_indices (c); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && device_ਸਾਰਣੀ.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	caret_ਮੁੱਲ_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16_ਮੰਜ਼ਿਲ<Device>
		device_ਸਾਰਣੀ;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ Caret_ਮੁੱਲ
{
  hb_position_t get_caret_ਮੁੱਲ (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 ਅਚਲ ਵਸਤੂ_Variation_Store &var_store) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_caret_ਮੁੱਲ (font, direction);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_caret_ਮੁੱਲ (font, direction, glyph_id);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_caret_ਮੁੱਲ (font, direction, var_store);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1:
    ਹਾਲਤ 2:
      ਵਾਪਸ;
    ਹਾਲਤ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ਵਾਪਸ;
    ਮੂਲ: ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  Caret_ਮੁੱਲ_Format1	format1;
  Caret_ਮੁੱਲ_Format2	format2;
  Caret_ਮੁੱਲ_Format3	format3;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};

ਬਣਤਰ LigGlyph
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   ਅਚਲ ਵਸਤੂ_Variation_Store &var_store,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ              ਸ਼ੁਰੂ_ਵਿੱਥ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ             *caret_ਗਿਣਤੀ /* IN/OUT */,
			   hb_position_t        *caret_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (caret_ਗਿਣਤੀ && caret_array)
    {
      + carets.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, caret_ਗਿਣਤੀ)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_map ([&] (ਅਚਲ Caret_ਮੁੱਲ &value) { ਵਾਪਸ value.get_caret_ਮੁੱਲ (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_ਗਿਣਤੀ))
      ;
    }

    ਵਾਪਸ carets.len;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    + hb_iter (carets)
    | hb_apply (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->carets, ਇਹ))
    ;

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->carets));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Caret_ਮੁੱਲ>& ਵਿੱਥ : carets.iter ())
      (ਇਹ+ਵਿੱਥ).collect_variation_indices (c);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (carets.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<Caret_ਮੁੱਲ>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ LigCaretList
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ਅਚਲ ਵਸਤੂ_Variation_Store &var_store,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *caret_ਗਿਣਤੀ /* IN/OUT */,
			       hb_position_t *caret_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph_id);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED)
    {
      ਜੇ (caret_ਗਿਣਤੀ)
	*caret_ਗਿਣਤੀ = 0;
      ਵਾਪਸ 0;
    }
    ਅਚਲ LigGlyph &lig_glyph = ਇਹ+ligGlyph[ਸੂਚਕਾਂਕ_2];
    ਵਾਪਸ lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, ਸ਼ੁਰੂ_ਵਿੱਥ, caret_ਗਿਣਤੀ, caret_array);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+coverage, ligGlyph)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->ligGlyph, ਇਹ), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਤਾਰਕਿਕ (new_coverage));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, ligGlyph)
    | hb_filter (c->glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && ligGlyph.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, ligGlyph);
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_ਮਾਪ ==
	       Offset32::static_ਮਾਪ + HBUINT24::static_ਮਾਪ, "");
#ਜੇ_ਅੰਤ


ਬਣਤਰ MarkGlyphSetsFormat1
{
  ਤਾਰਕਿਕ covers (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਸੂਚਕਾਂਕ, hb_codepoint_t glyph_id) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage[ਸੈੱਟ_ਸੂਚਕਾਂਕ]).get_coverage (glyph_id) != NOT_COVERED; }

  ਖਾਲੀ collect_ਵਰਤਿਆ_mark_sets (ਅਚਲ hb_ਸੈੱਟ_t& glyph_ਸੈੱਟ,
                               hb_ਸੈੱਟ_t& ਵਰਤਿਆ_mark_sets /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : coverage)
     {
       ਅਚਲ ਸਵੈਚਾਲਿਤ &cov = ਇਹ+ਵਿੱਥ;
       ਜੇ (cov.intersects (&glyph_ਸੈੱਟ))
         ਵਰਤਿਆ_mark_sets.ਜੋੜੋ (i);

       i++;
     }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_coverage (hb_ਸਦਿਸ਼_t<ਸੈੱਟ_t> &sets) ਅਚਲ
  {
     ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : coverage)
     {
       ਅਚਲ ਸਵੈਚਾਲਿਤ &cov = ਇਹ+ਵਿੱਥ;
       cov.collect_coverage (sets.push ());
     }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    ਤਾਰਕਿਕ ret = ਸੱਚ;
    ਲਈ (ਅਚਲ Offset32_ਮੰਜ਼ਿਲ<Coverage>& ਵਿੱਥ : coverage.iter ())
    {
      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->coverage.serialize_append (c->serializer);
      ਜੇ (unlikely (!o))
      {
	ret = ਝੂਠ;
	ਰੋਕੋ;
      }

      //skip empty coverage
      c->serializer->push ();
      ਤਾਰਕਿਕ res = ਝੂਠ;
      ਜੇ (ਵਿੱਥ) res = c->dispatch (ਇਹ+ਵਿੱਥ);
      ਜੇ (!res)
      {
        c->serializer->ਕੱਢੋ_discard ();
        c->serializer->revert (snap);
        (ਬਾਹਰਲਾ->coverage.len)--;
        ਜਾਰੀ;
      }
      c->serializer->ਜੋੜੋ_link (*o, c->serializer->ਕੱਢੋ_pack ());
    }

    return_trace (ret && ਬਾਹਰਲਾ->coverage.len);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_ਦਾ<Offset32_ਮੰਜ਼ਿਲ<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, coverage);
};

ਬਣਤਰ MarkGlyphSets
{
  ਤਾਰਕਿਕ covers (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਸੂਚਕਾਂਕ, hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.covers (ਸੈੱਟ_ਸੂਚਕਾਂਕ, glyph_id);
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_coverage (hb_ਸਦਿਸ਼_t<ਸੈੱਟ_t> &sets) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); u.format1.collect_coverage (sets); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਖਾਲੀ collect_ਵਰਤਿਆ_mark_sets (ਅਚਲ hb_ਸੈੱਟ_t& glyph_ਸੈੱਟ,
                               hb_ਸੈੱਟ_t& ਵਰਤਿਆ_mark_sets /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); u.format1.collect_ਵਰਤਿਆ_mark_sets (glyph_ਸੈੱਟ, ਵਰਤਿਆ_mark_sets); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.subset (c));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


ਬਣਤਰ GDEF
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_GDEF;

  ਗਿਣਤੀ GlyphClasses {
    UnclassifiedGlyph	= 0,
    ਅਧਾਰ_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ.major != 1) ਵਾਪਸ ਸੰਸਕਰਣ.static_ਮਾਪ;

    ਵਾਪਸ min_ਮਾਪ +
	   (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u ? markGlyphSetsDef.static_ਮਾਪ : 0) +
	   (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010003u ? varStore.static_ਮਾਪ : 0) +
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
	   (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u ? 5 * Offset32::static_ਮਾਪ : 0)
#ਨਹੀਂਤਾਂ
	   0
#ਜੇ_ਅੰਤ
	   ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!ਸੰਸਕਰਣ.sanitize (c) ||
		  ਸੰਸਕਰਣ.major != 1 ||
		  !c->check_range (ਇਹ, get_ਮਾਪ ())))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਤਾਰਕਿਕ glyph_class_def_ok = glyphClassDef.sanitize (c, ਇਹ);
    ਤਾਰਕਿਕ attach_list_ok = attachList.sanitize (c, ਇਹ);
    ਤਾਰਕਿਕ lig_caret_list_ok = ligCaretList.sanitize (c, ਇਹ);
    ਤਾਰਕਿਕ mark_attach_class_def_ok = markAttachClassDef.sanitize (c, ਇਹ);
    ਤਾਰਕਿਕ mark_glyph_sets_def_ok = ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, ਇਹ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u)
    {
      ਜੇ (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, ਇਹ);
      ਜੇ (attachList2) attach_list_ok = attachList2.sanitize (c, ਇਹ);
      ਜੇ (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, ਇਹ);
      ਜੇ (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, ਇਹ);
      ਜੇ (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, ਇਹ);
    }
#ਜੇ_ਅੰਤ

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010003u || varStore.sanitize (c, ਇਹ)));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (ਸੰਸਕਰਣ.major != 1) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    ਸਵੈਚਾਲਿਤ snapshot_version0 = c->serializer->snapshot ();
    ਜੇ (unlikely (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ snapshot_version2 = c->serializer->snapshot ();
    ਜੇ (unlikely (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ use_v14 = ਝੂਠ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    use_v14 = ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    ਜੇ (unlikely (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_ਮਾਪ (ਬਾਹਰਲਾ, get_ਮਾਪ ())))
      return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    ਤਾਰਕਿਕ subset_varstore = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ varstore_ਸੂਚਕਾਂਕ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010003u && hb_barrier ())
    {
      ਜੇ (c->plan->all_axes_pinned)
	ਬਾਹਰਲਾ->varStore = 0;
      ਨਹੀਂਤਾਂ ਜੇ (c->plan->normalized_coords)
      {
	ਜੇ (varStore)
	{
	  ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
	  ਜੇ (unlikely (!c->serializer->check_success (
                  ਵਸਤੂ_vars.instantiate (ਇਹ+varStore, c->plan, ਸੱਚ, ਸੱਚ,
                                          c->plan->gdef_varstore_inner_maps.as_array (), ਸੱਚ))))
            return_trace (ਝੂਠ);
	  subset_varstore = ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer,
							     ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ (),
							     c->plan->axis_ਟੈਗ_ਸੂਚੀ,
							     ਵਸਤੂ_vars.get_region_list (),
							     ਵਸਤੂ_vars.get_vardata_encodings ());
	  varstore_ਸੂਚਕਾਂਕ = c->serializer->ਆਖਰੀ_added_ਬੱਚਾ_ਸੂਚਕਾਂਕ();
	  remap_varidx_after_instantiation (ਵਸਤੂ_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (ਵਸਤੂ_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_ਮੁੱਲ_map);
	}
      }
      ਨਹੀਂਤਾਂ
      {
	subset_varstore = ਬਾਹਰਲਾ->varStore.serialize_subset (c, varStore, ਇਹ,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_ਸੂਚਕਾਂਕ = c->serializer->ਆਖਰੀ_added_ਬੱਚਾ_ਸੂਚਕਾਂਕ();
      }
    }

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = ਸੰਸਕਰਣ.major;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = ਸੰਸਕਰਣ.minor;

    ਜੇ (!subset_varstore && ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    ਜੇ (!subset_varstore)
      ਬਾਹਰਲਾ->varStore = 0;
    ਨਹੀਂਤਾਂ
      c->plan->has_gdef_varstore = ਸੱਚ;

    ਤਾਰਕਿਕ subset_markglyphsetsdef = ਝੂਠ;
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && hb_barrier ())
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (use_v14 && markGlyphSetsDef2)
      {
	ਬਾਹਰਲਾ->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_ਵਿੱਥ (c, ਬਾਹਰਲਾ->markGlyphSetsDef2,
							   markGlyphSetsDef2, ਇਹ);
      }
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
	ਜੇ (use_v14) ਬਾਹਰਲਾ->markGlyphSetsDef2 = 0;
#ਜੇ_ਅੰਤ
	subset_markglyphsetsdef = ਬਾਹਰਲਾ->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, ਇਹ);
      }
    }

    ਜੇ (use_v14)
    {
      ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 4;
    } ਨਹੀਂਤਾਂ ਜੇ (subset_varstore) {
      ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 3;
    } ਨਹੀਂਤਾਂ ਜੇ (subset_markglyphsetsdef) {
      ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 2;
    } ਨਹੀਂਤਾਂ  {
      ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    ਤਾਰਕਿਕ subset_glyphclassdef = ਝੂਠ;
    ਤਾਰਕਿਕ subset_attachlist = ਝੂਠ;
    ਤਾਰਕਿਕ subset_markattachclassdef = ਝੂਠ;
    ਤਾਰਕਿਕ subset_ligcaretlist = ਝੂਠ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (use_v14 && glyphClassDef2)
    {
      ਬਾਹਰਲਾ->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_ਵਿੱਥ (c, ਬਾਹਰਲਾ->glyphClassDef2,
						      glyphClassDef2, ਇਹ, ਖਾਲੀ_ਸੂਚਕ, ਝੂਠ, ਸੱਚ);
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (use_v14) ਬਾਹਰਲਾ->glyphClassDef2 = 0;
#ਜੇ_ਅੰਤ
      subset_glyphclassdef = ਬਾਹਰਲਾ->glyphClassDef.serialize_subset (c, glyphClassDef, ਇਹ, ਖਾਲੀ_ਸੂਚਕ, ਝੂਠ, ਸੱਚ);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (use_v14 && attachList2)
    {
      ਬਾਹਰਲਾ->attachList = 0;
      subset_attachlist = serialize_subset_ਵਿੱਥ (c, ਬਾਹਰਲਾ->attachList2, attachList2, ਇਹ);
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (use_v14) ਬਾਹਰਲਾ->attachList2 = 0;
#ਜੇ_ਅੰਤ
      subset_attachlist = ਬਾਹਰਲਾ->attachList.serialize_subset (c, attachList, ਇਹ);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (use_v14 && markAttachClassDef2)
    {
      ਬਾਹਰਲਾ->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_ਵਿੱਥ (c, ਬਾਹਰਲਾ->markAttachClassDef2,
							   markAttachClassDef2, ਇਹ, ਖਾਲੀ_ਸੂਚਕ, ਝੂਠ, ਸੱਚ);
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (use_v14) ਬਾਹਰਲਾ->markAttachClassDef2 = 0;
#ਜੇ_ਅੰਤ
      subset_markattachclassdef = ਬਾਹਰਲਾ->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   ਇਹ, ਖਾਲੀ_ਸੂਚਕ, ਝੂਠ, ਸੱਚ);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (use_v14 && ligCaretList2)
    {
      ਬਾਹਰਲਾ->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_ਵਿੱਥ (c, ਬਾਹਰਲਾ->ligCaretList2, ligCaretList2, ਇਹ);
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (use_v14) ਬਾਹਰਲਾ->ligCaretList2 = 0;
#ਜੇ_ਅੰਤ
      subset_ligcaretlist = ਬਾਹਰਲਾ->ligCaretList.serialize_subset (c, ligCaretList, ਇਹ);
    }

    ਜੇ (subset_varstore && varstore_ਸੂਚਕਾਂਕ != (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1) {
      c->serializer->repack_ਆਖਰੀ(varstore_ਸੂਚਕਾਂਕ);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010003u && subset_varstore));
  }

  ਤਾਰਕਿਕ has_glyph_classes () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && glyphClassDef2) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ glyphClassDef != 0;
  }
  ਅਚਲ ClassDef &get_glyph_class_def () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && glyphClassDef2) ਵਾਪਸ ਇਹ+glyphClassDef2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਇਹ+glyphClassDef;
  }
  ਤਾਰਕਿਕ has_attach_list () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && attachList2) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ attachList != 0;
  }
  ਅਚਲ AttachList &get_attach_list () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && attachList2) ਵਾਪਸ ਇਹ+attachList2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਇਹ+attachList;
  }
  ਤਾਰਕਿਕ has_lig_carets () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && ligCaretList2) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ligCaretList != 0;
  }
  ਤਾਰਕਿਕ has_mark_attachment_types () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && markAttachClassDef2) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ markAttachClassDef != 0;
  }
  ਅਚਲ ClassDef &get_mark_attach_class_def () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && markAttachClassDef2) ਵਾਪਸ ਇਹ+markAttachClassDef2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਇਹ+markAttachClassDef;
  }
  ਤਾਰਕਿਕ has_mark_glyph_sets () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010002u) ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && markGlyphSetsDef2) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ markGlyphSetsDef != 0;
  }
  ਅਚਲ MarkGlyphSets &get_mark_glyph_sets () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010002u) ਵਾਪਸ Null(MarkGlyphSets);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && markGlyphSetsDef2) ਵਾਪਸ ਇਹ+markGlyphSetsDef2;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਇਹ+markGlyphSetsDef;
  }
  ਤਾਰਕਿਕ has_var_store () ਅਚਲ
  {
    ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  ਅਚਲ ਵਸਤੂ_Variation_Store &get_var_store () ਅਚਲ
  {
    ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010003u && hb_barrier () ? ਇਹ+varStore : Null(ਵਸਤੂ_Variation_Store);
  }


  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_class (hb_codepoint_t glyph) ਅਚਲ
  { ਵਾਪਸ get_glyph_class_def ().get_class (glyph); }
  ਖਾਲੀ get_glyphs_ਵਿੱਚ_class (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass, hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_mark_attachment_type (hb_codepoint_t glyph) ਅਚਲ
  { ਵਾਪਸ get_mark_attach_class_def ().get_class (glyph); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_attach_points (hb_codepoint_t glyph_id,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਬਿੰਦੂ_ਗਿਣਤੀ /* IN/OUT */,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਬਿੰਦੂ_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ get_attach_list ().get_attach_points (glyph_id, ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿੰਦੂ_ਗਿਣਤੀ, ਬਿੰਦੂ_array); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *caret_ਗਿਣਤੀ /* IN/OUT */,
			       hb_position_t *caret_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && ligCaretList2)
      ਵਾਪਸ (ਇਹ+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  ਸ਼ੁਰੂ_ਵਿੱਥ, caret_ਗਿਣਤੀ, caret_array);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ (ਇਹ+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       ਸ਼ੁਰੂ_ਵਿੱਥ, caret_ਗਿਣਤੀ, caret_array);
  }

  ਤਾਰਕਿਕ mark_ਸੈੱਟ_covers (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਸੂਚਕਾਂਕ, hb_codepoint_t glyph_id) ਅਚਲ
  { ਵਾਪਸ get_mark_glyph_sets ().covers (ਸੈੱਟ_ਸੂਚਕਾਂਕ, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_props (hb_codepoint_t glyph) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass = get_glyph_class (glyph);

    static_assert (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) HB_OT_LAYOUT_GLYPH_PROPS_ਅਧਾਰ_GLYPH == (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) LookupFlag::Ignore_ਅਧਾਰ_Glyphs), "");
    static_assert (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) LookupFlag::IgnoreLigatures), "");
    static_assert (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) LookupFlag::IgnoreMarks), "");

    ਚੋਣ (klass) {
    ਮੂਲ:			ਵਾਪਸ HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    ਹਾਲਤ ਅਧਾਰ_Glyph:		ਵਾਪਸ HB_OT_LAYOUT_GLYPH_PROPS_ਅਧਾਰ_GLYPH;
    ਹਾਲਤ LigatureGlyph:		ਵਾਪਸ HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    ਹਾਲਤ MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  ਵਾਪਸ HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) ਅਚਲ;

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<GDEF> (face);
      ਜੇ (unlikely (ਸਾਰਣੀ->is_blocklisted (ਸਾਰਣੀ.get_blob (), face)))
      {
	hb_blob_destroy (ਸਾਰਣੀ.get_blob ());
	ਸਾਰਣੀ = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
      }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t> sets;
	ਸਾਰਣੀ->get_mark_glyph_sets ().collect_coverage (sets);
	ਜੇ (!sets.ਵਿੱਚ_error () &&
	    mark_glyph_ਸੈੱਟ_bitmaps.resize (sets.ਲੰਮਾਈ))
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sets.ਲੰਮਾਈ; i++)
	  {
	    ਸਵੈਚਾਲਿਤ &bitmap = mark_glyph_ਸੈੱਟ_bitmaps.arrayZ[i];
	    bitmap.bias = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
	    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਸੈੱਟ = sets.arrayZ[i];
	    ਜੇ (unlikely (ਸੈੱਟ.ਵਿੱਚ_error ())) ਜਾਰੀ;
	    hb_codepoint_t min_gid = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
	    hb_codepoint_t max_gid = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
	    hb_ਸੈੱਟ_ਅਗਲਾ (&ਸੈੱਟ, &min_gid);
	    hb_ਸੈੱਟ_ਪਿਛਲਾ (&ਸੈੱਟ, &max_gid);
	    ਜੇ (max_gid == HB_ਸੈੱਟ_ਮੁੱਲ_INVALID)
	      ਜਾਰੀ;
	    ਜੇ (max_gid - min_gid >= hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      ਲਈ (hb_codepoint_t gid : ਸੈੱਟ)
		bitmap.u.digest.ਜੋੜੋ (gid);
	      ਜਾਰੀ;
	    }
	    bitmap.bias = min_gid;
	    ਲਈ (hb_codepoint_t gid : ਸੈੱਟ)
	      bitmap.u.ਪੰਨਾ.ਜੋੜੋ (gid - min_gid);
	  }
      }
#ਜੇ_ਅੰਤ
    }
    ~accelerator_t () { ਸਾਰਣੀ.destroy (); }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_props (hb_codepoint_t glyph) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ v;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
      ਜੇ (glyph_props_cache.get (glyph, &v))
        ਵਾਪਸ v;
#ਜੇ_ਅੰਤ

      v = ਸਾਰਣੀ->get_glyph_props (glyph);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
      ਜੇ (likely (ਸਾਰਣੀ.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.ਸੈੱਟ (glyph, v);
#ਜੇ_ਅੰਤ

      ਵਾਪਸ v;

    }

    HB_ALWAYS_INLINE
    ਤਾਰਕਿਕ mark_ਸੈੱਟ_covers (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਸੂਚਕਾਂਕ, hb_codepoint_t glyph_id) ਅਚਲ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      ਅਚਲ ਸਵੈਚਾਲਿਤ &bitmap = mark_glyph_ਸੈੱਟ_bitmaps.arrayZ[ਸੈੱਟ_ਸੂਚਕਾਂਕ];
      ਜੇ (likely (bitmap.bias != HB_ਸੈੱਟ_ਮੁੱਲ_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	ਵਾਪਸ biased < hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਪੰਨਾ_ਬਿੱਟ &&
	       bitmap.u.ਪੰਨਾ.get (biased);
      }
      ਵਾਪਸ mark_ਸੈੱਟ_covers_slow (ਸੈੱਟ_ਸੂਚਕਾਂਕ, glyph_id);
#ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਸਾਰਣੀ->mark_ਸੈੱਟ_covers (ਸੈੱਟ_ਸੂਚਕਾਂਕ, glyph_id);
#ਜੇ_ਅੰਤ
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    ਤਾਰਕਿਕ mark_ਸੈੱਟ_covers_slow (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਸੂਚਕਾਂਕ, hb_codepoint_t glyph_id) ਅਚਲ
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &bitmap = mark_glyph_ਸੈੱਟ_bitmaps.arrayZ[ਸੈੱਟ_ਸੂਚਕਾਂਕ];
      ਜੇ (!bitmap.u.digest.may_have (glyph_id))
	ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ ਸਾਰਣੀ->mark_ਸੈੱਟ_covers (ਸੈੱਟ_ਸੂਚਕਾਂਕ, glyph_id);
    }
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ sanitize_lookup_props (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_props) ਅਚਲ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
      ਜੇ (lookup_props & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ &&
	  (lookup_props >> 16) >= mark_glyph_ਸੈੱਟ_bitmaps.ਲੰਮਾਈ)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_ਸੈੱਟ;
      }
#ਜੇ_ਅੰਤ
      ਵਾਪਸ lookup_props;
    }

    hb_blob_ptr_t<GDEF> ਸਾਰਣੀ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_GDEF_CACHE
    ਬਣਤਰ mark_glyph_ਸੈੱਟ_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਸੰਘ u_t {
	u_t () : ਪੰਨਾ () {}
	hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t ਪੰਨਾ;
	hb_ਸੈੱਟ_digest_t digest;
      } u;
    };

    hb_ਸਦਿਸ਼_t<mark_glyph_ਸੈੱਟ_bitmap_t> mark_glyph_ਸੈੱਟ_bitmaps;
    ਬਦਲਣਯੋਗ hb_cache_t<21, 3> glyph_props_cache;
    static_assert (ਆਕਾਰ (glyph_props_cache) == 512, "");
#ਜੇ_ਅੰਤ
  };

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010004u && ligCaretList2)
    {
      (ਇਹ+ligCaretList2).collect_variation_indices (c);
      ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    (ਇਹ+ligCaretList).collect_variation_indices (c);
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਕੁੰਜੀ>
  ਸਥਿਰ ਖਾਲੀ remap_varidx_after_instantiation (ਅਚਲ hb_map_t& varidx_map,
                                                hb_hashmap_t<ਕੁੰਜੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.ਪਹਿਲਾ;
      uint32_t *new_varidx;
      ਜੇ (varidx_map.has (varidx, &new_varidx))
	_.second.ਪਹਿਲਾ = *new_varidx;
      ਨਹੀਂਤਾਂ
	_.second.ਪਹਿਲਾ = HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
    }
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

  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;		/* Version of the GDEF table--currently
					 * 0x00010004u */
  Offset16_ਮੰਜ਼ਿਲ<ClassDef>
		glyphClassDef;		/* Offset to class definition table
					 * for glyph type--from beginning of
					 * GDEF header (may be Null) */
  Offset16_ਮੰਜ਼ਿਲ<AttachList>
		attachList;		/* Offset to list of glyphs with
					 * attachment points--from beginning
					 * of GDEF header (may be Null) */
  Offset16_ਮੰਜ਼ਿਲ<LigCaretList<SmallTypes>>
		ligCaretList;		/* Offset to list of positioning points
					 * for ligature carets--from beginning
					 * of GDEF header (may be Null) */
  Offset16_ਮੰਜ਼ਿਲ<ClassDef>
		markAttachClassDef;	/* Offset to class definition table for
					 * mark attachment type--from beginning
					 * of GDEF header (may be Null) */
  Offset16_ਮੰਜ਼ਿਲ<MarkGlyphSets>
		markGlyphSetsDef;	/* Offset to the table of mark set
					 * definitions--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010002. */
  Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  Offset32_ਮੰਜ਼ਿਲ<ClassDef>
		glyphClassDef2;		/* 32-bit offset to class definition
					 * table for glyph type.  Introduced
					 * in version 0x00010004. */
  Offset32_ਮੰਜ਼ਿਲ<AttachList>
		attachList2;		/* 32-bit offset to list of glyphs
					 * with attachment points.  Introduced
					 * in version 0x00010004. */
  Offset32_ਮੰਜ਼ਿਲ<LigCaretList<MediumTypes>>
		ligCaretList2;		/* 32-bit offset to LigCaretList2.
					 * Introduced in version 0x00010004. */
  Offset32_ਮੰਜ਼ਿਲ<ClassDef>
		markAttachClassDef2;	/* 32-bit offset to class definition
					 * table for mark attachment type.
					 * Introduced in version 0x00010004. */
  Offset32_ਮੰਜ਼ਿਲ<MarkGlyphSets>
		markGlyphSetsDef2;	/* 32-bit offset to the table of mark
					 * set definitions.  Introduced in
					 * version 0x00010004. */
#ਜੇ_ਅੰਤ
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (12);
};

ਬਣਤਰ GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_LAYOUT_GDEF_GDEF_HH */
