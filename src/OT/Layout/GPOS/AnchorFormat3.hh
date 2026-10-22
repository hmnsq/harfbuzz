#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સંરચના AnchorFormat3
{
  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_STATIC (10);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ))) return_trace (ખોટું);

    return_trace (xDeviceTable.sanitize (c, આ) && yDeviceTable.sanitize (c, આ));
  }

  શૂન્ય get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   દશાંશ *x, દશાંશ *y) અચલ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    જો ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, આ))
    {
      hb_barrier ();
      *x += (આ+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    જો ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, આ))
    {
      hb_barrier ();
      *y += (આ+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->embed (format))) return_trace (ખોટું);
    જો (unlikely (!c->serializer->embed (xCoordinate))) return_trace (ખોટું);
    જો (unlikely (!c->serializer->embed (yCoordinate))) return_trace (ખોટું);

    unsigned x_varidx = xDeviceTable ? (આ+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    જો (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
      જો (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (ખોટું);

      x_varidx = hb_first (*new_varidx_delta);
      પૂર્ણાંક delta = hb_second (*new_varidx_delta);
      જો (delta != 0)
      {
        જો (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ખોટું);
      }
    }

    unsigned y_varidx = yDeviceTable ? (આ+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    જો (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
      જો (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (ખોટું);

      y_varidx = hb_first (*new_varidx_delta);
      પૂર્ણાંક delta = hb_second (*new_varidx_delta);
      જો (delta != 0)
      {
        જો (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ખોટું);
      }
    }


    તાર્કિક no_downgrade = (!xDeviceTable.is_null () && !(આ+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(આ+yDeviceTable).is_variation_device ());

    જો (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    જો (!c->serializer->embed (xDeviceTable)) return_trace (ખોટું);
    જો (!c->serializer->embed (yDeviceTable)) return_trace (ખોટું);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, આ, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, આ, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    (આ+xDeviceTable).collect_variation_indices (c);
    (આ+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
