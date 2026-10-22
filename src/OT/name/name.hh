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

#నిర్వచితం_కాకపోతే OT_NAME_NAME_HH
#నిర్వచించు OT_NAME_NAME_HH

#అన్తర్భావ "../../hb-open-type.hh"
#అన్తర్భావ "../../hb-ot-name-language.hh"
#అన్తర్భావ "../../hb-aat-layout.hh"
#అన్తర్భావ "../../hb-utf.hh"


నామాకాశ OT {

మూస <రకం_పేరు లోపల_utf_t, రకం_పేరు బహిర్గతం_utf_t>
అంతఃవరుస గుర్తులేని పూర్ణాంకం
hb_ot_name_convert_utf (hb_బైట్లు_t                       బైట్లు,
			గుర్తులేని పూర్ణాంకం                    *పాఠ్యం_కొలత /* IN/OUT */,
			రకం_పేరు బహిర్గతం_utf_t::codepoint_t *పాఠ్యం /*బహిర్గతం*/)
{
  గుర్తులేని పూర్ణాంకం src_len = బైట్లు.నిడివి / పరిమాణం (రకం_పేరు లోపల_utf_t::codepoint_t);
  స్థిరరాశి రకం_పేరు లోపల_utf_t::codepoint_t *src = (స్థిరరాశి రకం_పేరు లోపల_utf_t::codepoint_t *) బైట్లు.arrayZ;
  స్థిరరాశి రకం_పేరు లోపల_utf_t::codepoint_t *src_చివర = src + src_len;

  రకం_పేరు బహిర్గతం_utf_t::codepoint_t *dst = పాఠ్యం;

  hb_codepoint_t unicode;
  స్థిరరాశి hb_codepoint_t replacement = HB_బఫరు_REPLACEMENT_CODEPOINT_DEFAULT;

  ఒకవేళ (పాఠ్యం_కొలత && *పాఠ్యం_కొలత)
  {
    (*పాఠ్యం_కొలత)--; /* Save room for NUL-termination. */
    స్థిరరాశి రకం_పేరు బహిర్గతం_utf_t::codepoint_t *dst_చివర = పాఠ్యం + *పాఠ్యం_కొలత;

    అయితే (src < src_చివర && dst < dst_చివర)
    {
      స్థిరరాశి రకం_పేరు లోపల_utf_t::codepoint_t *src_తదుపరి = లోపల_utf_t::next (src, src_చివర, &unicode, replacement);
      రకం_పేరు బహిర్గతం_utf_t::codepoint_t *dst_తదుపరి = బహిర్గతం_utf_t::encode (dst, dst_చివర, unicode);
      ఒకవేళ (dst_తదుపరి == dst)
	ఆపు; /* Out-of-room. */

      dst = dst_తదుపరి;
      src = src_తదుపరి;
    }

    *పాఠ్యం_కొలత = dst - పాఠ్యం;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  గుర్తులేని పూర్ణాంకం dst_len = dst - పాఠ్యం;
  అయితే (src < src_చివర)
  {
    src = లోపల_utf_t::next (src, src_చివర, &unicode, replacement);
    dst_len += బహిర్గతం_utf_t::encode_len (unicode);
  }
  తిరిగి dst_len;
}

#నిర్వచించు నమోదు_score var.u16[0]
#నిర్వచించు నమోదు_సూచీ var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#నిర్వచించు HB_OT_ట్యాగ్_name HB_ట్యాగ్('n','a','m','e')

#నిర్వచించు UNSUPPORTED	42

నిర్మాణం Name_రికార్డు
{
  hb_language_t language (hb_face_t *face) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_OT_NAME_LANGUAGE
    గుర్తులేని పూర్ణాంకం p = platformID;
    గుర్తులేని పూర్ణాంకం l = languageID;

    ఒకవేళ (p == 3)
      తిరిగి hb_ot_name_language_for_ms_సంకేతం (l);

    ఒకవేళ (p == 1)
      తిరిగి hb_ot_name_language_for_mac_సంకేతం (l);

#నిర్వచితం_కాకపోతే HB_NO_OT_NAME_LANGUAGE_AAT
    ఒకవేళ (p == 0)
      తిరిగి face->పట్టిక.ltag->get_language (l);
#అయితే_ముగింపు

#అయితే_ముగింపు
    తిరిగి HB_LANGUAGE_INVALID;
  }

  uint16_t score () స్థిరరాశి
  {
    /* Same order as in cmap::find_best_subtable(). */
    గుర్తులేని పూర్ణాంకం p = platformID;
    గుర్తులేని పూర్ణాంకం e = encodingID;

    /* 32-bit. */
    ఒకవేళ (p == 3 && e == 10) తిరిగి 0;
    ఒకవేళ (p == 0 && e ==  6) తిరిగి 1;
    ఒకవేళ (p == 0 && e ==  4) తిరిగి 2;

    /* 16-bit. */
    ఒకవేళ (p == 3 && e ==  1) తిరిగి 3;
    ఒకవేళ (p == 0 && e ==  3) తిరిగి 4;
    ఒకవేళ (p == 0 && e ==  2) తిరిగి 5;
    ఒకవేళ (p == 0 && e ==  1) తిరిగి 6;
    ఒకవేళ (p == 0 && e ==  0) తిరిగి 7;

    /* Symbol. */
    ఒకవేళ (p == 3 && e ==  0) తిరిగి 8;

    /* We treat all Mac Latin names as ASCII only. */
    ఒకవేళ (p == 1 && e ==  0) తిరిగి 10; /* 10 is magic number :| */

    తిరిగి UNSUPPORTED;
  }

  Name_రికార్డు* copy (hb_serialize_context_t *c, స్థిరరాశి శూన్య *పునాది
#నిర్వచితమైతే HB_EXPERIMENTAL_API
                    , స్థిరరాశి hb_hashmap_t<hb_ot_name_రికార్డు_ids_t, hb_బైట్లు_t> *name_పట్టిక_overrides
#అయితే_ముగింపు
		    ) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    HB_వాడని స్వయంచాలక snap = c->snapshot ();
    స్వయంచాలక *బహిర్గతం = c->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (శూన్యసూచిక);
#నిర్వచితమైతే HB_EXPERIMENTAL_API
    hb_ot_name_రికార్డు_ids_t రికార్డు_ids (platformID, encodingID, languageID, nameID);
    hb_బైట్లు_t* name_బైట్లు;

    ఒకవేళ (name_పట్టిక_overrides->has (రికార్డు_ids, &name_బైట్లు)) {
      hb_బైట్లు_t encoded_బైట్లు = *name_బైట్లు;
      అక్షరం *name_str_utf16_be = శూన్యసూచిక;

      ఒకవేళ (platformID != 1)
      {
        గుర్తులేని పాఠ్యం_కొలత = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_బైట్లు, శూన్యసూచిక, శూన్యసూచిక);

        పాఠ్యం_కొలత++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        గుర్తులేని byte_len = పాఠ్యం_కొలత * hb_utf16_be_t::codepoint_t::static_కొలత;
        name_str_utf16_be = (అక్షరం *) hb_calloc (byte_len, 1);
        ఒకవేళ (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (శూన్యసూచిక);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_బైట్లు, &పాఠ్యం_కొలత,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        గుర్తులేని encoded_byte_len = పాఠ్యం_కొలత * hb_utf16_be_t::codepoint_t::static_కొలత;
        ఒకవేళ (!encoded_byte_len || !c->check_కేటాయించు (బహిర్గతం->నిడివి, encoded_byte_len, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (శూన్యసూచిక);
        }

        encoded_బైట్లు = hb_బైట్లు_t (name_str_utf16_be, encoded_byte_len);
      }
      లేకపోతే
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        ఒకవేళ (!c->check_కేటాయించు (బహిర్గతం->నిడివి, encoded_బైట్లు.నిడివి, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) {
          c->revert (snap);
          return_trace (శూన్యసూచిక);
        }
      }

      బహిర్గతం->స్థానభ్రంశం = 0;
      c->push ();
      encoded_బైట్లు.copy (c);
      c->కూడు_link (బహిర్గతం->స్థానభ్రంశం, c->తీయి_pack (), hb_serialize_context_t::తోక_2, 0);
      hb_free (name_str_utf16_be);
    }
    లేకపోతే
#అయితే_ముగింపు
    {
      బహిర్గతం->స్థానభ్రంశం.serialize_copy (c, స్థానభ్రంశం, పునాది, 0, hb_serialize_context_t::తోక_2, నిడివి);
    }
    return_trace (బహిర్గతం);
  }

  తార్కిక isUnicode () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం p = platformID;
    గుర్తులేని పూర్ణాంకం e = encodingID;

    తిరిగి (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి Name_రికార్డు *a = (స్థిరరాశి Name_రికార్డు *)pa;
    స్థిరరాశి Name_రికార్డు *b = (స్థిరరాశి Name_రికార్డు *)pb;

    ఒకవేళ (a->platformID != b->platformID)
      తిరిగి a->platformID - b->platformID;

    ఒకవేళ (a->encodingID != b->encodingID)
      తిరిగి a->encodingID - b->encodingID;

    ఒకవేళ (a->languageID != b->languageID)
      తిరిగి a->languageID - b->languageID;

    ఒకవేళ (a->nameID != b->nameID)
      తిరిగి a->nameID - b->nameID;

    ఒకవేళ (a->నిడివి != b->నిడివి)
      తిరిగి a->నిడివి - b->నిడివి;

    తిరిగి 0;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  స్థానభ్రంశం.sanitize (c, పునాది, నిడివి));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	నిడివి;		/* String length (in bytes). */
  NNOffset16_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>
		స్థానభ్రంశం;		/* String offset from start of storage area (in bytes). */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

స్థిర పూర్ణాంకం
hb_ot_name_నమోదు_cmp_కీలకం (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb, తార్కిక exact)
{
  స్థిరరాశి hb_ot_name_నమోదు_t *a = (స్థిరరాశి hb_ot_name_నమోదు_t *) pa;
  స్థిరరాశి hb_ot_name_నమోదు_t *b = (స్థిరరాశి hb_ot_name_నమోదు_t *) pb;

  /* Compare by name_id, then language. */

  ఒకవేళ (a->name_id != b->name_id)
    తిరిగి a->name_id - b->name_id;

  ఒకవేళ (a->language == b->language) తిరిగి 0;
  ఒకవేళ (!a->language) తిరిగి -1;
  ఒకవేళ (!b->language) తిరిగి +1;

  స్థిరరాశి అక్షరం *astr = hb_language_గమ్యం_string (a->language);
  స్థిరరాశి అక్షరం *bstr = hb_language_గమ్యం_string (b->language);

  గుర్తుగల c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  ఒకవేళ (!exact && c &&
      hb_language_matches (b->language, a->language))
    తిరిగి 0;

  తిరిగి c;
}

స్థిర పూర్ణాంకం
hb_ot_name_నమోదు_cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  పూర్ణాంకం v = hb_ot_name_నమోదు_cmp_కీలకం (pa, pb, నిజం);
  ఒకవేళ (v)
    తిరిగి v;

  స్థిరరాశి hb_ot_name_నమోదు_t *a = (స్థిరరాశి hb_ot_name_నమోదు_t *) pa;
  స్థిరరాశి hb_ot_name_నమోదు_t *b = (స్థిరరాశి hb_ot_name_నమోదు_t *) pb;

  ఒకవేళ (a->నమోదు_score != b->నమోదు_score)
    తిరిగి a->నమోదు_score - b->నమోదు_score;

  ఒకవేళ (a->నమోదు_సూచీ != b->నమోదు_సూచీ)
    తిరిగి a->నమోదు_సూచీ - b->నమోదు_సూచీ;

  తిరిగి 0;
}

నిర్మాణం name
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_name;

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి min_కొలత + లెక్క * name_రికార్డు_Z.అంశం_కొలత; }

  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_ఆధారం_యొక్క (Iterator, స్థిరరాశి Name_రికార్డు &))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it,
		  స్థిరరాశి శూన్య *src_string_pool
#నిర్వచితమైతే HB_EXPERIMENTAL_API
                  , స్థిరరాశి hb_సదిశ_t<hb_ot_name_రికార్డు_ids_t>& చొప్పించు_name_రికార్డులు
		  , స్థిరరాశి hb_hashmap_t<hb_ot_name_రికార్డు_ids_t, hb_బైట్లు_t> *name_పట్టిక_overrides
#అయితే_ముగింపు
		  )
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (unlikely (!c->extend_min ((*ఇది))))  return_trace (అబద్ధం);

    గుర్తులేని మొత్తం_లెక్క = it.len ()
#నిర్వచితమైతే HB_EXPERIMENTAL_API
        + చొప్పించు_name_రికార్డులు.నిడివి
#అయితే_ముగింపు
        ;
    ఇది->format = 0;
    ఒకవేళ (!c->check_కేటాయించు (ఇది->లెక్క, మొత్తం_లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      తిరిగి అబద్ధం;

    Name_రికార్డు *name_రికార్డులు = (Name_రికార్డు *) hb_calloc (మొత్తం_లెక్క, Name_రికార్డు::static_కొలత);
    ఒకవేళ (unlikely (!name_రికార్డులు)) return_trace (అబద్ధం);

    hb_array_t<Name_రికార్డు> రికార్డులు (name_రికార్డులు, మొత్తం_లెక్క);

    కొరకు (స్థిరరాశి Name_రికార్డు& రికార్డు : it)
    {
      hb_memcpy (name_రికార్డులు, &రికార్డు, Name_రికార్డు::static_కొలత);
      name_రికార్డులు++;
    }

#నిర్వచితమైతే HB_EXPERIMENTAL_API
    కొరకు (గుర్తులేని i = 0; i < చొప్పించు_name_రికార్డులు.నిడివి; i++)
    {
      స్థిరరాశి hb_ot_name_రికార్డు_ids_t& ids = చొప్పించు_name_రికార్డులు[i];
      Name_రికార్డు రికార్డు;
      రికార్డు.platformID = ids.platform_id;
      రికార్డు.encodingID = ids.encoding_id;
      రికార్డు.languageID = ids.language_id;
      రికార్డు.nameID = ids.name_id;
      రికార్డు.నిడివి = 0; // handled in NameRecord copy()
      రికార్డు.స్థానభ్రంశం = 0;
      hb_memcpy (name_రికార్డులు, &రికార్డు, Name_రికార్డు::static_కొలత);
      name_రికార్డులు++;
    }
#అయితే_ముగింపు

    రికార్డులు.qsort ();

    c->copy_all (రికార్డులు,
		 src_string_pool
#నిర్వచితమైతే HB_EXPERIMENTAL_API
		 , name_పట్టిక_overrides
#అయితే_ముగింపు
		 );
    hb_free (రికార్డులు.arrayZ);


    ఒకవేళ (unlikely (c->ran_బహిర్గతం_యొక్క_స్థలం ())) return_trace (అబద్ధం);

    ఇది->string_స్థానభ్రంశం = c->నిడివి ();

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    స్వయంచాలక *name_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<name> ();

#నిర్వచితమైతే HB_EXPERIMENTAL_API
    స్థిరరాశి hb_hashmap_t<hb_ot_name_రికార్డు_ids_t, hb_బైట్లు_t> *name_పట్టిక_overrides =
        &c->plan->name_పట్టిక_overrides;
#అయితే_ముగింపు

    స్వయంచాలక it =
    + name_రికార్డు_Z.as_array (లెక్క)
    | hb_filter (c->plan->name_ids, &Name_రికార్డు::nameID)
    | hb_filter (c->plan->name_languages, &Name_రికార్డు::languageID)
    | hb_filter ([&] (స్థిరరాశి Name_రికార్డు& namerecord) {
      తిరిగి
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#నిర్వచితమైతే HB_EXPERIMENTAL_API
    | hb_filter ([&] (స్థిరరాశి Name_రికార్డు& namerecord) {
      ఒకవేళ (name_పట్టిక_overrides->is_empty ())
        తిరిగి నిజం;
      hb_ot_name_రికార్డు_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_బైట్లు_t *p;
      ఒకవేళ (name_పట్టిక_overrides->has (rec_ids, &p) &&
          (*p).నిడివి == 0)
        తిరిగి అబద్ధం;
      తిరిగి నిజం;
    })
#అయితే_ముగింపు
    ;

#నిర్వచితమైతే HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_రికార్డు_ids_t, గుర్తులేని> retained_name_రికార్డు_ids;
    కొరకు (స్థిరరాశి Name_రికార్డు& rec : it)
    {
      hb_ot_name_రికార్డు_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_రికార్డు_ids.సమితి (rec_ids, 1);
    }

    hb_సదిశ_t<hb_ot_name_రికార్డు_ids_t> చొప్పించు_name_రికార్డులు;
    ఒకవేళ (!name_పట్టిక_overrides->is_empty ())
    {
      ఒకవేళ (unlikely (!చొప్పించు_name_రికార్డులు.alloc (name_పట్టిక_overrides->get_population (), నిజం)))
        తిరిగి అబద్ధం;
      కొరకు (స్థిరరాశి స్వయంచాలక& రికార్డు_ids : name_పట్టిక_overrides->కీలకాలు ())
      {
        ఒకవేళ (name_పట్టిక_overrides->get (రికార్డు_ids).నిడివి == 0)
          కొనసాగు;
        ఒకవేళ (retained_name_రికార్డు_ids.has (రికార్డు_ids))
          కొనసాగు;
        చొప్పించు_name_రికార్డులు.push (రికార్డు_ids);
      }
    }
#అయితే_ముగింపు

    తిరిగి name_ప్రధానసంఖ్య->serialize (c->serializer, it,
				  std::addressof (ఇది + string_స్థానభ్రంశం)
#నిర్వచితమైతే HB_EXPERIMENTAL_API
				  , చొప్పించు_name_రికార్డులు
				  , name_పట్టిక_overrides
#అయితే_ముగింపు
				  );
  }

  తార్కిక sanitize_రికార్డులు (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    స్థిరరాశి శూన్య *string_pool = (ఇది+string_స్థానభ్రంశం).arrayZ;
    return_trace (name_రికార్డు_Z.sanitize (c, లెక్క, string_pool));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_రికార్డు_Z.arrayZ, లెక్క) &&
		  c->check_range (ఇది, string_స్థానభ్రంశం) &&
		  sanitize_రికార్డులు (c));
  }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ఇది->పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<name> (face);
      ప్రతిజ్ఞా (ఇది->పట్టిక.get_నిడివి () >= ఇది->పట్టిక->string_స్థానభ్రంశం);
      ఇది->pool = (స్థిరరాశి అక్షరం *) (స్థిరరాశి శూన్య *) (ఇది->పట్టిక+ఇది->పట్టిక->string_స్థానభ్రంశం);
      ఇది->pool_len = ఇది->పట్టిక.get_నిడివి () - ఇది->పట్టిక->string_స్థానభ్రంశం;
      స్థిరరాశి hb_array_t<స్థిరరాశి Name_రికార్డు> all_పేర్లు (ఇది->పట్టిక->name_రికార్డు_Z.arrayZ,
						    ఇది->పట్టిక->లెక్క);

      ఇది->పేర్లు.alloc_exact (all_పేర్లు.నిడివి);

      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < all_పేర్లు.నిడివి; i++)
      {
	hb_ot_name_నమోదు_t *నమోదు_2 = ఇది->పేర్లు.push ();

	నమోదు_2->name_id = all_పేర్లు[i].nameID;
	నమోదు_2->language = all_పేర్లు[i].language (face);
	నమోదు_2->నమోదు_score =  all_పేర్లు[i].score ();
	నమోదు_2->నమోదు_సూచీ = i;
      }

      ఇది->పేర్లు.qsort (hb_ot_name_నమోదు_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      గుర్తులేని పూర్ణాంకం j = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ఇది->పేర్లు.నిడివి; i++)
      {
	ఒకవేళ (ఇది->పేర్లు[i].నమోదు_score == UNSUPPORTED ||
	    ఇది->పేర్లు[i].language == HB_LANGUAGE_INVALID)
	  కొనసాగు;
	ఒకవేళ (i &&
	    ఇది->పేర్లు[i - 1].name_id  == ఇది->పేర్లు[i].name_id &&
	    ఇది->పేర్లు[i - 1].language == ఇది->పేర్లు[i].language)
	  కొనసాగు;
	ఇది->పేర్లు[j++] = ఇది->పేర్లు[i];
      }
      ఇది->పేర్లు.resize (j);
    }
    ~accelerator_t ()
    {
      ఇది->పట్టిక.destroy ();
    }

    పూర్ణాంకం get_సూచీ (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   గుర్తులేని పూర్ణాంకం    *వెడల్పు=శూన్యసూచిక) స్థిరరాశి
    {
      స్థిరరాశి hb_ot_name_నమోదు_t key = {name_id, {0}, language};
      స్థిరరాశి hb_ot_name_నమోదు_t *నమోదు_2 = hb_bsearch (key, (స్థిరరాశి hb_ot_name_నమోదు_t *) ఇది->పేర్లు,
						    ఇది->పేర్లు.నిడివి,
						    పరిమాణం (hb_ot_name_నమోదు_t),
						    hb_ot_name_నమోదు_cmp_కీలకం,
						    నిజం);

      ఒకవేళ (!నమోదు_2)
      {
	నమోదు_2 = hb_bsearch (key, (స్థిరరాశి hb_ot_name_నమోదు_t *) ఇది->పేర్లు,
			    ఇది->పేర్లు.నిడివి,
			    పరిమాణం (hb_ot_name_నమోదు_t),
			    hb_ot_name_నమోదు_cmp_కీలకం,
			    అబద్ధం);
      }

      ఒకవేళ (!నమోదు_2)
	తిరిగి -1;

      ఒకవేళ (వెడల్పు)
	*వెడల్పు = నమోదు_2->నమోదు_score < 10 ? 2 : 1;

      తిరిగి నమోదు_2->నమోదు_సూచీ;
    }

    hb_బైట్లు_t get_name (గుర్తులేని పూర్ణాంకం idx) స్థిరరాశి
    {
      స్థిరరాశి hb_array_t<స్థిరరాశి Name_రికార్డు> all_పేర్లు (పట్టిక->name_రికార్డు_Z.arrayZ, పట్టిక->లెక్క);
      స్థిరరాశి Name_రికార్డు &రికార్డు = all_పేర్లు[idx];
      స్థిరరాశి hb_బైట్లు_t string_pool (pool, pool_len);
      తిరిగి string_pool.sub_array (రికార్డు.స్థానభ్రంశం, రికార్డు.నిడివి);
    }

    అంతరంగ:
    స్థిరరాశి అక్షరం *pool;
    గుర్తులేని పూర్ణాంకం pool_len;
    బహిరంగ:
    hb_blob_ptr_t<name> పట్టిక;
    hb_సదిశ_t<hb_ot_name_నమోదు_t> పేర్లు;
  };

  బహిరంగ:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	లెక్క;		/* Number of name records. */
  NNOffset16_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>
		string_స్థానభ్రంశం;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_యొక్క<Name_రికార్డు>
		name_రికార్డు_Z;	/* The name records where count is the number of records. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, name_రికార్డు_Z);
};

#నిర్వచనం_రద్దు నమోదు_సూచీ
#నిర్వచనం_రద్దు నమోదు_score

నిర్మాణం name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#అయితే_ముగింపు /* OT_NAME_NAME_HH */
