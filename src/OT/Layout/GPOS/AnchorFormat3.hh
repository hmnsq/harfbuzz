#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار AnchorFormat3
{
  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_STATIC (10);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این))) return_trace (نادرست);

    return_trace (xDeviceTable.sanitize (c, این) && yDeviceTable.sanitize (c, این));
  }

  تهی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   اعشاری *x, اعشاری *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    اگر ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, این))
    {
      hb_barrier ();
      *x += (این+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    اگر ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, این))
    {
      hb_barrier ();
      *y += (این+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->embed (format))) return_trace (نادرست);
    اگر (unlikely (!c->serializer->embed (xCoordinate))) return_trace (نادرست);
    اگر (unlikely (!c->serializer->embed (yCoordinate))) return_trace (نادرست);

    unsigned x_varidx = xDeviceTable ? (این+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    اگر (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, صحیح> *new_varidx_delta;
      اگر (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (نادرست);

      x_varidx = hb_first (*new_varidx_delta);
      صحیح delta = hb_second (*new_varidx_delta);
      اگر (delta != 0)
      {
        اگر (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (نادرست);
      }
    }

    unsigned y_varidx = yDeviceTable ? (این+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    اگر (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, صحیح> *new_varidx_delta;
      اگر (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (نادرست);

      y_varidx = hb_first (*new_varidx_delta);
      صحیح delta = hb_second (*new_varidx_delta);
      اگر (delta != 0)
      {
        اگر (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (نادرست);
      }
    }


    منطقی no_downgrade = (!xDeviceTable.is_null () && !(این+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(این+yDeviceTable).is_variation_device ());

    اگر (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    اگر (!c->serializer->embed (xDeviceTable)) return_trace (نادرست);
    اگر (!c->serializer->embed (yDeviceTable)) return_trace (نادرست);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, این, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, این, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (این+xDeviceTable).collect_variation_indices (c);
    (این+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
