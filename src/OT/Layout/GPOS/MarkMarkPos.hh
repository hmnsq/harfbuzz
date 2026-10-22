#ifndef OT_LAYOUT_GPOS_MARKMARKPOS_HH
#define OT_LAYOUT_GPOS_MARKMARKPOS_HH

#அந்தர்பாவ "MarkMarkPosFormat1.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

கட்டமைப்பு MarkMarkPos
{
  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }		format;         /* Format identifier */
  MarkMarkPosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkMarkPosFormat1_2<MediumTypes>	format2;
#endif
  } u;

  பொது:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOS_HH */
