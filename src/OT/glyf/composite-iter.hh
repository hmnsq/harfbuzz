#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#অন্তর্ভাব "../../hb.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


template <typename CompositeGlyphRecord>
কাঠামো composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ধ্রুবক CompositeGlyphRecord &>
{
  প্রকারলেখ ধ্রুবক CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, যৌক্তিক extended_) :
      glyph (glyph_), current (nullptr), current_আকার (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_আকার (0), extended (মিথ্যা) {}

  ধ্রুবক CompositeGlyphRecord & __item__ () ধ্রুবক { ফেরত *current; }
  যৌক্তিক __more__ () ধ্রুবক { ফেরত current; }
  শূন্য __next__ ()
  {
    যদি (!current->has_more ()) { current = nullptr; ফেরত; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_আকার));
  }
  composite_iter_tmpl __end__ () ধ্রুবক { ফেরত composite_iter_tmpl (); }
  যৌক্তিক operator != (ধ্রুবক composite_iter_tmpl& o) ধ্রুবক
  { ফেরত current != o.current; }


  শূন্য set_current (__item_t__ current_)
  {
    যদি (!glyph.check_range (current_, CompositeGlyphRecord::min_আকার))
    {
      current = nullptr;
      current_আকার = 0;
      ফেরত;
    }
    unsigned আকার_2 = current_->get_আকার (extended);
    যদি (!glyph.check_range (current_, আকার_2))
    {
      current = nullptr;
      current_আকার = 0;
      ফেরত;
    }

    current = current_;
    current_আকার = আকার_2;
  }

  ব্যক্তিগত:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_আকার;
  যৌক্তিক extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
