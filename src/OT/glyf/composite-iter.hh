#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#અન્તર્ભાવ "../../hb.hh"


નામાકાશ OT {
નામાકાશ glyf_impl {


template <typename CompositeGlyphRecord>
સંરચના composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     અચલ CompositeGlyphRecord &>
{
  પ્રકારવ્યાખ્યા અચલ CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, તાર્કિક extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (ખોટું) {}

  અચલ CompositeGlyphRecord & __item__ () અચલ { પાછા *current; }
  તાર્કિક __more__ () અચલ { પાછા current; }
  શૂન્ય __next__ ()
  {
    જો (!current->has_more ()) { current = nullptr; પાછા; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () અચલ { પાછા composite_iter_tmpl (); }
  તાર્કિક operator != (અચલ composite_iter_tmpl& o) અચલ
  { પાછા current != o.current; }


  શૂન્ય set_current (__item_t__ current_)
  {
    જો (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      પાછા;
    }
    unsigned size = current_->get_size (extended);
    જો (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      પાછા;
    }

    current = current_;
    current_size = size;
  }

  ખાનગી:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  તાર્કિક extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
