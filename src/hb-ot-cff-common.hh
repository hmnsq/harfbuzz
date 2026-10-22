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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-bimap.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-cff-interp-dict-common.hh"
#અન્તર્ભાવ "hb-subset-plan.hh"

નામાકાશ CFF {

using નામાકાશ OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
સ્થિર અંતઃરેખ અચલ Type& StructAtOffsetOrNull (અચલ શૂન્ય *P, પૂર્ણાંક offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  જો (!offset) પાછા Null (Type);

  અચલ અક્ષર *p = (અચલ અક્ષર *) P + offset;
  જો (!sc.check_point (p)) પાછા Null (Type);

  અચલ Type &obj = *reinterpret_cast<અચલ Type *> (p);
  જો (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) પાછા Null (Type);

  પાછા obj;
}


સંરચના code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned અક્ષર>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
સંરચના Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (આ);
    માટે (unsigned પૂર્ણાંક i = 0; i < dictval.get_ગણતરી (); i++)
      જો (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (ખોટું);

    return_trace (સાચું);
  }

  template <typename T, typename V>
  સ્થિર તાર્કિક serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    જો (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      પાછા ખોટું;

    TRACE_SERIALIZE (આ);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), ખોટું);
    જો (unlikely (!p)) return_trace (ખોટું);
    જો (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (સાચું);
  }

  template <typename V>
  સ્થિર તાર્કિક serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { પાછા serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  સ્થિર તાર્કિક serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { પાછા serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, પૂર્ણાંક int_op>
  સ્થિર તાર્કિક serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    જો (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) પાછા ખોટું;
    c->add_link (ofs, link, whence);
    પાછા સાચું;
  }

  સ્થિર તાર્કિક serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { પાછા serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  સ્થિર તાર્કિક serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { પાછા serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

સંરચના ટોચ_Dict : Dict {};
સંરચના FontDict : Dict {};
સંરચના PrivateDict : Dict {};

સંરચના table_info_t
{
  શૂન્ય init () { offset = size = 0; link = 0; }

  unsigned પૂર્ણાંક    offset;
  unsigned પૂર્ણાંક    size;
  objidx_t	  link;
};

template <typename ગણતરી_3>
સંરચના FDArray : CFFIndex<ગણતરી_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (આ);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    જો (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    અક્ષર *data_base = c->head;
    + it
    | hb_map ([&] (અચલ hb_pair_t<અચલ DICTVAL&, અચલ INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		પાછા c->head - (અચલ અક્ષર*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->કાઢો_pack (ખોટું);

    જો (unlikely (sizes.in_error ())) return_trace (ખોટું);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<ગણતરી_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
સંરચના FDSelect0 {
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક fdcount,
                 unsigned પૂર્ણાંક glyph_ગણતરી = HB_CODEPOINT_INVALID) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!(c->check_struct (આ))))
      return_trace (ખોટું);
    hb_barrier ();
    જો (unlikely (!c->check_array (fds, glyph_ગણતરી == HB_CODEPOINT_INVALID ?
                                     c->get_અંક_glyphs () : glyph_ગણતરી)))
      return_trace (ખોટું);

    return_trace (સાચું);
  }

  unsigned get_fd (hb_codepoint_t glyph) અચલ
  { પાછા fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) અચલ
  { પાછા {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned પૂર્ણાંક અંક_glyphs) અચલ
  { પાછા hb_unsigned_mul_saturate (HBUINT8::static_size, અંક_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
સંરચના FDSelect3_4_Range
{
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય * /*nullptr*/, unsigned પૂર્ણાંક fdcount) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  first < c->get_અંક_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  જાહેર:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
સંરચના FDSelect3_4
{
  size_t get_size () અચલ
  { પાછા hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક fdcount,
                 unsigned પૂર્ણાંક min_glyphs = HB_CODEPOINT_INVALID) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!(c->check_struct (આ) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (ખોટું);

    માટે (unsigned પૂર્ણાંક i = 1; i < nRanges (); i++)
      જો (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (ખોટું);

    જો (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_અંક_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_અંક_glyphs () : min_glyphs))))
      return_trace (ખોટું);

    return_trace (સાચું);
  }

  સ્થિર પૂર્ણાંક _cmp_range (અચલ શૂન્ય *ચાવી_3, અચલ શૂન્ય *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) ચાવી_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    જો (glyph < range[0].first) પાછા -1;
    જો (glyph < range[1].first) પાછા 0;
    પાછા +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) અચલ
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, કદ (ranges[0]), _cmp_range);
    પાછા range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) અચલ
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, કદ (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    પાછા {fd, end};
  }

  GID_TYPE        &nRanges ()       { પાછા ranges.len; }
  GID_TYPE         nRanges () અચલ { પાછા ranges.len; }
  GID_TYPE       &sentinel ()       { પાછા StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  અચલ GID_TYPE &sentinel () અચલ { પાછા StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

પ્રકારવ્યાખ્યા FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
પ્રકારવ્યાખ્યા FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

સંરચના FDSelect
{
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ FDSelect &src, unsigned પૂર્ણાંક અંક_glyphs)
  {
    TRACE_SERIALIZE (આ);
    unsigned પૂર્ણાંક size = src.get_size (અંક_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, ખોટું);
    જો (unlikely (!dest)) return_trace (ખોટું);
    hb_memcpy (dest, &src, size);
    return_trace (સાચું);
  }

  size_t get_size (unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા hb_unsigned_add_saturate (format.static_size, u.format0.get_size (અંક_glyphs));
    સ્થિતિ 3: hb_barrier (); પાછા hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    મૂળભૂત:પાછા 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) અચલ
  {
    જો (આ == &Null (FDSelect)) પાછા 0;

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_fd (glyph);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_fd (glyph);
    મૂળભૂત:પાછા 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) અચલ
  {
    જો (આ == &Null (FDSelect)) પાછા {0, 1};

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_fd_range (glyph);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_fd_range (glyph);
    મૂળભૂત:પાછા {0, 1};
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક fdcount) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  HBUINT8	format;
  સંઘ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (1);
};

template <typename ગણતરી_3>
સંરચના Subrs : CFFIndex<ગણતરી_3>
{
  પ્રકારવ્યાખ્યા ગણતરી_3 ગણતરી_type;
  પ્રકારવ્યાખ્યા CFFIndex<ગણતરી_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
