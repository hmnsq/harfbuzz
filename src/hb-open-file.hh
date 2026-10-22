/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OPEN_FILE_HH
#define HB_OPEN_FILE_HH

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-head-table.hh"


நாமாகாஶ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

கட்டமைப்பு OpenTypeFontFile;
கட்டமைப்பு OpenTypeOffsetTable;
கட்டமைப்பு TTCHeader;


வகைவரை கட்டமைப்பு TableRecord
{
  முழு cmp (Tag t) மாறிலி { திரும்பு -t.cmp (tag); }

  HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி TableRecord *a = (மாறிலி TableRecord *) pa;
    மாறிலி TableRecord *b = (மாறிலி TableRecord *) pb;
    திரும்பு b->cmp (a->tag);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  பொது:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

வகைவரை கட்டமைப்பு OpenTypeOffsetTable
{
  friend கட்டமைப்பு OpenTypeFontFile;

  unsigned முழு get_table_மொத்தம் () மாறிலி { திரும்பு tables.len; }
  மாறிலி TableRecord& get_table (unsigned முழு i) மாறிலி
  { திரும்பு tables[i]; }
  unsigned முழு get_table_tags (unsigned முழு  start_offset,
			       unsigned முழு *table_மொத்தம்_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) மாறிலி
  {
    எனில் (table_மொத்தம்_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_மொத்தம்_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_மொத்தம்_2))
      ;
    }
    திரும்பு tables.len;
  }
  தருக்க find_table_index (hb_tag_t tag, unsigned முழு *table_index) மாறிலி
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    எனில் (tables.len < 16)
      திரும்பு tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    இல்லையெனில்
      திரும்பு tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  மாறிலி TableRecord& get_table_by_tag (hb_tag_t tag) மாறிலி
  {
    unsigned முழு table_index;
    find_table_index (tag, &table_index);
    திரும்பு get_table (table_index);
  }

  பொது:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  தருக்க serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (இது);
    /* Alloc 12 for the OTHeader. */
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned இலக்கம்_பொருட்கள் = hb_len (it);
    எனில் (unlikely (!tables.serialize (c, இலக்கம்_பொருட்கள்))) return_trace (பொய்);

    மாறிலி எழுத்து *dir_end = (மாறிலி எழுத்து *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    க்கு (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      எழுத்து *start = (எழுத்து *) c->allocate_size<வெறுமை> (len, பொய்);
      எனில் (unlikely (!start)) திரும்பு பொய்;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      எனில் (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((எழுத்து *) start - (எழுத்து *) இது),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (பொய்);

      எனில் (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      மாறிலி எழுத்து *end = (மாறிலி எழுத்து *) c->head;

      எனில் (entry.first == HB_OT_TAG_head &&
	  (unsigned) (end - start) >= head::static_size)
      {
	head *h = (head *) start;
	checksum_adjustment = &h->checkSumAdjustment;
	*checksum_adjustment = 0;
      }

      rec.checkSum.set_for_data (start, end - start);
      i++;
    }

    tables.qsort ();

    எனில் (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (இது, dir_end - (மாறிலி எழுத்து *) இது);
      க்கு (unsigned முழு i = 0; i < இலக்கம்_பொருட்கள்; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && tables.sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_தேடல்_Array_Of<TableRecord>
		tables;
  பொது:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

கட்டமைப்பு TTCHeaderVersion1
{
  friend கட்டமைப்பு TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned முழு get_face_மொத்தம் () மாறிலி { திரும்பு get_directory_list ().len; }
  மாறிலி OpenTypeFontFace& get_face (unsigned முழு i) மாறிலி
  { திரும்பு இது+get_directory_list ()[i]; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
#ifndef HB_NO_BEYOND_64K
    எனில் (version.minor >= 1)
    {
      எனில் (unlikely (!table.sanitize_shallow (c)))
	return_trace (பொய்);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      எனில் (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (பொய்);
      மாறிலி DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, இது));
    }
#endif
    return_trace (table.sanitize (c, இது));
  }

  தனிப்பட்ட:
  மாறிலி DirectoryList& get_directory_list () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      திரும்பு StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    திரும்பு table;
  }

  பாதுகாக்கப்பட்ட:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  பொது:
  DEFINE_SIZE_ARRAY (12, table);
};

கட்டமைப்பு TTCHeader
{
  friend கட்டமைப்பு OpenTypeFontFile;

  தனிப்பட்ட:

  unsigned முழு get_face_மொத்தம் () மாறிலி
  {
    தேர்வு (u.header.version.major) {
    நிலை 2: /* version 2 is compatible with version 1 */
    நிலை 1: hb_barrier (); திரும்பு u.version1.get_face_மொத்தம் ();
    இயல்பு:திரும்பு 0;
    }
  }
  மாறிலி OpenTypeFontFace& get_face (unsigned முழு i) மாறிலி
  {
    தேர்வு (u.header.version.major) {
    நிலை 2: /* version 2 is compatible with version 1 */
    நிலை 1: hb_barrier (); திரும்பு u.version1.get_face (i);
    இயல்பு:திரும்பு Null (OpenTypeFontFace);
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!u.header.version.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.header.version.major) {
    நிலை 2: /* version 2 is compatible with version 1 */
    நிலை 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு {
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			header;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

கட்டமைப்பு ResourceRecord
{
  மாறிலி OpenTypeFontFace & get_face (மாறிலி வெறுமை *data_base) மாறிலி
  { திரும்பு * reinterpret_cast<மாறிலி OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி வெறுமை *data_base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  பொது:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

கட்டமைப்பு ResourceTypeRecord
{
  unsigned முழு get_resource_மொத்தம் () மாறிலி
  { திரும்பு tag == HB_TAG_sfnt ? res_மொத்தம்_M1 + 1 : 0; }

  தருக்க is_sfnt () மாறிலி { திரும்பு tag == HB_TAG_sfnt; }

  மாறிலி ResourceRecord& get_resource_record (unsigned முழு i,
					     மாறிலி வெறுமை *type_base) மாறிலி
  { திரும்பு (type_base+resourcesZ).as_array (get_resource_மொத்தம் ())[i]; }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி வெறுமை *type_base,
		 மாறிலி வெறுமை *data_base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_மொத்தம் (),
				       data_base));
  }

  பாதுகாக்கப்பட்ட:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_மொத்தம்_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு ResourceMap
{
  unsigned முழு get_face_மொத்தம் () மாறிலி
  {
    unsigned முழு மொத்தம் = get_type_மொத்தம் ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      மாறிலி ResourceTypeRecord& type = get_type_record (i);
      எனில் (type.is_sfnt ())
	திரும்பு type.get_resource_மொத்தம் ();
    }
    திரும்பு 0;
  }

  மாறிலி OpenTypeFontFace& get_face (unsigned முழு idx,
				    மாறிலி வெறுமை *data_base) மாறிலி
  {
    unsigned முழு மொத்தம் = get_type_மொத்தம் ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      மாறிலி ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      எனில் (type.is_sfnt () && idx < type.get_resource_மொத்தம் ())
	திரும்பு type.get_resource_record (idx, &(இது+typeList)).get_face (data_base);
    }
    திரும்பு Null (OpenTypeFontFace);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *data_base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  typeList.sanitize (c, இது,
				     &(இது+typeList),
				     data_base));
  }

  தனிப்பட்ட:
  unsigned முழு get_type_மொத்தம் () மாறிலி { திரும்பு (இது+typeList).lenM1 + 1; }

  மாறிலி ResourceTypeRecord& get_type_record (unsigned முழு i) மாறிலி
  { திரும்பு (இது+typeList)[i]; }

  பாதுகாக்கப்பட்ட:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  பொது:
  DEFINE_SIZE_STATIC (28);
};

கட்டமைப்பு ResourceForkHeader
{
  unsigned முழு get_face_மொத்தம் () மாறிலி
  { திரும்பு (இது+map).get_face_மொத்தம் (); }

  மாறிலி OpenTypeFontFace& get_face (unsigned முழு idx,
				    unsigned முழு *base_offset = nullptr) மாறிலி
  {
    மாறிலி OpenTypeFontFace &face = (இது+map).get_face (idx, &(இது+data));
    எனில் (base_offset)
      *base_offset = (மாறிலி எழுத்து *) &face - (மாறிலி எழுத்து *) இது;
    திரும்பு face;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  data.sanitize (c, இது, dataLen) &&
		  map.sanitize (c, இது, &(இது+data)));
  }

  பாதுகாக்கப்பட்ட:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  பொது:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

கட்டமைப்பு OpenTypeFontFile
{
  எண்ணிக்கை {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () மாறிலி { திரும்பு u.tag.v; }

  unsigned முழு get_face_மொத்தம் () மாறிலி
  {
    தேர்வு (u.tag.v) {
    நிலை CFFTag:	/* All the non-collection tags */
    நிலை TrueTag:
    நிலை Typ1Tag:
    நிலை TrueTypeTag:	திரும்பு 1;
    நிலை TTCTag:	hb_barrier (); திரும்பு u.ttcHeader.get_face_மொத்தம் ();
    நிலை DFontTag:	hb_barrier (); திரும்பு u.rfHeader.get_face_மொத்தம் ();
    இயல்பு:		திரும்பு 0;
    }
  }
  மாறிலி OpenTypeFontFace& get_face (unsigned முழு i, unsigned முழு *base_offset = nullptr) மாறிலி
  {
    எனில் (base_offset)
      *base_offset = 0;
    தேர்வு (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    நிலை CFFTag:	/* All the non-collection tags */
    நிலை TrueTag:
    நிலை Typ1Tag:
    நிலை TrueTypeTag:	hb_barrier (); திரும்பு u.fontFace;
    நிலை TTCTag:	hb_barrier (); திரும்பு u.ttcHeader.get_face (i);
    நிலை DFontTag:	hb_barrier (); திரும்பு u.rfHeader.get_face (i, base_offset);
    இயல்பு:		திரும்பு Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  தருக்க serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator பொருட்கள்)
  {
    TRACE_SERIALIZE (இது);
    ப்ரதிஜ்ஞா (sfnt_tag != TTCTag);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    return_trace (u.fontFace.serialize (c, sfnt_tag, பொருட்கள்));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!u.tag.v.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.tag.v) {
    நிலை CFFTag:	/* All the non-collection tags */
    நிலை TrueTag:
    நிலை Typ1Tag:
    நிலை TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    நிலை TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    நிலை DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    இயல்பு:		return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  பொது:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
