#که_نه_تعریف_شوی OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
#تعریف_کړه OT_LAYOUT_GPOS_ANCHORFORMAT3_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية AnchorFormat3
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 3 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  Offset16_منزل<Device>
                x_Device_جدول;           /* Offset to Device table for X
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  Offset16_منزل<Device>
                y_Device_جدول;           /* Offset to Device table for Y
                                         * coordinate-- from beginning of
                                         * Anchor table (may be NULL) */
  عام:
  DEFINE_اندازه_STATIC (10);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);

    return_trace (x_Device_جدول.sanitize (c, هذا) && y_Device_جدول.sanitize (c, هذا));
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_ناکارول_شوی,
                   عائم *x, عائم *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);

    إذا ((font->x_ppem || font->has_nonzero_coords) && x_Device_جدول.sanitize (&c->sanitizer, هذا))
    {
      hb_barrier ();
      *x += (هذا+x_Device_جدول).get_x_delta_f (font, c->var_store, c->var_store_cache);
    }
    إذا ((font->y_ppem || font->has_nonzero_coords) && y_Device_جدول.sanitize (&c->sanitizer, هذا))
    {
      hb_barrier ();
      *y += (هذا+y_Device_جدول).get_y_delta_f (font, c->var_store, c->var_store_cache);
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->embed (format))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (xCoordinate))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (yCoordinate))) return_trace (خطأ);

    بینښه x_varidx = x_Device_جدول ? (هذا+x_Device_جدول).get_variation_شاخص () : HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    إذا (x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
    {
      hb_pair_t<بینښه, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (x_varidx, &new_varidx_delta))
        return_trace (خطأ);

      x_varidx = hb_لومړی (*new_varidx_delta);
      صحيح delta = hb_دوهم (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_ټاکل (بهر->xCoordinate, xCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_ډکېدنه))
          return_trace (خطأ);
      }
    }

    بینښه y_varidx = y_Device_جدول ? (هذا+y_Device_جدول).get_variation_شاخص () : HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    إذا (y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
    {
      hb_pair_t<بینښه, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (y_varidx, &new_varidx_delta))
        return_trace (خطأ);

      y_varidx = hb_لومړی (*new_varidx_delta);
      صحيح delta = hb_دوهم (*new_varidx_delta);
      إذا (delta != 0)
      {
        إذا (!c->serializer->check_ټاکل (بهر->yCoordinate, yCoordinate + delta,
                                          HB_SERIALIZE_ERROR_INT_ډکېدنه))
          return_trace (خطأ);
      }
    }


    منطقي no_downgrade = (!x_Device_جدول.is_null () && !(هذا+x_Device_جدول).is_variation_device ()) ||
                        x_varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص ||
                        y_varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص ||
                        (!y_Device_جدول.is_null () && !(هذا+y_Device_جدول).is_variation_device ());

    إذا (!no_downgrade)
      return_trace (c->serializer->check_ټاکل (بهر->format, 1, HB_SERIALIZE_ERROR_INT_ډکېدنه));

    إذا (!c->serializer->embed (x_Device_جدول)) return_trace (خطأ);
    إذا (!c->serializer->embed (y_Device_جدول)) return_trace (خطأ);

    بهر->x_Device_جدول.serialize_copy (c->serializer, x_Device_جدول, هذا, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    بهر->y_Device_جدول.serialize_copy (c->serializer, y_Device_جدول, هذا, 0, hb_serialize_context_t::سر, &c->plan->layout_variation_idx_delta_map);
    return_trace (بهر);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+x_Device_جدول).collect_variation_indices (c);
    (هذا+y_Device_جدول).collect_variation_indices (c);
  }
};


}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_ANCHORFORMAT3_HH
