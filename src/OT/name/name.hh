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

#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-ot-name-language.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-aat-layout.hh"
#ଅନ୍ତର୍ଭାଵ "../../hb-utf.hh"


ନାମାକାଶ OT {

template <typename in_utf_t, typename out_utf_t>
ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_len = bytes.length / ଆକାର (typename in_utf_t::codepoint_t);
  ଅଚଳ typename in_utf_t::codepoint_t *src = (ଅଚଳ typename in_utf_t::codepoint_t *) bytes.arrayZ;
  ଅଚଳ typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  ଅଚଳ hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  ଯଦି (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    ଅଚଳ typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    ଯେତେବେଳେ (src < src_end && dst < dst_end)
    {
      ଅଚଳ typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      ଯଦି (dst_next == dst)
	ଭାଙ୍ଗ; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst_len = dst - text;
  ଯେତେବେଳେ (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  ଫେରନ୍ତୁ dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

ସଂରଚନା NameRecord
{
  hb_language_t language (hb_face_t *face) ଅଚଳ
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = platformID;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = languageID;

    ଯଦି (p == 3)
      ଫେରନ୍ତୁ _hb_ot_name_language_for_ms_code (l);

    ଯଦି (p == 1)
      ଫେରନ୍ତୁ _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    ଯଦି (p == 0)
      ଫେରନ୍ତୁ face->table.ltag->get_language (l);
#endif

#endif
    ଫେରନ୍ତୁ HB_LANGUAGE_INVALID;
  }

  uint16_t score () ଅଚଳ
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = platformID;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା e = encodingID;

    /* 32-bit. */
    ଯଦି (p == 3 && e == 10) ଫେରନ୍ତୁ 0;
    ଯଦି (p == 0 && e ==  6) ଫେରନ୍ତୁ 1;
    ଯଦି (p == 0 && e ==  4) ଫେରନ୍ତୁ 2;

    /* 16-bit. */
    ଯଦି (p == 3 && e ==  1) ଫେରନ୍ତୁ 3;
    ଯଦି (p == 0 && e ==  3) ଫେରନ୍ତୁ 4;
    ଯଦି (p == 0 && e ==  2) ଫେରନ୍ତୁ 5;
    ଯଦି (p == 0 && e ==  1) ଫେରନ୍ତୁ 6;
    ଯଦି (p == 0 && e ==  0) ଫେରନ୍ତୁ 7;

    /* Symbol. */
    ଯଦି (p == 3 && e ==  0) ଫେରନ୍ତୁ 8;

    /* We treat all Mac Latin names as ASCII only. */
    ଯଦି (p == 1 && e ==  0) ଫେରନ୍ତୁ 10; /* 10 is magic number :| */

    ଫେରନ୍ତୁ UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base
#ifdef HB_EXPERIMENTAL_API
                    , ଅଚଳ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    ଯଦି (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      ଅକ୍ଷର *name_str_utf16_be = nullptr;

      ଯଦି (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (ଅକ୍ଷର *) hb_calloc (byte_len, 1);
        ଯଦି (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        ଯଦି (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      ଅନ୍ୟଥା
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        ଯଦି (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->କାଢ଼_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    ଅନ୍ୟଥା
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  ତାର୍କିକ isUnicode () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = platformID;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା e = encodingID;

    ଫେରନ୍ତୁ (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    ଅଚଳ NameRecord *a = (ଅଚଳ NameRecord *)pa;
    ଅଚଳ NameRecord *b = (ଅଚଳ NameRecord *)pb;

    ଯଦି (a->platformID != b->platformID)
      ଫେରନ୍ତୁ a->platformID - b->platformID;

    ଯଦି (a->encodingID != b->encodingID)
      ଫେରନ୍ତୁ a->encodingID - b->encodingID;

    ଯଦି (a->languageID != b->languageID)
      ଫେରନ୍ତୁ a->languageID - b->languageID;

    ଯଦି (a->nameID != b->nameID)
      ଫେରନ୍ତୁ a->nameID - b->nameID;

    ଯଦି (a->length != b->length)
      ଫେରନ୍ତୁ a->length - b->length;

    ଫେରନ୍ତୁ 0;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_name_entry_cmp_ଚାବି (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb, ତାର୍କିକ exact)
{
  ଅଚଳ hb_ot_name_entry_t *a = (ଅଚଳ hb_ot_name_entry_t *) pa;
  ଅଚଳ hb_ot_name_entry_t *b = (ଅଚଳ hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  ଯଦି (a->name_id != b->name_id)
    ଫେରନ୍ତୁ a->name_id - b->name_id;

  ଯଦି (a->language == b->language) ଫେରନ୍ତୁ 0;
  ଯଦି (!a->language) ଫେରନ୍ତୁ -1;
  ଯଦି (!b->language) ଫେରନ୍ତୁ +1;

  ଅଚଳ ଅକ୍ଷର *astr = hb_language_to_string (a->language);
  ଅଚଳ ଅକ୍ଷର *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  ଯଦି (!exact && c &&
      hb_language_matches (b->language, a->language))
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ c;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_hb_ot_name_entry_cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = hb_ot_name_entry_cmp_ଚାବି (pa, pb, ସତ୍ୟ);
  ଯଦି (v)
    ଫେରନ୍ତୁ v;

  ଅଚଳ hb_ot_name_entry_t *a = (ଅଚଳ hb_ot_name_entry_t *) pa;
  ଅଚଳ hb_ot_name_entry_t *b = (ଅଚଳ hb_ot_name_entry_t *) pb;

  ଯଦି (a->entry_score != b->entry_score)
    ଫେରନ୍ତୁ a->entry_score - b->entry_score;

  ଯଦି (a->entry_index != b->entry_index)
    ଫେରନ୍ତୁ a->entry_index - b->entry_index;

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା name
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ min_size + ଗଣନା_2 * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, ଅଚଳ NameRecord &))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  ଅଚଳ ଶୂନ୍ୟ *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , ଅଚଳ hb_vector_t<hb_ot_name_record_ids_t>& ଭର୍ତ୍ତି_name_records
		  , ଅଚଳ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (unlikely (!c->extend_min ((*ଏହି))))  return_trace (ମିଥ୍ୟା);

    unsigned total_ଗଣନା = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + ଭର୍ତ୍ତି_name_records.length
#endif
        ;
    ଏହି->format = 0;
    ଯଦି (!c->check_assign (ଏହି->ଗଣନା_2, total_ଗଣନା, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_ଗଣନା, NameRecord::static_size);
    ଯଦି (unlikely (!name_records)) return_trace (ମିଥ୍ୟା);

    hb_array_t<NameRecord> records (name_records, total_ଗଣନା);

    ପାଇଁ (ଅଚଳ NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    ପାଇଁ (unsigned i = 0; i < ଭର୍ତ୍ତି_name_records.length; i++)
    {
      ଅଚଳ hb_ot_name_record_ids_t& ids = ଭର୍ତ୍ତି_name_records[i];
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


    ଯଦି (unlikely (c->ran_out_of_room ())) return_trace (ମିଥ୍ୟା);

    ଏହି->stringOffset = c->length ();

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    auto *name_ମୌଳିକ = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    ଅଚଳ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (ଗଣନା_2)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (ଅଚଳ NameRecord& namerecord) {
      ଫେରନ୍ତୁ
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (ଅଚଳ NameRecord& namerecord) {
      ଯଦି (name_table_overrides->is_empty ())
        ଫେରନ୍ତୁ ସତ୍ୟ;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      ଯଦି (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    ପାଇଁ (ଅଚଳ NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> ଭର୍ତ୍ତି_name_records;
    ଯଦି (!name_table_overrides->is_empty ())
    {
      ଯଦି (unlikely (!ଭର୍ତ୍ତି_name_records.alloc (name_table_overrides->get_population (), ସତ୍ୟ)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପାଇଁ (ଅଚଳ auto& record_ids : name_table_overrides->ଚାବିଗୁଡ଼ିକ ())
      {
        ଯଦି (name_table_overrides->get (record_ids).length == 0)
          ଜାରି;
        ଯଦି (retained_name_record_ids.has (record_ids))
          ଜାରି;
        ଭର୍ତ୍ତି_name_records.push (record_ids);
      }
    }
#endif

    ଫେରନ୍ତୁ name_ମୌଳିକ->serialize (c->serializer, it,
				  std::addressof (ଏହି + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , ଭର୍ତ୍ତି_name_records
				  , name_table_overrides
#endif
				  );
  }

  ତାର୍କିକ sanitize_records (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଅଚଳ ଶୂନ୍ୟ *string_pool = (ଏହି+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, ଗଣନା_2, string_pool));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, ଗଣନା_2) &&
		  c->check_range (ଏହି, stringOffset) &&
		  sanitize_records (c));
  }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ଏହି->table = hb_sanitize_context_t ().reference_table<name> (face);
      ପ୍ରତିଜ୍ଞା (ଏହି->table.get_length () >= ଏହି->table->stringOffset);
      ଏହି->pool = (ଅଚଳ ଅକ୍ଷର *) (ଅଚଳ ଶୂନ୍ୟ *) (ଏହି->table+ଏହି->table->stringOffset);
      ଏହି->pool_len = ଏହି->table.get_length () - ଏହି->table->stringOffset;
      ଅଚଳ hb_array_t<ଅଚଳ NameRecord> all_names (ଏହି->table->nameRecordZ.arrayZ,
						    ଏହି->table->ଗଣନା_2);

      ଏହି->names.alloc_exact (all_names.length);

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = ଏହି->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      ଏହି->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଏହି->names.length; i++)
      {
	ଯଦି (ଏହି->names[i].entry_score == UNSUPPORTED ||
	    ଏହି->names[i].language == HB_LANGUAGE_INVALID)
	  ଜାରି;
	ଯଦି (i &&
	    ଏହି->names[i - 1].name_id  == ଏହି->names[i].name_id &&
	    ଏହି->names[i - 1].language == ଏହି->names[i].language)
	  ଜାରି;
	ଏହି->names[j++] = ଏହି->names[i];
      }
      ଏହି->names.resize (j);
    }
    ~accelerator_t ()
    {
      ଏହି->table.destroy ();
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    *width=nullptr) ଅଚଳ
    {
      ଅଚଳ hb_ot_name_entry_t key = {name_id, {0}, language};
      ଅଚଳ hb_ot_name_entry_t *entry = hb_bsearch (key, (ଅଚଳ hb_ot_name_entry_t *) ଏହି->names,
						    ଏହି->names.length,
						    ଆକାର (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_ଚାବି,
						    ସତ୍ୟ);

      ଯଦି (!entry)
      {
	entry = hb_bsearch (key, (ଅଚଳ hb_ot_name_entry_t *) ଏହି->names,
			    ଏହି->names.length,
			    ଆକାର (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_ଚାବି,
			    ମିଥ୍ୟା);
      }

      ଯଦି (!entry)
	ଫେରନ୍ତୁ -1;

      ଯଦି (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      ଫେରନ୍ତୁ entry->entry_index;
    }

    hb_bytes_t get_name (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx) ଅଚଳ
    {
      ଅଚଳ hb_array_t<ଅଚଳ NameRecord> all_names (table->nameRecordZ.arrayZ, table->ଗଣନା_2);
      ଅଚଳ NameRecord &record = all_names[idx];
      ଅଚଳ hb_bytes_t string_pool (pool, pool_len);
      ଫେରନ୍ତୁ string_pool.sub_array (record.offset, record.length);
    }

    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ ଅକ୍ଷର *pool;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pool_len;
    ସର୍ବସାଧାରଣ:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  ସର୍ବସାଧାରଣ:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	ଗଣନା_2;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

ସଂରଚନା name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
