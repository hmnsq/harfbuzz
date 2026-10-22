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

#include "hb-open-type.hh"
#include "hb-ot-head-table.hh"


namespace OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

sa_nracanaa OpenTypeFontFile;
sa_nracanaa OpenTypeOffsetTable;
sa_nracanaa TTCHeader;


typedef sa_nracanaa TableRecord
{
  ចំនួនគត់ cmp (Tag t) const { ត្រឡប់ -t.cmp (tag); }

  HB_INTERNAL static ចំនួនគត់ cmp (const void *pa, const void *pb)
  {
    const TableRecord *a = (const TableRecord *) pa;
    const TableRecord *b = (const TableRecord *) pb;
    ត្រឡប់ b->cmp (a->tag);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  Tag		tag;		/* 4-byte identifier. */
  CheckSum	checkSum;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	length;		/* Length of this table. */
  public:
  DEFINE_SIZE_STATIC (16);
} OpenTypeTable;

typedef sa_nracanaa OpenTypeOffsetTable
{
  friend sa_nracanaa OpenTypeFontFile;

  unsigned ចំនួនគត់ get_table_count () const { ត្រឡប់ tables.len; }
  const TableRecord& get_table (unsigned ចំនួនគត់ i) const
  { ត្រឡប់ tables[i]; }
  unsigned ចំនួនគត់ get_table_tags (unsigned ចំនួនគត់  start_offset,
			       unsigned ចំនួនគត់ *table_count, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) const
  {
    ប្រសិនបើ (table_count && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_count)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_count))
      ;
    }
    ត្រឡប់ tables.len;
  }
  bool find_table_index (hb_tag_t tag, unsigned ចំនួនគត់ *table_index) const
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    ប្រសិនបើ (tables.len < 16)
      ត្រឡប់ tables.lfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
    បើមិនដូច្នេះ
      ត្រឡប់ tables.bfind (t, table_index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
  const TableRecord& get_table_by_tag (hb_tag_t tag) const
  {
    unsigned ចំនួនគត់ table_index;
    find_table_index (tag, &table_index);
    ត្រឡប់ get_table (table_index);
  }

  public:

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  bool serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    /* Alloc 12 for the OTHeader. */
    ប្រសិនបើ (unlikely (!c->extend_min (this))) return_trace (false);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    unsigned num_items = hb_len (it);
    ប្រសិនបើ (unlikely (!tables.serialize (c, num_items))) return_trace (false);

    const var_na *dir_end = (const var_na *) c->head;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    unsigned i = 0;
    សម្រាប់ (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      unsigned len = blob->length;

      /* Allocate room for the table and copy it. */
      var_na *start = (var_na *) c->allocate_size<void> (len, false);
      ប្រសិនបើ (unlikely (!start)) ត្រឡប់ false;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.length = len;
      rec.offset = 0;
      ប្រសិនបើ (unlikely (!c->check_assign (rec.offset,
				      (unsigned) ((var_na *) start - (var_na *) this),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (false);

      ប្រសិនបើ (likely (len))
	hb_memcpy (start, blob->data, len);

      /* 4-byte alignment. */
      c->align (4);
      const var_na *end = (const var_na *) c->head;

      ប្រសិនបើ (entry.first == HB_OT_TAG_head &&
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

    ប្រសិនបើ (checksum_adjustment)
    {
      CheckSum checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_data (this, dir_end - (const var_na *) this);
      សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < num_items; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.checkSum;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && tables.sanitize (c));
  }

  rak_sita:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  BinSearchArrayOf<TableRecord>
		tables;
  public:
  DEFINE_SIZE_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

sa_nracanaa TTCHeaderVersion1
{
  friend sa_nracanaa TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  unsigned ចំនួនគត់ get_face_count () const { ត្រឡប់ get_directory_list ().len; }
  const OpenTypeFontFace& get_face (unsigned ចំនួនគត់ i) const
  { ត្រឡប់ this+get_directory_list ()[i]; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
#ifndef HB_NO_BEYOND_64K
    ប្រសិនបើ (version.minor >= 1)
    {
      ប្រសិនបើ (unlikely (!table.sanitize_shallow (c)))
	return_trace (false);
      unsigned extra_size = version.major == 2 ? 12 : 0;
      ប្រសិនបើ (unlikely (!c->check_range (&table, table.get_size () + extra_size)))
	return_trace (false);
      const DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_size () + extra_size);
      return_trace (table2.sanitize (c, this));
    }
#endif
    return_trace (table.sanitize (c, this));
  }

  private:
  const DirectoryList& get_directory_list () const
  {
#ifndef HB_NO_BEYOND_64K
    ប្រសិនបើ (version.minor >= 1)
    {
      unsigned extra_size = version.major == 2 ? 12 : 0;
      ត្រឡប់ StructAtOffset<DirectoryList> (&table, table.get_size () + extra_size);
    }
#endif
    ត្រឡប់ table;
  }

  rak_sita:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  public:
  DEFINE_SIZE_ARRAY (12, table);
};

sa_nracanaa TTCHeader
{
  friend sa_nracanaa OpenTypeFontFile;

  private:

  unsigned ចំនួនគត់ get_face_count () const
  {
    switch (u.header.version.major) {
    pak_sa 2: /* version 2 is compatible with version 1 */
    pak_sa 1: hb_barrier (); ត្រឡប់ u.version1.get_face_count ();
    default:ត្រឡប់ 0;
    }
  }
  const OpenTypeFontFace& get_face (unsigned ចំនួនគត់ i) const
  {
    switch (u.header.version.major) {
    pak_sa 2: /* version 2 is compatible with version 1 */
    pak_sa 1: hb_barrier (); ត្រឡប់ u.version1.get_face (i);
    default:ត្រឡប់ Null (OpenTypeFontFace);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ប្រសិនបើ (unlikely (!u.header.version.sanitize (c))) return_trace (false);
    hb_barrier ();
    switch (u.header.version.major) {
    pak_sa 2: /* version 2 is compatible with version 1 */
    pak_sa 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    default:return_trace (true);
    }
  }

  rak_sita:
  sa_ngha {
  sa_nracanaa {
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

sa_nracanaa ResourceRecord
{
  const OpenTypeFontFace & get_face (const void *data_base) const
  { ត្រឡប់ * reinterpret_cast<const OpenTypeFontFace *> ((data_base+offset).arrayZ); }

  bool sanitize (hb_sanitize_context_t *c,
		 const void *data_base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  offset.sanitize (c, data_base) &&
		  hb_barrier () &&
		  get_face (data_base).sanitize (c));
  }

  rak_sita:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  public:
  DEFINE_SIZE_STATIC (12);
};

#define HB_TAG_sfnt HB_TAG ('s','f','n','t')

sa_nracanaa ResourceTypeRecord
{
  unsigned ចំនួនគត់ get_resource_count () const
  { ត្រឡប់ tag == HB_TAG_sfnt ? resCountM1 + 1 : 0; }

  bool is_sfnt () const { ត្រឡប់ tag == HB_TAG_sfnt; }

  const ResourceRecord& get_resource_record (unsigned ចំនួនគត់ i,
					     const void *type_base) const
  { ត្រឡប់ (type_base+resourcesZ).as_array (get_resource_count ())[i]; }

  bool sanitize (hb_sanitize_context_t *c,
		 const void *type_base,
		 const void *data_base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_count (),
				       data_base));
  }

  rak_sita:
  Tag		tag;		/* Resource type. */
  HBUINT16	resCountM1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa ResourceMap
{
  unsigned ចំនួនគត់ get_face_count () const
  {
    unsigned ចំនួនគត់ count = get_type_count ();
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < count; i++)
    {
      const ResourceTypeRecord& type = get_type_record (i);
      ប្រសិនបើ (type.is_sfnt ())
	ត្រឡប់ type.get_resource_count ();
    }
    ត្រឡប់ 0;
  }

  const OpenTypeFontFace& get_face (unsigned ចំនួនគត់ idx,
				    const void *data_base) const
  {
    unsigned ចំនួនគត់ count = get_type_count ();
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < count; i++)
    {
      const ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      ប្រសិនបើ (type.is_sfnt () && idx < type.get_resource_count ())
	ត្រឡប់ type.get_resource_record (idx, &(this+typeList)).get_face (data_base);
    }
    ត្រឡប់ Null (OpenTypeFontFace);
  }

  bool sanitize (hb_sanitize_context_t *c, const void *data_base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  typeList.sanitize (c, this,
				     &(this+typeList),
				     data_base));
  }

  private:
  unsigned ចំនួនគត់ get_type_count () const { ត្រឡប់ (this+typeList).lenM1 + 1; }

  const ResourceTypeRecord& get_type_record (unsigned ចំនួនគត់ i) const
  { ត្រឡប់ (this+typeList)[i]; }

  rak_sita:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  public:
  DEFINE_SIZE_STATIC (28);
};

sa_nracanaa ResourceForkHeader
{
  unsigned ចំនួនគត់ get_face_count () const
  { ត្រឡប់ (this+map).get_face_count (); }

  const OpenTypeFontFace& get_face (unsigned ចំនួនគត់ idx,
				    unsigned ចំនួនគត់ *base_offset = nullptr) const
  {
    const OpenTypeFontFace &face = (this+map).get_face (idx, &(this+data));
    ប្រសិនបើ (base_offset)
      *base_offset = (const var_na *) &face - (const var_na *) this;
    ត្រឡប់ face;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  data.sanitize (c, this, dataLen) &&
		  map.sanitize (c, this, &(this+data)));
  }

  rak_sita:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		data;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	dataLen;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  public:
  DEFINE_SIZE_STATIC (16);
};

/*
 * OpenType Font File
 */

sa_nracanaa OpenTypeFontFile
{
  ga_nanaa {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () const { ត្រឡប់ u.tag.v; }

  unsigned ចំនួនគត់ get_face_count () const
  {
    switch (u.tag.v) {
    pak_sa CFFTag:	/* All the non-collection tags */
    pak_sa TrueTag:
    pak_sa Typ1Tag:
    pak_sa TrueTypeTag:	ត្រឡប់ 1;
    pak_sa TTCTag:	hb_barrier (); ត្រឡប់ u.ttcHeader.get_face_count ();
    pak_sa DFontTag:	hb_barrier (); ត្រឡប់ u.rfHeader.get_face_count ();
    default:		ត្រឡប់ 0;
    }
  }
  const OpenTypeFontFace& get_face (unsigned ចំនួនគត់ i, unsigned ចំនួនគត់ *base_offset = nullptr) const
  {
    ប្រសិនបើ (base_offset)
      *base_offset = 0;
    switch (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    pak_sa CFFTag:	/* All the non-collection tags */
    pak_sa TrueTag:
    pak_sa Typ1Tag:
    pak_sa TrueTypeTag:	hb_barrier (); ត្រឡប់ u.fontFace;
    pak_sa TTCTag:	hb_barrier (); ត្រឡប់ u.ttcHeader.get_face (i);
    pak_sa DFontTag:	hb_barrier (); ត្រឡប់ u.rfHeader.get_face (i, base_offset);
    default:		ត្រឡប់ Null (OpenTypeFontFace);
    }
  }

  template <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  bool serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator items)
  {
    TRACE_SERIALIZE (this);
    assert (sfnt_tag != TTCTag);
    ប្រសិនបើ (unlikely (!c->extend_min (this))) return_trace (false);
    return_trace (u.fontFace.serialize (c, sfnt_tag, items));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ប្រសិនបើ (unlikely (!u.tag.v.sanitize (c))) return_trace (false);
    hb_barrier ();
    switch (u.tag.v) {
    pak_sa CFFTag:	/* All the non-collection tags */
    pak_sa TrueTag:
    pak_sa Typ1Tag:
    pak_sa TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    pak_sa TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    pak_sa DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    default:		return_trace (true);
    }
  }

  rak_sita:
  sa_ngha {
  sa_nracanaa { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  public:
  DEFINE_SIZE_UNION (4, tag.v);
};


} /* namespace OT */


#endif /* HB_OPEN_FILE_HH */
