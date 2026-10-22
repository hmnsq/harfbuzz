#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#অন্তর্ভাৱ "Common.hh"
#অন্তর্ভাৱ "MultipleSubstFormat1.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

সংরচনা MultipleSubst
{
  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  সার্ৱজনিক:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (u.format.v))) return_trace (মিছা);
    unsigned পূর্ণাঙ্ক format = 1;
    u.format.v = format;
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
