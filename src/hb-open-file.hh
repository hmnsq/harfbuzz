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

#جيڪڏهن_وصف_نه_ٿيل HB_OPEN_FILE_HH
#وصف_ڪريو HB_OPEN_FILE_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-head-table.hh"


نطاق OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

بنية OpenTypeFontFile;
بنية Open_Type_وٿي_جدول;
بنية TTCHeader;


تعريفنوع بنية جدول_رڪارڊ
{
  صحيح cmp (ٽيگ t) ثابت { أرجع -t.cmp (ٽيگ_2); }

  HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت جدول_رڪارڊ *a = (ثابت جدول_رڪارڊ *) pa;
    ثابت جدول_رڪارڊ *b = (ثابت جدول_رڪارڊ *) pb;
    أرجع b->cmp (a->ٽيگ_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  ٽيگ		ٽيگ_2;		/* 4-byte identifier. */
  Check_جوڙ	check_جوڙ;	/* CheckSum for this table. */
  Offset32	وٿي;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	ڊيگهائي;		/* Length of this table. */
  عام:
  DEFINE_سائيز_STATIC (16);
} Open_Type_جدول;

تعريفنوع بنية Open_Type_وٿي_جدول
{
  دوست بنية OpenTypeFontFile;

  بنانشان صحيح get_جدول_ڳڻپ () ثابت { أرجع tables.len; }
  ثابت جدول_رڪارڊ& get_جدول (بنانشان صحيح i) ثابت
  { أرجع tables[i]; }
  بنانشان صحيح get_جدول_ٽيگ_فهرست (بنانشان صحيح  شروعات_وٿي,
			       بنانشان صحيح *جدول_ڳڻپ_2, /* IN/OUT */
			       hb_ٽيگ_t     *جدول_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  {
    إذا (جدول_ڳڻپ_2 && جدول_ٽيگ_فهرست)
    {
      + tables.as_array ().sub_array (شروعات_وٿي, جدول_ڳڻپ_2)
      | hb_map (&جدول_رڪارڊ::ٽيگ_2)
      | hb_sink (hb_array (جدول_ٽيگ_فهرست, *جدول_ڳڻپ_2))
      ;
    }
    أرجع tables.len;
  }
  منطقي ڳوليو_جدول_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *جدول_اشاريو) ثابت
  {
    ٽيگ t;
    t = ٽيگ_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    إذا (tables.len < 16)
      أرجع tables.lfind (t, جدول_اشاريو, HB_NOT_مليو_STORE, اشاريو::NOT_مليو_اشاريو);
    وإلا
      أرجع tables.bfind (t, جدول_اشاريو, HB_NOT_مليو_STORE, اشاريو::NOT_مليو_اشاريو);
  }
  ثابت جدول_رڪارڊ& get_جدول_by_ٽيگ (hb_ٽيگ_t ٽيگ_2) ثابت
  {
    بنانشان صحيح جدول_اشاريو;
    ڳوليو_جدول_اشاريو (ٽيگ_2, &جدول_اشاريو);
    أرجع get_جدول (جدول_اشاريو);
  }

  عام:

  سانچو <قسم_نالو Iterator,
	    hb_requires ((hb_is_ذريعو_جو<Iterator, hb_pair_t<hb_ٽيگ_t, hb_blob_t *>>::value))>
  منطقي serialize (hb_serialize_context_t *c,
		  hb_ٽيگ_t sfnt_ٽيگ,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    /* Alloc 12 for the OTHeader. */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_نسخو = sfnt_ٽيگ;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    بنانشان انگ_شيون = hb_len (it);
    إذا (unlikely (!tables.serialize (c, انگ_شيون))) return_trace (خطأ);

    ثابت حرف *dir_پڇاڙي = (ثابت حرف *) c->سر_2;
    HBUINT32 *checksum_adjustment = خالي_اشارو;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    بنانشان i = 0;
    لكل (hb_pair_t<hb_ٽيگ_t, hb_blob_t*> داخلا : it)
    {
      hb_blob_t *blob = داخلا.second;
      بنانشان len = blob->ڊيگهائي;

      /* Allocate room for the table and copy it. */
      حرف *شروعات = (حرف *) c->allocate_سائيز<فراغ> (len, خطأ);
      إذا (unlikely (!شروعات)) أرجع خطأ;

      جدول_رڪارڊ &rec = tables.arrayZ[i];
      rec.ٽيگ_2 = داخلا.پهريون;
      rec.ڊيگهائي = len;
      rec.وٿي = 0;
      إذا (unlikely (!c->check_سونپيو (rec.وٿي,
				      (بنانشان) ((حرف *) شروعات - (حرف *) هذا),
				      HB_SERIALIZE_ERROR_وٿي_حد_کان_مٿي)))
        return_trace (خطأ);

      إذا (likely (len))
	hb_memcpy (شروعات, blob->معلومات, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت حرف *پڇاڙي = (ثابت حرف *) c->سر_2;

      إذا (داخلا.پهريون == HB_OT_ٽيگ_سر &&
	  (بنانشان) (پڇاڙي - شروعات) >= سر_2::static_سائيز)
      {
	سر_2 *h = (سر_2 *) شروعات;
	checksum_adjustment = &h->check_جوڙ_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_جوڙ.سيٽ_for_معلومات (شروعات, پڇاڙي - شروعات);
      i++;
    }

    tables.qsort ();

    إذا (checksum_adjustment)
    {
      Check_جوڙ checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.سيٽ_for_معلومات (هذا, dir_پڇاڙي - (ثابت حرف *) هذا);
      لكل (بنانشان صحيح i = 0; i < انگ_شيون; i++)
      {
	جدول_رڪارڊ &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_جوڙ;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && tables.sanitize (c));
  }

  محمي:
  ٽيگ		sfnt_نسخو;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_ڳولا_Array_جو<جدول_رڪارڊ>
		tables;
  عام:
  DEFINE_سائيز_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

بنية TTCHeaderVersion1
{
  دوست بنية TTCHeader;

  استعمال DirectoryList = Array32_جو<Offset32_منزل<Open_Type_وٿي_جدول>>;

  بنانشان صحيح get_face_ڳڻپ () ثابت { أرجع get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (بنانشان صحيح i) ثابت
  { أرجع هذا+get_directory_list ()[i]; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.minor >= 1)
    {
      إذا (unlikely (!جدول.sanitize_shallow (c)))
	return_trace (خطأ);
      بنانشان extra_سائيز = نسخو.major == 2 ? 12 : 0;
      إذا (unlikely (!c->check_range (&جدول, جدول.get_سائيز () + extra_سائيز)))
	return_trace (خطأ);
      ثابت DirectoryList &table2 = Struct_At_وٿي<DirectoryList> (&جدول,
								   جدول.get_سائيز () + extra_سائيز);
      return_trace (table2.sanitize (c, هذا));
    }
#جيڪڏهن_پورو
    return_trace (جدول.sanitize (c, هذا));
  }

  خاص:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.minor >= 1)
    {
      بنانشان extra_سائيز = نسخو.major == 2 ? 12 : 0;
      أرجع Struct_At_وٿي<DirectoryList> (&جدول, جدول.get_سائيز () + extra_سائيز);
    }
#جيڪڏهن_پورو
    أرجع جدول;
  }

  محمي:
  ٽيگ		ttc_ٽيگ;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخو<>نسخو;	/* Version of the TTC Header. */
  DirectoryList
		جدول;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عام:
  DEFINE_سائيز_ARRAY (12, جدول);
};

بنية TTCHeader
{
  دوست بنية OpenTypeFontFile;

  خاص:

  بنانشان صحيح get_face_ڳڻپ () ثابت
  {
    بدل (u.سرو.نسخو.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face_ڳڻپ ();
    افتراضي:أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بنانشان صحيح i) ثابت
  {
    بدل (u.سرو.نسخو.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face (i);
    افتراضي:أرجع Null (OpenTypeFontFace);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.سرو.نسخو.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرو.نسخو.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    افتراضي:return_trace (سچ);
    }
  }

  محمي:
  اتحاد {
  بنية {
  ٽيگ		ttc_ٽيگ;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخو<>نسخو;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			سرو;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

بنية Resource_رڪارڊ
{
  ثابت OpenTypeFontFace & get_face (ثابت فراغ *معلومات_بنياد) ثابت
  { أرجع * reinterpret_cast<ثابت OpenTypeFontFace *> ((معلومات_بنياد+وٿي).arrayZ); }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *معلومات_بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  وٿي.sanitize (c, معلومات_بنياد) &&
		  hb_barrier () &&
		  get_face (معلومات_بنياد).sanitize (c));
  }

  محمي:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_وٿي;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_منزل<Array32_جو<HBUINT8>>
		وٿي;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عام:
  DEFINE_سائيز_STATIC (12);
};

#وصف_ڪريو HB_ٽيگ_sfnt HB_ٽيگ ('s','f','n','t')

بنية Resource_Type_رڪارڊ
{
  بنانشان صحيح get_resource_ڳڻپ () ثابت
  { أرجع ٽيگ_2 == HB_ٽيگ_sfnt ? res_ڳڻپ_M1 + 1 : 0; }

  منطقي is_sfnt () ثابت { أرجع ٽيگ_2 == HB_ٽيگ_sfnt; }

  ثابت Resource_رڪارڊ& get_resource_رڪارڊ (بنانشان صحيح i,
					     ثابت فراغ *type_بنياد) ثابت
  { أرجع (type_بنياد+resourcesZ).as_array (get_resource_ڳڻپ ())[i]; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *type_بنياد,
		 ثابت فراغ *معلومات_بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_بنياد,
				       get_resource_ڳڻپ (),
				       معلومات_بنياد));
  }

  محمي:
  ٽيگ		ٽيگ_2;		/* Resource type. */
  HBUINT16	res_ڳڻپ_M1;	/* Number of resources minus 1. */
  NNOffset16_منزل<Unsized_Array_جو<Resource_رڪارڊ>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية ResourceMap
{
  بنانشان صحيح get_face_ڳڻپ () ثابت
  {
    بنانشان صحيح ڳڻپ = get_type_ڳڻپ ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      ثابت Resource_Type_رڪارڊ& type = get_type_رڪارڊ (i);
      إذا (type.is_sfnt ())
	أرجع type.get_resource_ڳڻپ ();
    }
    أرجع 0;
  }

  ثابت OpenTypeFontFace& get_face (بنانشان صحيح idx,
				    ثابت فراغ *معلومات_بنياد) ثابت
  {
    بنانشان صحيح ڳڻپ = get_type_ڳڻپ ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      ثابت Resource_Type_رڪارڊ& type = get_type_رڪارڊ (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      إذا (type.is_sfnt () && idx < type.get_resource_ڳڻپ ())
	أرجع type.get_resource_رڪارڊ (idx, &(هذا+typeList)).get_face (معلومات_بنياد);
    }
    أرجع Null (OpenTypeFontFace);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *معلومات_بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  typeList.sanitize (c, هذا,
				     &(هذا+typeList),
				     معلومات_بنياد));
  }

  خاص:
  بنانشان صحيح get_type_ڳڻپ () ثابت { أرجع (هذا+typeList).lenM1 + 1; }

  ثابت Resource_Type_رڪارڊ& get_type_رڪارڊ (بنانشان صحيح i) ثابت
  { أرجع (هذا+typeList)[i]; }

  محمي:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_منزل<Array_جو_M1<Resource_Type_رڪارڊ>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عام:
  DEFINE_سائيز_STATIC (28);
};

بنية Resource_Fork_سرو
{
  بنانشان صحيح get_face_ڳڻپ () ثابت
  { أرجع (هذا+map).get_face_ڳڻپ (); }

  ثابت OpenTypeFontFace& get_face (بنانشان صحيح idx,
				    بنانشان صحيح *بنياد_وٿي = خالي_اشارو) ثابت
  {
    ثابت OpenTypeFontFace &face = (هذا+map).get_face (idx, &(هذا+معلومات));
    إذا (بنياد_وٿي)
      *بنياد_وٿي = (ثابت حرف *) &face - (ثابت حرف *) هذا;
    أرجع face;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  معلومات.sanitize (c, هذا, معلومات_Len) &&
		  map.sanitize (c, هذا, &(هذا+معلومات)));
  }

  محمي:
  NNOffset32_منزل<Unsized_Array_جو<HBUINT8>>
		معلومات;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_منزل<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	معلومات_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عام:
  DEFINE_سائيز_STATIC (16);
};

/*
 * OpenType Font File
 */

بنية OpenTypeFontFile
{
  تعداد {
    CFFTag		= HB_ٽيگ ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_ٽيگ		= HB_ٽيگ ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_ٽيگ ('t','t','c','f'), /* TrueType Collection */
    DFont_ٽيگ		= HB_ٽيگ ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_ٽيگ		= HB_ٽيگ ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_ٽيگ		= HB_ٽيگ ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_ٽيگ_t get_ٽيگ () ثابت { أرجع u.ٽيگ_2.v; }

  بنانشان صحيح get_face_ڳڻپ () ثابت
  {
    بدل (u.ٽيگ_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_ٽيگ:
    حالة Typ1_ٽيگ:
    حالة True_Type_ٽيگ:	أرجع 1;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرو.get_face_ڳڻپ ();
    حالة DFont_ٽيگ:	hb_barrier (); أرجع u.rf_سرو.get_face_ڳڻپ ();
    افتراضي:		أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بنانشان صحيح i, بنانشان صحيح *بنياد_وٿي = خالي_اشارو) ثابت
  {
    إذا (بنياد_وٿي)
      *بنياد_وٿي = 0;
    بدل (u.ٽيگ_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_ٽيگ:
    حالة Typ1_ٽيگ:
    حالة True_Type_ٽيگ:	hb_barrier (); أرجع u.fontFace;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرو.get_face (i);
    حالة DFont_ٽيگ:	hb_barrier (); أرجع u.rf_سرو.get_face (i, بنياد_وٿي);
    افتراضي:		أرجع Null (OpenTypeFontFace);
    }
  }

  سانچو <قسم_نالو Iterator,
	    hb_requires ((hb_is_ذريعو_جو<Iterator, hb_pair_t<hb_ٽيگ_t, hb_blob_t *>>::value))>
  منطقي serialize_single (hb_serialize_context_t *c,
			 hb_ٽيگ_t sfnt_ٽيگ,
			 Iterator شيون)
  {
    TRACE_SERIALIZE (هذا);
    أكد (sfnt_ٽيگ != TTCTag);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    return_trace (u.fontFace.serialize (c, sfnt_ٽيگ, شيون));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.ٽيگ_2.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.ٽيگ_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_ٽيگ:
    حالة Typ1_ٽيگ:
    حالة True_Type_ٽيگ:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالة TTCTag:	hb_barrier (); return_trace (u.ttc_سرو.sanitize (c));
    حالة DFont_ٽيگ:	hb_barrier (); return_trace (u.rf_سرو.sanitize (c));
    افتراضي:		return_trace (سچ);
    }
  }

  محمي:
  اتحاد {
  بنية { ٽيگ v; }	ٽيگ_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_سرو;
  Resource_Fork_سرو	rf_سرو;
  } u;
  عام:
  DEFINE_سائيز_UNION (4, ٽيگ_2.v);
};


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OPEN_FILE_HH */
