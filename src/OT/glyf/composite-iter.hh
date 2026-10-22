#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#अन्तर्भाव "../../hb.hh"


नामाकाश OT {
नामाकाश glyf_impl {


ढाँचा <typename CompositeGlyphRecord>
संरचना composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     स्थिरांक CompositeGlyphRecord &>
{
  प्रकारव्याख्या स्थिरांक CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, तार्किक extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (असत्य) {}

  स्थिरांक CompositeGlyphRecord & __item__ () स्थिरांक { परत *current; }
  तार्किक __more__ () स्थिरांक { परत current; }
  शून्य __next__ ()
  {
    जर (!current->has_more ()) { current = nullptr; परत; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () स्थिरांक { परत composite_iter_tmpl (); }
  तार्किक चालक != (स्थिरांक composite_iter_tmpl& o) स्थिरांक
  { परत current != o.current; }


  शून्य set_current (__item_t__ current_)
  {
    जर (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      परत;
    }
    अचिन्हित size = current_->get_size (extended);
    जर (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      परत;
    }

    current = current_;
    current_size = size;
  }

  खाजगी:
  hb_bytes_t glyph;
  __item_t__ current;
  अचिन्हित current_size;
  तार्किक extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
