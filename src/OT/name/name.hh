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

#அந்தர்பாவ "../../hb-open-type.hh"
#அந்தர்பாவ "../../hb-ot-name-language.hh"
#அந்தர்பாவ "../../hb-aat-layout.hh"
#அந்தர்பாவ "../../hb-utf.hh"


நாமாகாஶ OT {

template <typename in_utf_t, typename out_utf_t>
உள்வரி unsigned முழு
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned முழு                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned முழு src_len = bytes.length / அளவு (typename in_utf_t::codepoint_t);
  மாறிலி typename in_utf_t::codepoint_t *src = (மாறிலி typename in_utf_t::codepoint_t *) bytes.arrayZ;
  மாறிலி typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  மாறிலி hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  எனில் (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    மாறிலி typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    போது (src < src_end && dst < dst_end)
    {
      மாறிலி typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      எனில் (dst_next == dst)
	நிறுத்து; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned முழு dst_len = dst - text;
  போது (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  திரும்பு dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

கட்டமைப்பு NameRecord
{
  hb_language_t language (hb_face_t *face) மாறிலி
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned முழு p = platformID;
    unsigned முழு l = languageID;

    எனில் (p == 3)
      திரும்பு _hb_ot_name_language_for_ms_code (l);

    எனில் (p == 1)
      திரும்பு _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    எனில் (p == 0)
      திரும்பு face->table.ltag->get_language (l);
#endif

#endif
    திரும்பு HB_LANGUAGE_INVALID;
  }

  uint16_t score () மாறிலி
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned முழு p = platformID;
    unsigned முழு e = encodingID;

    /* 32-bit. */
    எனில் (p == 3 && e == 10) திரும்பு 0;
    எனில் (p == 0 && e ==  6) திரும்பு 1;
    எனில் (p == 0 && e ==  4) திரும்பு 2;

    /* 16-bit. */
    எனில் (p == 3 && e ==  1) திரும்பு 3;
    எனில் (p == 0 && e ==  3) திரும்பு 4;
    எனில் (p == 0 && e ==  2) திரும்பு 5;
    எனில் (p == 0 && e ==  1) திரும்பு 6;
    எனில் (p == 0 && e ==  0) திரும்பு 7;

    /* Symbol. */
    எனில் (p == 3 && e ==  0) திரும்பு 8;

    /* We treat all Mac Latin names as ASCII only. */
    எனில் (p == 1 && e ==  0) திரும்பு 10; /* 10 is magic number :| */

    திரும்பு UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, மாறிலி வெறுமை *base
#ifdef HB_EXPERIMENTAL_API
                    , மாறிலி hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (இது);
    எனில் (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    எனில் (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      எழுத்து *name_str_utf16_be = nullptr;

      எனில் (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (எழுத்து *) hb_calloc (byte_len, 1);
        எனில் (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        எனில் (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      இல்லையெனில்
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        எனில் (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->எடு_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    இல்லையெனில்
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  தருக்க isUnicode () மாறிலி
  {
    unsigned முழு p = platformID;
    unsigned முழு e = encodingID;

    திரும்பு (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி NameRecord *a = (மாறிலி NameRecord *)pa;
    மாறிலி NameRecord *b = (மாறிலி NameRecord *)pb;

    எனில் (a->platformID != b->platformID)
      திரும்பு a->platformID - b->platformID;

    எனில் (a->encodingID != b->encodingID)
      திரும்பு a->encodingID - b->encodingID;

    எனில் (a->languageID != b->languageID)
      திரும்பு a->languageID - b->languageID;

    எனில் (a->nameID != b->nameID)
      திரும்பு a->nameID - b->nameID;

    எனில் (a->length != b->length)
      திரும்பு a->length - b->length;

    திரும்பு 0;
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
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
  பொது:
  DEFINE_SIZE_STATIC (12);
};

நிலையான முழு
hb_ot_name_entry_cmp_சாவி (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb, தருக்க exact)
{
  மாறிலி hb_ot_name_entry_t *a = (மாறிலி hb_ot_name_entry_t *) pa;
  மாறிலி hb_ot_name_entry_t *b = (மாறிலி hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  எனில் (a->name_id != b->name_id)
    திரும்பு a->name_id - b->name_id;

  எனில் (a->language == b->language) திரும்பு 0;
  எனில் (!a->language) திரும்பு -1;
  எனில் (!b->language) திரும்பு +1;

  மாறிலி எழுத்து *astr = hb_language_to_string (a->language);
  மாறிலி எழுத்து *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  எனில் (!exact && c &&
      hb_language_matches (b->language, a->language))
    திரும்பு 0;

  திரும்பு c;
}

நிலையான முழு
_hb_ot_name_entry_cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  முழு v = hb_ot_name_entry_cmp_சாவி (pa, pb, உண்மை);
  எனில் (v)
    திரும்பு v;

  மாறிலி hb_ot_name_entry_t *a = (மாறிலி hb_ot_name_entry_t *) pa;
  மாறிலி hb_ot_name_entry_t *b = (மாறிலி hb_ot_name_entry_t *) pb;

  எனில் (a->entry_score != b->entry_score)
    திரும்பு a->entry_score - b->entry_score;

  எனில் (a->entry_index != b->entry_index)
    திரும்பு a->entry_index - b->entry_index;

  திரும்பு 0;
}

கட்டமைப்பு name
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () மாறிலி
  { திரும்பு min_size + மொத்தம் * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, மாறிலி NameRecord &))>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it,
		  மாறிலி வெறுமை *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , மாறிலி hb_vector_t<hb_ot_name_record_ids_t>& செருகு_name_records
		  , மாறிலி hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (இது);

    எனில் (unlikely (!c->extend_min ((*இது))))  return_trace (பொய்);

    unsigned total_மொத்தம் = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + செருகு_name_records.length
#endif
        ;
    இது->format = 0;
    எனில் (!c->check_assign (இது->மொத்தம், total_மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW))
      திரும்பு பொய்;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_மொத்தம், NameRecord::static_size);
    எனில் (unlikely (!name_records)) return_trace (பொய்);

    hb_array_t<NameRecord> records (name_records, total_மொத்தம்);

    க்கு (மாறிலி NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    க்கு (unsigned i = 0; i < செருகு_name_records.length; i++)
    {
      மாறிலி hb_ot_name_record_ids_t& ids = செருகு_name_records[i];
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


    எனில் (unlikely (c->ran_out_of_room ())) return_trace (பொய்);

    இது->stringOffset = c->length ();

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    auto *name_பகாஎண் = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    மாறிலி hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (மொத்தம்)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (மாறிலி NameRecord& namerecord) {
      திரும்பு
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (மாறிலி NameRecord& namerecord) {
      எனில் (name_table_overrides->is_empty ())
        திரும்பு உண்மை;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      எனில் (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        திரும்பு பொய்;
      திரும்பு உண்மை;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    க்கு (மாறிலி NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> செருகு_name_records;
    எனில் (!name_table_overrides->is_empty ())
    {
      எனில் (unlikely (!செருகு_name_records.alloc (name_table_overrides->get_population (), உண்மை)))
        திரும்பு பொய்;
      க்கு (மாறிலி auto& record_ids : name_table_overrides->சாவிகள் ())
      {
        எனில் (name_table_overrides->get (record_ids).length == 0)
          தொடர்;
        எனில் (retained_name_record_ids.has (record_ids))
          தொடர்;
        செருகு_name_records.push (record_ids);
      }
    }
#endif

    திரும்பு name_பகாஎண்->serialize (c->serializer, it,
				  std::addressof (இது + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , செருகு_name_records
				  , name_table_overrides
#endif
				  );
  }

  தருக்க sanitize_records (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    மாறிலி வெறுமை *string_pool = (இது+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, மொத்தம், string_pool));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, மொத்தம்) &&
		  c->check_range (இது, stringOffset) &&
		  sanitize_records (c));
  }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      இது->table = hb_sanitize_context_t ().reference_table<name> (face);
      ப்ரதிஜ்ஞா (இது->table.get_length () >= இது->table->stringOffset);
      இது->pool = (மாறிலி எழுத்து *) (மாறிலி வெறுமை *) (இது->table+இது->table->stringOffset);
      இது->pool_len = இது->table.get_length () - இது->table->stringOffset;
      மாறிலி hb_array_t<மாறிலி NameRecord> all_names (இது->table->nameRecordZ.arrayZ,
						    இது->table->மொத்தம்);

      இது->names.alloc_exact (all_names.length);

      க்கு (unsigned முழு i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = இது->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      இது->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned முழு j = 0;
      க்கு (unsigned முழு i = 0; i < இது->names.length; i++)
      {
	எனில் (இது->names[i].entry_score == UNSUPPORTED ||
	    இது->names[i].language == HB_LANGUAGE_INVALID)
	  தொடர்;
	எனில் (i &&
	    இது->names[i - 1].name_id  == இது->names[i].name_id &&
	    இது->names[i - 1].language == இது->names[i].language)
	  தொடர்;
	இது->names[j++] = இது->names[i];
      }
      இது->names.resize (j);
    }
    ~accelerator_t ()
    {
      இது->table.destroy ();
    }

    முழு get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned முழு    *width=nullptr) மாறிலி
    {
      மாறிலி hb_ot_name_entry_t key = {name_id, {0}, language};
      மாறிலி hb_ot_name_entry_t *entry = hb_bsearch (key, (மாறிலி hb_ot_name_entry_t *) இது->names,
						    இது->names.length,
						    அளவு (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_சாவி,
						    உண்மை);

      எனில் (!entry)
      {
	entry = hb_bsearch (key, (மாறிலி hb_ot_name_entry_t *) இது->names,
			    இது->names.length,
			    அளவு (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_சாவி,
			    பொய்);
      }

      எனில் (!entry)
	திரும்பு -1;

      எனில் (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      திரும்பு entry->entry_index;
    }

    hb_bytes_t get_name (unsigned முழு idx) மாறிலி
    {
      மாறிலி hb_array_t<மாறிலி NameRecord> all_names (table->nameRecordZ.arrayZ, table->மொத்தம்);
      மாறிலி NameRecord &record = all_names[idx];
      மாறிலி hb_bytes_t string_pool (pool, pool_len);
      திரும்பு string_pool.sub_array (record.offset, record.length);
    }

    தனிப்பட்ட:
    மாறிலி எழுத்து *pool;
    unsigned முழு pool_len;
    பொது:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  பொது:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	மொத்தம்;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  பொது:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

கட்டமைப்பு name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
