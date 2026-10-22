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

#include "hb-open-type.hh"
#include "hb-bimap.hh"
#include "hb-ot-layout-common.hh"
#include "hb-cff-interp-dict-common.hh"
#include "hb-subset-plan.hh"

namespace CFF {

using namespace OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
static anta_hstha const Type& StructAtOffsetOrNull (const void *P, puur_naa_nka offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  ပေႃး (!offset) ပွၵ်ႈ Null (Type);

  const var_na *p = (const var_na *) P + offset;
  ပေႃး (!sc.check_point (p)) ပွၵ်ႈ Null (Type);

  const Type &obj = *reinterpret_cast<const Type *> (p);
  ပေႃး (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) ပွၵ်ႈ Null (Type);

  ပွၵ်ႈ obj;
}


sa_nracanaa code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned var_na>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
sa_nracanaa Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  bool serialize (hb_serialize_context_t *c,
		  const DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (this);
    for (unsigned puur_naa_nka i = 0; i < dictval.get_count (); i++)
      ပေႃး (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (false);

    return_trace (true);
  }

  template <typename T, typename V>
  static bool serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    ပေႃး (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      ပွၵ်ႈ false;

    TRACE_SERIALIZE (this);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), false);
    ပေႃး (unlikely (!p)) return_trace (false);
    ပေႃး (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (true);
  }

  template <typename V>
  static bool serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ပွၵ်ႈ serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  static bool serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { ပွၵ်ႈ serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, puur_naa_nka int_op>
  static bool serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    ပေႃး (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) ပွၵ်ႈ false;
    c->add_link (ofs, link, whence);
    ပွၵ်ႈ true;
  }

  static bool serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ပွၵ်ႈ serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  static bool serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { ပွၵ်ႈ serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

sa_nracanaa TopDict : Dict {};
sa_nracanaa FontDict : Dict {};
sa_nracanaa PrivateDict : Dict {};

sa_nracanaa table_info_t
{
  void init () { offset = size = 0; link = 0; }

  unsigned puur_naa_nka    offset;
  unsigned puur_naa_nka    size;
  objidx_t	  link;
};

template <typename COUNT>
sa_nracanaa FDArray : CFFIndex<COUNT>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (this);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    ပေႃး (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    var_na *data_base = c->head;
    + it
    | hb_map ([&] (const hb_pair_t<const DICTVAL&, const INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		ပွၵ်ႈ c->head - (const var_na*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->pop_pack (false);

    ပေႃး (unlikely (sizes.in_error ())) return_trace (false);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<COUNT>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
sa_nracanaa FDSelect0 {
  bool sanitize (hb_sanitize_context_t *c, unsigned puur_naa_nka fdcount,
                 unsigned puur_naa_nka glyph_count = HB_CODEPOINT_INVALID) const
  {
    TRACE_SANITIZE (this);
    ပေႃး (unlikely (!(c->check_struct (this))))
      return_trace (false);
    hb_barrier ();
    ပေႃး (unlikely (!c->check_array (fds, glyph_count == HB_CODEPOINT_INVALID ?
                                     c->get_num_glyphs () : glyph_count)))
      return_trace (false);

    return_trace (true);
  }

  unsigned get_fd (hb_codepoint_t glyph) const
  { ပွၵ်ႈ fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) const
  { ပွၵ်ႈ {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned puur_naa_nka num_glyphs) const
  { ပွၵ်ႈ hb_unsigned_mul_saturate (HBUINT8::static_size, num_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
sa_nracanaa FDSelect3_4_Range
{
  bool sanitize (hb_sanitize_context_t *c, const void * /*nullptr*/, unsigned puur_naa_nka fdcount) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  first < c->get_num_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  public:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
sa_nracanaa FDSelect3_4
{
  size_t get_size () const
  { ပွၵ်ႈ hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  bool sanitize (hb_sanitize_context_t *c, unsigned puur_naa_nka fdcount,
                 unsigned puur_naa_nka min_glyphs = HB_CODEPOINT_INVALID) const
  {
    TRACE_SANITIZE (this);
    ပေႃး (unlikely (!(c->check_struct (this) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (false);

    for (unsigned puur_naa_nka i = 1; i < nRanges (); i++)
      ပေႃး (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (false);

    ပေႃး (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_num_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_num_glyphs () : min_glyphs))))
      return_trace (false);

    return_trace (true);
  }

  static puur_naa_nka _cmp_range (const void *_key, const void *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) _key;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    ပေႃး (glyph < range[0].first) ပွၵ်ႈ -1;
    ပေႃး (glyph < range[1].first) ပွၵ်ႈ 0;
    ပွၵ်ႈ +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) const
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, parimaa_na (ranges[0]), _cmp_range);
    ပွၵ်ႈ range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) const
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, parimaa_na (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    ပွၵ်ႈ {fd, end};
  }

  GID_TYPE        &nRanges ()       { ပွၵ်ႈ ranges.len; }
  GID_TYPE         nRanges () const { ပွၵ်ႈ ranges.len; }
  GID_TYPE       &sentinel ()       { ပွၵ်ႈ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  const GID_TYPE &sentinel () const { ပွၵ်ႈ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

typedef FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
typedef FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

sa_nracanaa FDSelect
{
  bool serialize (hb_serialize_context_t *c, const FDSelect &src, unsigned puur_naa_nka num_glyphs)
  {
    TRACE_SERIALIZE (this);
    unsigned puur_naa_nka size = src.get_size (num_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, false);
    ပေႃး (unlikely (!dest)) return_trace (false);
    hb_memcpy (dest, &src, size);
    return_trace (true);
  }

  size_t get_size (unsigned puur_naa_nka num_glyphs) const
  {
    switch (format)
    {
    pak_sa 0: hb_barrier (); ပွၵ်ႈ hb_unsigned_add_saturate (format.static_size, u.format0.get_size (num_glyphs));
    pak_sa 3: hb_barrier (); ပွၵ်ႈ hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    default:ပွၵ်ႈ 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) const
  {
    ပေႃး (this == &Null (FDSelect)) ပွၵ်ႈ 0;

    switch (format)
    {
    pak_sa 0: hb_barrier (); ပွၵ်ႈ u.format0.get_fd (glyph);
    pak_sa 3: hb_barrier (); ပွၵ်ႈ u.format3.get_fd (glyph);
    default:ပွၵ်ႈ 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) const
  {
    ပေႃး (this == &Null (FDSelect)) ပွၵ်ႈ {0, 1};

    switch (format)
    {
    pak_sa 0: hb_barrier (); ပွၵ်ႈ u.format0.get_fd_range (glyph);
    pak_sa 3: hb_barrier (); ပွၵ်ႈ u.format3.get_fd_range (glyph);
    default:ပွၵ်ႈ {0, 1};
    }
  }

  bool sanitize (hb_sanitize_context_t *c, unsigned puur_naa_nka fdcount) const
  {
    TRACE_SANITIZE (this);
    ပေႃး (unlikely (!c->check_struct (this)))
      return_trace (false);
    hb_barrier ();

    switch (format)
    {
    pak_sa 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    pak_sa 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    default:return_trace (false);
    }
  }

  HBUINT8	format;
  sa_ngha {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  public:
  DEFINE_SIZE_MIN (1);
};

template <typename COUNT>
sa_nracanaa Subrs : CFFIndex<COUNT>
{
  typedef COUNT count_type;
  typedef CFFIndex<COUNT> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
