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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-head-table.hh"


નામાકાશ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

સંરચના OpenTypeFontFile;
સંરચના OpenTypeOffsetTable;
સંરચના TTCHeader;


પ્રકારવ્યાખ્યા સંરચના TableRecord
{
  પૂર્ણાંક cmp (Tag t) અચલ { પાછા -t.cmp (tag); }

  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    અચલ TableRecord *a = (અચલ TableRecord *) pa;
    અચલ TableRecord *b = (અચલ TableRecord *) pb;
    પાછા b->cmp (a->tag);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  જાહેર:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

પ્રકારવ્યાખ્યા સંરચના OpenTypeOffsetTable
{
  friend સંરચના OpenTypeFontFile;

  unsigned પૂર્ણાંક get_table_ગણતરી () અચલ { પાછા tables.len; }
  અચલ TableRecord& get_table (unsigned પૂર્ણાંક i) અચલ
  { પાછા tables[i]; }
  unsigned પૂર્ણાંક get_table_tags (unsigned પૂર્ણાંક  start_offset,
			       unsigned પૂર્ણાંક *table_ગણતરી_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) અચલ
  {
    જો (table_ગણતરી_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_ગણતરી_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_ગણતરી_2))
      ;
    }
    પાછા tables.len;
  }
  તાર્કિક find_table_index (hb_tag_t tag, unsigned પૂર્ણાંક *table_index) અચલ
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    જો (tables.len < 16)
      પાછા tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    નહીંતર
      પાછા tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  અચલ TableRecord& get_table_by_tag (hb_tag_t tag) અચલ
  {
    unsigned પૂર્ણાંક table_index;
    find_table_index (tag, &table_index);
    પાછા get_table (table_index);
  }

  જાહેર:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    /* Alloc 12 for the OTHeader. */
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned અંક_વસ્તુઓ = hb_len (it);
    જો (unlikely (!tables.serialize (c, અંક_વસ્તુઓ))) return_trace (ખોટું);

    અચલ અક્ષર *dir_end = (અચલ અક્ષર *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    માટે (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      અક્ષર *start = (અક્ષર *) c->allocate_size<શૂન્ય> (len, ખોટું);
      જો (unlikely (!start)) પાછા ખોટું;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      જો (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((અક્ષર *) start - (અક્ષર *) આ),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (ખોટું);

      જો (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      અચલ અક્ષર *end = (અચલ અક્ષર *) c->head;

      જો (entry.first == HB_OT_TAG_head &&
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

    જો (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (આ, dir_end - (અચલ અક્ષર *) આ);
      માટે (unsigned પૂર્ણાંક i = 0; i < અંક_વસ્તુઓ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && tables.sanitize (c));
  }

  સુરક્ષિત:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_શોધ_Array_Of<TableRecord>
		tables;
  જાહેર:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

સંરચના TTCHeaderVersion1
{
  friend સંરચના TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned પૂર્ણાંક get_face_ગણતરી () અચલ { પાછા get_directory_list ().len; }
  અચલ OpenTypeFontFace& get_face (unsigned પૂર્ણાંક i) અચલ
  { પાછા આ+get_directory_list ()[i]; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
#ifndef HB_NO_BEYOND_64K
    જો (version.minor >= 1)
    {
      જો (unlikely (!table.sanitize_shallow (c)))
	return_trace (ખોટું);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      જો (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (ખોટું);
      અચલ DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, આ));
    }
#endif
    return_trace (table.sanitize (c, આ));
  }

  ખાનગી:
  અચલ DirectoryList& get_directory_list () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      પાછા StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    પાછા table;
  }

  સુરક્ષિત:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  જાહેર:
  DEFINE_SIZE_ARRAY (12, table);
};

સંરચના TTCHeader
{
  friend સંરચના OpenTypeFontFile;

  ખાનગી:

  unsigned પૂર્ણાંક get_face_ગણતરી () અચલ
  {
    પસંદગી (u.header.version.major) {
    સ્થિતિ 2: /* version 2 is compatible with version 1 */
    સ્થિતિ 1: hb_barrier (); પાછા u.version1.get_face_ગણતરી ();
    મૂળભૂત:પાછા 0;
    }
  }
  અચલ OpenTypeFontFace& get_face (unsigned પૂર્ણાંક i) અચલ
  {
    પસંદગી (u.header.version.major) {
    સ્થિતિ 2: /* version 2 is compatible with version 1 */
    સ્થિતિ 1: hb_barrier (); પાછા u.version1.get_face (i);
    મૂળભૂત:પાછા Null (OpenTypeFontFace);
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!u.header.version.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.header.version.major) {
    સ્થિતિ 2: /* version 2 is compatible with version 1 */
    સ્થિતિ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના {
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

સંરચના ResourceRecord
{
  અચલ OpenTypeFontFace & get_face (અચલ શૂન્ય *data_base) અચલ
  { પાછા * reinterpret_cast<અચલ OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ શૂન્ય *data_base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  સુરક્ષિત:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

સંરચના ResourceTypeRecord
{
  unsigned પૂર્ણાંક get_resource_ગણતરી () અચલ
  { પાછા tag == HB_TAG_sfnt ? res_ગણતરી_M1 + 1 : 0; }

  તાર્કિક is_sfnt () અચલ { પાછા tag == HB_TAG_sfnt; }

  અચલ ResourceRecord& get_resource_record (unsigned પૂર્ણાંક i,
					     અચલ શૂન્ય *type_base) અચલ
  { પાછા (type_base+resourcesZ).as_array (get_resource_ગણતરી ())[i]; }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ શૂન્ય *type_base,
		 અચલ શૂન્ય *data_base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_ગણતરી (),
				       data_base));
  }

  સુરક્ષિત:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_ગણતરી_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના ResourceMap
{
  unsigned પૂર્ણાંક get_face_ગણતરી () અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = get_type_ગણતરી ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      અચલ ResourceTypeRecord& type = get_type_record (i);
      જો (type.is_sfnt ())
	પાછા type.get_resource_ગણતરી ();
    }
    પાછા 0;
  }

  અચલ OpenTypeFontFace& get_face (unsigned પૂર્ણાંક idx,
				    અચલ શૂન્ય *data_base) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = get_type_ગણતરી ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      અચલ ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      જો (type.is_sfnt () && idx < type.get_resource_ગણતરી ())
	પાછા type.get_resource_record (idx, &(આ+typeList)).get_face (data_base);
    }
    પાછા Null (OpenTypeFontFace);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *data_base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  typeList.sanitize (c, આ,
				     &(આ+typeList),
				     data_base));
  }

  ખાનગી:
  unsigned પૂર્ણાંક get_type_ગણતરી () અચલ { પાછા (આ+typeList).lenM1 + 1; }

  અચલ ResourceTypeRecord& get_type_record (unsigned પૂર્ણાંક i) અચલ
  { પાછા (આ+typeList)[i]; }

  સુરક્ષિત:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  જાહેર:
  DEFINE_SIZE_STATIC (28);
};

સંરચના ResourceForkHeader
{
  unsigned પૂર્ણાંક get_face_ગણતરી () અચલ
  { પાછા (આ+map).get_face_ગણતરી (); }

  અચલ OpenTypeFontFace& get_face (unsigned પૂર્ણાંક idx,
				    unsigned પૂર્ણાંક *base_offset = nullptr) અચલ
  {
    અચલ OpenTypeFontFace &face = (આ+map).get_face (idx, &(આ+data));
    જો (base_offset)
      *base_offset = (અચલ અક્ષર *) &face - (અચલ અક્ષર *) આ;
    પાછા face;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  data.sanitize (c, આ, dataLen) &&
		  map.sanitize (c, આ, &(આ+data)));
  }

  સુરક્ષિત:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  જાહેર:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

સંરચના OpenTypeFontFile
{
  ગણના {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () અચલ { પાછા u.tag.v; }

  unsigned પૂર્ણાંક get_face_ગણતરી () અચલ
  {
    પસંદગી (u.tag.v) {
    સ્થિતિ CFFTag:	/* All the non-collection tags */
    સ્થિતિ TrueTag:
    સ્થિતિ Typ1Tag:
    સ્થિતિ TrueTypeTag:	પાછા 1;
    સ્થિતિ TTCTag:	hb_barrier (); પાછા u.ttcHeader.get_face_ગણતરી ();
    સ્થિતિ DFontTag:	hb_barrier (); પાછા u.rfHeader.get_face_ગણતરી ();
    મૂળભૂત:		પાછા 0;
    }
  }
  અચલ OpenTypeFontFace& get_face (unsigned પૂર્ણાંક i, unsigned પૂર્ણાંક *base_offset = nullptr) અચલ
  {
    જો (base_offset)
      *base_offset = 0;
    પસંદગી (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    સ્થિતિ CFFTag:	/* All the non-collection tags */
    સ્થિતિ TrueTag:
    સ્થિતિ Typ1Tag:
    સ્થિતિ TrueTypeTag:	hb_barrier (); પાછા u.fontFace;
    સ્થિતિ TTCTag:	hb_barrier (); પાછા u.ttcHeader.get_face (i);
    સ્થિતિ DFontTag:	hb_barrier (); પાછા u.rfHeader.get_face (i, base_offset);
    મૂળભૂત:		પાછા Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  તાર્કિક serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator વસ્તુઓ)
  {
    TRACE_SERIALIZE (આ);
    પ્રતિજ્ઞા (sfnt_tag != TTCTag);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    return_trace (u.fontFace.serialize (c, sfnt_tag, વસ્તુઓ));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!u.tag.v.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.tag.v) {
    સ્થિતિ CFFTag:	/* All the non-collection tags */
    સ્થિતિ TrueTag:
    સ્થિતિ Typ1Tag:
    સ્થિતિ TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    સ્થિતિ TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    સ્થિતિ DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    મૂળભૂત:		return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
