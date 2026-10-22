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
স্থির অন্তঃস্থ ধ্রুব Type& StructAtOffsetOrNull (ধ্রুব শূন্য *P, পূর্ণাঙ্ক offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  যদি (!offset) প্রত্যাগম Null (Type);

  ধ্রুব বর্ণ *p = (ধ্রুব বর্ণ *) P + offset;
  যদি (!sc.check_point (p)) প্রত্যাগম Null (Type);

  ধ্রুব Type &obj = *reinterpret_cast<ধ্রুব Type *> (p);
  যদি (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) প্রত্যাগম Null (Type);

  প্রত্যাগম obj;
}


সংরচনা code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned বর্ণ>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
সংরচনা Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুব DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (এতত্);
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < dictval.get_গুনতি (); i++)
      যদি (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (অসত্য);

    return_trace (সত্য);
  }

  template <typename T, typename V>
  স্থির তার্কিক serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    যদি (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      প্রত্যাগম অসত্য;

    TRACE_SERIALIZE (এতত্);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), অসত্য);
    যদি (unlikely (!p)) return_trace (অসত্য);
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
  স্থির তার্কিক serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { প্রত্যাগম serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  স্থির তার্কিক serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { প্রত্যাগম serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, পূর্ণাঙ্ক int_op>
  স্থির তার্কিক serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    যদি (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) প্রত্যাগম অসত্য;
    c->add_link (ofs, link, whence);
    প্রত্যাগম সত্য;
  }

  স্থির তার্কিক serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { প্রত্যাগম serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  স্থির তার্কিক serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { প্রত্যাগম serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

সংরচনা শীর্ষ_Dict : Dict {};
সংরচনা FontDict : Dict {};
সংরচনা PrivateDict : Dict {};

সংরচনা table_info_t
{
  শূন্য init () { offset = size = 0; link = 0; }

  unsigned পূর্ণাঙ্ক    offset;
  unsigned পূর্ণাঙ্ক    size;
  objidx_t	  link;
};

template <typename গুনতি_3>
সংরচনা FDArray : CFFIndex<গুনতি_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (এতত্);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    যদি (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    বর্ণ *data_base = c->head;
    + it
    | hb_map ([&] (ধ্রুব hb_pair_t<ধ্রুব DICTVAL&, ধ্রুব INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		প্রত্যাগম c->head - (ধ্রুব বর্ণ*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->বের_করো_pack (অসত্য);

    যদি (unlikely (sizes.in_error ())) return_trace (অসত্য);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<গুনতি_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
সংরচনা FDSelect0 {
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক fdcount,
                 unsigned পূর্ণাঙ্ক glyph_গুনতি = HB_CODEPOINT_INVALID) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!(c->check_struct (এতত্))))
      return_trace (অসত্য);
    hb_barrier ();
    যদি (unlikely (!c->check_array (fds, glyph_গুনতি == HB_CODEPOINT_INVALID ?
                                     c->get_অঙ্ক_glyphs () : glyph_গুনতি)))
      return_trace (অসত্য);

    return_trace (সত্য);
  }

  unsigned get_fd (hb_codepoint_t glyph) ধ্রুব
  { প্রত্যাগম fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ধ্রুব
  { প্রত্যাগম {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুব
  { প্রত্যাগম hb_unsigned_mul_saturate (HBUINT8::static_size, অঙ্ক_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
সংরচনা FDSelect3_4_Range
{
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য * /*nullptr*/, unsigned পূর্ণাঙ্ক fdcount) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  first < c->get_অঙ্ক_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
সংরচনা FDSelect3_4
{
  size_t get_size () ধ্রুব
  { প্রত্যাগম hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক fdcount,
                 unsigned পূর্ণাঙ্ক min_glyphs = HB_CODEPOINT_INVALID) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!(c->check_struct (এতত্) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (অসত্য);

    প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < nRanges (); i++)
      যদি (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (অসত্য);

    যদি (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_অঙ্ক_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_অঙ্ক_glyphs () : min_glyphs))))
      return_trace (অসত্য);

    return_trace (সত্য);
  }

  স্থির পূর্ণাঙ্ক _cmp_range (ধ্রুব শূন্য *চাবি_3, ধ্রুব শূন্য *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) চাবি_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    যদি (glyph < range[0].first) প্রত্যাগম -1;
    যদি (glyph < range[1].first) প্রত্যাগম 0;
    প্রত্যাগম +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ধ্রুব
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, পরিমাণ (ranges[0]), _cmp_range);
    প্রত্যাগম range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ধ্রুব
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, পরিমাণ (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    প্রত্যাগম {fd, end};
  }

  GID_TYPE        &nRanges ()       { প্রত্যাগম ranges.len; }
  GID_TYPE         nRanges () ধ্রুব { প্রত্যাগম ranges.len; }
  GID_TYPE       &sentinel ()       { প্রত্যাগম StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ধ্রুব GID_TYPE &sentinel () ধ্রুব { প্রত্যাগম StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

প্রকারনির্দেশ FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
প্রকারনির্দেশ FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

সংরচনা FDSelect
{
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুব FDSelect &src, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned পূর্ণাঙ্ক size = src.get_size (অঙ্ক_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, অসত্য);
    যদি (unlikely (!dest)) return_trace (অসত্য);
    hb_memcpy (dest, &src, size);
    return_trace (সত্য);
  }

  size_t get_size (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুব
  {
    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); প্রত্যাগম hb_unsigned_add_saturate (format.static_size, u.format0.get_size (অঙ্ক_glyphs));
    পক্ষ 3: hb_barrier (); প্রত্যাগম hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    স্বভাব:প্রত্যাগম 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ধ্রুব
  {
    যদি (এতত্ == &Null (FDSelect)) প্রত্যাগম 0;

    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.format0.get_fd (glyph);
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_fd (glyph);
    স্বভাব:প্রত্যাগম 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ধ্রুব
  {
    যদি (এতত্ == &Null (FDSelect)) প্রত্যাগম {0, 1};

    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.format0.get_fd_range (glyph);
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_fd_range (glyph);
    স্বভাব:প্রত্যাগম {0, 1};
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক fdcount) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্)))
      return_trace (অসত্য);
    hb_barrier ();

    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    স্বভাব:return_trace (অসত্য);
    }
  }

  HBUINT8	format;
  সঙ্ঘ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_MIN (1);
};

template <typename গুনতি_3>
সংরচনা Subrs : CFFIndex<গুনতি_3>
{
  প্রকারনির্দেশ গুনতি_3 গুনতি_type;
  প্রকারনির্দেশ CFFIndex<গুনতি_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
