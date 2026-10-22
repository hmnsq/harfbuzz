#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#నిర్వచించు OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం AnchorFormat3
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 3 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  Offset16_గమ్యం<Device>
                x_Device_పట్టిక;           /* Offset to Device table for X
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  Offset16_గమ్యం<Device>
                y_Device_పట్టిక;           /* Offset to Device table for Y
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది))) return_trace (అబద్ధం);

    return_trace (x_Device_పట్టిక.sanitize (c, ఇది) && y_Device_పట్టిక.sanitize (c, ఇది));
  }

  శూన్య get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_వాడని,
                   దశాంశం *x, దశాంశం *y) స్థిరరాశి
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    ఒకవేళ ((font->x_ppem || font->has_nonzero_coords) && x_Device_పట్టిక.sanitize (&c->sanitizer, ఇది))
    {
      hb_barrier ();
      *x += (ఇది+x_Device_పట్టిక).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    ఒకవేళ ((font->y_ppem || font->has_nonzero_coords) && y_Device_పట్టిక.sanitize (&c->sanitizer, ఇది))
    {
      hb_barrier ();
      *y += (ఇది+y_Device_పట్టిక).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->embed (format))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->serializer->embed (xCoordinate))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->serializer->embed (yCoordinate))) return_trace (అబద్ధం);

    గుర్తులేని x_varidx = x_Device_పట్టిక ? (ఇది+x_Device_పట్టిక).get_variation_సూచీ () : HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
    ఒకవేళ (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
    {
      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
      ఒకవేళ (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (అబద్ధం);

      x_varidx = hb_మొదటి (*new_varidx_delta);
      పూర్ణాంకం delta = hb_రెండవ (*new_varidx_delta);
      ఒకవేళ (delta != 0)
      {
        ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
          return_trace (అబద్ధం);
      }
    }

    గుర్తులేని y_varidx = y_Device_పట్టిక ? (ఇది+y_Device_పట్టిక).get_variation_సూచీ () : HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
    ఒకవేళ (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
    {
      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
      ఒకవేళ (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (అబద్ధం);

      y_varidx = hb_మొదటి (*new_varidx_delta);
      పూర్ణాంకం delta = hb_రెండవ (*new_varidx_delta);
      ఒకవేళ (delta != 0)
      {
        ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
          return_trace (అబద్ధం);
      }
    }


    తార్కిక no_downgrade = (!x_Device_పట్టిక.is_null () && !(ఇది+x_Device_పట్టిక).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ ||
                        (!y_Device_పట్టిక.is_null () && !(ఇది+y_Device_పట్టిక).is_variation_device ());

    ఒకవేళ (!no_downgrade)
      return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->format, 1, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));

    ఒకవేళ (!c->serializer->embed (x_Device_పట్టిక)) return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->embed (y_Device_పట్టిక)) return_trace (అబద్ధం);

    బహిర్గతం->x_Device_పట్టిక.serialize_copy (c->serializer, x_Device_పట్టిక, ఇది, 0, hb_serialize_context_t::తల, &c->plan->layout_variation_idx_delta_map);
    బహిర్గతం->y_Device_పట్టిక.serialize_copy (c->serializer, y_Device_పట్టిక, ఇది, 0, hb_serialize_context_t::తల, &c->plan->layout_variation_idx_delta_map);
    return_trace (బహిర్గతం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    (ఇది+x_Device_పట్టిక).collect_variation_indices (c);
    (ఇది+y_Device_పట్టిక).collect_variation_indices (c);
  }
};


}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
