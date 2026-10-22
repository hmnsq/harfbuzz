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
#اگر_تعریف_نشده HB_OT_CFF_COMMON_HH
#تعریف_کن HB_OT_CFF_COMMON_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-cff-interp-dict-common.hh"
#ضمن "hb-subset-plan.hh"

نطاق CFF {

استفاده نطاق OT;

#تعریف_کن CFF_UNDEF_کد  0xFFFFFFFF

استفاده objidx_t = hb_serialize_context_t::objidx_t;
استفاده whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
قالب<نام_نوع Type, نام_نوع ...Ts>
ساكن مضمن ثابت Type& Struct_At_جابه_جایی_Or_Null (ثابت فراغ *P, صحيح جابه_جایی, hb_sanitize_context_t &sc, Ts&&... ds)
{
  إذا (!جابه_جایی) أرجع Null (Type);

  ثابت حرف *p = (ثابت حرف *) P + جابه_جایی;
  إذا (!sc.check_نقطه (p)) أرجع Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  إذا (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) أرجع Null (Type);

  أرجع obj;
}


بنية کد_pair_t
{
  بیعلامت کد;
  hb_codepoint_t glyph;
};


استفاده str_buff_t = hb_وکتور_t<بیعلامت حرف>;
استفاده str_buff_vec_t = hb_وکتور_t<str_buff_t>;
استفاده glyph_مقصد_sid_map_t = hb_وکتور_t<کد_pair_t>;

/* Top Dict, Font Dict, Private Dict */
بنية Dict : UnsizedByteStr
{
  قالب <نام_نوع DICTVAL, نام_نوع OP_SERIALIZER, نام_نوع ...Ts>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (هذا);
    لكل (بیعلامت صحيح i = 0; i < dictval.get_شمار (); i++)
      إذا (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (خطأ);

    return_trace (درست);
  }

  قالب <نام_نوع T, نام_نوع V>
  ساكن منطقي serialize_int_op (hb_serialize_context_t *c, op_کد_t op, V value, op_کد_t intOp)
  {
    إذا (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      أرجع خطأ;

    TRACE_SERIALIZE (هذا);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_اندازه<HBUINT8> (Op_کد_اندازه (op), خطأ);
    إذا (unlikely (!p)) return_trace (خطأ);
    إذا (Is_Op_کد_ESC (op))
    {
      *p = Op_کد_escape;
      op = Unmake_Op_کد_ESC (op);
      p++;
    }
    *p = op;
    return_trace (درست);
  }

  قالب <نام_نوع V>
  ساكن منطقي serialize_int4_op (hb_serialize_context_t *c, op_کد_t op, V value)
  { أرجع serialize_int_op<HBINT32> (c, op, value, Op_کد_longintdict); }

  قالب <نام_نوع V>
  ساكن منطقي serialize_int2_op (hb_serialize_context_t *c, op_کد_t op, V value)
  { أرجع serialize_int_op<HBINT16> (c, op, value, Op_کد_shortint); }

  قالب <نام_نوع T, صحيح int_op>
  ساكن منطقي serialize_link_op (hb_serialize_context_t *c, op_کد_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->سر_2 + Op_کد_اندازه (int_op));
    إذا (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) أرجع خطأ;
    c->جمع_کن_link (ofs, link, whence);
    أرجع درست;
  }

  ساكن منطقي serialize_link4_op (hb_serialize_context_t *c, op_کد_t op, objidx_t link, whence_t whence = whence_t::سر)
  { أرجع serialize_link_op<HBINT32, Op_کد_longintdict> (c, op, link, whence); }

  ساكن منطقي serialize_link2_op (hb_serialize_context_t *c, op_کد_t op, objidx_t link, whence_t whence = whence_t::سر)
  { أرجع serialize_link_op<HBINT16, Op_کد_shortint> (c, op, link, whence); }
};

بنية بالاترین_Dict : Dict {};
بنية FontDict : Dict {};
بنية خصوصی_Dict : Dict {};

بنية جدول_info_t
{
  فراغ init () { جابه_جایی = اندازه = 0; link = 0; }

  بیعلامت صحيح    جابه_جایی;
  بیعلامت صحيح    اندازه;
  objidx_t	  link;
};

قالب <نام_نوع شمار_3>
بنية FDArray : CFFIndex<شمار_3>
{
  قالب <نام_نوع DICTVAL, نام_نوع INFO, نام_نوع Iterator, نام_نوع OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (هذا);

    /* serialize INDEX data */
    hb_وکتور_t<بیعلامت> sizes;
    إذا (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    حرف *داده_پایه = c->سر_2;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->شروع_embed<FontDict> ();
		dict->serialize (c, _.اولی, opszr, _.second);
		أرجع c->سر_2 - (ثابت حرف*)dict;
	      })
    | hb_sink (sizes)
    ;
    بیعلامت داده_اندازه = c->سر_2 - داده_پایه;
    c->برداشت_pack (خطأ);

    إذا (unlikely (sizes.درون_error ())) return_trace (خطأ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<شمار_3>::serialize_سرآیند (c, hb_iter (sizes), داده_اندازه));
  }
};

/* FDSelect */
بنية FDSelect0 {
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح fdcount,
                 بیعلامت صحيح glyph_شمار = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا))))
      return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!c->check_array (fds, glyph_شمار == HB_CODEPOINT_INVALID ?
                                     c->get_رقم_glyphs () : glyph_شمار)))
      return_trace (خطأ);

    return_trace (درست);
  }

  بیعلامت get_fd (hb_codepoint_t glyph) ثابت
  { أرجع fds[glyph]; }

  hb_pair_t<بیعلامت, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { أرجع {fds[glyph], glyph + 1}; }

  size_t get_اندازه (بیعلامت صحيح رقم_glyphs) ثابت
  { أرجع hb_unsigned_mul_saturate (HBUINT8::static_اندازه, رقم_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_اندازه_MIN (0);
};

قالب <نام_نوع GID_TYPE, نام_نوع FD_TYPE>
بنية FDSelect3_4_Range
{
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ * /*nullptr*/, بیعلامت صحيح fdcount) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  اولی < c->get_رقم_glyphs () && (fd < fdcount));
  }

  GID_TYPE    اولی;
  FD_TYPE     fd;
  عام:
  DEFINE_اندازه_STATIC (GID_TYPE::static_اندازه + FD_TYPE::static_اندازه);
};

قالب <نام_نوع GID_TYPE, نام_نوع FD_TYPE>
بنية FDSelect3_4
{
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کن_saturate ((size_t) GID_TYPE::static_اندازه * 2, ranges.get_اندازه ()); }

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح fdcount,
                 بیعلامت صحيح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    ranges.sanitize (c, اشاره_تهی, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].اولی == 0)))
      return_trace (خطأ);

    لكل (بیعلامت صحيح i = 1; i < nRanges (); i++)
      إذا (unlikely (ranges[i - 1].اولی >= ranges[i].اولی))
	return_trace (خطأ);

    إذا (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].اولی &&
                   sentinel() <= c->get_رقم_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_رقم_glyphs () : min_glyphs))))
      return_trace (خطأ);

    return_trace (درست);
  }

  ساكن صحيح _cmp_range (ثابت فراغ *کلید_3, ثابت فراغ *قلم_2)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) کلید_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) قلم_2;

    إذا (glyph < range[0].اولی) أرجع -1;
    إذا (glyph < range[1].اولی) أرجع 0;
    أرجع +1;
  }

  بیعلامت get_fd (hb_codepoint_t glyph) ثابت
  {
    خودکار *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    أرجع range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<بیعلامت, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    خودکار *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    بیعلامت fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t انتها = range ? range[1].اولی : ranges[nRanges () - 1].اولی;
    أرجع {fd, انتها};
  }

  GID_TYPE        &nRanges ()       { أرجع ranges.len; }
  GID_TYPE         nRanges () ثابت { أرجع ranges.len; }
  GID_TYPE       &sentinel ()       { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  Array_از<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_اندازه_ARRAY (GID_TYPE::static_اندازه, ranges);
};

تعريفنوع FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
تعريفنوع FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

بنية FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت FDSelect &src, بیعلامت صحيح رقم_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت صحيح اندازه = src.get_اندازه (رقم_glyphs);
    FDSelect *dest = c->allocate_اندازه<FDSelect> (اندازه, خطأ);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, اندازه);
    return_trace (درست);
  }

  size_t get_اندازه (بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جمع_کن_saturate (format.static_اندازه, u.format0.get_اندازه (رقم_glyphs));
    حالة 3: hb_barrier (); أرجع hb_unsigned_جمع_کن_saturate (format.static_اندازه, u.format3.get_اندازه ());
    افتراضي:أرجع 0;
    }
  }

  بیعلامت get_fd (hb_codepoint_t glyph) ثابت
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
  hb_pair_t<بیعلامت, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    إذا (هذا == &Null (FDSelect)) أرجع {0, 1};

    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_fd_range (glyph);
    حالة 3: hb_barrier (); أرجع u.format3.get_fd_range (glyph);
    افتراضي:أرجع {0, 1};
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح fdcount) ثابت
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

قالب <نام_نوع شمار_3>
بنية Subrs : CFFIndex<شمار_3>
{
  تعريفنوع شمار_3 شمار_type;
  تعريفنوع CFFIndex<شمار_3> SUPER;
};

} /* namespace CFF */

#پایان_اگر /* HB_OT_CFF_COMMON_HH */
