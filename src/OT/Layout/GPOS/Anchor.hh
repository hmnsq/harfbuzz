#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#அந்தர்பாவ "AnchorFormat1.hh"
#அந்தர்பாவ "AnchorFormat2.hh"
#அந்தர்பாவ "AnchorFormat3.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

கட்டமைப்பு Anchor
{
  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }

  வெறுமை get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   தசம *x, தசம *y) மாறிலி
  {
    *x = *y = 0;
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); திரும்பு;
    நிலை 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); திரும்பு;
    நிலை 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); திரும்பு;
    இயல்பு:                                          திரும்பு;
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (தருக்க (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    நிலை 2:
      hb_barrier ();
      எனில் (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (தருக்க (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (தருக்க (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    நிலை 3: hb_barrier (); return_trace (u.format3.subset (c));
    இயல்பு:return_trace (பொய்);
    }
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: நிலை 2:
      திரும்பு;
    நிலை 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      திரும்பு;
    இயல்பு: திரும்பு;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
