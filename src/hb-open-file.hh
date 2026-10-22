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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPEN_FILE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPEN_FILE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-head-table.hh"


ਨਾਮਾਕਾਸ਼ OT {

/*
 *
 * The OpenType Font File
 *
 */


/*
 * Organization of an OpenType Font
 */

ਬਣਤਰ OpenTypeFontFile;
ਬਣਤਰ Open_Type_ਵਿੱਥ_ਸਾਰਣੀ;
ਬਣਤਰ TTCHeader;


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਸਾਰਣੀ_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (ਟੈਗ t) ਅਚਲ { ਵਾਪਸ -t.cmp (ਟੈਗ_2); }

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ ਸਾਰਣੀ_ਰਿਕਾਰਡ *a = (ਅਚਲ ਸਾਰਣੀ_ਰਿਕਾਰਡ *) pa;
    ਅਚਲ ਸਾਰਣੀ_ਰਿਕਾਰਡ *b = (ਅਚਲ ਸਾਰਣੀ_ਰਿਕਾਰਡ *) pb;
    ਵਾਪਸ b->cmp (a->ਟੈਗ_2);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਟੈਗ		ਟੈਗ_2;		/* 4-byte identifier. */
  Check_ਜੋੜ	check_ਜੋੜ;	/* CheckSum for this table. */
  Offset32	ਵਿੱਥ;		/* Offset from beginning of TrueType font
				 * file. */
  HBUINT32	ਲੰਮਾਈ;		/* Length of this table. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (16);
} Open_Type_ਸਾਰਣੀ;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ Open_Type_ਵਿੱਥ_ਸਾਰਣੀ
{
  ਮਿੱਤਰ ਬਣਤਰ OpenTypeFontFile;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਸਾਰਣੀ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ tables.len; }
  ਅਚਲ ਸਾਰਣੀ_ਰਿਕਾਰਡ& get_ਸਾਰਣੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ tables[i]; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2, /* IN/OUT */
			       hb_ਟੈਗ_t     *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (ਸਾਰਣੀ_ਗਿਣਤੀ_2 && ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ)
    {
      + tables.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਸਾਰਣੀ_ਗਿਣਤੀ_2)
      | hb_map (&ਸਾਰਣੀ_ਰਿਕਾਰਡ::ਟੈਗ_2)
      | hb_sink (hb_array (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ, *ਸਾਰਣੀ_ਗਿਣਤੀ_2))
      ;
    }
    ਵਾਪਸ tables.len;
  }
  ਤਾਰਕਿਕ ਲੱਭੋ_ਸਾਰਣੀ_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਟੈਗ t;
    t = ਟੈਗ_2;
    /* Use lfind for small fonts; there are fonts that have unsorted table entries;
     * those tend to work in other tools, so tolerate them.
     * https://github.com/harfbuzz/harfbuzz/issues/3065 */
    ਜੇ (tables.len < 16)
      ਵਾਪਸ tables.lfind (t, ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, HB_NOT_ਮਿਲਿਆ_STORE, ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ);
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ tables.bfind (t, ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, HB_NOT_ਮਿਲਿਆ_STORE, ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ);
  }
  ਅਚਲ ਸਾਰਣੀ_ਰਿਕਾਰਡ& get_ਸਾਰਣੀ_by_ਟੈਗ (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ;
    ਲੱਭੋ_ਸਾਰਣੀ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, &ਸਾਰਣੀ_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ get_ਸਾਰਣੀ (ਸਾਰਣੀ_ਸੂਚਕਾਂਕ);
  }

  ਜਨਤਕ:

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires ((hb_is_ਸਰੋਤ_ਦਾ<Iterator, hb_pair_t<hb_ਟੈਗ_t, hb_blob_t *>>::value))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  hb_ਟੈਗ_t sfnt_ਟੈਗ,
		  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    /* Alloc 12 for the OTHeader. */
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    /* Write sfntVersion (bytes 0..3). */
    sfnt_ਸੰਸਕਰਣ = sfnt_ਟੈਗ;
    /* Take space for numTables, searchRange, entrySelector, RangeShift
     * and the TableRecords themselves.  */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਚੀਜ਼ਾਂ = hb_len (it);
    ਜੇ (unlikely (!tables.serialize (c, ਅੰਕ_ਚੀਜ਼ਾਂ))) return_trace (ਝੂਠ);

    ਅਚਲ ਅੱਖਰ *dir_ਸਿਰਾ = (ਅਚਲ ਅੱਖਰ *) c->ਸਿਰ_2;
    HBUINT32 *checksum_adjustment = ਖਾਲੀ_ਸੂਚਕ;

    /* Write OffsetTables, alloc for and write actual table blobs. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਲਈ (hb_pair_t<hb_ਟੈਗ_t, hb_blob_t*> ਇੰਦਰਾਜ : it)
    {
      hb_blob_t *blob = ਇੰਦਰਾਜ.second;
      ਬਿਨਾਨਿਸ਼ਾਨੀ len = blob->ਲੰਮਾਈ;

      /* Allocate room for the table and copy it. */
      ਅੱਖਰ *ਸ਼ੁਰੂ = (ਅੱਖਰ *) c->allocate_ਮਾਪ<ਖਾਲੀ> (len, ਝੂਠ);
      ਜੇ (unlikely (!ਸ਼ੁਰੂ)) ਵਾਪਸ ਝੂਠ;

      ਸਾਰਣੀ_ਰਿਕਾਰਡ &rec = tables.arrayZ[i];
      rec.ਟੈਗ_2 = ਇੰਦਰਾਜ.ਪਹਿਲਾ;
      rec.ਲੰਮਾਈ = len;
      rec.ਵਿੱਥ = 0;
      ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (rec.ਵਿੱਥ,
				      (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅੱਖਰ *) ਸ਼ੁਰੂ - (ਅੱਖਰ *) ਇਹ),
				      HB_SERIALIZE_ERROR_ਵਿੱਥ_ਉਛਾਲ)))
        return_trace (ਝੂਠ);

      ਜੇ (likely (len))
	hb_memcpy (ਸ਼ੁਰੂ, blob->ਅੰਕੜੇ, len);

      /* 4-byte alignment. */
      c->align (4);
      ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = (ਅਚਲ ਅੱਖਰ *) c->ਸਿਰ_2;

      ਜੇ (ਇੰਦਰਾਜ.ਪਹਿਲਾ == HB_OT_ਟੈਗ_ਸਿਰ &&
	  (ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਸਿਰਾ - ਸ਼ੁਰੂ) >= ਸਿਰ_2::static_ਮਾਪ)
      {
	ਸਿਰ_2 *h = (ਸਿਰ_2 *) ਸ਼ੁਰੂ;
	checksum_adjustment = &h->check_ਜੋੜ_Adjustment;
	*checksum_adjustment = 0;
      }

      rec.check_ਜੋੜ.ਸੈੱਟ_for_ਅੰਕੜੇ (ਸ਼ੁਰੂ, ਸਿਰਾ - ਸ਼ੁਰੂ);
      i++;
    }

    tables.qsort ();

    ਜੇ (checksum_adjustment)
    {
      Check_ਜੋੜ checksum;

      /* The following line is a slower version of the following block. */
      //checksum.set_for_data (this, (const char *) c->head - (const char *) this);
      checksum.ਸੈੱਟ_for_ਅੰਕੜੇ (ਇਹ, dir_ਸਿਰਾ - (ਅਚਲ ਅੱਖਰ *) ਇਹ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ਚੀਜ਼ਾਂ; i++)
      {
	ਸਾਰਣੀ_ਰਿਕਾਰਡ &rec = tables.arrayZ[i];
	checksum = checksum + rec.check_ਜੋੜ;
      }

      *checksum_adjustment = 0xB1B0AFBAu - checksum;
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && tables.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		sfnt_ਸੰਸਕਰਣ;	/* '\0\001\0\00' if TrueType / 'OTTO' if CFF */
  Bin_ਖੋਜ_Array_ਦਾ<ਸਾਰਣੀ_ਰਿਕਾਰਡ>
		tables;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, tables);
} OpenTypeFontFace;


/*
 * TrueType Collections
 */

ਬਣਤਰ TTCHeaderVersion1
{
  ਮਿੱਤਰ ਬਣਤਰ TTCHeader;

  ਵਰਤੋਂ DirectoryList = Array32_ਦਾ<Offset32_ਮੰਜ਼ਿਲ<Open_Type_ਵਿੱਥ_ਸਾਰਣੀ>>;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_face_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ get_directory_list ().len; }
  ਅਚਲ OpenTypeFontFace& get_face (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ ਇਹ+get_directory_list ()[i]; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.minor >= 1)
    {
      ਜੇ (unlikely (!ਸਾਰਣੀ.sanitize_shallow (c)))
	return_trace (ਝੂਠ);
      ਬਿਨਾਨਿਸ਼ਾਨੀ extra_ਮਾਪ = ਸੰਸਕਰਣ.major == 2 ? 12 : 0;
      ਜੇ (unlikely (!c->check_range (&ਸਾਰਣੀ, ਸਾਰਣੀ.get_ਮਾਪ () + extra_ਮਾਪ)))
	return_trace (ਝੂਠ);
      ਅਚਲ DirectoryList &table2 = Struct_At_ਵਿੱਥ<DirectoryList> (&ਸਾਰਣੀ,
								   ਸਾਰਣੀ.get_ਮਾਪ () + extra_ਮਾਪ);
      return_trace (table2.sanitize (c, ਇਹ));
    }
#ਜੇ_ਅੰਤ
    return_trace (ਸਾਰਣੀ.sanitize (c, ਇਹ));
  }

  ਨਿਜੀ:
  ਅਚਲ DirectoryList& get_directory_list () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਸੰਸਕਰਣ.minor >= 1)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ extra_ਮਾਪ = ਸੰਸਕਰਣ.major == 2 ? 12 : 0;
      ਵਾਪਸ Struct_At_ਵਿੱਥ<DirectoryList> (&ਸਾਰਣੀ, ਸਾਰਣੀ.get_ਮਾਪ () + extra_ਮਾਪ);
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਸਾਰਣੀ;
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ttc_ਟੈਗ;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the TTC Header. */
  DirectoryList
		ਸਾਰਣੀ;		/* Array of offsets to the OffsetTable for each font
				 * from the beginning of the file */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, ਸਾਰਣੀ);
};

ਬਣਤਰ TTCHeader
{
  ਮਿੱਤਰ ਬਣਤਰ OpenTypeFontFile;

  ਨਿਜੀ:

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_face_ਗਿਣਤੀ () ਅਚਲ
  {
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੰਸਕਰਣ.major) {
    ਹਾਲਤ 2: /* version 2 is compatible with version 1 */
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.version1.get_face_ਗਿਣਤੀ ();
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }
  ਅਚਲ OpenTypeFontFace& get_face (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੰਸਕਰਣ.major) {
    ਹਾਲਤ 2: /* version 2 is compatible with version 1 */
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.version1.get_face (i);
    ਮੂਲ:ਵਾਪਸ Null (OpenTypeFontFace);
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!u.ਸਿਰਲੇਖ.ਸੰਸਕਰਣ.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੰਸਕਰਣ.major) {
    ਹਾਲਤ 2: /* version 2 is compatible with version 1 */
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ {
  ਟੈਗ		ttc_ਟੈਗ;		/* TrueType Collection ID string: 'ttcf' */
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the TTC Header (1.0 or 2.0),
				 * 0x00010000u or 0x00020000u */
  }			ਸਿਰਲੇਖ;
  TTCHeaderVersion1	version1;
  } u;
};

/*
 * Mac Resource Fork
 *
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/MoreToolbox/MoreToolbox-99.html
 */

ਬਣਤਰ Resource_ਰਿਕਾਰਡ
{
  ਅਚਲ OpenTypeFontFace & get_face (ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_ਅਧਾਰ) ਅਚਲ
  { ਵਾਪਸ * reinterpret_cast<ਅਚਲ OpenTypeFontFace *> ((ਅੰਕੜੇ_ਅਧਾਰ+ਵਿੱਥ).arrayZ); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  ਵਿੱਥ.sanitize (c, ਅੰਕੜੇ_ਅਧਾਰ) &&
		  hb_barrier () &&
		  get_face (ਅੰਕੜੇ_ਅਧਾਰ).sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	id;		/* Resource ID. */
  HBINT16	name_ਵਿੱਥ;	/* Offset from beginning of resource name list
				 * to resource name, -1 means there is none. */
  HBUINT8	attrs;		/* Resource attributes */
  NNOffset24_ਮੰਜ਼ਿਲ<Array32_ਦਾ<HBUINT8>>
		ਵਿੱਥ;		/* Offset from beginning of data block to
				 * data for this resource */
  HBUINT32	reserved;	/* Reserved for handle to resource */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਟੈਗ_sfnt HB_ਟੈਗ ('s','f','n','t')

ਬਣਤਰ Resource_Type_ਰਿਕਾਰਡ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_resource_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ ਟੈਗ_2 == HB_ਟੈਗ_sfnt ? res_ਗਿਣਤੀ_M1 + 1 : 0; }

  ਤਾਰਕਿਕ is_sfnt () ਅਚਲ { ਵਾਪਸ ਟੈਗ_2 == HB_ਟੈਗ_sfnt; }

  ਅਚਲ Resource_ਰਿਕਾਰਡ& get_resource_ਰਿਕਾਰਡ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,
					     ਅਚਲ ਖਾਲੀ *type_ਅਧਾਰ) ਅਚਲ
  { ਵਾਪਸ (type_ਅਧਾਰ+resourcesZ).as_array (get_resource_ਗਿਣਤੀ ())[i]; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਖਾਲੀ *type_ਅਧਾਰ,
		 ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  resourcesZ.sanitize (c, type_ਅਧਾਰ,
				       get_resource_ਗਿਣਤੀ (),
				       ਅੰਕੜੇ_ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ਟੈਗ_2;		/* Resource type. */
  HBUINT16	res_ਗਿਣਤੀ_M1;	/* Number of resources minus 1. */
  NNOffset16_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<Resource_ਰਿਕਾਰਡ>>
		resourcesZ;	/* Offset from beginning of resource type list
				 * to reference item list for this type. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ ResourceMap
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_face_ਗਿਣਤੀ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = get_type_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ Resource_Type_ਰਿਕਾਰਡ& type = get_type_ਰਿਕਾਰਡ (i);
      ਜੇ (type.is_sfnt ())
	ਵਾਪਸ type.get_resource_ਗਿਣਤੀ ();
    }
    ਵਾਪਸ 0;
  }

  ਅਚਲ OpenTypeFontFace& get_face (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx,
				    ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_ਅਧਾਰ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = get_type_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ Resource_Type_ਰਿਕਾਰਡ& type = get_type_ਰਿਕਾਰਡ (i);
      /* The check for idx < count is here because ResourceRecord is NOT null-safe.
       * Because an offset of 0 there does NOT mean null. */
      ਜੇ (type.is_sfnt () && idx < type.get_resource_ਗਿਣਤੀ ())
	ਵਾਪਸ type.get_resource_ਰਿਕਾਰਡ (idx, &(ਇਹ+typeList)).get_face (ਅੰਕੜੇ_ਅਧਾਰ);
    }
    ਵਾਪਸ Null (OpenTypeFontFace);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  typeList.sanitize (c, ਇਹ,
				     &(ਇਹ+typeList),
				     ਅੰਕੜੇ_ਅਧਾਰ));
  }

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ (ਇਹ+typeList).lenM1 + 1; }

  ਅਚਲ Resource_Type_ਰਿਕਾਰਡ& get_type_ਰਿਕਾਰਡ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+typeList)[i]; }

  ਸੁਰੱਖਿਅਤ:
  HBUINT8	reserved0[16];	/* Reserved for copy of resource header */
  HBUINT32	reserved1;	/* Reserved for handle to next resource map */
  HBUINT16	resreved2;	/* Reserved for file reference number */
  HBUINT16	attrs;		/* Resource fork attribute */
  NNOffset16_ਮੰਜ਼ਿਲ<Array_ਦਾ_M1<Resource_Type_ਰਿਕਾਰਡ>>
		typeList;	/* Offset from beginning of map to
				 * resource type list */
  Offset16	nameList;	/* Offset from beginning of map to
				 * resource name list */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (28);
};

ਬਣਤਰ Resource_Fork_ਸਿਰਲੇਖ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_face_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ (ਇਹ+map).get_face_ਗਿਣਤੀ (); }

  ਅਚਲ OpenTypeFontFace& get_face (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅਧਾਰ_ਵਿੱਥ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਅਚਲ OpenTypeFontFace &face = (ਇਹ+map).get_face (idx, &(ਇਹ+ਅੰਕੜੇ));
    ਜੇ (ਅਧਾਰ_ਵਿੱਥ)
      *ਅਧਾਰ_ਵਿੱਥ = (ਅਚਲ ਅੱਖਰ *) &face - (ਅਚਲ ਅੱਖਰ *) ਇਹ;
    ਵਾਪਸ face;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਅੰਕੜੇ.sanitize (c, ਇਹ, ਅੰਕੜੇ_Len) &&
		  map.sanitize (c, ਇਹ, &(ਇਹ+ਅੰਕੜੇ)));
  }

  ਸੁਰੱਖਿਅਤ:
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>
		ਅੰਕੜੇ;		/* Offset from beginning of resource fork
				 * to resource data */
  NNOffset32_ਮੰਜ਼ਿਲ<ResourceMap >
		map;		/* Offset from beginning of resource fork
				 * to resource map */
  HBUINT32	ਅੰਕੜੇ_Len;	/* Length of resource data */
  HBUINT32	mapLen;		/* Length of resource map */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (16);
};

/*
 * OpenType Font File
 */

ਬਣਤਰ OpenTypeFontFile
{
  ਗਿਣਤੀ {
    CFFTag		= HB_ਟੈਗ ('O','T','T','O'), /* OpenType with Postscript outlines */
    True_Type_ਟੈਗ		= HB_ਟੈਗ ( 0 , 1 , 0 , 0 ), /* OpenType with TrueType outlines */
    TTCTag		= HB_ਟੈਗ ('t','t','c','f'), /* TrueType Collection */
    DFont_ਟੈਗ		= HB_ਟੈਗ ( 0 , 0 , 1 , 0 ), /* DFont Mac Resource Fork */
    True_ਟੈਗ		= HB_ਟੈਗ ('t','r','u','e'), /* Obsolete Apple TrueType */
    Typ1_ਟੈਗ		= HB_ਟੈਗ ('t','y','p','1')  /* Obsolete Apple Type1 font in SFNT container */
  };

  hb_ਟੈਗ_t get_ਟੈਗ () ਅਚਲ { ਵਾਪਸ u.ਟੈਗ_2.v; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_face_ਗਿਣਤੀ () ਅਚਲ
  {
    ਚੋਣ (u.ਟੈਗ_2.v) {
    ਹਾਲਤ CFFTag:	/* All the non-collection tags */
    ਹਾਲਤ True_ਟੈਗ:
    ਹਾਲਤ Typ1_ਟੈਗ:
    ਹਾਲਤ True_Type_ਟੈਗ:	ਵਾਪਸ 1;
    ਹਾਲਤ TTCTag:	hb_barrier (); ਵਾਪਸ u.ttc_ਸਿਰਲੇਖ.get_face_ਗਿਣਤੀ ();
    ਹਾਲਤ DFont_ਟੈਗ:	hb_barrier (); ਵਾਪਸ u.rf_ਸਿਰਲੇਖ.get_face_ਗਿਣਤੀ ();
    ਮੂਲ:		ਵਾਪਸ 0;
    }
  }
  ਅਚਲ OpenTypeFontFace& get_face (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅਧਾਰ_ਵਿੱਥ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (ਅਧਾਰ_ਵਿੱਥ)
      *ਅਧਾਰ_ਵਿੱਥ = 0;
    ਚੋਣ (u.ਟੈਗ_2.v) {
    /* Note: for non-collection SFNT data we ignore index.  This is because
     * Apple dfont container is a container of SFNT's.  So each SFNT is a
     * non-TTC, but the index is more than zero. */
    ਹਾਲਤ CFFTag:	/* All the non-collection tags */
    ਹਾਲਤ True_ਟੈਗ:
    ਹਾਲਤ Typ1_ਟੈਗ:
    ਹਾਲਤ True_Type_ਟੈਗ:	hb_barrier (); ਵਾਪਸ u.fontFace;
    ਹਾਲਤ TTCTag:	hb_barrier (); ਵਾਪਸ u.ttc_ਸਿਰਲੇਖ.get_face (i);
    ਹਾਲਤ DFont_ਟੈਗ:	hb_barrier (); ਵਾਪਸ u.rf_ਸਿਰਲੇਖ.get_face (i, ਅਧਾਰ_ਵਿੱਥ);
    ਮੂਲ:		ਵਾਪਸ Null (OpenTypeFontFace);
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires ((hb_is_ਸਰੋਤ_ਦਾ<Iterator, hb_pair_t<hb_ਟੈਗ_t, hb_blob_t *>>::value))>
  ਤਾਰਕਿਕ serialize_single (hb_serialize_context_t *c,
			 hb_ਟੈਗ_t sfnt_ਟੈਗ,
			 Iterator ਚੀਜ਼ਾਂ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਪ੍ਰਤਿਜ੍ਞਾ (sfnt_ਟੈਗ != TTCTag);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    return_trace (u.fontFace.serialize (c, sfnt_ਟੈਗ, ਚੀਜ਼ਾਂ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!u.ਟੈਗ_2.v.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.ਟੈਗ_2.v) {
    ਹਾਲਤ CFFTag:	/* All the non-collection tags */
    ਹਾਲਤ True_ਟੈਗ:
    ਹਾਲਤ Typ1_ਟੈਗ:
    ਹਾਲਤ True_Type_ਟੈਗ:	hb_barrier (); return_trace (u.fontFace.sanitize (c));
    ਹਾਲਤ TTCTag:	hb_barrier (); return_trace (u.ttc_ਸਿਰਲੇਖ.sanitize (c));
    ਹਾਲਤ DFont_ਟੈਗ:	hb_barrier (); return_trace (u.rf_ਸਿਰਲੇਖ.sanitize (c));
    ਮੂਲ:		return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { ਟੈਗ v; }	ਟੈਗ_2;		/* 4-byte identifier. */
  OpenTypeFontFace	fontFace;
  TTCHeader		ttc_ਸਿਰਲੇਖ;
  Resource_Fork_ਸਿਰਲੇਖ	rf_ਸਿਰਲੇਖ;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (4, ਟੈਗ_2.v);
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OPEN_FILE_HH */
