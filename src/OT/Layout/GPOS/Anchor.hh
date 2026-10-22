#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#അന്തര്ഭാവ "AnchorFormat1.hh"
#അന്തര്ഭാവ "AnchorFormat2.hh"
#അന്തര്ഭാവ "AnchorFormat3.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന Anchor
{
  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  ശൂന്യം get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ദശാംശം *x, ദശാംശം *y) സ്ഥിരം
  {
    *x = *y = 0;
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); തിരികെ;
    അവസ്ഥ 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); തിരികെ;
    അവസ്ഥ 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); തിരികെ;
    സ്ഥിരസ്ഥിതി:                                          തിരികെ;
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (തർക്കം (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    അവസ്ഥ 2:
      hb_barrier ();
      എങ്കിൽ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (തർക്കം (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (തർക്കം (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.subset (c));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: അവസ്ഥ 2:
      തിരികെ;
    അവസ്ഥ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      തിരികെ;
    സ്ഥിരസ്ഥിതി: തിരികെ;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
