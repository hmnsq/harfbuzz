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

#अगर_अघोषित OT_NAME_NAME_HH
#घोषणा OT_NAME_NAME_HH

#समावेश "../../hb-open-type.hh"
#समावेश "../../hb-ot-name-language.hh"
#समावेश "../../hb-aat-layout.hh"
#समावेश "../../hb-utf.hh"


namespace OT {

ढाँचा <typename in_utf_t, typename out_utf_t>
अंतरभूत अचिन्हित पूर्णांक
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			अचिन्हित पूर्णांक                    *text_आकार /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  अचिन्हित पूर्णांक src_len = bytes.लंबाई / माप (typename in_utf_t::codepoint_t);
  स्थिर typename in_utf_t::codepoint_t *src = (स्थिर typename in_utf_t::codepoint_t *) bytes.arrayZ;
  स्थिर typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  स्थिर hb_codepoint_t replacement = HB_बफर_REPLACEMENT_CODEPOINT_DEFAULT;

  अगर (text_आकार && *text_आकार)
  {
    (*text_आकार)--; /* Save room for NUL-termination. */
    स्थिर typename out_utf_t::codepoint_t *dst_end = text + *text_आकार;

    जबतक (src < src_end && dst < dst_end)
    {
      स्थिर typename in_utf_t::codepoint_t *src_अग्र = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_अग्र = out_utf_t::encode (dst, dst_end, unicode);
      अगर (dst_अग्र == dst)
	अवरोध; /* Out-of-room. */

      dst = dst_अग्र;
      src = src_अग्र;
    }

    *text_आकार = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  अचिन्हित पूर्णांक dst_len = dst - text;
  जबतक (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  वापस dst_len;
}

#घोषणा entry_score var.u16[0]
#घोषणा entry_सूचकांक var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#घोषणा HB_OT_TAG_name HB_TAG('n','a','m','e')

#घोषणा UNSUPPORTED	42

काष्ठा NameRecord
{
  hb_language_t language (hb_face_t *face) स्थिर
  {
#अगर_अघोषित HB_NO_OT_NAME_LANGUAGE
    अचिन्हित पूर्णांक p = platformID;
    अचिन्हित पूर्णांक l = languageID;

    अगर (p == 3)
      वापस _hb_ot_name_language_for_ms_code (l);

    अगर (p == 1)
      वापस _hb_ot_name_language_for_mac_code (l);

#अगर_अघोषित HB_NO_OT_NAME_LANGUAGE_AAT
    अगर (p == 0)
      वापस face->table.ltag->get_language (l);
#पूर्ण_अगर

#पूर्ण_अगर
    वापस HB_LANGUAGE_INVALID;
  }

  uint16_t score () स्थिर
  {
    /* Same order as in cmap::find_best_subtable(). */
    अचिन्हित पूर्णांक p = platformID;
    अचिन्हित पूर्णांक e = encodingID;

    /* 32-bit. */
    अगर (p == 3 && e == 10) वापस 0;
    अगर (p == 0 && e ==  6) वापस 1;
    अगर (p == 0 && e ==  4) वापस 2;

    /* 16-bit. */
    अगर (p == 3 && e ==  1) वापस 3;
    अगर (p == 0 && e ==  3) वापस 4;
    अगर (p == 0 && e ==  2) वापस 5;
    अगर (p == 0 && e ==  1) वापस 6;
    अगर (p == 0 && e ==  0) वापस 7;

    /* Symbol. */
    अगर (p == 3 && e ==  0) वापस 8;

    /* We treat all Mac Latin names as ASCII only. */
    अगर (p == 1 && e ==  0) वापस 10; /* 10 is magic number :| */

    वापस UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, स्थिर व्योम *base
#अगर_घोषित HB_EXPERIMENTAL_API
                    , स्थिर hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#पूर्ण_अगर
		    ) स्थिर
  {
    TRACE_SERIALIZE (यह);
    HB_UNUSED स्वतः snap = c->snapshot ();
    स्वतः *out = c->embed (यह);
    अगर (unlikely (!out)) return_trace (nullptr);
#अगर_घोषित HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    अगर (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      अक्षर *name_str_utf16_be = nullptr;

      अगर (platformID != 1)
      {
        अचिन्हित text_आकार = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_आकार++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        अचिन्हित byte_len = text_आकार * hb_utf16_be_t::codepoint_t::static_आकार;
        name_str_utf16_be = (अक्षर *) hb_calloc (byte_len, 1);
        अगर (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_आकार,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        अचिन्हित encoded_byte_len = text_आकार * hb_utf16_be_t::codepoint_t::static_आकार;
        अगर (!encoded_byte_len || !c->check_assign (out->लंबाई, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      अन्यथा
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        अगर (!c->check_assign (out->लंबाई, encoded_bytes.लंबाई, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          return_trace (nullptr);
        }
      }

      out->offset = 0;
      c->push ();
      encoded_bytes.copy (c);
      c->add_link (out->offset, c->निकालो_pack (), hb_serialize_context_t::पुच्छ_2, 0);
      hb_free (name_str_utf16_be);
    }
    अन्यथा
#पूर्ण_अगर
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::पुच्छ_2, लंबाई);
    }
    return_trace (out);
  }

  bool isUnicode () स्थिर
  {
    अचिन्हित पूर्णांक p = platformID;
    अचिन्हित पूर्णांक e = encodingID;

    वापस (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर NameRecord *a = (स्थिर NameRecord *)pa;
    स्थिर NameRecord *b = (स्थिर NameRecord *)pb;

    अगर (a->platformID != b->platformID)
      वापस a->platformID - b->platformID;

    अगर (a->encodingID != b->encodingID)
      वापस a->encodingID - b->encodingID;

    अगर (a->languageID != b->languageID)
      वापस a->languageID - b->languageID;

    अगर (a->nameID != b->nameID)
      वापस a->nameID - b->nameID;

    अगर (a->लंबाई != b->लंबाई)
      वापस a->लंबाई - b->लंबाई;

    वापस 0;
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  offset.sanitize (c, base, लंबाई));
  }

  HBUINT16	platformID;	/* Platform ID. */
  HBUINT16	encodingID;	/* Platform-specific encoding ID. */
  HBUINT16	languageID;	/* Language ID. */
  HBUINT16	nameID;		/* Name ID. */
  HBUINT16	लंबाई;		/* String length (in bytes). */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		offset;		/* String offset from start of storage area (in bytes). */
  खुला:
  DEFINE_आकार_STATIC (12);
};

अटल पूर्णांक
hb_ot_name_entry_cmp_कुंजी (स्थिर व्योम *pa, स्थिर व्योम *pb, bool exact)
{
  स्थिर hb_ot_name_entry_t *a = (स्थिर hb_ot_name_entry_t *) pa;
  स्थिर hb_ot_name_entry_t *b = (स्थिर hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  अगर (a->name_id != b->name_id)
    वापस a->name_id - b->name_id;

  अगर (a->language == b->language) वापस 0;
  अगर (!a->language) वापस -1;
  अगर (!b->language) वापस +1;

  स्थिर अक्षर *astr = hb_language_to_string (a->language);
  स्थिर अक्षर *bstr = hb_language_to_string (b->language);

  चिन्हित c = म_भेद (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  अगर (!exact && c &&
      hb_language_matches (b->language, a->language))
    वापस 0;

  वापस c;
}

अटल पूर्णांक
_hb_ot_name_entry_cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  पूर्णांक v = hb_ot_name_entry_cmp_कुंजी (pa, pb, true);
  अगर (v)
    वापस v;

  स्थिर hb_ot_name_entry_t *a = (स्थिर hb_ot_name_entry_t *) pa;
  स्थिर hb_ot_name_entry_t *b = (स्थिर hb_ot_name_entry_t *) pb;

  अगर (a->entry_score != b->entry_score)
    वापस a->entry_score - b->entry_score;

  अगर (a->entry_सूचकांक != b->entry_सूचकांक)
    वापस a->entry_सूचकांक - b->entry_सूचकांक;

  वापस 0;
}

काष्ठा name
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  माप_प्रकार get_आकार () स्थिर
  { वापस min_आकार + गिनती * nameRecordZ.item_आकार; }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, स्थिर NameRecord &))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
		  स्थिर व्योम *src_string_pool
#अगर_घोषित HB_EXPERIMENTAL_API
                  , स्थिर hb_vector_t<hb_ot_name_record_ids_t>& जोड़ो_name_records
		  , स्थिर hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#पूर्ण_अगर
		  )
  {
    TRACE_SERIALIZE (यह);

    अगर (unlikely (!c->extend_min ((*यह))))  return_trace (false);

    अचिन्हित योग_गिनती = it.len ()
#अगर_घोषित HB_EXPERIMENTAL_API
        + जोड़ो_name_records.लंबाई
#पूर्ण_अगर
        ;
    यह->format = 0;
    अगर (!c->check_assign (यह->गिनती, योग_गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      वापस false;

    NameRecord *name_records = (NameRecord *) hb_calloc (योग_गिनती, NameRecord::static_आकार);
    अगर (unlikely (!name_records)) return_trace (false);

    hb_array_t<NameRecord> records (name_records, योग_गिनती);

    क्रम (स्थिर NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_आकार);
      name_records++;
    }

#अगर_घोषित HB_EXPERIMENTAL_API
    क्रम (अचिन्हित i = 0; i < जोड़ो_name_records.लंबाई; i++)
    {
      स्थिर hb_ot_name_record_ids_t& ids = जोड़ो_name_records[i];
      NameRecord record;
      record.platformID = ids.platform_id;
      record.encodingID = ids.encoding_id;
      record.languageID = ids.language_id;
      record.nameID = ids.name_id;
      record.लंबाई = 0; // handled in NameRecord copy()
      record.offset = 0;
      hb_memcpy (name_records, &record, NameRecord::static_आकार);
      name_records++;
    }
#पूर्ण_अगर

    records.क्विक ();

    c->copy_all (records,
		 src_string_pool
#अगर_घोषित HB_EXPERIMENTAL_API
		 , name_table_overrides
#पूर्ण_अगर
		 );
    hb_free (records.arrayZ);


    अगर (unlikely (c->ran_out_of_room ())) return_trace (false);

    यह->stringOffset = c->लंबाई ();

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    स्वतः *name_अभाज्य = c->serializer->start_embed<name> ();

#अगर_घोषित HB_EXPERIMENTAL_API
    स्थिर hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#पूर्ण_अगर

    स्वतः it =
    + nameRecordZ.as_array (गिनती)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (स्थिर NameRecord& namerecord) {
      वापस
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#अगर_घोषित HB_EXPERIMENTAL_API
    | hb_filter ([&] (स्थिर NameRecord& namerecord) {
      अगर (name_table_overrides->is_empty ())
        वापस true;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      अगर (name_table_overrides->has (rec_ids, &p) &&
          (*p).लंबाई == 0)
        वापस false;
      वापस true;
    })
#पूर्ण_अगर
    ;

#अगर_घोषित HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, अचिन्हित> retained_name_record_ids;
    क्रम (स्थिर NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> जोड़ो_name_records;
    अगर (!name_table_overrides->is_empty ())
    {
      अगर (unlikely (!जोड़ो_name_records.alloc (name_table_overrides->get_population (), true)))
        वापस false;
      क्रम (स्थिर स्वतः& record_ids : name_table_overrides->कुंजियाँ ())
      {
        अगर (name_table_overrides->लो (record_ids).लंबाई == 0)
          जारी;
        अगर (retained_name_record_ids.has (record_ids))
          जारी;
        जोड़ो_name_records.push (record_ids);
      }
    }
#पूर्ण_अगर

    वापस name_अभाज्य->serialize (c->serializer, it,
				  std::addressof (यह + stringOffset)
#अगर_घोषित HB_EXPERIMENTAL_API
				  , जोड़ो_name_records
				  , name_table_overrides
#पूर्ण_अगर
				  );
  }

  bool sanitize_records (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    स्थिर व्योम *string_pool = (यह+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, गिनती, string_pool));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, गिनती) &&
		  c->check_range (यह, stringOffset) &&
		  sanitize_records (c));
  }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      यह->table = hb_sanitize_context_t ().reference_table<name> (face);
      निश्चित (यह->table.get_लंबाई () >= यह->table->stringOffset);
      यह->pool = (स्थिर अक्षर *) (स्थिर व्योम *) (यह->table+यह->table->stringOffset);
      यह->pool_len = यह->table.get_लंबाई () - यह->table->stringOffset;
      स्थिर hb_array_t<स्थिर NameRecord> all_names (यह->table->nameRecordZ.arrayZ,
						    यह->table->गिनती);

      यह->names.alloc_exact (all_names.लंबाई);

      क्रम (अचिन्हित पूर्णांक i = 0; i < all_names.लंबाई; i++)
      {
	hb_ot_name_entry_t *entry = यह->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_सूचकांक = i;
      }

      यह->names.क्विक (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      अचिन्हित पूर्णांक j = 0;
      क्रम (अचिन्हित पूर्णांक i = 0; i < यह->names.लंबाई; i++)
      {
	अगर (यह->names[i].entry_score == UNSUPPORTED ||
	    यह->names[i].language == HB_LANGUAGE_INVALID)
	  जारी;
	अगर (i &&
	    यह->names[i - 1].name_id  == यह->names[i].name_id &&
	    यह->names[i - 1].language == यह->names[i].language)
	  जारी;
	यह->names[j++] = यह->names[i];
      }
      यह->names.resize (j);
    }
    ~accelerator_t ()
    {
      यह->table.destroy ();
    }

    पूर्णांक get_सूचकांक (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   अचिन्हित पूर्णांक    *width=nullptr) स्थिर
    {
      स्थिर hb_ot_name_entry_t key = {name_id, {0}, language};
      स्थिर hb_ot_name_entry_t *entry = hb_bsearch (key, (स्थिर hb_ot_name_entry_t *) यह->names,
						    यह->names.लंबाई,
						    माप (hb_ot_name_entry_t),
						    hb_ot_name_entry_cmp_कुंजी,
						    true);

      अगर (!entry)
      {
	entry = hb_bsearch (key, (स्थिर hb_ot_name_entry_t *) यह->names,
			    यह->names.लंबाई,
			    माप (hb_ot_name_entry_t),
			    hb_ot_name_entry_cmp_कुंजी,
			    false);
      }

      अगर (!entry)
	वापस -1;

      अगर (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      वापस entry->entry_सूचकांक;
    }

    hb_bytes_t get_name (अचिन्हित पूर्णांक idx) स्थिर
    {
      स्थिर hb_array_t<स्थिर NameRecord> all_names (table->nameRecordZ.arrayZ, table->गिनती);
      स्थिर NameRecord &record = all_names[idx];
      स्थिर hb_bytes_t string_pool (pool, pool_len);
      वापस string_pool.sub_array (record.offset, record.लंबाई);
    }

    निजी:
    स्थिर अक्षर *pool;
    अचिन्हित पूर्णांक pool_len;
    खुला:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  खुला:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	गिनती;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  खुला:
  DEFINE_आकार_ARRAY (6, nameRecordZ);
};

#अघोषित entry_सूचकांक
#अघोषित entry_score

काष्ठा name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#पूर्ण_अगर /* OT_NAME_NAME_HH */
