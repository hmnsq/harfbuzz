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

ढाँचा <typename in_utf_t, typename out_utf_t>
अन्तःस्थ अचिन्हित पूर्णाङ्क
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			अचिन्हित पूर्णाङ्क                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  अचिन्हित पूर्णाङ्क src_len = bytes.length / परिमाण (typename in_utf_t::codepoint_t);
  अचर typename in_utf_t::codepoint_t *src = (अचर typename in_utf_t::codepoint_t *) bytes.arrayZ;
  अचर typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  अचर hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  यदि (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    अचर typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    जबसम्म (src < src_end && dst < dst_end)
    {
      अचर typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      यदि (dst_next == dst)
	रोक; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  अचिन्हित पूर्णाङ्क dst_len = dst - text;
  जबसम्म (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  फर्काऊ dst_len;
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
  hb_language_t language (hb_face_t *face) अचर
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    अचिन्हित पूर्णाङ्क p = platformID;
    अचिन्हित पूर्णाङ्क l = languageID;

    यदि (p == 3)
      फर्काऊ _hb_ot_name_language_for_ms_code (l);

    यदि (p == 1)
      फर्काऊ _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    यदि (p == 0)
      फर्काऊ face->table.ltag->get_language (l);
#endif

#endif
    फर्काऊ HB_LANGUAGE_INVALID;
  }

  uint16_t score () अचर
  {
    /* Same order as in cmap::find_best_subtable(). */
    अचिन्हित पूर्णाङ्क p = platformID;
    अचिन्हित पूर्णाङ्क e = encodingID;

    /* 32-bit. */
    यदि (p == 3 && e == 10) फर्काऊ 0;
    यदि (p == 0 && e ==  6) फर्काऊ 1;
    यदि (p == 0 && e ==  4) फर्काऊ 2;

    /* 16-bit. */
    यदि (p == 3 && e ==  1) फर्काऊ 3;
    यदि (p == 0 && e ==  3) फर्काऊ 4;
    यदि (p == 0 && e ==  2) फर्काऊ 5;
    यदि (p == 0 && e ==  1) फर्काऊ 6;
    यदि (p == 0 && e ==  0) फर्काऊ 7;

    /* Symbol. */
    यदि (p == 3 && e ==  0) फर्काऊ 8;

    /* We treat all Mac Latin names as ASCII only. */
    यदि (p == 1 && e ==  0) फर्काऊ 10; /* 10 is magic number :| */

    फर्काऊ UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, अचर शून्य *base
#ifdef HB_EXPERIMENTAL_API
                    , अचर hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) अचर
  {
    TRACE_SERIALIZE (एतत्);
    HB_UNUSED स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed (एतत्);
    यदि (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    यदि (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      वर्ण *name_str_utf16_be = nullptr;

      यदि (platformID != 1)
      {
        अचिन्हित text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        अचिन्हित byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (वर्ण *) hb_calloc (byte_len, 1);
        यदि (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        अचिन्हित encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        यदि (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      अन्यथा
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        यदि (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->झिक_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    अन्यथा
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  तार्किक isUnicode () अचर
  {
    अचिन्हित पूर्णाङ्क p = platformID;
    अचिन्हित पूर्णाङ्क e = encodingID;

    फर्काऊ (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb)
  {
    अचर NameRecord *a = (अचर NameRecord *)pa;
    अचर NameRecord *b = (अचर NameRecord *)pb;

    यदि (a->platformID != b->platformID)
      फर्काऊ a->platformID - b->platformID;

    यदि (a->encodingID != b->encodingID)
      फर्काऊ a->encodingID - b->encodingID;

    यदि (a->languageID != b->languageID)
      फर्काऊ a->languageID - b->languageID;

    यदि (a->nameID != b->nameID)
      फर्काऊ a->nameID - b->nameID;

    यदि (a->length != b->length)
      फर्काऊ a->length - b->length;

    फर्काऊ 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
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
hb_ot_name_entry_cmp_साँचो (अचर शून्य *pa, अचर शून्य *pb, तार्किक exact)
{
  अचर hb_ot_name_entry_t *a = (अचर hb_ot_name_entry_t *) pa;
  अचर hb_ot_name_entry_t *b = (अचर hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  यदि (a->name_id != b->name_id)
    फर्काऊ a->name_id - b->name_id;

  यदि (a->language == b->language) फर्काऊ 0;
  यदि (!a->language) फर्काऊ -1;
  यदि (!b->language) फर्काऊ +1;

  अचर वर्ण *astr = hb_language_to_string (a->language);
  अचर वर्ण *bstr = hb_language_to_string (b->language);

  चिन्हित c = म_भेद (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  यदि (!exact && c &&
      hb_language_matches (b->language, a->language))
    फर्काऊ 0;

  फर्काऊ c;
}

स्थिर पूर्णाङ्क
_hb_ot_name_entry_cmp (अचर शून्य *pa, अचर शून्य *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  पूर्णाङ्क v = hb_ot_name_entry_cmp_साँचो (pa, pb, सत्य);
  यदि (v)
    फर्काऊ v;

  अचर hb_ot_name_entry_t *a = (अचर hb_ot_name_entry_t *) pa;
  अचर hb_ot_name_entry_t *b = (अचर hb_ot_name_entry_t *) pb;

  यदि (a->entry_score != b->entry_score)
    फर्काऊ a->entry_score - b->entry_score;

  यदि (a->entry_index != b->entry_index)
    फर्काऊ a->entry_index - b->entry_index;

  फर्काऊ 0;
}

संरचना name
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  माप_प्रकार get_size () अचर
  { फर्काऊ min_size + गन्ती * nameRecordZ.item_size; }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, अचर NameRecord &))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  अचर शून्य *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , अचर hb_vector_t<hb_ot_name_record_ids_t>& घुसाउ_name_records
		  , अचर hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (एतत्);

    यदि (unlikely (!c->extend_min ((*एतत्))))  return_trace (असत्य);

    अचिन्हित total_गन्ती = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + घुसाउ_name_records.length
#endif
        ;
    एतत्->format = 0;
    यदि (!c->check_assign (एतत्->गन्ती, total_गन्ती, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      फर्काऊ असत्य;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_गन्ती, NameRecord::static_size);
    यदि (unlikely (!name_records)) return_trace (असत्य);

    hb_array_t<NameRecord> records (name_records, total_गन्ती);

    लागि (अचर NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    लागि (अचिन्हित i = 0; i < घुसाउ_name_records.length; i++)
    {
      अचर hb_ot_name_record_ids_t& ids = घुसाउ_name_records[i];
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

    records.क्विक ();

    c->copy_all (records,
		 src_string_pool
#ifdef HB_EXPERIMENTAL_API
		 , name_table_overrides
#endif
		 );
    hb_free (records.arrayZ);


    यदि (unlikely (c->ran_out_of_room ())) return_trace (असत्य);

    एतत्->stringOffset = c->length ();

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    स्वतः *name_अविभाज्य = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    अचर hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    स्वतः it =
    + nameRecordZ.as_array (गन्ती)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (अचर NameRecord& namerecord) {
      फर्काऊ
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (अचर NameRecord& namerecord) {
      यदि (name_table_overrides->is_empty ())
        फर्काऊ सत्य;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      यदि (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        फर्काऊ असत्य;
      फर्काऊ सत्य;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, अचिन्हित> retained_name_record_ids;
    लागि (अचर NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> घुसाउ_name_records;
    यदि (!name_table_overrides->is_empty ())
    {
      यदि (unlikely (!घुसाउ_name_records.alloc (name_table_overrides->get_population (), सत्य)))
        फर्काऊ असत्य;
      लागि (अचर स्वतः& record_ids : name_table_overrides->साँचोहरू ())
      {
        यदि (name_table_overrides->get (record_ids).length == 0)
          जारी;
        यदि (retained_name_record_ids.has (record_ids))
          जारी;
        घुसाउ_name_records.push (record_ids);
      }
    }
#endif

    फर्काऊ name_अविभाज्य->serialize (c->serializer, it,
				  std::addressof (एतत् + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , घुसाउ_name_records
				  , name_table_overrides
#endif
				  );
  }

  तार्किक sanitize_records (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    अचर शून्य *string_pool = (एतत्+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, गन्ती, string_pool));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, गन्ती) &&
		  c->check_range (एतत्, stringOffset) &&
		  sanitize_records (c));
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      एतत्->table = hb_sanitize_context_t ().reference_table<name> (face);
      प्रतिज्ञा (एतत्->table.get_length () >= एतत्->table->stringOffset);
      एतत्->pool = (अचर वर्ण *) (अचर शून्य *) (एतत्->table+एतत्->table->stringOffset);
      एतत्->pool_len = एतत्->table.get_length () - एतत्->table->stringOffset;
      अचर hb_array_t<अचर NameRecord> all_names (एतत्->table->nameRecordZ.arrayZ,
						    एतत्->table->गन्ती);

      एतत्->names.alloc_exact (all_names.length);

      लागि (अचिन्हित पूर्णाङ्क i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = एतत्->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      एतत्->names.क्विक (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      अचिन्हित पूर्णाङ्क j = 0;
      लागि (अचिन्हित पूर्णाङ्क i = 0; i < एतत्->names.length; i++)
      {
	यदि (एतत्->names[i].entry_score == UNSUPPORTED ||
	    एतत्->names[i].language == HB_LANGUAGE_INVALID)
	  जारी;
	यदि (i &&
	    एतत्->names[i - 1].name_id  == एतत्->names[i].name_id &&
	    एतत्->names[i - 1].language == एतत्->names[i].language)
	  जारी;
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
		   अचिन्हित पूर्णाङ्क    *width=nullptr) अचर
    {
      अचर hb_ot_name_entry_t key = {name_id, {0}, language};
      अचर hb_ot_name_entry_t *entry = hb_bsearch (key, (अचर hb_ot_name_entry_t *) एतत्->names,
						    एतत्->names.length,
						    परिमाण (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_साँचो,
						    सत्य);

      यदि (!entry)
      {
	entry = hb_bsearch (key, (अचर hb_ot_name_entry_t *) एतत्->names,
			    एतत्->names.length,
			    परिमाण (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_साँचो,
			    असत्य);
      }

      यदि (!entry)
	फर्काऊ -1;

      यदि (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      फर्काऊ entry->entry_index;
    }

    hb_bytes_t get_name (अचिन्हित पूर्णाङ्क idx) अचर
    {
      अचर hb_array_t<अचर NameRecord> all_names (table->nameRecordZ.arrayZ, table->गन्ती);
      अचर NameRecord &record = all_names[idx];
      अचर hb_bytes_t string_pool (pool, pool_len);
      फर्काऊ string_pool.sub_array (record.offset, record.length);
    }

    गुप्त:
    अचर वर्ण *pool;
    अचिन्हित पूर्णाङ्क pool_len;
    सार्वजनिक:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  सार्वजनिक:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	गन्ती;		/* Number of name records. */
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
