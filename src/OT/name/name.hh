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

#അന്തര്ഭാവ "../../hb-open-type.hh"
#അന്തര്ഭാവ "../../hb-ot-name-language.hh"
#അന്തര്ഭാവ "../../hb-aat-layout.hh"
#അന്തര്ഭാവ "../../hb-utf.hh"


നാമാകാശ OT {

template <typename in_utf_t, typename out_utf_t>
വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned പൂർണ്ണസംഖ്യ                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned പൂർണ്ണസംഖ്യ src_len = bytes.length / വലുപ്പം (typename in_utf_t::codepoint_t);
  സ്ഥിരം typename in_utf_t::codepoint_t *src = (സ്ഥിരം typename in_utf_t::codepoint_t *) bytes.arrayZ;
  സ്ഥിരം typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  സ്ഥിരം hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  എങ്കിൽ (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    സ്ഥിരം typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    ആകുമ്പോൾ (src < src_end && dst < dst_end)
    {
      സ്ഥിരം typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      എങ്കിൽ (dst_next == dst)
	നിർത്തുക; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned പൂർണ്ണസംഖ്യ dst_len = dst - text;
  ആകുമ്പോൾ (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  തിരികെ dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

ഘടന NameRecord
{
  hb_language_t language (hb_face_t *face) സ്ഥിരം
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned പൂർണ്ണസംഖ്യ p = platformID;
    unsigned പൂർണ്ണസംഖ്യ l = languageID;

    എങ്കിൽ (p == 3)
      തിരികെ _hb_ot_name_language_for_ms_code (l);

    എങ്കിൽ (p == 1)
      തിരികെ _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    എങ്കിൽ (p == 0)
      തിരികെ face->table.ltag->get_language (l);
#endif

#endif
    തിരികെ HB_LANGUAGE_INVALID;
  }

  uint16_t score () സ്ഥിരം
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned പൂർണ്ണസംഖ്യ p = platformID;
    unsigned പൂർണ്ണസംഖ്യ e = encodingID;

    /* 32-bit. */
    എങ്കിൽ (p == 3 && e == 10) തിരികെ 0;
    എങ്കിൽ (p == 0 && e ==  6) തിരികെ 1;
    എങ്കിൽ (p == 0 && e ==  4) തിരികെ 2;

    /* 16-bit. */
    എങ്കിൽ (p == 3 && e ==  1) തിരികെ 3;
    എങ്കിൽ (p == 0 && e ==  3) തിരികെ 4;
    എങ്കിൽ (p == 0 && e ==  2) തിരികെ 5;
    എങ്കിൽ (p == 0 && e ==  1) തിരികെ 6;
    എങ്കിൽ (p == 0 && e ==  0) തിരികെ 7;

    /* Symbol. */
    എങ്കിൽ (p == 3 && e ==  0) തിരികെ 8;

    /* We treat all Mac Latin names as ASCII only. */
    എങ്കിൽ (p == 1 && e ==  0) തിരികെ 10; /* 10 is magic number :| */

    തിരികെ UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, സ്ഥിരം ശൂന്യം *base
#ifdef HB_EXPERIMENTAL_API
                    , സ്ഥിരം hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    എങ്കിൽ (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      അക്ഷരം *name_str_utf16_be = nullptr;

      എങ്കിൽ (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (അക്ഷരം *) hb_calloc (byte_len, 1);
        എങ്കിൽ (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        എങ്കിൽ (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      അല്ലെങ്കിൽ
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        എങ്കിൽ (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->എടുക്കുക_pack (), hb_serialize_context_t::Tail, 0);
      hb_free (name_str_utf16_be);
    }
    അല്ലെങ്കിൽ
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  തർക്കം isUnicode () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ p = platformID;
    unsigned പൂർണ്ണസംഖ്യ e = encodingID;

    തിരികെ (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം NameRecord *a = (സ്ഥിരം NameRecord *)pa;
    സ്ഥിരം NameRecord *b = (സ്ഥിരം NameRecord *)pb;

    എങ്കിൽ (a->platformID != b->platformID)
      തിരികെ a->platformID - b->platformID;

    എങ്കിൽ (a->encodingID != b->encodingID)
      തിരികെ a->encodingID - b->encodingID;

    എങ്കിൽ (a->languageID != b->languageID)
      തിരികെ a->languageID - b->languageID;

    എങ്കിൽ (a->nameID != b->nameID)
      തിരികെ a->nameID - b->nameID;

    എങ്കിൽ (a->length != b->length)
      തിരികെ a->length - b->length;

    തിരികെ 0;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
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
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
hb_ot_name_entry_cmp_താക്കോൽ (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb, തർക്കം exact)
{
  സ്ഥിരം hb_ot_name_entry_t *a = (സ്ഥിരം hb_ot_name_entry_t *) pa;
  സ്ഥിരം hb_ot_name_entry_t *b = (സ്ഥിരം hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  എങ്കിൽ (a->name_id != b->name_id)
    തിരികെ a->name_id - b->name_id;

  എങ്കിൽ (a->language == b->language) തിരികെ 0;
  എങ്കിൽ (!a->language) തിരികെ -1;
  എങ്കിൽ (!b->language) തിരികെ +1;

  സ്ഥിരം അക്ഷരം *astr = hb_language_to_string (a->language);
  സ്ഥിരം അക്ഷരം *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  എങ്കിൽ (!exact && c &&
      hb_language_matches (b->language, a->language))
    തിരികെ 0;

  തിരികെ c;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
_hb_ot_name_entry_cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  പൂർണ്ണസംഖ്യ v = hb_ot_name_entry_cmp_താക്കോൽ (pa, pb, സത്യം);
  എങ്കിൽ (v)
    തിരികെ v;

  സ്ഥിരം hb_ot_name_entry_t *a = (സ്ഥിരം hb_ot_name_entry_t *) pa;
  സ്ഥിരം hb_ot_name_entry_t *b = (സ്ഥിരം hb_ot_name_entry_t *) pb;

  എങ്കിൽ (a->entry_score != b->entry_score)
    തിരികെ a->entry_score - b->entry_score;

  എങ്കിൽ (a->entry_index != b->entry_index)
    തിരികെ a->entry_index - b->entry_index;

  തിരികെ 0;
}

ഘടന name
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () സ്ഥിരം
  { തിരികെ min_size + എണ്ണം * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, സ്ഥിരം NameRecord &))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it,
		  സ്ഥിരം ശൂന്യം *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , സ്ഥിരം hb_vector_t<hb_ot_name_record_ids_t>& തിരുകുക_name_records
		  , സ്ഥിരം hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (unlikely (!c->extend_min ((*ഇത്))))  return_trace (അസത്യം);

    unsigned total_എണ്ണം = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + തിരുകുക_name_records.length
#endif
        ;
    ഇത്->format = 0;
    എങ്കിൽ (!c->check_assign (ഇത്->എണ്ണം, total_എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      തിരികെ അസത്യം;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_എണ്ണം, NameRecord::static_size);
    എങ്കിൽ (unlikely (!name_records)) return_trace (അസത്യം);

    hb_array_t<NameRecord> records (name_records, total_എണ്ണം);

    വേണ്ടി (സ്ഥിരം NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    വേണ്ടി (unsigned i = 0; i < തിരുകുക_name_records.length; i++)
    {
      സ്ഥിരം hb_ot_name_record_ids_t& ids = തിരുകുക_name_records[i];
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


    എങ്കിൽ (unlikely (c->ran_out_of_room ())) return_trace (അസത്യം);

    ഇത്->stringOffset = c->length ();

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    auto *name_അഭാജ്യം = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    സ്ഥിരം hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (എണ്ണം)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (സ്ഥിരം NameRecord& namerecord) {
      തിരികെ
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (സ്ഥിരം NameRecord& namerecord) {
      എങ്കിൽ (name_table_overrides->is_empty ())
        തിരികെ സത്യം;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      എങ്കിൽ (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        തിരികെ അസത്യം;
      തിരികെ സത്യം;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    വേണ്ടി (സ്ഥിരം NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> തിരുകുക_name_records;
    എങ്കിൽ (!name_table_overrides->is_empty ())
    {
      എങ്കിൽ (unlikely (!തിരുകുക_name_records.alloc (name_table_overrides->get_population (), സത്യം)))
        തിരികെ അസത്യം;
      വേണ്ടി (സ്ഥിരം auto& record_ids : name_table_overrides->താക്കോലുകൾ ())
      {
        എങ്കിൽ (name_table_overrides->get (record_ids).length == 0)
          തുടരുക;
        എങ്കിൽ (retained_name_record_ids.has (record_ids))
          തുടരുക;
        തിരുകുക_name_records.push (record_ids);
      }
    }
#endif

    തിരികെ name_അഭാജ്യം->serialize (c->serializer, it,
				  std::addressof (ഇത് + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , തിരുകുക_name_records
				  , name_table_overrides
#endif
				  );
  }

  തർക്കം sanitize_records (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    സ്ഥിരം ശൂന്യം *string_pool = (ഇത്+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, എണ്ണം, string_pool));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, എണ്ണം) &&
		  c->check_range (ഇത്, stringOffset) &&
		  sanitize_records (c));
  }

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ഇത്->table = hb_sanitize_context_t ().reference_table<name> (face);
      പ്രതിജ്ഞാ (ഇത്->table.get_length () >= ഇത്->table->stringOffset);
      ഇത്->pool = (സ്ഥിരം അക്ഷരം *) (സ്ഥിരം ശൂന്യം *) (ഇത്->table+ഇത്->table->stringOffset);
      ഇത്->pool_len = ഇത്->table.get_length () - ഇത്->table->stringOffset;
      സ്ഥിരം hb_array_t<സ്ഥിരം NameRecord> all_names (ഇത്->table->nameRecordZ.arrayZ,
						    ഇത്->table->എണ്ണം);

      ഇത്->names.alloc_exact (all_names.length);

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = ഇത്->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      ഇത്->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned പൂർണ്ണസംഖ്യ j = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ഇത്->names.length; i++)
      {
	എങ്കിൽ (ഇത്->names[i].entry_score == UNSUPPORTED ||
	    ഇത്->names[i].language == HB_LANGUAGE_INVALID)
	  തുടരുക;
	എങ്കിൽ (i &&
	    ഇത്->names[i - 1].name_id  == ഇത്->names[i].name_id &&
	    ഇത്->names[i - 1].language == ഇത്->names[i].language)
	  തുടരുക;
	ഇത്->names[j++] = ഇത്->names[i];
      }
      ഇത്->names.resize (j);
    }
    ~accelerator_t ()
    {
      ഇത്->table.destroy ();
    }

    പൂർണ്ണസംഖ്യ get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned പൂർണ്ണസംഖ്യ    *width=nullptr) സ്ഥിരം
    {
      സ്ഥിരം hb_ot_name_entry_t key = {name_id, {0}, language};
      സ്ഥിരം hb_ot_name_entry_t *entry = hb_bsearch (key, (സ്ഥിരം hb_ot_name_entry_t *) ഇത്->names,
						    ഇത്->names.length,
						    വലുപ്പം (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_താക്കോൽ,
						    സത്യം);

      എങ്കിൽ (!entry)
      {
	entry = hb_bsearch (key, (സ്ഥിരം hb_ot_name_entry_t *) ഇത്->names,
			    ഇത്->names.length,
			    വലുപ്പം (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_താക്കോൽ,
			    അസത്യം);
      }

      എങ്കിൽ (!entry)
	തിരികെ -1;

      എങ്കിൽ (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      തിരികെ entry->entry_index;
    }

    hb_bytes_t get_name (unsigned പൂർണ്ണസംഖ്യ idx) സ്ഥിരം
    {
      സ്ഥിരം hb_array_t<സ്ഥിരം NameRecord> all_names (table->nameRecordZ.arrayZ, table->എണ്ണം);
      സ്ഥിരം NameRecord &record = all_names[idx];
      സ്ഥിരം hb_bytes_t string_pool (pool, pool_len);
      തിരികെ string_pool.sub_array (record.offset, record.length);
    }

    സ്വകാര്യ:
    സ്ഥിരം അക്ഷരം *pool;
    unsigned പൂർണ്ണസംഖ്യ pool_len;
    പൊതു:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  പൊതു:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	എണ്ണം;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  പൊതു:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

ഘടന name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
