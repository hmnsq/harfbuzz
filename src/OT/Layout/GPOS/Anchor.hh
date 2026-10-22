#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_ANCHOR_HH
#నిర్వచించు OT_LAYOUT_GPOS_ANCHOR_HH

#అన్తర్భావ "AnchorFormat1.hh"
#అన్తర్భావ "AnchorFormat2.hh"
#అన్తర్భావ "AnchorFormat3.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం Anchor
{
  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  శూన్య get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   దశాంశం *x, దశాంశం *y) స్థిరరాశి
  {
    *x = *y = 0;
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); తిరిగి;
    స్థితి 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); తిరిగి;
    స్థితి 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); తిరిగి;
    అప్రమేయ:                                          తిరిగి;
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (తార్కిక (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    స్థితి 2:
      hb_barrier ();
      ఒకవేళ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (తార్కిక (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (తార్కిక (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    స్థితి 3: hb_barrier (); return_trace (u.format3.subset (c));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: స్థితి 2:
      తిరిగి;
    స్థితి 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      తిరిగి;
    అప్రమేయ: తిరిగి;
    }
  }
};

}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_ANCHOR_HH
