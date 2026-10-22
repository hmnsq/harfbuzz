#که_نه_تعریف_شوی OT_GLYF_مرکب_ITER_HH
#تعریف_کړه OT_GLYF_مرکب_ITER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


کينډۍ <ډول_نوم مرکب_Glyph_ریکارډ>
بنية مرکب_iter_tmpl : hb_iter_with_fallback_t<مرکب_iter_tmpl<مرکب_Glyph_ریکارډ>,
						     ثابت مرکب_Glyph_ریکارډ &>
{
  تعريفنوع ثابت مرکب_Glyph_ریکارډ *__item_t__;
  مرکب_iter_tmpl (hb_بایټونه_t glyph_, __item_t__ current_, منطقي extended_) :
      glyph (glyph_), current (تش_اشاره), current_اندازه (0), extended (extended_)
  {
    سېټ_current (current_);
  }

  مرکب_iter_tmpl () : glyph (hb_بایټونه_t ()), current (تش_اشاره), current_اندازه (0), extended (خطأ) {}

  ثابت مرکب_Glyph_ریکارډ & __item__ () ثابت { أرجع *current; }
  منطقي __more__ () ثابت { أرجع current; }
  فراغ __next__ ()
  {
    إذا (!current->has_نور ()) { current = تش_اشاره; أرجع; }

    سېټ_current (&Struct_At_دننه_واټن<مرکب_Glyph_ریکارډ> (current, current_اندازه));
  }
  مرکب_iter_tmpl __end__ () ثابت { أرجع مرکب_iter_tmpl (); }
  منطقي چلوونکی != (ثابت مرکب_iter_tmpl& o) ثابت
  { أرجع current != o.current; }


  فراغ سېټ_current (__item_t__ current_)
  {
    إذا (!glyph.check_range (current_, مرکب_Glyph_ریکارډ::min_اندازه))
    {
      current = تش_اشاره;
      current_اندازه = 0;
      أرجع;
    }
    بینښه اندازه = current_->get_اندازه (extended);
    إذا (!glyph.check_range (current_, اندازه))
    {
      current = تش_اشاره;
      current_اندازه = 0;
      أرجع;
    }

    current = current_;
    current_اندازه = اندازه;
  }

  خاص:
  hb_بایټونه_t glyph;
  __item_t__ current;
  بینښه current_اندازه;
  منطقي extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#د_که_پای /* OT_GLYF_COMPOSITE_ITER_HH */
