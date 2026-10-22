#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#include "AnchorFormat1.hh"
#include "AnchorFormat2.hh"
#include "AnchorFormat3.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

sa_nracanaa Anchor
{
  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ᬬᬾᬦ᭄ (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    pak_sa 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    pak_sa 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    default:return_trace (true);
    }
  }

  void get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   float *x, float *y) const
  {
    *x = *y = 0;
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); ᬯᬮᬶ;
    pak_sa 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); ᬯᬮᬶ;
    pak_sa 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); ᬯᬮᬶ;
    default:                                          ᬯᬮᬶ;
    }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (bool (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    pak_sa 2:
      hb_barrier ();
      ᬬᬾᬦ᭄ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (bool (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (bool (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    pak_sa 3: hb_barrier (); return_trace (u.format3.subset (c));
    default:return_trace (false);
    }
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    switch (u.format.v) {
    pak_sa 1: pak_sa 2:
      ᬯᬮᬶ;
    pak_sa 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ᬯᬮᬶ;
    default: ᬯᬮᬶ;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
