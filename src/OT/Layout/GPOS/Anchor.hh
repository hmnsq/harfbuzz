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
  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  शून्य get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   दशांश *x, दशांश *y) स्थिरांक
  {
    *x = *y = 0;
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); परत;
    स्थिती 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); परत;
    स्थिती 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); परत;
    पूर्वनिर्धारित:                                          परत;
    }
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (तार्किक (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    स्थिती 2:
      hb_barrier ();
      जर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (तार्किक (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (तार्किक (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    स्थिती 3: hb_barrier (); return_trace (u.format3.subset (c));
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: स्थिती 2:
      परत;
    स्थिती 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      परत;
    पूर्वनिर्धारित: परत;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
