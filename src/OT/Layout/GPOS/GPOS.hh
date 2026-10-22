#अगर_अघोषित OT_LAYOUT_GPOS_GPOS_HH
#घोषणा OT_LAYOUT_GPOS_GPOS_HH

#समावेश "../../../hb-ot-layout-common.hh"
#समावेश "../../../hb-ot-layout-gsubgpos.hh"
#समावेश "Common.hh"
#समावेश "PosLookup.hh"

namespace OT {

using Layout::GPOS_impl::PosLookup;

namespace Layout {

अटल व्योम
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              अचिन्हित पूर्णांक len,
                              अचिन्हित पूर्णांक i,
                              hb_direction_t direction,
                              अचिन्हित nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

काष्ठा GPOS : GSUBGPOS
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  स्थिर PosLookup& get_lookup (अचिन्हित पूर्णांक i) स्थिर
  { वापस static_cast<स्थिर PosLookup &> (GSUBGPOS::get_lookup (i)); }

  अटल अंतरभूत व्योम position_start (hb_font_t *font, hb_बफर_t *बफर);
  अटल अंतरभूत व्योम position_finish_advances (hb_font_t *font, hb_बफर_t *बफर);
  अटल अंतरभूत व्योम position_finish_offsets (hb_font_t *font, hb_बफर_t *बफर);

  bool subset (hb_subset_context_t *c) स्थिर
  {
    hb_subset_layout_context_t l (c, tableTag);
    वापस GSUBGPOS::subset<PosLookup> (&l);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिर;

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < GSUBGPOS::get_lookup_गिनती (); i++)
    {
      अगर (!c->gpos_lookups->has (i)) जारी;
      स्थिर PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  व्योम closure_lookups (hb_face_t      *face,
                        स्थिर hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) स्थिर
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  प्रकार GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


अटल व्योम
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              अचिन्हित पूर्णांक len,
                              अचिन्हित पूर्णांक i,
                              hb_direction_t direction,
                              अचिन्हित nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  पूर्णांक chain = pos[i].attach_chain();
  पूर्णांक type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  अचिन्हित पूर्णांक j = (पूर्णांक) i + chain;

  अगर (unlikely (j >= len))
    वापस;

  अगर (unlikely (!nesting_level))
    वापस;

  अगर (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  निश्चित (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  अगर (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    अगर (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    अन्यथा
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  अन्यथा /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    अगर (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    अन्यथा
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    अगर (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      अगर (HB_DIRECTION_IS_FORWARD (direction))
	क्रम (अचिन्हित पूर्णांक k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      अन्यथा
	क्रम (अचिन्हित पूर्णांक k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    अन्यथा // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      अगर (HB_DIRECTION_IS_FORWARD (direction))
	क्रम (अचिन्हित पूर्णांक k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      अन्यथा
	क्रम (अचिन्हित पूर्णांक k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

व्योम
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_बफर_t *बफर)
{
  अचिन्हित पूर्णांक गिनती = बफर->len;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    बफर->pos[i].attach_chain() = बफर->pos[i].attach_type() = 0;
}

व्योम
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_बफर_t *बफर HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

व्योम
GPOS::position_finish_offsets (hb_font_t *font, hb_बफर_t *बफर)
{
  hb_बफर_assert_gsubgpos_vars (बफर);

  अचिन्हित पूर्णांक len;
  hb_glyph_position_t *pos = hb_बफर_get_glyph_positions (बफर, &len);
  hb_direction_t direction = बफर->props.direction;

  /* Handle attachments */
  अगर (बफर->scratch_flags & HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    स्वतः *pos = बफर->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    अगर (HB_DIRECTION_IS_FORWARD (direction))
    {
      क्रम (अचिन्हित i = 0; i < len; i++)
	अगर (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } अन्यथा {
      क्रम (अचिन्हित i = len; i-- > 0; )
	अगर (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  अगर (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    क्रम (अचिन्हित i = 0; i < len; i++)
      अगर (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((द्विगुन) pos[i].x_offset +
						      (द्विगुन) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

काष्ठा GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#पूर्ण_अगर  /* OT_LAYOUT_GPOS_GPOS_HH */
