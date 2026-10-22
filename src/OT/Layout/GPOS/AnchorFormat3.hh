#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

সংরচনা AnchorFormat3
{
  রক্ষিত:
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
  সার্বজনিক:
  DEFINE_SIZE_STATIC (10);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্))) return_trace (অসত্য);

    return_trace (xDeviceTable.sanitize (c, এতত্) && yDeviceTable.sanitize (c, এতত্));
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ভিন্ন *x, ভিন্ন *y) ধ্রুব
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    যদি ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, এতত্))
    {
      hb_barrier ();
      *x += (এতত্+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    যদি ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, এতত্))
    {
      hb_barrier ();
      *y += (এতত্+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->embed (format))) return_trace (অসত্য);
    যদি (unlikely (!c->serializer->embed (xCoordinate))) return_trace (অসত্য);
    যদি (unlikely (!c->serializer->embed (yCoordinate))) return_trace (অসত্য);

    unsigned x_varidx = xDeviceTable ? (এতত্+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    যদি (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
      যদি (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (অসত্য);

      x_varidx = hb_first (*new_varidx_delta);
      পূর্ণাঙ্ক delta = hb_second (*new_varidx_delta);
      যদি (delta != 0)
      {
        যদি (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (অসত্য);
      }
    }

    unsigned y_varidx = yDeviceTable ? (এতত্+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    যদি (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
      যদি (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (অসত্য);

      y_varidx = hb_first (*new_varidx_delta);
      পূর্ণাঙ্ক delta = hb_second (*new_varidx_delta);
      যদি (delta != 0)
      {
        যদি (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (অসত্য);
      }
    }


    তার্কিক no_downgrade = (!xDeviceTable.is_null () && !(এতত্+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(এতত্+yDeviceTable).is_variation_device ());

    যদি (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    যদি (!c->serializer->embed (xDeviceTable)) return_trace (অসত্য);
    যদি (!c->serializer->embed (yDeviceTable)) return_trace (অসত্য);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, এতত্, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, এতত্, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    (এতত্+xDeviceTable).collect_variation_indices (c);
    (এতত্+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
