#अगर_अघोषित OT_LAYOUT_GPOS_MARKRECORD_HH
#घोषणा OT_LAYOUT_GPOS_MARKRECORD_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

ढाँचा <typename Types> काष्ठा MarkArray;

ढाँचा <typename Types>
काष्ठा MarkRecord
{
  मित्र काष्ठा MarkArray<Types>;

  खुला:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::ढाँचा OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  खुला:
  DEFINE_आकार_STATIC (2 + Types::आकार);

  अचिन्हित get_class () स्थिर { वापस (अचिन्हित) klass; }
  bool sanitize (hb_sanitize_context_t *c, स्थिर MarkArray<Types> *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && markAnchor.sanitize (c, base));
  }

  bool subset (hb_subset_context_t    *c,
	       स्थिर MarkArray<Types> *src_base,
	       स्थिर hb_map_t         *klass_mapping) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    out->klass = klass_mapping->लो (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर MarkArray<Types> *src_base) स्थिर
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_MARKRECORD_HH */
