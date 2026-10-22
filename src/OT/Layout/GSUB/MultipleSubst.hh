#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "MultipleSubstFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना MultipleSubst
{
  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  सार्वजनिक:

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) फर्काऊ c->no_dispatch_return_मान ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मान ());
    }
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (u.format.v))) return_trace (असत्य);
    अचिन्हित पूर्णाङ्क format = 1;
    u.format.v = format;
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    स्वभाव:return_trace (असत्य);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
