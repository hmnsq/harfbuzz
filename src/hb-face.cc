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
#అన్తర్భావ "hb-blob.hh"
#అన్తర్భావ "hb-open-file.hh"
#అన్తర్భావ "hb-ot-face.hh"
#అన్తర్భావ "hb-ot-cmap-table.hh"

#నిర్వచితమైతే HAVE_FREETYPE
#అన్తర్భావ "hb-ft.h"
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_CORETEXT
#అన్తర్భావ "hb-coretext.h"
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_DIRECTWRITE
#అన్తర్భావ "hb-directwrite.h"
#అయితే_ముగింపు


/**
 * SECTION:hb-face
 * @title: hb-face
 * @short_description: Font face objects
 * @include: hb.h
 *
 * A font face is an object that represents a single face from within a
 * font family.
 *
 * More precisely, a font face represents a single face in a binary font file.
 * Font faces are typically built from a binary blob and a face index.
 * Font faces are used to create fonts.
 *
 * A font face can be created from a binary blob using hb_face_create().
 * The face index is used to select a face from a binary blob that contains
 * multiple faces.  For example, a binary blob that contains both a regular
 * and a bold face can be used to create two font faces, one for each face
 * index.
 **/


/**
 * hb_face_count:
 * @blob: a blob.
 *
 * Fetches the number of faces in a blob.
 *
 * Return value: Number of faces in @blob
 *
 * Since: 1.7.7
 **/
గుర్తులేని పూర్ణాంకం
hb_face_లెక్క (hb_blob_t *blob)
{
  ఒకవేళ (unlikely (!blob))
    తిరిగి 0;

  hb_sanitize_context_t c (blob);

  స్వయంచాలక *ot = blob->as<OT::OpenTypeFontFile> ();
  ఒకవేళ (unlikely (!ot->sanitize (&c)))
    తిరిగి 0;

  తిరిగి ot->get_face_లెక్క ();
}

/*
 * hb_face_t
 */

DEFINE_NULL_INSTANCE (hb_face_t) =
{
  HB_OBJECT_శీర్షిక_STATIC,

  0,    /*సూచీ*/
  1000, /* upem */
  0,    /* num_glyphs */

  /* Zero for the rest is fine. */
};


/**
 * hb_face_create_for_tables:
 * @reference_table_func: (closure user_data) (destroy destroy) (scope notified): Table-referencing function
 * @user_data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 *
 * Variant of hb_face_create(), built for those cases where it is more
 * convenient to provide data for individual tables instead of the whole font
 * data. With the caveat that hb_face_get_table_tags() would not work
 * with faces created this way. You can address that by calling the
 * hb_face_set_get_table_tags_func() function and setting the appropriate callback.
 *
 * Creates a new face object from the specified @user_data and @reference_table_func,
 * with the @destroy callback.
 *
 * Return value: (transfer full): The new face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_create_for_tables (hb_reference_పట్టిక_func_t  reference_పట్టిక_func,
			   శూన్య                      *user_దత్తాంశం,
			   hb_destroy_func_t          destroy)
{
  hb_face_t *face;

  ఒకవేళ (!reference_పట్టిక_func || !(face = hb_object_create<hb_face_t> ())) {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి hb_face_get_ఖాళీలు ();
  }

  face->reference_పట్టిక_func = reference_పట్టిక_func;
  face->user_దత్తాంశం = user_దత్తాంశం;
  face->destroy = destroy;

  face->అంకె_glyphs = -1;

  face->దత్తాంశం.init0 (face);
  face->పట్టిక.init0 (face);

  తిరిగి face;
}


రకనిర్వచనం నిర్మాణం hb_face_for_దత్తాంశం_సంవృతం_t {
  hb_blob_t *blob;
  uint16_t  సూచీ_2;
} hb_face_for_దత్తాంశం_సంవృతం_t;

స్థిర hb_face_for_దత్తాంశం_సంవృతం_t *
hb_face_for_దత్తాంశం_సంవృతం_create (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం సూచీ_2)
{
  hb_face_for_దత్తాంశం_సంవృతం_t *సంవృతం;

  సంవృతం = (hb_face_for_దత్తాంశం_సంవృతం_t *) hb_calloc (1, పరిమాణం (hb_face_for_దత్తాంశం_సంవృతం_t));
  ఒకవేళ (unlikely (!సంవృతం))
    తిరిగి శూన్యసూచిక;

  సంవృతం->blob = blob;
  సంవృతం->సూచీ_2 = (uint16_t) (సూచీ_2 & 0xFFFFu);

  తిరిగి సంవృతం;
}

స్థిర శూన్య
hb_face_for_దత్తాంశం_సంవృతం_destroy (శూన్య *దత్తాంశం)
{
  hb_face_for_దత్తాంశం_సంవృతం_t *సంవృతం = (hb_face_for_దత్తాంశం_సంవృతం_t *) దత్తాంశం;

  hb_blob_destroy (సంవృతం->blob);
  hb_free (సంవృతం);
}

స్థిర hb_blob_t *
hb_face_for_దత్తాంశం_reference_పట్టిక (hb_face_t *face HB_వాడని, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం)
{
  hb_face_for_దత్తాంశం_సంవృతం_t *దత్తాంశం = (hb_face_for_దత్తాంశం_సంవృతం_t *) user_దత్తాంశం;

  ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్_NONE)
    తిరిగి hb_blob_reference (దత్తాంశం->blob);

  స్థిరరాశి OT::OpenTypeFontFile &ot_file = *దత్తాంశం->blob->as<OT::OpenTypeFontFile> ();
  గుర్తులేని పూర్ణాంకం పునాది_స్థానభ్రంశం;
  స్థిరరాశి OT::OpenTypeFontFace &ot_face = ot_file.get_face (దత్తాంశం->సూచీ_2, &పునాది_స్థానభ్రంశం);

  స్థిరరాశి OT::Open_Type_పట్టిక &పట్టిక = ot_face.get_పట్టిక_by_ట్యాగ్ (ట్యాగ్_2);

  hb_blob_t *blob = hb_blob_create_sub_blob (దత్తాంశం->blob, పునాది_స్థానభ్రంశం + పట్టిక.స్థానభ్రంశం, పట్టిక.నిడివి);

  తిరిగి blob;
}

స్థిర గుర్తులేని
hb_face_for_దత్తాంశం_get_పట్టిక_ట్యాగులు (స్థిరరాశి hb_face_t *face HB_వాడని,
				  గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2,
				  hb_ట్యాగ్_t *పట్టిక_ట్యాగులు,
				  శూన్య *user_దత్తాంశం)
{
  hb_face_for_దత్తాంశం_సంవృతం_t *దత్తాంశం = (hb_face_for_దత్తాంశం_సంవృతం_t *) user_దత్తాంశం;

  స్థిరరాశి OT::OpenTypeFontFile &ot_file = *దత్తాంశం->blob->as<OT::OpenTypeFontFile> ();
  స్థిరరాశి OT::OpenTypeFontFace &ot_face = ot_file.get_face (దత్తాంశం->సూచీ_2);

  తిరిగి ot_face.get_పట్టిక_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, పట్టిక_లెక్క_2, పట్టిక_ట్యాగులు);
}


/**
 * hb_face_create:
 * @blob: #hb_blob_t to work upon
 * @index: The index of the face within @blob
 *
 * Constructs a new face object from the specified blob and
 * a face index into that blob.
 *
 * The face index is used for blobs of file formats such as TTC and
 * DFont that can contain more than one face.  Face indices within
 * such collections are zero-based.
 *
 * <note>Note: If the blob font format is not a collection, @index
 * is ignored.  Otherwise, only the lower 16-bits of @index are used.
 * The unmodified @index can be accessed via hb_face_get_index().</note>
 *
 * <note>Note: The high 16-bits of @index, if non-zero, are used by
 * hb_font_create() to load named-instances in variable fonts.  See
 * hb_font_create() for details.</note>
 *
 * Return value: (transfer full): The new face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_create (hb_blob_t    *blob,
		గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  hb_face_t *face;

  ఒకవేళ (unlikely (!blob))
    blob = hb_blob_get_ఖాళీలు ();

  blob = hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (hb_blob_reference (blob));

  hb_face_for_దత్తాంశం_సంవృతం_t *సంవృతం = hb_face_for_దత్తాంశం_సంవృతం_create (blob, సూచీ_2);

  ఒకవేళ (unlikely (!సంవృతం))
  {
    hb_blob_destroy (blob);
    తిరిగి hb_face_get_ఖాళీలు ();
  }

  face = hb_face_create_for_tables (hb_face_for_దత్తాంశం_reference_పట్టిక,
				    సంవృతం,
				    hb_face_for_దత్తాంశం_సంవృతం_destroy);
  hb_face_సమితి_get_పట్టిక_ట్యాగులు_func (face,
				   hb_face_for_దత్తాంశం_get_పట్టిక_ట్యాగులు,
				   సంవృతం,
				   శూన్యసూచిక);

  face->సూచీ_2 = సూచీ_2;

  తిరిగి face;
}

/**
 * hb_face_create_or_fail:
 * @blob: #hb_blob_t to work upon
 * @index: The index of the face within @blob
 *
 * Like hb_face_create(), but returns `NULL` if the blob data
 * contains no usable font face at the specified index.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index.
 *
 * Since: 10.1.0
 **/
hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  గుర్తులేని అంకె_faces = hb_face_లెక్క (blob);
  ఒకవేళ (సూచీ_2 >= అంకె_faces)
    తిరిగి శూన్యసూచిక;

  hb_face_t *face = hb_face_create (blob, సూచీ_2);
  ఒకవేళ (hb_object_is_immutable (face))
    తిరిగి శూన్యసూచిక;

  తిరిగి face;
}

#నిర్వచితం_కాకపోతే HB_NO_OPEN
/**
 * hb_face_create_from_file_or_fail:
 * @file_name: A font filename
 * @index: The index of the face within the file
 *
 * A thin wrapper around hb_blob_create_from_file_or_fail()
 * followed by hb_face_create_or_fail().
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the file cannot be read.
 *
 * Since: 10.1.0
 **/
HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (స్థిరరాశి అక్షరం   *file_name,
				  గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (file_name);
  ఒకవేళ (unlikely (!blob))
    తిరిగి శూన్యసూచిక;

  hb_face_t *face = hb_face_create_or_fail (blob, సూచీ_2);
  hb_blob_destroy (blob);

  తిరిగి face;
}

స్థిర స్థిరరాశి నిర్మాణం supported_face_loaders_t {
	అక్షరం name[16];
	hb_face_t * (*from_file) (స్థిరరాశి అక్షరం *font_file, గుర్తులేని face_సూచీ);
	hb_face_t * (*from_blob) (hb_blob_t *blob, గుర్తులేని face_సూచీ);
} supported_face_loaders[] =
{
  {"ot",
#నిర్వచితం_కాకపోతే HB_NO_OPEN
   hb_face_create_from_file_or_fail,
#లేకపోతే
   శూన్యసూచిక,
#అయితే_ముగింపు
   hb_face_create_or_fail
  },
#నిర్వచితమైతే HAVE_FREETYPE
  {"ft",
   hb_ft_face_create_from_file_or_fail,
   hb_ft_face_create_from_blob_or_fail
  },
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_CORETEXT
  {"coretext",
   hb_coretext_face_create_from_file_or_fail,
   hb_coretext_face_create_from_blob_or_fail
  },
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_DIRECTWRITE
  {"directwrite",
   hb_directwrite_face_create_from_file_or_fail,
   hb_directwrite_face_create_from_blob_or_fail
  },
#అయితే_ముగింపు
};

స్థిర స్థిరరాశి అక్షరం *get_default_loader_name ()
{
  స్థిర hb_atomic_t<స్థిరరాశి అక్షరం *> static_loader_name;
  స్థిరరాశి అక్షరం *loader_name = static_loader_name.get_acquire ();
  ఒకవేళ (!loader_name)
  {
    loader_name = getenv ("HB_FACE_LOADER");
    ఒకవేళ (!loader_name)
      loader_name = "";
    ఒకవేళ (!static_loader_name.cmpexch (శూన్యసూచిక, loader_name))
      loader_name = static_loader_name.get_acquire ();
  }
  తిరిగి loader_name;
}

/**
 * hb_face_create_from_file_or_fail_using:
 * @file_name: A font filename
 * @index: The index of the face within the file
 * @loader_name: (nullable): The name of the loader to use, or `NULL`
 *
 * A thin wrapper around the face loader functions registered with HarfBuzz.
 * If @loader_name is `NULL` or the empty string, the first available loader
 * is used.
 *
 * For example, the FreeType ("ft") loader might be able to load
 * WOFF and WOFF2 files if FreeType is built with those features,
 * whereas the OpenType ("ot") loader will not.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * the file cannot be read or the loader fails to load the face.
 *
 * Since: 11.0.0
 **/
hb_face_t *
hb_face_create_from_file_or_fail_using (స్థిరరాశి అక్షరం   *file_name,
					గుర్తులేని పూర్ణాంకం  సూచీ_2,
					స్థిరరాశి అక్షరం   *loader_name)
{
  // Duplicated in hb_face_create_or_fail_using
  తార్కిక retry = అబద్ధం;
  ఒకవేళ (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = నిజం;
  }
  ఒకవేళ (loader_name && !*loader_name) loader_name = శూన్యసూచిక;

retry:
  కొరకు (గుర్తులేని i = 0; i < ARRAY_నిడివి (supported_face_loaders); i++)
  {
    ఒకవేళ (!loader_name || (supported_face_loaders[i].from_file && !strcmp (supported_face_loaders[i].name, loader_name)))
      తిరిగి supported_face_loaders[i].from_file (file_name, సూచీ_2);
  }

  ఒకవేళ (retry)
  {
    retry = అబద్ధం;
    loader_name = శూన్యసూచిక;
    వెళ్ళు retry;
  }

  తిరిగి శూన్యసూచిక;
}

/**
 * hb_face_create_or_fail_using:
 * @blob: #hb_blob_t to work upon
 * @index: The index of the face within @blob
 * @loader_name: (nullable): The name of the loader to use, or `NULL`
 *
 * A thin wrapper around the face loader functions registered with HarfBuzz.
 * If @loader_name is `NULL` or the empty string, the first available loader
 * is used.
 *
 * For example, the FreeType ("ft") loader might be able to load
 * WOFF and WOFF2 files if FreeType is built with those features,
 * whereas the OpenType ("ot") loader will not.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * the loader fails to load the face.
 *
 * Since: 11.0.0
 **/
hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      గుర్తులేని పూర్ణాంకం  సూచీ_2,
			      స్థిరరాశి అక్షరం   *loader_name)
{
  // Duplicated in hb_face_create_from_file_or_fail_using
  తార్కిక retry = అబద్ధం;
  ఒకవేళ (!loader_name || !*loader_name)
  {
    loader_name = get_default_loader_name ();
    retry = నిజం;
  }
  ఒకవేళ (loader_name && !*loader_name) loader_name = శూన్యసూచిక;

retry:
  కొరకు (గుర్తులేని i = 0; i < ARRAY_నిడివి (supported_face_loaders); i++)
  {
    ఒకవేళ (!loader_name || (supported_face_loaders[i].from_blob && !strcmp (supported_face_loaders[i].name, loader_name)))
      తిరిగి supported_face_loaders[i].from_blob (blob, సూచీ_2);
  }

  ఒకవేళ (retry)
  {
    retry = అబద్ధం;
    loader_name = శూన్యసూచిక;
    వెళ్ళు retry;
  }

  తిరిగి శూన్యసూచిక;
}

స్థిర అంతఃవరుస శూన్య free_static_face_loader_list ();

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి nil_face_loader_list[] = {శూన్యసూచిక};

స్థిర నిర్మాణం hb_face_loader_list_lazy_loader_t : hb_lazy_loader_t<స్థిరరాశి అక్షరం *,
								  hb_face_loader_list_lazy_loader_t>
{
  స్థిర స్థిరరాశి అక్షరం ** create ()
  {
    స్థిరరాశి అక్షరం **face_loader_list = (స్థిరరాశి అక్షరం **) hb_calloc (1 + ARRAY_నిడివి (supported_face_loaders), పరిమాణం (స్థిరరాశి అక్షరం *));
    ఒకవేళ (unlikely (!face_loader_list))
      తిరిగి శూన్యసూచిక;

    గుర్తులేని i;
    కొరకు (i = 0; i < ARRAY_నిడివి (supported_face_loaders); i++)
      face_loader_list[i] = supported_face_loaders[i].name;
    face_loader_list[i] = శూన్యసూచిక;

    hb_atexit (free_static_face_loader_list);

    తిరిగి face_loader_list;
  }
  స్థిర శూన్య destroy (స్థిరరాశి అక్షరం **l)
  { hb_free (l); }
  స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి * get_null ()
  { తిరిగి nil_face_loader_list; }
} static_face_loader_list;

స్థిర అంతఃవరుస
శూన్య free_static_face_loader_list ()
{
  static_face_loader_list.free_instance ();
}

/**
 * hb_face_list_loaders:
 *
 * Retrieves the list of face loaders supported by HarfBuzz.
 *
 * Return value: (transfer none) (array zero-terminated=1): a
 *    `NULL`-terminated array of supported face loaders
 *    constant strings. The returned array is owned by HarfBuzz
 *    and should not be modified or freed.
 *
 * Since: 11.0.0
 **/
స్థిరరాశి అక్షరం **
hb_face_list_loaders ()
{
  తిరిగి static_face_loader_list.get_unconst ();
}
#అయితే_ముగింపు


/**
 * hb_face_get_empty:
 *
 * Fetches the singleton empty face object.
 *
 * Return value: (transfer full): The empty face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_face_t *> (&Null (hb_face_t));
}


/**
 * hb_face_reference: (skip)
 * @face: A face object
 *
 * Increases the reference count on a face object.
 *
 * Return value: The @face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_face_reference (hb_face_t *face)
{
  తిరిగి hb_object_reference (face);
}

/**
 * hb_face_destroy: (skip)
 * @face: A face object
 *
 * Decreases the reference count on a face object. When the
 * reference count reaches zero, the face is destroyed,
 * freeing all memory.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_face_destroy (hb_face_t *face)
{
  ఒకవేళ (!hb_object_destroy (face)) తిరిగి;

#నిర్వచితం_కాకపోతే HB_NO_SHAPER
  కొరకు (hb_face_t::plan_కణుపు_t *node = face->shape_plans; node; )
  {
    hb_face_t::plan_కణుపు_t *next = node->next;
    hb_shape_plan_destroy (node->shape_plan);
    hb_free (node);
    node = next;
  }
#అయితే_ముగింపు

  face->దత్తాంశం.fini ();
  face->పట్టిక.fini ();

  ఒకవేళ (face->get_పట్టిక_ట్యాగులు_destroy)
    face->get_పట్టిక_ట్యాగులు_destroy (face->get_పట్టిక_ట్యాగులు_user_దత్తాంశం);

  ఒకవేళ (face->destroy)
    face->destroy (face->user_దత్తాంశం);

  hb_free (face);
}

/**
 * hb_face_set_user_data: (skip)
 * @face: A face object
 * @key: The user-data key to set
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the given face object.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_face_సమితి_user_దత్తాంశం (hb_face_t          *face,
		       hb_user_దత్తాంశం_కీలకం_t *key,
		       శూన్య *              దత్తాంశం,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (face, key, దత్తాంశం, destroy, replace);
}

/**
 * hb_face_get_user_data: (skip)
 * @face: A face object
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified face object.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
శూన్య *
hb_face_get_user_దత్తాంశం (స్థిరరాశి hb_face_t    *face,
		       hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (face, key);
}

/**
 * hb_face_make_immutable:
 * @face: A face object
 *
 * Makes the given face object immutable.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_face_make_immutable (hb_face_t *face)
{
  ఒకవేళ (hb_object_is_immutable (face))
    తిరిగి;

  hb_object_make_immutable (face);
}

/**
 * hb_face_is_immutable:
 * @face: A face object
 *
 * Tests whether the given face object is immutable.
 *
 * Return value: `true` is @face is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_face_is_immutable (hb_face_t *face)
{
  తిరిగి hb_object_is_immutable (face);
}


/**
 * hb_face_reference_table:
 * @face: A face object
 * @tag: The #hb_tag_t of the table to query
 *
 * Fetches a reference to the specified table within
 * the specified face. Returns an empty blob if referencing table data is not
 * possible.
 *
 * Return value: (transfer full): A pointer to the @tag table within @face
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_face_reference_పట్టిక (స్థిరరాశి hb_face_t *face,
			 hb_ట్యాగ్_t ట్యాగ్_2)
{
  ఒకవేళ (unlikely (ట్యాగ్_2 == HB_ట్యాగ్_NONE))
    తిరిగి hb_blob_get_ఖాళీలు ();

  తిరిగి face->reference_పట్టిక (ట్యాగ్_2);
}

/**
 * hb_face_reference_blob:
 * @face: A face object
 *
 * Fetches a pointer to the binary blob that contains the specified face.
 * If referencing the face data is not possible, this function creates a blob
 * out of individual table blobs if hb_face_get_table_tags() works with this
 * face, otherwise it returns an empty blob.
 *
 * Return value: (transfer full): A pointer to the blob for @face
 *
 * Since: 0.9.2
 **/
hb_blob_t *
hb_face_reference_blob (hb_face_t *face)
{
  hb_blob_t *blob = face->reference_పట్టిక (HB_ట్యాగ్_NONE);

  ఒకవేళ (blob == hb_blob_get_ఖాళీలు ())
  {
    // If referencing the face blob is not possible (e.g. not implemented by the
    // font functions), use face builder to create a blob out of individual
    // table blobs.
    గుర్తులేని మొత్తం_లెక్క = hb_face_get_పట్టిక_ట్యాగులు (face, 0, శూన్యసూచిక, శూన్యసూచిక);
    ఒకవేళ (మొత్తం_లెక్క)
    {
      hb_ట్యాగ్_t ట్యాగులు[64];
      గుర్తులేని లెక్క = ARRAY_నిడివి (ట్యాగులు);
      hb_face_t* builder = hb_face_builder_create ();

      కొరకు (గుర్తులేని స్థానభ్రంశం = 0; స్థానభ్రంశం < మొత్తం_లెక్క; స్థానభ్రంశం += లెక్క)
      {
        hb_face_get_పట్టిక_ట్యాగులు (face, స్థానభ్రంశం, &లెక్క, ట్యాగులు);
	ఒకవేళ (unlikely (!లెక్క))
	  ఆపు; // Allocation error
        కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
        {
	  ఒకవేళ (unlikely (!ట్యాగులు[i]))
	    కొనసాగు;
	  hb_blob_t *పట్టిక = hb_face_reference_పట్టిక (face, ట్యాగులు[i]);
	  hb_face_builder_కూడు_పట్టిక (builder, ట్యాగులు[i], పట్టిక);
	  hb_blob_destroy (పట్టిక);
        }
      }

      blob = hb_face_reference_blob (builder);
      hb_face_destroy (builder);
    }
  }

  తిరిగి blob;
}

/**
 * hb_face_set_index:
 * @face: A face object
 * @index: The index to assign
 *
 * Assigns the specified face-index to @face. Fails if the
 * face is immutable.
 *
 * <note>Note: changing the index has no effect on the face itself
 * This only changes the value returned by hb_face_get_index().</note>
 *
 * Since: 0.9.2
 **/
శూన్య
hb_face_సమితి_సూచీ (hb_face_t    *face,
		   గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  ఒకవేళ (hb_object_is_immutable (face))
    తిరిగి;

  face->సూచీ_2 = సూచీ_2;
}

/**
 * hb_face_get_index:
 * @face: A face object
 *
 * Fetches the face-index corresponding to the given face.
 *
 * <note>Note: face indices within a collection are zero-based.</note>
 *
 * Return value: The index of @face.
 *
 * Since: 0.9.2
 **/
గుర్తులేని పూర్ణాంకం
hb_face_get_సూచీ (స్థిరరాశి hb_face_t *face)
{
  తిరిగి face->సూచీ_2;
}

/**
 * hb_face_set_upem:
 * @face: A face object
 * @upem: The units-per-em value to assign
 *
 * Sets the units-per-em (upem) for a face object to the specified value.
 *
 * This API is used in rare circumstances.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_face_సమితి_upem (hb_face_t    *face,
		  గుర్తులేని పూర్ణాంకం  upem)
{
  ఒకవేళ (hb_object_is_immutable (face))
    తిరిగి;

  face->upem = upem;
}

/**
 * hb_face_get_upem:
 * @face: A face object
 *
 * Fetches the units-per-em (UPEM) value of the specified face object.
 *
 * Typical UPEM values for fonts are 1000, or 2048, but any value
 * in between 16 and 16,384 is allowed for OpenType fonts.
 *
 * Return value: The upem value of @face
 *
 * Since: 0.9.2
 **/
గుర్తులేని పూర్ణాంకం
hb_face_get_upem (స్థిరరాశి hb_face_t *face)
{
  తిరిగి face->get_upem ();
}

/**
 * hb_face_set_glyph_count:
 * @face: A face object
 * @glyph_count: The glyph-count value to assign
 *
 * Sets the glyph count for a face object to the specified value.
 *
 * This API is used in rare circumstances.
 *
 * Since: 0.9.7
 **/
శూన్య
hb_face_సమితి_glyph_లెక్క (hb_face_t    *face,
			 గుర్తులేని పూర్ణాంకం  glyph_లెక్క)
{
  ఒకవేళ (hb_object_is_immutable (face))
    తిరిగి;

  face->అంకె_glyphs = glyph_లెక్క;
}

/**
 * hb_face_get_glyph_count:
 * @face: A face object
 *
 * Fetches the glyph-count value of the specified face object.
 *
 * Return value: The glyph-count value of @face
 *
 * Since: 0.9.7
 **/
గుర్తులేని పూర్ణాంకం
hb_face_get_glyph_లెక్క (స్థిరరాశి hb_face_t *face)
{
  తిరిగి face->get_అంకె_glyphs ();
}

/**
 * hb_face_set_get_table_tags_func:
 * @face: A face object
 * @func: (closure user_data) (destroy destroy) (scope notified): The table-tag-fetching function
 * @user_data: A pointer to the user data, to be destroyed by @destroy when not needed anymore
 * @destroy: (nullable): A callback to call when @func is not needed anymore
 *
 * Sets the table-tag-fetching function for the specified face object.
 *
 * Since: 10.0.0
 */
HB_EXTERN శూన్య
hb_face_సమితి_get_పట్టిక_ట్యాగులు_func (hb_face_t *face,
				 hb_get_పట్టిక_ట్యాగులు_func_t func,
				 శూన్య                    *user_దత్తాంశం,
				 hb_destroy_func_t        destroy)
{
  ఒకవేళ (hb_object_is_immutable (face))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }

  ఒకవేళ (face->get_పట్టిక_ట్యాగులు_destroy)
    face->get_పట్టిక_ట్యాగులు_destroy (face->get_పట్టిక_ట్యాగులు_user_దత్తాంశం);

  face->get_పట్టిక_ట్యాగులు_func = func;
  face->get_పట్టిక_ట్యాగులు_user_దత్తాంశం = user_దత్తాంశం;
  face->get_పట్టిక_ట్యాగులు_destroy = destroy;
}

/**
 * hb_face_get_table_tags:
 * @face: A face object
 * @start_offset: The index of first table tag to retrieve
 * @table_count: (inout): Input = the maximum number of table tags to return;
 *                Output = the actual number of table tags returned (may be zero)
 * @table_tags: (out) (array length=table_count) (nullable): The array of table tags found
 *
 * Fetches a list of all table tags for a face, if possible. The list returned will
 * begin at the offset provided
 *
 * Return value: Total number of tables, or zero if it is not possible to list
 *
 * Since: 1.6.0
 **/
గుర్తులేని పూర్ణాంకం
hb_face_get_పట్టిక_ట్యాగులు (స్థిరరాశి hb_face_t *face,
			గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
			గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2, /* IN/OUT */
			hb_ట్యాగ్_t     *పట్టిక_ట్యాగులు /*బహిర్గతం*/)
{
  ఒకవేళ (!face->get_పట్టిక_ట్యాగులు_func)
  {
    ఒకవేళ (పట్టిక_లెక్క_2)
      *పట్టిక_లెక్క_2 = 0;
    తిరిగి 0;
  }

  తిరిగి face->get_పట్టిక_ట్యాగులు_func (face, ప్రారంభం_స్థానభ్రంశం, పట్టిక_లెక్క_2, పట్టిక_ట్యాగులు, face->get_పట్టిక_ట్యాగులు_user_దత్తాంశం);
}


/*
 * Character set.
 */


#నిర్వచితం_కాకపోతే HB_NO_FACE_COLLECT_UNICODES
/**
 * hb_face_collect_unicodes:
 * @face: A face object
 * @out: (out): The set to add Unicode characters to
 *
 * Collects all of the Unicode characters covered by @face and adds
 * them to the #hb_set_t set @out.
 *
 * Since: 1.9.0
 */
శూన్య
hb_face_collect_unicodes (hb_face_t *face,
			  hb_సమితి_t  *బహిర్గతం)
{
  face->పట్టిక.cmap->collect_unicodes (బహిర్గతం, face->get_అంకె_glyphs ());
}
/**
 * hb_face_collect_nominal_glyph_mapping:
 * @face: A face object
 * @mapping: (out): The map to add Unicode-to-glyph mapping to
 * @unicodes: (nullable) (out): The set to add Unicode characters to, or `NULL`
 *
 * Collects the mapping from Unicode characters to nominal glyphs of the @face,
 * and optionally all of the Unicode characters covered by @face.
 *
 * Since: 7.0.0
 */
శూన్య
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_సమితి_t  *unicodes)
{
  hb_సమితి_t దొంతర_unicodes;
  ఒకవేళ (!unicodes)
    unicodes = &దొంతర_unicodes;
  face->పట్టిక.cmap->collect_mapping (unicodes, mapping, face->get_అంకె_glyphs ());
}
/**
 * hb_face_collect_variation_selectors:
 * @face: A face object
 * @out: (out): The set to add Variation Selector characters to
 *
 * Collects all Unicode "Variation Selector" characters covered by @face and adds
 * them to the #hb_set_t set @out.
 *
 * Since: 1.9.0
 */
శూన్య
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_సమితి_t  *బహిర్గతం)
{
  face->పట్టిక.cmap->collect_variation_selectors (బహిర్గతం);
}
/**
 * hb_face_collect_variation_unicodes:
 * @face: A face object
 * @variation_selector: The Variation Selector to query
 * @out: (out): The set to add Unicode characters to
 *
 * Collects all Unicode characters for @variation_selector covered by @face and adds
 * them to the #hb_set_t set @out.
 *
 * Since: 1.9.0
 */
శూన్య
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_సమితి_t  *బహిర్గతం)
{
  face->పట్టిక.cmap->collect_variation_unicodes (variation_selector, బహిర్గతం);
}
#అయితే_ముగింపు
