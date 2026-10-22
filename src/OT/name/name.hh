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

#ifndef OT_NAME_NAME_HH
#define OT_NAME_NAME_HH

#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-ot-name-language.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-aat-layout.hh"
#ಅನ್ತರ್ಭಾವ "../../hb-utf.hh"


ನಾಮಾಕಾಶ OT {

template <typename in_utf_t, typename out_utf_t>
ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned ಪೂರ್ಣಾಙ್ಕ                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ src_len = bytes.length / ಪರಿಮಾಣ (typename in_utf_t::codepoint_t);
  ಧ್ರುವ typename in_utf_t::codepoint_t *src = (ಧ್ರುವ typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ಧ್ರುವ typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ಧ್ರುವ hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  ಯದಿ (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    ಧ್ರುವ typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    ಯಾವತ್ (src < src_end && dst < dst_end)
    {
      ಧ್ರುವ typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      ಯದಿ (dst_next == dst)
	ಭಙ್ಗ; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned ಪೂರ್ಣಾಙ್ಕ dst_len = dst - text;
  ಯಾವತ್ (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  ಪ್ರತ್ಯಾಗಮ dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

ಸಂರಚನಾ NameRecord
{
  hb_language_t language (hb_face_t *face) ಧ್ರುವ
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned ಪೂರ್ಣಾಙ್ಕ p = platformID;
    unsigned ಪೂರ್ಣಾಙ್ಕ l = languageID;

    ಯದಿ (p == 3)
      ಪ್ರತ್ಯಾಗಮ _hb_ot_name_language_for_ms_code (l);

    ಯದಿ (p == 1)
      ಪ್ರತ್ಯಾಗಮ _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    ಯದಿ (p == 0)
      ಪ್ರತ್ಯಾಗಮ face->table.ltag->get_language (l);
#endif

#endif
    ಪ್ರತ್ಯಾಗಮ HB_LANGUAGE_INVALID;
  }

  uint16_t score () ಧ್ರುವ
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned ಪೂರ್ಣಾಙ್ಕ p = platformID;
    unsigned ಪೂರ್ಣಾಙ್ಕ e = encodingID;

    /* 32-bit. */
    ಯದಿ (p == 3 && e == 10) ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (p == 0 && e ==  6) ಪ್ರತ್ಯಾಗಮ 1;
    ಯದಿ (p == 0 && e ==  4) ಪ್ರತ್ಯಾಗಮ 2;

    /* 16-bit. */
    ಯದಿ (p == 3 && e ==  1) ಪ್ರತ್ಯಾಗಮ 3;
    ಯದಿ (p == 0 && e ==  3) ಪ್ರತ್ಯಾಗಮ 4;
    ಯದಿ (p == 0 && e ==  2) ಪ್ರತ್ಯಾಗಮ 5;
    ಯದಿ (p == 0 && e ==  1) ಪ್ರತ್ಯಾಗಮ 6;
    ಯದಿ (p == 0 && e ==  0) ಪ್ರತ್ಯಾಗಮ 7;

    /* Symbol. */
    ಯದಿ (p == 3 && e ==  0) ಪ್ರತ್ಯಾಗಮ 8;

    /* We treat all Mac Latin names as ASCII only. */
    ಯದಿ (p == 1 && e ==  0) ಪ್ರತ್ಯಾಗಮ 10; /* 10 is magic number :| */

    ಪ್ರತ್ಯಾಗಮ UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base
#ifdef HB_EXPERIMENTAL_API
                    , ಧ್ರುವ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    ಯದಿ (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      ವರ್ಣ *name_str_utf16_be = nullptr;

      ಯದಿ (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (ವರ್ಣ *) hb_calloc (byte_len, 1);
        ಯದಿ (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        ಯದಿ (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      ಅನ್ಯಥಾ
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        ಯದಿ (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->ತೆಗೆ_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    ಅನ್ಯಥಾ
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  ತಾರ್ಕಿಕ isUnicode () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ p = platformID;
    unsigned ಪೂರ್ಣಾಙ್ಕ e = encodingID;

    ಪ್ರತ್ಯಾಗಮ (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ NameRecord *a = (ಧ್ರುವ NameRecord *)pa;
    ಧ್ರುವ NameRecord *b = (ಧ್ರುವ NameRecord *)pb;

    ಯದಿ (a->platformID != b->platformID)
      ಪ್ರತ್ಯಾಗಮ a->platformID - b->platformID;

    ಯದಿ (a->encodingID != b->encodingID)
      ಪ್ರತ್ಯಾಗಮ a->encodingID - b->encodingID;

    ಯದಿ (a->languageID != b->languageID)
      ಪ್ರತ್ಯಾಗಮ a->languageID - b->languageID;

    ಯದಿ (a->nameID != b->nameID)
      ಪ್ರತ್ಯಾಗಮ a->nameID - b->nameID;

    ಯದಿ (a->length != b->length)
      ಪ್ರತ್ಯಾಗಮ a->length - b->length;

    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  offset.sanitize (c, base, length));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	length;		/* String length (in bytes). */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		offset;		/* String offset from start of storage area (in bytes). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
hb_ot_name_entry_cmp_ಕೀಲಿ (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb, ತಾರ್ಕಿಕ exact)
{
  ಧ್ರುವ hb_ot_name_entry_t *a = (ಧ್ರುವ hb_ot_name_entry_t *) pa;
  ಧ್ರುವ hb_ot_name_entry_t *b = (ಧ್ರುವ hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  ಯದಿ (a->name_id != b->name_id)
    ಪ್ರತ್ಯಾಗಮ a->name_id - b->name_id;

  ಯದಿ (a->language == b->language) ಪ್ರತ್ಯಾಗಮ 0;
  ಯದಿ (!a->language) ಪ್ರತ್ಯಾಗಮ -1;
  ಯದಿ (!b->language) ಪ್ರತ್ಯಾಗಮ +1;

  ಧ್ರುವ ವರ್ಣ *astr = hb_language_to_string (a->language);
  ಧ್ರುವ ವರ್ಣ *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  ಯದಿ (!exact && c &&
      hb_language_matches (b->language, a->language))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ c;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
_hb_ot_name_entry_cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  ಪೂರ್ಣಾಙ್ಕ v = hb_ot_name_entry_cmp_ಕೀಲಿ (pa, pb, ಸತ್ಯ);
  ಯದಿ (v)
    ಪ್ರತ್ಯಾಗಮ v;

  ಧ್ರುವ hb_ot_name_entry_t *a = (ಧ್ರುವ hb_ot_name_entry_t *) pa;
  ಧ್ರುವ hb_ot_name_entry_t *b = (ಧ್ರುವ hb_ot_name_entry_t *) pb;

  ಯದಿ (a->entry_score != b->entry_score)
    ಪ್ರತ್ಯಾಗಮ a->entry_score - b->entry_score;

  ಯದಿ (a->entry_index != b->entry_index)
    ಪ್ರತ್ಯಾಗಮ a->entry_index - b->entry_index;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ name
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ min_size + ಎಣಿಕೆ * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ಧ್ರುವ NameRecord &))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ಧ್ರುವ ಶೂನ್ಯ *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ಧ್ರುವ hb_vector_t<hb_ot_name_record_ids_t>& ಸೇರಿಸು_name_records
		  , ಧ್ರುವ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->extend_min ((*ಏತತ್))))  return_trace (ಅಸತ್ಯ);

    unsigned total_ಎಣಿಕೆ = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + ಸೇರಿಸು_name_records.length
#endif
        ;
    ಏತತ್->format = 0;
    ಯದಿ (!c->check_assign (ಏತತ್->ಎಣಿಕೆ, total_ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_ಎಣಿಕೆ, NameRecord::static_size);
    ಯದಿ (unlikely (!name_records)) return_trace (ಅಸತ್ಯ);

    hb_array_t<NameRecord> records (name_records, total_ಎಣಿಕೆ);

    ಪ್ರತಿ (ಧ್ರುವ NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    ಪ್ರತಿ (unsigned i = 0; i < ಸೇರಿಸು_name_records.length; i++)
    {
      ಧ್ರುವ hb_ot_name_record_ids_t& ids = ಸೇರಿಸು_name_records[i];
      NameRecord record;
      record.platformID = ids.platform_id;
      record.encodingID = ids.encoding_id;
      record.languageID = ids.language_id;
      record.nameID = ids.name_id;
      record.length = 0; // handled in NameRecord copy()
      record.offset = 0;
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }
#endif

    records.qsort ();

    c->copy_all (records,
		 src_string_pool
#ifdef HB_EXPERIMENTAL_API
		 , name_table_overrides
#endif
		 );
    hb_free (records.arrayZ);


    ಯದಿ (unlikely (c->ran_out_of_room ())) return_trace (ಅಸತ್ಯ);

    ಏತತ್->stringOffset = c->length ();

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    auto *name_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ಧ್ರುವ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (ಎಣಿಕೆ)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ಧ್ರುವ NameRecord& namerecord) {
      ಪ್ರತ್ಯಾಗಮ
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ಧ್ರುವ NameRecord& namerecord) {
      ಯದಿ (name_table_overrides->is_empty ())
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      ಯದಿ (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    ಪ್ರತಿ (ಧ್ರುವ NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> ಸೇರಿಸು_name_records;
    ಯದಿ (!name_table_overrides->is_empty ())
    {
      ಯದಿ (unlikely (!ಸೇರಿಸು_name_records.alloc (name_table_overrides->get_population (), ಸತ್ಯ)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿ (ಧ್ರುವ auto& record_ids : name_table_overrides->ಕೀಲಿಗಳು ())
      {
        ಯದಿ (name_table_overrides->get (record_ids).length == 0)
          ಅನುವರ್ತ;
        ಯದಿ (retained_name_record_ids.has (record_ids))
          ಅನುವರ್ತ;
        ಸೇರಿಸು_name_records.push (record_ids);
      }
    }
#endif

    ಪ್ರತ್ಯಾಗಮ name_ಅವಿಭಾಜ್ಯ->serialize (c->serializer, it,
				  std::addressof (ಏತತ್ + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , ಸೇರಿಸು_name_records
				  , name_table_overrides
#endif
				  );
  }

  ತಾರ್ಕಿಕ sanitize_records (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಧ್ರುವ ಶೂನ್ಯ *string_pool = (ಏತತ್+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, ಎಣಿಕೆ, string_pool));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, ಎಣಿಕೆ) &&
		  c->check_range (ಏತತ್, stringOffset) &&
		  sanitize_records (c));
  }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ಏತತ್->table = hb_sanitize_context_t ().reference_table<name> (face);
      ಪ್ರತಿಜ್ಞಾ (ಏತತ್->table.get_length () >= ಏತತ್->table->stringOffset);
      ಏತತ್->pool = (ಧ್ರುವ ವರ್ಣ *) (ಧ್ರುವ ಶೂನ್ಯ *) (ಏತತ್->table+ಏತತ್->table->stringOffset);
      ಏತತ್->pool_len = ಏತತ್->table.get_length () - ಏತತ್->table->stringOffset;
      ಧ್ರುವ hb_array_t<ಧ್ರುವ NameRecord> all_names (ಏತತ್->table->nameRecordZ.arrayZ,
						    ಏತತ್->table->ಎಣಿಕೆ);

      ಏತತ್->names.alloc_exact (all_names.length);

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = ಏತತ್->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      ಏತತ್->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned ಪೂರ್ಣಾಙ್ಕ j = 0;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಏತತ್->names.length; i++)
      {
	ಯದಿ (ಏತತ್->names[i].entry_score == UNSUPPORTED ||
	    ಏತತ್->names[i].language == HB_LANGUAGE_INVALID)
	  ಅನುವರ್ತ;
	ಯದಿ (i &&
	    ಏತತ್->names[i - 1].name_id  == ಏತತ್->names[i].name_id &&
	    ಏತತ್->names[i - 1].language == ಏತತ್->names[i].language)
	  ಅನುವರ್ತ;
	ಏತತ್->names[j++] = ಏತತ್->names[i];
      }
      ಏತತ್->names.resize (j);
    }
    ~accelerator_t ()
    {
      ಏತತ್->table.destroy ();
    }

    ಪೂರ್ಣಾಙ್ಕ get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned ಪೂರ್ಣಾಙ್ಕ    *width=nullptr) ಧ್ರುವ
    {
      ಧ್ರುವ hb_ot_name_entry_t key = {name_id, {0}, language};
      ಧ್ರುವ hb_ot_name_entry_t *entry = hb_bsearch (key, (ಧ್ರುವ hb_ot_name_entry_t *) ಏತತ್->names,
						    ಏತತ್->names.length,
						    ಪರಿಮಾಣ (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_ಕೀಲಿ,
						    ಸತ್ಯ);

      ಯದಿ (!entry)
      {
	entry = hb_bsearch (key, (ಧ್ರುವ hb_ot_name_entry_t *) ಏತತ್->names,
			    ಏತತ್->names.length,
			    ಪರಿಮಾಣ (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_ಕೀಲಿ,
			    ಅಸತ್ಯ);
      }

      ಯದಿ (!entry)
	ಪ್ರತ್ಯಾಗಮ -1;

      ಯದಿ (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      ಪ್ರತ್ಯಾಗಮ entry->entry_index;
    }

    hb_bytes_t get_name (unsigned ಪೂರ್ಣಾಙ್ಕ idx) ಧ್ರುವ
    {
      ಧ್ರುವ hb_array_t<ಧ್ರುವ NameRecord> all_names (table->nameRecordZ.arrayZ, table->ಎಣಿಕೆ);
      ಧ್ರುವ NameRecord &record = all_names[idx];
      ಧ್ರುವ hb_bytes_t string_pool (pool, pool_len);
      ಪ್ರತ್ಯಾಗಮ string_pool.sub_array (record.offset, record.length);
    }

    ಗುಪ್ತ:
    ಧ್ರುವ ವರ್ಣ *pool;
    unsigned ಪೂರ್ಣಾಙ್ಕ pool_len;
    ಸಾರ್ವಜನಿಕ:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  ಸಾರ್ವಜನಿಕ:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	ಎಣಿಕೆ;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

ಸಂರಚನಾ name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
