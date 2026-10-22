#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#અન્તર્ભાવ "AnchorFormat1.hh"
#અન્તર્ભાવ "AnchorFormat2.hh"
#અન્તર્ભાવ "AnchorFormat3.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સંરચના Anchor
{
  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  શૂન્ય get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   દશાંશ *x, દશાંશ *y) અચલ
  {
    *x = *y = 0;
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); પાછા;
    સ્થિતિ 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); પાછા;
    સ્થિતિ 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); પાછા;
    મૂળભૂત:                                          પાછા;
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (તાર્કિક (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    સ્થિતિ 2:
      hb_barrier ();
      જો (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (તાર્કિક (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (તાર્કિક (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.subset (c));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: સ્થિતિ 2:
      પાછા;
    સ્થિતિ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      પાછા;
    મૂળભૂત: પાછા;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
