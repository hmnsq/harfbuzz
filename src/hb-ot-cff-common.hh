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

#ضمن "hb-open-type.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-cff-interp-dict-common.hh"
#ضمن "hb-subset-plan.hh"

نطاق CFF {

using نطاق OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
ساكن مضمن ثابت Type& StructAtOffsetOrNull (ثابت فراغ *P, صحيح offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  ܐܢ (!offset) ܦܢܐ Null (Type);

  ثابت حرف *p = (ثابت حرف *) P + offset;
  ܐܢ (!sc.check_point (p)) ܦܢܐ Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  ܐܢ (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) ܦܢܐ Null (Type);

  ܦܢܐ obj;
}


بنية code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned حرف>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
بنية Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (هذا);
    لكل (unsigned صحيح i = 0; i < dictval.get_count (); i++)
      ܐܢ (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (ܕܓܠܐ);

    return_trace (ܫܪܪܐ);
  }

  template <typename T, typename V>
  ساكن منطقي serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    ܐܢ (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      ܦܢܐ ܕܓܠܐ;

    TRACE_SERIALIZE (هذا);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), ܕܓܠܐ);
    ܐܢ (unlikely (!p)) return_trace (ܕܓܠܐ);
    ܐܢ (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (ܫܪܪܐ);
  }

  template <typename V>
  ساكن منطقي serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ܦܢܐ serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  ساكن منطقي serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ܦܢܐ serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, صحيح int_op>
  ساكن منطقي serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    ܐܢ (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) ܦܢܐ ܕܓܠܐ;
    c->add_link (ofs, link, whence);
    ܦܢܐ ܫܪܪܐ;
  }

  ساكن منطقي serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ܦܢܐ serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  ساكن منطقي serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ܦܢܐ serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

بنية TopDict : Dict {};
بنية FontDict : Dict {};
بنية PrivateDict : Dict {};

بنية table_info_t
{
  فراغ init () { offset = size = 0; link = 0; }

  unsigned صحيح    offset;
  unsigned صحيح    size;
  objidx_t	  link;
};

template <typename COUNT>
بنية FDArray : CFFIndex<COUNT>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (هذا);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    ܐܢ (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    حرف *data_base = c->head;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		ܦܢܐ c->head - (ثابت حرف*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->pop_pack (ܕܓܠܐ);

    ܐܢ (unlikely (sizes.in_error ())) return_trace (ܕܓܠܐ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<COUNT>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
بنية FDSelect0 {
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount,
                 unsigned صحيح glyph_count = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!(c->check_struct (هذا))))
      return_trace (ܕܓܠܐ);
    hb_barrier ();
    ܐܢ (unlikely (!c->check_array (fds, glyph_count == HB_CODEPOINT_INVALID ?
                                     c->get_num_glyphs () : glyph_count)))
      return_trace (ܕܓܠܐ);

    return_trace (ܫܪܪܐ);
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  { ܦܢܐ fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { ܦܢܐ {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned صحيح num_glyphs) ثابت
  { ܦܢܐ hb_unsigned_mul_saturate (HBUINT8::static_size, num_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
بنية FDSelect3_4_Range
{
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ * /*nullptr*/, unsigned صحيح fdcount) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  first < c->get_num_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  عام:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
بنية FDSelect3_4
{
  size_t get_size () ثابت
  { ܦܢܐ hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount,
                 unsigned صحيح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!(c->check_struct (هذا) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (ܕܓܠܐ);

    لكل (unsigned صحيح i = 1; i < nRanges (); i++)
      ܐܢ (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (ܕܓܠܐ);

    ܐܢ (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_num_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_num_glyphs () : min_glyphs))))
      return_trace (ܕܓܠܐ);

    return_trace (ܫܪܪܐ);
  }

  ساكن صحيح _cmp_range (ثابت فراغ *_key, ثابت فراغ *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) _key;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    ܐܢ (glyph < range[0].first) ܦܢܐ -1;
    ܐܢ (glyph < range[1].first) ܦܢܐ 0;
    ܦܢܐ +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    ܦܢܐ range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    ܦܢܐ {fd, end};
  }

  GID_TYPE        &nRanges ()       { ܦܢܐ ranges.len; }
  GID_TYPE         nRanges () ثابت { ܦܢܐ ranges.len; }
  GID_TYPE       &sentinel ()       { ܦܢܐ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { ܦܢܐ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

تعريفنوع FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
تعريفنوع FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

بنية FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت FDSelect &src, unsigned صحيح num_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    unsigned صحيح size = src.get_size (num_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, ܕܓܠܐ);
    ܐܢ (unlikely (!dest)) return_trace (ܕܓܠܐ);
    hb_memcpy (dest, &src, size);
    return_trace (ܫܪܪܐ);
  }

  size_t get_size (unsigned صحيح num_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); ܦܢܐ hb_unsigned_add_saturate (format.static_size, u.format0.get_size (num_glyphs));
    حالة 3: hb_barrier (); ܦܢܐ hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    افتراضي:ܦܢܐ 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  {
    ܐܢ (هذا == &Null (FDSelect)) ܦܢܐ 0;

    بدل (format)
    {
    حالة 0: hb_barrier (); ܦܢܐ u.format0.get_fd (glyph);
    حالة 3: hb_barrier (); ܦܢܐ u.format3.get_fd (glyph);
    افتراضي:ܦܢܐ 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    ܐܢ (هذا == &Null (FDSelect)) ܦܢܐ {0, 1};

    بدل (format)
    {
    حالة 0: hb_barrier (); ܦܢܐ u.format0.get_fd_range (glyph);
    حالة 3: hb_barrier (); ܦܢܐ u.format3.get_fd_range (glyph);
    افتراضي:ܦܢܐ {0, 1};
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!c->check_struct (هذا)))
      return_trace (ܕܓܠܐ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    افتراضي:return_trace (ܕܓܠܐ);
    }
  }

  HBUINT8	format;
  اتحاد {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  عام:
  DEFINE_SIZE_MIN (1);
};

template <typename COUNT>
بنية Subrs : CFFIndex<COUNT>
{
  تعريفنوع COUNT count_type;
  تعريفنوع CFFIndex<COUNT> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
