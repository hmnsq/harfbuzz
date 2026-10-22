#अगर_अघोषित OT_GLYF_COMPOSITE_ITER_HH
#घोषणा OT_GLYF_COMPOSITE_ITER_HH


#समावेश "../../hb.hh"


namespace OT {
namespace glyf_impl {


ढाँचा <typename CompositeGlyphRecord>
काष्ठा composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     स्थिर CompositeGlyphRecord &>
{
  प्रकार स्थिर CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, bool extended_) :
      glyph (glyph_), current (nullptr), current_आकार (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_आकार (0), extended (false) {}

  स्थिर CompositeGlyphRecord & __item__ () स्थिर { वापस *current; }
  bool __more__ () स्थिर { वापस current; }
  व्योम __next__ ()
  {
    अगर (!current->has_more ()) { current = nullptr; वापस; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_आकार));
  }
  composite_iter_tmpl __end__ () स्थिर { वापस composite_iter_tmpl (); }
  bool चालक != (स्थिर composite_iter_tmpl& o) स्थिर
  { वापस current != o.current; }


  व्योम set_current (__item_t__ current_)
  {
    अगर (!glyph.check_range (current_, CompositeGlyphRecord::min_आकार))
    {
      current = nullptr;
      current_आकार = 0;
      वापस;
    }
    अचिन्हित आकार = current_->get_आकार (extended);
    अगर (!glyph.check_range (current_, आकार))
    {
      current = nullptr;
      current_आकार = 0;
      वापस;
    }

    current = current_;
    current_आकार = आकार;
  }

  निजी:
  hb_bytes_t glyph;
  __item_t__ current;
  अचिन्हित current_आकार;
  bool extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#पूर्ण_अगर /* OT_GLYF_COMPOSITE_ITER_HH */
