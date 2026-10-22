#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#অন্তর্ভাব "AnchorFormat1.hh"
#অন্তর্ভাব "AnchorFormat2.hh"
#অন্তর্ভাব "AnchorFormat3.hh"

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
  সার্বজনিক:
  DEFINE_SIZE_UNION (2, format.v);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    স্বভাব:return_trace (সত্য);
    }
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ভিন্ন *x, ভিন্ন *y) ধ্রুব
  {
    *x = *y = 0;
    বিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); প্রত্যাগম;
    পক্ষ 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); প্রত্যাগম;
    পক্ষ 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); প্রত্যাগম;
    স্বভাব:                                          প্রত্যাগম;
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    বিকল্প (u.format.v) {
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
    স্বভাব:return_trace (অসত্য);
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    বিকল্প (u.format.v) {
    পক্ষ 1: পক্ষ 2:
      প্রত্যাগম;
    পক্ষ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      প্রত্যাগম;
    স্বভাব: প্রত্যাগম;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
