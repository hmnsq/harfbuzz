#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#ଅନ୍ତର୍ଭାଵ "AnchorFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "AnchorFormat2.hh"
#ଅନ୍ତର୍ଭାଵ "AnchorFormat3.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା Anchor
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ଶୂନ୍ୟ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ଦଶମିକ *x, ଦଶମିକ *y) ଅଚଳ
  {
    *x = *y = 0;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                                          ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (ତାର୍କିକ (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    ସ୍ଥିତି 2:
      hb_barrier ();
      ଯଦି (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (ତାର୍କିକ (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (ତାର୍କିକ (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.subset (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: ସ୍ଥିତି 2:
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
