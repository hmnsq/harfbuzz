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

#که_نه_تعریف_شوی OT_NAME_NAME_HH
#تعریف_کړه OT_NAME_NAME_HH

#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-ot-name-language.hh"
#ضمن "../../hb-aat-layout.hh"
#ضمن "../../hb-utf.hh"


نطاق OT {

کينډۍ <ډول_نوم دننه_utf_t, ډول_نوم بهر_utf_t>
مضمن بینښه صحيح
hb_ot_name_convert_utf (hb_بایټونه_t                       بایټونه,
			بینښه صحيح                    *متن_اندازه /* IN/OUT */,
			ډول_نوم بهر_utf_t::codepoint_t *متن /*بهر*/)
{
  بینښه صحيح src_len = بایټونه.اوږدتیا / حجم (ډول_نوم دننه_utf_t::codepoint_t);
  ثابت ډول_نوم دننه_utf_t::codepoint_t *src = (ثابت ډول_نوم دننه_utf_t::codepoint_t *) بایټونه.arrayZ;
  ثابت ډول_نوم دننه_utf_t::codepoint_t *src_پای_ټکی = src + src_len;

  ډول_نوم بهر_utf_t::codepoint_t *dst = متن;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;

  إذا (متن_اندازه && *متن_اندازه)
  {
    (*متن_اندازه)--; /* Save room for NUL-termination. */
    ثابت ډول_نوم بهر_utf_t::codepoint_t *dst_پای_ټکی = متن + *متن_اندازه;

    طالما (src < src_پای_ټکی && dst < dst_پای_ټکی)
    {
      ثابت ډول_نوم دننه_utf_t::codepoint_t *src_بل = دننه_utf_t::next (src, src_پای_ټکی, &unicode, replacement);
      ډول_نوم بهر_utf_t::codepoint_t *dst_بل = بهر_utf_t::encode (dst, dst_پای_ټکی, unicode);
      إذا (dst_بل == dst)
	اكسر; /* Out-of-room. */

      dst = dst_بل;
      src = src_بل;
    }

    *متن_اندازه = dst - متن;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  بینښه صحيح dst_len = dst - متن;
  طالما (src < src_پای_ټکی)
  {
    src = دننه_utf_t::next (src, src_پای_ټکی, &unicode, replacement);
    dst_len += بهر_utf_t::encode_len (unicode);
  }
  أرجع dst_len;
}

#تعریف_کړه ثبت_score var.u16[0]
#تعریف_کړه ثبت_شاخص var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#تعریف_کړه HB_OT_نښان_name HB_نښان('n','a','m','e')

#تعریف_کړه UNSUPPORTED	42

بنية Name_ریکارډ
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#که_نه_تعریف_شوی HB_NO_OT_NAME_LANGUAGE
    بینښه صحيح p = platformID;
    بینښه صحيح l = languageID;

    إذا (p == 3)
      أرجع hb_ot_name_language_for_ms_کوډ (l);

    إذا (p == 1)
      أرجع hb_ot_name_language_for_mac_کوډ (l);

#که_نه_تعریف_شوی HB_NO_OT_NAME_LANGUAGE_AAT
    إذا (p == 0)
      أرجع face->جدول.ltag->get_language (l);
#د_که_پای

#د_که_پای
    أرجع HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    بینښه صحيح p = platformID;
    بینښه صحيح e = encodingID;

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

  Name_ریکارډ* copy (hb_serialize_context_t *c, ثابت فراغ *بنسټ
#که_تعریف_شوی HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_ریکارډ_ids_t, hb_بایټونه_t> *name_جدول_overrides
#د_که_پای
		    ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    HB_ناکارول_شوی خپلکاری snap = c->snapshot ();
    خپلکاری *بهر = c->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (تش_اشاره);
#که_تعریف_شوی HB_EXPERIMENTAL_API
    hb_ot_name_ریکارډ_ids_t ریکارډ_ids (platformID, encodingID, languageID, nameID);
    hb_بایټونه_t* name_بایټونه;

    إذا (name_جدول_overrides->has (ریکارډ_ids, &name_بایټونه)) {
      hb_بایټونه_t encoded_بایټونه = *name_بایټونه;
      حرف *name_str_utf16_be = تش_اشاره;

      إذا (platformID != 1)
      {
        بینښه متن_اندازه = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بایټونه, تش_اشاره, تش_اشاره);

        متن_اندازه++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        بینښه byte_len = متن_اندازه * hb_utf16_be_t::codepoint_t::static_اندازه;
        name_str_utf16_be = (حرف *) hb_calloc (byte_len, 1);
        إذا (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (تش_اشاره);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_بایټونه, &متن_اندازه,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        بینښه encoded_byte_len = متن_اندازه * hb_utf16_be_t::codepoint_t::static_اندازه;
        إذا (!encoded_byte_len || !c->check_ټاکل (بهر->اوږدتیا, encoded_byte_len, HB_SERIALIZE_ERROR_INT_ډکېدنه)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (تش_اشاره);
        }

        encoded_بایټونه = hb_بایټونه_t (name_str_utf16_be, encoded_byte_len);
      }
      وإلا
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        إذا (!c->check_ټاکل (بهر->اوږدتیا, encoded_بایټونه.اوږدتیا, HB_SERIALIZE_ERROR_INT_ډکېدنه)) {
          c->revert (snap);
          return_trace (تش_اشاره);
        }
      }

      بهر->دننه_واټن = 0;
      c->push ();
      encoded_بایټونه.copy (c);
      c->جمع_کړه_link (بهر->دننه_واټن, c->راوباسه_pack (), hb_serialize_context_t::لکۍ_2, 0);
      hb_free (name_str_utf16_be);
    }
    وإلا
#د_که_پای
    {
      بهر->دننه_واټن.serialize_copy (c, دننه_واټن, بنسټ, 0, hb_serialize_context_t::لکۍ_2, اوږدتیا);
    }
    return_trace (بهر);
  }

  منطقي isUnicode () ثابت
  {
    بینښه صحيح p = platformID;
    بینښه صحيح e = encodingID;

    أرجع (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت Name_ریکارډ *a = (ثابت Name_ریکارډ *)pa;
    ثابت Name_ریکارډ *b = (ثابت Name_ریکارډ *)pb;

    إذا (a->platformID != b->platformID)
      أرجع a->platformID - b->platformID;

    إذا (a->encodingID != b->encodingID)
      أرجع a->encodingID - b->encodingID;

    إذا (a->languageID != b->languageID)
      أرجع a->languageID - b->languageID;

    إذا (a->nameID != b->nameID)
      أرجع a->nameID - b->nameID;

    إذا (a->اوږدتیا != b->اوږدتیا)
      أرجع a->اوږدتیا - b->اوږدتیا;

    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  دننه_واټن.sanitize (c, بنسټ, اوږدتیا));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	اوږدتیا;		/* String length (in bytes). */
  NNOffset16_منزل<Unsized_Array_د<HBUINT8>>
		دننه_واټن;		/* String offset from start of storage area (in bytes). */
  عام:
  DEFINE_اندازه_STATIC (12);
};

ساكن صحيح
hb_ot_name_ثبت_cmp_کلی (ثابت فراغ *pa, ثابت فراغ *pb, منطقي exact)
{
  ثابت hb_ot_name_ثبت_t *a = (ثابت hb_ot_name_ثبت_t *) pa;
  ثابت hb_ot_name_ثبت_t *b = (ثابت hb_ot_name_ثبت_t *) pb;

  /* Compare by name_id, then language. */

  إذا (a->name_id != b->name_id)
    أرجع a->name_id - b->name_id;

  إذا (a->language == b->language) أرجع 0;
  إذا (!a->language) أرجع -1;
  إذا (!b->language) أرجع +1;

  ثابت حرف *astr = hb_language_منزل_string (a->language);
  ثابت حرف *bstr = hb_language_منزل_string (b->language);

  نښهدار c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  إذا (!exact && c &&
      hb_language_matches (b->language, a->language))
    أرجع 0;

  أرجع c;
}

ساكن صحيح
hb_ot_name_ثبت_cmp (ثابت فراغ *pa, ثابت فراغ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحيح v = hb_ot_name_ثبت_cmp_کلی (pa, pb, ریښتیا);
  إذا (v)
    أرجع v;

  ثابت hb_ot_name_ثبت_t *a = (ثابت hb_ot_name_ثبت_t *) pa;
  ثابت hb_ot_name_ثبت_t *b = (ثابت hb_ot_name_ثبت_t *) pb;

  إذا (a->ثبت_score != b->ثبت_score)
    أرجع a->ثبت_score - b->ثبت_score;

  إذا (a->ثبت_شاخص != b->ثبت_شاخص)
    أرجع a->ثبت_شاخص - b->ثبت_شاخص;

  أرجع 0;
}

بنية name
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_name;

  size_t get_اندازه () ثابت
  { أرجع min_اندازه + شمېر * name_ریکارډ_Z.توکی_اندازه; }

  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_سرچینه_د (Iterator, ثابت Name_ریکارډ &))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت فراغ *src_string_pool
#که_تعریف_شوی HB_EXPERIMENTAL_API
                  , ثابت hb_ویکټور_t<hb_ot_name_ریکارډ_ids_t>& ورننباسه_name_ریکارډونه
		  , ثابت hb_hashmap_t<hb_ot_name_ریکارډ_ids_t, hb_بایټونه_t> *name_جدول_overrides
#د_که_پای
		  )
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    بینښه ټول_شمېر = it.len ()
#که_تعریف_شوی HB_EXPERIMENTAL_API
        + ورننباسه_name_ریکارډونه.اوږدتیا
#د_که_پای
        ;
    هذا->format = 0;
    إذا (!c->check_ټاکل (هذا->شمېر, ټول_شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      أرجع خطأ;

    Name_ریکارډ *name_ریکارډونه = (Name_ریکارډ *) hb_calloc (ټول_شمېر, Name_ریکارډ::static_اندازه);
    إذا (unlikely (!name_ریکارډونه)) return_trace (خطأ);

    hb_array_t<Name_ریکارډ> ریکارډونه (name_ریکارډونه, ټول_شمېر);

    لكل (ثابت Name_ریکارډ& ریکارډ : it)
    {
      hb_memcpy (name_ریکارډونه, &ریکارډ, Name_ریکارډ::static_اندازه);
      name_ریکارډونه++;
    }

#که_تعریف_شوی HB_EXPERIMENTAL_API
    لكل (بینښه i = 0; i < ورننباسه_name_ریکارډونه.اوږدتیا; i++)
    {
      ثابت hb_ot_name_ریکارډ_ids_t& ids = ورننباسه_name_ریکارډونه[i];
      Name_ریکارډ ریکارډ;
      ریکارډ.platformID = ids.platform_id;
      ریکارډ.encodingID = ids.encoding_id;
      ریکارډ.languageID = ids.language_id;
      ریکارډ.nameID = ids.name_id;
      ریکارډ.اوږدتیا = 0; // handled in NameRecord copy()
      ریکارډ.دننه_واټن = 0;
      hb_memcpy (name_ریکارډونه, &ریکارډ, Name_ریکارډ::static_اندازه);
      name_ریکارډونه++;
    }
#د_که_پای

    ریکارډونه.qsort ();

    c->copy_all (ریکارډونه,
		 src_string_pool
#که_تعریف_شوی HB_EXPERIMENTAL_API
		 , name_جدول_overrides
#د_که_پای
		 );
    hb_free (ریکارډونه.arrayZ);


    إذا (unlikely (c->ran_بهر_د_ځای ())) return_trace (خطأ);

    هذا->string_دننه_واټن = c->اوږدتیا ();

    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    خپلکاری *name_اولي = c->serializer->پیل_embed<name> ();

#که_تعریف_شوی HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_ریکارډ_ids_t, hb_بایټونه_t> *name_جدول_overrides =
        &c->plan->name_جدول_overrides;
#د_که_پای

    خپلکاری it =
    + name_ریکارډ_Z.as_array (شمېر)
    | hb_filter (c->plan->name_ids, &Name_ریکارډ::nameID)
    | hb_filter (c->plan->name_languages, &Name_ریکارډ::languageID)
    | hb_filter ([&] (ثابت Name_ریکارډ& namerecord) {
      أرجع
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#که_تعریف_شوی HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت Name_ریکارډ& namerecord) {
      إذا (name_جدول_overrides->is_empty ())
        أرجع ریښتیا;
      hb_ot_name_ریکارډ_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_بایټونه_t *p;
      إذا (name_جدول_overrides->has (rec_ids, &p) &&
          (*p).اوږدتیا == 0)
        أرجع خطأ;
      أرجع ریښتیا;
    })
#د_که_پای
    ;

#که_تعریف_شوی HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_ریکارډ_ids_t, بینښه> retained_name_ریکارډ_ids;
    لكل (ثابت Name_ریکارډ& rec : it)
    {
      hb_ot_name_ریکارډ_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_ریکارډ_ids.سېټ (rec_ids, 1);
    }

    hb_ویکټور_t<hb_ot_name_ریکارډ_ids_t> ورننباسه_name_ریکارډونه;
    إذا (!name_جدول_overrides->is_empty ())
    {
      إذا (unlikely (!ورننباسه_name_ریکارډونه.alloc (name_جدول_overrides->get_population (), ریښتیا)))
        أرجع خطأ;
      لكل (ثابت خپلکاری& ریکارډ_ids : name_جدول_overrides->کلیګانې ())
      {
        إذا (name_جدول_overrides->get (ریکارډ_ids).اوږدتیا == 0)
          استمر;
        إذا (retained_name_ریکارډ_ids.has (ریکارډ_ids))
          استمر;
        ورننباسه_name_ریکارډونه.push (ریکارډ_ids);
      }
    }
#د_که_پای

    أرجع name_اولي->serialize (c->serializer, it,
				  std::addressof (هذا + string_دننه_واټن)
#که_تعریف_شوی HB_EXPERIMENTAL_API
				  , ورننباسه_name_ریکارډونه
				  , name_جدول_overrides
#د_که_پای
				  );
  }

  منطقي sanitize_ریکارډونه (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت فراغ *string_pool = (هذا+string_دننه_واټن).arrayZ;
    return_trace (name_ریکارډ_Z.sanitize (c, شمېر, string_pool));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_ریکارډ_Z.arrayZ, شمېر) &&
		  c->check_range (هذا, string_دننه_واټن) &&
		  sanitize_ریکارډونه (c));
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->جدول = hb_sanitize_context_t ().reference_جدول<name> (face);
      أكد (هذا->جدول.get_اوږدتیا () >= هذا->جدول->string_دننه_واټن);
      هذا->pool = (ثابت حرف *) (ثابت فراغ *) (هذا->جدول+هذا->جدول->string_دننه_واټن);
      هذا->pool_len = هذا->جدول.get_اوږدتیا () - هذا->جدول->string_دننه_واټن;
      ثابت hb_array_t<ثابت Name_ریکارډ> all_نومونه (هذا->جدول->name_ریکارډ_Z.arrayZ,
						    هذا->جدول->شمېر);

      هذا->نومونه.alloc_exact (all_نومونه.اوږدتیا);

      لكل (بینښه صحيح i = 0; i < all_نومونه.اوږدتیا; i++)
      {
	hb_ot_name_ثبت_t *ثبت = هذا->نومونه.push ();

	ثبت->name_id = all_نومونه[i].nameID;
	ثبت->language = all_نومونه[i].language (face);
	ثبت->ثبت_score =  all_نومونه[i].score ();
	ثبت->ثبت_شاخص = i;
      }

      هذا->نومونه.qsort (hb_ot_name_ثبت_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      بینښه صحيح j = 0;
      لكل (بینښه صحيح i = 0; i < هذا->نومونه.اوږدتیا; i++)
      {
	إذا (هذا->نومونه[i].ثبت_score == UNSUPPORTED ||
	    هذا->نومونه[i].language == HB_LANGUAGE_INVALID)
	  استمر;
	إذا (i &&
	    هذا->نومونه[i - 1].name_id  == هذا->نومونه[i].name_id &&
	    هذا->نومونه[i - 1].language == هذا->نومونه[i].language)
	  استمر;
	هذا->نومونه[j++] = هذا->نومونه[i];
      }
      هذا->نومونه.resize (j);
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    صحيح get_شاخص (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   بینښه صحيح    *پلنوالی=تش_اشاره) ثابت
    {
      ثابت hb_ot_name_ثبت_t key = {name_id, {0}, language};
      ثابت hb_ot_name_ثبت_t *ثبت = hb_bsearch (key, (ثابت hb_ot_name_ثبت_t *) هذا->نومونه,
						    هذا->نومونه.اوږدتیا,
						    حجم (hb_ot_name_ثبت_t),
						    hb_ot_name_ثبت_cmp_کلی,
						    ریښتیا);

      إذا (!ثبت)
      {
	ثبت = hb_bsearch (key, (ثابت hb_ot_name_ثبت_t *) هذا->نومونه,
			    هذا->نومونه.اوږدتیا,
			    حجم (hb_ot_name_ثبت_t),
			    hb_ot_name_ثبت_cmp_کلی,
			    خطأ);
      }

      إذا (!ثبت)
	أرجع -1;

      إذا (پلنوالی)
	*پلنوالی = ثبت->ثبت_score < 10 ? 2 : 1;

      أرجع ثبت->ثبت_شاخص;
    }

    hb_بایټونه_t get_name (بینښه صحيح idx) ثابت
    {
      ثابت hb_array_t<ثابت Name_ریکارډ> all_نومونه (جدول->name_ریکارډ_Z.arrayZ, جدول->شمېر);
      ثابت Name_ریکارډ &ریکارډ = all_نومونه[idx];
      ثابت hb_بایټونه_t string_pool (pool, pool_len);
      أرجع string_pool.sub_array (ریکارډ.دننه_واټن, ریکارډ.اوږدتیا);
    }

    خاص:
    ثابت حرف *pool;
    بینښه صحيح pool_len;
    عام:
    hb_blob_ptr_t<name> جدول;
    hb_ویکټور_t<hb_ot_name_ثبت_t> نومونه;
  };

  عام:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	شمېر;		/* Number of name records. */
  NNOffset16_منزل<Unsized_Array_د<HBUINT8>>
		string_دننه_واټن;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_د<Name_ریکارډ>
		name_ریکارډ_Z;	/* The name records where count is the number of records. */
  عام:
  DEFINE_اندازه_ARRAY (6, name_ریکارډ_Z);
};

#تعریف_لغوه ثبت_شاخص
#تعریف_لغوه ثبت_score

بنية name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#د_که_پای /* OT_NAME_NAME_HH */
