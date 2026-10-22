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

#ضمن "../../hb-open-type.hh"
#ضمن "../../hb-ot-name-language.hh"
#ضمن "../../hb-aat-layout.hh"
#ضمن "../../hb-utf.hh"


نطاق OT {

template <typename in_utf_t, typename out_utf_t>
مضمن unsigned صحيح
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned صحيح                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned صحيح src_len = bytes.length / حجم (typename in_utf_t::codepoint_t);
  ثابت typename in_utf_t::codepoint_t *src = (ثابت typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ثابت typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ثابت hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  𐤀𐤌 (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    ثابت typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    طالما (src < src_end && dst < dst_end)
    {
      ثابت typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      𐤀𐤌 (dst_next == dst)
	اكسر; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned صحيح dst_len = dst - text;
  طالما (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  أرجع dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

بنية NameRecord
{
  hb_language_t language (hb_face_t *face) ثابت
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned صحيح p = platformID;
    unsigned صحيح l = languageID;

    𐤀𐤌 (p == 3)
      أرجع _hb_ot_name_language_for_ms_code (l);

    𐤀𐤌 (p == 1)
      أرجع _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    𐤀𐤌 (p == 0)
      أرجع face->table.ltag->get_language (l);
#endif

#endif
    أرجع HB_LANGUAGE_INVALID;
  }

  uint16_t score () ثابت
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned صحيح p = platformID;
    unsigned صحيح e = encodingID;

    /* 32-bit. */
    𐤀𐤌 (p == 3 && e == 10) أرجع 0;
    𐤀𐤌 (p == 0 && e ==  6) أرجع 1;
    𐤀𐤌 (p == 0 && e ==  4) أرجع 2;

    /* 16-bit. */
    𐤀𐤌 (p == 3 && e ==  1) أرجع 3;
    𐤀𐤌 (p == 0 && e ==  3) أرجع 4;
    𐤀𐤌 (p == 0 && e ==  2) أرجع 5;
    𐤀𐤌 (p == 0 && e ==  1) أرجع 6;
    𐤀𐤌 (p == 0 && e ==  0) أرجع 7;

    /* Symbol. */
    𐤀𐤌 (p == 3 && e ==  0) أرجع 8;

    /* We treat all Mac Latin names as ASCII only. */
    𐤀𐤌 (p == 1 && e ==  0) أرجع 10; /* 10 is magic number :| */

    أرجع UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ثابت فراغ *base
#ifdef HB_EXPERIMENTAL_API
                    , ثابت hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (هذا);
    𐤀𐤌 (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    𐤀𐤌 (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      حرف *name_str_utf16_be = nullptr;

      𐤀𐤌 (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (حرف *) hb_calloc (byte_len, 1);
        𐤀𐤌 (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        𐤀𐤌 (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      وإلا
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        𐤀𐤌 (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->pop_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    وإلا
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  منطقي isUnicode () ثابت
  {
    unsigned صحيح p = platformID;
    unsigned صحيح e = encodingID;

    أرجع (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت NameRecord *a = (ثابت NameRecord *)pa;
    ثابت NameRecord *b = (ثابت NameRecord *)pb;

    𐤀𐤌 (a->platformID != b->platformID)
      أرجع a->platformID - b->platformID;

    𐤀𐤌 (a->encodingID != b->encodingID)
      أرجع a->encodingID - b->encodingID;

    𐤀𐤌 (a->languageID != b->languageID)
      أرجع a->languageID - b->languageID;

    𐤀𐤌 (a->nameID != b->nameID)
      أرجع a->nameID - b->nameID;

    𐤀𐤌 (a->length != b->length)
      أرجع a->length - b->length;

    أرجع 0;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
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
  عام:
  DEFINE_SIZE_STATIC (12);
};

ساكن صحيح
_hb_ot_name_entry_cmp_key (ثابت فراغ *pa, ثابت فراغ *pb, منطقي exact)
{
  ثابت hb_ot_name_entry_t *a = (ثابت hb_ot_name_entry_t *) pa;
  ثابت hb_ot_name_entry_t *b = (ثابت hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  𐤀𐤌 (a->name_id != b->name_id)
    أرجع a->name_id - b->name_id;

  𐤀𐤌 (a->language == b->language) أرجع 0;
  𐤀𐤌 (!a->language) أرجع -1;
  𐤀𐤌 (!b->language) أرجع +1;

  ثابت حرف *astr = hb_language_to_string (a->language);
  ثابت حرف *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  𐤀𐤌 (!exact && c &&
      hb_language_matches (b->language, a->language))
    أرجع 0;

  أرجع c;
}

ساكن صحيح
_hb_ot_name_entry_cmp (ثابت فراغ *pa, ثابت فراغ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  صحيح v = _hb_ot_name_entry_cmp_key (pa, pb, true);
  𐤀𐤌 (v)
    أرجع v;

  ثابت hb_ot_name_entry_t *a = (ثابت hb_ot_name_entry_t *) pa;
  ثابت hb_ot_name_entry_t *b = (ثابت hb_ot_name_entry_t *) pb;

  𐤀𐤌 (a->entry_score != b->entry_score)
    أرجع a->entry_score - b->entry_score;

  𐤀𐤌 (a->entry_index != b->entry_index)
    أرجع a->entry_index - b->entry_index;

  أرجع 0;
}

بنية name
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () ثابت
  { أرجع min_size + count * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ثابت NameRecord &))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ثابت فراغ *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ثابت hb_vector_t<hb_ot_name_record_ids_t>& insert_name_records
		  , ثابت hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (هذا);

    𐤀𐤌 (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    unsigned total_count = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + insert_name_records.length
#endif
        ;
    هذا->format = 0;
    𐤀𐤌 (!c->check_assign (هذا->count, total_count, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      أرجع خطأ;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_count, NameRecord::static_size);
    𐤀𐤌 (unlikely (!name_records)) return_trace (خطأ);

    hb_array_t<NameRecord> records (name_records, total_count);

    لكل (ثابت NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    لكل (unsigned i = 0; i < insert_name_records.length; i++)
    {
      ثابت hb_ot_name_record_ids_t& ids = insert_name_records[i];
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


    𐤀𐤌 (unlikely (c->ran_out_of_room ())) return_trace (خطأ);

    هذا->stringOffset = c->length ();

    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    auto *name_prime = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ثابت hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (count)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ثابت NameRecord& namerecord) {
      أرجع
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ثابت NameRecord& namerecord) {
      𐤀𐤌 (name_table_overrides->is_empty ())
        أرجع true;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      𐤀𐤌 (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        أرجع خطأ;
      أرجع true;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    لكل (ثابت NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> insert_name_records;
    𐤀𐤌 (!name_table_overrides->is_empty ())
    {
      𐤀𐤌 (unlikely (!insert_name_records.alloc (name_table_overrides->get_population (), true)))
        أرجع خطأ;
      لكل (ثابت auto& record_ids : name_table_overrides->keys ())
      {
        𐤀𐤌 (name_table_overrides->get (record_ids).length == 0)
          استمر;
        𐤀𐤌 (retained_name_record_ids.has (record_ids))
          استمر;
        insert_name_records.push (record_ids);
      }
    }
#endif

    أرجع name_prime->serialize (c->serializer, it,
				  std::addressof (هذا + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , insert_name_records
				  , name_table_overrides
#endif
				  );
  }

  منطقي sanitize_records (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت فراغ *string_pool = (هذا+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, count, string_pool));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, count) &&
		  c->check_range (هذا, stringOffset) &&
		  sanitize_records (c));
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->table = hb_sanitize_context_t ().reference_table<name> (face);
      أكد (هذا->table.get_length () >= هذا->table->stringOffset);
      هذا->pool = (ثابت حرف *) (ثابت فراغ *) (هذا->table+هذا->table->stringOffset);
      هذا->pool_len = هذا->table.get_length () - هذا->table->stringOffset;
      ثابت hb_array_t<ثابت NameRecord> all_names (هذا->table->nameRecordZ.arrayZ,
						    هذا->table->count);

      هذا->names.alloc_exact (all_names.length);

      لكل (unsigned صحيح i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = هذا->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      هذا->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned صحيح j = 0;
      لكل (unsigned صحيح i = 0; i < هذا->names.length; i++)
      {
	𐤀𐤌 (هذا->names[i].entry_score == UNSUPPORTED ||
	    هذا->names[i].language == HB_LANGUAGE_INVALID)
	  استمر;
	𐤀𐤌 (i &&
	    هذا->names[i - 1].name_id  == هذا->names[i].name_id &&
	    هذا->names[i - 1].language == هذا->names[i].language)
	  استمر;
	هذا->names[j++] = هذا->names[i];
      }
      هذا->names.resize (j);
    }
    ~accelerator_t ()
    {
      هذا->table.destroy ();
    }

    صحيح get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned صحيح    *width=nullptr) ثابت
    {
      ثابت hb_ot_name_entry_t key = {name_id, {0}, language};
      ثابت hb_ot_name_entry_t *entry = hb_bsearch (key, (ثابت hb_ot_name_entry_t *) هذا->names,
						    هذا->names.length,
						    حجم (hb_ot_name_entry_t),
						    _hb_ot_name_entry_cmp_key,
						    true);

      𐤀𐤌 (!entry)
      {
	entry = hb_bsearch (key, (ثابت hb_ot_name_entry_t *) هذا->names,
			    هذا->names.length,
			    حجم (hb_ot_name_entry_t),
			    _hb_ot_name_entry_cmp_key,
			    خطأ);
      }

      𐤀𐤌 (!entry)
	أرجع -1;

      𐤀𐤌 (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      أرجع entry->entry_index;
    }

    hb_bytes_t get_name (unsigned صحيح idx) ثابت
    {
      ثابت hb_array_t<ثابت NameRecord> all_names (table->nameRecordZ.arrayZ, table->count);
      ثابت NameRecord &record = all_names[idx];
      ثابت hb_bytes_t string_pool (pool, pool_len);
      أرجع string_pool.sub_array (record.offset, record.length);
    }

    خاص:
    ثابت حرف *pool;
    unsigned صحيح pool_len;
    عام:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  عام:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	count;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  عام:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

بنية name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
