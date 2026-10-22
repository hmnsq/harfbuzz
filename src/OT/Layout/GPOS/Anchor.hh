#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#شامل "AnchorFormat1.hh"
#شامل "AnchorFormat2.hh"
#شامل "AnchorFormat3.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار Anchor
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }

  تهی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   اعشاری *x, اعشاری *y) ثابت
  {
    *x = *y = 0;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); بازگشت;
    حالت 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); بازگشت;
    حالت 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); بازگشت;
    پیشفرض:                                          بازگشت;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (منطقی (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    حالت 2:
      hb_barrier ();
      اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (منطقی (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (منطقی (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    حالت 3: hb_barrier (); return_trace (u.format3.subset (c));
    پیشفرض:return_trace (نادرست);
    }
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: حالت 2:
      بازگشت;
    حالت 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      بازگشت;
    پیشفرض: بازگشت;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
