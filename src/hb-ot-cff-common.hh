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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-bimap.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-cff-interp-dict-common.hh"
#अन्तर्भाव "hb-subset-plan.hh"

नामाकाश CFF {

using नामाकाश OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
स्थिर अन्तःस्थ ध्रुव Type& StructAtOffsetOrNull (ध्रुव शून्य *P, पूर्णाङ्क offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  सचे (!offset) पटिनिवत्त Null (Type);

  ध्रुव वर्ण *p = (ध्रुव वर्ण *) P + offset;
  सचे (!sc.check_point (p)) पटिनिवत्त Null (Type);

  ध्रुव Type &obj = *reinterpret_cast<ध्रुव Type *> (p);
  सचे (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) पटिनिवत्त Null (Type);

  पटिनिवत्त obj;
}


संरचना code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned वर्ण>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
संरचना Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (एतत्);
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < dictval.get_गणनम् (); i++)
      सचे (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (मिच्छा);

    return_trace (सच्च);
  }

  template <typename T, typename V>
  स्थिर तार्किक serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    सचे (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      पटिनिवत्त मिच्छा;

    TRACE_SERIALIZE (एतत्);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), मिच्छा);
    सचे (unlikely (!p)) return_trace (मिच्छा);
    सचे (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (सच्च);
  }

  template <typename V>
  स्थिर तार्किक serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { पटिनिवत्त serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  स्थिर तार्किक serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { पटिनिवत्त serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, पूर्णाङ्क int_op>
  स्थिर तार्किक serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    सचे (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) पटिनिवत्त मिच्छा;
    c->add_link (ofs, link, whence);
    पटिनिवत्त सच्च;
  }

  स्थिर तार्किक serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { पटिनिवत्त serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  स्थिर तार्किक serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { पटिनिवत्त serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

संरचना शिखरम्_Dict : Dict {};
संरचना FontDict : Dict {};
संरचना PrivateDict : Dict {};

संरचना table_info_t
{
  शून्य init () { offset = size = 0; link = 0; }

  unsigned पूर्णाङ्क    offset;
  unsigned पूर्णाङ्क    size;
  objidx_t	  link;
};

template <typename गणनम्_3>
संरचना FDArray : CFFIndex<गणनम्_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (एतत्);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    सचे (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    वर्ण *data_base = c->head;
    + it
    | hb_map ([&] (ध्रुव hb_pair_t<ध्रुव DICTVAL&, ध्रुव INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		पटिनिवत्त c->head - (ध्रुव वर्ण*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->निष्कासय_pack (मिच्छा);

    सचे (unlikely (sizes.in_error ())) return_trace (मिच्छा);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<गणनम्_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
संरचना FDSelect0 {
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क fdcount,
                 unsigned पूर्णाङ्क glyph_गणनम् = HB_CODEPOINT_INVALID) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!(c->check_struct (एतत्))))
      return_trace (मिच्छा);
    hb_barrier ();
    सचे (unlikely (!c->check_array (fds, glyph_गणनम् == HB_CODEPOINT_INVALID ?
                                     c->get_अङ्कः_glyphs () : glyph_गणनम्)))
      return_trace (मिच्छा);

    return_trace (सच्च);
  }

  unsigned get_fd (hb_codepoint_t glyph) ध्रुव
  { पटिनिवत्त fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ध्रुव
  { पटिनिवत्त {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  { पटिनिवत्त hb_unsigned_mul_saturate (HBUINT8::static_size, अङ्कः_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
संरचना FDSelect3_4_Range
{
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य * /*nullptr*/, unsigned पूर्णाङ्क fdcount) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  first < c->get_अङ्कः_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
संरचना FDSelect3_4
{
  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क fdcount,
                 unsigned पूर्णाङ्क min_glyphs = HB_CODEPOINT_INVALID) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!(c->check_struct (एतत्) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (मिच्छा);

    अत्थाय (unsigned पूर्णाङ्क i = 1; i < nRanges (); i++)
      सचे (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (मिच्छा);

    सचे (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_अङ्कः_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_अङ्कः_glyphs () : min_glyphs))))
      return_trace (मिच्छा);

    return_trace (सच्च);
  }

  स्थिर पूर्णाङ्क _cmp_range (ध्रुव शून्य *कुञ्चिका_3, ध्रुव शून्य *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) कुञ्चिका_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    सचे (glyph < range[0].first) पटिनिवत्त -1;
    सचे (glyph < range[1].first) पटिनिवत्त 0;
    पटिनिवत्त +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ध्रुव
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, परिमाण (ranges[0]), _cmp_range);
    पटिनिवत्त range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ध्रुव
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, परिमाण (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    पटिनिवत्त {fd, end};
  }

  GID_TYPE        &nRanges ()       { पटिनिवत्त ranges.len; }
  GID_TYPE         nRanges () ध्रुव { पटिनिवत्त ranges.len; }
  GID_TYPE       &sentinel ()       { पटिनिवत्त StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ध्रुव GID_TYPE &sentinel () ध्रुव { पटिनिवत्त StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

प्रकारनिर्देश FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
प्रकारनिर्देश FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

संरचना FDSelect
{
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव FDSelect &src, unsigned पूर्णाङ्क अङ्कः_glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned पूर्णाङ्क size = src.get_size (अङ्कः_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, मिच्छा);
    सचे (unlikely (!dest)) return_trace (मिच्छा);
    hb_memcpy (dest, &src, size);
    return_trace (सच्च);
  }

  size_t get_size (unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); पटिनिवत्त hb_unsigned_add_saturate (format.static_size, u.format0.get_size (अङ्कः_glyphs));
    पक्ष 3: hb_barrier (); पटिनिवत्त hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    स्वभाव:पटिनिवत्त 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ध्रुव
  {
    सचे (एतत् == &Null (FDSelect)) पटिनिवत्त 0;

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); पटिनिवत्त u.format0.get_fd (glyph);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_fd (glyph);
    स्वभाव:पटिनिवत्त 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ध्रुव
  {
    सचे (एतत् == &Null (FDSelect)) पटिनिवत्त {0, 1};

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); पटिनिवत्त u.format0.get_fd_range (glyph);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_fd_range (glyph);
    स्वभाव:पटिनिवत्त {0, 1};
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क fdcount) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!c->check_struct (एतत्)))
      return_trace (मिच्छा);
    hb_barrier ();

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    स्वभाव:return_trace (मिच्छा);
    }
  }

  HBUINT8	format;
  सङ्घ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (1);
};

template <typename गणनम्_3>
संरचना Subrs : CFFIndex<गणनम्_3>
{
  प्रकारनिर्देश गणनम्_3 गणनम्_type;
  प्रकारनिर्देश CFFIndex<गणनम्_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
