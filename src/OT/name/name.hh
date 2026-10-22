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
inline unsigned số_nguyên
hb_ot_name_convert_utf (hb_bytes_t                       bytes,
			unsigned số_nguyên                    *text_size /* IN/OUT */,
			typename out_utf_t::codepoint_t *text /* OUT */)
{
  unsigned số_nguyên src_len = bytes.length / sizeof (typename in_utf_t::codepoint_t);
  const typename in_utf_t::codepoint_t *src = (const typename in_utf_t::codepoint_t *) bytes.arrayZ;
  const typename in_utf_t::codepoint_t *src_end = src + src_len;

  typename out_utf_t::codepoint_t *dst = text;

  hb_codepoint_t unicode;
  const hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;

  nếu (text_size && *text_size)
  {
    (*text_size)--; /* Save room for NUL-termination. */
    const typename out_utf_t::codepoint_t *dst_end = text + *text_size;

    trong_khi (src < src_end && dst < dst_end)
    {
      const typename in_utf_t::codepoint_t *src_next = in_utf_t::next (src, src_end, &unicode, replacement);
      typename out_utf_t::codepoint_t *dst_next = out_utf_t::encode (dst, dst_end, unicode);
      nếu (dst_next == dst)
	break; /* Out-of-room. */

      dst = dst_next;
      src = src_next;
    }

    *text_size = dst - text;
    *dst = 0; /* NUL-terminate. */
  }

  /* Accumulate length of rest. */
  unsigned số_nguyên dst_len = dst - text;
  trong_khi (src < src_end)
  {
    src = in_utf_t::next (src, src_end, &unicode, replacement);
    dst_len += out_utf_t::encode_len (unicode);
  }
  trả_về dst_len;
}

#define entry_score var.u16[0]
#define entry_index var.u16[1]


/*
 * name -- Naming
 * https://docs.microsoft.com/en-us/typography/opentype/spec/name
 */
#define HB_OT_TAG_name HB_TAG('n','a','m','e')

#define UNSUPPORTED	42

struct NameRecord
{
  hb_language_t language (hb_face_t *face) const
  {
#ifndef HB_NO_OT_NAME_LANGUAGE
    unsigned số_nguyên p = platformID;
    unsigned số_nguyên l = languageID;

    nếu (p == 3)
      trả_về _hb_ot_name_language_for_ms_code (l);

    nếu (p == 1)
      trả_về _hb_ot_name_language_for_mac_code (l);

#ifndef HB_NO_OT_NAME_LANGUAGE_AAT
    nếu (p == 0)
      trả_về face->table.ltag->get_language (l);
#endif

#endif
    trả_về HB_LANGUAGE_INVALID;
  }

  uint16_t score () const
  {
    /* Same order as in cmap::find_best_subtable(). */
    unsigned số_nguyên p = platformID;
    unsigned số_nguyên e = encodingID;

    /* 32-bit. */
    nếu (p == 3 && e == 10) trả_về 0;
    nếu (p == 0 && e ==  6) trả_về 1;
    nếu (p == 0 && e ==  4) trả_về 2;

    /* 16-bit. */
    nếu (p == 3 && e ==  1) trả_về 3;
    nếu (p == 0 && e ==  3) trả_về 4;
    nếu (p == 0 && e ==  2) trả_về 5;
    nếu (p == 0 && e ==  1) trả_về 6;
    nếu (p == 0 && e ==  0) trả_về 7;

    /* Symbol. */
    nếu (p == 3 && e ==  0) trả_về 8;

    /* We treat all Mac Latin names as ASCII only. */
    nếu (p == 1 && e ==  0) trả_về 10; /* 10 is magic number :| */

    trả_về UNSUPPORTED;
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
    nếu (unlikely (!out)) return_trace (nullptr);
#ifdef HB_EXPERIMENTAL_API
    hb_ot_name_record_ids_t record_ids (platformID, encodingID, languageID, nameID);
    hb_bytes_t* name_bytes;

    nếu (name_table_overrides->has (record_ids, &name_bytes)) {
      hb_bytes_t encoded_bytes = *name_bytes;
      char *name_str_utf16_be = nullptr;

      nếu (platformID != 1)
      {
        unsigned text_size = hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, nullptr, nullptr);

        text_size++; // needs to consider NULL terminator for use in hb_ot_name_convert_utf()
        unsigned byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        name_str_utf16_be = (char *) hb_calloc (byte_len, 1);
        nếu (!name_str_utf16_be)
        {
          c->revert (snap);
          return_trace (nullptr);
        }
        hb_ot_name_convert_utf<hb_utf8_t, hb_utf16_be_t> (*name_bytes, &text_size,
                                                          (hb_utf16_be_t::codepoint_t *) name_str_utf16_be);

        unsigned encoded_byte_len = text_size * hb_utf16_be_t::codepoint_t::static_size;
        nếu (!encoded_byte_len || !c->check_assign (out->length, encoded_byte_len, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
          c->revert (snap);
          hb_free (name_str_utf16_be);
          return_trace (nullptr);
        }

        encoded_bytes = hb_bytes_t (name_str_utf16_be, encoded_byte_len);
      }
      không_thì
      {
        // mac platform, copy the UTF-8 string(all ascii characters) as is
        nếu (!c->check_assign (out->length, encoded_bytes.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) {
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
    không_thì
#endif
    {
      out->offset.serialize_copy (c, offset, base, 0, hb_serialize_context_t::Tail, length);
    }
    return_trace (out);
  }

  bool isUnicode () const
  {
    unsigned số_nguyên p = platformID;
    unsigned số_nguyên e = encodingID;

    trả_về (p == 0 ||
	    (p == 3 && (e == 0 || e == 1 || e == 10)));
  }

  static số_nguyên cmp (const void *pa, const void *pb)
  {
    const NameRecord *a = (const NameRecord *)pa;
    const NameRecord *b = (const NameRecord *)pb;

    nếu (a->platformID != b->platformID)
      trả_về a->platformID - b->platformID;

    nếu (a->encodingID != b->encodingID)
      trả_về a->encodingID - b->encodingID;

    nếu (a->languageID != b->languageID)
      trả_về a->languageID - b->languageID;

    nếu (a->nameID != b->nameID)
      trả_về a->nameID - b->nameID;

    nếu (a->length != b->length)
      trả_về a->length - b->length;

    trả_về 0;
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

static số_nguyên
_hb_ot_name_entry_cmp_key (const void *pa, const void *pb, bool exact)
{
  const hb_ot_name_entry_t *a = (const hb_ot_name_entry_t *) pa;
  const hb_ot_name_entry_t *b = (const hb_ot_name_entry_t *) pb;

  /* Compare by name_id, then language. */

  nếu (a->name_id != b->name_id)
    trả_về a->name_id - b->name_id;

  nếu (a->language == b->language) trả_về 0;
  nếu (!a->language) trả_về -1;
  nếu (!b->language) trả_về +1;

  const char *astr = hb_language_to_string (a->language);
  const char *bstr = hb_language_to_string (b->language);

  signed c = strcmp (astr, bstr);

  // 'a' is the user request, and 'b' is string in the font.
  // If eg. user asks for "en-us" and font has "en", approve.
  nếu (!exact && c &&
      hb_language_matches (b->language, a->language))
    trả_về 0;

  trả_về c;
}

static số_nguyên
_hb_ot_name_entry_cmp (const void *pa, const void *pb)
{
  /* Compare by name_id, then language, then score, then index. */

  số_nguyên v = _hb_ot_name_entry_cmp_key (pa, pb, true);
  nếu (v)
    trả_về v;

  const hb_ot_name_entry_t *a = (const hb_ot_name_entry_t *) pa;
  const hb_ot_name_entry_t *b = (const hb_ot_name_entry_t *) pb;

  nếu (a->entry_score != b->entry_score)
    trả_về a->entry_score - b->entry_score;

  nếu (a->entry_index != b->entry_index)
    trả_về a->entry_index - b->entry_index;

  trả_về 0;
}

struct name
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_name;

  size_t get_size () const
  { trả_về min_size + count * nameRecordZ.item_size; }

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

    nếu (unlikely (!c->extend_min ((*this))))  return_trace (false);

    unsigned total_count = it.len ()
#ifdef HB_EXPERIMENTAL_API
        + insert_name_records.length
#endif
        ;
    this->format = 0;
    nếu (!c->check_assign (this->count, total_count, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      trả_về false;

    NameRecord *name_records = (NameRecord *) hb_calloc (total_count, NameRecord::static_size);
    nếu (unlikely (!name_records)) return_trace (false);

    hb_array_t<NameRecord> records (name_records, total_count);

    cho (const NameRecord& record : it)
    {
      hb_memcpy (name_records, &record, NameRecord::static_size);
      name_records++;
    }

#ifdef HB_EXPERIMENTAL_API
    cho (unsigned i = 0; i < insert_name_records.length; i++)
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


    nếu (unlikely (c->ran_out_of_room ())) return_trace (false);

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
      trả_về
          (c->plan->flags & HB_SUBSET_FLAGS_NAME_LEGACY)
          || namerecord.isUnicode ();
    })
#ifdef HB_EXPERIMENTAL_API
    | hb_filter ([&] (const NameRecord& namerecord) {
      nếu (name_table_overrides->is_empty ())
        trả_về true;
      hb_ot_name_record_ids_t rec_ids (namerecord.platformID,
                                       namerecord.encodingID,
                                       namerecord.languageID,
                                       namerecord.nameID);

      hb_bytes_t *p;
      nếu (name_table_overrides->has (rec_ids, &p) &&
          (*p).length == 0)
        trả_về false;
      trả_về true;
    })
#endif
    ;

#ifdef HB_EXPERIMENTAL_API
    hb_hashmap_t<hb_ot_name_record_ids_t, unsigned> retained_name_record_ids;
    cho (const NameRecord& rec : it)
    {
      hb_ot_name_record_ids_t rec_ids (rec.platformID,
                                       rec.encodingID,
                                       rec.languageID,
                                       rec.nameID);
      retained_name_record_ids.set (rec_ids, 1);
    }

    hb_vector_t<hb_ot_name_record_ids_t> insert_name_records;
    nếu (!name_table_overrides->is_empty ())
    {
      nếu (unlikely (!insert_name_records.alloc (name_table_overrides->get_population (), true)))
        trả_về false;
      cho (const auto& record_ids : name_table_overrides->keys ())
      {
        nếu (name_table_overrides->get (record_ids).length == 0)
          continue;
        nếu (retained_name_record_ids.has (record_ids))
          continue;
        insert_name_records.push (record_ids);
      }
    }
#endif

    trả_về name_prime->serialize (c->serializer, it,
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

  struct accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      this->table = hb_sanitize_context_t ().reference_table<name> (face);
      assert (this->table.get_length () >= this->table->stringOffset);
      this->pool = (const char *) (const void *) (this->table+this->table->stringOffset);
      this->pool_len = this->table.get_length () - this->table->stringOffset;
      const hb_array_t<const NameRecord> all_names (this->table->nameRecordZ.arrayZ,
						    this->table->count);

      this->names.alloc_exact (all_names.length);

      cho (unsigned số_nguyên i = 0; i < all_names.length; i++)
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
      unsigned số_nguyên j = 0;
      cho (unsigned số_nguyên i = 0; i < this->names.length; i++)
      {
	nếu (this->names[i].entry_score == UNSUPPORTED ||
	    this->names[i].language == HB_LANGUAGE_INVALID)
	  continue;
	nếu (i &&
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

    số_nguyên get_index (hb_ot_name_id_t  name_id,
		   hb_language_t    language,
		   unsigned số_nguyên    *width=nullptr) const
    {
      const hb_ot_name_entry_t key = {name_id, {0}, language};
      const hb_ot_name_entry_t *entry = hb_bsearch (key, (const hb_ot_name_entry_t *) this->names,
						    this->names.length,
						    sizeof (hb_ot_name_entry_t),
						    _hb_ot_name_entry_cmp_key,
						    true);

      nếu (!entry)
      {
	entry = hb_bsearch (key, (const hb_ot_name_entry_t *) this->names,
			    this->names.length,
			    sizeof (hb_ot_name_entry_t),
			    _hb_ot_name_entry_cmp_key,
			    false);
      }

      nếu (!entry)
	trả_về -1;

      nếu (width)
	*width = entry->entry_score < 10 ? 2 : 1;

      trả_về entry->entry_index;
    }

    hb_bytes_t get_name (unsigned số_nguyên idx) const
    {
      const hb_array_t<const NameRecord> all_names (table->nameRecordZ.arrayZ, table->count);
      const NameRecord &record = all_names[idx];
      const hb_bytes_t string_pool (pool, pool_len);
      trả_về string_pool.sub_array (record.offset, record.length);
    }

    private:
    const char *pool;
    unsigned số_nguyên pool_len;
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

struct name_accelerator_t : name::accelerator_t {
  name_accelerator_t (hb_face_t *face) : name::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_NAME_NAME_HH */
