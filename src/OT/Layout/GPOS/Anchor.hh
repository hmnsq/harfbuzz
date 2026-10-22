#ifndef OT_LAYOUT_GPOS_ANCHOR_HH
#define OT_LAYOUT_GPOS_ANCHOR_HH

#ಅನ್ತರ್ಭಾವ "AnchorFormat1.hh"
#ಅನ್ತರ್ಭಾವ "AnchorFormat2.hh"
#ಅನ್ತರ್ಭಾವ "AnchorFormat3.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ Anchor
{
  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; } format;        /* Format identifier */
  AnchorFormat1         format1;
  AnchorFormat2         format2;
  AnchorFormat3         format3;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ಶೂನ್ಯ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ಭಿನ್ನ *x, ಭಿನ್ನ *y) ಧ್ರುವ
  {
    *x = *y = 0;
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); u.format1.get_anchor (c, glyph_id, x, y); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2: hb_barrier (); u.format2.get_anchor (c, glyph_id, x, y); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 3: hb_barrier (); u.format3.get_anchor (c, glyph_id, x, y); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:                                          ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (ತಾರ್ಕಿಕ (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
    ಪಕ್ಷ 2:
      hb_barrier ();
      ಯದಿ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      {
        // AnchorFormat 2 just containins extra hinting information, so
        // if hints are being dropped convert to format 1.
        return_trace (ತಾರ್ಕಿಕ (reinterpret_cast<Anchor *> (u.format1.copy (c->serializer))));
      }
      return_trace (ತಾರ್ಕಿಕ (reinterpret_cast<Anchor *> (u.format2.copy (c->serializer))));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.subset (c));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: ಪಕ್ಷ 2:
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHOR_HH
