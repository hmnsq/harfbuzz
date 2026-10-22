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

#جے_بنا_تعریف OT_NAME_NAME_HH
#تعریف_کرو OT_NAME_NAME_HH

#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-ot-name-language.hh"
#ضمن "../../hb-aat-layout.hh"
#ضمن "../../hb-utf.hh"


نطاق OT {

سانچا <قسم_ناں وچ_utf_t, قسم_ناں باہرلا_utf_t>
مضمن بنانشانی صحيح
hb_ot_name_convert_utf (hb_بائٹاں_t                       بائٹاں,
			بنانشانی صحيح                    *متن_ناپ /* IN/OUT */,
			قسم_ناں باہرلا_utf_t::codepoint_t *متن /*باہرلا*/)
{
  بنانشانی صحيح src_len = بائٹاں.لمائی / حجم (قسم_ناں وچ_utf_t::codepoint_t);
  ثابت قسم_ناں وچ_utf_t::codepoint_t *src = (ثابت قسم_ناں وچ_utf_t::codepoint_t *) بائٹاں.arrayZ;
  ثابت قسم_ناں وچ_utf_t::codepoint_t *src_سرا = src + src_len;

  قسم_ناں باہرلا_utf_t::codepoint_t *dst = متن;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;

  إذا (متن_ناپ && *متن_ناپ)
  {
    (*متن_ناپ)--; /* Save room for NUL-termination. */
    ثابت قسم_ناں باہرلا_utf_t::codepoint_t *dst_سرا = متن + *متن_ناپ;

    طالما (src < src_سرا && dst < dst_سرا)
    {
      ثابت قسم_ناں وچ_utf_t::codepoint_t *src_اگلا = وچ_utf_t::next (src, src_سرا, &unicode, replacement);
      قسم_ناں باہرلا_utf_t::codepoint_t *dst_اگلا = باہرلا_utf_t::encode (dst, dst_سرا, unicode);
      إذا (dst_اگلا == dst)
	اكسر; /* Out-of-room. */

      dst = dst_اگلا;
      src = src_اگلا;
    }

    *متن_ناپ = dst - متن;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  بنانشانی صحيح dst_len = dst - متن;
  طالما (src < src_سرا)
  {
    src = وچ_utf_t::next (src, src_سرا, &unicode, replacement);
    dst_len += باہرلا_utf_t::encode_len (unicode);
  }
  أرجع dst_len;
}

#تعریف_کرو اندراج_score var.u16[0]
#تعریف_کرو اندراج_اشاریہ var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#تعریف_کرو HB_OT_ٹیگ_name HB_ٹیگ('n','a','m','e')

#تعریف_کرو UNSUPPORTED	42

بنية Name_ریکارڈ
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#جے_بنا_تعریف HB_NO_OT_NAME_LANGUAGE
    بنانشانی صحيح p = platformID;
    بنانشانی صحيح l = languageID;

    إذا (p == 3)
      أرجع hb_ot_name_language_for_ms_کوڈ (l);

    إذا (p == 1)
      أرجع hb_ot_name_language_for_mac_کوڈ (l);

#جے_بنا_تعریف HB_NO_OT_NAME_LANGUAGE_AAT
    إذا (p == 0)
      أرجع face->جدول.ltag->get_language (l);
#جے_انت

#جے_انت
    أرجع HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    بنانشانی صحيح p = platformID;
    بنانشانی صحيح e = encodingID;

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

  Name_ریکارڈ* copy (hb_serialize_context_t *c, ثابت فراغ *ادھار
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹاں_t> *name_جدول_overrides
#جے_انت
		    ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    HB_بن_ورتیا خودکار snap = c->snapshot ();
    خودکار *باہرلا = c->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خالی_سوچک);
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    hb_ot_name_ریکارڈ_ids_t ریکارڈ_ids (platformID, encodingID, languageID, nameID);
    hb_بائٹاں_t* name_بائٹاں;

    إذا (name_جدول_overrides->has (ریکارڈ_ids, &name_بائٹاں)) {
      hb_بائٹاں_t encoded_بائٹاں = *name_بائٹاں;
      حرف *name_str_utf16_be = خالی_سوچک;

      إذا (platformID != 1)
      {
        بنانشانی متن_ناپ = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بائٹاں, خالی_سوچک, خالی_سوچک);

        متن_ناپ++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        بنانشانی byte_len = متن_ناپ * hb_utf16_be_t::codepoint_t::static_ناپ;
        name_str_utf16_be = (حرف *) hb_calloc (byte_len, 1);
        إذا (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (خالی_سوچک);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بائٹاں, &متن_ناپ,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        بنانشانی encoded_byte_len = متن_ناپ * hb_utf16_be_t::codepoint_t::static_ناپ;
        إذا (!encoded_byte_len || !c->check_سونپو (باہرلا->لمائی, encoded_byte_len, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (خالی_سوچک);
        }

        encoded_بائٹاں = hb_بائٹاں_t (name_str_utf16_be, encoded_byte_len);
      }
      وإلا
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        إذا (!c->check_سونپو (باہرلا->لمائی, encoded_بائٹاں.لمائی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) {
          c->revert (snap);
          return_trace (خالی_سوچک);
        }
      }

      باہرلا->وتھ = 0;
      c->push ();
      encoded_بائٹاں.copy (c);
      c->جوڑو_link (باہرلا->وتھ, c->نکالو_pack (), hb_serialize_context_t::پوچھ_2, 0);
      hb_free (name_str_utf16_be);
    }
    وإلا
#جے_انت
    {
      باہرلا->وتھ.serialize_copy (c, وتھ, ادھار, 0, hb_serialize_context_t::پوچھ_2, لمائی);
    }
    return_trace (باہرلا);
  }

  منطقي isUnicode () ثابت
  {
    بنانشانی صحيح p = platformID;
    بنانشانی صحيح e = encodingID;

    أرجع (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت Name_ریکارڈ *a = (ثابت Name_ریکارڈ *)pa;
    ثابت Name_ریکارڈ *b = (ثابت Name_ریکارڈ *)pb;

    إذا (a->platformID != b->platformID)
      أرجع a->platformID - b->platformID;

    إذا (a->encodingID != b->encodingID)
      أرجع a->encodingID - b->encodingID;

    إذا (a->languageID != b->languageID)
      أرجع a->languageID - b->languageID;

    إذا (a->nameID != b->nameID)
      أرجع a->nameID - b->nameID;

    إذا (a->لمائی != b->لمائی)
      أرجع a->لمائی - b->لمائی;

    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  وتھ.sanitize (c, ادھار, لمائی));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	لمائی;		/* String length (in bytes). */
  NNOffset16_منزل<Unsized_Array_دا<HBUINT8>>
		وتھ;		/* String offset from start of storage area (in bytes). */
  عام:
  DEFINE_ناپ_STATIC (12);
};

ساكن صحيح
hb_ot_name_اندراج_cmp_کلید (ثابت فراغ *pa, ثابت فراغ *pb, منطقي exact)
{
  ثابت hb_ot_name_اندراج_t *a = (ثابت hb_ot_name_اندراج_t *) pa;
  ثابت hb_ot_name_اندراج_t *b = (ثابت hb_ot_name_اندراج_t *) pb;

  /* Compare by name_id, then language. */

  إذا (a->name_id != b->name_id)
    أرجع a->name_id - b->name_id;

  إذا (a->language == b->language) أرجع 0;
  إذا (!a->language) أرجع -1;
  إذا (!b->language) أرجع +1;

  ثابت حرف *astr = hb_language_منزل_string (a->language);
  ثابت حرف *bstr = hb_language_منزل_string (b->language);

  نشانیوالا c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  إذا (!exact && c &&
      hb_language_matches (b->language, a->language))
    أرجع 0;

  أرجع c;
}

ساكن صحيح
hb_ot_name_اندراج_cmp (ثابت فراغ *pa, ثابت فراغ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحيح v = hb_ot_name_اندراج_cmp_کلید (pa, pb, سچ);
  إذا (v)
    أرجع v;

  ثابت hb_ot_name_اندراج_t *a = (ثابت hb_ot_name_اندراج_t *) pa;
  ثابت hb_ot_name_اندراج_t *b = (ثابت hb_ot_name_اندراج_t *) pb;

  إذا (a->اندراج_score != b->اندراج_score)
    أرجع a->اندراج_score - b->اندراج_score;

  إذا (a->اندراج_اشاریہ != b->اندراج_اشاریہ)
    أرجع a->اندراج_اشاریہ - b->اندراج_اشاریہ;

  أرجع 0;
}

بنية name
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_name;

  size_t get_ناپ () ثابت
  { أرجع min_ناپ + گنتی * name_ریکارڈ_Z.شے_ناپ; }

  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_سروت_دا (Iterator, ثابت Name_ریکارڈ &))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت فراغ *src_string_pool
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
                  , ثابت hb_سمتیہ_t<hb_ot_name_ریکارڈ_ids_t>& داخل_کرو_name_ریکارڈاں
		  , ثابت hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹاں_t> *name_جدول_overrides
#جے_انت
		  )
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    بنانشانی کل_گنتی = it.len ()
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
        + داخل_کرو_name_ریکارڈاں.لمائی
#جے_انت
        ;
    هذا->format = 0;
    إذا (!c->check_سونپو (هذا->گنتی, کل_گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      أرجع خطأ;

    Name_ریکارڈ *name_ریکارڈاں = (Name_ریکارڈ *) hb_calloc (کل_گنتی, Name_ریکارڈ::static_ناپ);
    إذا (unlikely (!name_ریکارڈاں)) return_trace (خطأ);

    hb_array_t<Name_ریکارڈ> ریکارڈاں (name_ریکارڈاں, کل_گنتی);

    لكل (ثابت Name_ریکارڈ& ریکارڈ : it)
    {
      hb_memcpy (name_ریکارڈاں, &ریکارڈ, Name_ریکارڈ::static_ناپ);
      name_ریکارڈاں++;
    }

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    لكل (بنانشانی i = 0; i < داخل_کرو_name_ریکارڈاں.لمائی; i++)
    {
      ثابت hb_ot_name_ریکارڈ_ids_t& ids = داخل_کرو_name_ریکارڈاں[i];
      Name_ریکارڈ ریکارڈ;
      ریکارڈ.platformID = ids.platform_id;
      ریکارڈ.encodingID = ids.encoding_id;
      ریکارڈ.languageID = ids.language_id;
      ریکارڈ.nameID = ids.name_id;
      ریکارڈ.لمائی = 0; // handled in NameRecord copy()
      ریکارڈ.وتھ = 0;
      hb_memcpy (name_ریکارڈاں, &ریکارڈ, Name_ریکارڈ::static_ناپ);
      name_ریکارڈاں++;
    }
#جے_انت

    ریکارڈاں.qsort ();

    c->copy_all (ریکارڈاں,
		 src_string_pool
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
		 , name_جدول_overrides
#جے_انت
		 );
    hb_free (ریکارڈاں.arrayZ);


    إذا (unlikely (c->ran_باہرلا_دا_گنجائش ())) return_trace (خطأ);

    هذا->string_وتھ = c->لمائی ();

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    خودکار *name_مفرد = c->serializer->شروع_embed<name> ();

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹاں_t> *name_جدول_overrides =
        &c->plan->name_جدول_overrides;
#جے_انت

    خودکار it =
    + name_ریکارڈ_Z.as_array (گنتی)
    | hb_filter (c->plan->name_ids, &Name_ریکارڈ::nameID)
    | hb_filter (c->plan->name_languages, &Name_ریکارڈ::languageID)
    | hb_filter ([&] (ثابت Name_ریکارڈ& namerecord) {
      أرجع
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت Name_ریکارڈ& namerecord) {
      إذا (name_جدول_overrides->is_empty ())
        أرجع سچ;
      hb_ot_name_ریکارڈ_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_بائٹاں_t *p;
      إذا (name_جدول_overrides->has (rec_ids, &p) &&
          (*p).لمائی == 0)
        أرجع خطأ;
      أرجع سچ;
    })
#جے_انت
    ;

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, بنانشانی> retained_name_ریکارڈ_ids;
    لكل (ثابت Name_ریکارڈ& rec : it)
    {
      hb_ot_name_ریکارڈ_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_ریکارڈ_ids.سیٹ (rec_ids, 1);
    }

    hb_سمتیہ_t<hb_ot_name_ریکارڈ_ids_t> داخل_کرو_name_ریکارڈاں;
    إذا (!name_جدول_overrides->is_empty ())
    {
      إذا (unlikely (!داخل_کرو_name_ریکارڈاں.alloc (name_جدول_overrides->get_population (), سچ)))
        أرجع خطأ;
      لكل (ثابت خودکار& ریکارڈ_ids : name_جدول_overrides->کلیدیں ())
      {
        إذا (name_جدول_overrides->get (ریکارڈ_ids).لمائی == 0)
          استمر;
        إذا (retained_name_ریکارڈ_ids.has (ریکارڈ_ids))
          استمر;
        داخل_کرو_name_ریکارڈاں.push (ریکارڈ_ids);
      }
    }
#جے_انت

    أرجع name_مفرد->serialize (c->serializer, it,
				  std::addressof (هذا + string_وتھ)
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
				  , داخل_کرو_name_ریکارڈاں
				  , name_جدول_overrides
#جے_انت
				  );
  }

  منطقي sanitize_ریکارڈاں (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت فراغ *string_pool = (هذا+string_وتھ).arrayZ;
    return_trace (name_ریکارڈ_Z.sanitize (c, گنتی, string_pool));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_ریکارڈ_Z.arrayZ, گنتی) &&
		  c->check_range (هذا, string_وتھ) &&
		  sanitize_ریکارڈاں (c));
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<name> (face);
      أكد (هذا->جدول.get_لمائی () >= هذا->جدول->string_وتھ);
      هذا->pool = (ثابت حرف *) (ثابت فراغ *) (هذا->جدول+هذا->جدول->string_وتھ);
      هذا->pool_len = هذا->جدول.get_لمائی () - هذا->جدول->string_وتھ;
      ثابت hb_array_t<ثابت Name_ریکارڈ> all_ناں (هذا->جدول->name_ریکارڈ_Z.arrayZ,
						    هذا->جدول->گنتی);

      هذا->ناں.alloc_exact (all_ناں.لمائی);

      لكل (بنانشانی صحيح i = 0; i < all_ناں.لمائی; i++)
      {
	hb_ot_name_اندراج_t *اندراج = هذا->ناں.push ();

	اندراج->name_id = all_ناں[i].nameID;
	اندراج->language = all_ناں[i].language (face);
	اندراج->اندراج_score =  all_ناں[i].score ();
	اندراج->اندراج_اشاریہ = i;
      }

      هذا->ناں.qsort (hb_ot_name_اندراج_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      بنانشانی صحيح j = 0;
      لكل (بنانشانی صحيح i = 0; i < هذا->ناں.لمائی; i++)
      {
	إذا (هذا->ناں[i].اندراج_score == UNSUPPORTED ||
	    هذا->ناں[i].language == HB_LANGUAGE_INVALID)
	  استمر;
	إذا (i &&
	    هذا->ناں[i - 1].name_id  == هذا->ناں[i].name_id &&
	    هذا->ناں[i - 1].language == هذا->ناں[i].language)
	  استمر;
	هذا->ناں[j++] = هذا->ناں[i];
      }
      هذا->ناں.resize (j);
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    صحيح get_اشاریہ (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   بنانشانی صحيح    *چوڑائی=خالی_سوچک) ثابت
    {
      ثابت hb_ot_name_اندراج_t key = {name_id, {0}, language};
      ثابت hb_ot_name_اندراج_t *اندراج = hb_bsearch (key, (ثابت hb_ot_name_اندراج_t *) هذا->ناں,
						    هذا->ناں.لمائی,
						    حجم (hb_ot_name_اندراج_t),
						    hb_ot_name_اندراج_cmp_کلید,
						    سچ);

      إذا (!اندراج)
      {
	اندراج = hb_bsearch (key, (ثابت hb_ot_name_اندراج_t *) هذا->ناں,
			    هذا->ناں.لمائی,
			    حجم (hb_ot_name_اندراج_t),
			    hb_ot_name_اندراج_cmp_کلید,
			    خطأ);
      }

      إذا (!اندراج)
	أرجع -1;

      إذا (چوڑائی)
	*چوڑائی = اندراج->اندراج_score < 10 ? 2 : 1;

      أرجع اندراج->اندراج_اشاریہ;
    }

    hb_بائٹاں_t get_name (بنانشانی صحيح idx) ثابت
    {
      ثابت hb_array_t<ثابت Name_ریکارڈ> all_ناں (جدول->name_ریکارڈ_Z.arrayZ, جدول->گنتی);
      ثابت Name_ریکارڈ &ریکارڈ = all_ناں[idx];
      ثابت hb_بائٹاں_t string_pool (pool, pool_len);
      أرجع string_pool.sub_array (ریکارڈ.وتھ, ریکارڈ.لمائی);
    }

    خاص:
    ثابت حرف *pool;
    بنانشانی صحيح pool_len;
    عام:
    hb_blob_ptr_t<name> جدول;
    hb_سمتیہ_t<hb_ot_name_اندراج_t> ناں;
  };

  عام:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	گنتی;		/* Number of name records. */
  NNOffset16_منزل<Unsized_Array_دا<HBUINT8>>
		string_وتھ;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_دا<Name_ریکارڈ>
		name_ریکارڈ_Z;	/* The name records where count is the number of records. */
  عام:
  DEFINE_ناپ_ARRAY (6, name_ریکارڈ_Z);
};

#تعریف_رد اندراج_اشاریہ
#تعریف_رد اندراج_score

بنية name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#جے_انت /* OT_NAME_NAME_HH */
