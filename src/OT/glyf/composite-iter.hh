#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


template <typename CompositeGlyphRecord>
بنية composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     ثابت CompositeGlyphRecord &>
{
  تعريفنوع ثابت CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, منطقي extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (خطأ) {}

  ثابت CompositeGlyphRecord & __item__ () ثابت { أرجع *current; }
  منطقي __more__ () ثابت { أرجع current; }
  فراغ __next__ ()
  {
    إذا (!current->has_more ()) { current = nullptr; أرجع; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () ثابت { أرجع composite_iter_tmpl (); }
  منطقي operator != (ثابت composite_iter_tmpl& o) ثابت
  { أرجع current != o.current; }


  فراغ set_current (__item_t__ current_)
  {
    إذا (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      أرجع;
    }
    unsigned size = current_->get_size (extended);
    إذا (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      أرجع;
    }

    current = current_;
    current_size = size;
  }

  خاص:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
