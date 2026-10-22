#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#ಅನ್ತರ್ಭಾವ "../../hb.hh"


ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ glyf_impl {


template <typename CompositeGlyphRecord>
ಸಂರಚನಾ composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ಧ್ರುವ CompositeGlyphRecord &>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಧ್ರುವ CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, ತಾರ್ಕಿಕ extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (ಅಸತ್ಯ) {}

  ಧ್ರುವ CompositeGlyphRecord & __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *current; }
  ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ current; }
  ಶೂನ್ಯ __next__ ()
  {
    ಯದಿ (!current->has_more ()) { current = nullptr; ಪ್ರತ್ಯಾಗಮ; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ composite_iter_tmpl (); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ composite_iter_tmpl& o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ current != o.current; }


  ಶೂನ್ಯ set_current (__item_t__ current_)
  {
    ಯದಿ (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      ಪ್ರತ್ಯಾಗಮ;
    }
    unsigned size = current_->get_size (extended);
    ಯದಿ (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      ಪ್ರತ್ಯಾಗಮ;
    }

    current = current_;
    current_size = size;
  }

  ಗುಪ್ತ:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  ತಾರ್ಕಿಕ extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
