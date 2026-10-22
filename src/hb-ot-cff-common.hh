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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-bimap.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-cff-interp-dict-common.hh"
#அந்தர்பாவ "hb-subset-plan.hh"

நாமாகாஶ CFF {

using நாமாகாஶ OT;

#define CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
template<typename Type, typename ...Ts>
நிலையான உள்வரி மாறிலி Type& StructAtOffsetOrNull (மாறிலி வெறுமை *P, முழு offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  எனில் (!offset) திரும்பு Null (Type);

  மாறிலி எழுத்து *p = (மாறிலி எழுத்து *) P + offset;
  எனில் (!sc.check_point (p)) திரும்பு Null (Type);

  மாறிலி Type &obj = *reinterpret_cast<மாறிலி Type *> (p);
  எனில் (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) திரும்பு Null (Type);

  திரும்பு obj;
}


கட்டமைப்பு code_pair_t
{
  unsigned code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<unsigned எழுத்து>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
கட்டமைப்பு Dict : UnsizedByteStr
{
  template <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (இது);
    க்கு (unsigned முழு i = 0; i < dictval.get_மொத்தம் (); i++)
      எனில் (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (பொய்);

    return_trace (உண்மை);
  }

  template <typename T, typename V>
  நிலையான தருக்க serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    எனில் (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      திரும்பு பொய்;

    TRACE_SERIALIZE (இது);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_size<HBUINT8> (OpCode_Size (op), பொய்);
    எனில் (unlikely (!p)) return_trace (பொய்);
    எனில் (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (உண்மை);
  }

  template <typename V>
  நிலையான தருக்க serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { திரும்பு serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  template <typename V>
  நிலையான தருக்க serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { திரும்பு serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  template <typename T, முழு int_op>
  நிலையான தருக்க serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->head + OpCode_Size (int_op));
    எனில் (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) திரும்பு பொய்;
    c->add_link (ofs, link, whence);
    திரும்பு உண்மை;
  }

  நிலையான தருக்க serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { திரும்பு serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  நிலையான தருக்க serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::Head)
  { திரும்பு serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

கட்டமைப்பு உச்சி_Dict : Dict {};
கட்டமைப்பு FontDict : Dict {};
கட்டமைப்பு PrivateDict : Dict {};

கட்டமைப்பு table_info_t
{
  வெறுமை init () { offset = size = 0; link = 0; }

  unsigned முழு    offset;
  unsigned முழு    size;
  objidx_t	  link;
};

template <typename மொத்தம்_3>
கட்டமைப்பு FDArray : CFFIndex<மொத்தம்_3>
{
  template <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (இது);

    /* serialize INDEX data */
    hb_vector_t<unsigned> sizes;
    எனில் (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    எழுத்து *data_base = c->head;
    + it
    | hb_map ([&] (மாறிலி hb_pair_t<மாறிலி DICTVAL&, மாறிலி INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		திரும்பு c->head - (மாறிலி எழுத்து*)dict;
	      })
    | hb_sink (sizes)
    ;
    unsigned data_size = c->head - data_base;
    c->எடு_pack (பொய்);

    எனில் (unlikely (sizes.in_error ())) return_trace (பொய்);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<மொத்தம்_3>::serialize_header (c, hb_iter (sizes), data_size));
  }
};

/* FDSelect */
கட்டமைப்பு FDSelect0 {
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு fdcount,
                 unsigned முழு glyph_மொத்தம் = HB_CODEPOINT_INVALID) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!(c->check_struct (இது))))
      return_trace (பொய்);
    hb_barrier ();
    எனில் (unlikely (!c->check_array (fds, glyph_மொத்தம் == HB_CODEPOINT_INVALID ?
                                     c->get_இலக்கம்_glyphs () : glyph_மொத்தம்)))
      return_trace (பொய்);

    return_trace (உண்மை);
  }

  unsigned get_fd (hb_codepoint_t glyph) மாறிலி
  { திரும்பு fds[glyph]; }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) மாறிலி
  { திரும்பு {fds[glyph], glyph + 1}; }

  size_t get_size (unsigned முழு இலக்கம்_glyphs) மாறிலி
  { திரும்பு hb_unsigned_mul_saturate (HBUINT8::static_size, இலக்கம்_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_SIZE_MIN (0);
};

template <typename GID_TYPE, typename FD_TYPE>
கட்டமைப்பு FDSelect3_4_Range
{
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை * /*nullptr*/, unsigned முழு fdcount) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  first < c->get_இலக்கம்_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  பொது:
  DEFINE_SIZE_STATIC (GID_TYPE::static_size + FD_TYPE::static_size);
};

template <typename GID_TYPE, typename FD_TYPE>
கட்டமைப்பு FDSelect3_4
{
  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_add_saturate ((size_t) GID_TYPE::static_size * 2, ranges.get_size ()); }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு fdcount,
                 unsigned முழு min_glyphs = HB_CODEPOINT_INVALID) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!(c->check_struct (இது) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (பொய்);

    க்கு (unsigned முழு i = 1; i < nRanges (); i++)
      எனில் (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (பொய்);

    எனில் (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_இலக்கம்_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_இலக்கம்_glyphs () : min_glyphs))))
      return_trace (பொய்);

    return_trace (உண்மை);
  }

  நிலையான முழு _cmp_range (மாறிலி வெறுமை *சாவி_3, மாறிலி வெறுமை *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) சாவி_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    எனில் (glyph < range[0].first) திரும்பு -1;
    எனில் (glyph < range[1].first) திரும்பு 0;
    திரும்பு +1;
  }

  unsigned get_fd (hb_codepoint_t glyph) மாறிலி
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, அளவு (ranges[0]), _cmp_range);
    திரும்பு range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) மாறிலி
  {
    auto *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, அளவு (ranges[0]), _cmp_range);
    unsigned fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    திரும்பு {fd, end};
  }

  GID_TYPE        &nRanges ()       { திரும்பு ranges.len; }
  GID_TYPE         nRanges () மாறிலி { திரும்பு ranges.len; }
  GID_TYPE       &sentinel ()       { திரும்பு StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  மாறிலி GID_TYPE &sentinel () மாறிலி { திரும்பு StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_SIZE_ARRAY (GID_TYPE::static_size, ranges);
};

வகைவரை FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
வகைவரை FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

கட்டமைப்பு FDSelect
{
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி FDSelect &src, unsigned முழு இலக்கம்_glyphs)
  {
    TRACE_SERIALIZE (இது);
    unsigned முழு size = src.get_size (இலக்கம்_glyphs);
    FDSelect *dest = c->allocate_size<FDSelect> (size, பொய்);
    எனில் (unlikely (!dest)) return_trace (பொய்);
    hb_memcpy (dest, &src, size);
    return_trace (உண்மை);
  }

  size_t get_size (unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு hb_unsigned_add_saturate (format.static_size, u.format0.get_size (இலக்கம்_glyphs));
    நிலை 3: hb_barrier (); திரும்பு hb_unsigned_add_saturate (format.static_size, u.format3.get_size ());
    இயல்பு:திரும்பு 0;
    }
  }

  unsigned get_fd (hb_codepoint_t glyph) மாறிலி
  {
    எனில் (இது == &Null (FDSelect)) திரும்பு 0;

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_fd (glyph);
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_fd (glyph);
    இயல்பு:திரும்பு 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<unsigned, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) மாறிலி
  {
    எனில் (இது == &Null (FDSelect)) திரும்பு {0, 1};

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_fd_range (glyph);
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_fd_range (glyph);
    இயல்பு:திரும்பு {0, 1};
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு fdcount) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    இயல்பு:return_trace (பொய்);
    }
  }

  HBUINT8	format;
  ஒன்றிப்பு {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  பொது:
  DEFINE_SIZE_MIN (1);
};

template <typename மொத்தம்_3>
கட்டமைப்பு Subrs : CFFIndex<மொத்தம்_3>
{
  வகைவரை மொத்தம்_3 மொத்தம்_type;
  வகைவரை CFFIndex<மொத்தம்_3> SUPER;
};

} /* namespace CFF */

#endif /* HB_OT_CFF_COMMON_HH */
