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
#جے_بنا_تعریف HB_OT_CFF_COMMON_HH
#تعریف_کرو HB_OT_CFF_COMMON_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-cff-interp-dict-common.hh"
#ضمن "hb-subset-plan.hh"

نطاق CFF {

ورتوں نطاق OT;

#تعریف_کرو CFF_UNDEF_کوڈ  0xFFFFFFFF

ورتوں objidx_t = hb_serialize_context_t::objidx_t;
ورتوں whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
سانچا<قسم_ناں Type, قسم_ناں ...Ts>
ساكن مضمن ثابت Type& Struct_At_وتھ_Or_Null (ثابت فراغ *P, صحيح وتھ, hb_sanitize_context_t &sc, Ts&&... ds)
{
  إذا (!وتھ) أرجع Null (Type);

  ثابت حرف *p = (ثابت حرف *) P + وتھ;
  إذا (!sc.check_نقطہ (p)) أرجع Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  إذا (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) أرجع Null (Type);

  أرجع obj;
}


بنية کوڈ_pair_t
{
  بنانشانی کوڈ;
  hb_codepoint_t glyph;
};


ورتوں str_buff_t = hb_سمتیہ_t<بنانشانی حرف>;
ورتوں str_buff_vec_t = hb_سمتیہ_t<str_buff_t>;
ورتوں glyph_منزل_sid_map_t = hb_سمتیہ_t<کوڈ_pair_t>;

/* Top Dict, Font Dict, Private Dict */
بنية Dict : UnsizedByteStr
{
  سانچا <قسم_ناں DICTVAL, قسم_ناں OP_SERIALIZER, قسم_ناں ...Ts>
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (هذا);
    لكل (بنانشانی صحيح i = 0; i < dictval.get_گنتی (); i++)
      إذا (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (خطأ);

    return_trace (سچ);
  }

  سانچا <قسم_ناں T, قسم_ناں V>
  ساكن منطقي serialize_int_op (hb_serialize_context_t *c, op_کوڈ_t op, V value, op_کوڈ_t intOp)
  {
    إذا (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      أرجع خطأ;

    TRACE_SERIALIZE (هذا);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_ناپ<HBUINT8> (Op_کوڈ_ناپ (op), خطأ);
    إذا (unlikely (!p)) return_trace (خطأ);
    إذا (Is_Op_کوڈ_ESC (op))
    {
      *p = Op_کوڈ_escape;
      op = Unmake_Op_کوڈ_ESC (op);
      p++;
    }
    *p = op;
    return_trace (سچ);
  }

  سانچا <قسم_ناں V>
  ساكن منطقي serialize_int4_op (hb_serialize_context_t *c, op_کوڈ_t op, V value)
  { أرجع serialize_int_op<HBINT32> (c, op, value, Op_کوڈ_longintdict); }

  سانچا <قسم_ناں V>
  ساكن منطقي serialize_int2_op (hb_serialize_context_t *c, op_کوڈ_t op, V value)
  { أرجع serialize_int_op<HBINT16> (c, op, value, Op_کوڈ_shortint); }

  سانچا <قسم_ناں T, صحيح int_op>
  ساكن منطقي serialize_link_op (hb_serialize_context_t *c, op_کوڈ_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->سر_2 + Op_کوڈ_ناپ (int_op));
    إذا (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) أرجع خطأ;
    c->جوڑو_link (ofs, link, whence);
    أرجع سچ;
  }

  ساكن منطقي serialize_link4_op (hb_serialize_context_t *c, op_کوڈ_t op, objidx_t link, whence_t whence = whence_t::سر)
  { أرجع serialize_link_op<HBINT32, Op_کوڈ_longintdict> (c, op, link, whence); }

  ساكن منطقي serialize_link2_op (hb_serialize_context_t *c, op_کوڈ_t op, objidx_t link, whence_t whence = whence_t::سر)
  { أرجع serialize_link_op<HBINT16, Op_کوڈ_shortint> (c, op, link, whence); }
};

بنية چوٹی_Dict : Dict {};
بنية FontDict : Dict {};
بنية نجی_Dict : Dict {};

بنية جدول_info_t
{
  فراغ init () { وتھ = ناپ = 0; link = 0; }

  بنانشانی صحيح    وتھ;
  بنانشانی صحيح    ناپ;
  objidx_t	  link;
};

سانچا <قسم_ناں گنتی_3>
بنية FDArray : CFFIndex<گنتی_3>
{
  سانچا <قسم_ناں DICTVAL, قسم_ناں INFO, قسم_ناں Iterator, قسم_ناں OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (هذا);

    /* serialize INDEX data */
    hb_سمتیہ_t<بنانشانی> sizes;
    إذا (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    حرف *انکڑے_ادھار = c->سر_2;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->شروع_embed<FontDict> ();
		dict->serialize (c, _.پہلا, opszr, _.second);
		أرجع c->سر_2 - (ثابت حرف*)dict;
	      })
    | hb_sink (sizes)
    ;
    بنانشانی انکڑے_ناپ = c->سر_2 - انکڑے_ادھار;
    c->نکالو_pack (خطأ);

    إذا (unlikely (sizes.وچ_error ())) return_trace (خطأ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<گنتی_3>::serialize_سرخی (c, hb_iter (sizes), انکڑے_ناپ));
  }
};

/* FDSelect */
بنية FDSelect0 {
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح fdcount,
                 بنانشانی صحيح glyph_گنتی = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا))))
      return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!c->check_array (fds, glyph_گنتی == HB_CODEPOINT_INVALID ?
                                     c->get_عدد_glyphs () : glyph_گنتی)))
      return_trace (خطأ);

    return_trace (سچ);
  }

  بنانشانی get_fd (hb_codepoint_t glyph) ثابت
  { أرجع fds[glyph]; }

  hb_pair_t<بنانشانی, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { أرجع {fds[glyph], glyph + 1}; }

  size_t get_ناپ (بنانشانی صحيح عدد_glyphs) ثابت
  { أرجع hb_unsigned_mul_saturate (HBUINT8::static_ناپ, عدد_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_ناپ_MIN (0);
};

سانچا <قسم_ناں GID_TYPE, قسم_ناں FD_TYPE>
بنية FDSelect3_4_Range
{
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ * /*nullptr*/, بنانشانی صحيح fdcount) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پہلا < c->get_عدد_glyphs () && (fd < fdcount));
  }

  GID_TYPE    پہلا;
  FD_TYPE     fd;
  عام:
  DEFINE_ناپ_STATIC (GID_TYPE::static_ناپ + FD_TYPE::static_ناپ);
};

سانچا <قسم_ناں GID_TYPE, قسم_ناں FD_TYPE>
بنية FDSelect3_4
{
  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_جوڑو_saturate ((size_t) GID_TYPE::static_ناپ * 2, ranges.get_ناپ ()); }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح fdcount,
                 بنانشانی صحيح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    ranges.sanitize (c, خالی_سوچک, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].پہلا == 0)))
      return_trace (خطأ);

    لكل (بنانشانی صحيح i = 1; i < nRanges (); i++)
      إذا (unlikely (ranges[i - 1].پہلا >= ranges[i].پہلا))
	return_trace (خطأ);

    إذا (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].پہلا &&
                   sentinel() <= c->get_عدد_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_عدد_glyphs () : min_glyphs))))
      return_trace (خطأ);

    return_trace (سچ);
  }

  ساكن صحيح _cmp_range (ثابت فراغ *کلید_3, ثابت فراغ *شے_2)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) کلید_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) شے_2;

    إذا (glyph < range[0].پہلا) أرجع -1;
    إذا (glyph < range[1].پہلا) أرجع 0;
    أرجع +1;
  }

  بنانشانی get_fd (hb_codepoint_t glyph) ثابت
  {
    خودکار *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    أرجع range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<بنانشانی, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    خودکار *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    بنانشانی fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t سرا = range ? range[1].پہلا : ranges[nRanges () - 1].پہلا;
    أرجع {fd, سرا};
  }

  GID_TYPE        &nRanges ()       { أرجع ranges.len; }
  GID_TYPE         nRanges () ثابت { أرجع ranges.len; }
  GID_TYPE       &sentinel ()       { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { أرجع StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  Array_دا<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_ناپ_ARRAY (GID_TYPE::static_ناپ, ranges);
};

تعريفنوع FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
تعريفنوع FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

بنية FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت FDSelect &src, بنانشانی صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی صحيح ناپ = src.get_ناپ (عدد_glyphs);
    FDSelect *dest = c->allocate_ناپ<FDSelect> (ناپ, خطأ);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, ناپ);
    return_trace (سچ);
  }

  size_t get_ناپ (بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع hb_unsigned_جوڑو_saturate (format.static_ناپ, u.format0.get_ناپ (عدد_glyphs));
    حالة 3: hb_barrier (); أرجع hb_unsigned_جوڑو_saturate (format.static_ناپ, u.format3.get_ناپ ());
    افتراضي:أرجع 0;
    }
  }

  بنانشانی get_fd (hb_codepoint_t glyph) ثابت
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
  hb_pair_t<بنانشانی, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    إذا (هذا == &Null (FDSelect)) أرجع {0, 1};

    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_fd_range (glyph);
    حالة 3: hb_barrier (); أرجع u.format3.get_fd_range (glyph);
    افتراضي:أرجع {0, 1};
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح fdcount) ثابت
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
  DEFINE_ناپ_MIN (1);
};

سانچا <قسم_ناں گنتی_3>
بنية Subrs : CFFIndex<گنتی_3>
{
  تعريفنوع گنتی_3 گنتی_type;
  تعريفنوع CFFIndex<گنتی_3> SUPER;
};

} /* namespace CFF */

#جے_انت /* HB_OT_CFF_COMMON_HH */
