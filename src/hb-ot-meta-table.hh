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

#నిర్వచితం_కాకపోతే HB_OT_META_పట్టిక_HH
#నిర్వచించు HB_OT_META_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"

/*
 * meta -- Metadata Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6meta.html
 */
#నిర్వచించు HB_OT_ట్యాగ్_meta HB_ట్యాగ్ ('m','e','t','a')


నామాకాశ OT {


నిర్మాణం దత్తాంశం_Map
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t a) స్థిరరాశి { తిరిగి ట్యాగ్_2.cmp (a); }

  hb_ట్యాగ్_t get_ట్యాగ్ () స్థిరరాశి { తిరిగి ట్యాగ్_2; }

  hb_blob_t *reference_నమోదు (hb_blob_t *meta_blob) స్థిరరాశి
  { తిరిగి hb_blob_create_sub_blob (meta_blob, దత్తాంశం_Z, దత్తాంశం_నిడివి_2); }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  దత్తాంశం_Z.sanitize (c, పునాది, దత్తాంశం_నిడివి_2)));
  }

  రక్షిత:
  ట్యాగ్		ట్యాగ్_2;		/* A tag indicating the type of metadata. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>
		దత్తాంశం_Z;		/* Offset in bytes from the beginning of the
				 * metadata table to the data for this tag. */
  HBUINT32	దత్తాంశం_నిడివి_2;	/* Length of the data. The data is not required to
				 * be padded to any byte boundary. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

నిర్మాణం meta
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_meta;

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    { పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<meta> (face); }
    ~accelerator_t () { పట్టిక.destroy (); }

    hb_blob_t *reference_నమోదు (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
    { తిరిగి పట్టిక->దత్తాంశం_Maps.lsearch (ట్యాగ్_2).reference_నమోదు (పట్టిక.get_blob ()); }

    గుర్తులేని పూర్ణాంకం get_entries (గుర్తులేని పూర్ణాంకం      ప్రారంభం_స్థానభ్రంశం,
			      గుర్తులేని పూర్ణాంకం     *లెక్క,
			      hb_ot_meta_ట్యాగ్_t *entries) స్థిరరాశి
    {
      ఒకవేళ (లెక్క && entries)
      {
	+ పట్టిక->దత్తాంశం_Maps.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, లెక్క)
	| hb_map (&దత్తాంశం_Map::get_ట్యాగ్)
	| hb_map ([](hb_ట్యాగ్_t ట్యాగ్_2) { తిరిగి (hb_ot_meta_ట్యాగ్_t) ట్యాగ్_2; })
	| hb_sink (hb_array (entries, *లెక్క))
	;
      }
      తిరిగి పట్టిక->దత్తాంశం_Maps.len;
    }

    అంతరంగ:
    hb_blob_ptr_t<meta> పట్టిక;
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ == 1 &&
			  దత్తాంశం_Maps.sanitize (c, ఇది)));
  }

  రక్షిత:
  HBUINT32	సంస్కరణ;	/* Version number of the metadata table — set to 1. */
  HBUINT32	flags;		/* Flags — currently unused; set to 0. */
  HBUINT32	దత్తాంశం_స్థానభ్రంశం;
				/* Per Apple specification:
				 * Offset from the beginning of the table to the data.
				 * Per OT specification:
				 * Reserved. Not used; should be set to 0. */
  Array32_యొక్క<దత్తాంశం_Map>
		దత్తాంశం_Maps;/* Array of data map records. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (16, దత్తాంశం_Maps);
};

నిర్మాణం meta_accelerator_t : meta::accelerator_t {
  meta_accelerator_t (hb_face_t *face) : meta::accelerator_t (face) {}
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_META_TABLE_HH */
