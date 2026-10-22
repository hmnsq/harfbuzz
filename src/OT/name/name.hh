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

#اگر_تعریف_نشده OT_NAME_NAME_HH
#تعریف_کن OT_NAME_NAME_HH

#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-ot-name-language.hh"
#ضمن "../../hb-aat-layout.hh"
#ضمن "../../hb-utf.hh"


نطاق OT {

قالب <نام_نوع درون_utf_t, نام_نوع بیرون_utf_t>
مضمن بیعلامت صحيح
hb_ot_name_convert_utf (hb_بایتها_t                       بایتها,
			بیعلامت صحيح                    *متن_اندازه /* IN/OUT */,
			نام_نوع بیرون_utf_t::codepoint_t *متن /*بیرون*/)
{
  بیعلامت صحيح src_len = بایتها.درازی / حجم (نام_نوع درون_utf_t::codepoint_t);
  ثابت نام_نوع درون_utf_t::codepoint_t *src = (ثابت نام_نوع درون_utf_t::codepoint_t *) بایتها.arrayZ;
  ثابت نام_نوع درون_utf_t::codepoint_t *src_انتها = src + src_len;

  نام_نوع بیرون_utf_t::codepoint_t *dst = متن;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_میانگیر_REPLACEMENT_CODEPOINT_DEFAULT;

  إذا (متن_اندازه && *متن_اندازه)
  {
    (*متن_اندازه)--; /* Save room for NUL-termination. */
    ثابت نام_نوع بیرون_utf_t::codepoint_t *dst_انتها = متن + *متن_اندازه;

    طالما (src < src_انتها && dst < dst_انتها)
    {
      ثابت نام_نوع درون_utf_t::codepoint_t *src_بعدی = درون_utf_t::next (src, src_انتها, &unicode, replacement);
      نام_نوع بیرون_utf_t::codepoint_t *dst_بعدی = بیرون_utf_t::encode (dst, dst_انتها, unicode);
      إذا (dst_بعدی == dst)
	اكسر; /* Out-of-room. */

      dst = dst_بعدی;
      src = src_بعدی;
    }

    *متن_اندازه = dst - متن;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  بیعلامت صحيح dst_len = dst - متن;
  طالما (src < src_انتها)
  {
    src = درون_utf_t::next (src, src_انتها, &unicode, replacement);
    dst_len += بیرون_utf_t::encode_len (unicode);
  }
  أرجع dst_len;
}

#تعریف_کن مدخل_score var.u16[0]
#تعریف_کن مدخل_شاخص var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#تعریف_کن HB_OT_برچسب_name HB_برچسب('n','a','m','e')

#تعریف_کن UNSUPPORTED	42

بنية Name_رکورد
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#اگر_تعریف_نشده HB_NO_OT_NAME_LANGUAGE
    بیعلامت صحيح p = platformID;
    بیعلامت صحيح l = languageID;

    إذا (p == 3)
      أرجع hb_ot_name_language_for_ms_کد (l);

    إذا (p == 1)
      أرجع hb_ot_name_language_for_mac_کد (l);

#اگر_تعریف_نشده HB_NO_OT_NAME_LANGUAGE_AAT
    إذا (p == 0)
      أرجع face->جدول.ltag->get_language (l);
#پایان_اگر

#پایان_اگر
    أرجع HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    بیعلامت صحيح p = platformID;
    بیعلامت صحيح e = encodingID;

    /* 32-bit. */
    إذا (p == 3 && e == 10) أرجع 0;
    إذا (p == 0 && e ==  6) أرجع 1;
    إذا (p == 0 && e ==  4) أرجع 2;

    /* 16-bit. */
    إذا (p == 3 && e ==  1) أرجع 3;
    إذا (p == 0 && e ==  3) أرجع 4;
    إذا (p == 0 && e ==  2) أرجع 5;
    إذا (p == 0 && e ==  1) أرجع 6;
    إذا (p == 0 && e ==  0) أرجع 7;

    /* Symbol. */
    إذا (p == 3 && e ==  0) أرجع 8;

    /* We treat all Mac Latin names as ASCII only. */
    إذا (p == 1 && e ==  0) أرجع 10; /* 10 is magic number :| */

    أرجع UNSUPPORTED;
  }

  Name_رکورد* copy (hb_serialize_context_t *c, ثابت فراغ *پایه
#اگر_تعریف_شده HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_رکورد_ids_t, hb_بایتها_t> *name_جدول_overrides
#پایان_اگر
		    ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    HB_استفادهنشده خودکار snap = c->snapshot ();
    خودکار *بیرون = c->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (اشاره_تهی);
#اگر_تعریف_شده HB_EXPERIMENTAL_API
    hb_ot_name_رکورد_ids_t رکورد_ids (platformID, encodingID, languageID, nameID);
    hb_بایتها_t* name_بایتها;

    إذا (name_جدول_overrides->has (رکورد_ids, &name_بایتها)) {
      hb_بایتها_t encoded_بایتها = *name_بایتها;
      حرف *name_str_utf16_be = اشاره_تهی;

      إذا (platformID != 1)
      {
        بیعلامت متن_اندازه = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بایتها, اشاره_تهی, اشاره_تهی);

        متن_اندازه++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        بیعلامت byte_len = متن_اندازه * hb_utf16_be_t::codepoint_t::static_اندازه;
        name_str_utf16_be = (حرف *) hb_calloc (byte_len, 1);
        إذا (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (اشاره_تهی);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بایتها, &متن_اندازه,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        بیعلامت encoded_byte_len = متن_اندازه * hb_utf16_be_t::codepoint_t::static_اندازه;
        إذا (!encoded_byte_len || !c->check_تخصیص_بده (بیرون->درازی, encoded_byte_len, HB_SERIALIZE_ERROR_INT_سرریز)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (اشاره_تهی);
        }

        encoded_بایتها = hb_بایتها_t (name_str_utf16_be, encoded_byte_len);
      }
      وإلا
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        إذا (!c->check_تخصیص_بده (بیرون->درازی, encoded_بایتها.درازی, HB_SERIALIZE_ERROR_INT_سرریز)) {
          c->revert (snap);
          return_trace (اشاره_تهی);
        }
      }

      بیرون->جابه_جایی = 0;
      c->push ();
      encoded_بایتها.copy (c);
      c->جمع_کن_link (بیرون->جابه_جایی, c->برداشت_pack (), hb_serialize_context_t::دم_2, 0);
      hb_free (name_str_utf16_be);
    }
    وإلا
#پایان_اگر
    {
      بیرون->جابه_جایی.serialize_copy (c, جابه_جایی, پایه, 0, hb_serialize_context_t::دم_2, درازی);
    }
    return_trace (بیرون);
  }

  منطقي isUnicode () ثابت
  {
    بیعلامت صحيح p = platformID;
    بیعلامت صحيح e = encodingID;

    أرجع (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت Name_رکورد *a = (ثابت Name_رکورد *)pa;
    ثابت Name_رکورد *b = (ثابت Name_رکورد *)pb;

    إذا (a->platformID != b->platformID)
      أرجع a->platformID - b->platformID;

    إذا (a->encodingID != b->encodingID)
      أرجع a->encodingID - b->encodingID;

    إذا (a->languageID != b->languageID)
      أرجع a->languageID - b->languageID;

    إذا (a->nameID != b->nameID)
      أرجع a->nameID - b->nameID;

    إذا (a->درازی != b->درازی)
      أرجع a->درازی - b->درازی;

    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  جابه_جایی.sanitize (c, پایه, درازی));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	درازی;		/* String length (in bytes). */
  NNOffset16_مقصد<Unsized_Array_از<HBUINT8>>
		جابه_جایی;		/* String offset from start of storage area (in bytes). */
  عام:
  DEFINE_اندازه_STATIC (12);
};

ساكن صحيح
hb_ot_name_مدخل_cmp_کلید (ثابت فراغ *pa, ثابت فراغ *pb, منطقي exact)
{
  ثابت hb_ot_name_مدخل_t *a = (ثابت hb_ot_name_مدخل_t *) pa;
  ثابت hb_ot_name_مدخل_t *b = (ثابت hb_ot_name_مدخل_t *) pb;

  /* Compare by name_id, then language. */

  إذا (a->name_id != b->name_id)
    أرجع a->name_id - b->name_id;

  إذا (a->language == b->language) أرجع 0;
  إذا (!a->language) أرجع -1;
  إذا (!b->language) أرجع +1;

  ثابت حرف *astr = hb_language_مقصد_string (a->language);
  ثابت حرف *bstr = hb_language_مقصد_string (b->language);

  علامتدار c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  إذا (!exact && c &&
      hb_language_matches (b->language, a->language))
    أرجع 0;

  أرجع c;
}

ساكن صحيح
hb_ot_name_مدخل_cmp (ثابت فراغ *pa, ثابت فراغ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحيح v = hb_ot_name_مدخل_cmp_کلید (pa, pb, درست);
  إذا (v)
    أرجع v;

  ثابت hb_ot_name_مدخل_t *a = (ثابت hb_ot_name_مدخل_t *) pa;
  ثابت hb_ot_name_مدخل_t *b = (ثابت hb_ot_name_مدخل_t *) pb;

  إذا (a->مدخل_score != b->مدخل_score)
    أرجع a->مدخل_score - b->مدخل_score;

  إذا (a->مدخل_شاخص != b->مدخل_شاخص)
    أرجع a->مدخل_شاخص - b->مدخل_شاخص;

  أرجع 0;
}

بنية name
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_name;

  size_t get_اندازه () ثابت
  { أرجع min_اندازه + شمار * name_رکورد_Z.قلم_اندازه; }

  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_منبع_از (Iterator, ثابت Name_رکورد &))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت فراغ *src_string_pool
#اگر_تعریف_شده HB_EXPERIMENTAL_API
                  , ثابت hb_وکتور_t<hb_ot_name_رکورد_ids_t>& درج_name_رکوردها
		  , ثابت hb_hashmap_t<hb_ot_name_رکورد_ids_t, hb_بایتها_t> *name_جدول_overrides
#پایان_اگر
		  )
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    بیعلامت مجموع_شمار = it.len ()
#اگر_تعریف_شده HB_EXPERIMENTAL_API
        + درج_name_رکوردها.درازی
#پایان_اگر
        ;
    هذا->format = 0;
    إذا (!c->check_تخصیص_بده (هذا->شمار, مجموع_شمار, HB_SERIALIZE_ERROR_INT_سرریز))
      أرجع خطأ;

    Name_رکورد *name_رکوردها = (Name_رکورد *) hb_calloc (مجموع_شمار, Name_رکورد::static_اندازه);
    إذا (unlikely (!name_رکوردها)) return_trace (خطأ);

    hb_array_t<Name_رکورد> رکوردها (name_رکوردها, مجموع_شمار);

    لكل (ثابت Name_رکورد& رکورد : it)
    {
      hb_memcpy (name_رکوردها, &رکورد, Name_رکورد::static_اندازه);
      name_رکوردها++;
    }

#اگر_تعریف_شده HB_EXPERIMENTAL_API
    لكل (بیعلامت i = 0; i < درج_name_رکوردها.درازی; i++)
    {
      ثابت hb_ot_name_رکورد_ids_t& ids = درج_name_رکوردها[i];
      Name_رکورد رکورد;
      رکورد.platformID = ids.platform_id;
      رکورد.encodingID = ids.encoding_id;
      رکورد.languageID = ids.language_id;
      رکورد.nameID = ids.name_id;
      رکورد.درازی = 0; // handled in NameRecord copy()
      رکورد.جابه_جایی = 0;
      hb_memcpy (name_رکوردها, &رکورد, Name_رکورد::static_اندازه);
      name_رکوردها++;
    }
#پایان_اگر

    رکوردها.qsort ();

    c->copy_all (رکوردها,
		 src_string_pool
#اگر_تعریف_شده HB_EXPERIMENTAL_API
		 , name_جدول_overrides
#پایان_اگر
		 );
    hb_free (رکوردها.arrayZ);


    إذا (unlikely (c->ran_بیرون_از_جا ())) return_trace (خطأ);

    هذا->string_جابه_جایی = c->درازی ();

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    خودکار *name_اول = c->serializer->شروع_embed<name> ();

#اگر_تعریف_شده HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_رکورد_ids_t, hb_بایتها_t> *name_جدول_overrides =
        &c->plan->name_جدول_overrides;
#پایان_اگر

    خودکار it =
    + name_رکورد_Z.as_array (شمار)
    | hb_filter (c->plan->name_ids, &Name_رکورد::nameID)
    | hb_filter (c->plan->name_languages, &Name_رکورد::languageID)
    | hb_filter ([&] (ثابت Name_رکورد& namerecord) {
      أرجع
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#اگر_تعریف_شده HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت Name_رکورد& namerecord) {
      إذا (name_جدول_overrides->is_empty ())
        أرجع درست;
      hb_ot_name_رکورد_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_بایتها_t *p;
      إذا (name_جدول_overrides->has (rec_ids, &p) &&
          (*p).درازی == 0)
        أرجع خطأ;
      أرجع درست;
    })
#پایان_اگر
    ;

#اگر_تعریف_شده HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_رکورد_ids_t, بیعلامت> retained_name_رکورد_ids;
    لكل (ثابت Name_رکورد& rec : it)
    {
      hb_ot_name_رکورد_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_رکورد_ids.مجموعه (rec_ids, 1);
    }

    hb_وکتور_t<hb_ot_name_رکورد_ids_t> درج_name_رکوردها;
    إذا (!name_جدول_overrides->is_empty ())
    {
      إذا (unlikely (!درج_name_رکوردها.alloc (name_جدول_overrides->get_population (), درست)))
        أرجع خطأ;
      لكل (ثابت خودکار& رکورد_ids : name_جدول_overrides->کلیدها ())
      {
        إذا (name_جدول_overrides->get (رکورد_ids).درازی == 0)
          استمر;
        إذا (retained_name_رکورد_ids.has (رکورد_ids))
          استمر;
        درج_name_رکوردها.push (رکورد_ids);
      }
    }
#پایان_اگر

    أرجع name_اول->serialize (c->serializer, it,
				  std::addressof (هذا + string_جابه_جایی)
#اگر_تعریف_شده HB_EXPERIMENTAL_API
				  , درج_name_رکوردها
				  , name_جدول_overrides
#پایان_اگر
				  );
  }

  منطقي sanitize_رکوردها (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت فراغ *string_pool = (هذا+string_جابه_جایی).arrayZ;
    return_trace (name_رکورد_Z.sanitize (c, شمار, string_pool));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_رکورد_Z.arrayZ, شمار) &&
		  c->check_range (هذا, string_جابه_جایی) &&
		  sanitize_رکوردها (c));
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<name> (face);
      أكد (هذا->جدول.get_درازی () >= هذا->جدول->string_جابه_جایی);
      هذا->pool = (ثابت حرف *) (ثابت فراغ *) (هذا->جدول+هذا->جدول->string_جابه_جایی);
      هذا->pool_len = هذا->جدول.get_درازی () - هذا->جدول->string_جابه_جایی;
      ثابت hb_array_t<ثابت Name_رکورد> all_نامها (هذا->جدول->name_رکورد_Z.arrayZ,
						    هذا->جدول->شمار);

      هذا->نامها.alloc_exact (all_نامها.درازی);

      لكل (بیعلامت صحيح i = 0; i < all_نامها.درازی; i++)
      {
	hb_ot_name_مدخل_t *مدخل = هذا->نامها.push ();

	مدخل->name_id = all_نامها[i].nameID;
	مدخل->language = all_نامها[i].language (face);
	مدخل->مدخل_score =  all_نامها[i].score ();
	مدخل->مدخل_شاخص = i;
      }

      هذا->نامها.qsort (hb_ot_name_مدخل_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      بیعلامت صحيح j = 0;
      لكل (بیعلامت صحيح i = 0; i < هذا->نامها.درازی; i++)
      {
	إذا (هذا->نامها[i].مدخل_score == UNSUPPORTED ||
	    هذا->نامها[i].language == HB_LANGUAGE_INVALID)
	  استمر;
	إذا (i &&
	    هذا->نامها[i - 1].name_id  == هذا->نامها[i].name_id &&
	    هذا->نامها[i - 1].language == هذا->نامها[i].language)
	  استمر;
	هذا->نامها[j++] = هذا->نامها[i];
      }
      هذا->نامها.resize (j);
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    صحيح get_شاخص (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   بیعلامت صحيح    *عرض=اشاره_تهی) ثابت
    {
      ثابت hb_ot_name_مدخل_t key = {name_id, {0}, language};
      ثابت hb_ot_name_مدخل_t *مدخل = hb_bsearch (key, (ثابت hb_ot_name_مدخل_t *) هذا->نامها,
						    هذا->نامها.درازی,
						    حجم (hb_ot_name_مدخل_t),
						    hb_ot_name_مدخل_cmp_کلید,
						    درست);

      إذا (!مدخل)
      {
	مدخل = hb_bsearch (key, (ثابت hb_ot_name_مدخل_t *) هذا->نامها,
			    هذا->نامها.درازی,
			    حجم (hb_ot_name_مدخل_t),
			    hb_ot_name_مدخل_cmp_کلید,
			    خطأ);
      }

      إذا (!مدخل)
	أرجع -1;

      إذا (عرض)
	*عرض = مدخل->مدخل_score < 10 ? 2 : 1;

      أرجع مدخل->مدخل_شاخص;
    }

    hb_بایتها_t get_name (بیعلامت صحيح idx) ثابت
    {
      ثابت hb_array_t<ثابت Name_رکورد> all_نامها (جدول->name_رکورد_Z.arrayZ, جدول->شمار);
      ثابت Name_رکورد &رکورد = all_نامها[idx];
      ثابت hb_بایتها_t string_pool (pool, pool_len);
      أرجع string_pool.sub_array (رکورد.جابه_جایی, رکورد.درازی);
    }

    خاص:
    ثابت حرف *pool;
    بیعلامت صحيح pool_len;
    عام:
    hb_blob_ptr_t<name> جدول;
    hb_وکتور_t<hb_ot_name_مدخل_t> نامها;
  };

  عام:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	شمار;		/* Number of name records. */
  NNOffset16_مقصد<Unsized_Array_از<HBUINT8>>
		string_جابه_جایی;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_از<Name_رکورد>
		name_رکورد_Z;	/* The name records where count is the number of records. */
  عام:
  DEFINE_اندازه_ARRAY (6, name_رکورد_Z);
};

#لغو_تعریف مدخل_شاخص
#لغو_تعریف مدخل_score

بنية name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#پایان_اگر /* OT_NAME_NAME_HH */
