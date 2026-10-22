#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_ANCHOR_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_ANCHOR_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "AnchorFormat1.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "AnchorFormat2.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "AnchorFormat3.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ Anchor
{
  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਖਾਲੀ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *y) ਅਚਲ
  {
    *x = *y = 0;
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); ਵਾਪਸ;
    ਹਾਲਤ 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); ਵਾਪਸ;
    ਹਾਲਤ 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); ਵਾਪਸ;
    ਮੂਲ:                                          ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (ਤਾਰਕਿਕ (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    ਹਾਲਤ 2:
      hb_barrier ();
      ਜੇ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (ਤਾਰਕਿਕ (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (ਤਾਰਕਿਕ (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.subset (c));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: ਹਾਲਤ 2:
      ਵਾਪਸ;
    ਹਾਲਤ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ਵਾਪਸ;
    ਮੂਲ: ਵਾਪਸ;
    }
  }
};

}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_ANCHOR_HH
