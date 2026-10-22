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
ओळीत अचिन्हित पूर्णांक
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			अचिन्हित पूर्णांक                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  अचिन्हित पूर्णांक src_len = bytes.length / आकार (typename in_utf_t::codepoint_t);
  स्थिरांक typename in_utf_t::codepoint_t *src = (स्थिरांक typename in_utf_t::codepoint_t *) bytes.arrayZ;
  स्थिरांक typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  स्थिरांक hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  जर (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    स्थिरांक typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    पर्यंत (src < src_end && dst < dst_end)
    {
      स्थिरांक typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      जर (dst_next == dst)
	थांबा; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  अचिन्हित पूर्णांक dst_len = dst - text;
  पर्यंत (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  परत dst_len;
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
  hb_language_t language (hb_face_t *face) स्थिरांक
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    अचिन्हित पूर्णांक p = platformID;
    अचिन्हित पूर्णांक l = languageID;

    जर (p == 3)
      परत _hb_ot_name_language_for_ms_code (l);

    जर (p == 1)
      परत _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    जर (p == 0)
      परत face->table.ltag->get_language (l);
#endif

#endif
    परत HB_LANGUAGE_INVALID;
  }

  uint16_t score () स्थिरांक
  {
    /* Same order as in cmap::find_best_subtable(). */
    अचिन्हित पूर्णांक p = platformID;
    अचिन्हित पूर्णांक e = encodingID;

    /* 32-bit. */
    जर (p == 3 && e == 10) परत 0;
    जर (p == 0 && e ==  6) परत 1;
    जर (p == 0 && e ==  4) परत 2;

    /* 16-bit. */
    जर (p == 3 && e ==  1) परत 3;
    जर (p == 0 && e ==  3) परत 4;
    जर (p == 0 && e ==  2) परत 5;
    जर (p == 0 && e ==  1) परत 6;
    जर (p == 0 && e ==  0) परत 7;

    /* Symbol. */
    जर (p == 3 && e ==  0) परत 8;

    /* We treat all Mac Latin names as ASCII only. */
    जर (p == 1 && e ==  0) परत 10; /* 10 is magic number :| */

    परत UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, स्थिरांक शून्य *base
#ifdef HB_EXPERIMENTAL_API
                    , स्थिरांक hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    HB_UNUSED स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed (हे);
    जर (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    जर (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      अक्षर *name_str_utf16_be = nullptr;

      जर (platformID != 1)
      {
        अचिन्हित text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        अचिन्हित byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (अक्षर *) hb_calloc (byte_len, 1);
        जर (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        अचिन्हित encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        जर (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      अन्यथा
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        जर (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->काढा_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    अन्यथा
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  तार्किक isUnicode () स्थिरांक
  {
    अचिन्हित पूर्णांक p = platformID;
    अचिन्हित पूर्णांक e = encodingID;

    परत (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  स्थिर पूर्णांक cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    स्थिरांक NameRecord *a = (स्थिरांक NameRecord *)pa;
    स्थिरांक NameRecord *b = (स्थिरांक NameRecord *)pb;

    जर (a->platformID != b->platformID)
      परत a->platformID - b->platformID;

    जर (a->encodingID != b->encodingID)
      परत a->encodingID - b->encodingID;

    जर (a->languageID != b->languageID)
      परत a->languageID - b->languageID;

    जर (a->nameID != b->nameID)
      परत a->nameID - b->nameID;

    जर (a->length != b->length)
      परत a->length - b->length;

    परत 0;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
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

स्थिर पूर्णांक
hb_ot_name_entry_cmp_किल्ली (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb, तार्किक exact)
{
  स्थिरांक hb_ot_name_entry_t *a = (स्थिरांक hb_ot_name_entry_t *) pa;
  स्थिरांक hb_ot_name_entry_t *b = (स्थिरांक hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  जर (a->name_id != b->name_id)
    परत a->name_id - b->name_id;

  जर (a->language == b->language) परत 0;
  जर (!a->language) परत -1;
  जर (!b->language) परत +1;

  स्थिरांक अक्षर *astr = hb_language_to_string (a->language);
  स्थिरांक अक्षर *bstr = hb_language_to_string (b->language);

  चिन्हित c = म_भेद (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  जर (!exact && c &&
      hb_language_matches (b->language, a->language))
    परत 0;

  परत c;
}

स्थिर पूर्णांक
_hb_ot_name_entry_cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  पूर्णांक v = hb_ot_name_entry_cmp_किल्ली (pa, pb, सत्य);
  जर (v)
    परत v;

  स्थिरांक hb_ot_name_entry_t *a = (स्थिरांक hb_ot_name_entry_t *) pa;
  स्थिरांक hb_ot_name_entry_t *b = (स्थिरांक hb_ot_name_entry_t *) pb;

  जर (a->entry_score != b->entry_score)
    परत a->entry_score - b->entry_score;

  जर (a->entry_index != b->entry_index)
    परत a->entry_index - b->entry_index;

  परत 0;
}

संरचना name
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  माप_प्रकार get_size () स्थिरांक
  { परत min_size + मोजणी * nameRecordZ.item_size; }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, स्थिरांक NameRecord &))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it,
		  स्थिरांक शून्य *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , स्थिरांक hb_vector_t<hb_ot_name_record_ids_t>& घाला_name_records
		  , स्थिरांक hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (हे);

    जर (unlikely (!c->extend_min ((*हे))))  return_trace (असत्य);

    अचिन्हित total_मोजणी = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + घाला_name_records.length
#endif
        ;
    हे->format = 0;
    जर (!c->check_assign (हे->मोजणी, total_मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      परत असत्य;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_मोजणी, NameRecord::static_size);
    जर (unlikely (!name_records)) return_trace (असत्य);

    hb_array_t<NameRecord> records (name_records, total_मोजणी);

    साठी (स्थिरांक NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    साठी (अचिन्हित i = 0; i < घाला_name_records.length; i++)
    {
      स्थिरांक hb_ot_name_record_ids_t& ids = घाला_name_records[i];
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


    जर (unlikely (c->ran_out_of_room ())) return_trace (असत्य);

    हे->stringOffset = c->length ();

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    स्वतः *name_मूळसंख्या = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    स्थिरांक hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    स्वतः it =
    + nameRecordZ.as_array (मोजणी)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (स्थिरांक NameRecord& namerecord) {
      परत
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (स्थिरांक NameRecord& namerecord) {
      जर (name_table_overrides->is_empty ())
        परत सत्य;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      जर (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        परत असत्य;
      परत सत्य;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, अचिन्हित> retained_name_record_ids;
    साठी (स्थिरांक NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> घाला_name_records;
    जर (!name_table_overrides->is_empty ())
    {
      जर (unlikely (!घाला_name_records.alloc (name_table_overrides->get_population (), सत्य)))
        परत असत्य;
      साठी (स्थिरांक स्वतः& record_ids : name_table_overrides->किल्ल्या ())
      {
        जर (name_table_overrides->get (record_ids).length == 0)
          सुरूठेवा;
        जर (retained_name_record_ids.has (record_ids))
          सुरूठेवा;
        घाला_name_records.push (record_ids);
      }
    }
#endif

    परत name_मूळसंख्या->serialize (c->serializer, it,
				  std::addressof (हे + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , घाला_name_records
				  , name_table_overrides
#endif
				  );
  }

  तार्किक sanitize_records (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    स्थिरांक शून्य *string_pool = (हे+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, मोजणी, string_pool));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, मोजणी) &&
		  c->check_range (हे, stringOffset) &&
		  sanitize_records (c));
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      हे->table = hb_sanitize_context_t ().reference_table<name> (face);
      प्रतिज्ञा (हे->table.get_length () >= हे->table->stringOffset);
      हे->pool = (स्थिरांक अक्षर *) (स्थिरांक शून्य *) (हे->table+हे->table->stringOffset);
      हे->pool_len = हे->table.get_length () - हे->table->stringOffset;
      स्थिरांक hb_array_t<स्थिरांक NameRecord> all_names (हे->table->nameRecordZ.arrayZ,
						    हे->table->मोजणी);

      हे->names.alloc_exact (all_names.length);

      साठी (अचिन्हित पूर्णांक i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = हे->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      हे->names.क्विक (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      अचिन्हित पूर्णांक j = 0;
      साठी (अचिन्हित पूर्णांक i = 0; i < हे->names.length; i++)
      {
	जर (हे->names[i].entry_score == UNSUPPORTED ||
	    हे->names[i].language == HB_LANGUAGE_INVALID)
	  सुरूठेवा;
	जर (i &&
	    हे->names[i - 1].name_id  == हे->names[i].name_id &&
	    हे->names[i - 1].language == हे->names[i].language)
	  सुरूठेवा;
	हे->names[j++] = हे->names[i];
      }
      हे->names.resize (j);
    }
    ~accelerator_t ()
    {
      हे->table.destroy ();
    }

    पूर्णांक get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   अचिन्हित पूर्णांक    *width=nullptr) स्थिरांक
    {
      स्थिरांक hb_ot_name_entry_t key = {name_id, {0}, language};
      स्थिरांक hb_ot_name_entry_t *entry = hb_bsearch (key, (स्थिरांक hb_ot_name_entry_t *) हे->names,
						    हे->names.length,
						    आकार (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_किल्ली,
						    सत्य);

      जर (!entry)
      {
	entry = hb_bsearch (key, (स्थिरांक hb_ot_name_entry_t *) हे->names,
			    हे->names.length,
			    आकार (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_किल्ली,
			    असत्य);
      }

      जर (!entry)
	परत -1;

      जर (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      परत entry->entry_index;
    }

    hb_bytes_t get_name (अचिन्हित पूर्णांक idx) स्थिरांक
    {
      स्थिरांक hb_array_t<स्थिरांक NameRecord> all_names (table->nameRecordZ.arrayZ, table->मोजणी);
      स्थिरांक NameRecord &record = all_names[idx];
      स्थिरांक hb_bytes_t string_pool (pool, pool_len);
      परत string_pool.sub_array (record.offset, record.length);
    }

    खाजगी:
    स्थिरांक अक्षर *pool;
    अचिन्हित पूर्णांक pool_len;
    सार्वजनिक:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  सार्वजनिक:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	मोजणी;		/* Number of name records. */
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
