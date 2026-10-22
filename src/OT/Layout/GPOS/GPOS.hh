#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_GPOS_HH
#నిర్వచించు OT_LAYOUT_GPOS_GPOS_HH

#అన్తర్భావ "../../../hb-ot-layout-common.hh"
#అన్తర్భావ "../../../hb-ot-layout-gsubgpos.hh"
#అన్తర్భావ "Common.hh"
#అన్తర్భావ "PosLookup.hh"

నామాకాశ OT {

ఉపయోగించి Layout::GPOS_impl::PosLookup;

నామాకాశ Layout {

స్థిర శూన్య
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              గుర్తులేని పూర్ణాంకం len,
                              గుర్తులేని పూర్ణాంకం i,
                              hb_direction_t direction,
                              గుర్తులేని nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

నిర్మాణం GPOS : GSUBGPOS
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_GPOS;

  ఉపయోగించి Lookup = PosLookup;

  స్థిరరాశి PosLookup& get_lookup (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి static_cast<స్థిరరాశి PosLookup &> (GSUBGPOS::get_lookup (i)); }

  స్థిర అంతఃవరుస శూన్య position_ప్రారంభం (hb_font_t *font, hb_బఫరు_t *బఫరు);
  స్థిర అంతఃవరుస శూన్య position_ముగింపు_advances (hb_font_t *font, hb_బఫరు_t *బఫరు);
  స్థిర అంతఃవరుస శూన్య position_ముగింపు_offsets (hb_font_t *font, hb_బఫరు_t *బఫరు);

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    hb_subset_layout_context_t l (c, పట్టిక_ట్యాగ్);
    తిరిగి GSUBGPOS::subset<PosLookup> (&l);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_అంతర్గత తార్కిక is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) స్థిరరాశి;

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < GSUBGPOS::get_lookup_లెక్క (); i++)
    {
      ఒకవేళ (!c->gpos_lookups->has (i)) కొనసాగు;
      స్థిరరాశి PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  శూన్య సంవృతం_lookups (hb_face_t      *face,
                        స్థిరరాశి hb_సమితి_t *glyphs,
                        hb_సమితి_t       *lookup_indexes /* IN/OUT */) స్థిరరాశి
  { GSUBGPOS::సంవృతం_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  రకనిర్వచనం GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


స్థిర శూన్య
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              గుర్తులేని పూర్ణాంకం len,
                              గుర్తులేని పూర్ణాంకం i,
                              hb_direction_t direction,
                              గుర్తులేని nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  పూర్ణాంకం chain = pos[i].attach_chain();
  పూర్ణాంకం type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  గుర్తులేని పూర్ణాంకం j = (పూర్ణాంకం) i + chain;

  ఒకవేళ (unlikely (j >= len))
    తిరిగి;

  ఒకవేళ (unlikely (!nesting_level))
    తిరిగి;

  ఒకవేళ (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  ప్రతిజ్ఞా (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  ఒకవేళ (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].y_స్థానభ్రంశం_2, pos[j].y_స్థానభ్రంశం_2);
    లేకపోతే
      pos[i].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].x_స్థానభ్రంశం_2, pos[j].x_స్థానభ్రంశం_2);
  }
  లేకపోతే /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].x_స్థానభ్రంశం_2, pos[j].x_స్థానభ్రంశం_2);
    లేకపోతే
      pos[i].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].y_స్థానభ్రంశం_2, pos[j].y_స్థానభ్రంశం_2);

    // i is the position of the mark; j is the base.
    ఒకవేళ (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      ఒకవేళ (HB_DIRECTION_IS_FORWARD (direction))
	కొరకు (గుర్తులేని పూర్ణాంకం k = j; k < i; k++) {
	  pos[i].x_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].x_స్థానభ్రంశం_2, pos[k].x_advance);
	  pos[i].y_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].y_స్థానభ్రంశం_2, pos[k].y_advance);
	}
      లేకపోతే
	కొరకు (గుర్తులేని పూర్ణాంకం k = j + 1; k < i + 1; k++) {
	  pos[i].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].x_స్థానభ్రంశం_2, pos[k].x_advance);
	  pos[i].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].y_స్థానభ్రంశం_2, pos[k].y_advance);
	}
    }
    లేకపోతే // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      ఒకవేళ (HB_DIRECTION_IS_FORWARD (direction))
	కొరకు (గుర్తులేని పూర్ణాంకం k = i; k < j; k++) {
	  pos[i].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].x_స్థానభ్రంశం_2, pos[k].x_advance);
	  pos[i].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].y_స్థానభ్రంశం_2, pos[k].y_advance);
	}
      లేకపోతే
	కొరకు (గుర్తులేని పూర్ణాంకం k = i + 1; k < j + 1; k++) {
	  pos[i].x_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].x_స్థానభ్రంశం_2, pos[k].x_advance);
	  pos[i].y_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].y_స్థానభ్రంశం_2, pos[k].y_advance);
	}
    }
  }
}

శూన్య
GPOS::position_ప్రారంభం (hb_font_t *font HB_వాడని, hb_బఫరు_t *బఫరు)
{
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    బఫరు->pos[i].attach_chain() = బఫరు->pos[i].attach_type() = 0;
}

శూన్య
GPOS::position_ముగింపు_advances (hb_font_t *font HB_వాడని, hb_బఫరు_t *బఫరు HB_వాడని)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

శూన్య
GPOS::position_ముగింపు_offsets (hb_font_t *font, hb_బఫరు_t *బఫరు)
{
  hb_బఫరు_assert_gsubgpos_vars (బఫరు);

  గుర్తులేని పూర్ణాంకం len;
  hb_glyph_position_t *pos = hb_బఫరు_get_glyph_positions (బఫరు, &len);
  hb_direction_t direction = బఫరు->props.direction;

  /* Handle attachments */
  ఒకవేళ (బఫరు->scratch_flags & HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    స్వయంచాలక *pos = బఫరు->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    ఒకవేళ (HB_DIRECTION_IS_FORWARD (direction))
    {
      కొరకు (గుర్తులేని i = 0; i < len; i++)
	ఒకవేళ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } లేకపోతే {
      కొరకు (గుర్తులేని i = len; i-- > 0; )
	ఒకవేళ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  ఒకవేళ (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    కొరకు (గుర్తులేని i = 0; i < len; i++)
      ఒకవేళ (unlikely (pos[i].y_స్థానభ్రంశం_2))
        pos[i].x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) pos[i].x_స్థానభ్రంశం_2 +
						      (ద్విగుణ) roundf (font->slant_xy * pos[i].y_స్థానభ్రంశం_2));
  }
}

}

నిర్మాణం GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#అయితే_ముగింపు  /* OT_LAYOUT_GPOS_GPOS_HH */
