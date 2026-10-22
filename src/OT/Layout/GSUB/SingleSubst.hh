#ifndef OT_LAYOUT_GSUB_SINGLESUBST_HH
#define OT_LAYOUT_GSUB_SINGLESUBST_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "SingleSubstFormat1.hh"
#अन्तर्भाव "SingleSubstFormat2.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना SingleSubst
{
  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#endif
  } u;

  सार्वजनिक:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मूल्य ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                ध्रुव hb_codepoint_pair_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (u.format.v))) return_trace (असत्य);
    unsigned format = 2;
    unsigned delta = 0;
    यदि (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#ifndef HB_NO_BEYOND_64K
       यदि (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { प्रत्यागम gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#endif

      auto get_delta = [=] (hb_codepoint_pair_t _)
                       { प्रत्यागम (unsigned) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      यदि (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    पक्ष 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    पक्ष 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    स्वभाव:return_trace (असत्य);
    }
  }
};

template<typename Iterator>
स्थिर शून्य
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
