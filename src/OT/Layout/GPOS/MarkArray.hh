#अगर_अघोषित OT_LAYOUT_GPOS_MARKARRAY_HH
#घोषणा OT_LAYOUT_GPOS_MARKARRAY_HH

#समावेश "AnchorMatrix.hh"
#समावेश "MarkRecord.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

अटल अंतरभूत hb_position_t
resolve_cross_offset (स्थिर hb_glyph_position_t *pos,
		      अचिन्हित पूर्णांक len,
		      अचिन्हित पूर्णांक glyph_pos,
		      hb_direction_t direction)
{
  bool horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  जबतक (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    पूर्णांक chain = pos[glyph_pos].attach_chain();
    अगर (!chain)
      अवरोध;
    अचिन्हित पूर्णांक parent = (पूर्णांक) glyph_pos + chain;
    अगर (unlikely (parent >= len))
      अवरोध;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  वापस offset;
}

अटल अंतरभूत bool
apply_mark_attachment (hb_ot_apply_context_t *c,
                       स्थिर Anchor &mark_anchor,
                       स्थिर Anchor &glyph_anchor,
                       अचिन्हित glyph_pos)
{
  hb_बफर_t *बफर = c->बफर;
  भग्न mark_x, mark_y, base_x, base_y;

  बफर->unsafe_to_break (glyph_pos, बफर->idx + 1);
  mark_anchor.get_anchor (c, बफर->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, बफर->info[glyph_pos].codepoint, &base_x, &base_y);

  अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
  {
    c->बफर->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->बफर->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (बफर->pos, बफर->len, glyph_pos,
						      बफर->props.direction);

  hb_glyph_position_t &mark = बफर->cur_pos();
  mark.attach_chain() = (पूर्णांक) glyph_pos - (पूर्णांक) बफर->idx;
  अगर (mark.attach_chain() != (पूर्णांक) glyph_pos - (पूर्णांक) बफर->idx)
  {
    mark.attach_chain() = 0;
    जाओ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  अन्यथा
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
  {
    c->बफर->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->बफर->idx, glyph_pos);
  }

overflow:
  बफर->idx++;
  वापस true;
}

HB_INTERNAL अंतरभूत
व्योम collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    स्थिर Coverage &coverage,
                                    स्थिर व्योम *base,
                                    glyph_तथ्य_t anchors,
                                    अचिन्हित गिनती,
                                    अचिन्हित record_आकार)
{
  क्रम (स्थिर स्वतः _ : hb_zip (coverage, hb_range (गिनती)))
  {
    अगर (!c->glyph_set->has (_.first)) जारी;
    glyph_तथ्य_t anchor = anchors;
    anchor.तथ्य = (स्थिर अक्षर *) anchors.तथ्य + _.second * record_आकार;
    अचिन्हित offset = anchor[0];
    अगर (offset)
      StructAtOffset<स्थिर Anchor> (base, offset).collect_variation_indices (c);
  }
}

ढाँचा <typename Types>
काष्ठा MarkArray : Types::ढाँचा ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  प्रकार typename Types::ढाँचा ArrayOf<MarkRecord<Types>> Super;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (Super::sanitize (c, यह));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर Coverage &coverage) स्थिर
  {
    अगर (!यह->len) वापस;
    collect_mark_variation_indices (c, coverage, यह,
                                   &यह->arrayZ[0].markAnchor,
                                   यह->len, MarkRecord<Types>::static_आकार);
  }

  ढाँचा <typename AnchorMatrixType>
  bool apply (hb_ot_apply_context_t *c,
              अचिन्हित पूर्णांक mark_सूचकांक, अचिन्हित पूर्णांक glyph_सूचकांक,
              स्थिर AnchorMatrixType &anchors, अचिन्हित पूर्णांक class_गिनती,
              अचिन्हित पूर्णांक glyph_pos) स्थिर
  {
    TRACE_APPLY (यह);
    स्थिर MarkRecord<Types> &record = Super::चालक[](mark_सूचकांक);
    अचिन्हित पूर्णांक mark_class = record.klass;

    स्थिर Anchor& mark_anchor = यह + record.markAnchor;
    bool found;
    स्थिर Anchor& glyph_anchor = anchors.get_anchor (c, glyph_सूचकांक, mark_class, class_गिनती, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    अगर (unlikely (!found)) return_trace (false);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  bool subset (hb_subset_context_t *c,
               Iterator             coverage,
               स्थिर hb_map_t      *klass_mapping) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    स्वतः* out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    स्वतः mark_iter =
    + hb_zip (coverage, यह->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    bool ret = false;
    अचिन्हित new_लंबाई = 0;
    क्रम (स्थिर स्वतः& mark_record : mark_iter) {
      ret |= mark_record.subset (c, यह, klass_mapping);
      new_लंबाई++;
    }

    अगर (unlikely (!c->serializer->check_assign (out->len, new_लंबाई,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (false);

    return_trace (ret);
  }
};

HB_INTERNAL अंतरभूत
व्योम Markclass_closure_and_remap_indexes (स्थिर Coverage &mark_coverage,
                                    स्थिर व्योम *mark_records,
                                    अचिन्हित गिनती,
                                    अचिन्हित record_आकार,
                                    स्थिर hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  क्रम (स्थिर स्वतः _ : hb_zip (mark_coverage, hb_range (गिनती)))
    अगर (glyphset.has (_.first))
    {
      स्थिर स्वतः *klass = reinterpret_cast<स्थिर HBUINT16 *>
        ((स्थिर अक्षर *) mark_records + _.second * record_आकार);
      orig_classes.add (*klass);
    }

  अचिन्हित idx = 0;
  क्रम (स्वतः klass : orig_classes.iter ())
  {
    अगर (klass_mapping->has (klass)) जारी;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

ढाँचा <typename Types>
HB_INTERNAL अंतरभूत
व्योम Markclass_closure_and_remap_indexes (स्थिर Coverage     &mark_coverage,
                                          स्थिर MarkArray<Types> &mark_array,
                                          स्थिर hb_set_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_closure_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       MarkRecord<Types>::static_आकार,
                                       glyphset, klass_mapping);
}

}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_MARKARRAY_HH */
