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

#شامل "hb-open-type.hh"
#شامل "hb-bimap.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-cff-interp-dict-common.hh"
#شامل "hb-subset-plan.hh"

فضاینام CFF {

using فضاینام OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
ایستا درونخطی ثابت Type& StructAtOffsetOrNull (ثابت تهی *P, صحیح offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  اگر (!offset) بازگشت Null (Type);

  ثابت نویسه *p = (ثابت نویسه *) P + offset;
  اگر (!sc.check_point (p)) بازگشت Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  اگر (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) بازگشت Null (Type);

  بازگشت obj;
}


ساختار code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned نویسه>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
ساختار Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (این);
    برای (unsigned صحیح i = 0; i < dictval.get_شمار (); i++)
      اگر (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (نادرست);

    return_trace (درست);
  }

  template <typename T, typename V>
  ایستا منطقی serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    اگر (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      بازگشت نادرست;

    TRACE_SERIALIZE (این);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), نادرست);
    اگر (unlikely (!p)) return_trace (نادرست);
    اگر (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (درست);
  }

  template <typename V>
  ایستا منطقی serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { بازگشت serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  ایستا منطقی serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { بازگشت serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, صحیح int_op>
  ایستا منطقی serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    اگر (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) بازگشت نادرست;
    c->add_link (ofs, link, whence);
    بازگشت درست;
  }

  ایستا منطقی serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { بازگشت serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  ایستا منطقی serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { بازگشت serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

ساختار بالاترین_Dict : Dict {};
ساختار FontDict : Dict {};
ساختار PrivateDict : Dict {};

ساختار table_info_t
{
  تهی init () { offset = size = 0; link = 0; }

  unsigned صحیح    offset;
  unsigned صحیح    size;
  objidx_t	  link;
};

template <typename شمار_3>
ساختار FDArray : CFFIndex<شمار_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (این);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    اگر (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    نویسه *data_base = c->head;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		بازگشت c->head - (ثابت نویسه*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->برداشت_pack (نادرست);

    اگر (unlikely (sizes.in_error ())) return_trace (نادرست);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<شمار_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
ساختار FDSelect0 {
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح fdcount,
                 unsigned صحیح glyph_شمار = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!(c->check_struct (این))))
      return_trace (نادرست);
    hb_barrier ();
    اگر (unlikely (!c->check_array (fds, glyph_شمار == HB_CODEPOINT_INVALID ?
                                     c->get_رقم_glyphs () : glyph_شمار)))
      return_trace (نادرست);

    return_trace (درست);
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  { بازگشت fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { بازگشت {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned صحیح رقم_glyphs) ثابت
  { بازگشت hb_unsigned_mul_saturate (HBUINT8::static_size, رقم_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
ساختار FDSelect3_4_Range
{
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی * /*nullptr*/, unsigned صحیح fdcount) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  first < c->get_رقم_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  عمومی:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
ساختار FDSelect3_4
{
  size_t get_size () ثابت
  { بازگشت hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح fdcount,
                 unsigned صحیح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!(c->check_struct (این) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (نادرست);

    برای (unsigned صحیح i = 1; i < nRanges (); i++)
      اگر (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (نادرست);

    اگر (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_رقم_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_رقم_glyphs () : min_glyphs))))
      return_trace (نادرست);

    return_trace (درست);
  }

  ایستا صحیح _cmp_range (ثابت تهی *کلید_3, ثابت تهی *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) کلید_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    اگر (glyph < range[0].first) بازگشت -1;
    اگر (glyph < range[1].first) بازگشت 0;
    بازگشت +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, اندازه (ranges[0]), _cmp_range);
    بازگشت range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, اندازه (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    بازگشت {fd, end};
  }

  GID_TYPE        &nRanges ()       { بازگشت ranges.len; }
  GID_TYPE         nRanges () ثابت { بازگشت ranges.len; }
  GID_TYPE       &sentinel ()       { بازگشت StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { بازگشت StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

تعریفنوع FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
تعریفنوع FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

ساختار FDSelect
{
  منطقی serialize (hb_serialize_context_t *c, ثابت FDSelect &src, unsigned صحیح رقم_glyphs)
  {
    TRACE_SERIALIZE (این);
    unsigned صحیح size = src.get_size (رقم_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, نادرست);
    اگر (unlikely (!dest)) return_trace (نادرست);
    hb_memcpy (dest, &src, size);
    return_trace (درست);
  }

  size_t get_size (unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت hb_unsigned_add_saturate (format.static_size, u.format0.get_size (رقم_glyphs));
    حالت 3: hb_barrier (); بازگشت hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    پیشفرض:بازگشت 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  {
    اگر (این == &Null (FDSelect)) بازگشت 0;

    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_fd (glyph);
    حالت 3: hb_barrier (); بازگشت u.format3.get_fd (glyph);
    پیشفرض:بازگشت 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    اگر (این == &Null (FDSelect)) بازگشت {0, 1};

    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_fd_range (glyph);
    حالت 3: hb_barrier (); بازگشت u.format3.get_fd_range (glyph);
    پیشفرض:بازگشت {0, 1};
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح fdcount) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این)))
      return_trace (نادرست);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    پیشفرض:return_trace (نادرست);
    }
  }

  HBUINT8	format;
  اجتماع {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (1);
};

template <typename شمار_3>
ساختار Subrs : CFFIndex<شمار_3>
{
  تعریفنوع شمار_3 شمار_type;
  تعریفنوع CFFIndex<شمار_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
