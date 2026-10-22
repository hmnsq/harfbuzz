#अगर_अघोषित OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#घोषणा OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा AnchorFormat3
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 3 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  Offset16To<Device>
                xDeviceTable;           /* Offset to Device table for X
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  Offset16To<Device>
                yDeviceTable;           /* Offset to Device table for Y
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  खुला:
  DEFINE_आकार_STATIC (10);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह))) return_trace (false);

    return_trace (xDeviceTable.sanitize (c, यह) && yDeviceTable.sanitize (c, यह));
  }

  व्योम get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   भग्न *x, भग्न *y) स्थिर
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    अगर ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, यह))
    {
      hb_barrier ();
      *x += (यह+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    अगर ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, यह))
    {
      hb_barrier ();
      *y += (यह+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->embed (format))) return_trace (false);
    अगर (unlikely (!c->serializer->embed (xCoordinate))) return_trace (false);
    अगर (unlikely (!c->serializer->embed (yCoordinate))) return_trace (false);

    अचिन्हित x_varidx = xDeviceTable ? (यह+xDeviceTable).get_variation_सूचकांक () : HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
    अगर (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
    {
      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      अगर (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (false);

      x_varidx = hb_first (*new_varidx_delta);
      पूर्णांक delta = hb_second (*new_varidx_delta);
      अगर (delta != 0)
      {
        अगर (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (false);
      }
    }

    अचिन्हित y_varidx = yDeviceTable ? (यह+yDeviceTable).get_variation_सूचकांक () : HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
    अगर (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
    {
      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      अगर (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (false);

      y_varidx = hb_first (*new_varidx_delta);
      पूर्णांक delta = hb_second (*new_varidx_delta);
      अगर (delta != 0)
      {
        अगर (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (false);
      }
    }


    bool no_downgrade = (!xDeviceTable.is_null () && !(यह+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक ||
                        (!yDeviceTable.is_null () && !(यह+yDeviceTable).is_variation_device ());

    अगर (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    अगर (!c->serializer->embed (xDeviceTable)) return_trace (false);
    अगर (!c->serializer->embed (yDeviceTable)) return_trace (false);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, यह, 0, hb_serialize_context_t::शीर्ष, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, यह, 0, hb_serialize_context_t::शीर्ष, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    (यह+xDeviceTable).collect_variation_indices (c);
    (यह+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
