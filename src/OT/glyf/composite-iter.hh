#جے_بنا_تعریف OT_GLYF_مرکب_ITER_HH
#تعریف_کرو OT_GLYF_مرکب_ITER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


سانچا <قسم_ناں مرکب_Glyph_ریکارڈ>
بنية مرکب_iter_tmpl : hb_iter_with_fallback_t<مرکب_iter_tmpl<مرکب_Glyph_ریکارڈ>,
						     ثابت مرکب_Glyph_ریکارڈ &>
{
  تعريفنوع ثابت مرکب_Glyph_ریکارڈ *__item_t__;
  مرکب_iter_tmpl (hb_بائٹاں_t glyph_, __item_t__ current_, منطقي extended_) :
      glyph (glyph_), current (خالی_سوچک), current_ناپ (0), extended (extended_)
  {
    سیٹ_current (current_);
  }

  مرکب_iter_tmpl () : glyph (hb_بائٹاں_t ()), current (خالی_سوچک), current_ناپ (0), extended (خطأ) {}

  ثابت مرکب_Glyph_ریکارڈ & __item__ () ثابت { أرجع *current; }
  منطقي __more__ () ثابت { أرجع current; }
  فراغ __next__ ()
  {
    إذا (!current->has_ہور ()) { current = خالی_سوچک; أرجع; }

    سیٹ_current (&Struct_At_وتھ<مرکب_Glyph_ریکارڈ> (current, current_ناپ));
  }
  مرکب_iter_tmpl __end__ () ثابت { أرجع مرکب_iter_tmpl (); }
  منطقي عامل != (ثابت مرکب_iter_tmpl& o) ثابت
  { أرجع current != o.current; }


  فراغ سیٹ_current (__item_t__ current_)
  {
    إذا (!glyph.check_range (current_, مرکب_Glyph_ریکارڈ::min_ناپ))
    {
      current = خالی_سوچک;
      current_ناپ = 0;
      أرجع;
    }
    بنانشانی ناپ = current_->get_ناپ (extended);
    إذا (!glyph.check_range (current_, ناپ))
    {
      current = خالی_سوچک;
      current_ناپ = 0;
      أرجع;
    }

    current = current_;
    current_ناپ = ناپ;
  }

  خاص:
  hb_بائٹاں_t glyph;
  __item_t__ current;
  بنانشانی current_ناپ;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#جے_انت /* OT_GLYF_COMPOSITE_ITER_HH */
