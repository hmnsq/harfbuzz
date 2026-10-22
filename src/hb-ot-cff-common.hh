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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_CFF_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bimap.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-dict-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"

ਨਾਮਾਕਾਸ਼ CFF {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CFF_UNDEF_ਕੋਡ  0xFFFFFFFF

ਵਰਤੋਂ objidx_t = hb_serialize_context_t::objidx_t;
ਵਰਤੋਂ whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ Type& Struct_At_ਵਿੱਥ_Or_Null (ਅਚਲ ਖਾਲੀ *P, ਪੂਰਨ ਵਿੱਥ, hb_sanitize_context_t &sc, Ts&&... ds)
{
  ਜੇ (!ਵਿੱਥ) ਵਾਪਸ Null (Type);

  ਅਚਲ ਅੱਖਰ *p = (ਅਚਲ ਅੱਖਰ *) P + ਵਿੱਥ;
  ਜੇ (!sc.check_ਬਿੰਦੂ (p)) ਵਾਪਸ Null (Type);

  ਅਚਲ Type &obj = *reinterpret_cast<ਅਚਲ Type *> (p);
  ਜੇ (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) ਵਾਪਸ Null (Type);

  ਵਾਪਸ obj;
}


ਬਣਤਰ ਕੋਡ_pair_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੋਡ;
  hb_codepoint_t glyph;
};


ਵਰਤੋਂ str_buff_t = hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>;
ਵਰਤੋਂ str_buff_vec_t = hb_ਸਦਿਸ਼_t<str_buff_t>;
ਵਰਤੋਂ glyph_ਮੰਜ਼ਿਲ_sid_map_t = hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>;

/* Top Dict, Font Dict, Private Dict */
ਬਣਤਰ Dict : UnsizedByteStr
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ DICTVAL, ਕਿਸਮ_ਨਾਂ OP_SERIALIZER, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < dictval.get_ਗਿਣਤੀ (); i++)
      ਜੇ (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_int_op (hb_serialize_context_t *c, op_ਕੋਡ_t op, V value, op_ਕੋਡ_t intOp)
  {
    ਜੇ (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      ਵਾਪਸ ਝੂਠ;

    TRACE_SERIALIZE (ਇਹ);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_ਮਾਪ<HBUINT8> (Op_ਕੋਡ_ਮਾਪ (op), ਝੂਠ);
    ਜੇ (unlikely (!p)) return_trace (ਝੂਠ);
    ਜੇ (Is_Op_ਕੋਡ_ESC (op))
    {
      *p = Op_ਕੋਡ_escape;
      op = Unmake_Op_ਕੋਡ_ESC (op);
      p++;
    }
    *p = op;
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_int4_op (hb_serialize_context_t *c, op_ਕੋਡ_t op, V value)
  { ਵਾਪਸ serialize_int_op<HBINT32> (c, op, value, Op_ਕੋਡ_longintdict); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_int2_op (hb_serialize_context_t *c, op_ਕੋਡ_t op, V value)
  { ਵਾਪਸ serialize_int_op<HBINT16> (c, op, value, Op_ਕੋਡ_shortint); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਪੂਰਨ int_op>
  ਸਥਿਰ ਤਾਰਕਿਕ serialize_link_op (hb_serialize_context_t *c, op_ਕੋਡ_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->ਸਿਰ_2 + Op_ਕੋਡ_ਮਾਪ (int_op));
    ਜੇ (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) ਵਾਪਸ ਝੂਠ;
    c->ਜੋੜੋ_link (ofs, link, whence);
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ serialize_link4_op (hb_serialize_context_t *c, op_ਕੋਡ_t op, objidx_t link, whence_t whence = whence_t::ਸਿਰ)
  { ਵਾਪਸ serialize_link_op<HBINT32, Op_ਕੋਡ_longintdict> (c, op, link, whence); }

  ਸਥਿਰ ਤਾਰਕਿਕ serialize_link2_op (hb_serialize_context_t *c, op_ਕੋਡ_t op, objidx_t link, whence_t whence = whence_t::ਸਿਰ)
  { ਵਾਪਸ serialize_link_op<HBINT16, Op_ਕੋਡ_shortint> (c, op, link, whence); }
};

ਬਣਤਰ ਸਿਖਰ_Dict : Dict {};
ਬਣਤਰ FontDict : Dict {};
ਬਣਤਰ ਨਿੱਜੀ_Dict : Dict {};

ਬਣਤਰ ਸਾਰਣੀ_info_t
{
  ਖਾਲੀ init () { ਵਿੱਥ = ਮਾਪ = 0; link = 0; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਵਿੱਥ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ;
  objidx_t	  link;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਗਿਣਤੀ_4>
ਬਣਤਰ FDArray : CFFIndex<ਗਿਣਤੀ_4>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ DICTVAL, ਕਿਸਮ_ਨਾਂ INFO, ਕਿਸਮ_ਨਾਂ Iterator, ਕਿਸਮ_ਨਾਂ OP_SERIALIZER>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (ਇਹ);

    /* serialize INDEX data */
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> sizes;
    ਜੇ (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    ਅੱਖਰ *ਅੰਕੜੇ_ਅਧਾਰ = c->ਸਿਰ_2;
    + it
    | hb_map ([&] (ਅਚਲ hb_pair_t<ਅਚਲ DICTVAL&, ਅਚਲ INFO&> &_)
    {
      FontDict *dict = c->ਸ਼ੁਰੂ_embed<FontDict> ();
		dict->serialize (c, _.ਪਹਿਲਾ, opszr, _.second);
		ਵਾਪਸ c->ਸਿਰ_2 - (ਅਚਲ ਅੱਖਰ*)dict;
	      })
    | hb_sink (sizes)
    ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = c->ਸਿਰ_2 - ਅੰਕੜੇ_ਅਧਾਰ;
    c->ਕੱਢੋ_pack (ਝੂਠ);

    ਜੇ (unlikely (sizes.ਵਿੱਚ_error ())) return_trace (ਝੂਠ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<ਗਿਣਤੀ_4>::serialize_ਸਿਰਲੇਖ (c, hb_iter (sizes), ਅੰਕੜੇ_ਮਾਪ));
  }
};

/* FDSelect */
ਬਣਤਰ FDSelect0 {
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdcount,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਗਿਣਤੀ = HB_CODEPOINT_INVALID) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!(c->check_struct (ਇਹ))))
      return_trace (ਝੂਠ);
    hb_barrier ();
    ਜੇ (unlikely (!c->check_array (fds, glyph_ਗਿਣਤੀ == HB_CODEPOINT_INVALID ?
                                     c->get_ਅੰਕ_glyphs () : glyph_ਗਿਣਤੀ)))
      return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_fd (hb_codepoint_t glyph) ਅਚਲ
  { ਵਾਪਸ fds[glyph]; }

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ਅਚਲ
  { ਵਾਪਸ {fds[glyph], glyph + 1}; }

  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_saturate (HBUINT8::static_ਮਾਪ, ਅੰਕ_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_ਮਾਪ_MIN (0);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GID_TYPE, ਕਿਸਮ_ਨਾਂ FD_TYPE>
ਬਣਤਰ FDSelect3_4_Range
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ * /*nullptr*/, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdcount) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਪਹਿਲਾ < c->get_ਅੰਕ_glyphs () && (fd < fdcount));
  }

  GID_TYPE    ਪਹਿਲਾ;
  FD_TYPE     fd;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (GID_TYPE::static_ਮਾਪ + FD_TYPE::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GID_TYPE, ਕਿਸਮ_ਨਾਂ FD_TYPE>
ਬਣਤਰ FDSelect3_4
{
  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate ((size_t) GID_TYPE::static_ਮਾਪ * 2, ranges.get_ਮਾਪ ()); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdcount,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ min_glyphs = HB_CODEPOINT_INVALID) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!(c->check_struct (ਇਹ) &&
		    ranges.sanitize (c, ਖਾਲੀ_ਸੂਚਕ, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].ਪਹਿਲਾ == 0)))
      return_trace (ਝੂਠ);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < nRanges (); i++)
      ਜੇ (unlikely (ranges[i - 1].ਪਹਿਲਾ >= ranges[i].ਪਹਿਲਾ))
	return_trace (ਝੂਠ);

    ਜੇ (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].ਪਹਿਲਾ &&
                   sentinel() <= c->get_ਅੰਕ_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_ਅੰਕ_glyphs () : min_glyphs))))
      return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਸਥਿਰ ਪੂਰਨ _cmp_range (ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) ਕੁੰਜੀ_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) ਵਸਤੂ_2;

    ਜੇ (glyph < range[0].ਪਹਿਲਾ) ਵਾਪਸ -1;
    ਜੇ (glyph < range[1].ਪਹਿਲਾ) ਵਾਪਸ 0;
    ਵਾਪਸ +1;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_fd (hb_codepoint_t glyph) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, ਆਕਾਰ (ranges[0]), _cmp_range);
    ਵਾਪਸ range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, ਆਕਾਰ (ranges[0]), _cmp_range);
    ਬਿਨਾਨਿਸ਼ਾਨੀ fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t ਸਿਰਾ = range ? range[1].ਪਹਿਲਾ : ranges[nRanges () - 1].ਪਹਿਲਾ;
    ਵਾਪਸ {fd, ਸਿਰਾ};
  }

  GID_TYPE        &nRanges ()       { ਵਾਪਸ ranges.len; }
  GID_TYPE         nRanges () ਅਚਲ { ਵਾਪਸ ranges.len; }
  GID_TYPE       &sentinel ()       { ਵਾਪਸ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ਅਚਲ GID_TYPE &sentinel () ਅਚਲ { ਵਾਪਸ StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  Array_ਦਾ<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_ਮਾਪ_ARRAY (GID_TYPE::static_ਮਾਪ, ranges);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

ਬਣਤਰ FDSelect
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ FDSelect &src, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = src.get_ਮਾਪ (ਅੰਕ_glyphs);
    FDSelect *dest = c->allocate_ਮਾਪ<FDSelect> (ਮਾਪ, ਝੂਠ);
    ਜੇ (unlikely (!dest)) return_trace (ਝੂਠ);
    hb_memcpy (dest, &src, ਮਾਪ);
    return_trace (ਸੱਚ);
  }

  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (format.static_ਮਾਪ, u.format0.get_ਮਾਪ (ਅੰਕ_glyphs));
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (format.static_ਮਾਪ, u.format3.get_ਮਾਪ ());
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_fd (hb_codepoint_t glyph) ਅਚਲ
  {
    ਜੇ (ਇਹ == &Null (FDSelect)) ਵਾਪਸ 0;

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_fd (glyph);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_fd (glyph);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ਅਚਲ
  {
    ਜੇ (ਇਹ == &Null (FDSelect)) ਵਾਪਸ {0, 1};

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_fd_range (glyph);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_fd_range (glyph);
    ਮੂਲ:ਵਾਪਸ {0, 1};
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdcount) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  HBUINT8	format;
  ਸੰਘ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (1);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਗਿਣਤੀ_4>
ਬਣਤਰ Subrs : CFFIndex<ਗਿਣਤੀ_4>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ_4 ਗਿਣਤੀ_type;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFFIndex<ਗਿਣਤੀ_4> SUPER;
};

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_OT_CFF_COMMON_HH */
