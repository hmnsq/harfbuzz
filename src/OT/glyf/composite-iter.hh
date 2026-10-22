#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#অন্তর্ভাৱ "../../hb.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


template <typename CompositeGlyphRecord>
সংরচনা composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ধ্রুৱ CompositeGlyphRecord &>
{
  প্রকারনির্দেশ ধ্রুৱ CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, তার্কিক extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (মিছা) {}

  ধ্রুৱ CompositeGlyphRecord & __item__ () ধ্রুৱ { ঘূৰাই *current; }
  তার্কিক __more__ () ধ্রুৱ { ঘূৰাই current; }
  শূন্য __next__ ()
  {
    যদি (!current->has_more ()) { current = nullptr; ঘূৰাই; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () ধ্রুৱ { ঘূৰাই composite_iter_tmpl (); }
  তার্কিক operator != (ধ্রুৱ composite_iter_tmpl& o) ধ্রুৱ
  { ঘূৰাই current != o.current; }


  শূন্য set_current (__item_t__ current_)
  {
    যদি (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      ঘূৰাই;
    }
    unsigned size = current_->get_size (extended);
    যদি (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      ঘূৰাই;
    }

    current = current_;
    current_size = size;
  }

  গুপ্ত:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  তার্কিক extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
