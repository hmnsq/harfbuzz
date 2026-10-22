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
  إذا (!offset) أرجع Null (Type);

  ثابت حرف *p = (ثابت حرف *) P + offset;
  إذا (!sc.check_point (p)) أرجع Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  إذا (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) أرجع Null (Type);

  أرجع obj;
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
    لكل (unsigned صحيح i = 0; i < dictval.get_ژماردن (); i++)
      إذا (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (خطأ);

    return_trace (true);
  }

  template <typename T, typename V>
  ساكن منطقي serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    إذا (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      أرجع خطأ;

    TRACE_SERIALIZE (هذا);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), خطأ);
    إذا (unlikely (!p)) return_trace (خطأ);
    إذا (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (true);
  }

  template <typename V>
  ساكن منطقي serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { أرجع serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  ساكن منطقي serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { أرجع serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, صحيح int_op>
  ساكن منطقي serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    إذا (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) أرجع خطأ;
    c->add_link (ofs, link, whence);
    أرجع true;
  }

  ساكن منطقي serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { أرجع serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  ساكن منطقي serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { أرجع serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

بنية لووتکە_Dict : Dict {};
بنية FontDict : Dict {};
بنية PrivateDict : Dict {};

بنية table_info_t
{
  فراغ init () { offset = size = 0; link = 0; }

  unsigned صحيح    offset;
  unsigned صحيح    size;
  objidx_t	  link;
};

template <typename ژماردن_3>
بنية FDArray : CFFIndex<ژماردن_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (هذا);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    إذا (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    حرف *data_base = c->head;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		أرجع c->head - (ثابت حرف*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->دەرهێنان_pack (خطأ);

    إذا (unlikely (sizes.in_error ())) return_trace (خطأ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<ژماردن_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
بنية FDSelect0 {
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount,
                 unsigned صحيح glyph_ژماردن = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا))))
      return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!c->check_array (fds, glyph_ژماردن == HB_CODEPOINT_INVALID ?
                                     c->get_ژمارە_glyphs () : glyph_ژماردن)))
      return_trace (خطأ);

    return_trace (true);
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  { أرجع fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { أرجع {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned صحيح ژمارە_glyphs) ثابت
  { أرجع hb_unsigned_mul_saturate (HBUINT8::static_size, ژمارە_glyphs); }

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
		  first < c->get_ژمارە_glyphs () && (fd < fdcount));
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
  { أرجع hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount,
                 unsigned صحيح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (خطأ);

    لكل (unsigned صحيح i = 1; i < nRanges (); i++)
      إذا (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (خطأ);

    إذا (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_ژمارە_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_ژمارە_glyphs () : min_glyphs))))
      return_trace (خطأ);

    return_trace (true);
  }

  ساكن صحيح _cmp_range (ثابت فراغ *کلیل_3, ثابت فراغ *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) کلیل_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    إذا (glyph < range[0].first) أرجع -1;
    إذا (glyph < range[1].first) أرجع 0;
    أرجع +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    أرجع range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    أرجع {fd, end};
  }

  GID_TYPE        &nRanges ()       { أرجع ranges.len; }
  GID_TYPE         nRanges () ثابت { أرجع ranges.len; }
  GID_TYPE       &sentinel ()       { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

تعريفنوع FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
تعريفنوع FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

بنية FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت FDSelect &src, unsigned صحيح ژمارە_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    unsigned صحيح size = src.get_size (ژمارە_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, خطأ);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, size);
    return_trace (true);
  }

  size_t get_size (unsigned صحيح ژمارە_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_add_saturate (format.static_size, u.format0.get_size (ژمارە_glyphs));
    حالة 3: hb_barrier (); أرجع hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    افتراضي:أرجع 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ثابت
  {
    إذا (هذا == &Null (FDSelect)) أرجع 0;

    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_fd (glyph);
    حالة 3: hb_barrier (); أرجع u.format3.get_fd (glyph);
    افتراضي:أرجع 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    إذا (هذا == &Null (FDSelect)) أرجع {0, 1};

    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_fd_range (glyph);
    حالة 3: hb_barrier (); أرجع u.format3.get_fd_range (glyph);
    افتراضي:أرجع {0, 1};
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    افتراضي:return_trace (خطأ);
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

template <typename ژماردن_3>
بنية Subrs : CFFIndex<ژماردن_3>
{
  تعريفنوع ژماردن_3 ژماردن_type;
  تعريفنوع CFFIndex<ژماردن_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
