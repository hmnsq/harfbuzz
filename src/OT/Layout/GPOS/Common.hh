#अगर_अघोषित OT_LAYOUT_GPOS_COMMON_HH
#घोषणा OT_LAYOUT_GPOS_COMMON_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

क्रमागत attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#घोषणा attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#घोषणा attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

काष्ठा मूल्य_Base;
काष्ठा मूल्य_Format;

ढाँचा<typename Iterator>
अटल व्योम SinglePos_serialize (hb_serialize_context_t *c,
                                 स्थिर मूल्य_Base *src,
                                 मूल्य_Format src_format,
                                 Iterator it,
                                 स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                                 अचिन्हित new_format);


}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_COMMON_HH
