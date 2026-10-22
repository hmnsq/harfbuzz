#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_مرکب_ITER_HH
#وصف_ڪريو OT_GLYF_مرکب_ITER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


سانچو <قسم_نالو مرکب_Glyph_رڪارڊ>
بنية مرکب_iter_tmpl : hb_iter_with_fallback_t<مرکب_iter_tmpl<مرکب_Glyph_رڪارڊ>,
						     ثابت مرکب_Glyph_رڪارڊ &>
{
  تعريفنوع ثابت مرکب_Glyph_رڪارڊ *__item_t__;
  مرکب_iter_tmpl (hb_بائيٽون_t glyph_, __item_t__ current_, منطقي extended_) :
      glyph (glyph_), current (خالي_اشارو), current_سائيز (0), extended (extended_)
  {
    سيٽ_current (current_);
  }

  مرکب_iter_tmpl () : glyph (hb_بائيٽون_t ()), current (خالي_اشارو), current_سائيز (0), extended (خطأ) {}

  ثابت مرکب_Glyph_رڪارڊ & __item__ () ثابت { أرجع *current; }
  منطقي __more__ () ثابت { أرجع current; }
  فراغ __next__ ()
  {
    إذا (!current->has_وڌيڪ ()) { current = خالي_اشارو; أرجع; }

    سيٽ_current (&Struct_At_وٿي<مرکب_Glyph_رڪارڊ> (current, current_سائيز));
  }
  مرکب_iter_tmpl __end__ () ثابت { أرجع مرکب_iter_tmpl (); }
  منطقي عامل != (ثابت مرکب_iter_tmpl& o) ثابت
  { أرجع current != o.current; }


  فراغ سيٽ_current (__item_t__ current_)
  {
    إذا (!glyph.check_range (current_, مرکب_Glyph_رڪارڊ::min_سائيز))
    {
      current = خالي_اشارو;
      current_سائيز = 0;
      أرجع;
    }
    بنانشان سائيز = current_->get_سائيز (extended);
    إذا (!glyph.check_range (current_, سائيز))
    {
      current = خالي_اشارو;
      current_سائيز = 0;
      أرجع;
    }

    current = current_;
    current_سائيز = سائيز;
  }

  خاص:
  hb_بائيٽون_t glyph;
  __item_t__ current;
  بنانشان current_سائيز;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#جيڪڏهن_پورو /* OT_GLYF_COMPOSITE_ITER_HH */
