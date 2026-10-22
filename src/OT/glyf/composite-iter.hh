#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#അന്തര്ഭാവ "../../hb.hh"


നാമാകാശ OT {
നാമാകാശ glyf_impl {


template <typename CompositeGlyphRecord>
ഘടന composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     സ്ഥിരം CompositeGlyphRecord &>
{
  തരനിർവചനം സ്ഥിരം CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, തർക്കം extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (അസത്യം) {}

  സ്ഥിരം CompositeGlyphRecord & __item__ () സ്ഥിരം { തിരികെ *current; }
  തർക്കം __more__ () സ്ഥിരം { തിരികെ current; }
  ശൂന്യം __next__ ()
  {
    എങ്കിൽ (!current->has_more ()) { current = nullptr; തിരികെ; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () സ്ഥിരം { തിരികെ composite_iter_tmpl (); }
  തർക്കം operator != (സ്ഥിരം composite_iter_tmpl& o) സ്ഥിരം
  { തിരികെ current != o.current; }


  ശൂന്യം set_current (__item_t__ current_)
  {
    എങ്കിൽ (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      തിരികെ;
    }
    unsigned size = current_->get_size (extended);
    എങ്കിൽ (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      തിരികെ;
    }

    current = current_;
    current_size = size;
  }

  സ്വകാര്യ:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  തർക്കം extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
