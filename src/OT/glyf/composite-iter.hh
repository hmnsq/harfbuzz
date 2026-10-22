#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#شامل "../../hb.hh"


فضاینام OT {
فضاینام glyf_impl {


template <typename CompositeGlyphRecord>
ساختار composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ثابت CompositeGlyphRecord &>
{
  تعریفنوع ثابت CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, منطقی extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (نادرست) {}

  ثابت CompositeGlyphRecord & __item__ () ثابت { بازگشت *current; }
  منطقی __more__ () ثابت { بازگشت current; }
  تهی __next__ ()
  {
    اگر (!current->has_more ()) { current = nullptr; بازگشت; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () ثابت { بازگشت composite_iter_tmpl (); }
  منطقی operator != (ثابت composite_iter_tmpl& o) ثابت
  { بازگشت current != o.current; }


  تهی set_current (__item_t__ current_)
  {
    اگر (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      بازگشت;
    }
    unsigned size = current_->get_size (extended);
    اگر (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      بازگشت;
    }

    current = current_;
    current_size = size;
  }

  خصوصی:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  منطقی extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
