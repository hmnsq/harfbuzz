#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت AnchorFormat3
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 3 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  Offset16_مقصد<Device>
                x_Device_جدول;           /* Offset to Device table for X
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  Offset16_مقصد<Device>
                y_Device_جدول;           /* Offset to Device table for Y
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  عوامی:
  DEFINE_سائز_STATIC (10);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ))) return_trace (جھوٹ);

    return_trace (x_Device_جدول.sanitize (c, یہ) && y_Device_جدول.sanitize (c, یہ));
  }

  خالی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_غیر_مستعمل,
                   اعشاری *x, اعشاری *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    اگر ((font->x_ppem || font->has_nonzero_coords) && x_Device_جدول.sanitize (&c->sanitizer, یہ))
    {
      hb_barrier ();
      *x += (یہ+x_Device_جدول).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    اگر ((font->y_ppem || font->has_nonzero_coords) && y_Device_جدول.sanitize (&c->sanitizer, یہ))
    {
      hb_barrier ();
      *y += (یہ+y_Device_جدول).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->embed (format))) return_trace (جھوٹ);
    اگر (unlikely (!c->serializer->embed (xCoordinate))) return_trace (جھوٹ);
    اگر (unlikely (!c->serializer->embed (yCoordinate))) return_trace (جھوٹ);

    غیرعلامتی x_varidx = x_Device_جدول ? (یہ+x_Device_جدول).get_variation_اشاریہ () : HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    اگر (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
    {
      hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
      اگر (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (جھوٹ);

      x_varidx = hb_پہلا (*new_varidx_delta);
      صحیح delta = hb_دوسرا (*new_varidx_delta);
      اگر (delta != 0)
      {
        اگر (!c->serializer->check_تفویض (برآمد->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
          return_trace (جھوٹ);
      }
    }

    غیرعلامتی y_varidx = y_Device_جدول ? (یہ+y_Device_جدول).get_variation_اشاریہ () : HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    اگر (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
    {
      hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
      اگر (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (جھوٹ);

      y_varidx = hb_پہلا (*new_varidx_delta);
      صحیح delta = hb_دوسرا (*new_varidx_delta);
      اگر (delta != 0)
      {
        اگر (!c->serializer->check_تفویض (برآمد->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
          return_trace (جھوٹ);
      }
    }


    منطقی no_downgrade = (!x_Device_جدول.is_null () && !(یہ+x_Device_جدول).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ ||
                        (!y_Device_جدول.is_null () && !(یہ+y_Device_جدول).is_variation_device ());

    اگر (!no_downgrade)
      return_trace (c->serializer->check_تفویض (برآمد->format, 1, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));

    اگر (!c->serializer->embed (x_Device_جدول)) return_trace (جھوٹ);
    اگر (!c->serializer->embed (y_Device_جدول)) return_trace (جھوٹ);

    برآمد->x_Device_جدول.serialize_copy (c->serializer, x_Device_جدول, یہ, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    برآمد->y_Device_جدول.serialize_copy (c->serializer, y_Device_جدول, یہ, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    return_trace (برآمد);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (یہ+x_Device_جدول).collect_variation_indices (c);
    (یہ+y_Device_جدول).collect_variation_indices (c);
  }
};


}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
