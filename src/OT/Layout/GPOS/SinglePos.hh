#अगर_अघोषित OT_LAYOUT_GPOS_SINGLEPOS_HH
#घोषणा OT_LAYOUT_GPOS_SINGLEPOS_HH

#समावेश "SinglePosFormat1.hh"
#समावेश "SinglePosFormat2.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा SinglePos
{
  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#अगर_अघोषित HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#पूर्ण_अगर
  } u;

  खुला:
  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<स्थिर मूल्य> first_val_iter = hb_second (*glyph_val_iter_pairs);

    क्रम (स्थिर स्वतः iter : glyph_val_iter_pairs)
      क्रम (स्थिर स्वतः _ : hb_zip (iter.second, first_val_iter))
        अगर (_.first != _.second)
          वापस 2;

    वापस 1;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
                  स्थिर मूल्य_Base* src,
                  मूल्य_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                  अचिन्हित newFormat)
  {
    अगर (unlikely (!c->extend_min (u.format.v))) वापस;
    अचिन्हित format = 2;
    मूल्य_Format new_format;
    new_format = newFormat;

    अगर (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { वापस gid > 0xFFFFu; }))
      format += 2;
#पूर्ण_अगर

    u.format.v = format;
    चयन (u.format.v) {
    हाल 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      वापस;
    हाल 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      वापस;
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      वापस;
    हाल 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      वापस;
#पूर्ण_अगर
    शेष:वापस;
    }
  }

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
};


ढाँचा<typename Iterator>
अटल व्योम
SinglePos_serialize (hb_serialize_context_t *c,
                     स्थिर मूल्य_Base *src,
                     मूल्य_Format src_format,
                     Iterator it,
                     स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                     अचिन्हित new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
