/*
 * Copyright © 2018 Adobe Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Adobe Author(s): Michiharu Ariza
 */
#ifndef HB_OT_CFF_COMMON_HH
#define HB_OT_CFF_COMMON_HH

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bimap.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-dict-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"

ନାମାକାଶ CFF {

using ନାମାକାଶ OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ Type& StructAtOffsetOrNull (ଅଚଳ ଶୂନ୍ୟ *P, ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  ଯଦି (!offset) ଫେରନ୍ତୁ Null (Type);

  ଅଚଳ ଅକ୍ଷର *p = (ଅଚଳ ଅକ୍ଷର *) P + offset;
  ଯଦି (!sc.check_point (p)) ଫେରନ୍ତୁ Null (Type);

  ଅଚଳ Type &obj = *reinterpret_cast<ଅଚଳ Type *> (p);
  ଯଦି (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) ଫେରନ୍ତୁ Null (Type);

  ଫେରନ୍ତୁ obj;
}


ସଂରଚନା code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned ଅକ୍ଷର>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
ସଂରଚନା Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (ଏହି);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dictval.get_ଗଣନା (); i++)
      ଯଦି (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  template <typename T, typename V>
  ସ୍ଥିର ତାର୍କିକ serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    ଯଦି (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    TRACE_SERIALIZE (ଏହି);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), ମିଥ୍ୟା);
    ଯଦି (unlikely (!p)) return_trace (ମିଥ୍ୟା);
    ଯଦି (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (ସତ୍ୟ);
  }

  template <typename V>
  ସ୍ଥିର ତାର୍କିକ serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ଫେରନ୍ତୁ serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  ସ୍ଥିର ତାର୍କିକ serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ଫେରନ୍ତୁ serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା int_op>
  ସ୍ଥିର ତାର୍କିକ serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    ଯଦି (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    c->add_link (ofs, link, whence);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର ତାର୍କିକ serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ଫେରନ୍ତୁ serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  ସ୍ଥିର ତାର୍କିକ serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ଫେରନ୍ତୁ serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

ସଂରଚନା ଶୀର୍ଷ_Dict : Dict {};
ସଂରଚନା FontDict : Dict {};
ସଂରଚନା PrivateDict : Dict {};

ସଂରଚନା table_info_t
{
  ଶୂନ୍ୟ init () { offset = size = 0; link = 0; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    offset;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size;
  objidx_t	  link;
};

template <typename ଗଣନା_4>
ସଂରଚନା FDArray : CFFIndex<ଗଣନା_4>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (ଏହି);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    ଯଦି (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    ଅକ୍ଷର *data_base = c->head;
    + it
    | hb_map ([&] (ଅଚଳ hb_pair_t<ଅଚଳ DICTVAL&, ଅଚଳ INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		ଫେରନ୍ତୁ c->head - (ଅଚଳ ଅକ୍ଷର*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->କାଢ଼_pack (ମିଥ୍ୟା);

    ଯଦି (unlikely (sizes.in_error ())) return_trace (ମିଥ୍ୟା);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<ଗଣନା_4>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
ସଂରଚନା FDSelect0 {
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdcount,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_ଗଣନା = HB_CODEPOINT_INVALID) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!(c->check_struct (ଏହି))))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଯଦି (unlikely (!c->check_array (fds, glyph_ଗଣନା == HB_CODEPOINT_INVALID ?
                                     c->get_ଅଙ୍କ_glyphs () : glyph_ଗଣନା)))
      return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  unsigned get_fd (hb_codepoint_t glyph) ଅଚଳ
  { ଫେରନ୍ତୁ fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ଅଚଳ
  { ଫେରନ୍ତୁ {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_saturate (HBUINT8::static_size, ଅଙ୍କ_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
ସଂରଚନା FDSelect3_4_Range
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ * /*nullptr*/, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdcount) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  first < c->get_ଅଙ୍କ_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
ସଂରଚନା FDSelect3_4
{
  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdcount,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_glyphs = HB_CODEPOINT_INVALID) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!(c->check_struct (ଏହି) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (ମିଥ୍ୟା);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < nRanges (); i++)
      ଯଦି (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_ଅଙ୍କ_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_ଅଙ୍କ_glyphs () : min_glyphs))))
      return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cmp_range (ଅଚଳ ଶୂନ୍ୟ *ଚାବି_3, ଅଚଳ ଶୂନ୍ୟ *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) ଚାବି_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    ଯଦି (glyph < range[0].first) ଫେରନ୍ତୁ -1;
    ଯଦି (glyph < range[1].first) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ଅଚଳ
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, ଆକାର (ranges[0]), _cmp_range);
    ଫେରନ୍ତୁ range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ଅଚଳ
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, ଆକାର (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    ଫେରନ୍ତୁ {fd, end};
  }

  GID_TYPE        &nRanges ()       { ଫେରନ୍ତୁ ranges.len; }
  GID_TYPE         nRanges () ଅଚଳ { ଫେରନ୍ତୁ ranges.len; }
  GID_TYPE       &sentinel ()       { ଫେରନ୍ତୁ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ଅଚଳ GID_TYPE &sentinel () ଅଚଳ { ଫେରନ୍ତୁ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
ପ୍ରକାରବ୍ୟାଖ୍ୟା FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

ସଂରଚନା FDSelect
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ FDSelect &src, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = src.get_size (ଅଙ୍କ_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, ମିଥ୍ୟା);
    ଯଦି (unlikely (!dest)) return_trace (ମିଥ୍ୟା);
    hb_memcpy (dest, &src, size);
    return_trace (ସତ୍ୟ);
  }

  size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ hb_unsigned_add_saturate (format.static_size, u.format0.get_size (ଅଙ୍କ_glyphs));
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ଅଚଳ
  {
    ଯଦି (ଏହି == &Null (FDSelect)) ଫେରନ୍ତୁ 0;

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_fd (glyph);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_fd (glyph);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ଅଚଳ
  {
    ଯଦି (ଏହି == &Null (FDSelect)) ଫେରନ୍ତୁ {0, 1};

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_fd_range (glyph);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_fd_range (glyph);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ {0, 1};
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdcount) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  HBUINT8	format;
  ସଂଘ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (1);
};

template <typename ଗଣନା_4>
ସଂରଚନା Subrs : CFFIndex<ଗଣନା_4>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା_4 ଗଣନା_type;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା CFFIndex<ଗଣନା_4> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
