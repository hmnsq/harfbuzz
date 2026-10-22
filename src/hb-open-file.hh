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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-head-table.hh"


നാമാകാശ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

ഘടന OpenTypeFontFile;
ഘടന OpenTypeOffsetTable;
ഘടന TTCHeader;


തരനിർവചനം ഘടന TableRecord
{
  പൂർണ്ണസംഖ്യ cmp (Tag t) സ്ഥിരം { തിരികെ -t.cmp (tag); }

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം TableRecord *a = (സ്ഥിരം TableRecord *) pa;
    സ്ഥിരം TableRecord *b = (സ്ഥിരം TableRecord *) pb;
    തിരികെ b->cmp (a->tag);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  പൊതു:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

തരനിർവചനം ഘടന OpenTypeOffsetTable
{
  friend ഘടന OpenTypeFontFile;

  unsigned പൂർണ്ണസംഖ്യ get_table_എണ്ണം () സ്ഥിരം { തിരികെ tables.len; }
  സ്ഥിരം TableRecord& get_table (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ tables[i]; }
  unsigned പൂർണ്ണസംഖ്യ get_table_tags (unsigned പൂർണ്ണസംഖ്യ  start_offset,
			       unsigned പൂർണ്ണസംഖ്യ *table_എണ്ണം_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (table_എണ്ണം_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_എണ്ണം_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_എണ്ണം_2))
      ;
    }
    തിരികെ tables.len;
  }
  തർക്കം find_table_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *table_index) സ്ഥിരം
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    എങ്കിൽ (tables.len < 16)
      തിരികെ tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    അല്ലെങ്കിൽ
      തിരികെ tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  സ്ഥിരം TableRecord& get_table_by_tag (hb_tag_t tag) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ table_index;
    find_table_index (tag, &table_index);
    തിരികെ get_table (table_index);
  }

  പൊതു:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (ഇത്);
    /* Alloc 12 for the OTHeader. */
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned അക്കം_ഇനങ്ങൾ = hb_len (it);
    എങ്കിൽ (unlikely (!tables.serialize (c, അക്കം_ഇനങ്ങൾ))) return_trace (അസത്യം);

    സ്ഥിരം അക്ഷരം *dir_end = (സ്ഥിരം അക്ഷരം *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    വേണ്ടി (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      അക്ഷരം *start = (അക്ഷരം *) c->allocate_size<ശൂന്യം> (len, അസത്യം);
      എങ്കിൽ (unlikely (!start)) തിരികെ അസത്യം;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      എങ്കിൽ (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((അക്ഷരം *) start - (അക്ഷരം *) ഇത്),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (അസത്യം);

      എങ്കിൽ (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      സ്ഥിരം അക്ഷരം *end = (സ്ഥിരം അക്ഷരം *) c->head;

      എങ്കിൽ (entry.first == HB_OT_TAG_head &&
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

    എങ്കിൽ (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (ഇത്, dir_end - (സ്ഥിരം അക്ഷരം *) ഇത്);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_ഇനങ്ങൾ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && tables.sanitize (c));
  }

  സംരക്ഷിത:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_തിരയൽ_Array_Of<TableRecord>
		tables;
  പൊതു:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

ഘടന TTCHeaderVersion1
{
  friend ഘടന TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned പൂർണ്ണസംഖ്യ get_face_എണ്ണം () സ്ഥിരം { തിരികെ get_directory_list ().len; }
  സ്ഥിരം OpenTypeFontFace& get_face (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ ഇത്+get_directory_list ()[i]; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.minor >= 1)
    {
      എങ്കിൽ (unlikely (!table.sanitize_shallow (c)))
	return_trace (അസത്യം);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      എങ്കിൽ (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (അസത്യം);
      സ്ഥിരം DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, ഇത്));
    }
#endif
    return_trace (table.sanitize (c, ഇത്));
  }

  സ്വകാര്യ:
  സ്ഥിരം DirectoryList& get_directory_list () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      തിരികെ StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    തിരികെ table;
  }

  സംരക്ഷിത:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  പൊതു:
  DEFINE_SIZE_ARRAY (12, table);
};

ഘടന TTCHeader
{
  friend ഘടന OpenTypeFontFile;

  സ്വകാര്യ:

  unsigned പൂർണ്ണസംഖ്യ get_face_എണ്ണം () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.header.version.major) {
    അവസ്ഥ 2: /* version 2 is compatible with version 1 */
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.version1.get_face_എണ്ണം ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }
  സ്ഥിരം OpenTypeFontFace& get_face (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.header.version.major) {
    അവസ്ഥ 2: /* version 2 is compatible with version 1 */
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.version1.get_face (i);
    സ്ഥിരസ്ഥിതി:തിരികെ Null (OpenTypeFontFace);
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!u.header.version.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.header.version.major) {
    അവസ്ഥ 2: /* version 2 is compatible with version 1 */
    അവസ്ഥ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന {
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

ഘടന ResourceRecord
{
  സ്ഥിരം OpenTypeFontFace & get_face (സ്ഥിരം ശൂന്യം *data_base) സ്ഥിരം
  { തിരികെ * reinterpret_cast<സ്ഥിരം OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം ശൂന്യം *data_base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  സംരക്ഷിത:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

ഘടന ResourceTypeRecord
{
  unsigned പൂർണ്ണസംഖ്യ get_resource_എണ്ണം () സ്ഥിരം
  { തിരികെ tag == HB_TAG_sfnt ? res_എണ്ണം_M1 + 1 : 0; }

  തർക്കം is_sfnt () സ്ഥിരം { തിരികെ tag == HB_TAG_sfnt; }

  സ്ഥിരം ResourceRecord& get_resource_record (unsigned പൂർണ്ണസംഖ്യ i,
					     സ്ഥിരം ശൂന്യം *type_base) സ്ഥിരം
  { തിരികെ (type_base+resourcesZ).as_array (get_resource_എണ്ണം ())[i]; }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം ശൂന്യം *type_base,
		 സ്ഥിരം ശൂന്യം *data_base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_എണ്ണം (),
				       data_base));
  }

  സംരക്ഷിത:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_എണ്ണം_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന ResourceMap
{
  unsigned പൂർണ്ണസംഖ്യ get_face_എണ്ണം () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = get_type_എണ്ണം ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      സ്ഥിരം ResourceTypeRecord& type = get_type_record (i);
      എങ്കിൽ (type.is_sfnt ())
	തിരികെ type.get_resource_എണ്ണം ();
    }
    തിരികെ 0;
  }

  സ്ഥിരം OpenTypeFontFace& get_face (unsigned പൂർണ്ണസംഖ്യ idx,
				    സ്ഥിരം ശൂന്യം *data_base) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = get_type_എണ്ണം ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      സ്ഥിരം ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      എങ്കിൽ (type.is_sfnt () && idx < type.get_resource_എണ്ണം ())
	തിരികെ type.get_resource_record (idx, &(ഇത്+typeList)).get_face (data_base);
    }
    തിരികെ Null (OpenTypeFontFace);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *data_base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  typeList.sanitize (c, ഇത്,
				     &(ഇത്+typeList),
				     data_base));
  }

  സ്വകാര്യ:
  unsigned പൂർണ്ണസംഖ്യ get_type_എണ്ണം () സ്ഥിരം { തിരികെ (ഇത്+typeList).lenM1 + 1; }

  സ്ഥിരം ResourceTypeRecord& get_type_record (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ (ഇത്+typeList)[i]; }

  സംരക്ഷിത:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  പൊതു:
  DEFINE_SIZE_STATIC (28);
};

ഘടന ResourceForkHeader
{
  unsigned പൂർണ്ണസംഖ്യ get_face_എണ്ണം () സ്ഥിരം
  { തിരികെ (ഇത്+map).get_face_എണ്ണം (); }

  സ്ഥിരം OpenTypeFontFace& get_face (unsigned പൂർണ്ണസംഖ്യ idx,
				    unsigned പൂർണ്ണസംഖ്യ *base_offset = nullptr) സ്ഥിരം
  {
    സ്ഥിരം OpenTypeFontFace &face = (ഇത്+map).get_face (idx, &(ഇത്+data));
    എങ്കിൽ (base_offset)
      *base_offset = (സ്ഥിരം അക്ഷരം *) &face - (സ്ഥിരം അക്ഷരം *) ഇത്;
    തിരികെ face;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  data.sanitize (c, ഇത്, dataLen) &&
		  map.sanitize (c, ഇത്, &(ഇത്+data)));
  }

  സംരക്ഷിത:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  പൊതു:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

ഘടന OpenTypeFontFile
{
  എണ്ണൽ {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () സ്ഥിരം { തിരികെ u.tag.v; }

  unsigned പൂർണ്ണസംഖ്യ get_face_എണ്ണം () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.tag.v) {
    അവസ്ഥ CFFTag:	/* All the non-collection tags */
    അവസ്ഥ TrueTag:
    അവസ്ഥ Typ1Tag:
    അവസ്ഥ TrueTypeTag:	തിരികെ 1;
    അവസ്ഥ TTCTag:	hb_barrier (); തിരികെ u.ttcHeader.get_face_എണ്ണം ();
    അവസ്ഥ DFontTag:	hb_barrier (); തിരികെ u.rfHeader.get_face_എണ്ണം ();
    സ്ഥിരസ്ഥിതി:		തിരികെ 0;
    }
  }
  സ്ഥിരം OpenTypeFontFace& get_face (unsigned പൂർണ്ണസംഖ്യ i, unsigned പൂർണ്ണസംഖ്യ *base_offset = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (base_offset)
      *base_offset = 0;
    തിരഞ്ഞെടുപ്പ് (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    അവസ്ഥ CFFTag:	/* All the non-collection tags */
    അവസ്ഥ TrueTag:
    അവസ്ഥ Typ1Tag:
    അവസ്ഥ TrueTypeTag:	hb_barrier (); തിരികെ u.fontFace;
    അവസ്ഥ TTCTag:	hb_barrier (); തിരികെ u.ttcHeader.get_face (i);
    അവസ്ഥ DFontTag:	hb_barrier (); തിരികെ u.rfHeader.get_face (i, base_offset);
    സ്ഥിരസ്ഥിതി:		തിരികെ Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  തർക്കം serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator ഇനങ്ങൾ)
  {
    TRACE_SERIALIZE (ഇത്);
    പ്രതിജ്ഞാ (sfnt_tag != TTCTag);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    return_trace (u.fontFace.serialize (c, sfnt_tag, ഇനങ്ങൾ));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!u.tag.v.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.tag.v) {
    അവസ്ഥ CFFTag:	/* All the non-collection tags */
    അവസ്ഥ TrueTag:
    അവസ്ഥ Typ1Tag:
    അവസ്ഥ TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    അവസ്ഥ TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    അവസ്ഥ DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    സ്ഥിരസ്ഥിതി:		return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
