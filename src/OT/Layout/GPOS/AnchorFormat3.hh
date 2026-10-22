#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന AnchorFormat3
{
  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_STATIC (10);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്))) return_trace (അസത്യം);

    return_trace (xDeviceTable.sanitize (c, ഇത്) && yDeviceTable.sanitize (c, ഇത്));
  }

  ശൂന്യം get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ദശാംശം *x, ദശാംശം *y) സ്ഥിരം
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    എങ്കിൽ ((font->x_ppem || font->has_nonzero_coords) && xDeviceTable.sanitize (&c->sanitizer, ഇത്))
    {
      hb_barrier ();
      *x += (ഇത്+xDeviceTable).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    എങ്കിൽ ((font->y_ppem || font->has_nonzero_coords) && yDeviceTable.sanitize (&c->sanitizer, ഇത്))
    {
      hb_barrier ();
      *y += (ഇത്+yDeviceTable).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->embed (format))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->serializer->embed (xCoordinate))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->serializer->embed (yCoordinate))) return_trace (അസത്യം);

    unsigned x_varidx = xDeviceTable ? (ഇത്+xDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    എങ്കിൽ (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *new_varidx_delta;
      എങ്കിൽ (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (അസത്യം);

      x_varidx = hb_first (*new_varidx_delta);
      പൂർണ്ണസംഖ്യ delta = hb_second (*new_varidx_delta);
      എങ്കിൽ (delta != 0)
      {
        എങ്കിൽ (!c->serializer->check_assign (out->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (അസത്യം);
      }
    }

    unsigned y_varidx = yDeviceTable ? (ഇത്+yDeviceTable).get_variation_index () : HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    എങ്കിൽ (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
    {
      hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *new_varidx_delta;
      എങ്കിൽ (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (അസത്യം);

      y_varidx = hb_first (*new_varidx_delta);
      പൂർണ്ണസംഖ്യ delta = hb_second (*new_varidx_delta);
      എങ്കിൽ (delta != 0)
      {
        എങ്കിൽ (!c->serializer->check_assign (out->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_OVERFLOW))
          return_trace (അസത്യം);
      }
    }


    തർക്കം no_downgrade = (!xDeviceTable.is_null () && !(ഇത്+xDeviceTable).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX ||
                        (!yDeviceTable.is_null () && !(ഇത്+yDeviceTable).is_variation_device ());

    എങ്കിൽ (!no_downgrade)
      return_trace (c->serializer->check_assign (out->format, 1, HB_SERIALIZE_ERROR_INT_OVERFLOW));

    എങ്കിൽ (!c->serializer->embed (xDeviceTable)) return_trace (അസത്യം);
    എങ്കിൽ (!c->serializer->embed (yDeviceTable)) return_trace (അസത്യം);

    out->xDeviceTable.serialize_copy (c->serializer, xDeviceTable, ഇത്, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    out->yDeviceTable.serialize_copy (c->serializer, yDeviceTable, ഇത്, 0, hb_serialize_context_t::Head, &c->plan->layout_variation_idx_delta_map);
    return_trace (out);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    (ഇത്+xDeviceTable).collect_variation_indices (c);
    (ഇത്+yDeviceTable).collect_variation_indices (c);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
