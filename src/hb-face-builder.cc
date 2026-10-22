/*
 * Copyright © 2009  Red Hat, Inc.
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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-face.hh"

#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-open-file.hh"
#అన్తర్భావ "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

నిర్మాణం face_పట్టిక_info_t
{
  hb_blob_t* దత్తాంశం;
  గుర్తులేని క్రమం;
};

నిర్మాణం hb_face_builder_దత్తాంశం_t
{
  hb_hashmap_t<hb_ట్యాగ్_t, face_పట్టిక_info_t> tables;
};

స్థిర పూర్ణాంకం compare_entries (స్థిరరాశి శూన్య* pa, స్థిరరాశి శూన్య* pb)
{
  స్థిరరాశి స్వయంచాలక& a = * (స్థిరరాశి hb_pair_t<hb_ట్యాగ్_t, face_పట్టిక_info_t> *) pa;
  స్థిరరాశి స్వయంచాలక& b = * (స్థిరరాశి hb_pair_t<hb_ట్యాగ్_t, face_పట్టిక_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  ఒకవేళ (a.second.క్రమం != b.second.క్రమం)
    తిరిగి a.second.క్రమం < b.second.క్రమం ? -1 : +1;

  ఒకవేళ (a.second.దత్తాంశం->నిడివి != b.second.దత్తాంశం->నిడివి)
    తిరిగి a.second.దత్తాంశం->నిడివి < b.second.దత్తాంశం->నిడివి ? -1 : +1;

  తిరిగి a.మొదటి < b.మొదటి ? -1 : a.మొదటి == b.మొదటి ? 0 : +1;
}

స్థిర hb_face_builder_దత్తాంశం_t *
hb_face_builder_దత్తాంశం_create ()
{
  hb_face_builder_దత్తాంశం_t *దత్తాంశం = (hb_face_builder_దత్తాంశం_t *) hb_calloc (1, పరిమాణం (hb_face_builder_దత్తాంశం_t));
  ఒకవేళ (unlikely (!దత్తాంశం))
    తిరిగి శూన్యసూచిక;

  దత్తాంశం->tables.init ();

  తిరిగి దత్తాంశం;
}

స్థిర శూన్య
hb_face_builder_దత్తాంశం_destroy (శూన్య *user_దత్తాంశం)
{
  hb_face_builder_దత్తాంశం_t *దత్తాంశం = (hb_face_builder_దత్తాంశం_t *) user_దత్తాంశం;

  కొరకు (స్వయంచాలక info : దత్తాంశం->tables.values())
    hb_blob_destroy (info.దత్తాంశం);

  దత్తాంశం->tables.fini ();

  hb_free (దత్తాంశం);
}

స్థిర hb_blob_t *
hb_face_builder_దత్తాంశం_reference_blob (hb_face_builder_దత్తాంశం_t *దత్తాంశం)
{

  గుర్తులేని పూర్ణాంకం పట్టిక_లెక్క_2 = దత్తాంశం->tables.get_population ();
  గుర్తులేని పూర్ణాంకం face_నిడివి = పట్టిక_లెక్క_2 * 16 + 12;

  కొరకు (స్వయంచాలక info : దత్తాంశం->tables.values())
    face_నిడివి += hb_ceil_గమ్యం_4 (hb_blob_get_నిడివి (info.దత్తాంశం));

  అక్షరం *buf = (అక్షరం *) hb_malloc (face_నిడివి);
  ఒకవేళ (unlikely (!buf))
    తిరిగి శూన్యసూచిక;

  hb_serialize_context_t c (buf, face_నిడివి);
  c.propagate_error (దత్తాంశం->tables);
  OT::OpenTypeFontFile *f = c.ప్రారంభం_serialize<OT::OpenTypeFontFile> ();

  తార్కిక is_cff = (దత్తాంశం->tables.has (HB_ట్యాగ్ ('C','F','F',' '))
                 || దత్తాంశం->tables.has (HB_ట్యాగ్ ('C','F','F','2')));
  hb_ట్యాగ్_t sfnt_ట్యాగ్ = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_ట్యాగ్;

  // Sort the tags so that produced face is deterministic.
  hb_సదిశ_t<hb_pair_t <hb_ట్యాగ్_t, face_పట్టిక_info_t>> sorted_entries;
  దత్తాంశం->tables.iter () | hb_sink (sorted_entries);
  ఒకవేళ (unlikely (sorted_entries.లోపల_error ()))
  {
    hb_free (buf);
    తిరిగి శూన్యసూచిక;
  }

  sorted_entries.qsort (compare_entries);

  తార్కిక ret = f->serialize_single (&c,
                                  sfnt_ట్యాగ్,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_ట్యాగ్_t, face_పట్టిక_info_t> _) {
                                    తిరిగి hb_pair_t<hb_ట్యాగ్_t, hb_blob_t*> (_.మొదటి, _.second.దత్తాంశం);
                                  }));

  c.చివర_serialize ();

  ఒకవేళ (unlikely (!ret))
  {
    hb_free (buf);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి hb_blob_create (buf, face_నిడివి, HB_జ్ఞాపకం_బహుళకం_WRITABLE, buf, hb_free);
}

స్థిర hb_blob_t *
hb_face_builder_reference_పట్టిక (hb_face_t *face HB_వాడని, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం)
{
  hb_face_builder_దత్తాంశం_t *దత్తాంశం = (hb_face_builder_దత్తాంశం_t *) user_దత్తాంశం;

  ఒకవేళ (!ట్యాగ్_2)
    తిరిగి hb_face_builder_దత్తాంశం_reference_blob (దత్తాంశం);

  తిరిగి hb_blob_reference (దత్తాంశం->tables[ట్యాగ్_2].దత్తాంశం);
}

స్థిర గుర్తులేని
hb_face_builder_get_పట్టిక_ట్యాగులు (స్థిరరాశి hb_face_t *face HB_వాడని,
				 గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				 గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2,
				 hb_ట్యాగ్_t *పట్టిక_ట్యాగులు,
				 శూన్య *user_దత్తాంశం)
{
  hb_face_builder_దత్తాంశం_t *దత్తాంశం = (hb_face_builder_దత్తాంశం_t *) user_దత్తాంశం;

  గుర్తులేని population = దత్తాంశం->tables.get_population ();

  ఒకవేళ (!పట్టిక_లెక్క_2)
    తిరిగి population;

  ఒకవేళ (unlikely (ప్రారంభం_స్థానభ్రంశం >= population))
  {
    *పట్టిక_లెక్క_2 = 0;
    తిరిగి population;
  }

  // Sort the tags.
  hb_సదిశ_t<hb_ట్యాగ్_t> sorted_ట్యాగులు;
  దత్తాంశం->tables.కీలకాలు () | hb_sink (sorted_ట్యాగులు);
  ఒకవేళ (unlikely (sorted_ట్యాగులు.లోపల_error ()))
  {
    // Not much to do...
  }
  sorted_ట్యాగులు.qsort ([] (స్థిరరాశి hb_ట్యాగ్_t &a, స్థిరరాశి hb_ట్యాగ్_t &b) {
    తిరిగి (a > b) - (a < b);
  });

  స్వయంచాలక array = sorted_ట్యాగులు.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, పట్టిక_లెక్క_2);
  ఒకవేళ (పట్టిక_ట్యాగులు)
  {
    స్వయంచాలక బహిర్గతం = hb_array (పట్టిక_ట్యాగులు, *పట్టిక_లెక్క_2);

    + array.iter ()
    | hb_sink (బహిర్గతం)
    ;
  }

  తిరిగి population;
}


/**
 * hb_face_builder_create:
 *
 * Creates a #hb_face_t that can be used with hb_face_builder_add_table().
 * After tables are added to the face, it can be compiled to a binary
 * font file by calling hb_face_reference_blob().
 *
 * Return value: (transfer full): New face.
 *
 * Since: 1.9.0
 **/
hb_face_t *
hb_face_builder_create ()
{
  hb_face_builder_దత్తాంశం_t *దత్తాంశం = hb_face_builder_దత్తాంశం_create ();
  ఒకవేళ (unlikely (!దత్తాంశం)) తిరిగి hb_face_get_ఖాళీలు ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_పట్టిక,
					       దత్తాంశం,
					       hb_face_builder_దత్తాంశం_destroy);

  hb_face_సమితి_get_పట్టిక_ట్యాగులు_func (face,
				   hb_face_builder_get_పట్టిక_ట్యాగులు,
				   దత్తాంశం,
				   శూన్యసూచిక);

  తిరిగి face;
}

/**
 * hb_face_builder_add_table:
 * @face: A face object created with hb_face_builder_create()
 * @tag: The #hb_tag_t of the table to add
 * @blob: The blob containing the table data to add
 *
 * Add table for @tag with data provided by @blob to the face.  @face must
 * be created using hb_face_builder_create().
 *
 * Since: 1.9.0
 **/
hb_bool_t
hb_face_builder_కూడు_పట్టిక (hb_face_t *face, hb_ట్యాగ్_t ట్యాగ్_2, hb_blob_t *blob)
{
  ఒకవేళ (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_దత్తాంశం_destroy))
    తిరిగి అబద్ధం;

  ఒకవేళ (ట్యాగ్_2 == HB_MAP_విలువ_INVALID)
    తిరిగి అబద్ధం;

  hb_face_builder_దత్తాంశం_t *దత్తాంశం = (hb_face_builder_దత్తాంశం_t *) face->user_దత్తాంశం;

  hb_blob_t* మునుపటి = దత్తాంశం->tables.get (ట్యాగ్_2).దత్తాంశం;
  ఒకవేళ (!దత్తాంశం->tables.సమితి (ట్యాగ్_2, face_పట్టిక_info_t {hb_blob_reference (blob), (గుర్తులేని) -1}))
  {
    hb_blob_destroy (blob);
    తిరిగి అబద్ధం;
  }

  hb_blob_destroy (మునుపటి);
  తిరిగి నిజం;
}

/**
 * hb_face_builder_sort_tables:
 * @face: A face object created with hb_face_builder_create()
 * @tags: (array zero-terminated=1): ordered list of table tags terminated by
 *   %HB_TAG_NONE
 *
 * Set the ordering of tables for serialization. Any tables not
 * specified in the tags list will be ordered after the tables in
 * tags, ordered by the default sort ordering.
 *
 * Since: 5.3.0
 **/
శూన్య
hb_face_builder_క్రమీకరించు_tables (hb_face_t *face,
                             స్థిరరాశి hb_ట్యాగ్_t  *ట్యాగులు)
{
  ఒకవేళ (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_దత్తాంశం_destroy))
    తిరిగి;

  hb_face_builder_దత్తాంశం_t *దత్తాంశం = (hb_face_builder_దత్తాంశం_t *) face->user_దత్తాంశం;

  // Sort all unspecified tables after any specified tables.
  కొరకు (స్వయంచాలక& info : దత్తాంశం->tables.విలువలు_ref())
    info.క్రమం = (గుర్తులేని) -1;

  గుర్తుగల క్రమం = 0;
  కొరకు (స్థిరరాశి hb_ట్యాగ్_t* ట్యాగ్_2 = ట్యాగులు;
       *ట్యాగ్_2;
       ట్యాగ్_2++)
  {
    face_పట్టిక_info_t* info;
    ఒకవేళ (!దత్తాంశం->tables.has (*ట్యాగ్_2, &info)) కొనసాగు;
    info->క్రమం = క్రమం++;
  }
}
