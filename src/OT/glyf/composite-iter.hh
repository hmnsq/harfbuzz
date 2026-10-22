#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#ଅନ୍ତର୍ଭାଵ "../../hb.hh"


ନାମାକାଶ OT {
ନାମାକାଶ glyf_impl {


template <typename CompositeGlyphRecord>
ସଂରଚନା composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ଅଚଳ CompositeGlyphRecord &>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଚଳ CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, ତାର୍କିକ extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (ମିଥ୍ୟା) {}

  ଅଚଳ CompositeGlyphRecord & __item__ () ଅଚଳ { ଫେରନ୍ତୁ *current; }
  ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ current; }
  ଶୂନ୍ୟ __next__ ()
  {
    ଯଦି (!current->has_more ()) { current = nullptr; ଫେରନ୍ତୁ; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () ଅଚଳ { ଫେରନ୍ତୁ composite_iter_tmpl (); }
  ତାର୍କିକ operator != (ଅଚଳ composite_iter_tmpl& o) ଅଚଳ
  { ଫେରନ୍ତୁ current != o.current; }


  ଶୂନ୍ୟ set_current (__item_t__ current_)
  {
    ଯଦି (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      ଫେରନ୍ତୁ;
    }
    unsigned size = current_->get_size (extended);
    ଯଦି (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      ଫେରନ୍ତୁ;
    }

    current = current_;
    current_size = size;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  ତାର୍କିକ extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
