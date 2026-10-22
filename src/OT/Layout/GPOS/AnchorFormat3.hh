#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା AnchorFormat3
{
  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି))) return_trace (ମିଥ୍ୟା);

    return_trace (xDeviceTable.sanitize (c, ଏହି) && yDeviceTable.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ଦଶମିକ *x, ଦଶମିକ *y) ଅଚଳ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    ଯଦି ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, ଏହି))
    {
      hb_barrier ();
      *x += (ଏହି+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    ଯଦି ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, ଏହି))
    {
      hb_barrier ();
      *y += (ଏହି+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->embed (format))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->serializer->embed (xCoordinate))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->serializer->embed (yCoordinate))) return_trace (ମିଥ୍ୟା);

    unsigned x_varidx = xDeviceTable ? (ଏହି+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ଯଦି (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
      ଯଦି (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (ମିଥ୍ୟା);

      x_varidx = hb_first (*new_varidx_delta);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = hb_second (*new_varidx_delta);
      ଯଦି (delta != 0)
      {
        ଯଦି (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ମିଥ୍ୟା);
      }
    }

    unsigned y_varidx = yDeviceTable ? (ଏହି+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ଯଦି (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
      ଯଦି (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (ମିଥ୍ୟା);

      y_varidx = hb_first (*new_varidx_delta);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = hb_second (*new_varidx_delta);
      ଯଦି (delta != 0)
      {
        ଯଦି (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ମିଥ୍ୟା);
      }
    }


    ତାର୍କିକ no_downgrade = (!xDeviceTable.is_null () && !(ଏହି+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(ଏହି+yDeviceTable).is_variation_device ());

    ଯଦି (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    ଯଦି (!c->serializer->embed (xDeviceTable)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->serializer->embed (yDeviceTable)) return_trace (ମିଥ୍ୟା);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, ଏହି, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, ଏହି, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    (ଏହି+xDeviceTable).collect_variation_indices (c);
    (ଏହି+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
