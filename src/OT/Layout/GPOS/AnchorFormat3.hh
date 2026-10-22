#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية AnchorFormat3
{
  محمي:
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
  عام:
  DEFINE_SIZE_STATIC (10);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!c->check_struct (هذا))) return_trace (ܕܓܠܐ);

    return_trace (xDeviceTable.sanitize (c, هذا) && yDeviceTable.sanitize (c, هذا));
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   عائم *x, عائم *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    ܐܢ ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, هذا))
    {
      hb_barrier ();
      *x += (هذا+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    ܐܢ ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, هذا))
    {
      hb_barrier ();
      *y += (هذا+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->embed (format))) return_trace (ܕܓܠܐ);
    ܐܢ (unlikely (!c->serializer->embed (xCoordinate))) return_trace (ܕܓܠܐ);
    ܐܢ (unlikely (!c->serializer->embed (yCoordinate))) return_trace (ܕܓܠܐ);

    unsigned x_varidx = xDeviceTable ? (هذا+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ܐܢ (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, صحيح> *new_varidx_delta;
      ܐܢ (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (ܕܓܠܐ);

      x_varidx = hb_first (*new_varidx_delta);
      صحيح delta = hb_second (*new_varidx_delta);
      ܐܢ (delta != 0)
      {
        ܐܢ (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ܕܓܠܐ);
      }
    }

    unsigned y_varidx = yDeviceTable ? (هذا+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ܐܢ (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, صحيح> *new_varidx_delta;
      ܐܢ (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (ܕܓܠܐ);

      y_varidx = hb_first (*new_varidx_delta);
      صحيح delta = hb_second (*new_varidx_delta);
      ܐܢ (delta != 0)
      {
        ܐܢ (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ܕܓܠܐ);
      }
    }


    منطقي no_downgrade = (!xDeviceTable.is_null () && !(هذا+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(هذا+yDeviceTable).is_variation_device ());

    ܐܢ (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    ܐܢ (!c->serializer->embed (xDeviceTable)) return_trace (ܕܓܠܐ);
    ܐܢ (!c->serializer->embed (yDeviceTable)) return_trace (ܕܓܠܐ);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, هذا, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, هذا, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+xDeviceTable).collect_variation_indices (c);
    (هذا+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
