#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "MultipleSubstFormat1.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

কাঠামো MultipleSubst
{
  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  সর্বজনীন:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (u.format.v))) return_trace (মিথ্যা);
    unsigned পূর্ণসংখ্যা format = 1;
    u.format.v = format;
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
