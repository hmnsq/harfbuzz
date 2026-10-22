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

#અન્તર્ભાવ "../../hb-open-type.hh"
#અન્તર્ભાવ "../../hb-ot-name-language.hh"
#અન્તર્ભાવ "../../hb-aat-layout.hh"
#અન્તર્ભાવ "../../hb-utf.hh"


નામાકાશ OT {

template <typename in_utf_t, typename out_utf_t>
અંતઃરેખ unsigned પૂર્ણાંક
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned પૂર્ણાંક                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned પૂર્ણાંક src_len = bytes.length / કદ (typename in_utf_t::codepoint_t);
  અચલ typename in_utf_t::codepoint_t *src = (અચલ typename in_utf_t::codepoint_t *) bytes.arrayZ;
  અચલ typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  અચલ hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  જો (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    અચલ typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    જ્યારે (src < src_end && dst < dst_end)
    {
      અચલ typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      જો (dst_next == dst)
	તોડો; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned પૂર્ણાંક dst_len = dst - text;
  જ્યારે (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  પાછા dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

સંરચના NameRecord
{
  hb_language_t language (hb_face_t *face) અચલ
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned પૂર્ણાંક p = platformID;
    unsigned પૂર્ણાંક l = languageID;

    જો (p == 3)
      પાછા _hb_ot_name_language_for_ms_code (l);

    જો (p == 1)
      પાછા _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    જો (p == 0)
      પાછા face->table.ltag->get_language (l);
#endif

#endif
    પાછા HB_LANGUAGE_INVALID;
  }

  uint16_t score () અચલ
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned પૂર્ણાંક p = platformID;
    unsigned પૂર્ણાંક e = encodingID;

    /* 32-bit. */
    જો (p == 3 && e == 10) પાછા 0;
    જો (p == 0 && e ==  6) પાછા 1;
    જો (p == 0 && e ==  4) પાછા 2;

    /* 16-bit. */
    જો (p == 3 && e ==  1) પાછા 3;
    જો (p == 0 && e ==  3) પાછા 4;
    જો (p == 0 && e ==  2) પાછા 5;
    જો (p == 0 && e ==  1) પાછા 6;
    જો (p == 0 && e ==  0) પાછા 7;

    /* Symbol. */
    જો (p == 3 && e ==  0) પાછા 8;

    /* We treat all Mac Latin names as ASCII only. */
    જો (p == 1 && e ==  0) પાછા 10; /* 10 is magic number :| */

    પાછા UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, અચલ શૂન્ય *base
#ifdef HB_EXPERIMENTAL_API
                    , અચલ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) અચલ
  {
    TRACE_SERIALIZE (આ);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (આ);
    જો (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    જો (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      અક્ષર *name_str_utf16_be = nullptr;

      જો (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (અક્ષર *) hb_calloc (byte_len, 1);
        જો (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        જો (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      નહીંતર
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        જો (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->કાઢો_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    નહીંતર
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  તાર્કિક isUnicode () અચલ
  {
    unsigned પૂર્ણાંક p = platformID;
    unsigned પૂર્ણાંક e = encodingID;

    પાછા (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    અચલ NameRecord *a = (અચલ NameRecord *)pa;
    અચલ NameRecord *b = (અચલ NameRecord *)pb;

    જો (a->platformID != b->platformID)
      પાછા a->platformID - b->platformID;

    જો (a->encodingID != b->encodingID)
      પાછા a->encodingID - b->encodingID;

    જો (a->languageID != b->languageID)
      પાછા a->languageID - b->languageID;

    જો (a->nameID != b->nameID)
      પાછા a->nameID - b->nameID;

    જો (a->length != b->length)
      પાછા a->length - b->length;

    પાછા 0;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
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
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

સ્થિર પૂર્ણાંક
hb_ot_name_entry_cmp_ચાવી (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb, તાર્કિક exact)
{
  અચલ hb_ot_name_entry_t *a = (અચલ hb_ot_name_entry_t *) pa;
  અચલ hb_ot_name_entry_t *b = (અચલ hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  જો (a->name_id != b->name_id)
    પાછા a->name_id - b->name_id;

  જો (a->language == b->language) પાછા 0;
  જો (!a->language) પાછા -1;
  જો (!b->language) પાછા +1;

  અચલ અક્ષર *astr = hb_language_to_string (a->language);
  અચલ અક્ષર *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  જો (!exact && c &&
      hb_language_matches (b->language, a->language))
    પાછા 0;

  પાછા c;
}

સ્થિર પૂર્ણાંક
_hb_ot_name_entry_cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  પૂર્ણાંક v = hb_ot_name_entry_cmp_ચાવી (pa, pb, સાચું);
  જો (v)
    પાછા v;

  અચલ hb_ot_name_entry_t *a = (અચલ hb_ot_name_entry_t *) pa;
  અચલ hb_ot_name_entry_t *b = (અચલ hb_ot_name_entry_t *) pb;

  જો (a->entry_score != b->entry_score)
    પાછા a->entry_score - b->entry_score;

  જો (a->entry_index != b->entry_index)
    પાછા a->entry_index - b->entry_index;

  પાછા 0;
}

સંરચના name
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () અચલ
  { પાછા min_size + ગણતરી * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, અચલ NameRecord &))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it,
		  અચલ શૂન્ય *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , અચલ hb_vector_t<hb_ot_name_record_ids_t>& ઉમેરો_name_records
		  , અચલ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (આ);

    જો (unlikely (!c->extend_min ((*આ))))  return_trace (ખોટું);

    unsigned total_ગણતરી = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + ઉમેરો_name_records.length
#endif
        ;
    આ->format = 0;
    જો (!c->check_assign (આ->ગણતરી, total_ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      પાછા ખોટું;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_ગણતરી, NameRecord::static_size);
    જો (unlikely (!name_records)) return_trace (ખોટું);

    hb_array_t<NameRecord> records (name_records, total_ગણતરી);

    માટે (અચલ NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    માટે (unsigned i = 0; i < ઉમેરો_name_records.length; i++)
    {
      અચલ hb_ot_name_record_ids_t& ids = ઉમેરો_name_records[i];
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


    જો (unlikely (c->ran_out_of_room ())) return_trace (ખોટું);

    આ->stringOffset = c->length ();

    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    auto *name_અવિભાજ્ય = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    અચલ hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (ગણતરી)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (અચલ NameRecord& namerecord) {
      પાછા
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (અચલ NameRecord& namerecord) {
      જો (name_table_overrides->is_empty ())
        પાછા સાચું;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      જો (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        પાછા ખોટું;
      પાછા સાચું;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    માટે (અચલ NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> ઉમેરો_name_records;
    જો (!name_table_overrides->is_empty ())
    {
      જો (unlikely (!ઉમેરો_name_records.alloc (name_table_overrides->get_population (), સાચું)))
        પાછા ખોટું;
      માટે (અચલ auto& record_ids : name_table_overrides->ચાવીઓ ())
      {
        જો (name_table_overrides->get (record_ids).length == 0)
          ચાલુ;
        જો (retained_name_record_ids.has (record_ids))
          ચાલુ;
        ઉમેરો_name_records.push (record_ids);
      }
    }
#endif

    પાછા name_અવિભાજ્ય->serialize (c->serializer, it,
				  std::addressof (આ + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , ઉમેરો_name_records
				  , name_table_overrides
#endif
				  );
  }

  તાર્કિક sanitize_records (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    અચલ શૂન્ય *string_pool = (આ+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, ગણતરી, string_pool));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, ગણતરી) &&
		  c->check_range (આ, stringOffset) &&
		  sanitize_records (c));
  }

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      આ->table = hb_sanitize_context_t ().reference_table<name> (face);
      પ્રતિજ્ઞા (આ->table.get_length () >= આ->table->stringOffset);
      આ->pool = (અચલ અક્ષર *) (અચલ શૂન્ય *) (આ->table+આ->table->stringOffset);
      આ->pool_len = આ->table.get_length () - આ->table->stringOffset;
      અચલ hb_array_t<અચલ NameRecord> all_names (આ->table->nameRecordZ.arrayZ,
						    આ->table->ગણતરી);

      આ->names.alloc_exact (all_names.length);

      માટે (unsigned પૂર્ણાંક i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = આ->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      આ->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned પૂર્ણાંક j = 0;
      માટે (unsigned પૂર્ણાંક i = 0; i < આ->names.length; i++)
      {
	જો (આ->names[i].entry_score == UNSUPPORTED ||
	    આ->names[i].language == HB_LANGUAGE_INVALID)
	  ચાલુ;
	જો (i &&
	    આ->names[i - 1].name_id  == આ->names[i].name_id &&
	    આ->names[i - 1].language == આ->names[i].language)
	  ચાલુ;
	આ->names[j++] = આ->names[i];
      }
      આ->names.resize (j);
    }
    ~accelerator_t ()
    {
      આ->table.destroy ();
    }

    પૂર્ણાંક get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned પૂર્ણાંક    *width=nullptr) અચલ
    {
      અચલ hb_ot_name_entry_t key = {name_id, {0}, language};
      અચલ hb_ot_name_entry_t *entry = hb_bsearch (key, (અચલ hb_ot_name_entry_t *) આ->names,
						    આ->names.length,
						    કદ (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_ચાવી,
						    સાચું);

      જો (!entry)
      {
	entry = hb_bsearch (key, (અચલ hb_ot_name_entry_t *) આ->names,
			    આ->names.length,
			    કદ (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_ચાવી,
			    ખોટું);
      }

      જો (!entry)
	પાછા -1;

      જો (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      પાછા entry->entry_index;
    }

    hb_bytes_t get_name (unsigned પૂર્ણાંક idx) અચલ
    {
      અચલ hb_array_t<અચલ NameRecord> all_names (table->nameRecordZ.arrayZ, table->ગણતરી);
      અચલ NameRecord &record = all_names[idx];
      અચલ hb_bytes_t string_pool (pool, pool_len);
      પાછા string_pool.sub_array (record.offset, record.length);
    }

    ખાનગી:
    અચલ અક્ષર *pool;
    unsigned પૂર્ણાંક pool_len;
    જાહેર:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  જાહેર:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	ગણતરી;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  જાહેર:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

સંરચના name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
