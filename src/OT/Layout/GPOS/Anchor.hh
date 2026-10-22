#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_ANCHOR_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHOR_HH

#شامل "AnchorFormat1.hh"
#شامل "AnchorFormat2.hh"
#شامل "AnchorFormat3.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت Anchor
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }

  خالی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   اعشاری *x, اعشاری *y) ثابت
  {
    *x = *y = 0;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); واپس;
    حالت 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); واپس;
    حالت 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); واپس;
    طےشدہ:                                          واپس;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
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
    طےشدہ:return_trace (جھوٹ);
    }
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: حالت 2:
      واپس;
    حالت 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      واپس;
    طےشدہ: واپس;
    }
  }
};

}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_ANCHOR_HH
