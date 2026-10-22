#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#अन्तर्भाव "../../hb.hh"


नामाकाश OT {
नामाकाश glyf_impl {


ढाँचा <typename CompositeGlyphRecord>
संरचना composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     अचर CompositeGlyphRecord &>
{
  प्रकारनिर्देश अचर CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, तार्किक extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (असत्य) {}

  अचर CompositeGlyphRecord & __item__ () अचर { फर्काऊ *current; }
  तार्किक __more__ () अचर { फर्काऊ current; }
  शून्य __next__ ()
  {
    यदि (!current->has_more ()) { current = nullptr; फर्काऊ; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () अचर { फर्काऊ composite_iter_tmpl (); }
  तार्किक चालक != (अचर composite_iter_tmpl& o) अचर
  { फर्काऊ current != o.current; }


  शून्य set_current (__item_t__ current_)
  {
    यदि (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      फर्काऊ;
    }
    अचिन्हित size = current_->get_size (extended);
    यदि (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      फर्काऊ;
    }

    current = current_;
    current_size = size;
  }

  गुप्त:
  hb_bytes_t glyph;
  __item_t__ current;
  अचिन्हित current_size;
  तार्किक extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
