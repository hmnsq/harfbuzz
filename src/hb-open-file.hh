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

#جے_بنا_تعریف HB_OPEN_FILE_HH
#تعریف_کرو HB_OPEN_FILE_HH

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
بنية Open_Type_وتھ_جدول;
بنية TTCHeader;


تعريفنوع بنية جدول_ریکارڈ
{
  صحيح cmp (ٹیگ t) ثابت { أرجع -t.cmp (ٹیگ_2); }

  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت جدول_ریکارڈ *a = (ثابت جدول_ریکارڈ *) pa;
    ثابت جدول_ریکارڈ *b = (ثابت جدول_ریکارڈ *) pb;
    أرجع b->cmp (a->ٹیگ_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  ٹیگ		ٹیگ_2;		/* 4-byte identifier. */
  Check_جوڑ	check_جوڑ;	/* CheckSum for this table. */
  Offset32	وتھ;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	لمائی;		/* Length of this table. */
  عام:
  DEFINE_ناپ_STATIC (16);
} Open_Type_جدول;

تعريفنوع بنية Open_Type_وتھ_جدول
{
  یار بنية OpenTypeFontFile;

  بنانشانی صحيح get_جدول_گنتی () ثابت { أرجع tables.len; }
  ثابت جدول_ریکارڈ& get_جدول (بنانشانی صحيح i) ثابت
  { أرجع tables[i]; }
  بنانشانی صحيح get_جدول_ٹیگ_سوچی (بنانشانی صحيح  شروع_وتھ,
			       بنانشانی صحيح *جدول_گنتی_2, /* IN/OUT */
			       hb_ٹیگ_t     *جدول_ٹیگ_سوچی /*باہرلا*/) ثابت
  {
    إذا (جدول_گنتی_2 && جدول_ٹیگ_سوچی)
    {
      + tables.as_array ().sub_array (شروع_وتھ, جدول_گنتی_2)
      | hb_map (&جدول_ریکارڈ::ٹیگ_2)
      | hb_sink (hb_array (جدول_ٹیگ_سوچی, *جدول_گنتی_2))
      ;
    }
    أرجع tables.len;
  }
  منطقي لبھو_جدول_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *جدول_اشاریہ) ثابت
  {
    ٹیگ t;
    t = ٹیگ_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    إذا (tables.len < 16)
      أرجع tables.lfind (t, جدول_اشاریہ, HB_NOT_لبھیا_STORE, اشاریہ::NOT_لبھیا_اشاریہ);
    وإلا
      أرجع tables.bfind (t, جدول_اشاریہ, HB_NOT_لبھیا_STORE, اشاریہ::NOT_لبھیا_اشاریہ);
  }
  ثابت جدول_ریکارڈ& get_جدول_by_ٹیگ (hb_ٹیگ_t ٹیگ_2) ثابت
  {
    بنانشانی صحيح جدول_اشاریہ;
    لبھو_جدول_اشاریہ (ٹیگ_2, &جدول_اشاریہ);
    أرجع get_جدول (جدول_اشاریہ);
  }

  عام:

  سانچا <قسم_ناں Iterator,
	    hb_requires ((hb_is_سروت_دا<Iterator, hb_pair_t<hb_ٹیگ_t, hb_blob_t *>>::value))>
  منطقي serialize (hb_serialize_context_t *c,
		  hb_ٹیگ_t sfnt_ٹیگ,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    /* Alloc 12 for the OTHeader. */
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_نسخہ = sfnt_ٹیگ;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    بنانشانی عدد_اشیاء = hb_len (it);
    إذا (unlikely (!tables.serialize (c, عدد_اشیاء))) return_trace (خطأ);

    ثابت حرف *dir_سرا = (ثابت حرف *) c->سر_2;
    HBUINT32 *checksum_adjustment = خالی_سوچک;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    بنانشانی i = 0;
    لكل (hb_pair_t<hb_ٹیگ_t, hb_blob_t*> اندراج : it)
    {
      hb_blob_t *blob = اندراج.second;
      بنانشانی len = blob->لمائی;

      /* Allocate room for the table and copy it. */
      حرف *شروع = (حرف *) c->allocate_ناپ<فراغ> (len, خطأ);
      إذا (unlikely (!شروع)) أرجع خطأ;

      جدول_ریکارڈ &rec = tables.arrayZ[i];
      rec.ٹیگ_2 = اندراج.پہلا;
      rec.لمائی = len;
      rec.وتھ = 0;
      إذا (unlikely (!c->check_سونپو (rec.وتھ,
				      (بنانشانی) ((حرف *) شروع - (حرف *) هذا),
				      HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ)))
        return_trace (خطأ);

      إذا (likely (len))
	hb_memcpy (شروع, blob->انکڑے, len);

      /* 4-byte alignment. */
      c->align (4);
      ثابت حرف *سرا = (ثابت حرف *) c->سر_2;

      إذا (اندراج.پہلا == HB_OT_ٹیگ_سر &&
	  (بنانشانی) (سرا - شروع) >= سر_2::static_ناپ)
      {
	سر_2 *h = (سر_2 *) شروع;
	checksum_adjustment = &h->check_جوڑ_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_جوڑ.سیٹ_for_انکڑے (شروع, سرا - شروع);
      i++;
    }

    tables.qsort ();

    إذا (checksum_adjustment)
    {
      Check_جوڑ checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.سیٹ_for_انکڑے (هذا, dir_سرا - (ثابت حرف *) هذا);
      لكل (بنانشانی صحيح i = 0; i < عدد_اشیاء; i++)
      {
	جدول_ریکارڈ &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_جوڑ;
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
  ٹیگ		sfnt_نسخہ;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_تلاش_Array_دا<جدول_ریکارڈ>
		tables;
  عام:
  DEFINE_ناپ_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

بنية TTCHeaderVersion1
{
  یار بنية TTCHeader;

  ورتوں DirectoryList = Array32_دا<Offset32_منزل<Open_Type_وتھ_جدول>>;

  بنانشانی صحيح get_face_گنتی () ثابت { أرجع get_directory_list ().len; }
  ثابت OpenTypeFontFace& get_face (بنانشانی صحيح i) ثابت
  { أرجع هذا+get_directory_list ()[i]; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.minor >= 1)
    {
      إذا (unlikely (!جدول.sanitize_shallow (c)))
	return_trace (خطأ);
      بنانشانی extra_ناپ = نسخہ.major == 2 ? 12 : 0;
      إذا (unlikely (!c->check_range (&جدول, جدول.get_ناپ () + extra_ناپ)))
	return_trace (خطأ);
      ثابت DirectoryList &table2 = Struct_At_وتھ<DirectoryList> (&جدول,
								   جدول.get_ناپ () + extra_ناپ);
      return_trace (table2.sanitize (c, هذا));
    }
#جے_انت
    return_trace (جدول.sanitize (c, هذا));
  }

  خاص:
  ثابت DirectoryList& get_directory_list () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.minor >= 1)
    {
      بنانشانی extra_ناپ = نسخہ.major == 2 ? 12 : 0;
      أرجع Struct_At_وتھ<DirectoryList> (&جدول, جدول.get_ناپ () + extra_ناپ);
    }
#جے_انت
    أرجع جدول;
  }

  محمي:
  ٹیگ		ttc_ٹیگ;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخہ<>نسخہ;	/* Version of the TTC Header. */
  DirectoryList
		جدول;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  عام:
  DEFINE_ناپ_ARRAY (12, جدول);
};

بنية TTCHeader
{
  یار بنية OpenTypeFontFile;

  خاص:

  بنانشانی صحيح get_face_گنتی () ثابت
  {
    بدل (u.سرخی.نسخہ.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face_گنتی ();
    افتراضي:أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بنانشانی صحيح i) ثابت
  {
    بدل (u.سرخی.نسخہ.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); أرجع u.version1.get_face (i);
    افتراضي:أرجع Null (OpenTypeFontFace);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.سرخی.نسخہ.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرخی.نسخہ.major) {
    حالة 2: /* version 2 is compatible with version 1 */
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    افتراضي:return_trace (سچ);
    }
  }

  محمي:
  اتحاد {
  بنية {
  ٹیگ		ttc_ٹیگ;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_نسخہ<>نسخہ;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			سرخی;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

بنية Resource_ریکارڈ
{
  ثابت OpenTypeFontFace & get_face (ثابت فراغ *انکڑے_ادھار) ثابت
  { أرجع * reinterpret_cast<ثابت OpenTypeFontFace *> ((انکڑے_ادھار+وتھ).arrayZ); }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *انکڑے_ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  وتھ.sanitize (c, انکڑے_ادھار) &&
		  hb_barrier () &&
		  get_face (انکڑے_ادھار).sanitize (c));
  }

  محمي:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_وتھ;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_منزل<Array32_دا<HBUINT8>>
		وتھ;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  عام:
  DEFINE_ناپ_STATIC (12);
};

#تعریف_کرو HB_ٹیگ_sfnt HB_ٹیگ ('s','f','n','t')

بنية Resource_Type_ریکارڈ
{
  بنانشانی صحيح get_resource_گنتی () ثابت
  { أرجع ٹیگ_2 == HB_ٹیگ_sfnt ? res_گنتی_M1 + 1 : 0; }

  منطقي is_sfnt () ثابت { أرجع ٹیگ_2 == HB_ٹیگ_sfnt; }

  ثابت Resource_ریکارڈ& get_resource_ریکارڈ (بنانشانی صحيح i,
					     ثابت فراغ *type_ادھار) ثابت
  { أرجع (type_ادھار+resourcesZ).as_array (get_resource_گنتی ())[i]; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *type_ادھار,
		 ثابت فراغ *انکڑے_ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_ادھار,
				       get_resource_گنتی (),
				       انکڑے_ادھار));
  }

  محمي:
  ٹیگ		ٹیگ_2;		/* Resource type. */
  HBUINT16	res_گنتی_M1;	/* Number of resources minus 1. */
  NNOffset16_منزل<Unsized_Array_دا<Resource_ریکارڈ>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية ResourceMap
{
  بنانشانی صحيح get_face_گنتی () ثابت
  {
    بنانشانی صحيح گنتی = get_type_گنتی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      ثابت Resource_Type_ریکارڈ& type = get_type_ریکارڈ (i);
      إذا (type.is_sfnt ())
	أرجع type.get_resource_گنتی ();
    }
    أرجع 0;
  }

  ثابت OpenTypeFontFace& get_face (بنانشانی صحيح idx,
				    ثابت فراغ *انکڑے_ادھار) ثابت
  {
    بنانشانی صحيح گنتی = get_type_گنتی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      ثابت Resource_Type_ریکارڈ& type = get_type_ریکارڈ (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      إذا (type.is_sfnt () && idx < type.get_resource_گنتی ())
	أرجع type.get_resource_ریکارڈ (idx, &(هذا+typeList)).get_face (انکڑے_ادھار);
    }
    أرجع Null (OpenTypeFontFace);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *انکڑے_ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  typeList.sanitize (c, هذا,
				     &(هذا+typeList),
				     انکڑے_ادھار));
  }

  خاص:
  بنانشانی صحيح get_type_گنتی () ثابت { أرجع (هذا+typeList).lenM1 + 1; }

  ثابت Resource_Type_ریکارڈ& get_type_ریکارڈ (بنانشانی صحيح i) ثابت
  { أرجع (هذا+typeList)[i]; }

  محمي:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_منزل<Array_دا_M1<Resource_Type_ریکارڈ>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  عام:
  DEFINE_ناپ_STATIC (28);
};

بنية Resource_Fork_سرخی
{
  بنانشانی صحيح get_face_گنتی () ثابت
  { أرجع (هذا+map).get_face_گنتی (); }

  ثابت OpenTypeFontFace& get_face (بنانشانی صحيح idx,
				    بنانشانی صحيح *ادھار_وتھ = خالی_سوچک) ثابت
  {
    ثابت OpenTypeFontFace &face = (هذا+map).get_face (idx, &(هذا+انکڑے));
    إذا (ادھار_وتھ)
      *ادھار_وتھ = (ثابت حرف *) &face - (ثابت حرف *) هذا;
    أرجع face;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  انکڑے.sanitize (c, هذا, انکڑے_Len) &&
		  map.sanitize (c, هذا, &(هذا+انکڑے)));
  }

  محمي:
  NNOffset32_منزل<Unsized_Array_دا<HBUINT8>>
		انکڑے;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_منزل<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	انکڑے_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  عام:
  DEFINE_ناپ_STATIC (16);
};

/*
 * OpenType Font File
 */

بنية OpenTypeFontFile
{
  تعداد {
    CFFTag		= HB_ٹیگ ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_ٹیگ		= HB_ٹیگ ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_ٹیگ ('t','t','c','f'), /* TrueType Collection */
    DFont_ٹیگ		= HB_ٹیگ ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_ٹیگ		= HB_ٹیگ ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_ٹیگ		= HB_ٹیگ ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_ٹیگ_t get_ٹیگ () ثابت { أرجع u.ٹیگ_2.v; }

  بنانشانی صحيح get_face_گنتی () ثابت
  {
    بدل (u.ٹیگ_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_ٹیگ:
    حالة Typ1_ٹیگ:
    حالة True_Type_ٹیگ:	أرجع 1;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرخی.get_face_گنتی ();
    حالة DFont_ٹیگ:	hb_barrier (); أرجع u.rf_سرخی.get_face_گنتی ();
    افتراضي:		أرجع 0;
    }
  }
  ثابت OpenTypeFontFace& get_face (بنانشانی صحيح i, بنانشانی صحيح *ادھار_وتھ = خالی_سوچک) ثابت
  {
    إذا (ادھار_وتھ)
      *ادھار_وتھ = 0;
    بدل (u.ٹیگ_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_ٹیگ:
    حالة Typ1_ٹیگ:
    حالة True_Type_ٹیگ:	hb_barrier (); أرجع u.fontFace;
    حالة TTCTag:	hb_barrier (); أرجع u.ttc_سرخی.get_face (i);
    حالة DFont_ٹیگ:	hb_barrier (); أرجع u.rf_سرخی.get_face (i, ادھار_وتھ);
    افتراضي:		أرجع Null (OpenTypeFontFace);
    }
  }

  سانچا <قسم_ناں Iterator,
	    hb_requires ((hb_is_سروت_دا<Iterator, hb_pair_t<hb_ٹیگ_t, hb_blob_t *>>::value))>
  منطقي serialize_single (hb_serialize_context_t *c,
			 hb_ٹیگ_t sfnt_ٹیگ,
			 Iterator اشیاء)
  {
    TRACE_SERIALIZE (هذا);
    أكد (sfnt_ٹیگ != TTCTag);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    return_trace (u.fontFace.serialize (c, sfnt_ٹیگ, اشیاء));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.ٹیگ_2.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.ٹیگ_2.v) {
    حالة CFFTag:	/* All the non-collection tags */
    حالة True_ٹیگ:
    حالة Typ1_ٹیگ:
    حالة True_Type_ٹیگ:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    حالة TTCTag:	hb_barrier (); return_trace (u.ttc_سرخی.sanitize (c));
    حالة DFont_ٹیگ:	hb_barrier (); return_trace (u.rf_سرخی.sanitize (c));
    افتراضي:		return_trace (سچ);
    }
  }

  محمي:
  اتحاد {
  بنية { ٹیگ v; }	ٹیگ_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_سرخی;
  Resource_Fork_سرخی	rf_سرخی;
  } u;
  عام:
  DEFINE_ناپ_UNION (4, ٹیگ_2.v);
};


} /* namespace OT */


#جے_انت /* HB_OPEN_FILE_HH */
