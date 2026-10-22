#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#अन्तर्भाव "../../hb.hh"


नामाकाश OT {
नामाकाश glyf_impl {


template <typename CompositeGlyphRecord>
संरचना composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ध्रुव CompositeGlyphRecord &>
{
  प्रकारनिर्देश ध्रुव CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, तार्किक extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (मिच्छ) {}

  ध्रुव CompositeGlyphRecord & __item__ () ध्रुव { प्रत्यागम *current; }
  तार्किक __more__ () ध्रुव { प्रत्यागम current; }
  शून्य __next__ ()
  {
    जइ (!current->has_more ()) { current = nullptr; प्रत्यागम; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () ध्रुव { प्रत्यागम composite_iter_tmpl (); }
  तार्किक operator != (ध्रुव composite_iter_tmpl& o) ध्रुव
  { प्रत्यागम current != o.current; }


  शून्य set_current (__item_t__ current_)
  {
    जइ (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      प्रत्यागम;
    }
    unsigned size = current_->get_size (extended);
    जइ (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      प्रत्यागम;
    }

    current = current_;
    current_size = size;
  }

  गुप्त:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  तार्किक extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
