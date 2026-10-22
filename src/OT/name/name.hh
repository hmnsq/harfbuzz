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

#جيڪڏهن_وصف_نه_ٿيل OT_NAME_NAME_HH
#وصف_ڪريو OT_NAME_NAME_HH

#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-ot-name-language.hh"
#ضمن "../../hb-aat-layout.hh"
#ضمن "../../hb-utf.hh"


نطاق OT {

سانچو <قسم_نالو اندريون_utf_t, قسم_نالو ٻاهريون_utf_t>
مضمن بنانشان صحيح
hb_ot_name_convert_utf (hb_بائيٽون_t                       بائيٽون,
			بنانشان صحيح                    *متن_سائيز /* IN/OUT */,
			قسم_نالو ٻاهريون_utf_t::codepoint_t *متن /*ٻاهريون*/)
{
  بنانشان صحيح src_len = بائيٽون.ڊيگهائي / حجم (قسم_نالو اندريون_utf_t::codepoint_t);
  ثابت قسم_نالو اندريون_utf_t::codepoint_t *src = (ثابت قسم_نالو اندريون_utf_t::codepoint_t *) بائيٽون.arrayZ;
  ثابت قسم_نالو اندريون_utf_t::codepoint_t *src_پڇاڙي = src + src_len;

  قسم_نالو ٻاهريون_utf_t::codepoint_t *dst = متن;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;

  إذا (متن_سائيز && *متن_سائيز)
  {
    (*متن_سائيز)--; /* Save room for NUL-termination. */
    ثابت قسم_نالو ٻاهريون_utf_t::codepoint_t *dst_پڇاڙي = متن + *متن_سائيز;

    طالما (src < src_پڇاڙي && dst < dst_پڇاڙي)
    {
      ثابت قسم_نالو اندريون_utf_t::codepoint_t *src_ايندڙ = اندريون_utf_t::next (src, src_پڇاڙي, &unicode, replacement);
      قسم_نالو ٻاهريون_utf_t::codepoint_t *dst_ايندڙ = ٻاهريون_utf_t::encode (dst, dst_پڇاڙي, unicode);
      إذا (dst_ايندڙ == dst)
	اكسر; /* Out-of-room. */

      dst = dst_ايندڙ;
      src = src_ايندڙ;
    }

    *متن_سائيز = dst - متن;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  بنانشان صحيح dst_len = dst - متن;
  طالما (src < src_پڇاڙي)
  {
    src = اندريون_utf_t::next (src, src_پڇاڙي, &unicode, replacement);
    dst_len += ٻاهريون_utf_t::encode_len (unicode);
  }
  أرجع dst_len;
}

#وصف_ڪريو داخلا_score var.u16[0]
#وصف_ڪريو داخلا_اشاريو var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#وصف_ڪريو HB_OT_ٽيگ_name HB_ٽيگ('n','a','m','e')

#وصف_ڪريو UNSUPPORTED	42

بنية Name_رڪارڊ
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_NAME_LANGUAGE
    بنانشان صحيح p = platformID;
    بنانشان صحيح l = languageID;

    إذا (p == 3)
      أرجع hb_ot_name_language_for_ms_ڪوڊ (l);

    إذا (p == 1)
      أرجع hb_ot_name_language_for_mac_ڪوڊ (l);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_NAME_LANGUAGE_AAT
    إذا (p == 0)
      أرجع face->جدول.ltag->get_language (l);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
    أرجع HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    بنانشان صحيح p = platformID;
    بنانشان صحيح e = encodingID;

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

  Name_رڪارڊ* copy (hb_serialize_context_t *c, ثابت فراغ *بنياد
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_رڪارڊ_ids_t, hb_بائيٽون_t> *name_جدول_overrides
#جيڪڏهن_پورو
		    ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    HB_اڻ_استعمال_ٿيل خودڪار snap = c->snapshot ();
    خودڪار *ٻاهريون = c->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خالي_اشارو);
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    hb_ot_name_رڪارڊ_ids_t رڪارڊ_ids (platformID, encodingID, languageID, nameID);
    hb_بائيٽون_t* name_بائيٽون;

    إذا (name_جدول_overrides->has (رڪارڊ_ids, &name_بائيٽون)) {
      hb_بائيٽون_t encoded_بائيٽون = *name_بائيٽون;
      حرف *name_str_utf16_be = خالي_اشارو;

      إذا (platformID != 1)
      {
        بنانشان متن_سائيز = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بائيٽون, خالي_اشارو, خالي_اشارو);

        متن_سائيز++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        بنانشان byte_len = متن_سائيز * hb_utf16_be_t::codepoint_t::static_سائيز;
        name_str_utf16_be = (حرف *) hb_calloc (byte_len, 1);
        إذا (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (خالي_اشارو);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بائيٽون, &متن_سائيز,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        بنانشان encoded_byte_len = متن_سائيز * hb_utf16_be_t::codepoint_t::static_سائيز;
        إذا (!encoded_byte_len || !c->check_سونپيو (ٻاهريون->ڊيگهائي, encoded_byte_len, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (خالي_اشارو);
        }

        encoded_بائيٽون = hb_بائيٽون_t (name_str_utf16_be, encoded_byte_len);
      }
      وإلا
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        إذا (!c->check_سونپيو (ٻاهريون->ڊيگهائي, encoded_بائيٽون.ڊيگهائي, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) {
          c->revert (snap);
          return_trace (خالي_اشارو);
        }
      }

      ٻاهريون->وٿي = 0;
      c->push ();
      encoded_بائيٽون.copy (c);
      c->جوڙيو_link (ٻاهريون->وٿي, c->ڪڍو_pack (), hb_serialize_context_t::پڇ_2, 0);
      hb_free (name_str_utf16_be);
    }
    وإلا
#جيڪڏهن_پورو
    {
      ٻاهريون->وٿي.serialize_copy (c, وٿي, بنياد, 0, hb_serialize_context_t::پڇ_2, ڊيگهائي);
    }
    return_trace (ٻاهريون);
  }

  منطقي isUnicode () ثابت
  {
    بنانشان صحيح p = platformID;
    بنانشان صحيح e = encodingID;

    أرجع (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت Name_رڪارڊ *a = (ثابت Name_رڪارڊ *)pa;
    ثابت Name_رڪارڊ *b = (ثابت Name_رڪارڊ *)pb;

    إذا (a->platformID != b->platformID)
      أرجع a->platformID - b->platformID;

    إذا (a->encodingID != b->encodingID)
      أرجع a->encodingID - b->encodingID;

    إذا (a->languageID != b->languageID)
      أرجع a->languageID - b->languageID;

    إذا (a->nameID != b->nameID)
      أرجع a->nameID - b->nameID;

    إذا (a->ڊيگهائي != b->ڊيگهائي)
      أرجع a->ڊيگهائي - b->ڊيگهائي;

    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  وٿي.sanitize (c, بنياد, ڊيگهائي));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	ڊيگهائي;		/* String length (in bytes). */
  NNOffset16_منزل<Unsized_Array_جو<HBUINT8>>
		وٿي;		/* String offset from start of storage area (in bytes). */
  عام:
  DEFINE_سائيز_STATIC (12);
};

ساكن صحيح
hb_ot_name_داخلا_cmp_ڪنجي (ثابت فراغ *pa, ثابت فراغ *pb, منطقي exact)
{
  ثابت hb_ot_name_داخلا_t *a = (ثابت hb_ot_name_داخلا_t *) pa;
  ثابت hb_ot_name_داخلا_t *b = (ثابت hb_ot_name_داخلا_t *) pb;

  /* Compare by name_id, then language. */

  إذا (a->name_id != b->name_id)
    أرجع a->name_id - b->name_id;

  إذا (a->language == b->language) أرجع 0;
  إذا (!a->language) أرجع -1;
  إذا (!b->language) أرجع +1;

  ثابت حرف *astr = hb_language_منزل_string (a->language);
  ثابت حرف *bstr = hb_language_منزل_string (b->language);

  نشانوارو c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  إذا (!exact && c &&
      hb_language_matches (b->language, a->language))
    أرجع 0;

  أرجع c;
}

ساكن صحيح
hb_ot_name_داخلا_cmp (ثابت فراغ *pa, ثابت فراغ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحيح v = hb_ot_name_داخلا_cmp_ڪنجي (pa, pb, سچ);
  إذا (v)
    أرجع v;

  ثابت hb_ot_name_داخلا_t *a = (ثابت hb_ot_name_داخلا_t *) pa;
  ثابت hb_ot_name_داخلا_t *b = (ثابت hb_ot_name_داخلا_t *) pb;

  إذا (a->داخلا_score != b->داخلا_score)
    أرجع a->داخلا_score - b->داخلا_score;

  إذا (a->داخلا_اشاريو != b->داخلا_اشاريو)
    أرجع a->داخلا_اشاريو - b->داخلا_اشاريو;

  أرجع 0;
}

بنية name
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_name;

  size_t get_سائيز () ثابت
  { أرجع min_سائيز + ڳڻپ * name_رڪارڊ_Z.شي_سائيز; }

  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_ذريعو_جو (Iterator, ثابت Name_رڪارڊ &))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت فراغ *src_string_pool
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
                  , ثابت hb_ويڪٽر_t<hb_ot_name_رڪارڊ_ids_t>& وجهو_name_رڪارڊون
		  , ثابت hb_hashmap_t<hb_ot_name_رڪارڊ_ids_t, hb_بائيٽون_t> *name_جدول_overrides
#جيڪڏهن_پورو
		  )
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    بنانشان ڪل_ڳڻپ = it.len ()
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
        + وجهو_name_رڪارڊون.ڊيگهائي
#جيڪڏهن_پورو
        ;
    هذا->format = 0;
    إذا (!c->check_سونپيو (هذا->ڳڻپ, ڪل_ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      أرجع خطأ;

    Name_رڪارڊ *name_رڪارڊون = (Name_رڪارڊ *) hb_calloc (ڪل_ڳڻپ, Name_رڪارڊ::static_سائيز);
    إذا (unlikely (!name_رڪارڊون)) return_trace (خطأ);

    hb_array_t<Name_رڪارڊ> رڪارڊون (name_رڪارڊون, ڪل_ڳڻپ);

    لكل (ثابت Name_رڪارڊ& رڪارڊ : it)
    {
      hb_memcpy (name_رڪارڊون, &رڪارڊ, Name_رڪارڊ::static_سائيز);
      name_رڪارڊون++;
    }

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    لكل (بنانشان i = 0; i < وجهو_name_رڪارڊون.ڊيگهائي; i++)
    {
      ثابت hb_ot_name_رڪارڊ_ids_t& ids = وجهو_name_رڪارڊون[i];
      Name_رڪارڊ رڪارڊ;
      رڪارڊ.platformID = ids.platform_id;
      رڪارڊ.encodingID = ids.encoding_id;
      رڪارڊ.languageID = ids.language_id;
      رڪارڊ.nameID = ids.name_id;
      رڪارڊ.ڊيگهائي = 0; // handled in NameRecord copy()
      رڪارڊ.وٿي = 0;
      hb_memcpy (name_رڪارڊون, &رڪارڊ, Name_رڪارڊ::static_سائيز);
      name_رڪارڊون++;
    }
#جيڪڏهن_پورو

    رڪارڊون.qsort ();

    c->copy_all (رڪارڊون,
		 src_string_pool
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
		 , name_جدول_overrides
#جيڪڏهن_پورو
		 );
    hb_free (رڪارڊون.arrayZ);


    إذا (unlikely (c->ran_ٻاهريون_جو_گنجائش ())) return_trace (خطأ);

    هذا->string_وٿي = c->ڊيگهائي ();

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    خودڪار *name_مفرد = c->serializer->شروعات_embed<name> ();

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_رڪارڊ_ids_t, hb_بائيٽون_t> *name_جدول_overrides =
        &c->plan->name_جدول_overrides;
#جيڪڏهن_پورو

    خودڪار it =
    + name_رڪارڊ_Z.as_array (ڳڻپ)
    | hb_filter (c->plan->name_ids, &Name_رڪارڊ::nameID)
    | hb_filter (c->plan->name_languages, &Name_رڪارڊ::languageID)
    | hb_filter ([&] (ثابت Name_رڪارڊ& namerecord) {
      أرجع
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت Name_رڪارڊ& namerecord) {
      إذا (name_جدول_overrides->is_empty ())
        أرجع سچ;
      hb_ot_name_رڪارڊ_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_بائيٽون_t *p;
      إذا (name_جدول_overrides->has (rec_ids, &p) &&
          (*p).ڊيگهائي == 0)
        أرجع خطأ;
      أرجع سچ;
    })
#جيڪڏهن_پورو
    ;

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_رڪارڊ_ids_t, بنانشان> retained_name_رڪارڊ_ids;
    لكل (ثابت Name_رڪارڊ& rec : it)
    {
      hb_ot_name_رڪارڊ_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_رڪارڊ_ids.سيٽ (rec_ids, 1);
    }

    hb_ويڪٽر_t<hb_ot_name_رڪارڊ_ids_t> وجهو_name_رڪارڊون;
    إذا (!name_جدول_overrides->is_empty ())
    {
      إذا (unlikely (!وجهو_name_رڪارڊون.alloc (name_جدول_overrides->get_population (), سچ)))
        أرجع خطأ;
      لكل (ثابت خودڪار& رڪارڊ_ids : name_جدول_overrides->ڪنجيون ())
      {
        إذا (name_جدول_overrides->get (رڪارڊ_ids).ڊيگهائي == 0)
          استمر;
        إذا (retained_name_رڪارڊ_ids.has (رڪارڊ_ids))
          استمر;
        وجهو_name_رڪارڊون.push (رڪارڊ_ids);
      }
    }
#جيڪڏهن_پورو

    أرجع name_مفرد->serialize (c->serializer, it,
				  std::addressof (هذا + string_وٿي)
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
				  , وجهو_name_رڪارڊون
				  , name_جدول_overrides
#جيڪڏهن_پورو
				  );
  }

  منطقي sanitize_رڪارڊون (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت فراغ *string_pool = (هذا+string_وٿي).arrayZ;
    return_trace (name_رڪارڊ_Z.sanitize (c, ڳڻپ, string_pool));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_رڪارڊ_Z.arrayZ, ڳڻپ) &&
		  c->check_range (هذا, string_وٿي) &&
		  sanitize_رڪارڊون (c));
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<name> (face);
      أكد (هذا->جدول.get_ڊيگهائي () >= هذا->جدول->string_وٿي);
      هذا->pool = (ثابت حرف *) (ثابت فراغ *) (هذا->جدول+هذا->جدول->string_وٿي);
      هذا->pool_len = هذا->جدول.get_ڊيگهائي () - هذا->جدول->string_وٿي;
      ثابت hb_array_t<ثابت Name_رڪارڊ> all_نالا (هذا->جدول->name_رڪارڊ_Z.arrayZ,
						    هذا->جدول->ڳڻپ);

      هذا->نالا.alloc_exact (all_نالا.ڊيگهائي);

      لكل (بنانشان صحيح i = 0; i < all_نالا.ڊيگهائي; i++)
      {
	hb_ot_name_داخلا_t *داخلا = هذا->نالا.push ();

	داخلا->name_id = all_نالا[i].nameID;
	داخلا->language = all_نالا[i].language (face);
	داخلا->داخلا_score =  all_نالا[i].score ();
	داخلا->داخلا_اشاريو = i;
      }

      هذا->نالا.qsort (hb_ot_name_داخلا_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      بنانشان صحيح j = 0;
      لكل (بنانشان صحيح i = 0; i < هذا->نالا.ڊيگهائي; i++)
      {
	إذا (هذا->نالا[i].داخلا_score == UNSUPPORTED ||
	    هذا->نالا[i].language == HB_LANGUAGE_INVALID)
	  استمر;
	إذا (i &&
	    هذا->نالا[i - 1].name_id  == هذا->نالا[i].name_id &&
	    هذا->نالا[i - 1].language == هذا->نالا[i].language)
	  استمر;
	هذا->نالا[j++] = هذا->نالا[i];
      }
      هذا->نالا.resize (j);
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    صحيح get_اشاريو (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   بنانشان صحيح    *ويڪر=خالي_اشارو) ثابت
    {
      ثابت hb_ot_name_داخلا_t key = {name_id, {0}, language};
      ثابت hb_ot_name_داخلا_t *داخلا = hb_bsearch (key, (ثابت hb_ot_name_داخلا_t *) هذا->نالا,
						    هذا->نالا.ڊيگهائي,
						    حجم (hb_ot_name_داخلا_t),
						    hb_ot_name_داخلا_cmp_ڪنجي,
						    سچ);

      إذا (!داخلا)
      {
	داخلا = hb_bsearch (key, (ثابت hb_ot_name_داخلا_t *) هذا->نالا,
			    هذا->نالا.ڊيگهائي,
			    حجم (hb_ot_name_داخلا_t),
			    hb_ot_name_داخلا_cmp_ڪنجي,
			    خطأ);
      }

      إذا (!داخلا)
	أرجع -1;

      إذا (ويڪر)
	*ويڪر = داخلا->داخلا_score < 10 ? 2 : 1;

      أرجع داخلا->داخلا_اشاريو;
    }

    hb_بائيٽون_t get_name (بنانشان صحيح idx) ثابت
    {
      ثابت hb_array_t<ثابت Name_رڪارڊ> all_نالا (جدول->name_رڪارڊ_Z.arrayZ, جدول->ڳڻپ);
      ثابت Name_رڪارڊ &رڪارڊ = all_نالا[idx];
      ثابت hb_بائيٽون_t string_pool (pool, pool_len);
      أرجع string_pool.sub_array (رڪارڊ.وٿي, رڪارڊ.ڊيگهائي);
    }

    خاص:
    ثابت حرف *pool;
    بنانشان صحيح pool_len;
    عام:
    hb_blob_ptr_t<name> جدول;
    hb_ويڪٽر_t<hb_ot_name_داخلا_t> نالا;
  };

  عام:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	ڳڻپ;		/* Number of name records. */
  NNOffset16_منزل<Unsized_Array_جو<HBUINT8>>
		string_وٿي;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_جو<Name_رڪارڊ>
		name_رڪارڊ_Z;	/* The name records where count is the number of records. */
  عام:
  DEFINE_سائيز_ARRAY (6, name_رڪارڊ_Z);
};

#وصف_رد داخلا_اشاريو
#وصف_رد داخلا_score

بنية name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#جيڪڏهن_پورو /* OT_NAME_NAME_HH */
