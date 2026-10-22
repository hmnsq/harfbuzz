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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-bimap.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-dict-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-plan.hh"

ನಾಮಾಕಾಶ CFF {

using ನಾಮಾಕಾಶ OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ Type& StructAtOffsetOrNull (ಧ್ರುವ ಶೂನ್ಯ *P, ಪೂರ್ಣಾಙ್ಕ offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  ಯದಿ (!offset) ಪ್ರತ್ಯಾಗಮ Null (Type);

  ಧ್ರುವ ವರ್ಣ *p = (ಧ್ರುವ ವರ್ಣ *) P + offset;
  ಯದಿ (!sc.check_point (p)) ಪ್ರತ್ಯಾಗಮ Null (Type);

  ಧ್ರುವ Type &obj = *reinterpret_cast<ಧ್ರುವ Type *> (p);
  ಯದಿ (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) ಪ್ರತ್ಯಾಗಮ Null (Type);

  ಪ್ರತ್ಯಾಗಮ obj;
}


ಸಂರಚನಾ code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned ವರ್ಣ>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
ಸಂರಚನಾ Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < dictval.get_ಎಣಿಕೆ (); i++)
      ಯದಿ (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  template <typename T, typename V>
  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    ಯದಿ (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    TRACE_SERIALIZE (ಏತತ್);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), ಅಸತ್ಯ);
    ಯದಿ (unlikely (!p)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (ಸತ್ಯ);
  }

  template <typename V>
  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ಪ್ರತ್ಯಾಗಮ serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ಪ್ರತ್ಯಾಗಮ serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, ಪೂರ್ಣಾಙ್ಕ int_op>
  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    ಯದಿ (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    c->add_link (ofs, link, whence);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ಪ್ರತ್ಯಾಗಮ serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ಪ್ರತ್ಯಾಗಮ serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

ಸಂರಚನಾ ತುದಿ_Dict : Dict {};
ಸಂರಚನಾ FontDict : Dict {};
ಸಂರಚನಾ PrivateDict : Dict {};

ಸಂರಚನಾ table_info_t
{
  ಶೂನ್ಯ init () { offset = size = 0; link = 0; }

  unsigned ಪೂರ್ಣಾಙ್ಕ    offset;
  unsigned ಪೂರ್ಣಾಙ್ಕ    size;
  objidx_t	  link;
};

template <typename ಎಣಿಕೆ_3>
ಸಂರಚನಾ FDArray : CFFIndex<ಎಣಿಕೆ_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (ಏತತ್);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    ಯದಿ (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    ವರ್ಣ *data_base = c->head;
    + it
    | hb_map ([&] (ಧ್ರುವ hb_pair_t<ಧ್ರುವ DICTVAL&, ಧ್ರುವ INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		ಪ್ರತ್ಯಾಗಮ c->head - (ಧ್ರುವ ವರ್ಣ*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->ತೆಗೆ_pack (ಅಸತ್ಯ);

    ಯದಿ (unlikely (sizes.in_error ())) return_trace (ಅಸತ್ಯ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<ಎಣಿಕೆ_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
ಸಂರಚನಾ FDSelect0 {
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ fdcount,
                 unsigned ಪೂರ್ಣಾಙ್ಕ glyph_ಎಣಿಕೆ = HB_CODEPOINT_INVALID) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!(c->check_struct (ಏತತ್))))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಯದಿ (unlikely (!c->check_array (fds, glyph_ಎಣಿಕೆ == HB_CODEPOINT_INVALID ?
                                     c->get_ಅಂಕೆ_glyphs () : glyph_ಎಣಿಕೆ)))
      return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  unsigned get_fd (hb_codepoint_t glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_saturate (HBUINT8::static_size, ಅಂಕೆ_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
ಸಂರಚನಾ FDSelect3_4_Range
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ * /*nullptr*/, unsigned ಪೂರ್ಣಾಙ್ಕ fdcount) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  first < c->get_ಅಂಕೆ_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
ಸಂರಚನಾ FDSelect3_4
{
  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ fdcount,
                 unsigned ಪೂರ್ಣಾಙ್ಕ min_glyphs = HB_CODEPOINT_INVALID) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!(c->check_struct (ಏತತ್) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < nRanges (); i++)
      ಯದಿ (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_ಅಂಕೆ_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_ಅಂಕೆ_glyphs () : min_glyphs))))
      return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _cmp_range (ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ_3, ಧ್ರುವ ಶೂನ್ಯ *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) ಕೀಲಿ_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    ಯದಿ (glyph < range[0].first) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (glyph < range[1].first) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) ಧ್ರುವ
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, ಪರಿಮಾಣ (ranges[0]), _cmp_range);
    ಪ್ರತ್ಯಾಗಮ range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ಧ್ರುವ
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, ಪರಿಮಾಣ (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    ಪ್ರತ್ಯಾಗಮ {fd, end};
  }

  GID_TYPE        &nRanges ()       { ಪ್ರತ್ಯಾಗಮ ranges.len; }
  GID_TYPE         nRanges () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ranges.len; }
  GID_TYPE       &sentinel ()       { ಪ್ರತ್ಯಾಗಮ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ಧ್ರುವ GID_TYPE &sentinel () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

ಪ್ರಕಾರನಿರ್ದೇಶ FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
ಪ್ರಕಾರನಿರ್ದೇಶ FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

ಸಂರಚನಾ FDSelect
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ FDSelect &src, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ size = src.get_size (ಅಂಕೆ_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, ಅಸತ್ಯ);
    ಯದಿ (unlikely (!dest)) return_trace (ಅಸತ್ಯ);
    hb_memcpy (dest, &src, size);
    return_trace (ಸತ್ಯ);
  }

  size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (format.static_size, u.format0.get_size (ಅಂಕೆ_glyphs));
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ಯದಿ (ಏತತ್ == &Null (FDSelect)) ಪ್ರತ್ಯಾಗಮ 0;

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_fd (glyph);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_fd (glyph);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ಯದಿ (ಏತತ್ == &Null (FDSelect)) ಪ್ರತ್ಯಾಗಮ {0, 1};

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_fd_range (glyph);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_fd_range (glyph);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ {0, 1};
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ fdcount) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  HBUINT8	format;
  ಸಙ್ಘ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (1);
};

template <typename ಎಣಿಕೆ_3>
ಸಂರಚನಾ Subrs : CFFIndex<ಎಣಿಕೆ_3>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಎಣಿಕೆ_3 ಎಣಿಕೆ_type;
  ಪ್ರಕಾರನಿರ್ದೇಶ CFFIndex<ಎಣಿಕೆ_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
