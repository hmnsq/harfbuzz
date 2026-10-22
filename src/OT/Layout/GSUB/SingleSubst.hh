#अगर_अघोषित OT_LAYOUT_GSUB_SINGLESUBST_HH
#घोषणा OT_LAYOUT_GSUB_SINGLESUBST_HH

#समावेश "Common.hh"
#समावेश "SingleSubstFormat1.hh"
#समावेश "SingleSubstFormat2.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

काष्ठा SingleSubst
{
  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#अगर_अघोषित HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#पूर्ण_अगर
  } u;

  खुला:

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                स्थिर hb_codepoint_pair_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (u.format.v))) return_trace (false);
    अचिन्हित format = 2;
    अचिन्हित delta = 0;
    अगर (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#अगर_अघोषित HB_NO_BEYOND_64K
       अगर (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { वापस gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#पूर्ण_अगर

      स्वतः get_delta = [=] (hb_codepoint_pair_t _)
                       { वापस (अचिन्हित) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      अगर (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    हाल 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    हाल 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#पूर्ण_अगर
    शेष:return_trace (false);
    }
  }
};

ढाँचा<typename Iterator>
अटल व्योम
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
