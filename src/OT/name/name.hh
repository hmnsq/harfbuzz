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

#include "../../hb-open-type.hh"
#include "../../hb-ot-name-language.hh"
#include "../../hb-aat-layout.hh"
#include "../../hb-utf.hh"


namespace OT {

template <typename in_utf_t, typename out_utf_t>
anta_hstha unsigned ကိန်းပြည့်
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned ကိန်းပြည့်                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned ကိန်းပြည့် src_len = bytes.length / parimaa_na (typename in_utf_t::codepoint_t);
  const typename in_utf_t::codepoint_t *src = (const typename in_utf_t::codepoint_t *) bytes.arrayZ;
  const typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  const hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  အကယ်၍ (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    const typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    နေစဉ် (src < src_end && dst < dst_end)
    {
      const typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      အကယ်၍ (dst_next == dst)
	bha_nga; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned ကိန်းပြည့် dst_len = dst - text;
  နေစဉ် (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  ပြန်ပေး dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

sa_nracanaa NameRecord
{
  hb_language_t language (hb_face_t *face) const
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned ကိန်းပြည့် p = platformID;
    unsigned ကိန်းပြည့် l = languageID;

    အကယ်၍ (p == 3)
      ပြန်ပေး _hb_ot_name_language_for_ms_code (l);

    အကယ်၍ (p == 1)
      ပြန်ပေး _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    အကယ်၍ (p == 0)
      ပြန်ပေး face->table.ltag->get_language (l);
#endif

#endif
    ပြန်ပေး HB_LANGUAGE_INVALID;
  }

  uint16_t score () const
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned ကိန်းပြည့် p = platformID;
    unsigned ကိန်းပြည့် e = encodingID;

    /* 32-bit. */
    အကယ်၍ (p == 3 && e == 10) ပြန်ပေး 0;
    အကယ်၍ (p == 0 && e ==  6) ပြန်ပေး 1;
    အကယ်၍ (p == 0 && e ==  4) ပြန်ပေး 2;

    /* 16-bit. */
    အကယ်၍ (p == 3 && e ==  1) ပြန်ပေး 3;
    အကယ်၍ (p == 0 && e ==  3) ပြန်ပေး 4;
    အကယ်၍ (p == 0 && e ==  2) ပြန်ပေး 5;
    အကယ်၍ (p == 0 && e ==  1) ပြန်ပေး 6;
    အကယ်၍ (p == 0 && e ==  0) ပြန်ပေး 7;

    /* Symbol. */
    အကယ်၍ (p == 3 && e ==  0) ပြန်ပေး 8;

    /* We treat all Mac Latin names as ASCII only. */
    အကယ်၍ (p == 1 && e ==  0) ပြန်ပေး 10; /* 10 is magic number :| */

    ပြန်ပေး UNSUPPORTED;
  }

  NameRecord* copy (hb_serialize_context_t *c, const void *base
#ifdef HB_EXPERIMENTAL_API
                    , const hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		    ) const
  {
    TRACE_SERIALIZE (this);
    HB_UNUSED auto snap = c->snapshot ();
    auto *out = c->embed (this);
    အကယ်၍ (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    အကယ်၍ (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      var_na *name_str_utf16_be = nullptr;

      အကယ်၍ (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (var_na *) hb_calloc (byte_len, 1);
        အကယ်၍ (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        အကယ်၍ (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      မဟုတ်ပါက
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        အကယ်၍ (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
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
    မဟုတ်ပါက
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  bool isUnicode () const
  {
    unsigned ကိန်းပြည့် p = platformID;
    unsigned ကိန်းပြည့် e = encodingID;

    ပြန်ပေး (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  static ကိန်းပြည့် cmp (const void *pa, const void *pb)
  {
    const NameRecord *a = (const NameRecord *)pa;
    const NameRecord *b = (const NameRecord *)pb;

    အကယ်၍ (a->platformID != b->platformID)
      ပြန်ပေး a->platformID - b->platformID;

    အကယ်၍ (a->encodingID != b->encodingID)
      ပြန်ပေး a->encodingID - b->encodingID;

    အကယ်၍ (a->languageID != b->languageID)
      ပြန်ပေး a->languageID - b->languageID;

    အကယ်၍ (a->nameID != b->nameID)
      ပြန်ပေး a->nameID - b->nameID;

    အကယ်၍ (a->length != b->length)
      ပြန်ပေး a->length - b->length;

    ပြန်ပေး 0;
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
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
  public:
  DEFINE_SIZE_STATIC (12);
};

static ကိန်းပြည့်
_hb_ot_name_entry_cmp_key (const void *pa, const void *pb, bool exact)
{
  const hb_ot_name_entry_t *a = (const hb_ot_name_entry_t *) pa;
  const hb_ot_name_entry_t *b = (const hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  အကယ်၍ (a->name_id != b->name_id)
    ပြန်ပေး a->name_id - b->name_id;

  အကယ်၍ (a->language == b->language) ပြန်ပေး 0;
  အကယ်၍ (!a->language) ပြန်ပေး -1;
  အကယ်၍ (!b->language) ပြန်ပေး +1;

  const var_na *astr = hb_language_to_string (a->language);
  const var_na *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  အကယ်၍ (!exact && c &&
      hb_language_matches (b->language, a->language))
    ပြန်ပေး 0;

  ပြန်ပေး c;
}

static ကိန်းပြည့်
_hb_ot_name_entry_cmp (const void *pa, const void *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  ကိန်းပြည့် v = _hb_ot_name_entry_cmp_key (pa, pb, true);
  အကယ်၍ (v)
    ပြန်ပေး v;

  const hb_ot_name_entry_t *a = (const hb_ot_name_entry_t *) pa;
  const hb_ot_name_entry_t *b = (const hb_ot_name_entry_t *) pb;

  အကယ်၍ (a->entry_score != b->entry_score)
    ပြန်ပေး a->entry_score - b->entry_score;

  အကယ်၍ (a->entry_index != b->entry_index)
    ပြန်ပေး a->entry_index - b->entry_index;

  ပြန်ပေး 0;
}

sa_nracanaa name
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () const
  { ပြန်ပေး min_size + count * nameRecordZ.item_size; }

  template <typename Iterator,
	    hb_requires (hb_is_source_of (Iterator, const NameRecord &))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
		  const void *src_string_pool
#ifdef HB_EXPERIMENTAL_API
                  , const hb_vector_t<hb_ot_name_record_ids_t>& insert_name_records
		  , const hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides
#endif
		  )
  {
    TRACE_SERIALIZE (this);

    အကယ်၍ (unlikely (!c->extend_min ((*this))))  return_trace (false);

    unsigned total_count = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + insert_name_records.length
#endif
        ;
    this->format = 0;
    အကယ်၍ (!c->check_assign (this->count, total_count, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      ပြန်ပေး false;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_count, NameRecord::static_size);
    အကယ်၍ (unlikely (!name_records)) return_trace (false);

    hb_array_t<NameRecord> records (name_records, total_count);

    အတွက် (const NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    အတွက် (unsigned i = 0; i < insert_name_records.length; i++)
    {
      const hb_ot_name_record_ids_t& ids = insert_name_records[i];
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


    အကယ်၍ (unlikely (c->ran_out_of_room ())) return_trace (false);

    this->stringOffset = c->length ();

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) const
  {
    auto *name_prime = c->serializer->start_embed<name> ();

#ifdef HB_EXPERIMENTAL_API
    const hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> *name_table_overrides =
        &c->plan->name_table_overrides;
#endif

    auto it =
    + nameRecordZ.as_array (count)
    | hb_filter (c->plan->name_ids, &NameRecord::nameID)
    | hb_filter (c->plan->name_languages, &NameRecord::languageID)
    | hb_filter ([&] (const NameRecord& namerecord) {
      ပြန်ပေး
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (const NameRecord& namerecord) {
      အကယ်၍ (name_table_overrides->is_empty ())
        ပြန်ပေး true;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      အကယ်၍ (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        ပြန်ပေး false;
      ပြန်ပေး true;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    အတွက် (const NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> insert_name_records;
    အကယ်၍ (!name_table_overrides->is_empty ())
    {
      အကယ်၍ (unlikely (!insert_name_records.alloc (name_table_overrides->get_population (), true)))
        ပြန်ပေး false;
      အတွက် (const auto& record_ids : name_table_overrides->keys ())
      {
        အကယ်၍ (name_table_overrides->get (record_ids).length == 0)
          continue;
        အကယ်၍ (retained_name_record_ids.has (record_ids))
          continue;
        insert_name_records.push (record_ids);
      }
    }
#endif

    ပြန်ပေး name_prime->serialize (c->serializer, it,
				  std::addressof (this + stringOffset)
#ifdef HB_EXPERIMENTAL_API
				  , insert_name_records
				  , name_table_overrides
#endif
				  );
  }

  bool sanitize_records (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    const void *string_pool = (this+stringOffset).arrayZ;
    return_trace (nameRecordZ.sanitize (c, count, string_pool));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  likely (format == 0 || format == 1) &&
		  c->check_array (nameRecordZ.arrayZ, count) &&
		  c->check_range (this, stringOffset) &&
		  sanitize_records (c));
  }

  sa_nracanaa accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      this->table = hb_sanitize_context_t ().reference_table<name> (face);
      assert (this->table.get_length () >= this->table->stringOffset);
      this->pool = (const var_na *) (const void *) (this->table+this->table->stringOffset);
      this->pool_len = this->table.get_length () - this->table->stringOffset;
      const hb_array_t<const NameRecord> all_names (this->table->nameRecordZ.arrayZ,
						    this->table->count);

      this->names.alloc_exact (all_names.length);

      အတွက် (unsigned ကိန်းပြည့် i = 0; i < all_names.length; i++)
      {
	hb_ot_name_entry_t *entry = this->names.push ();

	entry->name_id = all_names[i].nameID;
	entry->language = all_names[i].language (face);
	entry->entry_score =  all_names[i].score ();
	entry->entry_index = i;
      }

      this->names.qsort (_hb_ot_name_entry_cmp);
      /* Walk and pick best only for each name_id,language pair,
       * while dropping unsupported encodings. */
      unsigned ကိန်းပြည့် j = 0;
      အတွက် (unsigned ကိန်းပြည့် i = 0; i < this->names.length; i++)
      {
	အကယ်၍ (this->names[i].entry_score == UNSUPPORTED ||
	    this->names[i].language == HB_LANGUAGE_INVALID)
	  continue;
	အကယ်၍ (i &&
	    this->names[i - 1].name_id  == this->names[i].name_id &&
	    this->names[i - 1].language == this->names[i].language)
	  continue;
	this->names[j++] = this->names[i];
      }
      this->names.resize (j);
    }
    ~accelerator_t ()
    {
      this->table.destroy ();
    }

    ကိန်းပြည့် get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned ကိန်းပြည့်    *width=nullptr) const
    {
      const hb_ot_name_entry_t key = {name_id, {0}, language};
      const hb_ot_name_entry_t *entry = hb_bsearch (key, (const hb_ot_name_entry_t *) this->names,
						    this->names.length,
						    parimaa_na (hb_ot_name_entry_t),
						    _hb_ot_name_entry_cmp_key,
						    true);

      အကယ်၍ (!entry)
      {
	entry = hb_bsearch (key, (const hb_ot_name_entry_t *) this->names,
			    this->names.length,
			    parimaa_na (hb_ot_name_entry_t),
			    _hb_ot_name_entry_cmp_key,
			    false);
      }

      အကယ်၍ (!entry)
	ပြန်ပေး -1;

      အကယ်၍ (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      ပြန်ပေး entry->entry_index;
    }

    hb_bytes_t get_name (unsigned ကိန်းပြည့် idx) const
    {
      const hb_array_t<const NameRecord> all_names (table->nameRecordZ.arrayZ, table->count);
      const NameRecord &record = all_names[idx];
      const hb_bytes_t string_pool (pool, pool_len);
      ပြန်ပေး string_pool.sub_array (record.offset, record.length);
    }

    private:
    const var_na *pool;
    unsigned ကိန်းပြည့် pool_len;
    public:
    hb_blob_ptr_t<name> table;
    hb_vector_t<hb_ot_name_entry_t> names;
  };

  public:
  /* We only implement format 0 for now. */
  HBUINT16	format;		/* Format selector (=0/1). */
  HBUINT16	count;		/* Number of name records. */
  NNOffset16To<UnsizedArrayOf<HBUINT8>>
		stringOffset;	/* Offset to start of string storage (from start of table). */
  UnsizedArrayOf<NameRecord>
		nameRecordZ;	/* The name records where count is the number of records. */
  public:
  DEFINE_SIZE_ARRAY (6, nameRecordZ);
};

#undef entry_index
#undef entry_score

sa_nracanaa name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
