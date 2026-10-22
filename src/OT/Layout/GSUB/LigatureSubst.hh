#अगर_अघोषित OT_LAYOUT_GSUB_LIGATURESUBST_HH
#घोषणा OT_LAYOUT_GSUB_LIGATURESUBST_HH

#समावेश "Common.hh"
#समावेश "LigatureSubstFormat1.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

काष्ठा LigatureSubst
{
  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }		format;         /* Format identifier */
  LigatureSubstFormat1_2<SmallTypes>	format1;
#अगर_अघोषित HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
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
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  ढाँचा <typename GlyphID>
  bool serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<स्थिर अचिन्हित पूर्णांक> ligature_per_first_glyph_गिनती_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<स्थिर अचिन्हित पूर्णांक> component_गिनती_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (u.format.v))) return_trace (false);
    अचिन्हित पूर्णांक format = 1;
    u.format.v = format;
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               first_glyphs,
                                               ligature_per_first_glyph_गिनती_list,
                                               ligatures_list,
                                               component_गिनती_list,
                                               component_list));
    शेष:return_trace (false);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
