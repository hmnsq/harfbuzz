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
অন্তঃরেখ unsigned পূর্ণসংখ্যা
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned পূর্ণসংখ্যা                    *text_আকার /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned পূর্ণসংখ্যা src_len = bytes.দৈর্ঘ্য / আকার (typename in_utf_t::codepoint_t);
  ধ্রুবক typename in_utf_t::codepoint_t *src = (ধ্রুবক typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ধ্রুবক typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ধ্রুবক hb_codepoint_t replacement = HB_বাফার_REPLACEMENT_CODEPOINT_DEFAULT;

  যদি (text_আকার && *text_আকার)
  {
    (*text_আকার)--; /* Save room for NUL-termination. */
    ধ্রুবক typename out_utf_t::codepoint_t *dst_end = text + *text_আকার;

    যখন (src < src_end && dst < dst_end)
    {
      ধ্রুবক typename in_utf_t::codepoint_t *src_পরবর্তী = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_পরবর্তী = out_utf_t::encode (dst, dst_end, unicode);
      যদি (dst_পরবর্তী == dst)
	বিরতি; /* Out-of-room. */

      dst = dst_পরবর্তী;
      src = src_পরবর্তী;
    }

    *text_আকার = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned পূর্ণসংখ্যা dst_len = dst - text;
  যখন (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  ফেরত dst_len;
}

#define entry_score var.u16[0]
#define entry_সূচক var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

কাঠামো NameRecord
{
  hb_language_t language (hb_face_t *face) ধ্রুবক
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned পূর্ণসংখ্যা p = platformID;
    unsigned পূর্ণসংখ্যা l = languageID;

    যদি (p == 3)
      ফেরত _hb_ot_name_language_for_ms_code (l);

    যদি (p == 1)
      ফেরত _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    যদি (p == 0)
      ফেরত face->table.ltag->get_language (l);
#endif

#endif
    ফেরত HB_LANGUAGE_INVALID;
  }

  uint16_t score () ধ্রুবক
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned পূর্ণসংখ্যা p = platformID;
    unsigned পূর্ণসংখ্যা e = encodingID;

    /* 32-bit. */
    যদি (p == 3 && e == 10) ফেরত 0;
    যদি (p == 0 && e ==  6) ফেরত 1;
    যদি (p == 0 && e ==  4) ফেরত 2;

    /* 16-bit. */
    যদি (p == 3 && e ==  1) ফেরত 3;
    যদি (p == 0 && e ==  3) ফেরত 4;
    যদি (p == 0 && e ==  2) ফেরত 5;
    যদি (p == 0 && e ==  1) ফেরত 6;
    যদি (p == 0 && e ==  0) ফেরত 7;

    /* Symbol. */
    যদি (p == 3 && e ==  0) ফেরত 8;

    /* We treat all Mac Latin names as ASCII only. */
    যদি (p == 1 && e ==  0) ফেরত 10; /* 10 is magic number :| */

    ফেরত UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ধ্রুবক শূন্য *base
#ifdef HB_EXPERIMENTAL_API
                    , ধ্রুবক hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (এই);
    যদি (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    যদি (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      অক্ষর *name_str_utf16_be = nullptr;

      যদি (platformID != 1)
      {
        unsigned text_আকার = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_আকার++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_আকার * hb_utf16_be_t::codepoint_t::static_আকার;
        name_str_utf16_be = (অক্ষর *) hb_calloc (byte_len, 1);
        যদি (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_আকার,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_আকার * hb_utf16_be_t::codepoint_t::static_আকার;
        যদি (!encoded_byte_len || !c->check_assign (out->দৈর্ঘ্য, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      অন্যথা
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        যদি (!c->check_assign (out->দৈর্ঘ্য, encoded_bytes.দৈর্ঘ্য, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->বের_করো_pack (), hb_serialize_context_t::লেজ_2, 0);
      hb_free (name_str_utf16_be);
    }
    অন্যথা
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::লেজ_2, দৈর্ঘ্য);
    }
    return_trace (out);
  }

  যৌক্তিক isUnicode () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা p = platformID;
    unsigned পূর্ণসংখ্যা e = encodingID;

    ফেরত (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক NameRecord *a = (ধ্রুবক NameRecord *)pa;
    ধ্রুবক NameRecord *b = (ধ্রুবক NameRecord *)pb;

    যদি (a->platformID != b->platformID)
      ফেরত a->platformID - b->platformID;

    যদি (a->encodingID != b->encodingID)
      ফেরত a->encodingID - b->encodingID;

    যদি (a->languageID != b->languageID)
      ফেরত a->languageID - b->languageID;

    যদি (a->nameID != b->nameID)
      ফেরত a->nameID - b->nameID;

    যদি (a->দৈর্ঘ্য != b->দৈর্ঘ্য)
      ফেরত a->দৈর্ঘ্য - b->দৈর্ঘ্য;

    ফেরত 0;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  offset.sanitize (c, base, দৈর্ঘ্য));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	দৈর্ঘ্য;		/* String length (in bytes). */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		offset;		/* String offset from start of storage area (in bytes). */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

স্থির পূর্ণসংখ্যা
hb_ot_name_entry_cmp_চাবি (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb, যৌক্তিক exact)
{
  ধ্রুবক hb_ot_name_entry_t *a = (ধ্রুবক hb_ot_name_entry_t *) pa;
  ধ্রুবক hb_ot_name_entry_t *b = (ধ্রুবক hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  যদি (a->name_id != b->name_id)
    ফেরত a->name_id - b->name_id;

  যদি (a->language == b->language) ফেরত 0;
  যদি (!a->language) ফেরত -1;
  যদি (!b->language) ফেরত +1;

  ধ্রুবক অক্ষর *astr = hb_language_to_string (a->language);
  ধ্রুবক অক্ষর *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  যদি (!exact && c &&
      hb_language_matches (b->language, a->language))
    ফেরত 0;

  ফেরত c;
}

স্থির পূর্ণসংখ্যা
_hb_ot_name_entry_cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  পূর্ণসংখ্যা v = hb_ot_name_entry_cmp_চাবি (pa, pb, সত্য);
  যদি (v)
    ফেরত v;

  ধ্রুবক hb_ot_name_entry_t *a = (ধ্রুবক hb_ot_name_entry_t *) pa;
  ধ্রুবক hb_ot_name_entry_t *b = (ধ্রুবক hb_ot_name_entry_t *) pb;

  যদি (a->entry_score != b->entry_score)
    ফেরত a->entry_score - b->entry_score;

  যদি (a->entry_সূচক != b->entry_সূচক)
    ফেরত a->entry_সূচক - b->entry_সূচক;

  ফেরত 0;
}

কাঠামো name
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_আকার () ধ্রুবক
  { ফেরত min_আকার + গুনতি * nameRecordZ.item_আকার; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ধ্রুবক NameRecord &))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ধ্রুবক শূন্য *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ধ্রুবক hb_vector_t<hb_ot_name_record_ids_t>& ঢোকাও_name_records
		  , ধ্রুবক hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (এই);

    যদি (unlikely (!c->extend_min ((*এই))))  return_trace (মিথ্যা);

    unsigned যোগ_গুনতি = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + ঢোকাও_name_records.দৈর্ঘ্য
#endif
        ;
    এই->format = 0;
    যদি (!c->check_assign (এই->গুনতি, যোগ_গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      ফেরত মিথ্যা;

    NameRecord *name_records = (NameRecord *) hb_calloc (যোগ_গুনতি, NameRecord::static_আকার);
    যদি (unlikely (!name_records)) return_trace (মিথ্যা);

    hb_array_t<NameRecord> records (name_records, যোগ_গুনতি);

    জন্য (ধ্রুবক NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_আকার);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    জন্য (unsigned i = 0; i < ঢোকাও_name_records.দৈর্ঘ্য; i++)
    {
      ধ্রুবক hb_ot_name_record_ids_t& ids = ঢোকাও_name_records[i];
      NameRecord record;
      record.platformID = ids.platform_id;
      record.encodingID = ids.encoding_id;
      record.languageID = ids.language_id;
      record.nameID = ids.name_id;
      record.দৈর্ঘ্য = 0; // handled in NameRecord copy()
      record.offset = 0;
      hb_memcpy (name_records, &record, NameRecord::static_আকার);
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


    যদি (unlikely (c->ran_out_of_room ())) return_trace (মিথ্যা);

    এই->stringOffset = c->দৈর্ঘ্য ();

    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    auto *name_মৌলিক = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ধ্রুবক hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (গুনতি)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ধ্রুবক NameRecord& namerecord) {
      ফেরত
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ধ্রুবক NameRecord& namerecord) {
      যদি (name_table_overrides->is_empty ())
        ফেরত সত্য;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      যদি (name_table_overrides->has (rec_ids, &p) &&
          (*p).দৈর্ঘ্য == 0)
        ফেরত মিথ্যা;
      ফেরত সত্য;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    জন্য (ধ্রুবক NameRecord& rec : it)
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
        ফেরত মিথ্যা;
      জন্য (ধ্রুবক auto& record_ids : name_table_overrides->চাবিগুলি ())
      {
        যদি (name_table_overrides->get (record_ids).দৈর্ঘ্য == 0)
          চালু;
        যদি (retained_name_record_ids.has (record_ids))
          চালু;
        ঢোকাও_name_records.push (record_ids);
      }
    }
#endif

    ফেরত name_মৌলিক->serialize (c->serializer, it,
				  std::addressof (এই + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , ঢোকাও_name_records
				  , name_table_overrides
#endif
				  );
  }

  যৌক্তিক sanitize_records (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    ধ্রুবক শূন্য *string_pool = (এই+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, গুনতি, string_pool));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, গুনতি) &&
		  c->check_range (এই, stringOffset) &&
		  sanitize_records (c));
  }

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      এই->table = hb_sanitize_context_t ().reference_table<name> (face);
      প্রতিজ্ঞা (এই->table.get_দৈর্ঘ্য () >= এই->table->stringOffset);
      এই->pool = (ধ্রুবক অক্ষর *) (ধ্রুবক শূন্য *) (এই->table+এই->table->stringOffset);
      এই->pool_len = এই->table.get_দৈর্ঘ্য () - এই->table->stringOffset;
      ধ্রুবক hb_array_t<ধ্রুবক NameRecord> all_names (এই->table->nameRecordZ.arrayZ,
						    এই->table->গুনতি);

      এই->names.alloc_exact (all_names.দৈর্ঘ্য);

      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < all_names.দৈর্ঘ্য; i++)
      {
	hb_ot_name_entry_t *entry = এই->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_সূচক = i;
      }

      এই->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned পূর্ণসংখ্যা j = 0;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < এই->names.দৈর্ঘ্য; i++)
      {
	যদি (এই->names[i].entry_score == UNSUPPORTED ||
	    এই->names[i].language == HB_LANGUAGE_INVALID)
	  চালু;
	যদি (i &&
	    এই->names[i - 1].name_id  == এই->names[i].name_id &&
	    এই->names[i - 1].language == এই->names[i].language)
	  চালু;
	এই->names[j++] = এই->names[i];
      }
      এই->names.resize (j);
    }
    ~accelerator_t ()
    {
      এই->table.destroy ();
    }

    পূর্ণসংখ্যা get_সূচক (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned পূর্ণসংখ্যা    *width=nullptr) ধ্রুবক
    {
      ধ্রুবক hb_ot_name_entry_t key = {name_id, {0}, language};
      ধ্রুবক hb_ot_name_entry_t *entry = hb_bsearch (key, (ধ্রুবক hb_ot_name_entry_t *) এই->names,
						    এই->names.দৈর্ঘ্য,
						    আকার (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_চাবি,
						    সত্য);

      যদি (!entry)
      {
	entry = hb_bsearch (key, (ধ্রুবক hb_ot_name_entry_t *) এই->names,
			    এই->names.দৈর্ঘ্য,
			    আকার (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_চাবি,
			    মিথ্যা);
      }

      যদি (!entry)
	ফেরত -1;

      যদি (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      ফেরত entry->entry_সূচক;
    }

    hb_bytes_t get_name (unsigned পূর্ণসংখ্যা idx) ধ্রুবক
    {
      ধ্রুবক hb_array_t<ধ্রুবক NameRecord> all_names (table->nameRecordZ.arrayZ, table->গুনতি);
      ধ্রুবক NameRecord &record = all_names[idx];
      ধ্রুবক hb_bytes_t string_pool (pool, pool_len);
      ফেরত string_pool.sub_array (record.offset, record.দৈর্ঘ্য);
    }

    ব্যক্তিগত:
    ধ্রুবক অক্ষর *pool;
    unsigned পূর্ণসংখ্যা pool_len;
    সর্বজনীন:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  সর্বজনীন:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	গুনতি;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, nameRecordZ);
};

#undef entry_সূচক
#undef entry_score

কাঠামো name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
