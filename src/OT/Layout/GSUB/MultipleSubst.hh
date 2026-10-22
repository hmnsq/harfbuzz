#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#تعریف_کرو OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#شامل "Common.hh"
#شامل "MultipleSubstFormat1.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

ساخت مضرب_Subst
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }		format;         /* Format identifier */
  مضرب_Subst_Format1_2<SmallTypes>	format1;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  مضرب_Subst_Format1_2<MediumTypes>	format2;
#اگر_ختم
  } u;

  عوامی:

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (u.format.v))) return_trace (جھوٹ);
    غیرعلامتی صحیح format = 1;
    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#اگر_ختم /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
