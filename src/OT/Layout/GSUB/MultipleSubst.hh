#جے_بنا_تعریف OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#تعریف_کرو OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#ضمن "Common.hh"
#ضمن "MultipleSubstFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية مضرب_Subst
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;         /* Format identifier */
  مضرب_Subst_Format1_2<SmallTypes>	format1;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  مضرب_Subst_Format1_2<MediumTypes>	format2;
#جے_انت
  } u;

  عام:

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بنانشانی صحيح format = 1;
    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    افتراضي:return_trace (خطأ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#جے_انت /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
