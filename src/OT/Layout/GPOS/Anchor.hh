#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#ضمن "AnchorFormat1.hh"
#ضمن "AnchorFormat2.hh"
#ضمن "AnchorFormat3.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية Anchor
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    افتراضي:return_trace (true);
    }
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   عائم *x, عائم *y) ثابت
  {
    *x = *y = 0;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); أرجع;
    حالة 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); أرجع;
    حالة 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); أرجع;
    افتراضي:                                          أرجع;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (منطقي (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    حالة 2:
      hb_barrier ();
      إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (منطقي (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (منطقي (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    حالة 3: hb_barrier (); return_trace (u.format3.subset (c));
    افتراضي:return_trace (خطأ);
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.format.v) {
    حالة 1: حالة 2:
      أرجع;
    حالة 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      أرجع;
    افتراضي: أرجع;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
