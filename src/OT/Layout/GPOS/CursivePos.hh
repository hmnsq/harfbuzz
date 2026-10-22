#ifndef OT_LAYOUT_GPOS_CURSIVEPOS_HH
#define OT_LAYOUT_GPOS_CURSIVEPOS_HH

#अन्तर्भाव "CursivePosFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना CursivePos
{
  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; } format;        /* Format identifier */
  CursivePosFormat1_2<SmallTypes> format1;
#ifndef HB_NO_BEYOND_64K
  CursivePosFormat1_2<MediumTypes> format2;
#endif
  } u;

  सार्वजनिक:
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOS_HH */
