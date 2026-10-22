#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#अन्तर्भाव "AnchorFormat1.hh"
#अन्तर्भाव "AnchorFormat2.hh"
#अन्तर्भाव "AnchorFormat3.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना Anchor
{
  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.format.v.sanitize (c)) return_trace (मिच्छा);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    स्वभाव:return_trace (सच्च);
    }
  }

  शून्य get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   भिन्न *x, भिन्न *y) ध्रुव
  {
    *x = *y = 0;
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); पटिनिवत्त;
    पक्ष 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); पटिनिवत्त;
    पक्ष 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); पटिनिवत्त;
    स्वभाव:                                          पटिनिवत्त;
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (तार्किक (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    पक्ष 2:
      hb_barrier ();
      सचे (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (तार्किक (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (तार्किक (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    पक्ष 3: hb_barrier (); return_trace (u.format3.subset (c));
    स्वभाव:return_trace (मिच्छा);
    }
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: पक्ष 2:
      पटिनिवत्त;
    पक्ष 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      पटिनिवत्त;
    स्वभाव: पटिनिवत्त;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
