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

#অন্তর্ভাব "../../hb-open-type.hh"
#অন্তর্ভাব "../../hb-ot-name-language.hh"
#অন্তর্ভাব "../../hb-aat-layout.hh"
#অন্তর্ভাব "../../hb-utf.hh"


নামাকাশ OT {

template <typename in_utf_t, typename out_utf_t>
অন্তঃস্থ unsigned পূর্ণাঙ্ক
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned পূর্ণাঙ্ক                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned পূর্ণাঙ্ক src_len = bytes.length / পরিমাণ (typename in_utf_t::codepoint_t);
  ধ্রুব typename in_utf_t::codepoint_t *src = (ধ্রুব typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ধ্রুব typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ধ্রুব hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  যদি (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    ধ্রুব typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    যাবত্ (src < src_end && dst < dst_end)
    {
      ধ্রুব typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      যদি (dst_next == dst)
	ভঙ্গ; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned পূর্ণাঙ্ক dst_len = dst - text;
  যাবত্ (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  প্রত্যাগম dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

সংরচনা NameRecord
{
  hb_language_t language (hb_face_t *face) ধ্রুব
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned পূর্ণাঙ্ক p = platformID;
    unsigned পূর্ণাঙ্ক l = languageID;

    যদি (p == 3)
      প্রত্যাগম _hb_ot_name_language_for_ms_code (l);

    যদি (p == 1)
      প্রত্যাগম _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    যদি (p == 0)
      প্রত্যাগম face->table.ltag->get_language (l);
#endif

#endif
    প্রত্যাগম HB_LANGUAGE_INVALID;
  }

  uint16_t score () ধ্রুব
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned পূর্ণাঙ্ক p = platformID;
    unsigned পূর্ণাঙ্ক e = encodingID;

    /* 32-bit. */
    যদি (p == 3 && e == 10) প্রত্যাগম 0;
    যদি (p == 0 && e ==  6) প্রত্যাগম 1;
    যদি (p == 0 && e ==  4) প্রত্যাগম 2;

    /* 16-bit. */
    যদি (p == 3 && e ==  1) প্রত্যাগম 3;
    যদি (p == 0 && e ==  3) প্রত্যাগম 4;
    যদি (p == 0 && e ==  2) প্রত্যাগম 5;
    যদি (p == 0 && e ==  1) প্রত্যাগম 6;
    যদি (p == 0 && e ==  0) প্রত্যাগম 7;

    /* Symbol. */
    যদি (p == 3 && e ==  0) প্রত্যাগম 8;

    /* We treat all Mac Latin names as ASCII only. */
    যদি (p == 1 && e ==  0) প্রত্যাগম 10; /* 10 is magic number :| */

    প্রত্যাগম UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ধ্রুব শূন্য *base
#ifdef HB_EXPERIMENTAL_API
                    , ধ্রুব hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (এতত্);
    যদি (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    যদি (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      বর্ণ *name_str_utf16_be = nullptr;

      যদি (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (বর্ণ *) hb_calloc (byte_len, 1);
        যদি (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        যদি (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      অন্যথা
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        যদি (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->বের_করো_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    অন্যথা
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  তার্কিক isUnicode () ধ্রুব
  {
    unsigned পূর্ণাঙ্ক p = platformID;
    unsigned পূর্ণাঙ্ক e = encodingID;

    প্রত্যাগম (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
  {
    ধ্রুব NameRecord *a = (ধ্রুব NameRecord *)pa;
    ধ্রুব NameRecord *b = (ধ্রুব NameRecord *)pb;

    যদি (a->platformID != b->platformID)
      প্রত্যাগম a->platformID - b->platformID;

    যদি (a->encodingID != b->encodingID)
      প্রত্যাগম a->encodingID - b->encodingID;

    যদি (a->languageID != b->languageID)
      প্রত্যাগম a->languageID - b->languageID;

    যদি (a->nameID != b->nameID)
      প্রত্যাগম a->nameID - b->nameID;

    যদি (a->length != b->length)
      প্রত্যাগম a->length - b->length;

    প্রত্যাগম 0;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
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
  সার্বজনিক:
  DEFINE_SIZE_STATIC (12);
};

স্থির পূর্ণাঙ্ক
hb_ot_name_entry_cmp_চাবি (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb, তার্কিক exact)
{
  ধ্রুব hb_ot_name_entry_t *a = (ধ্রুব hb_ot_name_entry_t *) pa;
  ধ্রুব hb_ot_name_entry_t *b = (ধ্রুব hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  যদি (a->name_id != b->name_id)
    প্রত্যাগম a->name_id - b->name_id;

  যদি (a->language == b->language) প্রত্যাগম 0;
  যদি (!a->language) প্রত্যাগম -1;
  যদি (!b->language) প্রত্যাগম +1;

  ধ্রুব বর্ণ *astr = hb_language_to_string (a->language);
  ধ্রুব বর্ণ *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  যদি (!exact && c &&
      hb_language_matches (b->language, a->language))
    প্রত্যাগম 0;

  প্রত্যাগম c;
}

স্থির পূর্ণাঙ্ক
_hb_ot_name_entry_cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  পূর্ণাঙ্ক v = hb_ot_name_entry_cmp_চাবি (pa, pb, সত্য);
  যদি (v)
    প্রত্যাগম v;

  ধ্রুব hb_ot_name_entry_t *a = (ধ্রুব hb_ot_name_entry_t *) pa;
  ধ্রুব hb_ot_name_entry_t *b = (ধ্রুব hb_ot_name_entry_t *) pb;

  যদি (a->entry_score != b->entry_score)
    প্রত্যাগম a->entry_score - b->entry_score;

  যদি (a->entry_index != b->entry_index)
    প্রত্যাগম a->entry_index - b->entry_index;

  প্রত্যাগম 0;
}

সংরচনা name
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () ধ্রুব
  { প্রত্যাগম min_size + গুনতি * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ধ্রুব NameRecord &))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ধ্রুব শূন্য *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ধ্রুব hb_vector_t<hb_ot_name_record_ids_t>& ঢোকাও_name_records
		  , ধ্রুব hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (এতত্);

    যদি (unlikely (!c->extend_min ((*এতত্))))  return_trace (অসত্য);

    unsigned total_গুনতি = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + ঢোকাও_name_records.length
#endif
        ;
    এতত্->format = 0;
    যদি (!c->check_assign (এতত্->গুনতি, total_গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      প্রত্যাগম অসত্য;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_গুনতি, NameRecord::static_size);
    যদি (unlikely (!name_records)) return_trace (অসত্য);

    hb_array_t<NameRecord> records (name_records, total_গুনতি);

    প্রতি (ধ্রুব NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    প্রতি (unsigned i = 0; i < ঢোকাও_name_records.length; i++)
    {
      ধ্রুব hb_ot_name_record_ids_t& ids = ঢোকাও_name_records[i];
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


    যদি (unlikely (c->ran_out_of_room ())) return_trace (অসত্য);

    এতত্->stringOffset = c->length ();

    return_trace (সত্য);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    auto *name_মৌলিক = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ধ্রুব hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (গুনতি)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ধ্রুব NameRecord& namerecord) {
      প্রত্যাগম
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ধ্রুব NameRecord& namerecord) {
      যদি (name_table_overrides->is_empty ())
        প্রত্যাগম সত্য;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      যদি (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        প্রত্যাগম অসত্য;
      প্রত্যাগম সত্য;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    প্রতি (ধ্রুব NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> ঢোকাও_name_records;
    যদি (!name_table_overrides->is_empty ())
    {
      যদি (unlikely (!ঢোকাও_name_records.alloc (name_table_overrides->get_population (), সত্য)))
        প্রত্যাগম অসত্য;
      প্রতি (ধ্রুব auto& record_ids : name_table_overrides->চাবিগুলি ())
      {
        যদি (name_table_overrides->get (record_ids).length == 0)
          অনুবর্ত;
        যদি (retained_name_record_ids.has (record_ids))
          অনুবর্ত;
        ঢোকাও_name_records.push (record_ids);
      }
    }
#endif

    প্রত্যাগম name_মৌলিক->serialize (c->serializer, it,
				  std::addressof (এতত্ + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , ঢোকাও_name_records
				  , name_table_overrides
#endif
				  );
  }

  তার্কিক sanitize_records (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    ধ্রুব শূন্য *string_pool = (এতত্+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, গুনতি, string_pool));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, গুনতি) &&
		  c->check_range (এতত্, stringOffset) &&
		  sanitize_records (c));
  }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      এতত্->table = hb_sanitize_context_t ().reference_table<name> (face);
      প্রতিজ্ঞা (এতত্->table.get_length () >= এতত্->table->stringOffset);
      এতত্->pool = (ধ্রুব বর্ণ *) (ধ্রুব শূন্য *) (এতত্->table+এতত্->table->stringOffset);
      এতত্->pool_len = এতত্->table.get_length () - এতত্->table->stringOffset;
      ধ্রুব hb_array_t<ধ্রুব NameRecord> all_names (এতত্->table->nameRecordZ.arrayZ,
						    এতত্->table->গুনতি);

      এতত্->names.alloc_exact (all_names.length);

      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = এতত্->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      এতত্->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned পূর্ণাঙ্ক j = 0;
      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < এতত্->names.length; i++)
      {
	যদি (এতত্->names[i].entry_score == UNSUPPORTED ||
	    এতত্->names[i].language == HB_LANGUAGE_INVALID)
	  অনুবর্ত;
	যদি (i &&
	    এতত্->names[i - 1].name_id  == এতত্->names[i].name_id &&
	    এতত্->names[i - 1].language == এতত্->names[i].language)
	  অনুবর্ত;
	এতত্->names[j++] = এতত্->names[i];
      }
      এতত্->names.resize (j);
    }
    ~accelerator_t ()
    {
      এতত্->table.destroy ();
    }

    পূর্ণাঙ্ক get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned পূর্ণাঙ্ক    *width=nullptr) ধ্রুব
    {
      ধ্রুব hb_ot_name_entry_t key = {name_id, {0}, language};
      ধ্রুব hb_ot_name_entry_t *entry = hb_bsearch (key, (ধ্রুব hb_ot_name_entry_t *) এতত্->names,
						    এতত্->names.length,
						    পরিমাণ (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_চাবি,
						    সত্য);

      যদি (!entry)
      {
	entry = hb_bsearch (key, (ধ্রুব hb_ot_name_entry_t *) এতত্->names,
			    এতত্->names.length,
			    পরিমাণ (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_চাবি,
			    অসত্য);
      }

      যদি (!entry)
	প্রত্যাগম -1;

      যদি (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      প্রত্যাগম entry->entry_index;
    }

    hb_bytes_t get_name (unsigned পূর্ণাঙ্ক idx) ধ্রুব
    {
      ধ্রুব hb_array_t<ধ্রুব NameRecord> all_names (table->nameRecordZ.arrayZ, table->গুনতি);
      ধ্রুব NameRecord &record = all_names[idx];
      ধ্রুব hb_bytes_t string_pool (pool, pool_len);
      প্রত্যাগম string_pool.sub_array (record.offset, record.length);
    }

    গুপ্ত:
    ধ্রুব বর্ণ *pool;
    unsigned পূর্ণাঙ্ক pool_len;
    সার্বজনিক:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  সার্বজনিক:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	গুনতি;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

সংরচনা name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
