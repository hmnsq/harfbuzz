#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ AnchorFormat3
{
  ರಕ್ಷಿತ:
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    return_trace (xDeviceTable.sanitize (c, ಏತತ್) && yDeviceTable.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ಭಿನ್ನ *x, ಭಿನ್ನ *y) ಧ್ರುವ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    ಯದಿ ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, ಏತತ್))
    {
      hb_barrier ();
      *x += (ಏತತ್+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    ಯದಿ ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, ಏತತ್))
    {
      hb_barrier ();
      *y += (ಏತತ್+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->embed (format))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->serializer->embed (xCoordinate))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->serializer->embed (yCoordinate))) return_trace (ಅಸತ್ಯ);

    unsigned x_varidx = xDeviceTable ? (ಏತತ್+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ಯದಿ (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *new_varidx_delta;
      ಯದಿ (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (ಅಸತ್ಯ);

      x_varidx = hb_first (*new_varidx_delta);
      ಪೂರ್ಣಾಙ್ಕ delta = hb_second (*new_varidx_delta);
      ಯದಿ (delta != 0)
      {
        ಯದಿ (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ಅಸತ್ಯ);
      }
    }

    unsigned y_varidx = yDeviceTable ? (ಏತತ್+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    ಯದಿ (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *new_varidx_delta;
      ಯದಿ (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (ಅಸತ್ಯ);

      y_varidx = hb_first (*new_varidx_delta);
      ಪೂರ್ಣಾಙ್ಕ delta = hb_second (*new_varidx_delta);
      ಯದಿ (delta != 0)
      {
        ಯದಿ (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (ಅಸತ್ಯ);
      }
    }


    ತಾರ್ಕಿಕ no_downgrade = (!xDeviceTable.is_null () && !(ಏತತ್+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(ಏತತ್+yDeviceTable).is_variation_device ());

    ಯದಿ (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    ಯದಿ (!c->serializer->embed (xDeviceTable)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->serializer->embed (yDeviceTable)) return_trace (ಅಸತ್ಯ);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, ಏತತ್, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, ಏತತ್, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+xDeviceTable).collect_variation_indices (c);
    (ಏತತ್+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
