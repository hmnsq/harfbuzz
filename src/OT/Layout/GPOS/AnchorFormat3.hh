#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना AnchorFormat3
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्))) return_trace (असत्य);

    return_trace (xDeviceTable.sanitize (c, एतत्) && yDeviceTable.sanitize (c, एतत्));
  }

  शून्य get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   भिन्न *x, भिन्न *y) ध्रुव
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    यदि ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, एतत्))
    {
      hb_barrier ();
      *x += (एतत्+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    यदि ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, एतत्))
    {
      hb_barrier ();
      *y += (एतत्+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->embed (format))) return_trace (असत्य);
    यदि (unlikely (!c->serializer->embed (xCoordinate))) return_trace (असत्य);
    यदि (unlikely (!c->serializer->embed (yCoordinate))) return_trace (असत्य);

    अचिन्हित x_varidx = xDeviceTable ? (एतत्+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    यदि (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<अचिन्हित, पूर्णाङ्क> *new_varidx_delta;
      यदि (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (असत्य);

      x_varidx = hb_first (*new_varidx_delta);
      पूर्णाङ्क delta = hb_second (*new_varidx_delta);
      यदि (delta != 0)
      {
        यदि (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (असत्य);
      }
    }

    अचिन्हित y_varidx = yDeviceTable ? (एतत्+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    यदि (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<अचिन्हित, पूर्णाङ्क> *new_varidx_delta;
      यदि (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (असत्य);

      y_varidx = hb_first (*new_varidx_delta);
      पूर्णाङ्क delta = hb_second (*new_varidx_delta);
      यदि (delta != 0)
      {
        यदि (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (असत्य);
      }
    }


    तार्किक no_downgrade = (!xDeviceTable.is_null () && !(एतत्+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(एतत्+yDeviceTable).is_variation_device ());

    यदि (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    यदि (!c->serializer->embed (xDeviceTable)) return_trace (असत्य);
    यदि (!c->serializer->embed (yDeviceTable)) return_trace (असत्य);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, एतत्, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, एतत्, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    (एतत्+xDeviceTable).collect_variation_indices (c);
    (एतत्+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
