#اگر_تعریف_نشده OT_GLYF_مرکب_ITER_HH
#تعریف_کن OT_GLYF_مرکب_ITER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


قالب <نام_نوع مرکب_Glyph_رکورد>
بنية مرکب_iter_tmpl : hb_iter_with_fallback_t<مرکب_iter_tmpl<مرکب_Glyph_رکورد>,
						     ثابت مرکب_Glyph_رکورد &>
{
  تعريفنوع ثابت مرکب_Glyph_رکورد *__item_t__;
  مرکب_iter_tmpl (hb_بایتها_t glyph_, __item_t__ current_, منطقي extended_) :
      glyph (glyph_), current (اشاره_تهی), current_اندازه (0), extended (extended_)
  {
    مجموعه_current (current_);
  }

  مرکب_iter_tmpl () : glyph (hb_بایتها_t ()), current (اشاره_تهی), current_اندازه (0), extended (خطأ) {}

  ثابت مرکب_Glyph_رکورد & __item__ () ثابت { أرجع *current; }
  منطقي __more__ () ثابت { أرجع current; }
  فراغ __next__ ()
  {
    إذا (!current->has_بیشتر ()) { current = اشاره_تهی; أرجع; }

    مجموعه_current (&Struct_At_جابه_جایی<مرکب_Glyph_رکورد> (current, current_اندازه));
  }
  مرکب_iter_tmpl __end__ () ثابت { أرجع مرکب_iter_tmpl (); }
  منطقي عملگر != (ثابت مرکب_iter_tmpl& o) ثابت
  { أرجع current != o.current; }


  فراغ مجموعه_current (__item_t__ current_)
  {
    إذا (!glyph.check_range (current_, مرکب_Glyph_رکورد::min_اندازه))
    {
      current = اشاره_تهی;
      current_اندازه = 0;
      أرجع;
    }
    بیعلامت اندازه = current_->get_اندازه (extended);
    إذا (!glyph.check_range (current_, اندازه))
    {
      current = اشاره_تهی;
      current_اندازه = 0;
      أرجع;
    }

    current = current_;
    current_اندازه = اندازه;
  }

  خاص:
  hb_بایتها_t glyph;
  __item_t__ current;
  بیعلامت current_اندازه;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#پایان_اگر /* OT_GLYF_COMPOSITE_ITER_HH */
