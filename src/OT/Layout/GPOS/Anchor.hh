#अगर_अघोषित OT_LAYOUT_GPOS_ANCHOR_HH
#घोषणा OT_LAYOUT_GPOS_ANCHOR_HH

#समावेश "AnchorFormat1.hh"
#समावेश "AnchorFormat2.hh"
#समावेश "AnchorFormat3.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा Anchor
{
  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    शेष:return_trace (true);
    }
  }

  व्योम get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   भग्न *x, भग्न *y) स्थिर
  {
    *x = *y = 0;
    चयन (u.format.v) {
    हाल 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); वापस;
    हाल 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); वापस;
    हाल 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); वापस;
    शेष:                                          वापस;
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (bool (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    हाल 2:
      hb_barrier ();
      अगर (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (bool (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (bool (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    हाल 3: hb_barrier (); return_trace (u.format3.subset (c));
    शेष:return_trace (false);
    }
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: हाल 2:
      वापस;
    हाल 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      वापस;
    शेष: वापस;
    }
  }
};

}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_ANCHOR_HH
