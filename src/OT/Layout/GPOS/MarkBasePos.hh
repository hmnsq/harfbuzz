#ifndef OT_LAYOUT_GPOS_MARKBASEPOS_HH
#define OT_LAYOUT_GPOS_MARKBASEPOS_HH

#অন্তর্ভাব "MarkBasePosFormat1.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো MarkBasePos
{
  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }		format;         /* Format identifier */
  MarkBasePosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkBasePosFormat1_2<MediumTypes>	format2;
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
};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOS_HH */
