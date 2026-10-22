#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ AnchorFormat3
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 3 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  Offset16_ਮੰਜ਼ਿਲ<Device>
                x_Device_ਸਾਰਣੀ;           /* Offset to Device table for X
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<Device>
                y_Device_ਸਾਰਣੀ;           /* Offset to Device table for Y
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ))) return_trace (ਝੂਠ);

    return_trace (x_Device_ਸਾਰਣੀ.sanitize (c, ਇਹ) && y_Device_ਸਾਰਣੀ.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_ਅਣਵਰਤਿਆ,
                   ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *y) ਅਚਲ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    ਜੇ ((font->x_ppem || font->has_nonzero_coords) && x_Device_ਸਾਰਣੀ.sanitize (&c->sanitizer, ਇਹ))
    {
      hb_barrier ();
      *x += (ਇਹ+x_Device_ਸਾਰਣੀ).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    ਜੇ ((font->y_ppem || font->has_nonzero_coords) && y_Device_ਸਾਰਣੀ.sanitize (&c->sanitizer, ਇਹ))
    {
      hb_barrier ();
      *y += (ਇਹ+y_Device_ਸਾਰਣੀ).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->embed (format))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->serializer->embed (xCoordinate))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->serializer->embed (yCoordinate))) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ x_varidx = x_Device_ਸਾਰਣੀ ? (ਇਹ+x_Device_ਸਾਰਣੀ).get_variation_ਸੂਚਕਾਂਕ () : HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
    ਜੇ (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
    {
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
      ਜੇ (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (ਝੂਠ);

      x_varidx = hb_ਪਹਿਲਾ (*new_varidx_delta);
      ਪੂਰਨ delta = hb_ਦੂਜਾ (*new_varidx_delta);
      ਜੇ (delta != 0)
      {
        ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
          return_trace (ਝੂਠ);
      }
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ y_varidx = y_Device_ਸਾਰਣੀ ? (ਇਹ+y_Device_ਸਾਰਣੀ).get_variation_ਸੂਚਕਾਂਕ () : HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
    ਜੇ (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
    {
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
      ਜੇ (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (ਝੂਠ);

      y_varidx = hb_ਪਹਿਲਾ (*new_varidx_delta);
      ਪੂਰਨ delta = hb_ਦੂਜਾ (*new_varidx_delta);
      ਜੇ (delta != 0)
      {
        ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
          return_trace (ਝੂਠ);
      }
    }


    ਤਾਰਕਿਕ no_downgrade = (!x_Device_ਸਾਰਣੀ.is_null () && !(ਇਹ+x_Device_ਸਾਰਣੀ).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ ||
                        (!y_Device_ਸਾਰਣੀ.is_null () && !(ਇਹ+y_Device_ਸਾਰਣੀ).is_variation_device ());

    ਜੇ (!no_downgrade)
      return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->format, 1, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));

    ਜੇ (!c->serializer->embed (x_Device_ਸਾਰਣੀ)) return_trace (ਝੂਠ);
    ਜੇ (!c->serializer->embed (y_Device_ਸਾਰਣੀ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->x_Device_ਸਾਰਣੀ.serialize_copy (c->serializer, x_Device_ਸਾਰਣੀ, ਇਹ, 0, hb_serialize_context_t::ਸਿਰ, &c->plan->layout_variation_idx_delta_map);
    ਬਾਹਰਲਾ->y_Device_ਸਾਰਣੀ.serialize_copy (c->serializer, y_Device_ਸਾਰਣੀ, ਇਹ, 0, hb_serialize_context_t::ਸਿਰ, &c->plan->layout_variation_idx_delta_map);
    return_trace (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    (ਇਹ+x_Device_ਸਾਰਣੀ).collect_variation_indices (c);
    (ਇਹ+y_Device_ਸਾਰਣੀ).collect_variation_indices (c);
  }
};


}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
