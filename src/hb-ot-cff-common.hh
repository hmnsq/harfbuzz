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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-bimap.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-cff-interp-dict-common.hh"
#অন্তর্ভাব "hb-subset-plan.hh"

নামাকাশ CFF {

using নামাকাশ OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
স্থির অন্তঃরেখ ধ্রুবক Type& StructAtOffsetOrNull (ধ্রুবক শূন্য *P, পূর্ণসংখ্যা offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  যদি (!offset) ফেরত Null (Type);

  ধ্রুবক অক্ষর *p = (ধ্রুবক অক্ষর *) P + offset;
  যদি (!sc.check_point (p)) ফেরত Null (Type);

  ধ্রুবক Type &obj = *reinterpret_cast<ধ্রুবক Type *> (p);
  যদি (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) ফেরত Null (Type);

  ফেরত obj;
}


কাঠামো code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned অক্ষর>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
কাঠামো Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (এই);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < dictval.get_গুনতি (); i++)
      যদি (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  template <typename T, typename V>
  স্থির যৌক্তিক serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    যদি (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      ফেরত মিথ্যা;

    TRACE_SERIALIZE (এই);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_আকার<HBUINT8> (Op_Code_আকার (op), মিথ্যা);
    যদি (unlikely (!p)) return_trace (মিথ্যা);
    যদি (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (সত্য);
  }

  template <typename V>
  স্থির যৌক্তিক serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ফেরত serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  স্থির যৌক্তিক serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ফেরত serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, পূর্ণসংখ্যা int_op>
  স্থির যৌক্তিক serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->শীর্ষ_2 + Op_Code_আকার (int_op));
    যদি (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) ফেরত মিথ্যা;
    c->add_link (ofs, link, whence);
    ফেরত সত্য;
  }

  স্থির যৌক্তিক serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::শীর্ষ)
  { ফেরত serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  স্থির যৌক্তিক serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::শীর্ষ)
  { ফেরত serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

কাঠামো শীর্ষ_Dict : Dict {};
কাঠামো FontDict : Dict {};
কাঠামো PrivateDict : Dict {};

কাঠামো table_info_t
{
  শূন্য init () { offset = আকার_2 = 0; link = 0; }

  unsigned পূর্ণসংখ্যা    offset;
  unsigned পূর্ণসংখ্যা    আকার_2;
  objidx_t	  link;
};

template <typename গুনতি_3>
কাঠামো FDArray : CFFIndex<গুনতি_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (এই);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    যদি (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    অক্ষর *তথ্য_base = c->শীর্ষ_2;
    + it
    | hb_map ([&] (ধ্রুবক hb_pair_t<ধ্রুবক DICTVAL&, ধ্রুবক INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		ফেরত c->শীর্ষ_2 - (ধ্রুবক অক্ষর*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned তথ্য_আকার = c->শীর্ষ_2 - তথ্য_base;
    c->বের_করো_pack (মিথ্যা);

    যদি (unlikely (sizes.in_error ())) return_trace (মিথ্যা);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<গুনতি_3>::serialize_header (c, hb_iter (sizes), তথ্য_আকার));
  }
};

/* FDSelect */
কাঠামো FDSelect0 {
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা fdcount,
                 unsigned পূর্ণসংখ্যা glyph_গুনতি = HB_CODEPOINT_INVALID) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!(c->check_struct (এই))))
      return_trace (মিথ্যা);
    hb_barrier ();
    যদি (unlikely (!c->check_array (fds, glyph_গুনতি == HB_CODEPOINT_INVALID ?
                                     c->get_অঙ্ক_glyphs () : glyph_গুনতি)))
      return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  unsigned get_fd (hb_codepoint_t glyph) ধ্রুবক
  { ফেরত fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ধ্রুবক
  { ফেরত {fds[glyph], glyph + 1}; }

  size_t get_আকার (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  { ফেরত hb_unsigned_mul_saturate (HBUINT8::static_আকার, অঙ্ক_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_আকার_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
কাঠামো FDSelect3_4_Range
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য * /*nullptr*/, unsigned পূর্ণসংখ্যা fdcount) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  first < c->get_অঙ্ক_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  সর্বজনীন:
  DEFINE_আকার_STATIC (GID_TYPE::static_আকার + FD_TYPE::static_আকার);
};

template <typename GID_TYPE, typename FD_TYPE>
কাঠামো FDSelect3_4
{
  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_add_saturate ((size_t) GID_TYPE::static_আকার * 2, ranges.get_আকার ()); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা fdcount,
                 unsigned পূর্ণসংখ্যা min_glyphs = HB_CODEPOINT_INVALID) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!(c->check_struct (এই) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (মিথ্যা);

    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < nRanges (); i++)
      যদি (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (মিথ্যা);

    যদি (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_অঙ্ক_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_অঙ্ক_glyphs () : min_glyphs))))
      return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  স্থির পূর্ণসংখ্যা _cmp_range (ধ্রুবক শূন্য *চাবি_3, ধ্রুবক শূন্য *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) চাবি_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    যদি (glyph < range[0].first) ফেরত -1;
    যদি (glyph < range[1].first) ফেরত 0;
    ফেরত +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ধ্রুবক
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, আকার (ranges[0]), _cmp_range);
    ফেরত range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ধ্রুবক
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, আকার (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    ফেরত {fd, end};
  }

  GID_TYPE        &nRanges ()       { ফেরত ranges.len; }
  GID_TYPE         nRanges () ধ্রুবক { ফেরত ranges.len; }
  GID_TYPE       &sentinel ()       { ফেরত StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ধ্রুবক GID_TYPE &sentinel () ধ্রুবক { ফেরত StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_আকার_ARRAY (GID_TYPE::static_আকার, ranges);
};

প্রকারলেখ FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
প্রকারলেখ FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

কাঠামো FDSelect
{
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক FDSelect &src, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এই);
    unsigned পূর্ণসংখ্যা আকার_2 = src.get_আকার (অঙ্ক_glyphs);
    FDSelect *dest = c->allocate_আকার<FDSelect> (আকার_2, মিথ্যা);
    যদি (unlikely (!dest)) return_trace (মিথ্যা);
    hb_memcpy (dest, &src, আকার_2);
    return_trace (সত্য);
  }

  size_t get_আকার (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত hb_unsigned_add_saturate (format.static_আকার, u.format0.get_আকার (অঙ্ক_glyphs));
    ক্ষেত্র 3: hb_barrier (); ফেরত hb_unsigned_add_saturate (format.static_আকার, u.format3.get_আকার ());
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ধ্রুবক
  {
    যদি (এই == &Null (FDSelect)) ফেরত 0;

    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_fd (glyph);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_fd (glyph);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ধ্রুবক
  {
    যদি (এই == &Null (FDSelect)) ফেরত {0, 1};

    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_fd_range (glyph);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_fd_range (glyph);
    পূর্বনির্ধারিত:ফেরত {0, 1};
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা fdcount) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();

    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  HBUINT8	format;
  সংঘ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  সর্বজনীন:
  DEFINE_আকার_MIN (1);
};

template <typename গুনতি_3>
কাঠামো Subrs : CFFIndex<গুনতি_3>
{
  প্রকারলেখ গুনতি_3 গুনতি_type;
  প্রকারলেখ CFFIndex<গুনতি_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
