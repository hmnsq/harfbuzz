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

#شامل "../../hb-open-type.hh"
#شامل "../../hb-ot-name-language.hh"
#شامل "../../hb-aat-layout.hh"
#شامل "../../hb-utf.hh"


فضاینام OT {

template <typename in_utf_t, typename out_utf_t>
درونخطی unsigned صحیح
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned صحیح                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned صحیح src_len = bytes.length / اندازه (typename in_utf_t::codepoint_t);
  ثابت typename in_utf_t::codepoint_t *src = (ثابت typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ثابت typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  اگر (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    ثابت typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    وقتی (src < src_end && dst < dst_end)
    {
      ثابت typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      اگر (dst_next == dst)
	بشکن; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned صحیح dst_len = dst - text;
  وقتی (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  بازگشت dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

ساختار NameRecord
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned صحیح p = platformID;
    unsigned صحیح l = languageID;

    اگر (p == 3)
      بازگشت _hb_ot_name_language_for_ms_code (l);

    اگر (p == 1)
      بازگشت _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    اگر (p == 0)
      بازگشت face->table.ltag->get_language (l);
#endif

#endif
    بازگشت HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned صحیح p = platformID;
    unsigned صحیح e = encodingID;

    /* 32-bit. */
    اگر (p == 3 && e == 10) بازگشت 0;
    اگر (p == 0 && e ==  6) بازگشت 1;
    اگر (p == 0 && e ==  4) بازگشت 2;

    /* 16-bit. */
    اگر (p == 3 && e ==  1) بازگشت 3;
    اگر (p == 0 && e ==  3) بازگشت 4;
    اگر (p == 0 && e ==  2) بازگشت 5;
    اگر (p == 0 && e ==  1) بازگشت 6;
    اگر (p == 0 && e ==  0) بازگشت 7;

    /* Symbol. */
    اگر (p == 3 && e ==  0) بازگشت 8;

    /* We treat all Mac Latin names as ASCII only. */
    اگر (p == 1 && e ==  0) بازگشت 10; /* 10 is magic number :| */

    بازگشت UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ثابت تهی *base
#ifdef HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ثابت
  {
    TRACE_SERIALIZE (این);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (این);
    اگر (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    اگر (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      نویسه *name_str_utf16_be = nullptr;

      اگر (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (نویسه *) hb_calloc (byte_len, 1);
        اگر (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        اگر (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      وگرنه
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        اگر (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->برداشت_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    وگرنه
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  منطقی isUnicode () ثابت
  {
    unsigned صحیح p = platformID;
    unsigned صحیح e = encodingID;

    بازگشت (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
  {
    ثابت NameRecord *a = (ثابت NameRecord *)pa;
    ثابت NameRecord *b = (ثابت NameRecord *)pb;

    اگر (a->platformID != b->platformID)
      بازگشت a->platformID - b->platformID;

    اگر (a->encodingID != b->encodingID)
      بازگشت a->encodingID - b->encodingID;

    اگر (a->languageID != b->languageID)
      بازگشت a->languageID - b->languageID;

    اگر (a->nameID != b->nameID)
      بازگشت a->nameID - b->nameID;

    اگر (a->length != b->length)
      بازگشت a->length - b->length;

    بازگشت 0;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
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
  عمومی:
  DEFINE_SIZE_STATIC (12);
};

ایستا صحیح
hb_ot_name_entry_cmp_کلید (ثابت تهی *pa, ثابت تهی *pb, منطقی exact)
{
  ثابت hb_ot_name_entry_t *a = (ثابت hb_ot_name_entry_t *) pa;
  ثابت hb_ot_name_entry_t *b = (ثابت hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  اگر (a->name_id != b->name_id)
    بازگشت a->name_id - b->name_id;

  اگر (a->language == b->language) بازگشت 0;
  اگر (!a->language) بازگشت -1;
  اگر (!b->language) بازگشت +1;

  ثابت نویسه *astr = hb_language_to_string (a->language);
  ثابت نویسه *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  اگر (!exact && c &&
      hb_language_matches (b->language, a->language))
    بازگشت 0;

  بازگشت c;
}

ایستا صحیح
_hb_ot_name_entry_cmp (ثابت تهی *pa, ثابت تهی *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحیح v = hb_ot_name_entry_cmp_کلید (pa, pb, درست);
  اگر (v)
    بازگشت v;

  ثابت hb_ot_name_entry_t *a = (ثابت hb_ot_name_entry_t *) pa;
  ثابت hb_ot_name_entry_t *b = (ثابت hb_ot_name_entry_t *) pb;

  اگر (a->entry_score != b->entry_score)
    بازگشت a->entry_score - b->entry_score;

  اگر (a->entry_index != b->entry_index)
    بازگشت a->entry_index - b->entry_index;

  بازگشت 0;
}

ساختار name
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () ثابت
  { بازگشت min_size + شمار * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ثابت NameRecord &))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت تهی *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ثابت hb_vector_t<hb_ot_name_record_ids_t>& درج_name_records
		  , ثابت hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (این);

    اگر (unlikely (!c->extend_min ((*این))))  return_trace (نادرست);

    unsigned total_شمار = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + درج_name_records.length
#endif
        ;
    این->format = 0;
    اگر (!c->check_assign (این->شمار, total_شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      بازگشت نادرست;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_شمار, NameRecord::static_size);
    اگر (unlikely (!name_records)) return_trace (نادرست);

    hb_array_t<NameRecord> records (name_records, total_شمار);

    برای (ثابت NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    برای (unsigned i = 0; i < درج_name_records.length; i++)
    {
      ثابت hb_ot_name_record_ids_t& ids = درج_name_records[i];
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


    اگر (unlikely (c->ran_out_of_room ())) return_trace (نادرست);

    این->stringOffset = c->length ();

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    auto *name_اول = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (شمار)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ثابت NameRecord& namerecord) {
      بازگشت
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت NameRecord& namerecord) {
      اگر (name_table_overrides->is_empty ())
        بازگشت درست;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      اگر (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        بازگشت نادرست;
      بازگشت درست;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    برای (ثابت NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> درج_name_records;
    اگر (!name_table_overrides->is_empty ())
    {
      اگر (unlikely (!درج_name_records.alloc (name_table_overrides->get_population (), درست)))
        بازگشت نادرست;
      برای (ثابت auto& record_ids : name_table_overrides->کلیدها ())
      {
        اگر (name_table_overrides->get (record_ids).length == 0)
          ادامه;
        اگر (retained_name_record_ids.has (record_ids))
          ادامه;
        درج_name_records.push (record_ids);
      }
    }
#endif

    بازگشت name_اول->serialize (c->serializer, it,
				  std::addressof (این + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , درج_name_records
				  , name_table_overrides
#endif
				  );
  }

  منطقی sanitize_records (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    ثابت تهی *string_pool = (این+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, شمار, string_pool));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, شمار) &&
		  c->check_range (این, stringOffset) &&
		  sanitize_records (c));
  }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      این->table = hb_sanitize_context_t ().reference_table<name> (face);
      أكد (این->table.get_length () >= این->table->stringOffset);
      این->pool = (ثابت نویسه *) (ثابت تهی *) (این->table+این->table->stringOffset);
      این->pool_len = این->table.get_length () - این->table->stringOffset;
      ثابت hb_array_t<ثابت NameRecord> all_names (این->table->nameRecordZ.arrayZ,
						    این->table->شمار);

      این->names.alloc_exact (all_names.length);

      برای (unsigned صحیح i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = این->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      این->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned صحیح j = 0;
      برای (unsigned صحیح i = 0; i < این->names.length; i++)
      {
	اگر (این->names[i].entry_score == UNSUPPORTED ||
	    این->names[i].language == HB_LANGUAGE_INVALID)
	  ادامه;
	اگر (i &&
	    این->names[i - 1].name_id  == این->names[i].name_id &&
	    این->names[i - 1].language == این->names[i].language)
	  ادامه;
	این->names[j++] = این->names[i];
      }
      این->names.resize (j);
    }
    ~accelerator_t ()
    {
      این->table.destroy ();
    }

    صحیح get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned صحیح    *width=nullptr) ثابت
    {
      ثابت hb_ot_name_entry_t key = {name_id, {0}, language};
      ثابت hb_ot_name_entry_t *entry = hb_bsearch (key, (ثابت hb_ot_name_entry_t *) این->names,
						    این->names.length,
						    اندازه (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_کلید,
						    درست);

      اگر (!entry)
      {
	entry = hb_bsearch (key, (ثابت hb_ot_name_entry_t *) این->names,
			    این->names.length,
			    اندازه (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_کلید,
			    نادرست);
      }

      اگر (!entry)
	بازگشت -1;

      اگر (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      بازگشت entry->entry_index;
    }

    hb_bytes_t get_name (unsigned صحیح idx) ثابت
    {
      ثابت hb_array_t<ثابت NameRecord> all_names (table->nameRecordZ.arrayZ, table->شمار);
      ثابت NameRecord &record = all_names[idx];
      ثابت hb_bytes_t string_pool (pool, pool_len);
      بازگشت string_pool.sub_array (record.offset, record.length);
    }

    خصوصی:
    ثابت نویسه *pool;
    unsigned صحیح pool_len;
    عمومی:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  عمومی:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	شمار;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  عمومی:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

ساختار name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
