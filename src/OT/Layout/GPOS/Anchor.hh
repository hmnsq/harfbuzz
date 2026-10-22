#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#অন্তর্ভাৱ "AnchorFormat1.hh"
#অন্তর্ভাৱ "AnchorFormat2.hh"
#অন্তর্ভাৱ "AnchorFormat3.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

সংরচনা Anchor
{
  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ভিন্ন *x, ভিন্ন *y) ধ্রুৱ
  {
    *x = *y = 0;
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); ঘূৰাই;
    পক্ষ 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); ঘূৰাই;
    পক্ষ 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); ঘূৰাই;
    স্ৱভাৱ:                                          ঘূৰাই;
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (তার্কিক (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    পক্ষ 2:
      hb_barrier ();
      যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (তার্কিক (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (তার্কিক (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.subset (c));
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: পক্ষ 2:
      ঘূৰাই;
    পক্ষ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ঘূৰাই;
    স্ৱভাৱ: ঘূৰাই;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
