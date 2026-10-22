#ifndef OT_LAYOUT_GPOS_MARKBASEPOS_HH
#define OT_LAYOUT_GPOS_MARKBASEPOS_HH

#अन्तर्भाव "MarkBasePosFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना MarkBasePos
{
  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }		format;         /* Format identifier */
  MarkBasePosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkBasePosFormat1_2<MediumTypes>	format2;
#endif
  } u;

  सार्वजनिक:
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मूल्य ());
    }
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOS_HH */
