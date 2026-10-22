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

#अन्तर्भाव "../../hb-open-type.hh"
#अन्तर्भाव "../../hb-ot-name-language.hh"
#अन्तर्भाव "../../hb-aat-layout.hh"
#अन्तर्भाव "../../hb-utf.hh"


नामाकाश OT {

template <typename in_utf_t, typename out_utf_t>
अन्तःस्थ unsigned पूर्णाङ्क
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned पूर्णाङ्क                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned पूर्णाङ्क src_len = bytes.length / परिमाण (typename in_utf_t::codepoint_t);
  ध्रुव typename in_utf_t::codepoint_t *src = (ध्रुव typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ध्रुव typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ध्रुव hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  सचे (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    ध्रुव typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    याव (src < src_end && dst < dst_end)
    {
      ध्रुव typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      सचे (dst_next == dst)
	भङ्ग; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned पूर्णाङ्क dst_len = dst - text;
  याव (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  पटिनिवत्त dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

संरचना NameRecord
{
  hb_language_t language (hb_face_t *face) ध्रुव
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned पूर्णाङ्क p = platformID;
    unsigned पूर्णाङ्क l = languageID;

    सचे (p == 3)
      पटिनिवत्त _hb_ot_name_language_for_ms_code (l);

    सचे (p == 1)
      पटिनिवत्त _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    सचे (p == 0)
      पटिनिवत्त face->table.ltag->get_language (l);
#endif

#endif
    पटिनिवत्त HB_LANGUAGE_INVALID;
  }

  uint16_t score () ध्रुव
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned पूर्णाङ्क p = platformID;
    unsigned पूर्णाङ्क e = encodingID;

    /* 32-bit. */
    सचे (p == 3 && e == 10) पटिनिवत्त 0;
    सचे (p == 0 && e ==  6) पटिनिवत्त 1;
    सचे (p == 0 && e ==  4) पटिनिवत्त 2;

    /* 16-bit. */
    सचे (p == 3 && e ==  1) पटिनिवत्त 3;
    सचे (p == 0 && e ==  3) पटिनिवत्त 4;
    सचे (p == 0 && e ==  2) पटिनिवत्त 5;
    सचे (p == 0 && e ==  1) पटिनिवत्त 6;
    सचे (p == 0 && e ==  0) पटिनिवत्त 7;

    /* Symbol. */
    सचे (p == 3 && e ==  0) पटिनिवत्त 8;

    /* We treat all Mac Latin names as ASCII only. */
    सचे (p == 1 && e ==  0) पटिनिवत्त 10; /* 10 is magic number :| */

    पटिनिवत्त UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ध्रुव शून्य *base
#ifdef HB_EXPERIMENTAL_API
                    , ध्रुव hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (एतत्);
    सचे (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    सचे (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      वर्ण *name_str_utf16_be = nullptr;

      सचे (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (वर्ण *) hb_calloc (byte_len, 1);
        सचे (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        सचे (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      अञ्ञथा
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        सचे (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->निष्कासय_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    अञ्ञथा
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  तार्किक isUnicode () ध्रुव
  {
    unsigned पूर्णाङ्क p = platformID;
    unsigned पूर्णाङ्क e = encodingID;

    पटिनिवत्त (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव NameRecord *a = (ध्रुव NameRecord *)pa;
    ध्रुव NameRecord *b = (ध्रुव NameRecord *)pb;

    सचे (a->platformID != b->platformID)
      पटिनिवत्त a->platformID - b->platformID;

    सचे (a->encodingID != b->encodingID)
      पटिनिवत्त a->encodingID - b->encodingID;

    सचे (a->languageID != b->languageID)
      पटिनिवत्त a->languageID - b->languageID;

    सचे (a->nameID != b->nameID)
      पटिनिवत्त a->nameID - b->nameID;

    सचे (a->length != b->length)
      पटिनिवत्त a->length - b->length;

    पटिनिवत्त 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

स्थिर पूर्णाङ्क
hb_ot_name_entry_cmp_कुञ्चिका (ध्रुव शून्य *pa, ध्रुव शून्य *pb, तार्किक exact)
{
  ध्रुव hb_ot_name_entry_t *a = (ध्रुव hb_ot_name_entry_t *) pa;
  ध्रुव hb_ot_name_entry_t *b = (ध्रुव hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  सचे (a->name_id != b->name_id)
    पटिनिवत्त a->name_id - b->name_id;

  सचे (a->language == b->language) पटिनिवत्त 0;
  सचे (!a->language) पटिनिवत्त -1;
  सचे (!b->language) पटिनिवत्त +1;

  ध्रुव वर्ण *astr = hb_language_to_string (a->language);
  ध्रुव वर्ण *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  सचे (!exact && c &&
      hb_language_matches (b->language, a->language))
    पटिनिवत्त 0;

  पटिनिवत्त c;
}

स्थिर पूर्णाङ्क
_hb_ot_name_entry_cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  पूर्णाङ्क v = hb_ot_name_entry_cmp_कुञ्चिका (pa, pb, सच्च);
  सचे (v)
    पटिनिवत्त v;

  ध्रुव hb_ot_name_entry_t *a = (ध्रुव hb_ot_name_entry_t *) pa;
  ध्रुव hb_ot_name_entry_t *b = (ध्रुव hb_ot_name_entry_t *) pb;

  सचे (a->entry_score != b->entry_score)
    पटिनिवत्त a->entry_score - b->entry_score;

  सचे (a->entry_index != b->entry_index)
    पटिनिवत्त a->entry_index - b->entry_index;

  पटिनिवत्त 0;
}

संरचना name
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () ध्रुव
  { पटिनिवत्त min_size + गणनम् * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ध्रुव NameRecord &))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ध्रुव शून्य *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ध्रुव hb_vector_t<hb_ot_name_record_ids_t>& निवेशय_name_records
		  , ध्रुव hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (एतत्);

    सचे (unlikely (!c->extend_min ((*एतत्))))  return_trace (मिच्छा);

    unsigned total_गणनम् = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + निवेशय_name_records.length
#endif
        ;
    एतत्->format = 0;
    सचे (!c->check_assign (एतत्->गणनम्, total_गणनम्, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      पटिनिवत्त मिच्छा;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_गणनम्, NameRecord::static_size);
    सचे (unlikely (!name_records)) return_trace (मिच्छा);

    hb_array_t<NameRecord> records (name_records, total_गणनम्);

    अत्थाय (ध्रुव NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    अत्थाय (unsigned i = 0; i < निवेशय_name_records.length; i++)
    {
      ध्रुव hb_ot_name_record_ids_t& ids = निवेशय_name_records[i];
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


    सचे (unlikely (c->ran_out_of_room ())) return_trace (मिच्छा);

    एतत्->stringOffset = c->length ();

    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    auto *name_अविभाज्यः = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ध्रुव hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (गणनम्)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ध्रुव NameRecord& namerecord) {
      पटिनिवत्त
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ध्रुव NameRecord& namerecord) {
      सचे (name_table_overrides->is_empty ())
        पटिनिवत्त सच्च;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      सचे (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        पटिनिवत्त मिच्छा;
      पटिनिवत्त सच्च;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    अत्थाय (ध्रुव NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> निवेशय_name_records;
    सचे (!name_table_overrides->is_empty ())
    {
      सचे (unlikely (!निवेशय_name_records.alloc (name_table_overrides->get_population (), सच्च)))
        पटिनिवत्त मिच्छा;
      अत्थाय (ध्रुव auto& record_ids : name_table_overrides->कुञ्चिकाः ())
      {
        सचे (name_table_overrides->get (record_ids).length == 0)
          अनुवर्त;
        सचे (retained_name_record_ids.has (record_ids))
          अनुवर्त;
        निवेशय_name_records.push (record_ids);
      }
    }
#endif

    पटिनिवत्त name_अविभाज्यः->serialize (c->serializer, it,
				  std::addressof (एतत् + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , निवेशय_name_records
				  , name_table_overrides
#endif
				  );
  }

  तार्किक sanitize_records (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    ध्रुव शून्य *string_pool = (एतत्+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, गणनम्, string_pool));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, गणनम्) &&
		  c->check_range (एतत्, stringOffset) &&
		  sanitize_records (c));
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      एतत्->table = hb_sanitize_context_t ().reference_table<name> (face);
      प्रतिज्ञा (एतत्->table.get_length () >= एतत्->table->stringOffset);
      एतत्->pool = (ध्रुव वर्ण *) (ध्रुव शून्य *) (एतत्->table+एतत्->table->stringOffset);
      एतत्->pool_len = एतत्->table.get_length () - एतत्->table->stringOffset;
      ध्रुव hb_array_t<ध्रुव NameRecord> all_names (एतत्->table->nameRecordZ.arrayZ,
						    एतत्->table->गणनम्);

      एतत्->names.alloc_exact (all_names.length);

      अत्थाय (unsigned पूर्णाङ्क i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = एतत्->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      एतत्->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned पूर्णाङ्क j = 0;
      अत्थाय (unsigned पूर्णाङ्क i = 0; i < एतत्->names.length; i++)
      {
	सचे (एतत्->names[i].entry_score == UNSUPPORTED ||
	    एतत्->names[i].language == HB_LANGUAGE_INVALID)
	  अनुवर्त;
	सचे (i &&
	    एतत्->names[i - 1].name_id  == एतत्->names[i].name_id &&
	    एतत्->names[i - 1].language == एतत्->names[i].language)
	  अनुवर्त;
	एतत्->names[j++] = एतत्->names[i];
      }
      एतत्->names.resize (j);
    }
    ~accelerator_t ()
    {
      एतत्->table.destroy ();
    }

    पूर्णाङ्क get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned पूर्णाङ्क    *width=nullptr) ध्रुव
    {
      ध्रुव hb_ot_name_entry_t key = {name_id, {0}, language};
      ध्रुव hb_ot_name_entry_t *entry = hb_bsearch (key, (ध्रुव hb_ot_name_entry_t *) एतत्->names,
						    एतत्->names.length,
						    परिमाण (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_कुञ्चिका,
						    सच्च);

      सचे (!entry)
      {
	entry = hb_bsearch (key, (ध्रुव hb_ot_name_entry_t *) एतत्->names,
			    एतत्->names.length,
			    परिमाण (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_कुञ्चिका,
			    मिच्छा);
      }

      सचे (!entry)
	पटिनिवत्त -1;

      सचे (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      पटिनिवत्त entry->entry_index;
    }

    hb_bytes_t get_name (unsigned पूर्णाङ्क idx) ध्रुव
    {
      ध्रुव hb_array_t<ध्रुव NameRecord> all_names (table->nameRecordZ.arrayZ, table->गणनम्);
      ध्रुव NameRecord &record = all_names[idx];
      ध्रुव hb_bytes_t string_pool (pool, pool_len);
      पटिनिवत्त string_pool.sub_array (record.offset, record.length);
    }

    गुप्त:
    ध्रुव वर्ण *pool;
    unsigned पूर्णाङ्क pool_len;
    सार्वजनिक:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  सार्वजनिक:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	गणनम्;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

संरचना name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
