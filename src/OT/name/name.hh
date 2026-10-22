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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_NAME_NAME_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_NAME_NAME_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-ot-name-language.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-aat-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-utf.hh"


ਨਾਮਾਕਾਸ਼ OT {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਵਿੱਚ_utf_t, ਕਿਸਮ_ਨਾਂ ਬਾਹਰਲਾ_utf_t>
ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_name_convert_utf (hb_ਬਾਈਟਾਂ_t                       ਬਾਈਟਾਂ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                    *ਪਾਠ_ਮਾਪ /* IN/OUT */,
			ਕਿਸਮ_ਨਾਂ ਬਾਹਰਲਾ_utf_t::codepoint_t *ਪਾਠ /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ src_len = ਬਾਈਟਾਂ.ਲੰਮਾਈ / ਆਕਾਰ (ਕਿਸਮ_ਨਾਂ ਵਿੱਚ_utf_t::codepoint_t);
  ਅਚਲ ਕਿਸਮ_ਨਾਂ ਵਿੱਚ_utf_t::codepoint_t *src = (ਅਚਲ ਕਿਸਮ_ਨਾਂ ਵਿੱਚ_utf_t::codepoint_t *) ਬਾਈਟਾਂ.arrayZ;
  ਅਚਲ ਕਿਸਮ_ਨਾਂ ਵਿੱਚ_utf_t::codepoint_t *src_ਸਿਰਾ = src + src_len;

  ਕਿਸਮ_ਨਾਂ ਬਾਹਰਲਾ_utf_t::codepoint_t *dst = ਪਾਠ;

  hb_codepoint_t unicode;
  ਅਚਲ hb_codepoint_t replacement = HB_ਬਫਰ_REPLACEMENT_CODEPOINT_DEFAULT;

  ਜੇ (ਪਾਠ_ਮਾਪ && *ਪਾਠ_ਮਾਪ)
  {
    (*ਪਾਠ_ਮਾਪ)--; /* Save room for NUL-termination. */
    ਅਚਲ ਕਿਸਮ_ਨਾਂ ਬਾਹਰਲਾ_utf_t::codepoint_t *dst_ਸਿਰਾ = ਪਾਠ + *ਪਾਠ_ਮਾਪ;

    ਜਦੋਂ (src < src_ਸਿਰਾ && dst < dst_ਸਿਰਾ)
    {
      ਅਚਲ ਕਿਸਮ_ਨਾਂ ਵਿੱਚ_utf_t::codepoint_t *src_ਅਗਲਾ = ਵਿੱਚ_utf_t::next (src, src_ਸਿਰਾ, &unicode, replacement);
      ਕਿਸਮ_ਨਾਂ ਬਾਹਰਲਾ_utf_t::codepoint_t *dst_ਅਗਲਾ = ਬਾਹਰਲਾ_utf_t::encode (dst, dst_ਸਿਰਾ, unicode);
      ਜੇ (dst_ਅਗਲਾ == dst)
	ਰੋਕੋ; /* Out-of-room. */

      dst = dst_ਅਗਲਾ;
      src = src_ਅਗਲਾ;
    }

    *ਪਾਠ_ਮਾਪ = dst - ਪਾਠ;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dst_len = dst - ਪਾਠ;
  ਜਦੋਂ (src < src_ਸਿਰਾ)
  {
    src = ਵਿੱਚ_utf_t::next (src, src_ਸਿਰਾ, &unicode, replacement);
    dst_len += ਬਾਹਰਲਾ_utf_t::encode_len (unicode);
  }
  ਵਾਪਸ dst_len;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਇੰਦਰਾਜ_score var.u16[0]
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_name HB_ਟੈਗ('n','a','m','e')

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNSUPPORTED	42

ਬਣਤਰ Name_ਰਿਕਾਰਡ
{
  hb_language_t language (hb_face_t *face) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_NAME_LANGUAGE
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ p = platformID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = languageID;

    ਜੇ (p == 3)
      ਵਾਪਸ hb_ot_name_language_for_ms_ਕੋਡ (l);

    ਜੇ (p == 1)
      ਵਾਪਸ hb_ot_name_language_for_mac_ਕੋਡ (l);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_NAME_LANGUAGE_AAT
    ਜੇ (p == 0)
      ਵਾਪਸ face->ਸਾਰਣੀ.ltag->get_language (l);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
    ਵਾਪਸ HB_LANGUAGE_INVALID;
  }

  uint16_t score () ਅਚਲ
  {
    /* Same order as in cmap::find_best_subtable(). */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ p = platformID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ e = encodingID;

    /* 32-bit. */
    ਜੇ (p == 3 && e == 10) ਵਾਪਸ 0;
    ਜੇ (p == 0 && e ==  6) ਵਾਪਸ 1;
    ਜੇ (p == 0 && e ==  4) ਵਾਪਸ 2;

    /* 16-bit. */
    ਜੇ (p == 3 && e ==  1) ਵਾਪਸ 3;
    ਜੇ (p == 0 && e ==  3) ਵਾਪਸ 4;
    ਜੇ (p == 0 && e ==  2) ਵਾਪਸ 5;
    ਜੇ (p == 0 && e ==  1) ਵਾਪਸ 6;
    ਜੇ (p == 0 && e ==  0) ਵਾਪਸ 7;

    /* Symbol. */
    ਜੇ (p == 3 && e ==  0) ਵਾਪਸ 8;

    /* We treat all Mac Latin names as ASCII only. */
    ਜੇ (p == 1 && e ==  0) ਵਾਪਸ 10; /* 10 is magic number :| */

    ਵਾਪਸ UNSUPPORTED;
  }

  Name_ਰਿਕਾਰਡ* copy (hb_serialize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
                    , ਅਚਲ hb_hashmap_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t, hb_ਬਾਈਟਾਂ_t> *name_ਸਾਰਣੀ_overrides
#ਜੇ_ਅੰਤ
		    ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    HB_ਅਣਵਰਤਿਆ ਸਵੈਚਾਲਿਤ snap = c->snapshot ();
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    hb_ot_name_ਰਿਕਾਰਡ_ids_t ਰਿਕਾਰਡ_ids (platformID, encodingID, languageID, nameID);
    hb_ਬਾਈਟਾਂ_t* name_ਬਾਈਟਾਂ;

    ਜੇ (name_ਸਾਰਣੀ_overrides->has (ਰਿਕਾਰਡ_ids, &name_ਬਾਈਟਾਂ)) {
      hb_ਬਾਈਟਾਂ_t encoded_ਬਾਈਟਾਂ = *name_ਬਾਈਟਾਂ;
      ਅੱਖਰ *name_str_utf16_be = ਖਾਲੀ_ਸੂਚਕ;

      ਜੇ (platformID != 1)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਠ_ਮਾਪ = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_ਬਾਈਟਾਂ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

        ਪਾਠ_ਮਾਪ++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        ਬਿਨਾਨਿਸ਼ਾਨੀ byte_len = ਪਾਠ_ਮਾਪ * hb_utf16_be_t::codepoint_t::static_ਮਾਪ;
        name_str_utf16_be = (ਅੱਖਰ *) hb_calloc (byte_len, 1);
        ਜੇ (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (ਖਾਲੀ_ਸੂਚਕ);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_ਬਾਈਟਾਂ, &ਪਾਠ_ਮਾਪ,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_byte_len = ਪਾਠ_ਮਾਪ * hb_utf16_be_t::codepoint_t::static_ਮਾਪ;
        ਜੇ (!encoded_byte_len || !c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਲੰਮਾਈ, encoded_byte_len, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (ਖਾਲੀ_ਸੂਚਕ);
        }

        encoded_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t (name_str_utf16_be, encoded_byte_len);
      }
      ਨਹੀਂਤਾਂ
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        ਜੇ (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਲੰਮਾਈ, encoded_ਬਾਈਟਾਂ.ਲੰਮਾਈ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) {
          c->revert (snap);
          return_trace (ਖਾਲੀ_ਸੂਚਕ);
        }
      }

      ਬਾਹਰਲਾ->ਵਿੱਥ = 0;
      c->push ();
      encoded_ਬਾਈਟਾਂ.copy (c);
      c->ਜੋੜੋ_link (ਬਾਹਰਲਾ->ਵਿੱਥ, c->ਕੱਢੋ_pack (), hb_serialize_context_t::ਪੂਛ_2, 0);
      hb_free (name_str_utf16_be);
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
      ਬਾਹਰਲਾ->ਵਿੱਥ.serialize_copy (c, ਵਿੱਥ, ਅਧਾਰ, 0, hb_serialize_context_t::ਪੂਛ_2, ਲੰਮਾਈ);
    }
    return_trace (ਬਾਹਰਲਾ);
  }

  ਤਾਰਕਿਕ isUnicode () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ p = platformID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ e = encodingID;

    ਵਾਪਸ (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ Name_ਰਿਕਾਰਡ *a = (ਅਚਲ Name_ਰਿਕਾਰਡ *)pa;
    ਅਚਲ Name_ਰਿਕਾਰਡ *b = (ਅਚਲ Name_ਰਿਕਾਰਡ *)pb;

    ਜੇ (a->platformID != b->platformID)
      ਵਾਪਸ a->platformID - b->platformID;

    ਜੇ (a->encodingID != b->encodingID)
      ਵਾਪਸ a->encodingID - b->encodingID;

    ਜੇ (a->languageID != b->languageID)
      ਵਾਪਸ a->languageID - b->languageID;

    ਜੇ (a->nameID != b->nameID)
      ਵਾਪਸ a->nameID - b->nameID;

    ਜੇ (a->ਲੰਮਾਈ != b->ਲੰਮਾਈ)
      ਵਾਪਸ a->ਲੰਮਾਈ - b->ਲੰਮਾਈ;

    ਵਾਪਸ 0;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਵਿੱਥ.sanitize (c, ਅਧਾਰ, ਲੰਮਾਈ));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	ਲੰਮਾਈ;		/* String length (in bytes). */
  NNOffset16_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>
		ਵਿੱਥ;		/* String offset from start of storage area (in bytes). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਸਥਿਰ ਪੂਰਨ
hb_ot_name_ਇੰਦਰਾਜ_cmp_ਕੁੰਜੀ (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb, ਤਾਰਕਿਕ exact)
{
  ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *a = (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) pa;
  ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *b = (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) pb;

  /* Compare by name_id, then language. */

  ਜੇ (a->name_id != b->name_id)
    ਵਾਪਸ a->name_id - b->name_id;

  ਜੇ (a->language == b->language) ਵਾਪਸ 0;
  ਜੇ (!a->language) ਵਾਪਸ -1;
  ਜੇ (!b->language) ਵਾਪਸ +1;

  ਅਚਲ ਅੱਖਰ *astr = hb_language_ਮੰਜ਼ਿਲ_string (a->language);
  ਅਚਲ ਅੱਖਰ *bstr = hb_language_ਮੰਜ਼ਿਲ_string (b->language);

  ਨਿਸ਼ਾਨੀਵਾਲਾ c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  ਜੇ (!exact && c &&
      hb_language_matches (b->language, a->language))
    ਵਾਪਸ 0;

  ਵਾਪਸ c;
}

ਸਥਿਰ ਪੂਰਨ
hb_ot_name_ਇੰਦਰਾਜ_cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  ਪੂਰਨ v = hb_ot_name_ਇੰਦਰਾਜ_cmp_ਕੁੰਜੀ (pa, pb, ਸੱਚ);
  ਜੇ (v)
    ਵਾਪਸ v;

  ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *a = (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) pa;
  ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *b = (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) pb;

  ਜੇ (a->ਇੰਦਰਾਜ_score != b->ਇੰਦਰਾਜ_score)
    ਵਾਪਸ a->ਇੰਦਰਾਜ_score - b->ਇੰਦਰਾਜ_score;

  ਜੇ (a->ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ != b->ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ)
    ਵਾਪਸ a->ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ - b->ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ;

  ਵਾਪਸ 0;
}

ਬਣਤਰ name
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_name;

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ min_ਮਾਪ + ਗਿਣਤੀ_2 * name_ਰਿਕਾਰਡ_Z.ਵਸਤੂ_ਮਾਪ; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, ਅਚਲ Name_ਰਿਕਾਰਡ &))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ਅਚਲ ਖਾਲੀ *src_string_pool
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
                  , ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t>& ਪਾਓ_name_ਰਿਕਾਰਡਾਂ
		  , ਅਚਲ hb_hashmap_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t, hb_ਬਾਈਟਾਂ_t> *name_ਸਾਰਣੀ_overrides
#ਜੇ_ਅੰਤ
		  )
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (unlikely (!c->extend_min ((*ਇਹ))))  return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਗਿਣਤੀ = it.len ()
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
        + ਪਾਓ_name_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ
#ਜੇ_ਅੰਤ
        ;
    ਇਹ->format = 0;
    ਜੇ (!c->check_ਸੌਂਪੋ (ਇਹ->ਗਿਣਤੀ_2, ਕੁੱਲ_ਗਿਣਤੀ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      ਵਾਪਸ ਝੂਠ;

    Name_ਰਿਕਾਰਡ *name_ਰਿਕਾਰਡਾਂ = (Name_ਰਿਕਾਰਡ *) hb_calloc (ਕੁੱਲ_ਗਿਣਤੀ, Name_ਰਿਕਾਰਡ::static_ਮਾਪ);
    ਜੇ (unlikely (!name_ਰਿਕਾਰਡਾਂ)) return_trace (ਝੂਠ);

    hb_array_t<Name_ਰਿਕਾਰਡ> ਰਿਕਾਰਡਾਂ (name_ਰਿਕਾਰਡਾਂ, ਕੁੱਲ_ਗਿਣਤੀ);

    ਲਈ (ਅਚਲ Name_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : it)
    {
      hb_memcpy (name_ਰਿਕਾਰਡਾਂ, &ਰਿਕਾਰਡ, Name_ਰਿਕਾਰਡ::static_ਮਾਪ);
      name_ਰਿਕਾਰਡਾਂ++;
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਪਾਓ_name_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
    {
      ਅਚਲ hb_ot_name_ਰਿਕਾਰਡ_ids_t& ids = ਪਾਓ_name_ਰਿਕਾਰਡਾਂ[i];
      Name_ਰਿਕਾਰਡ ਰਿਕਾਰਡ;
      ਰਿਕਾਰਡ.platformID = ids.platform_id;
      ਰਿਕਾਰਡ.encodingID = ids.encoding_id;
      ਰਿਕਾਰਡ.languageID = ids.language_id;
      ਰਿਕਾਰਡ.nameID = ids.name_id;
      ਰਿਕਾਰਡ.ਲੰਮਾਈ = 0; // handled in NameRecord copy()
      ਰਿਕਾਰਡ.ਵਿੱਥ = 0;
      hb_memcpy (name_ਰਿਕਾਰਡਾਂ, &ਰਿਕਾਰਡ, Name_ਰਿਕਾਰਡ::static_ਮਾਪ);
      name_ਰਿਕਾਰਡਾਂ++;
    }
#ਜੇ_ਅੰਤ

    ਰਿਕਾਰਡਾਂ.qsort ();

    c->copy_all (ਰਿਕਾਰਡਾਂ,
		 src_string_pool
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
		 , name_ਸਾਰਣੀ_overrides
#ਜੇ_ਅੰਤ
		 );
    hb_free (ਰਿਕਾਰਡਾਂ.arrayZ);


    ਜੇ (unlikely (c->ran_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼ ())) return_trace (ਝੂਠ);

    ਇਹ->string_ਵਿੱਥ = c->ਲੰਮਾਈ ();

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ *name_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<name> ();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    ਅਚਲ hb_hashmap_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t, hb_ਬਾਈਟਾਂ_t> *name_ਸਾਰਣੀ_overrides =
        &c->plan->name_ਸਾਰਣੀ_overrides;
#ਜੇ_ਅੰਤ

    ਸਵੈਚਾਲਿਤ it =
    + name_ਰਿਕਾਰਡ_Z.as_array (ਗਿਣਤੀ_2)
    | hb_filter (c->plan->name_ids, &Name_ਰਿਕਾਰਡ::nameID)
    | hb_filter (c->plan->name_languages, &Name_ਰਿਕਾਰਡ::languageID)
    | hb_filter ([&] (ਅਚਲ Name_ਰਿਕਾਰਡ& namerecord) {
      ਵਾਪਸ
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    | hb_filter ([&] (ਅਚਲ Name_ਰਿਕਾਰਡ& namerecord) {
      ਜੇ (name_ਸਾਰਣੀ_overrides->is_empty ())
        ਵਾਪਸ ਸੱਚ;
      hb_ot_name_ਰਿਕਾਰਡ_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_ਬਾਈਟਾਂ_t *p;
      ਜੇ (name_ਸਾਰਣੀ_overrides->has (rec_ids, &p) &&
          (*p).ਲੰਮਾਈ == 0)
        ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ ਸੱਚ;
    })
#ਜੇ_ਅੰਤ
    ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> retained_name_ਰਿਕਾਰਡ_ids;
    ਲਈ (ਅਚਲ Name_ਰਿਕਾਰਡ& rec : it)
    {
      hb_ot_name_ਰਿਕਾਰਡ_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_ਰਿਕਾਰਡ_ids.ਸੈੱਟ (rec_ids, 1);
    }

    hb_ਸਦਿਸ਼_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t> ਪਾਓ_name_ਰਿਕਾਰਡਾਂ;
    ਜੇ (!name_ਸਾਰਣੀ_overrides->is_empty ())
    {
      ਜੇ (unlikely (!ਪਾਓ_name_ਰਿਕਾਰਡਾਂ.alloc (name_ਸਾਰਣੀ_overrides->get_population (), ਸੱਚ)))
        ਵਾਪਸ ਝੂਠ;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਰਿਕਾਰਡ_ids : name_ਸਾਰਣੀ_overrides->ਕੁੰਜੀਆਂ ())
      {
        ਜੇ (name_ਸਾਰਣੀ_overrides->get (ਰਿਕਾਰਡ_ids).ਲੰਮਾਈ == 0)
          ਜਾਰੀ;
        ਜੇ (retained_name_ਰਿਕਾਰਡ_ids.has (ਰਿਕਾਰਡ_ids))
          ਜਾਰੀ;
        ਪਾਓ_name_ਰਿਕਾਰਡਾਂ.push (ਰਿਕਾਰਡ_ids);
      }
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ name_ਅਭਾਜ->serialize (c->serializer, it,
				  std::addressof (ਇਹ + string_ਵਿੱਥ)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
				  , ਪਾਓ_name_ਰਿਕਾਰਡਾਂ
				  , name_ਸਾਰਣੀ_overrides
#ਜੇ_ਅੰਤ
				  );
  }

  ਤਾਰਕਿਕ sanitize_ਰਿਕਾਰਡਾਂ (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਅਚਲ ਖਾਲੀ *string_pool = (ਇਹ+string_ਵਿੱਥ).arrayZ;
    return_trace (name_ਰਿਕਾਰਡ_Z.sanitize (c, ਗਿਣਤੀ_2, string_pool));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (name_ਰਿਕਾਰਡ_Z.arrayZ, ਗਿਣਤੀ_2) &&
		  c->check_range (ਇਹ, string_ਵਿੱਥ) &&
		  sanitize_ਰਿਕਾਰਡਾਂ (c));
  }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ਇਹ->ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<name> (face);
      ਪ੍ਰਤਿਜ੍ਞਾ (ਇਹ->ਸਾਰਣੀ.get_ਲੰਮਾਈ () >= ਇਹ->ਸਾਰਣੀ->string_ਵਿੱਥ);
      ਇਹ->pool = (ਅਚਲ ਅੱਖਰ *) (ਅਚਲ ਖਾਲੀ *) (ਇਹ->ਸਾਰਣੀ+ਇਹ->ਸਾਰਣੀ->string_ਵਿੱਥ);
      ਇਹ->pool_len = ਇਹ->ਸਾਰਣੀ.get_ਲੰਮਾਈ () - ਇਹ->ਸਾਰਣੀ->string_ਵਿੱਥ;
      ਅਚਲ hb_array_t<ਅਚਲ Name_ਰਿਕਾਰਡ> all_ਨਾਂ (ਇਹ->ਸਾਰਣੀ->name_ਰਿਕਾਰਡ_Z.arrayZ,
						    ਇਹ->ਸਾਰਣੀ->ਗਿਣਤੀ_2);

      ਇਹ->ਨਾਂ.alloc_exact (all_ਨਾਂ.ਲੰਮਾਈ);

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < all_ਨਾਂ.ਲੰਮਾਈ; i++)
      {
	hb_ot_name_ਇੰਦਰਾਜ_t *ਇੰਦਰਾਜ = ਇਹ->ਨਾਂ.push ();

	ਇੰਦਰਾਜ->name_id = all_ਨਾਂ[i].nameID;
	ਇੰਦਰਾਜ->language = all_ਨਾਂ[i].language (face);
	ਇੰਦਰਾਜ->ਇੰਦਰਾਜ_score =  all_ਨਾਂ[i].score ();
	ਇੰਦਰਾਜ->ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ = i;
      }

      ਇਹ->ਨਾਂ.qsort (hb_ot_name_ਇੰਦਰਾਜ_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਇਹ->ਨਾਂ.ਲੰਮਾਈ; i++)
      {
	ਜੇ (ਇਹ->ਨਾਂ[i].ਇੰਦਰਾਜ_score == UNSUPPORTED ||
	    ਇਹ->ਨਾਂ[i].language == HB_LANGUAGE_INVALID)
	  ਜਾਰੀ;
	ਜੇ (i &&
	    ਇਹ->ਨਾਂ[i - 1].name_id  == ਇਹ->ਨਾਂ[i].name_id &&
	    ਇਹ->ਨਾਂ[i - 1].language == ਇਹ->ਨਾਂ[i].language)
	  ਜਾਰੀ;
	ਇਹ->ਨਾਂ[j++] = ਇਹ->ਨਾਂ[i];
      }
      ਇਹ->ਨਾਂ.resize (j);
    }
    ~accelerator_t ()
    {
      ਇਹ->ਸਾਰਣੀ.destroy ();
    }

    ਪੂਰਨ get_ਸੂਚਕਾਂਕ (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਚੌੜਾਈ=ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
    {
      ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t key = {name_id, {0}, language};
      ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *ਇੰਦਰਾਜ = hb_bsearch (key, (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) ਇਹ->ਨਾਂ,
						    ਇਹ->ਨਾਂ.ਲੰਮਾਈ,
						    ਆਕਾਰ (hb_ot_name_ਇੰਦਰਾਜ_t),
						    hb_ot_name_ਇੰਦਰਾਜ_cmp_ਕੁੰਜੀ,
						    ਸੱਚ);

      ਜੇ (!ਇੰਦਰਾਜ)
      {
	ਇੰਦਰਾਜ = hb_bsearch (key, (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) ਇਹ->ਨਾਂ,
			    ਇਹ->ਨਾਂ.ਲੰਮਾਈ,
			    ਆਕਾਰ (hb_ot_name_ਇੰਦਰਾਜ_t),
			    hb_ot_name_ਇੰਦਰਾਜ_cmp_ਕੁੰਜੀ,
			    ਝੂਠ);
      }

      ਜੇ (!ਇੰਦਰਾਜ)
	ਵਾਪਸ -1;

      ਜੇ (ਚੌੜਾਈ)
	*ਚੌੜਾਈ = ਇੰਦਰਾਜ->ਇੰਦਰਾਜ_score < 10 ? 2 : 1;

      ਵਾਪਸ ਇੰਦਰਾਜ->ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ;
    }

    hb_ਬਾਈਟਾਂ_t get_name (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx) ਅਚਲ
    {
      ਅਚਲ hb_array_t<ਅਚਲ Name_ਰਿਕਾਰਡ> all_ਨਾਂ (ਸਾਰਣੀ->name_ਰਿਕਾਰਡ_Z.arrayZ, ਸਾਰਣੀ->ਗਿਣਤੀ_2);
      ਅਚਲ Name_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = all_ਨਾਂ[idx];
      ਅਚਲ hb_ਬਾਈਟਾਂ_t string_pool (pool, pool_len);
      ਵਾਪਸ string_pool.sub_array (ਰਿਕਾਰਡ.ਵਿੱਥ, ਰਿਕਾਰਡ.ਲੰਮਾਈ);
    }

    ਨਿਜੀ:
    ਅਚਲ ਅੱਖਰ *pool;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pool_len;
    ਜਨਤਕ:
    hb_blob_ptr_t<name> ਸਾਰਣੀ;
    hb_ਸਦਿਸ਼_t<hb_ot_name_ਇੰਦਰਾਜ_t> ਨਾਂ;
  };

  ਜਨਤਕ:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	ਗਿਣਤੀ_2;		/* Number of name records. */
  NNOffset16_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>
		string_ਵਿੱਥ;	/* Offset to start of string storage (from start of table). */
  Unsized_Array_ਦਾ<Name_ਰਿਕਾਰਡ>
		name_ਰਿਕਾਰਡ_Z;	/* The name records where count is the number of records. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, name_ਰਿਕਾਰਡ_Z);
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਇੰਦਰਾਜ_score

ਬਣਤਰ name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_NAME_NAME_HH */
