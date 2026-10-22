#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#அந்தர்பாவ "../../hb.hh"


நாமாகாஶ OT {
நாமாகாஶ glyf_impl {


template <typename CompositeGlyphRecord>
கட்டமைப்பு composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     மாறிலி CompositeGlyphRecord &>
{
  வகைவரை மாறிலி CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, தருக்க extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (பொய்) {}

  மாறிலி CompositeGlyphRecord & __item__ () மாறிலி { திரும்பு *current; }
  தருக்க __more__ () மாறிலி { திரும்பு current; }
  வெறுமை __next__ ()
  {
    எனில் (!current->has_more ()) { current = nullptr; திரும்பு; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () மாறிலி { திரும்பு composite_iter_tmpl (); }
  தருக்க operator != (மாறிலி composite_iter_tmpl& o) மாறிலி
  { திரும்பு current != o.current; }


  வெறுமை set_current (__item_t__ current_)
  {
    எனில் (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      திரும்பு;
    }
    unsigned size = current_->get_size (extended);
    எனில் (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      திரும்பு;
    }

    current = current_;
    current_size = size;
  }

  தனிப்பட்ட:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  தருக்க extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
