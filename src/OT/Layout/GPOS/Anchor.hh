#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#অন্তর্ভাব "AnchorFormat1.hh"
#অন্তর্ভাব "AnchorFormat2.hh"
#অন্তর্ভাব "AnchorFormat3.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো Anchor
{
  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   দশমিক *x, দশমিক *y) ধ্রুবক
  {
    *x = *y = 0;
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); ফেরত;
    ক্ষেত্র 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); ফেরত;
    ক্ষেত্র 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); ফেরত;
    পূর্বনির্ধারিত:                                          ফেরত;
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (যৌক্তিক (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    ক্ষেত্র 2:
      hb_barrier ();
      যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (যৌক্তিক (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (যৌক্তিক (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.subset (c));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: ক্ষেত্র 2:
      ফেরত;
    ক্ষেত্র 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ফেরত;
    পূর্বনির্ধারিত: ফেরত;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
