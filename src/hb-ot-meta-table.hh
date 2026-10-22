/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_META_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_META_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_meta HB_ਟੈਗ ('m','e','t','a')


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ ਅੰਕੜੇ_Map
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t a) ਅਚਲ { ਵਾਪਸ ਟੈਗ_2.cmp (a); }

  hb_ਟੈਗ_t get_ਟੈਗ () ਅਚਲ { ਵਾਪਸ ਟੈਗ_2; }

  hb_blob_t *reference_ਇੰਦਰਾਜ (hb_blob_t *meta_blob) ਅਚਲ
  { ਵਾਪਸ hb_blob_create_sub_blob (meta_blob, ਅੰਕੜੇ_Z, ਅੰਕੜੇ_ਲੰਮਾਈ_2); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਅੰਕੜੇ_Z.sanitize (c, ਅਧਾਰ, ਅੰਕੜੇ_ਲੰਮਾਈ_2)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ਟੈਗ_2;		/* A tag indicating the type of metadata. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>
		ਅੰਕੜੇ_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	ਅੰਕੜੇ_ਲੰਮਾਈ_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਬਣਤਰ meta
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_meta;

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<meta> (face); }
    ~accelerator_t () { ਸਾਰਣੀ.destroy (); }

    hb_blob_t *reference_ਇੰਦਰਾਜ (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
    { ਵਾਪਸ ਸਾਰਣੀ->ਅੰਕੜੇ_Maps.lsearch (ਟੈਗ_2).reference_ਇੰਦਰਾਜ (ਸਾਰਣੀ.get_blob ()); }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_entries (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ      ਸ਼ੁਰੂ_ਵਿੱਥ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     *ਗਿਣਤੀ_2,
			      hb_ot_meta_ਟੈਗ_t *entries) ਅਚਲ
    {
      ਜੇ (ਗਿਣਤੀ_2 && entries)
      {
	+ ਸਾਰਣੀ->ਅੰਕੜੇ_Maps.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਗਿਣਤੀ_2)
	| hb_map (&ਅੰਕੜੇ_Map::get_ਟੈਗ)
	| hb_map ([](hb_ਟੈਗ_t ਟੈਗ_2) { ਵਾਪਸ (hb_ot_meta_ਟੈਗ_t) ਟੈਗ_2; })
	| hb_sink (hb_array (entries, *ਗਿਣਤੀ_2))
	;
      }
      ਵਾਪਸ ਸਾਰਣੀ->ਅੰਕੜੇ_Maps.len;
    }

    ਨਿਜੀ:
    hb_blob_ptr_t<meta> ਸਾਰਣੀ;
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ == 1 &&
			  ਅੰਕੜੇ_Maps.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT32	ਸੰਸਕਰਣ;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	ਅੰਕੜੇ_ਵਿੱਥ;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_ਦਾ<ਅੰਕੜੇ_Map>
		ਅੰਕੜੇ_Maps;/* Array of data map records. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (16, ਅੰਕੜੇ_Maps);
};

ਬਣਤਰ meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_META_TABLE_HH */
