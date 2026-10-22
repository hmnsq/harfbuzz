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

#अगर_अघोषित HB_OPEN_FILE_HH
#घोषणा HB_OPEN_FILE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-head-table.hh"


namespace OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

काष्ठा OpenTypeFontFile;
काष्ठा OpenTypeOffsetTable;
काष्ठा TTCHeader;


प्रकार काष्ठा TableRecord
{
  पूर्णांक cmp (Tag t) स्थिर { वापस -t.cmp (tag); }

  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर TableRecord *a = (स्थिर TableRecord *) pa;
    स्थिर TableRecord *b = (स्थिर TableRecord *) pb;
    वापस b->cmp (a->tag);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  Tag		tag;		/* 4-byte identifier. */
  Check_योग	check_योग;	/* CheckSum for this table. */
  Offset32	offset;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	लंबाई;		/* Length of this table. */
  खुला:
  DEFINE_आकार_STATIC (16);
} OpenTypeTable;

प्रकार काष्ठा OpenTypeOffsetTable
{
  मित्र काष्ठा OpenTypeFontFile;

  अचिन्हित पूर्णांक get_table_गिनती () स्थिर { वापस tables.len; }
  स्थिर TableRecord& get_table (अचिन्हित पूर्णांक i) स्थिर
  { वापस tables[i]; }
  अचिन्हित पूर्णांक get_table_tags (अचिन्हित पूर्णांक  start_offset,
			       अचिन्हित पूर्णांक *table_गिनती_2, /* IN/OUT */
			       hb_tag_t     *table_tags /* OUT */) स्थिर
  {
    अगर (table_गिनती_2 && table_tags)
    {
      + tables.as_array ().sub_array (start_offset, table_गिनती_2)
      | hb_map (&TableRecord::tag)
      | hb_sink (hb_array (table_tags, *table_गिनती_2))
      ;
    }
    वापस tables.len;
  }
  bool find_table_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *table_सूचकांक) स्थिर
  {
    Tag t;
    t = tag;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    अगर (tables.len < 16)
      वापस tables.lfind (t, table_सूचकांक, HB_NOT_FOUND_STORE, सूचकांक::NOT_FOUND_सूचकांक);
    अन्यथा
      वापस tables.bfind (t, table_सूचकांक, HB_NOT_FOUND_STORE, सूचकांक::NOT_FOUND_सूचकांक);
  }
  स्थिर TableRecord& get_table_by_tag (hb_tag_t tag) स्थिर
  {
    अचिन्हित पूर्णांक table_सूचकांक;
    find_table_सूचकांक (tag, &table_सूचकांक);
    वापस get_table (table_सूचकांक);
  }

  खुला:

  ढाँचा <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  bool serialize (hb_serialize_context_t *c,
		  hb_tag_t sfnt_tag,
		  Iterator it)
  {
    TRACE_SERIALIZE (यह);
    /* Alloc 12 for the OTHeader. */
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_version = sfnt_tag;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    अचिन्हित अंक_वस्तुएँ = hb_len (it);
    अगर (unlikely (!tables.serialize (c, अंक_वस्तुएँ))) return_trace (false);

    स्थिर अक्षर *dir_end = (स्थिर अक्षर *) c->शीर्ष_2;
    HBUINT32 *checksum_adjustment = nullptr;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    अचिन्हित i = 0;
    क्रम (hb_pair_t<hb_tag_t, hb_blob_t*> entry : it)
    {
      hb_blob_t *blob = entry.second;
      अचिन्हित len = blob->लंबाई;

      /* Allocate room for the table and copy it. */
      अक्षर *start = (अक्षर *) c->allocate_आकार<व्योम> (len, false);
      अगर (unlikely (!start)) वापस false;

      TableRecord &rec = tables.arrayZ[i];
      rec.tag = entry.first;
      rec.लंबाई = len;
      rec.offset = 0;
      अगर (unlikely (!c->check_assign (rec.offset,
				      (अचिन्हित) ((अक्षर *) start - (अक्षर *) यह),
				      HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)))
        return_trace (false);

      अगर (likely (len))
	hb_memcpy (start, blob->तथ्य, len);

      /* 4-byte alignment. */
      c->align (4);
      स्थिर अक्षर *end = (स्थिर अक्षर *) c->शीर्ष_2;

      अगर (entry.first == HB_OT_TAG_शीर्ष &&
	  (अचिन्हित) (end - start) >= शीर्ष_2::static_आकार)
      {
	शीर्ष_2 *h = (शीर्ष_2 *) start;
	checksum_adjustment = &h->check_योग_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_योग.set_for_तथ्य (start, end - start);
      i++;
    }

    tables.क्विक ();

    अगर (checksum_adjustment)
    {
      Check_योग checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.set_for_तथ्य (यह, dir_end - (स्थिर अक्षर *) यह);
      क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_वस्तुएँ; i++)
      {
	TableRecord &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_योग;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && tables.sanitize (c));
  }

  रक्षित:
  Tag		sfnt_version;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_खोज_Array_Of<TableRecord>
		tables;
  खुला:
  DEFINE_आकार_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

काष्ठा TTCHeaderVersion1
{
  मित्र काष्ठा TTCHeader;

  using DirectoryList = Array32Of<Offset32To<OpenTypeOffsetTable>>;

  अचिन्हित पूर्णांक get_face_गिनती () स्थिर { वापस get_directory_list ().len; }
  स्थिर OpenTypeFontFace& get_face (अचिन्हित पूर्णांक i) स्थिर
  { वापस यह+get_directory_list ()[i]; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.minor >= 1)
    {
      अगर (unlikely (!table.sanitize_shallow (c)))
	return_trace (false);
      अचिन्हित extra_आकार = version.major == 2 ? 12 : 0;
      अगर (unlikely (!c->check_range (&table, table.get_आकार () + extra_आकार)))
	return_trace (false);
      स्थिर DirectoryList &table2 = StructAtOffset<DirectoryList> (&table,
								   table.get_आकार () + extra_आकार);
      return_trace (table2.sanitize (c, यह));
    }
#पूर्ण_अगर
    return_trace (table.sanitize (c, यह));
  }

  निजी:
  स्थिर DirectoryList& get_directory_list () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (version.minor >= 1)
    {
      अचिन्हित extra_आकार = version.major == 2 ? 12 : 0;
      वापस StructAtOffset<DirectoryList> (&table, table.get_आकार () + extra_आकार);
    }
#पूर्ण_अगर
    वापस table;
  }

  रक्षित:
  Tag		ttcTag;		/* TrueType Collection ID string: 'ttcf' */
  FixedVersion<>version;	/* Version of the TTC Header. */
  DirectoryList
		table;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  खुला:
  DEFINE_आकार_ARRAY (12, table);
};

काष्ठा TTCHeader
{
  मित्र काष्ठा OpenTypeFontFile;

  निजी:

  अचिन्हित पूर्णांक get_face_गिनती () स्थिर
  {
    चयन (u.header.version.major) {
    हाल 2: /* version 2 is compatible with version 1 */
    हाल 1: hb_barrier (); वापस u.version1.get_face_गिनती ();
    शेष:वापस 0;
    }
  }
  स्थिर OpenTypeFontFace& get_face (अचिन्हित पूर्णांक i) स्थिर
  {
    चयन (u.header.version.major) {
    हाल 2: /* version 2 is compatible with version 1 */
    हाल 1: hb_barrier (); वापस u.version1.get_face (i);
    शेष:वापस Null (OpenTypeFontFace);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!u.header.version.sanitize (c))) return_trace (false);
    hb_barrier ();
    चयन (u.header.version.major) {
    हाल 2: /* version 2 is compatible with version 1 */
    हाल 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    शेष:return_trace (true);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा {
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

काष्ठा ResourceRecord
{
  स्थिर OpenTypeFontFace & get_face (स्थिर व्योम *तथ्य_base) स्थिर
  { वापस * reinterpret_cast<स्थिर OpenTypeFontFace *> ((तथ्य_base+offset).arrayZ); }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर व्योम *तथ्य_base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  offset.sanitize (c, तथ्य_base) &&
		  hb_barrier () &&
		  get_face (तथ्य_base).sanitize (c));
  }

  रक्षित:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	nameOffset;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24To<Array32Of<HBUINT8>>
		offset;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  खुला:
  DEFINE_आकार_STATIC (12);
};

#घोषणा HB_TAG_sfnt HB_TAG ('s','f','n','t')

काष्ठा ResourceTypeRecord
{
  अचिन्हित पूर्णांक get_resource_गिनती () स्थिर
  { वापस tag == HB_TAG_sfnt ? res_गिनती_M1 + 1 : 0; }

  bool is_sfnt () स्थिर { वापस tag == HB_TAG_sfnt; }

  स्थिर ResourceRecord& get_resource_record (अचिन्हित पूर्णांक i,
					     स्थिर व्योम *type_base) स्थिर
  { वापस (type_base+resourcesZ).as_array (get_resource_गिनती ())[i]; }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर व्योम *type_base,
		 स्थिर व्योम *तथ्य_base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_base,
				       get_resource_गिनती (),
				       तथ्य_base));
  }

  रक्षित:
  Tag		tag;		/* Resource type. */
  HBUINT16	res_गिनती_M1;	/* Number of resources minus 1. */
  NNOffset16To<UnsizedArrayOf<ResourceRecord>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा ResourceMap
{
  अचिन्हित पूर्णांक get_face_गिनती () स्थिर
  {
    अचिन्हित पूर्णांक गिनती = get_type_गिनती ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      स्थिर ResourceTypeRecord& type = get_type_record (i);
      अगर (type.is_sfnt ())
	वापस type.get_resource_गिनती ();
    }
    वापस 0;
  }

  स्थिर OpenTypeFontFace& get_face (अचिन्हित पूर्णांक idx,
				    स्थिर व्योम *तथ्य_base) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = get_type_गिनती ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      स्थिर ResourceTypeRecord& type = get_type_record (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      अगर (type.is_sfnt () && idx < type.get_resource_गिनती ())
	वापस type.get_resource_record (idx, &(यह+typeList)).get_face (तथ्य_base);
    }
    वापस Null (OpenTypeFontFace);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *तथ्य_base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  typeList.sanitize (c, यह,
				     &(यह+typeList),
				     तथ्य_base));
  }

  निजी:
  अचिन्हित पूर्णांक get_type_गिनती () स्थिर { वापस (यह+typeList).lenM1 + 1; }

  स्थिर ResourceTypeRecord& get_type_record (अचिन्हित पूर्णांक i) स्थिर
  { वापस (यह+typeList)[i]; }

  रक्षित:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16To<ArrayOfM1<ResourceTypeRecord>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  खुला:
  DEFINE_आकार_STATIC (28);
};

काष्ठा ResourceForkHeader
{
  अचिन्हित पूर्णांक get_face_गिनती () स्थिर
  { वापस (यह+map).get_face_गिनती (); }

  स्थिर OpenTypeFontFace& get_face (अचिन्हित पूर्णांक idx,
				    अचिन्हित पूर्णांक *base_offset = nullptr) स्थिर
  {
    स्थिर OpenTypeFontFace &face = (यह+map).get_face (idx, &(यह+तथ्य));
    अगर (base_offset)
      *base_offset = (स्थिर अक्षर *) &face - (स्थिर अक्षर *) यह;
    वापस face;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  तथ्य.sanitize (c, यह, तथ्य_Len) &&
		  map.sanitize (c, यह, &(यह+तथ्य)));
  }

  रक्षित:
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		तथ्य;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32To<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	तथ्य_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  खुला:
  DEFINE_आकार_STATIC (16);
};

/*
 * OpenType Font File
 */

काष्ठा OpenTypeFontFile
{
  क्रमागत {
    CFFTag		= HB_TAG ('O','T','T','O'), /* OpenType with Postscript outlines */
    TrueTypeTag		= HB_TAG ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_TAG ('t','t','c','f'), /* TrueType Collection */
    DFontTag		= HB_TAG ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    TrueTag		= HB_TAG ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1Tag		= HB_TAG ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_tag_t get_tag () स्थिर { वापस u.tag.v; }

  अचिन्हित पूर्णांक get_face_गिनती () स्थिर
  {
    चयन (u.tag.v) {
    हाल CFFTag:	/* All the non-collection tags */
    हाल TrueTag:
    हाल Typ1Tag:
    हाल TrueTypeTag:	वापस 1;
    हाल TTCTag:	hb_barrier (); वापस u.ttcHeader.get_face_गिनती ();
    हाल DFontTag:	hb_barrier (); वापस u.rfHeader.get_face_गिनती ();
    शेष:		वापस 0;
    }
  }
  स्थिर OpenTypeFontFace& get_face (अचिन्हित पूर्णांक i, अचिन्हित पूर्णांक *base_offset = nullptr) स्थिर
  {
    अगर (base_offset)
      *base_offset = 0;
    चयन (u.tag.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    हाल CFFTag:	/* All the non-collection tags */
    हाल TrueTag:
    हाल Typ1Tag:
    हाल TrueTypeTag:	hb_barrier (); वापस u.fontFace;
    हाल TTCTag:	hb_barrier (); वापस u.ttcHeader.get_face (i);
    हाल DFontTag:	hb_barrier (); वापस u.rfHeader.get_face (i, base_offset);
    शेष:		वापस Null (OpenTypeFontFace);
    }
  }

  ढाँचा <typename Iterator,
	    hb_requires ((hb_is_source_of<Iterator, hb_pair_t<hb_tag_t, hb_blob_t *>>::value))>
  bool serialize_single (hb_serialize_context_t *c,
			 hb_tag_t sfnt_tag,
			 Iterator वस्तुएँ)
  {
    TRACE_SERIALIZE (यह);
    निश्चित (sfnt_tag != TTCTag);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    return_trace (u.fontFace.serialize (c, sfnt_tag, वस्तुएँ));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!u.tag.v.sanitize (c))) return_trace (false);
    hb_barrier ();
    चयन (u.tag.v) {
    हाल CFFTag:	/* All the non-collection tags */
    हाल TrueTag:
    हाल Typ1Tag:
    हाल TrueTypeTag:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    हाल TTCTag:	hb_barrier (); return_trace (u.ttcHeader.sanitize (c));
    हाल DFontTag:	hb_barrier (); return_trace (u.rfHeader.sanitize (c));
    शेष:		return_trace (true);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { Tag v; }	tag;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttcHeader;
  ResourceForkHeader	rfHeader;
  } u;
  खुला:
  DEFINE_आकार_UNION (4, tag.v);
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OPEN_FILE_HH */
