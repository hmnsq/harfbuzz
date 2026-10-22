#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো AnchorFormat3
{
  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (10);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই))) return_trace (মিথ্যা);

    return_trace (xDeviceTable.sanitize (c, এই) && yDeviceTable.sanitize (c, এই));
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   দশমিক *x, দশমিক *y) ধ্রুবক
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    যদি ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, এই))
    {
      hb_barrier ();
      *x += (এই+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    যদি ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, এই))
    {
      hb_barrier ();
      *y += (এই+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->embed (format))) return_trace (মিথ্যা);
    যদি (unlikely (!c->serializer->embed (xCoordinate))) return_trace (মিথ্যা);
    যদি (unlikely (!c->serializer->embed (yCoordinate))) return_trace (মিথ্যা);

    unsigned x_varidx = xDeviceTable ? (এই+xDeviceTable).get_variation_সূচক () : HB_OT_LAYOUT_NO_VARIATIONS_সূচক;
    যদি (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
    {
      hb_pair_t<unsigned, পূর্ণসংখ্যা> *new_varidx_delta;
      যদি (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (মিথ্যা);

      x_varidx = hb_first (*new_varidx_delta);
      পূর্ণসংখ্যা delta = hb_second (*new_varidx_delta);
      যদি (delta != 0)
      {
        যদি (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (মিথ্যা);
      }
    }

    unsigned y_varidx = yDeviceTable ? (এই+yDeviceTable).get_variation_সূচক () : HB_OT_LAYOUT_NO_VARIATIONS_সূচক;
    যদি (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
    {
      hb_pair_t<unsigned, পূর্ণসংখ্যা> *new_varidx_delta;
      যদি (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (মিথ্যা);

      y_varidx = hb_first (*new_varidx_delta);
      পূর্ণসংখ্যা delta = hb_second (*new_varidx_delta);
      যদি (delta != 0)
      {
        যদি (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (মিথ্যা);
      }
    }


    যৌক্তিক no_downgrade = (!xDeviceTable.is_null () && !(এই+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_সূচক ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_সূচক ||
                        (!yDeviceTable.is_null () && !(এই+yDeviceTable).is_variation_device ());

    যদি (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    যদি (!c->serializer->embed (xDeviceTable)) return_trace (মিথ্যা);
    যদি (!c->serializer->embed (yDeviceTable)) return_trace (মিথ্যা);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, এই, 0, hb_serialize_context_t::শীর্ষ, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, এই, 0, hb_serialize_context_t::শীর্ষ, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    (এই+xDeviceTable).collect_variation_indices (c);
    (এই+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
