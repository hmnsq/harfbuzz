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
#که_نه_تعریف_شوی HB_OT_CFF_COMMON_HH
#تعریف_کړه HB_OT_CFF_COMMON_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-cff-interp-dict-common.hh"
#ضمن "hb-subset-plan.hh"

نطاق CFF {

کارول نطاق OT;

#تعریف_کړه CFF_UNDEF_کوډ  0xFFFFFFFF

کارول objidx_t = hb_serialize_context_t::objidx_t;
کارول whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
کينډۍ<ډول_نوم Type, ډول_نوم ...Ts>
ساكن مضمن ثابت Type& Struct_At_دننه_واټن_Or_Null (ثابت فراغ *P, صحيح دننه_واټن, hb_sanitize_context_t &sc, Ts&&... ds)
{
  إذا (!دننه_واټن) أرجع Null (Type);

  ثابت حرف *p = (ثابت حرف *) P + دننه_واټن;
  إذا (!sc.check_ټکی (p)) أرجع Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  إذا (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) أرجع Null (Type);

  أرجع obj;
}


بنية کوډ_pair_t
{
  بینښه کوډ;
  hb_codepoint_t glyph;
};


کارول str_buff_t = hb_ویکټور_t<بینښه حرف>;
کارول str_buff_vec_t = hb_ویکټور_t<str_buff_t>;
کارول glyph_منزل_sid_map_t = hb_ویکټور_t<کوډ_pair_t>;

/* Top Dict, Font Dict, Private Dict */
بنية Dict : UnsizedByteStr
{
  کينډۍ <ډول_نوم DICTVAL, ډول_نوم OP_SERIALIZER, ډول_نوم ...Ts>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (هذا);
    لكل (بینښه صحيح i = 0; i < dictval.get_شمېر (); i++)
      إذا (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (خطأ);

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم T, ډول_نوم V>
  ساكن منطقي serialize_int_op (hb_serialize_context_t *c, op_کوډ_t op, V value, op_کوډ_t intOp)
  {
    إذا (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      أرجع خطأ;

    TRACE_SERIALIZE (هذا);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_اندازه<HBUINT8> (Op_کوډ_اندازه (op), خطأ);
    إذا (unlikely (!p)) return_trace (خطأ);
    إذا (Is_Op_کوډ_ESC (op))
    {
      *p = Op_کوډ_escape;
      op = Unmake_Op_کوډ_ESC (op);
      p++;
    }
    *p = op;
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم V>
  ساكن منطقي serialize_int4_op (hb_serialize_context_t *c, op_کوډ_t op, V value)
  { أرجع serialize_int_op<HBINT32> (c, op, value, Op_کوډ_longintdict); }

  کينډۍ <ډول_نوم V>
  ساكن منطقي serialize_int2_op (hb_serialize_context_t *c, op_کوډ_t op, V value)
  { أرجع serialize_int_op<HBINT16> (c, op, value, Op_کوډ_shortint); }

  کينډۍ <ډول_نوم T, صحيح int_op>
  ساكن منطقي serialize_link_op (hb_serialize_context_t *c, op_کوډ_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->سر_2 + Op_کوډ_اندازه (int_op));
    إذا (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) أرجع خطأ;
    c->جمع_کړه_link (ofs, link, whence);
    أرجع ریښتیا;
  }

  ساكن منطقي serialize_link4_op (hb_serialize_context_t *c, op_کوډ_t op, objidx_t link, whence_t whence = whence_t::سر)
  { أرجع serialize_link_op<HBINT32, Op_کوډ_longintdict> (c, op, link, whence); }

  ساكن منطقي serialize_link2_op (hb_serialize_context_t *c, op_کوډ_t op, objidx_t link, whence_t whence = whence_t::سر)
  { أرجع serialize_link_op<HBINT16, Op_کوډ_shortint> (c, op, link, whence); }
};

بنية سر_Dict : Dict {};
بنية FontDict : Dict {};
بنية شخصي_Dict : Dict {};

بنية جدول_info_t
{
  فراغ init () { دننه_واټن = اندازه = 0; link = 0; }

  بینښه صحيح    دننه_واټن;
  بینښه صحيح    اندازه;
  objidx_t	  link;
};

کينډۍ <ډول_نوم شمېر_3>
بنية FDArray : CFFIndex<شمېر_3>
{
  کينډۍ <ډول_نوم DICTVAL, ډول_نوم INFO, ډول_نوم Iterator, ډول_نوم OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (هذا);

    /* serialize INDEX data */
    hb_ویکټور_t<بینښه> sizes;
    إذا (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    حرف *معلومات_بنسټ = c->سر_2;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->پیل_embed<FontDict> ();
		dict->serialize (c, _.لومړی, opszr, _.second);
		أرجع c->سر_2 - (ثابت حرف*)dict;
	      })
    | hb_sink (sizes)
    ;
    بینښه معلومات_اندازه = c->سر_2 - معلومات_بنسټ;
    c->راوباسه_pack (خطأ);

    إذا (unlikely (sizes.دننه_error ())) return_trace (خطأ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<شمېر_3>::serialize_سرلیک (c, hb_iter (sizes), معلومات_اندازه));
  }
};

/* FDSelect */
بنية FDSelect0 {
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح fdcount,
                 بینښه صحيح glyph_شمېر = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا))))
      return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!c->check_array (fds, glyph_شمېر == HB_CODEPOINT_INVALID ?
                                     c->get_عدد_glyphs () : glyph_شمېر)))
      return_trace (خطأ);

    return_trace (ریښتیا);
  }

  بینښه get_fd (hb_codepoint_t glyph) ثابت
  { أرجع fds[glyph]; }

  hb_pair_t<بینښه, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { أرجع {fds[glyph], glyph + 1}; }

  size_t get_اندازه (بینښه صحيح عدد_glyphs) ثابت
  { أرجع hb_unsigned_mul_saturate (HBUINT8::static_اندازه, عدد_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_اندازه_MIN (0);
};

کينډۍ <ډول_نوم GID_TYPE, ډول_نوم FD_TYPE>
بنية FDSelect3_4_Range
{
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ * /*nullptr*/, بینښه صحيح fdcount) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  لومړی < c->get_عدد_glyphs () && (fd < fdcount));
  }

  GID_TYPE    لومړی;
  FD_TYPE     fd;
  عام:
  DEFINE_اندازه_STATIC (GID_TYPE::static_اندازه + FD_TYPE::static_اندازه);
};

کينډۍ <ډول_نوم GID_TYPE, ډول_نوم FD_TYPE>
بنية FDSelect3_4
{
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کړه_saturate ((size_t) GID_TYPE::static_اندازه * 2, ranges.get_اندازه ()); }

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح fdcount,
                 بینښه صحيح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    ranges.sanitize (c, تش_اشاره, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].لومړی == 0)))
      return_trace (خطأ);

    لكل (بینښه صحيح i = 1; i < nRanges (); i++)
      إذا (unlikely (ranges[i - 1].لومړی >= ranges[i].لومړی))
	return_trace (خطأ);

    إذا (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].لومړی &&
                   sentinel() <= c->get_عدد_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_عدد_glyphs () : min_glyphs))))
      return_trace (خطأ);

    return_trace (ریښتیا);
  }

  ساكن صحيح _cmp_range (ثابت فراغ *کلی_3, ثابت فراغ *توکی_2)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) کلی_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) توکی_2;

    إذا (glyph < range[0].لومړی) أرجع -1;
    إذا (glyph < range[1].لومړی) أرجع 0;
    أرجع +1;
  }

  بینښه get_fd (hb_codepoint_t glyph) ثابت
  {
    خپلکاری *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    أرجع range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<بینښه, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    خپلکاری *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    بینښه fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t پای_ټکی = range ? range[1].لومړی : ranges[nRanges () - 1].لومړی;
    أرجع {fd, پای_ټکی};
  }

  GID_TYPE        &nRanges ()       { أرجع ranges.len; }
  GID_TYPE         nRanges () ثابت { أرجع ranges.len; }
  GID_TYPE       &sentinel ()       { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  Array_د<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_اندازه_ARRAY (GID_TYPE::static_اندازه, ranges);
};

تعريفنوع FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
تعريفنوع FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

بنية FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت FDSelect &src, بینښه صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بینښه صحيح اندازه = src.get_اندازه (عدد_glyphs);
    FDSelect *dest = c->allocate_اندازه<FDSelect> (اندازه, خطأ);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, اندازه);
    return_trace (ریښتیا);
  }

  size_t get_اندازه (بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جمع_کړه_saturate (format.static_اندازه, u.format0.get_اندازه (عدد_glyphs));
    حالة 3: hb_barrier (); أرجع hb_unsigned_جمع_کړه_saturate (format.static_اندازه, u.format3.get_اندازه ());
    افتراضي:أرجع 0;
    }
  }

  بینښه get_fd (hb_codepoint_t glyph) ثابت
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
  hb_pair_t<بینښه, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    إذا (هذا == &Null (FDSelect)) أرجع {0, 1};

    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_fd_range (glyph);
    حالة 3: hb_barrier (); أرجع u.format3.get_fd_range (glyph);
    افتراضي:أرجع {0, 1};
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح fdcount) ثابت
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
  DEFINE_اندازه_MIN (1);
};

کينډۍ <ډول_نوم شمېر_3>
بنية Subrs : CFFIndex<شمېر_3>
{
  تعريفنوع شمېر_3 شمېر_type;
  تعريفنوع CFFIndex<شمېر_3> SUPER;
};

} /* namespace CFF */

#د_که_پای /* HB_OT_CFF_COMMON_HH */
