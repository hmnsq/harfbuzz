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

#నిర్వచితం_కాకపోతే HB_OPEN_FILE_HH
#నిర్వచించు HB_OPEN_FILE_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-head-table.hh"


నామాకాశ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

నిర్మాణం OpenTypeFontFile;
నిర్మాణం Open_Type_స్థానభ్రంశం_పట్టిక;
నిర్మాణం TTCHeader;


రకనిర్వచనం నిర్మాణం పట్టిక_రికార్డు
{
  పూర్ణాంకం cmp (ట్యాగ్ t) స్థిరరాశి { తిరిగి -t.cmp (ట్యాగ్_2); }

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి పట్టిక_రికార్డు *a = (స్థిరరాశి పట్టిక_రికార్డు *) pa;
    స్థిరరాశి పట్టిక_రికార్డు *b = (స్థిరరాశి పట్టిక_రికార్డు *) pb;
    తిరిగి b->cmp (a->ట్యాగ్_2);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  ట్యాగ్		ట్యాగ్_2;		/* 4-byte identifier. */
  Check_కూడిక	check_కూడిక;	/* CheckSum for this table. */
  Offset32	స్థానభ్రంశం;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	నిడివి;		/* Length of this table. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (16);
} Open_Type_పట్టిక;

రకనిర్వచనం నిర్మాణం Open_Type_స్థానభ్రంశం_పట్టిక
{
  మిత్ర నిర్మాణం OpenTypeFontFile;

  గుర్తులేని పూర్ణాంకం get_పట్టిక_లెక్క () స్థిరరాశి { తిరిగి tables.len; }
  స్థిరరాశి పట్టిక_రికార్డు& get_పట్టిక (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి tables[i]; }
  గుర్తులేని పూర్ణాంకం get_పట్టిక_ట్యాగులు (గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2, /* IN/OUT */
			       hb_ట్యాగ్_t     *పట్టిక_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (పట్టిక_లెక్క_2 && పట్టిక_ట్యాగులు)
    {
      + tables.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, పట్టిక_లెక్క_2)
      | hb_map (&పట్టిక_రికార్డు::ట్యాగ్_2)
      | hb_sink (hb_array (పట్టిక_ట్యాగులు, *పట్టిక_లెక్క_2))
      ;
    }
    తిరిగి tables.len;
  }
  తార్కిక కనుగొను_పట్టిక_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *పట్టిక_సూచీ) స్థిరరాశి
  {
    ట్యాగ్ t;
    t = ట్యాగ్_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    ఒకవేళ (tables.len < 16)
      తిరిగి tables.lfind (t, పట్టిక_సూచీ, HB_NOT_దొరికింది_STORE, సూచీ::NOT_దొరికింది_సూచీ);
    లేకపోతే
      తిరిగి tables.bfind (t, పట్టిక_సూచీ, HB_NOT_దొరికింది_STORE, సూచీ::NOT_దొరికింది_సూచీ);
  }
  స్థిరరాశి పట్టిక_రికార్డు& get_పట్టిక_by_ట్యాగ్ (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ;
    కనుగొను_పట్టిక_సూచీ (ట్యాగ్_2, &పట్టిక_సూచీ);
    తిరిగి get_పట్టిక (పట్టిక_సూచీ);
  }

  బహిరంగ:

  మూస <రకం_పేరు Iterator,
	    hb_requires ((hb_is_ఆధారం_యొక్క<Iterator, hb_pair_t<hb_ట్యాగ్_t, hb_blob_t *>>::value))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  hb_ట్యాగ్_t sfnt_ట్యాగ్,
		  Iterator it)
  {
    TRACE_SERIALIZE (ఇది);
    /* Alloc 12 for the OTHeader. */
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_సంస్కరణ = sfnt_ట్యాగ్;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    గుర్తులేని అంకె_వస్తువులు = hb_len (it);
    ఒకవేళ (unlikely (!tables.serialize (c, అంకె_వస్తువులు))) return_trace (అబద్ధం);

    స్థిరరాశి అక్షరం *dir_చివర = (స్థిరరాశి అక్షరం *) c->తల_2;
    HBUINT32 *checksum_adjustment = శూన్యసూచిక;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    గుర్తులేని i = 0;
    కొరకు (hb_pair_t<hb_ట్యాగ్_t, hb_blob_t*> నమోదు_2 : it)
    {
      hb_blob_t *blob = నమోదు_2.second;
      గుర్తులేని len = blob->నిడివి;

      /* Allocate room for the table and copy it. */
      అక్షరం *ప్రారంభం = (అక్షరం *) c->allocate_కొలత<శూన్య> (len, అబద్ధం);
      ఒకవేళ (unlikely (!ప్రారంభం)) తిరిగి అబద్ధం;

      పట్టిక_రికార్డు &rec = tables.arrayZ[i];
      rec.ట్యాగ్_2 = నమోదు_2.మొదటి;
      rec.నిడివి = len;
      rec.స్థానభ్రంశం = 0;
      ఒకవేళ (unlikely (!c->check_కేటాయించు (rec.స్థానభ్రంశం,
				      (గుర్తులేని) ((అక్షరం *) ప్రారంభం - (అక్షరం *) ఇది),
				      HB_SERIALIZE_ERROR_స్థానభ్రంశం_అధికప్రవాహం)))
        return_trace (అబద్ధం);

      ఒకవేళ (likely (len))
	hb_memcpy (ప్రారంభం, blob->దత్తాంశం, len);

      /* 4-byte alignment. */
      c->align (4);
      స్థిరరాశి అక్షరం *చివర = (స్థిరరాశి అక్షరం *) c->తల_2;

      ఒకవేళ (నమోదు_2.మొదటి == HB_OT_ట్యాగ్_తల &&
	  (గుర్తులేని) (చివర - ప్రారంభం) >= తల_2::static_కొలత)
      {
	తల_2 *h = (తల_2 *) ప్రారంభం;
	checksum_adjustment = &h->check_కూడిక_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_కూడిక.సమితి_for_దత్తాంశం (ప్రారంభం, చివర - ప్రారంభం);
      i++;
    }

    tables.qsort ();

    ఒకవేళ (checksum_adjustment)
    {
      Check_కూడిక checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.సమితి_for_దత్తాంశం (ఇది, dir_చివర - (స్థిరరాశి అక్షరం *) ఇది);
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_వస్తువులు; i++)
      {
	పట్టిక_రికార్డు &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_కూడిక;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && tables.sanitize (c));
  }

  రక్షిత:
  ట్యాగ్		sfnt_సంస్కరణ;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_శోధన_Array_యొక్క<పట్టిక_రికార్డు>
		tables;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

నిర్మాణం TTCHeaderVersion1
{
  మిత్ర నిర్మాణం TTCHeader;

  ఉపయోగించి DirectoryList = Array32_యొక్క<Offset32_గమ్యం<Open_Type_స్థానభ్రంశం_పట్టిక>>;

  గుర్తులేని పూర్ణాంకం get_face_లెక్క () స్థిరరాశి { తిరిగి get_directory_list ().len; }
  స్థిరరాశి OpenTypeFontFace& get_face (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి ఇది+get_directory_list ()[i]; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.minor >= 1)
    {
      ఒకవేళ (unlikely (!పట్టిక.sanitize_shallow (c)))
	return_trace (అబద్ధం);
      గుర్తులేని extra_కొలత = సంస్కరణ.major == 2 ? 12 : 0;
      ఒకవేళ (unlikely (!c->check_range (&పట్టిక, పట్టిక.get_కొలత () + extra_కొలత)))
	return_trace (అబద్ధం);
      స్థిరరాశి DirectoryList &table2 = Struct_At_స్థానభ్రంశం<DirectoryList> (&పట్టిక,
								   పట్టిక.get_కొలత () + extra_కొలత);
      return_trace (table2.sanitize (c, ఇది));
    }
#అయితే_ముగింపు
    return_trace (పట్టిక.sanitize (c, ఇది));
  }

  అంతరంగ:
  స్థిరరాశి DirectoryList& get_directory_list () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.minor >= 1)
    {
      గుర్తులేని extra_కొలత = సంస్కరణ.major == 2 ? 12 : 0;
      తిరిగి Struct_At_స్థానభ్రంశం<DirectoryList> (&పట్టిక, పట్టిక.get_కొలత () + extra_కొలత);
    }
#అయితే_ముగింపు
    తిరిగి పట్టిక;
  }

  రక్షిత:
  ట్యాగ్		ttc_ట్యాగ్;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the TTC Header. */
  DirectoryList
		పట్టిక;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, పట్టిక);
};

నిర్మాణం TTCHeader
{
  మిత్ర నిర్మాణం OpenTypeFontFile;

  అంతరంగ:

  గుర్తులేని పూర్ణాంకం get_face_లెక్క () స్థిరరాశి
  {
    ఎంపిక (u.శీర్షిక.సంస్కరణ.major) {
    స్థితి 2: /* version 2 is compatible with version 1 */
    స్థితి 1: hb_barrier (); తిరిగి u.version1.get_face_లెక్క ();
    అప్రమేయ:తిరిగి 0;
    }
  }
  స్థిరరాశి OpenTypeFontFace& get_face (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    ఎంపిక (u.శీర్షిక.సంస్కరణ.major) {
    స్థితి 2: /* version 2 is compatible with version 1 */
    స్థితి 1: hb_barrier (); తిరిగి u.version1.get_face (i);
    అప్రమేయ:తిరిగి Null (OpenTypeFontFace);
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!u.శీర్షిక.సంస్కరణ.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.శీర్షిక.సంస్కరణ.major) {
    స్థితి 2: /* version 2 is compatible with version 1 */
    స్థితి 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం {
  ట్యాగ్		ttc_ట్యాగ్;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			శీర్షిక;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

నిర్మాణం Resource_రికార్డు
{
  స్థిరరాశి OpenTypeFontFace & get_face (స్థిరరాశి శూన్య *దత్తాంశం_పునాది) స్థిరరాశి
  { తిరిగి * reinterpret_cast<స్థిరరాశి OpenTypeFontFace *> ((దత్తాంశం_పునాది+స్థానభ్రంశం).arrayZ); }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి శూన్య *దత్తాంశం_పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  స్థానభ్రంశం.sanitize (c, దత్తాంశం_పునాది) &&
		  hb_barrier () &&
		  get_face (దత్తాంశం_పునాది).sanitize (c));
  }

  రక్షిత:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_స్థానభ్రంశం;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_గమ్యం<Array32_యొక్క<HBUINT8>>
		స్థానభ్రంశం;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

#నిర్వచించు HB_ట్యాగ్_sfnt HB_ట్యాగ్ ('s','f','n','t')

నిర్మాణం Resource_Type_రికార్డు
{
  గుర్తులేని పూర్ణాంకం get_resource_లెక్క () స్థిరరాశి
  { తిరిగి ట్యాగ్_2 == HB_ట్యాగ్_sfnt ? res_లెక్క_M1 + 1 : 0; }

  తార్కిక is_sfnt () స్థిరరాశి { తిరిగి ట్యాగ్_2 == HB_ట్యాగ్_sfnt; }

  స్థిరరాశి Resource_రికార్డు& get_resource_రికార్డు (గుర్తులేని పూర్ణాంకం i,
					     స్థిరరాశి శూన్య *type_పునాది) స్థిరరాశి
  { తిరిగి (type_పునాది+resourcesZ).as_array (get_resource_లెక్క ())[i]; }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి శూన్య *type_పునాది,
		 స్థిరరాశి శూన్య *దత్తాంశం_పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_పునాది,
				       get_resource_లెక్క (),
				       దత్తాంశం_పునాది));
  }

  రక్షిత:
  ట్యాగ్		ట్యాగ్_2;		/* Resource type. */
  HBUINT16	res_లెక్క_M1;	/* Number of resources minus 1. */
  NNOffset16_గమ్యం<Unsized_Array_యొక్క<Resource_రికార్డు>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం ResourceMap
{
  గుర్తులేని పూర్ణాంకం get_face_లెక్క () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = get_type_లెక్క ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి Resource_Type_రికార్డు& type = get_type_రికార్డు (i);
      ఒకవేళ (type.is_sfnt ())
	తిరిగి type.get_resource_లెక్క ();
    }
    తిరిగి 0;
  }

  స్థిరరాశి OpenTypeFontFace& get_face (గుర్తులేని పూర్ణాంకం idx,
				    స్థిరరాశి శూన్య *దత్తాంశం_పునాది) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = get_type_లెక్క ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి Resource_Type_రికార్డు& type = get_type_రికార్డు (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      ఒకవేళ (type.is_sfnt () && idx < type.get_resource_లెక్క ())
	తిరిగి type.get_resource_రికార్డు (idx, &(ఇది+typeList)).get_face (దత్తాంశం_పునాది);
    }
    తిరిగి Null (OpenTypeFontFace);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *దత్తాంశం_పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  typeList.sanitize (c, ఇది,
				     &(ఇది+typeList),
				     దత్తాంశం_పునాది));
  }

  అంతరంగ:
  గుర్తులేని పూర్ణాంకం get_type_లెక్క () స్థిరరాశి { తిరిగి (ఇది+typeList).lenM1 + 1; }

  స్థిరరాశి Resource_Type_రికార్డు& get_type_రికార్డు (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి (ఇది+typeList)[i]; }

  రక్షిత:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_గమ్యం<Array_యొక్క_M1<Resource_Type_రికార్డు>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  బహిరంగ:
  DEFINE_కొలత_STATIC (28);
};

నిర్మాణం Resource_Fork_శీర్షిక
{
  గుర్తులేని పూర్ణాంకం get_face_లెక్క () స్థిరరాశి
  { తిరిగి (ఇది+map).get_face_లెక్క (); }

  స్థిరరాశి OpenTypeFontFace& get_face (గుర్తులేని పూర్ణాంకం idx,
				    గుర్తులేని పూర్ణాంకం *పునాది_స్థానభ్రంశం = శూన్యసూచిక) స్థిరరాశి
  {
    స్థిరరాశి OpenTypeFontFace &face = (ఇది+map).get_face (idx, &(ఇది+దత్తాంశం));
    ఒకవేళ (పునాది_స్థానభ్రంశం)
      *పునాది_స్థానభ్రంశం = (స్థిరరాశి అక్షరం *) &face - (స్థిరరాశి అక్షరం *) ఇది;
    తిరిగి face;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  దత్తాంశం.sanitize (c, ఇది, దత్తాంశం_Len) &&
		  map.sanitize (c, ఇది, &(ఇది+దత్తాంశం)));
  }

  రక్షిత:
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>
		దత్తాంశం;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_గమ్యం<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	దత్తాంశం_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  బహిరంగ:
  DEFINE_కొలత_STATIC (16);
};

/*
 * OpenType Font File
 */

నిర్మాణం OpenTypeFontFile
{
  గణన {
    CFFTag		= HB_ట్యాగ్ ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_ట్యాగ్		= HB_ట్యాగ్ ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_ట్యాగ్ ('t','t','c','f'), /* TrueType Collection */
    DFont_ట్యాగ్		= HB_ట్యాగ్ ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_ట్యాగ్		= HB_ట్యాగ్ ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_ట్యాగ్		= HB_ట్యాగ్ ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_ట్యాగ్_t get_ట్యాగ్ () స్థిరరాశి { తిరిగి u.ట్యాగ్_2.v; }

  గుర్తులేని పూర్ణాంకం get_face_లెక్క () స్థిరరాశి
  {
    ఎంపిక (u.ట్యాగ్_2.v) {
    స్థితి CFFTag:	/* All the non-collection tags */
    స్థితి True_ట్యాగ్:
    స్థితి Typ1_ట్యాగ్:
    స్థితి True_Type_ట్యాగ్:	తిరిగి 1;
    స్థితి TTCTag:	hb_barrier (); తిరిగి u.ttc_శీర్షిక.get_face_లెక్క ();
    స్థితి DFont_ట్యాగ్:	hb_barrier (); తిరిగి u.rf_శీర్షిక.get_face_లెక్క ();
    అప్రమేయ:		తిరిగి 0;
    }
  }
  స్థిరరాశి OpenTypeFontFace& get_face (గుర్తులేని పూర్ణాంకం i, గుర్తులేని పూర్ణాంకం *పునాది_స్థానభ్రంశం = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (పునాది_స్థానభ్రంశం)
      *పునాది_స్థానభ్రంశం = 0;
    ఎంపిక (u.ట్యాగ్_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    స్థితి CFFTag:	/* All the non-collection tags */
    స్థితి True_ట్యాగ్:
    స్థితి Typ1_ట్యాగ్:
    స్థితి True_Type_ట్యాగ్:	hb_barrier (); తిరిగి u.fontFace;
    స్థితి TTCTag:	hb_barrier (); తిరిగి u.ttc_శీర్షిక.get_face (i);
    స్థితి DFont_ట్యాగ్:	hb_barrier (); తిరిగి u.rf_శీర్షిక.get_face (i, పునాది_స్థానభ్రంశం);
    అప్రమేయ:		తిరిగి Null (OpenTypeFontFace);
    }
  }

  మూస <రకం_పేరు Iterator,
	    hb_requires ((hb_is_ఆధారం_యొక్క<Iterator, hb_pair_t<hb_ట్యాగ్_t, hb_blob_t *>>::value))>
  తార్కిక serialize_single (hb_serialize_context_t *c,
			 hb_ట్యాగ్_t sfnt_ట్యాగ్,
			 Iterator వస్తువులు)
  {
    TRACE_SERIALIZE (ఇది);
    ప్రతిజ్ఞా (sfnt_ట్యాగ్ != TTCTag);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    return_trace (u.fontFace.serialize (c, sfnt_ట్యాగ్, వస్తువులు));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!u.ట్యాగ్_2.v.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.ట్యాగ్_2.v) {
    స్థితి CFFTag:	/* All the non-collection tags */
    స్థితి True_ట్యాగ్:
    స్థితి Typ1_ట్యాగ్:
    స్థితి True_Type_ట్యాగ్:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    స్థితి TTCTag:	hb_barrier (); return_trace (u.ttc_శీర్షిక.sanitize (c));
    స్థితి DFont_ట్యాగ్:	hb_barrier (); return_trace (u.rf_శీర్షిక.sanitize (c));
    అప్రమేయ:		return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { ట్యాగ్ v; }	ట్యాగ్_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_శీర్షిక;
  Resource_Fork_శీర్షిక	rf_శీర్షిక;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (4, ట్యాగ్_2.v);
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OPEN_FILE_HH */
