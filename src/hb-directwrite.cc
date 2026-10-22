/*
 * Copyright © 2015-2019  Ebrahim Byagowi
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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_DIRECTWRITE

#అన్తర్భావ "hb-directwrite.hh"

#అన్తర్భావ "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

స్థిర అంతఃవరుస శూన్య free_static_directwrite_global ();

స్థిర నిర్మాణం hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  స్థిర hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    ఒకవేళ (unlikely (!global))
      తిరిగి శూన్యసూచిక;
    ఒకవేళ (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      తిరిగి శూన్యసూచిక;
    }

    hb_atexit (free_static_directwrite_global);

    తిరిగి global;
  }
  స్థిర శూన్య destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  స్థిర hb_directwrite_global_t * get_null ()
  {
    తిరిగి శూన్యసూచిక;
  }
} static_directwrite_global;


స్థిర అంతఃవరుస
శూన్య free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  తిరిగి static_directwrite_global.get_unconst ();
}

DWrite_Font_File_ప్రవాహం::DWrite_Font_File_ప్రవాహం (hb_blob_t *blob)
{
  స్వయంచాలక *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_లెక్క.init ();
  mLoader->కూడు_Ref ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_దత్తాంశం = (uint8_t *) hb_blob_get_దత్తాంశం (blob, &m_కొలత);
  font_File_కీలకం = mLoader->రిజిస్టరు_Font_File_ప్రవాహం (ఇది);
}

DWrite_Font_File_ప్రవాహం::~DWrite_Font_File_ప్రవాహం()
{
  mLoader->Unregister_Font_File_ప్రవాహం (font_File_కీలకం);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, గుర్తులేని సూచీ_2)
{
#నిర్వచించు FAIL(...) \
  HB_STMT_ప్రారంభం { \
    DEBUG_సందేశం (DIRECTWRITE, శూన్యసూచిక, __VA_ARGS__); \
    తిరిగి శూన్యసూచిక; \
  } HB_STMT_చివర

  స్వయంచాలక *global = get_directwrite_global ();
  ఒకవేళ (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWrite_Font_File_ప్రవాహం *font_File_ప్రవాహం = hb_directwrite_object_create<DWrite_Font_File_ప్రవాహం> (blob);
  ఒకవేళ (unlikely (!font_File_ప్రవాహం))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  స్వయంచాలక hr = global->dwriteFactory->CreateCustomFontFileReference (&font_File_ప్రవాహం->font_File_కీలకం, పరిమాణం (font_File_ప్రవాహం->font_File_కీలకం),
								  global->fontFileLoader, &fontFile);

  font_File_ప్రవాహం->Release ();

  ఒకవేళ (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t సంఖ్య_యొక్క_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &సంఖ్య_యొక్క_Faces);
  ఒకవేళ (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#నిర్వచనం_రద్దు FAIL

  IDWriteFontFace *fontFace = శూన్యసూచిక;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, సూచీ_2,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  తిరిగి fontFace;
}

నిర్మాణం hb_directwrite_font_పట్టిక_context {
  IDWriteFontFace *face;
  శూన్య *పట్టిక_context;
};

స్థిర శూన్య
hb_directwrite_పట్టిక_దత్తాంశం_release (శూన్య *దత్తాంశం)
{
  hb_directwrite_font_పట్టిక_context *context = (hb_directwrite_font_పట్టిక_context *) దత్తాంశం;
  context->face->Release_Font_పట్టిక (context->పట్టిక_context);
  hb_free (context);
}

స్థిర hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_లెక్క;
  ఒకవేళ (FAILED (dw_face->GetFiles(&file_లెక్క, ఖాళీ)))
    తిరిగి శూన్యసూచిక;

  hb_సదిశ_t<IDWriteFontFile *> files;
  ఒకవేళ (unlikely (!files.resize_exact (file_లెక్క)))
    తిరిగి శూన్యసూచిక;
  ఒకవేళ (FAILED (dw_face->GetFiles(&file_లెక్క, files.arrayZ)))
    తిరిగి శూన్యసూచిక;

  hb_blob_t *blob = శూన్యసూచిక;
  కొరకు (UINT32 i = 0; i < file_లెక్క; i++)
  {
    LPCVOID reference_కీలకం;
    UINT32 reference_కీలకం_కొలత;
    ఒకవేళ (FAILED (files[i]->Get_Reference_కీలకం(&reference_కీలకం, &reference_కీలకం_కొలత)))
      కొనసాగు;

    IDWriteFontFileLoader *loader;
    ఒకవేళ (FAILED (files[i]->GetLoader(&loader)))
      కొనసాగు;
    
    IDWrite_Font_File_ప్రవాహం *ప్రవాహం;
    ఒకవేళ (FAILED (loader->Create_ప్రవాహం_From_కీలకం (reference_కీలకం, reference_కీలకం_కొలత, &ప్రవాహం)))
    {
      loader->Release ();
      కొనసాగు;
    }

    UINT64 file_కొలత;
    స్థిరరాశి శూన్య *fragment;
    శూన్య *context;
    ఒకవేళ (FAILED (ప్రవాహం->Get_File_కొలత(&file_కొలత)) ||
        FAILED (ప్రవాహం->ReadFileFragment (&fragment, 0, file_కొలత, &context)))
    {
      loader->Release ();
      కొనసాగు;
    }
    blob = hb_blob_create ((స్థిరరాశి అక్షరం *) fragment, file_కొలత, HB_జ్ఞాపకం_బహుళకం_DUPLICATE, ఖాళీ, ఖాళీ);
    ప్రవాహం->ReleaseFileFragment (context);
    loader->Release ();
    ఆపు;
  }

  తిరిగి blob;
}

స్థిర hb_blob_t *
hb_directwrite_reference_పట్టిక (hb_face_t *face HB_వాడని, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_దత్తాంశం);
  స్థిరరాశి శూన్య *దత్తాంశం;
  uint32_t నిడివి;
  శూన్య *పట్టిక_context;
  BOOL exists;
  ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్_NONE)
    తిరిగి _hb_directwrite_get_file_blob (dw_face);

  ఒకవేళ (FAILED (dw_face->Try_Get_Font_పట్టిక (hb_uint32_మార్పిడి (ట్యాగ్_2), &దత్తాంశం,
					&నిడివి, &పట్టిక_context, &exists)))
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (!దత్తాంశం || !exists || !నిడివి)
  {
    dw_face->Release_Font_పట్టిక (పట్టిక_context);
    తిరిగి శూన్యసూచిక;
  }

  hb_directwrite_font_పట్టిక_context *context = (hb_directwrite_font_పట్టిక_context *) hb_malloc (పరిమాణం (hb_directwrite_font_పట్టిక_context));
  ఒకవేళ (unlikely (!context))
  {
    dw_face->Release_Font_పట్టిక (పట్టిక_context);
    తిరిగి శూన్యసూచిక;
  }
  context->face = dw_face;
  context->పట్టిక_context = పట్టిక_context;

  తిరిగి hb_blob_create ((స్థిరరాశి అక్షరం *) దత్తాంశం, నిడివి, HB_జ్ఞాపకం_బహుళకం_READONLY,
			 context, hb_directwrite_పట్టిక_దత్తాంశం_release);
}

స్థిర శూన్య
_hb_directwrite_face_release (శూన్య *దత్తాంశం)
{
  ((IDWriteFontFace *) దత్తాంశం)->Release ();
}

/**
 * hb_directwrite_face_create:
 * @dw_face: a DirectWrite IDWriteFontFace object.
 *
 * Constructs a new face object from the specified DirectWrite IDWriteFontFace.
 *
 * Return value: #hb_face_t object corresponding to the given input
 *
 * Since: 2.4.0
 **/
hb_face_t *
hb_directwrite_face_create (IDWriteFontFace *dw_face)
{
  ఒకవేళ (!dw_face)
    తిరిగి hb_face_get_ఖాళీలు ();

  dw_face->కూడు_Ref ();
  hb_face_t *face = hb_face_create_for_tables (hb_directwrite_reference_పట్టిక,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_సమితి_సూచీ (face, dw_face->Get_సూచీ ());
  hb_face_సమితి_glyph_లెక్క (face, dw_face->Get_Glyph_లెక్క ());

  తిరిగి face;
}

/**
 * hb_directwrite_face_create_from_file_or_fail:
 * @file_name: A font filename
 * @index: The index of the face within the file
 *
 * Creates an #hb_face_t face object from the specified
 * font file and face index.
 *
 * This is similar in functionality to hb_face_create_from_file_or_fail(),
 * but uses the DirectWrite library for loading the font file.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the file cannot be read.
 *
 * Since: 11.0.0
 */
hb_face_t *
hb_directwrite_face_create_from_file_or_fail (స్థిరరాశి అక్షరం   *file_name,
					      గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  స్వయంచాలక *blob = hb_blob_create_from_file_or_fail (file_name);
  ఒకవేళ (unlikely (!blob))
    తిరిగి శూన్యసూచిక;

  తిరిగి hb_directwrite_face_create_from_blob_or_fail (blob, సూచీ_2);
}

/**
 * hb_directwrite_face_create_from_blob_or_fail:
 * @blob: A blob containing the font data
 * @index: The index of the face within the blob
 *
 * Creates an #hb_face_t face object from the specified
 * blob and face index.
 *
 * This is similar in functionality to hb_face_create_or_fail(),
 * but uses the DirectWrite library for loading the font data.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the blob cannot be read.
 *
 * Since: 11.0.0
 */
HB_EXTERN hb_face_t *
hb_directwrite_face_create_from_blob_or_fail (hb_blob_t    *blob,
					      గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, సూచీ_2);
  ఒకవేళ (unlikely (!dw_face))
    తిరిగి శూన్యసూచిక;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  ఒకవేళ (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    తిరిగి face;
  }

  /* Let there be dragons here... */
  face->దత్తాంశం.directwrite.cmpexch (శూన్యసూచిక, (hb_directwrite_face_దత్తాంశం_t *) dw_face);

  తిరిగి face;
}

/**
* hb_directwrite_face_get_dw_font_face:
* @face: a #hb_face_t object
*
* Gets the DirectWrite IDWriteFontFace associated with @face.
*
* Return value: DirectWrite IDWriteFontFace object corresponding to the given input
*
* Since: 10.4.0
**/
IDWriteFontFace *
hb_directwrite_face_get_dw_font_face (hb_face_t *face)
{
  తిరిగి (IDWriteFontFace *) (స్థిరరాశి శూన్య *) face->దత్తాంశం.directwrite;
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED

/**
* hb_directwrite_face_get_font_face:
* @face: a #hb_face_t object
*
* Gets the DirectWrite IDWriteFontFace associated with @face.
*
* Return value: DirectWrite IDWriteFontFace object corresponding to the given input
*
* Since: 2.5.0
* Deprecated: 10.4.0: Use hb_directwrite_face_get_dw_font_face() instead
**/
IDWriteFontFace *
hb_directwrite_face_get_font_face (hb_face_t *face)
{
  తిరిగి hb_directwrite_face_get_dw_font_face (face);
}

#అయితే_ముగింపు

/**
 * hb_directwrite_font_create:
 * @dw_face: a DirectWrite IDWriteFontFace object.
 *
 * Constructs a new font object from the specified DirectWrite IDWriteFontFace.
 *
 * Return value: #hb_font_t object corresponding to the given input
 *
 * Since: 11.0.0
 **/
hb_font_t *
hb_directwrite_font_create (IDWriteFontFace *dw_face)
{
  IDWriteFontFace5 *dw_face5 = శూన్యసూచిక;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  ఒకవేళ (unlikely (hb_object_is_immutable (font)))
    తిరిగి font;

  /* Copy font variations */
  ఒకవేళ (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (శూన్య**) &dw_face5)))
  {
    ఒకవేళ (dw_face5->HasVariations ())
    {
      hb_సదిశ_t<DWRITE_FONT_AXIS_విలువ> values;
      uint32_t లెక్క = dw_face5->Get_Font_Axis_విలువ_లెక్క ();
      ఒకవేళ (likely (values.resize_exact (లెక్క)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_విలువలు (values.arrayZ, లెక్క)))
      {
	hb_సదిశ_t<hb_variation_t> vars;
	ఒకవేళ (likely (vars.resize_exact (లెక్క)))
	{
	  కొరకు (uint32_t i = 0; i < లెక్క; ++i)
	  {
	    hb_ట్యాగ్_t ట్యాగ్_2 = hb_uint32_మార్పిడి (values[i].axis_ట్యాగ్);
	    దశాంశం value = values[i].value;
	    vars[i] = {ట్యాగ్_2, value};
	  }
	  hb_font_సమితి_variations (font, vars.arrayZ, vars.నిడివి);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->కూడు_Ref ();
  font->దత్తాంశం.directwrite.cmpexch (శూన్యసూచిక, (hb_directwrite_font_దత్తాంశం_t *) dw_face);

  తిరిగి font;
}

/**
* hb_directwrite_font_get_dw_font_face:
* @font: a #hb_font_t object
*
* Gets the DirectWrite IDWriteFontFace associated with @font.
*
* Return value: DirectWrite IDWriteFontFace object corresponding to the given input
*
* Since: 11.0.0
**/
IDWriteFontFace *
hb_directwrite_font_get_dw_font_face (hb_font_t *font)
{
  తిరిగి (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED

/**
* hb_directwrite_font_get_dw_font:
* @font: a #hb_font_t object
*
* Deprecated.
*
* Return value: Returns `NULL`.
*
* Since: 10.3.0
* Deprecated: 11.0.0:
**/
IDWriteFont *
hb_directwrite_font_get_dw_font (hb_font_t *font)
{
  తిరిగి శూన్యసూచిక;
}

#అయితే_ముగింపు

#అయితే_ముగింపు
