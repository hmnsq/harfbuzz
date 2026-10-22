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
#اگر_غیر_تعریف_شدہ HB_OT_CFF_COMMON_HH
#تعریف_کرو HB_OT_CFF_COMMON_HH

#شامل "hb-open-type.hh"
#شامل "hb-bimap.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-cff-interp-dict-common.hh"
#شامل "hb-subset-plan.hh"

نامخلا CFF {

استعمال نامخلا OT;

#تعریف_کرو CFF_UNDEF_رمز  0xFFFFFFFF

استعمال objidx_t = hb_serialize_context_t::objidx_t;
استعمال whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
سانچہ<قسم_نام Type, قسم_نام ...Ts>
ساکن اندرونی ثابت Type& Struct_At_انحراف_Or_Null (ثابت خالی *P, صحیح انحراف, hb_sanitize_context_t &sc, Ts&&... ds)
{
  اگر (!انحراف) واپس Null (Type);

  ثابت حرف *p = (ثابت حرف *) P + انحراف;
  اگر (!sc.check_نقطہ (p)) واپس Null (Type);

  ثابت Type &obj = *reinterpret_cast<ثابت Type *> (p);
  اگر (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) واپس Null (Type);

  واپس obj;
}


ساخت رمز_pair_t
{
  غیرعلامتی رمز;
  hb_codepoint_t glyph;
};


استعمال str_buff_t = hb_سمتیہ_t<غیرعلامتی حرف>;
استعمال str_buff_vec_t = hb_سمتیہ_t<str_buff_t>;
استعمال glyph_مقصد_sid_map_t = hb_سمتیہ_t<رمز_pair_t>;

/* Top Dict, Font Dict, Private Dict */
ساخت Dict : UnsizedByteStr
{
  سانچہ <قسم_نام DICTVAL, قسم_نام OP_SERIALIZER, قسم_نام ...Ts>
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (یہ);
    برائے (غیرعلامتی صحیح i = 0; i < dictval.get_گنتی (); i++)
      اگر (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (جھوٹ);

    return_trace (سچ);
  }

  سانچہ <قسم_نام T, قسم_نام V>
  ساکن منطقی serialize_int_op (hb_serialize_context_t *c, op_رمز_t op, V value, op_رمز_t intOp)
  {
    اگر (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      واپس جھوٹ;

    TRACE_SERIALIZE (یہ);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_سائز<HBUINT8> (Op_رمز_سائز (op), جھوٹ);
    اگر (unlikely (!p)) return_trace (جھوٹ);
    اگر (Is_Op_رمز_ESC (op))
    {
      *p = Op_رمز_escape;
      op = Unmake_Op_رمز_ESC (op);
      p++;
    }
    *p = op;
    return_trace (سچ);
  }

  سانچہ <قسم_نام V>
  ساکن منطقی serialize_int4_op (hb_serialize_context_t *c, op_رمز_t op, V value)
  { واپس serialize_int_op<HBINT32> (c, op, value, Op_رمز_longintdict); }

  سانچہ <قسم_نام V>
  ساکن منطقی serialize_int2_op (hb_serialize_context_t *c, op_رمز_t op, V value)
  { واپس serialize_int_op<HBINT16> (c, op, value, Op_رمز_shortint); }

  سانچہ <قسم_نام T, صحیح int_op>
  ساکن منطقی serialize_link_op (hb_serialize_context_t *c, op_رمز_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->سر_2 + Op_رمز_سائز (int_op));
    اگر (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) واپس جھوٹ;
    c->جوڑو_link (ofs, link, whence);
    واپس سچ;
  }

  ساکن منطقی serialize_link4_op (hb_serialize_context_t *c, op_رمز_t op, objidx_t link, whence_t whence = whence_t::سر)
  { واپس serialize_link_op<HBINT32, Op_رمز_longintdict> (c, op, link, whence); }

  ساکن منطقی serialize_link2_op (hb_serialize_context_t *c, op_رمز_t op, objidx_t link, whence_t whence = whence_t::سر)
  { واپس serialize_link_op<HBINT16, Op_رمز_shortint> (c, op, link, whence); }
};

ساخت چوٹی_Dict : Dict {};
ساخت FontDict : Dict {};
ساخت نجی_Dict : Dict {};

ساخت جدول_info_t
{
  خالی init () { انحراف = سائز = 0; link = 0; }

  غیرعلامتی صحیح    انحراف;
  غیرعلامتی صحیح    سائز;
  objidx_t	  link;
};

سانچہ <قسم_نام گنتی_3>
ساخت FDArray : CFFIndex<گنتی_3>
{
  سانچہ <قسم_نام DICTVAL, قسم_نام INFO, قسم_نام Iterator, قسم_نام OP_SERIALIZER>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (یہ);

    /* serialize INDEX data */
    hb_سمتیہ_t<غیرعلامتی> sizes;
    اگر (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    حرف *ڈیٹا_بنیاد = c->سر_2;
    + it
    | hb_map ([&] (ثابت hb_pair_t<ثابت DICTVAL&, ثابت INFO&> &_)
    {
      FontDict *dict = c->آغاز_embed<FontDict> ();
		dict->serialize (c, _.پہلا, opszr, _.second);
		واپس c->سر_2 - (ثابت حرف*)dict;
	      })
    | hb_sink (sizes)
    ;
    غیرعلامتی ڈیٹا_سائز = c->سر_2 - ڈیٹا_بنیاد;
    c->نکالو_pack (جھوٹ);

    اگر (unlikely (sizes.اندر_error ())) return_trace (جھوٹ);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<گنتی_3>::serialize_سرخی (c, hb_iter (sizes), ڈیٹا_سائز));
  }
};

/* FDSelect */
ساخت FDSelect0 {
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح fdcount,
                 غیرعلامتی صحیح glyph_گنتی = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!(c->check_struct (یہ))))
      return_trace (جھوٹ);
    hb_barrier ();
    اگر (unlikely (!c->check_array (fds, glyph_گنتی == HB_CODEPOINT_INVALID ?
                                     c->get_عدد_glyphs () : glyph_گنتی)))
      return_trace (جھوٹ);

    return_trace (سچ);
  }

  غیرعلامتی get_fd (hb_codepoint_t glyph) ثابت
  { واپس fds[glyph]; }

  hb_pair_t<غیرعلامتی, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  { واپس {fds[glyph], glyph + 1}; }

  size_t get_سائز (غیرعلامتی صحیح عدد_glyphs) ثابت
  { واپس hb_unsigned_mul_saturate (HBUINT8::static_سائز, عدد_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_سائز_MIN (0);
};

سانچہ <قسم_نام GID_TYPE, قسم_نام FD_TYPE>
ساخت FDSelect3_4_Range
{
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی * /*nullptr*/, غیرعلامتی صحیح fdcount) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  پہلا < c->get_عدد_glyphs () && (fd < fdcount));
  }

  GID_TYPE    پہلا;
  FD_TYPE     fd;
  عوامی:
  DEFINE_سائز_STATIC (GID_TYPE::static_سائز + FD_TYPE::static_سائز);
};

سانچہ <قسم_نام GID_TYPE, قسم_نام FD_TYPE>
ساخت FDSelect3_4
{
  size_t get_سائز () ثابت
  { واپس hb_unsigned_جوڑو_saturate ((size_t) GID_TYPE::static_سائز * 2, ranges.get_سائز ()); }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح fdcount,
                 غیرعلامتی صحیح min_glyphs = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!(c->check_struct (یہ) &&
		    ranges.sanitize (c, خالی_اشاریہ, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].پہلا == 0)))
      return_trace (جھوٹ);

    برائے (غیرعلامتی صحیح i = 1; i < nRanges (); i++)
      اگر (unlikely (ranges[i - 1].پہلا >= ranges[i].پہلا))
	return_trace (جھوٹ);

    اگر (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].پہلا &&
                   sentinel() <= c->get_عدد_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_عدد_glyphs () : min_glyphs))))
      return_trace (جھوٹ);

    return_trace (سچ);
  }

  ساکن صحیح _cmp_range (ثابت خالی *کلید_3, ثابت خالی *شے_2)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) کلید_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) شے_2;

    اگر (glyph < range[0].پہلا) واپس -1;
    اگر (glyph < range[1].پہلا) واپس 0;
    واپس +1;
  }

  غیرعلامتی get_fd (hb_codepoint_t glyph) ثابت
  {
    خودکار *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    واپس range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<غیرعلامتی, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    خودکار *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, حجم (ranges[0]), _cmp_range);
    غیرعلامتی fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t انتہا = range ? range[1].پہلا : ranges[nRanges () - 1].پہلا;
    واپس {fd, انتہا};
  }

  GID_TYPE        &nRanges ()       { واپس ranges.len; }
  GID_TYPE         nRanges () ثابت { واپس ranges.len; }
  GID_TYPE       &sentinel ()       { واپس StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  ثابت GID_TYPE &sentinel () ثابت { واپس StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  Array_کا<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_سائز_ARRAY (GID_TYPE::static_سائز, ranges);
};

قسمتعریف FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
قسمتعریف FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

ساخت FDSelect
{
  منطقی serialize (hb_serialize_context_t *c, ثابت FDSelect &src, غیرعلامتی صحیح عدد_glyphs)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی صحیح سائز = src.get_سائز (عدد_glyphs);
    FDSelect *dest = c->allocate_سائز<FDSelect> (سائز, جھوٹ);
    اگر (unlikely (!dest)) return_trace (جھوٹ);
    hb_memcpy (dest, &src, سائز);
    return_trace (سچ);
  }

  size_t get_سائز (غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس hb_unsigned_جوڑو_saturate (format.static_سائز, u.format0.get_سائز (عدد_glyphs));
    حالت 3: hb_barrier (); واپس hb_unsigned_جوڑو_saturate (format.static_سائز, u.format3.get_سائز ());
    طےشدہ:واپس 0;
    }
  }

  غیرعلامتی get_fd (hb_codepoint_t glyph) ثابت
  {
    اگر (یہ == &Null (FDSelect)) واپس 0;

    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس u.format0.get_fd (glyph);
    حالت 3: hb_barrier (); واپس u.format3.get_fd (glyph);
    طےشدہ:واپس 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<غیرعلامتی, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) ثابت
  {
    اگر (یہ == &Null (FDSelect)) واپس {0, 1};

    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس u.format0.get_fd_range (glyph);
    حالت 3: hb_barrier (); واپس u.format3.get_fd_range (glyph);
    طےشدہ:واپس {0, 1};
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح fdcount) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  HBUINT8	format;
  اتحاد {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  عوامی:
  DEFINE_سائز_MIN (1);
};

سانچہ <قسم_نام گنتی_3>
ساخت Subrs : CFFIndex<گنتی_3>
{
  قسمتعریف گنتی_3 گنتی_type;
  قسمتعریف CFFIndex<گنتی_3> SUPER;
};

} /* namespace CFF */

#اگر_ختم /* HB_OT_CFF_COMMON_HH */
