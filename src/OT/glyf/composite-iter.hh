#اگر_غیر_تعریف_شدہ OT_GLYF_مرکب_ITER_HH
#تعریف_کرو OT_GLYF_مرکب_ITER_HH


#شامل "../../hb.hh"


نامخلا OT {
نامخلا glyf_impl {


سانچہ <قسم_نام مرکب_Glyph_ریکارڈ>
ساخت مرکب_iter_tmpl : hb_iter_with_fallback_t<مرکب_iter_tmpl<مرکب_Glyph_ریکارڈ>,
						     ثابت مرکب_Glyph_ریکارڈ &>
{
  قسمتعریف ثابت مرکب_Glyph_ریکارڈ *__item_t__;
  مرکب_iter_tmpl (hb_بائٹیں_t glyph_, __item_t__ current_, منطقی extended_) :
      glyph (glyph_), current (خالی_اشاریہ), current_سائز (0), extended (extended_)
  {
    سیٹ_current (current_);
  }

  مرکب_iter_tmpl () : glyph (hb_بائٹیں_t ()), current (خالی_اشاریہ), current_سائز (0), extended (جھوٹ) {}

  ثابت مرکب_Glyph_ریکارڈ & __item__ () ثابت { واپس *current; }
  منطقی __more__ () ثابت { واپس current; }
  خالی __next__ ()
  {
    اگر (!current->has_مزید ()) { current = خالی_اشاریہ; واپس; }

    سیٹ_current (&Struct_At_انحراف<مرکب_Glyph_ریکارڈ> (current, current_سائز));
  }
  مرکب_iter_tmpl __end__ () ثابت { واپس مرکب_iter_tmpl (); }
  منطقی عامل != (ثابت مرکب_iter_tmpl& o) ثابت
  { واپس current != o.current; }


  خالی سیٹ_current (__item_t__ current_)
  {
    اگر (!glyph.check_range (current_, مرکب_Glyph_ریکارڈ::min_سائز))
    {
      current = خالی_اشاریہ;
      current_سائز = 0;
      واپس;
    }
    غیرعلامتی سائز = current_->get_سائز (extended);
    اگر (!glyph.check_range (current_, سائز))
    {
      current = خالی_اشاریہ;
      current_سائز = 0;
      واپس;
    }

    current = current_;
    current_سائز = سائز;
  }

  نجی:
  hb_بائٹیں_t glyph;
  __item_t__ current;
  غیرعلامتی current_سائز;
  منطقی extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#اگر_ختم /* OT_GLYF_COMPOSITE_ITER_HH */
