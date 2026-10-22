#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

கட்டமைப்பு AnchorFormat3
{
  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_STATIC (10);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது))) return_trace (பொய்);

    return_trace (xDeviceTable.sanitize (c, இது) && yDeviceTable.sanitize (c, இது));
  }

  வெறுமை get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   தசம *x, தசம *y) மாறிலி
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    எனில் ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, இது))
    {
      hb_barrier ();
      *x += (இது+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    எனில் ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, இது))
    {
      hb_barrier ();
      *y += (இது+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->embed (format))) return_trace (பொய்);
    எனில் (unlikely (!c->serializer->embed (xCoordinate))) return_trace (பொய்);
    எனில் (unlikely (!c->serializer->embed (yCoordinate))) return_trace (பொய்);

    unsigned x_varidx = xDeviceTable ? (இது+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    எனில் (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, முழு> *new_varidx_delta;
      எனில் (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (பொய்);

      x_varidx = hb_first (*new_varidx_delta);
      முழு delta = hb_second (*new_varidx_delta);
      எனில் (delta != 0)
      {
        எனில் (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (பொய்);
      }
    }

    unsigned y_varidx = yDeviceTable ? (இது+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    எனில் (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, முழு> *new_varidx_delta;
      எனில் (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (பொய்);

      y_varidx = hb_first (*new_varidx_delta);
      முழு delta = hb_second (*new_varidx_delta);
      எனில் (delta != 0)
      {
        எனில் (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (பொய்);
      }
    }


    தருக்க no_downgrade = (!xDeviceTable.is_null () && !(இது+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(இது+yDeviceTable).is_variation_device ());

    எனில் (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    எனில் (!c->serializer->embed (xDeviceTable)) return_trace (பொய்);
    எனில் (!c->serializer->embed (yDeviceTable)) return_trace (பொய்);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, இது, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, இது, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    (இது+xDeviceTable).collect_variation_indices (c);
    (இது+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
