/*
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ OT_NAME_NAME_HH
#تعریف_کرو OT_NAME_NAME_HH

#شامل "../../hb-open-type.hh"
#شامل "../../hb-ot-name-language.hh"
#شامل "../../hb-aat-layout.hh"
#شامل "../../hb-utf.hh"


نامخلا OT {

سانچہ <قسم_نام اندر_utf_t, قسم_نام برآمد_utf_t>
اندرونی غیرعلامتی صحیح
hb_ot_name_convert_utf (hb_بائٹیں_t                       بائٹیں,
			غیرعلامتی صحیح                    *متن_سائز /* IN/OUT */,
			قسم_نام برآمد_utf_t::codepoint_t *متن /*برآمد*/)
{
  غیرعلامتی صحیح src_len = بائٹیں.لمبائی / حجم (قسم_نام اندر_utf_t::codepoint_t);
  ثابت قسم_نام اندر_utf_t::codepoint_t *src = (ثابت قسم_نام اندر_utf_t::codepoint_t *) بائٹیں.arrayZ;
  ثابت قسم_نام اندر_utf_t::codepoint_t *src_انتہا = src + src_len;

  قسم_نام برآمد_utf_t::codepoint_t *dst = متن;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;

  اگر (متن_سائز && *متن_سائز)
  {
    (*متن_سائز)--; /* Save room for NUL-termination. */
    ثابت قسم_نام برآمد_utf_t::codepoint_t *dst_انتہا = متن + *متن_سائز;

    جب (src < src_انتہا && dst < dst_انتہا)
    {
      ثابت قسم_نام اندر_utf_t::codepoint_t *src_اگلا = اندر_utf_t::next (src, src_انتہا, &unicode, replacement);
      قسم_نام برآمد_utf_t::codepoint_t *dst_اگلا = برآمد_utf_t::encode (dst, dst_انتہا, unicode);
      اگر (dst_اگلا == dst)
	توڑو; /* Out-of-room. */

      dst = dst_اگلا;
      src = src_اگلا;
    }

    *متن_سائز = dst - متن;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  غیرعلامتی صحیح dst_len = dst - متن;
  جب (src < src_انتہا)
  {
    src = اندر_utf_t::next (src, src_انتہا, &unicode, replacement);
    dst_len += برآمد_utf_t::encode_len (unicode);
  }
  واپس dst_len;
}

#تعریف_کرو اندراج_score var.u16[0]
#تعریف_کرو اندراج_اشاریہ var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#تعریف_کرو HB_OT_لیبل_name HB_لیبل('n','a','m','e')

#تعریف_کرو UNSUPPORTED	42

ساخت Name_ریکارڈ
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_OT_NAME_LANGUAGE
    غیرعلامتی صحیح p = platformID;
    غیرعلامتی صحیح l = languageID;

    اگر (p == 3)
      واپس hb_ot_name_language_for_ms_رمز (l);

    اگر (p == 1)
      واپس hb_ot_name_language_for_mac_رمز (l);

#اگر_غیر_تعریف_شدہ HB_NO_OT_NAME_LANGUAGE_AAT
    اگر (p == 0)
      واپس face->جدول.ltag->get_language (l);
#اگر_ختم

#اگر_ختم
    واپس HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    غیرعلامتی صحیح p = platformID;
    غیرعلامتی صحیح e = encodingID;

    /* 32-bit. */
    اگر (p == 3 && e == 10) واپس 0;
    اگر (p == 0 && e ==  6) واپس 1;
    اگر (p == 0 && e ==  4) واپس 2;

    /* 16-bit. */
    اگر (p == 3 && e ==  1) واپس 3;
    اگر (p == 0 && e ==  3) واپس 4;
    اگر (p == 0 && e ==  2) واپس 5;
    اگر (p == 0 && e ==  1) واپس 6;
    اگر (p == 0 && e ==  0) واپس 7;

    /* Symbol. */
    اگر (p == 3 && e ==  0) واپس 8;

    /* We treat all Mac Latin names as ASCII only. */
    اگر (p == 1 && e ==  0) واپس 10; /* 10 is magic number :| */

    واپس UNSUPPORTED;
  }

  Name_ریکارڈ* copy (hb_serialize_context_t *c, ثابت خالی *بنیاد
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹیں_t> *name_جدول_overrides
#اگر_ختم
		    ) ثابت
  {
    TRACE_SERIALIZE (یہ);
    HB_غیر_مستعمل خودکار snap = c->snapshot ();
    خودکار *برآمد = c->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (خالی_اشاریہ);
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    hb_ot_name_ریکارڈ_ids_t ریکارڈ_ids (platformID, encodingID, languageID, nameID);
    hb_بائٹیں_t* name_بائٹیں;

    اگر (name_جدول_overrides->has (ریکارڈ_ids, &name_بائٹیں)) {
      hb_بائٹیں_t encoded_بائٹیں = *name_بائٹیں;
      حرف *name_str_utf16_be = خالی_اشاریہ;

      اگر (platformID != 1)
      {
        غیرعلامتی متن_سائز = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بائٹیں, خالی_اشاریہ, خالی_اشاریہ);

        متن_سائز++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        غیرعلامتی byte_len = متن_سائز * hb_utf16_be_t::codepoint_t::static_سائز;
        name_str_utf16_be = (حرف *) hb_calloc (byte_len, 1);
        اگر (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (خالی_اشاریہ);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بائٹیں, &متن_سائز,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        غیرعلامتی encoded_byte_len = متن_سائز * hb_utf16_be_t::codepoint_t::static_سائز;
        اگر (!encoded_byte_len || !c->check_تفویض (برآمد->لمبائی, encoded_byte_len, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (خالی_اشاریہ);
        }

        encoded_بائٹیں = hb_بائٹیں_t (name_str_utf16_be, encoded_byte_len);
      }
      ورنہ
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        اگر (!c->check_تفویض (برآمد->لمبائی, encoded_بائٹیں.لمبائی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) {
          c->revert (snap);
          return_trace (خالی_اشاریہ);
        }
      }

      برآمد->انحراف = 0;
      c->push ();
      encoded_بائٹیں.copy (c);
      c->جوڑو_link (برآمد->انحراف, c->نکالو_pack (), hb_serialize_context_t::دم_2, 0);
      hb_free (name_str_utf16_be);
    }
    ورنہ
#اگر_ختم
    {
      برآمد->انحراف.serialize_copy (c, انحراف, بنیاد, 0, hb_serialize_context_t::دم_2, لمبائی);
    }
    return_trace (برآمد);
  }

  منطقی isUnicode () ثابت
  {
    غیرعلامتی صحیح p = platformID;
    غیرعلامتی صحیح e = encodingID;

    واپس (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت Name_ریکارڈ *a = (ثابت Name_ریکارڈ *)pa;
    ثابت Name_ریکارڈ *b = (ثابت Name_ریکارڈ *)pb;

    اگر (a->platformID != b->platformID)
      واپس a->platformID - b->platformID;

    اگر (a->encodingID != b->encodingID)
      واپس a->encodingID - b->encodingID;

    اگر (a->languageID != b->languageID)
      واپس a->languageID - b->languageID;

    اگر (a->nameID != b->nameID)
      واپس a->nameID - b->nameID;

    اگر (a->لمبائی != b->لمبائی)
      واپس a->لمبائی - b->لمبائی;

    واپس 0;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  انحراف.sanitize (c, بنیاد, لمبائی));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	لمبائی;		/* String length (in bytes). */
  NNOffset16_مقصد<Unsized_Array_کا<HBUINT8>>
		انحراف;		/* String offset from start of storage area (in bytes). */
  عوامی:
  DEFINE_سائز_STATIC (12);
};

ساکن صحیح
hb_ot_name_اندراج_cmp_کلید (ثابت خالی *pa, ثابت خالی *pb, منطقی exact)
{
  ثابت hb_ot_name_اندراج_t *a = (ثابت hb_ot_name_اندراج_t *) pa;
  ثابت hb_ot_name_اندراج_t *b = (ثابت hb_ot_name_اندراج_t *) pb;

  /* Compare by name_id, then language. */

  اگر (a->name_id != b->name_id)
    واپس a->name_id - b->name_id;

  اگر (a->language == b->language) واپس 0;
  اگر (!a->language) واپس -1;
  اگر (!b->language) واپس +1;

  ثابت حرف *astr = hb_language_مقصد_string (a->language);
  ثابت حرف *bstr = hb_language_مقصد_string (b->language);

  علامتی c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  اگر (!exact && c &&
      hb_language_matches (b->language, a->language))
    واپس 0;

  واپس c;
}

ساکن صحیح
hb_ot_name_اندراج_cmp (ثابت خالی *pa, ثابت خالی *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحیح v = hb_ot_name_اندراج_cmp_کلید (pa, pb, سچ);
  اگر (v)
    واپس v;

  ثابت hb_ot_name_اندراج_t *a = (ثابت hb_ot_name_اندراج_t *) pa;
  ثابت hb_ot_name_اندراج_t *b = (ثابت hb_ot_name_اندراج_t *) pb;

  اگر (a->اندراج_score != b->اندراج_score)
    واپس a->اندراج_score - b->اندراج_score;

  اگر (a->اندراج_اشاریہ != b->اندراج_اشاریہ)
    واپس a->اندراج_اشاریہ - b->اندراج_اشاریہ;

  واپس 0;
}

ساخت name
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_name;

  size_t get_سائز () ثابت
  { واپس min_سائز + گنتی * name_ریکارڈ_Z.شے_سائز; }

  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_ماخذ_کا (Iterator, ثابت Name_ریکارڈ &))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت خالی *src_string_pool
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
                  , ثابت hb_سمتیہ_t<hb_ot_name_ریکارڈ_ids_t>& داخل_کرو_name_اندراجات
		  , ثابت hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹیں_t> *name_جدول_overrides
#اگر_ختم
		  )
  {
    TRACE_SERIALIZE (یہ);

    اگر (unlikely (!c->extend_min ((*یہ))))  return_trace (جھوٹ);

    غیرعلامتی کل_گنتی = it.len ()
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
        + داخل_کرو_name_اندراجات.لمبائی
#اگر_ختم
        ;
    یہ->format = 0;
    اگر (!c->check_تفویض (یہ->گنتی, کل_گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      واپس جھوٹ;

    Name_ریکارڈ *name_اندراجات = (Name_ریکارڈ *) hb_calloc (کل_گنتی, Name_ریکارڈ::static_سائز);
    اگر (unlikely (!name_اندراجات)) return_trace (جھوٹ);

    hb_array_t<Name_ریکارڈ> اندراجات (name_اندراجات, کل_گنتی);

    برائے (ثابت Name_ریکارڈ& ریکارڈ : it)
    {
      hb_memcpy (name_اندراجات, &ریکارڈ, Name_ریکارڈ::static_سائز);
      name_اندراجات++;
    }

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    برائے (غیرعلامتی i = 0; i < داخل_کرو_name_اندراجات.لمبائی; i++)
    {
      ثابت hb_ot_name_ریکارڈ_ids_t& ids = داخل_کرو_name_اندراجات[i];
      Name_ریکارڈ ریکارڈ;
      ریکارڈ.platformID = ids.platform_id;
      ریکارڈ.encodingID = ids.encoding_id;
      ریکارڈ.languageID = ids.language_id;
      ریکارڈ.nameID = ids.name_id;
      ریکارڈ.لمبائی = 0; // handled in NameRecord copy()
      ریکارڈ.انحراف = 0;
      hb_memcpy (name_اندراجات, &ریکارڈ, Name_ریکارڈ::static_سائز);
      name_اندراجات++;
    }
#اگر_ختم

    اندراجات.qsort ();

    c->copy_all (اندراجات,
		 src_string_pool
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
		 , name_جدول_overrides
#اگر_ختم
		 );
    hb_free (اندراجات.arrayZ);


    اگر (unlikely (c->ran_برآمد_کا_گنجائش ())) return_trace (جھوٹ);

    یہ->string_انحراف = c->لمبائی ();

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    خودکار *name_مفرد = c->serializer->آغاز_embed<name> ();

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹیں_t> *name_جدول_overrides =
        &c->plan->name_جدول_overrides;
#اگر_ختم

    خودکار it =
    + name_ریکارڈ_Z.as_array (گنتی)
    | hb_filter (c->plan->name_ids, &Name_ریکارڈ::nameID)
    | hb_filter (c->plan->name_languages, &Name_ریکارڈ::languageID)
    | hb_filter ([&] (ثابت Name_ریکارڈ& namerecord) {
      واپس
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت Name_ریکارڈ& namerecord) {
      اگر (name_جدول_overrides->is_empty ())
        واپس سچ;
      hb_ot_name_ریکارڈ_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_بائٹیں_t *p;
      اگر (name_جدول_overrides->has (rec_ids, &p) &&
          (*p).لمبائی == 0)
        واپس جھوٹ;
      واپس سچ;
    })
#اگر_ختم
    ;

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, غیرعلامتی> retained_name_ریکارڈ_ids;
    برائے (ثابت Name_ریکارڈ& rec : it)
    {
      hb_ot_name_ریکارڈ_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_ریکارڈ_ids.سیٹ (rec_ids, 1);
    }

    hb_سمتیہ_t<hb_ot_name_ریکارڈ_ids_t> داخل_کرو_name_اندراجات;
    اگر (!name_جدول_overrides->is_empty ())
    {
      اگر (unlikely (!داخل_کرو_name_اندراجات.alloc (name_جدول_overrides->get_population (), سچ)))
        واپس جھوٹ;
      برائے (ثابت خودکار& ریکارڈ_ids : name_جدول_overrides->کلیدیں ())
      {
        اگر (name_جدول_overrides->get (ریکارڈ_ids).لمبائی == 0)
          جاری;
        اگر (retained_name_ریکارڈ_ids.has (ریکارڈ_ids))
          جاری;
        داخل_کرو_name_اندراجات.push (ریکارڈ_ids);
      }
    }
#اگر_ختم

    واپس name_مفرد->serialize (c->serializer, it,
				  std::addressof (یہ + string_انحراف)
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
				  , داخل_کرو_name_اندراجات
				  , name_جدول_overrides
#اگر_ختم
				  );
  }

  منطقی sanitize_اندراجات (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    ثابت خالی *string_pool = (یہ+string_انحراف).arrayZ;
    return_trace (name_ریکارڈ_Z.sanitize (c, گنتی, string_pool));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_ریکارڈ_Z.arrayZ, گنتی) &&
		  c->check_range (یہ, string_انحراف) &&
		  sanitize_اندراجات (c));
  }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      یہ->جدول = hb_sanitize_context_t ().reference_جدول<name> (face);
      أكد (یہ->جدول.get_لمبائی () >= یہ->جدول->string_انحراف);
      یہ->pool = (ثابت حرف *) (ثابت خالی *) (یہ->جدول+یہ->جدول->string_انحراف);
      یہ->pool_len = یہ->جدول.get_لمبائی () - یہ->جدول->string_انحراف;
      ثابت hb_array_t<ثابت Name_ریکارڈ> all_نام (یہ->جدول->name_ریکارڈ_Z.arrayZ,
						    یہ->جدول->گنتی);

      یہ->نام.alloc_exact (all_نام.لمبائی);

      برائے (غیرعلامتی صحیح i = 0; i < all_نام.لمبائی; i++)
      {
	hb_ot_name_اندراج_t *اندراج_2 = یہ->نام.push ();

	اندراج_2->name_id = all_نام[i].nameID;
	اندراج_2->language = all_نام[i].language (face);
	اندراج_2->اندراج_score =  all_نام[i].score ();
	اندراج_2->اندراج_اشاریہ = i;
      }

      یہ->نام.qsort (hb_ot_name_اندراج_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      غیرعلامتی صحیح j = 0;
      برائے (غیرعلامتی صحیح i = 0; i < یہ->نام.لمبائی; i++)
      {
	اگر (یہ->نام[i].اندراج_score == UNSUPPORTED ||
	    یہ->نام[i].language == HB_LANGUAGE_INVALID)
	  جاری;
	اگر (i &&
	    یہ->نام[i - 1].name_id  == یہ->نام[i].name_id &&
	    یہ->نام[i - 1].language == یہ->نام[i].language)
	  جاری;
	یہ->نام[j++] = یہ->نام[i];
      }
      یہ->نام.resize (j);
    }
    ~accelerator_t ()
    {
      یہ->جدول.destroy ();
    }

    صحیح get_اشاریہ (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   غیرعلامتی صحیح    *چوڑائی=خالی_اشاریہ) ثابت
    {
      ثابت hb_ot_name_اندراج_t key = {name_id, {0}, language};
      ثابت hb_ot_name_اندراج_t *اندراج_2 = hb_bsearch (key, (ثابت hb_ot_name_اندراج_t *) یہ->نام,
						    یہ->نام.لمبائی,
						    حجم (hb_ot_name_اندراج_t),
						    hb_ot_name_اندراج_cmp_کلید,
						    سچ);

      اگر (!اندراج_2)
      {
	اندراج_2 = hb_bsearch (key, (ثابت hb_ot_name_اندراج_t *) یہ->نام,
			    یہ->نام.لمبائی,
			    حجم (hb_ot_name_اندراج_t),
			    hb_ot_name_اندراج_cmp_کلید,
			    جھوٹ);
      }

      اگر (!اندراج_2)
	واپس -1;

      اگر (چوڑائی)
	*چوڑائی = اندراج_2->اندراج_score < 10 ? 2 : 1;

      واپس اندراج_2->اندراج_اشاریہ;
    }

    hb_بائٹیں_t get_name (غیرعلامتی صحیح idx) ثابت
    {
      ثابت hb_array_t<ثابت Name_ریکارڈ> all_نام (جدول->name_ریکارڈ_Z.arrayZ, جدول->گنتی);
      ثابت Name_ریکارڈ &ریکارڈ = all_نام[idx];
      ثابت hb_بائٹیں_t string_pool (pool, pool_len);
      واپس string_pool.sub_array (ریکارڈ.انحراف, ریکارڈ.لمبائی);
    }

    نجی:
    ثابت حرف *pool;
    غیرعلامتی صحیح pool_len;
    عوامی:
    hb_blob_ptr_t<name> جدول;
    hb_سمتیہ_t<hb_ot_name_اندراج_t> نام;
  };

  عوامی:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	گنتی;		/* Number of name records. */
  NNOffset16_مقصد<Unsized_Array_کا<HBUINT8>>
		string_انحراف;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_کا<Name_ریکارڈ>
		name_ریکارڈ_Z;	/* The name records where count is the number of records. */
  عوامی:
  DEFINE_سائز_ARRAY (6, name_ریکارڈ_Z);
};

#تعریف_منسوخ اندراج_اشاریہ
#تعریف_منسوخ اندراج_score

ساخت name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#اگر_ختم /* OT_NAME_NAME_HH */
