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

#که_نه_تعریف_شوی HB_OPEN_FILE_HH
#تعریف_کړه HB_OPEN_FILE_HH

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
بنية Open_Type_دننه_واټن_جدول;
بنية TTCHeader;


تعريفنوع بنية جدول_ریکارډ
{
  صحيح cmp (نښان t) ثابت { أرجع -t.cmp (نښان_2); }

  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت جدول_ریکارډ *a = (ثابت جدول_ریکارډ *) pa;
    ثابت جدول_ریکارډ *b = (ثابت جدول_ریکارډ *) pb;
    أرجع b->cmp (a->نښان_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  نښان		نښان_2;		/* 4-byte identifier. */
  Check_مجموعه	check_مجموعه;	/* CheckSum for this table. */
  Offset32	دننه_واټن;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	اوږدتیا;		/* Length of this table. */
  عام:
  DEFINE_اندازه_STATIC (16);
} Open_Type_جدول;

تعريفنوع بنية Open_Type_دننه_واټن_جدول
{
  ملګری بنية OpenTypeFontFile;

  بینښه صحيح get_جدول_شمېر () ثابت { أرجع tables.len; }
  ثابت جدول_ریکارډ& get_جدول (بینښه صحيح i) ثابت
  { أرجع tables[i]; }
  بینښه صحيح get_جدول_نښانونه (بینښه صحيح  پیل_دننه_واټن,
			       بینښه صحيح *جدول_شمېر_2, /* IN/OUT */
			       hb_نښان_t     *جدول_نښانونه /*بهر*/) ثابت
  {
    إذا (جدول_شمېر_2 && جدول_نښانونه)
    {
      + tables.as_array ().sub_array (پیل_دننه_واټن, جدول_شمېر_2)
      | hb_map (&جدول_ریکارډ::نښان_2)
      | hb_sink (hb_array (جدول_نښانونه, *جدول_شمېر_2))
      ;
    }
    أرجع tables.len;
  }
  منطقي پیدا_کړه_جدول_شاخص (hb_نښان_t نښان_2, بینښه صحيح *جدول_شاخص) ثابت
  {
    نښان t;
    t = نښان_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    إذا (tables.len < 16)
      أرجع tables.lfind (t, جدول_شاخص, HB_NOT_وموندل_STORE, شاخص::NOT_وموندل_شاخص);
    وإلا
      أرجع tables.bfind (t, جدول_شاخص, HB_NOT_وموندل_STORE, شاخص::NOT_وموندل_شاخص);
  }
  ثابت جدول_ریکارډ& get_جدول_by_نښان (hb_نښان_t نښان_2) ثابت
  {
    بینښه صحيح جدول_شاخص;
    پیدا_کړه_جدول_شاخص (نښان_2, &جدول_شاخص);
    أرجع get_جدول (جدول_شاخص);
  }

  عام:

  کينډۍ <ډول_نوم Iterator,
	    hb_requires ((hb_is_سرچینه_د<Iterator, hb_pair_t<hb_نښان_t, hb_blob_t *>>::value))>
  منطقي serialize (hb_serialize_context_t *c,
		  hb_نښان_t sfnt_نښان,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    /* Alloc 12 for the OTHeader. */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_نسخه = sfnt_نښان;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    بینښه عدد_توکي = hb_len (it);
    إذا (unlikely (!tables.serialize (c, عدد_توکي))) return_trace (خطأ);

    ثابت حرف *dir_پای_ټکی = (ثابت حرف *) c->سر_2;
    HBUINT32 *checksum_adjustment = تش_اشاره;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    بینښه i = 0;
    لكل (hb_pair_t<hb_نښان_t, hb_blob_t*> ثبت : it)
    {
      hb_blob_t *blob = ثبت.second;
      بینښه len = blob->اوږدتیا;

      /* Allocate room for the table and copy it. */
      حرف *پیل = (حرف *) c->allocate_اندازه<فراغ> (len, خطأ);
      إذا (unlikely (!پیل)) أرجع خطأ;

      جدول_ریکارډ &rec = tables.arrayZ[i];
      rec.نښان_2 = ثبت.لومړی;
      rec.اوږدتیا = len;
      rec.دننه_واټن = 0;
      إذا (unlikely (!c->check_ټاکل (rec.دننه_واټن,
				      (بینښه) ((حرف *) پیل - (حرف *) هذا),
				      HB_SERIALIZE_ERROR_دننه_واټن_ډکېدنه)))
        return_trace (خطأ);

      إذا (likely (len))
	hb_memcpy (پیل, blob->معلومات, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت حرف *پای_ټکی = (ثابت حرف *) c->سر_2;

      إذا (ثبت.لومړی == HB_OT_نښان_سر &&
	  (بینښه) (پای_ټکی - پیل) >= سر_2::static_اندازه)
      {
	سر_2 *h = (سر_2 *) پیل;
	checksum_adjustment = &h->check_مجموعه_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_مجموعه.سېټ_for_معلومات (پیل, پای_ټکی - پیل);
      i++;
    }

    tables.qsort ();

    إذا (checksum_adjustment)
    {
      Check_مجموعه checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.سېټ_for_معلومات (هذا, dir_پای_ټکی - (ثابت حرف *) هذا);
      لكل (بینښه صحيح i = 0; i < عدد_توکي; i++)
      {
	جدول_ریکارډ &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_مجموعه;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && tables.sanitize (c));
  }

  محمي:
  نښان		sfnt_نسخه;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_پلټنه_Array_د<جدول_ریکارډ>
		tables;
  عام:
  DEFINE_اندازه_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

بنية TTCHeaderVersion1
{
  ملګری بنية TTCHeader;

  کارول DirectoryList = Array32_د<Offset32_منزل<Open_Type_دننه_واټن_جدول>>;

  بینښه صحيح get_face_شمېر () ثابت { أرجع get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (بینښه صحيح i) ثابت
  { أرجع هذا+get_directory_list ()[i]; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.minor >= 1)
    {
      إذا (unlikely (!جدول.sanitize_shallow (c)))
	return_trace (خطأ);
      بینښه extra_اندازه = نسخه.major == 2 ? 12 : 0;
      إذا (unlikely (!c->check_range (&جدول, جدول.get_اندازه () + extra_اندازه)))
	return_trace (خطأ);
      ثابت DirectoryList &table2 = Struct_At_دننه_واټن<DirectoryList> (&جدول,
								   جدول.get_اندازه () + extra_اندازه);
      return_trace (table2.sanitize (c, هذا));
    }
#د_که_پای
    return_trace (جدول.sanitize (c, هذا));
  }

  خاص:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.minor >= 1)
    {
      بینښه extra_اندازه = نسخه.major == 2 ? 12 : 0;
      أرجع Struct_At_دننه_واټن<DirectoryList> (&جدول, جدول.get_اندازه () + extra_اندازه);
    }
#د_که_پای
    أرجع جدول;
  }

  محمي:
  نښان		ttc_نښان;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخه<>نسخه;	/* Version of the TTC Header. */
  DirectoryList
		جدول;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عام:
  DEFINE_اندازه_ARRAY (12, جدول);
};

بنية TTCHeader
{
  ملګری بنية OpenTypeFontFile;

  خاص:

  بینښه صحيح get_face_شمېر () ثابت
  {
    بدل (u.سرلیک.نسخه.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face_شمېر ();
    افتراضي:أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بینښه صحيح i) ثابت
  {
    بدل (u.سرلیک.نسخه.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face (i);
    افتراضي:أرجع Null (OpenTypeFontFace);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.سرلیک.نسخه.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرلیک.نسخه.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    افتراضي:return_trace (ریښتیا);
    }
  }

  محمي:
  اتحاد {
  بنية {
  نښان		ttc_نښان;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخه<>نسخه;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			سرلیک;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

بنية Resource_ریکارډ
{
  ثابت OpenTypeFontFace & get_face (ثابت فراغ *معلومات_بنسټ) ثابت
  { أرجع * reinterpret_cast<ثابت OpenTypeFontFace *> ((معلومات_بنسټ+دننه_واټن).arrayZ); }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *معلومات_بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  دننه_واټن.sanitize (c, معلومات_بنسټ) &&
		  hb_barrier () &&
		  get_face (معلومات_بنسټ).sanitize (c));
  }

  محمي:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_دننه_واټن;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_منزل<Array32_د<HBUINT8>>
		دننه_واټن;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عام:
  DEFINE_اندازه_STATIC (12);
};

#تعریف_کړه HB_نښان_sfnt HB_نښان ('s','f','n','t')

بنية Resource_Type_ریکارډ
{
  بینښه صحيح get_resource_شمېر () ثابت
  { أرجع نښان_2 == HB_نښان_sfnt ? res_شمېر_M1 + 1 : 0; }

  منطقي is_sfnt () ثابت { أرجع نښان_2 == HB_نښان_sfnt; }

  ثابت Resource_ریکارډ& get_resource_ریکارډ (بینښه صحيح i,
					     ثابت فراغ *type_بنسټ) ثابت
  { أرجع (type_بنسټ+resourcesZ).as_array (get_resource_شمېر ())[i]; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *type_بنسټ,
		 ثابت فراغ *معلومات_بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_بنسټ,
				       get_resource_شمېر (),
				       معلومات_بنسټ));
  }

  محمي:
  نښان		نښان_2;		/* Resource type. */
  HBUINT16	res_شمېر_M1;	/* Number of resources minus 1. */
  NNOffset16_منزل<Unsized_Array_د<Resource_ریکارډ>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية ResourceMap
{
  بینښه صحيح get_face_شمېر () ثابت
  {
    بینښه صحيح شمېر = get_type_شمېر ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      ثابت Resource_Type_ریکارډ& type = get_type_ریکارډ (i);
      إذا (type.is_sfnt ())
	أرجع type.get_resource_شمېر ();
    }
    أرجع 0;
  }

  ثابت OpenTypeFontFace& get_face (بینښه صحيح idx,
				    ثابت فراغ *معلومات_بنسټ) ثابت
  {
    بینښه صحيح شمېر = get_type_شمېر ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      ثابت Resource_Type_ریکارډ& type = get_type_ریکارډ (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      إذا (type.is_sfnt () && idx < type.get_resource_شمېر ())
	أرجع type.get_resource_ریکارډ (idx, &(هذا+typeList)).get_face (معلومات_بنسټ);
    }
    أرجع Null (OpenTypeFontFace);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *معلومات_بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  typeList.sanitize (c, هذا,
				     &(هذا+typeList),
				     معلومات_بنسټ));
  }

  خاص:
  بینښه صحيح get_type_شمېر () ثابت { أرجع (هذا+typeList).lenM1 + 1; }

  ثابت Resource_Type_ریکارډ& get_type_ریکارډ (بینښه صحيح i) ثابت
  { أرجع (هذا+typeList)[i]; }

  محمي:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_منزل<Array_د_M1<Resource_Type_ریکارډ>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عام:
  DEFINE_اندازه_STATIC (28);
};

بنية Resource_Fork_سرلیک
{
  بینښه صحيح get_face_شمېر () ثابت
  { أرجع (هذا+map).get_face_شمېر (); }

  ثابت OpenTypeFontFace& get_face (بینښه صحيح idx,
				    بینښه صحيح *بنسټ_دننه_واټن = تش_اشاره) ثابت
  {
    ثابت OpenTypeFontFace &face = (هذا+map).get_face (idx, &(هذا+معلومات));
    إذا (بنسټ_دننه_واټن)
      *بنسټ_دننه_واټن = (ثابت حرف *) &face - (ثابت حرف *) هذا;
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
  NNOffset32_منزل<Unsized_Array_د<HBUINT8>>
		معلومات;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_منزل<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	معلومات_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عام:
  DEFINE_اندازه_STATIC (16);
};

/*
 * OpenType Font File
 */

بنية OpenTypeFontFile
{
  تعداد {
    CFFTag		= HB_نښان ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_نښان		= HB_نښان ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_نښان ('t','t','c','f'), /* TrueType Collection */
    DFont_نښان		= HB_نښان ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_نښان		= HB_نښان ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_نښان		= HB_نښان ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_نښان_t get_نښان () ثابت { أرجع u.نښان_2.v; }

  بینښه صحيح get_face_شمېر () ثابت
  {
    بدل (u.نښان_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_نښان:
    حالة Typ1_نښان:
    حالة True_Type_نښان:	أرجع 1;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرلیک.get_face_شمېر ();
    حالة DFont_نښان:	hb_barrier (); أرجع u.rf_سرلیک.get_face_شمېر ();
    افتراضي:		أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بینښه صحيح i, بینښه صحيح *بنسټ_دننه_واټن = تش_اشاره) ثابت
  {
    إذا (بنسټ_دننه_واټن)
      *بنسټ_دننه_واټن = 0;
    بدل (u.نښان_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_نښان:
    حالة Typ1_نښان:
    حالة True_Type_نښان:	hb_barrier (); أرجع u.fontFace;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرلیک.get_face (i);
    حالة DFont_نښان:	hb_barrier (); أرجع u.rf_سرلیک.get_face (i, بنسټ_دننه_واټن);
    افتراضي:		أرجع Null (OpenTypeFontFace);
    }
  }

  کينډۍ <ډول_نوم Iterator,
	    hb_requires ((hb_is_سرچینه_د<Iterator, hb_pair_t<hb_نښان_t, hb_blob_t *>>::value))>
  منطقي serialize_single (hb_serialize_context_t *c,
			 hb_نښان_t sfnt_نښان,
			 Iterator توکي)
  {
    TRACE_SERIALIZE (هذا);
    أكد (sfnt_نښان != TTCTag);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    return_trace (u.fontFace.serialize (c, sfnt_نښان, توکي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.نښان_2.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.نښان_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_نښان:
    حالة Typ1_نښان:
    حالة True_Type_نښان:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالة TTCTag:	hb_barrier (); return_trace (u.ttc_سرلیک.sanitize (c));
    حالة DFont_نښان:	hb_barrier (); return_trace (u.rf_سرلیک.sanitize (c));
    افتراضي:		return_trace (ریښتیا);
    }
  }

  محمي:
  اتحاد {
  بنية { نښان v; }	نښان_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_سرلیک;
  Resource_Fork_سرلیک	rf_سرلیک;
  } u;
  عام:
  DEFINE_اندازه_UNION (4, نښان_2.v);
};


} /* namespace OT */


#د_که_پای /* HB_OPEN_FILE_HH */
