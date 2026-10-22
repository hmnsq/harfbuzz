#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_ਸੰਯੁਕਤ_ITER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_ਸੰਯੁਕਤ_ITER_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ glyf_impl {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ>
ਬਣਤਰ ਸੰਯੁਕਤ_iter_tmpl : hb_iter_with_fallback_t<ਸੰਯੁਕਤ_iter_tmpl<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ>,
						     ਅਚਲ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ &>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ *__item_t__;
  ਸੰਯੁਕਤ_iter_tmpl (hb_ਬਾਈਟਾਂ_t glyph_, __item_t__ current_, ਤਾਰਕਿਕ extended_) :
      glyph (glyph_), current (ਖਾਲੀ_ਸੂਚਕ), current_ਮਾਪ (0), extended (extended_)
  {
    ਸੈੱਟ_current (current_);
  }

  ਸੰਯੁਕਤ_iter_tmpl () : glyph (hb_ਬਾਈਟਾਂ_t ()), current (ਖਾਲੀ_ਸੂਚਕ), current_ਮਾਪ (0), extended (ਝੂਠ) {}

  ਅਚਲ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ & __item__ () ਅਚਲ { ਵਾਪਸ *current; }
  ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ current; }
  ਖਾਲੀ __next__ ()
  {
    ਜੇ (!current->has_ਹੋਰ ()) { current = ਖਾਲੀ_ਸੂਚਕ; ਵਾਪਸ; }

    ਸੈੱਟ_current (&Struct_At_ਵਿੱਥ<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ> (current, current_ਮਾਪ));
  }
  ਸੰਯੁਕਤ_iter_tmpl __end__ () ਅਚਲ { ਵਾਪਸ ਸੰਯੁਕਤ_iter_tmpl (); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ ਸੰਯੁਕਤ_iter_tmpl& o) ਅਚਲ
  { ਵਾਪਸ current != o.current; }


  ਖਾਲੀ ਸੈੱਟ_current (__item_t__ current_)
  {
    ਜੇ (!glyph.check_range (current_, ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ::min_ਮਾਪ))
    {
      current = ਖਾਲੀ_ਸੂਚਕ;
      current_ਮਾਪ = 0;
      ਵਾਪਸ;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = current_->get_ਮਾਪ (extended);
    ਜੇ (!glyph.check_range (current_, ਮਾਪ))
    {
      current = ਖਾਲੀ_ਸੂਚਕ;
      current_ਮਾਪ = 0;
      ਵਾਪਸ;
    }

    current = current_;
    current_ਮਾਪ = ਮਾਪ;
  }

  ਨਿਜੀ:
  hb_ਬਾਈਟਾਂ_t glyph;
  __item_t__ current;
  ਬਿਨਾਨਿਸ਼ਾਨੀ current_ਮਾਪ;
  ਤਾਰਕਿਕ extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#ਜੇ_ਅੰਤ /* OT_GLYF_COMPOSITE_ITER_HH */
