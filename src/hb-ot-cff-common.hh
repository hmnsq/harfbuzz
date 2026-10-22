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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-bimap.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"
#അന്തര്ഭാവ "hb-cff-interp-dict-common.hh"
#അന്തര്ഭാവ "hb-subset-plan.hh"

നാമാകാശ CFF {

using നാമാകാശ OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
സ്ഥിതം വരിയിൽ സ്ഥിരം Type& StructAtOffsetOrNull (സ്ഥിരം ശൂന്യം *P, പൂർണ്ണസംഖ്യ offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  എങ്കിൽ (!offset) തിരികെ Null (Type);

  സ്ഥിരം അക്ഷരം *p = (സ്ഥിരം അക്ഷരം *) P + offset;
  എങ്കിൽ (!sc.check_point (p)) തിരികെ Null (Type);

  സ്ഥിരം Type &obj = *reinterpret_cast<സ്ഥിരം Type *> (p);
  എങ്കിൽ (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) തിരികെ Null (Type);

  തിരികെ obj;
}


ഘടന code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned അക്ഷരം>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
ഘടന Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (ഇത്);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < dictval.get_എണ്ണം (); i++)
      എങ്കിൽ (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  template <typename T, typename V>
  സ്ഥിതം തർക്കം serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    എങ്കിൽ (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      തിരികെ അസത്യം;

    TRACE_SERIALIZE (ഇത്);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), അസത്യം);
    എങ്കിൽ (unlikely (!p)) return_trace (അസത്യം);
    എങ്കിൽ (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (സത്യം);
  }

  template <typename V>
  സ്ഥിതം തർക്കം serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { തിരികെ serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  സ്ഥിതം തർക്കം serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { തിരികെ serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, പൂർണ്ണസംഖ്യ int_op>
  സ്ഥിതം തർക്കം serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    എങ്കിൽ (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) തിരികെ അസത്യം;
    c->add_link (ofs, link, whence);
    തിരികെ സത്യം;
  }

  സ്ഥിതം തർക്കം serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { തിരികെ serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  സ്ഥിതം തർക്കം serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { തിരികെ serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

ഘടന മുകൾ_Dict : Dict {};
ഘടന FontDict : Dict {};
ഘടന PrivateDict : Dict {};

ഘടന table_info_t
{
  ശൂന്യം init () { offset = size = 0; link = 0; }

  unsigned പൂർണ്ണസംഖ്യ    offset;
  unsigned പൂർണ്ണസംഖ്യ    size;
  objidx_t	  link;
};

template <typename എണ്ണം_3>
ഘടന FDArray : CFFIndex<എണ്ണം_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (ഇത്);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    എങ്കിൽ (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    അക്ഷരം *data_base = c->head;
    + it
    | hb_map ([&] (സ്ഥിരം hb_pair_t<സ്ഥിരം DICTVAL&, സ്ഥിരം INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		തിരികെ c->head - (സ്ഥിരം അക്ഷരം*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->എടുക്കുക_pack (അസത്യം);

    എങ്കിൽ (unlikely (sizes.in_error ())) return_trace (അസത്യം);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<എണ്ണം_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
ഘടന FDSelect0 {
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ fdcount,
                 unsigned പൂർണ്ണസംഖ്യ glyph_എണ്ണം = HB_CODEPOINT_INVALID) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!(c->check_struct (ഇത്))))
      return_trace (അസത്യം);
    hb_barrier ();
    എങ്കിൽ (unlikely (!c->check_array (fds, glyph_എണ്ണം == HB_CODEPOINT_INVALID ?
                                     c->get_അക്കം_glyphs () : glyph_എണ്ണം)))
      return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  unsigned get_fd (hb_codepoint_t glyph) സ്ഥിരം
  { തിരികെ fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) സ്ഥിരം
  { തിരികെ {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  { തിരികെ hb_unsigned_mul_saturate (HBUINT8::static_size, അക്കം_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
ഘടന FDSelect3_4_Range
{
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം * /*nullptr*/, unsigned പൂർണ്ണസംഖ്യ fdcount) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  first < c->get_അക്കം_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  പൊതു:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
ഘടന FDSelect3_4
{
  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ fdcount,
                 unsigned പൂർണ്ണസംഖ്യ min_glyphs = HB_CODEPOINT_INVALID) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!(c->check_struct (ഇത്) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (അസത്യം);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < nRanges (); i++)
      എങ്കിൽ (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_അക്കം_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_അക്കം_glyphs () : min_glyphs))))
      return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  സ്ഥിതം പൂർണ്ണസംഖ്യ _cmp_range (സ്ഥിരം ശൂന്യം *താക്കോൽ_3, സ്ഥിരം ശൂന്യം *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) താക്കോൽ_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    എങ്കിൽ (glyph < range[0].first) തിരികെ -1;
    എങ്കിൽ (glyph < range[1].first) തിരികെ 0;
    തിരികെ +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) സ്ഥിരം
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, വലുപ്പം (ranges[0]), _cmp_range);
    തിരികെ range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) സ്ഥിരം
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, വലുപ്പം (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    തിരികെ {fd, end};
  }

  GID_TYPE        &nRanges ()       { തിരികെ ranges.len; }
  GID_TYPE         nRanges () സ്ഥിരം { തിരികെ ranges.len; }
  GID_TYPE       &sentinel ()       { തിരികെ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  സ്ഥിരം GID_TYPE &sentinel () സ്ഥിരം { തിരികെ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

തരനിർവചനം FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
തരനിർവചനം FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

ഘടന FDSelect
{
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം FDSelect &src, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ size = src.get_size (അക്കം_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, അസത്യം);
    എങ്കിൽ (unlikely (!dest)) return_trace (അസത്യം);
    hb_memcpy (dest, &src, size);
    return_trace (സത്യം);
  }

  size_t get_size (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ hb_unsigned_add_saturate (format.static_size, u.format0.get_size (അക്കം_glyphs));
    അവസ്ഥ 3: hb_barrier (); തിരികെ hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) സ്ഥിരം
  {
    എങ്കിൽ (ഇത് == &Null (FDSelect)) തിരികെ 0;

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_fd (glyph);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_fd (glyph);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) സ്ഥിരം
  {
    എങ്കിൽ (ഇത് == &Null (FDSelect)) തിരികെ {0, 1};

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_fd_range (glyph);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_fd_range (glyph);
    സ്ഥിരസ്ഥിതി:തിരികെ {0, 1};
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ fdcount) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  HBUINT8	format;
  സംഘം {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (1);
};

template <typename എണ്ണം_3>
ഘടന Subrs : CFFIndex<എണ്ണം_3>
{
  തരനിർവചനം എണ്ണം_3 എണ്ണം_type;
  തരനിർവചനം CFFIndex<എണ്ണം_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
