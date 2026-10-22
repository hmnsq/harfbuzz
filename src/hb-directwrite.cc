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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-directwrite.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_directwrite_global ();

ਸਥਿਰ ਬਣਤਰ hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  ਸਥਿਰ hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    ਜੇ (unlikely (!global))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

    hb_atexit (free_static_directwrite_global);

    ਵਾਪਸ global;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  ਸਥਿਰ hb_directwrite_global_t * get_null ()
  {
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
} static_directwrite_global;


ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  ਵਾਪਸ static_directwrite_global.get_unconst ();
}

DWrite_Font_File_ਧਾਰਾ::DWrite_Font_File_ਧਾਰਾ (hb_blob_t *blob)
{
  ਸਵੈਚਾਲਿਤ *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_ਗਿਣਤੀ.init ();
  mLoader->ਜੋੜੋ_Ref ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_ਅੰਕੜੇ = (uint8_t *) hb_blob_get_ਅੰਕੜੇ (blob, &m_ਮਾਪ);
  font_File_ਕੁੰਜੀ = mLoader->ਰਜਿਸਟਰ_Font_File_ਧਾਰਾ (ਇਹ);
}

DWrite_Font_File_ਧਾਰਾ::~DWrite_Font_File_ਧਾਰਾ()
{
  mLoader->Unregister_Font_File_ਧਾਰਾ (font_File_ਕੁੰਜੀ);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL(...) \
  HB_STMT_ਸ਼ੁਰੂ { \
    DEBUG_ਸੁਨੇਹਾ (DIRECTWRITE, ਖਾਲੀ_ਸੂਚਕ, __VA_ARGS__); \
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; \
  } HB_STMT_ਸਿਰਾ

  ਸਵੈਚਾਲਿਤ *global = get_directwrite_global ();
  ਜੇ (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWrite_Font_File_ਧਾਰਾ *font_File_ਧਾਰਾ = hb_directwrite_object_create<DWrite_Font_File_ਧਾਰਾ> (blob);
  ਜੇ (unlikely (!font_File_ਧਾਰਾ))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  ਸਵੈਚਾਲਿਤ hr = global->dwriteFactory->CreateCustomFontFileReference (&font_File_ਧਾਰਾ->font_File_ਕੁੰਜੀ, ਆਕਾਰ (font_File_ਧਾਰਾ->font_File_ਕੁੰਜੀ),
								  global->fontFileLoader, &fontFile);

  font_File_ਧਾਰਾ->Release ();

  ਜੇ (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t ਸੰਖਿਆ_ਦਾ_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &ਸੰਖਿਆ_ਦਾ_Faces);
  ਜੇ (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FAIL

  IDWriteFontFace *fontFace = ਖਾਲੀ_ਸੂਚਕ;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, ਸੂਚਕਾਂਕ_2,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  ਵਾਪਸ fontFace;
}

ਬਣਤਰ hb_directwrite_font_ਸਾਰਣੀ_context {
  IDWriteFontFace *face;
  ਖਾਲੀ *ਸਾਰਣੀ_context;
};

ਸਥਿਰ ਖਾਲੀ
hb_directwrite_ਸਾਰਣੀ_ਅੰਕੜੇ_release (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_directwrite_font_ਸਾਰਣੀ_context *context = (hb_directwrite_font_ਸਾਰਣੀ_context *) ਅੰਕੜੇ;
  context->face->Release_Font_ਸਾਰਣੀ (context->ਸਾਰਣੀ_context);
  hb_free (context);
}

ਸਥਿਰ hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_ਗਿਣਤੀ;
  ਜੇ (FAILED (dw_face->GetFiles(&file_ਗਿਣਤੀ, ਨਿਰਾ)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_ਸਦਿਸ਼_t<IDWriteFontFile *> files;
  ਜੇ (unlikely (!files.resize_exact (file_ਗਿਣਤੀ)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (FAILED (dw_face->GetFiles(&file_ਗਿਣਤੀ, files.arrayZ)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_blob_t *blob = ਖਾਲੀ_ਸੂਚਕ;
  ਲਈ (UINT32 i = 0; i < file_ਗਿਣਤੀ; i++)
  {
    LPCVOID reference_ਕੁੰਜੀ;
    UINT32 reference_ਕੁੰਜੀ_ਮਾਪ;
    ਜੇ (FAILED (files[i]->Get_Reference_ਕੁੰਜੀ(&reference_ਕੁੰਜੀ, &reference_ਕੁੰਜੀ_ਮਾਪ)))
      ਜਾਰੀ;

    IDWriteFontFileLoader *loader;
    ਜੇ (FAILED (files[i]->GetLoader(&loader)))
      ਜਾਰੀ;
    
    IDWrite_Font_File_ਧਾਰਾ *ਧਾਰਾ;
    ਜੇ (FAILED (loader->Create_ਧਾਰਾ_From_ਕੁੰਜੀ (reference_ਕੁੰਜੀ, reference_ਕੁੰਜੀ_ਮਾਪ, &ਧਾਰਾ)))
    {
      loader->Release ();
      ਜਾਰੀ;
    }

    UINT64 file_ਮਾਪ;
    ਅਚਲ ਖਾਲੀ *fragment;
    ਖਾਲੀ *context;
    ਜੇ (FAILED (ਧਾਰਾ->Get_File_ਮਾਪ(&file_ਮਾਪ)) ||
        FAILED (ਧਾਰਾ->ReadFileFragment (&fragment, 0, file_ਮਾਪ, &context)))
    {
      loader->Release ();
      ਜਾਰੀ;
    }
    blob = hb_blob_create ((ਅਚਲ ਅੱਖਰ *) fragment, file_ਮਾਪ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE, ਨਿਰਾ, ਨਿਰਾ);
    ਧਾਰਾ->ReleaseFileFragment (context);
    loader->Release ();
    ਰੋਕੋ;
  }

  ਵਾਪਸ blob;
}

ਸਥਿਰ hb_blob_t *
hb_directwrite_reference_ਸਾਰਣੀ (hb_face_t *face HB_ਅਣਵਰਤਿਆ, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_ਅੰਕੜੇ);
  ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ;
  uint32_t ਲੰਮਾਈ;
  ਖਾਲੀ *ਸਾਰਣੀ_context;
  BOOL exists;
  ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ_NONE)
    ਵਾਪਸ _hb_directwrite_get_file_blob (dw_face);

  ਜੇ (FAILED (dw_face->Try_Get_Font_ਸਾਰਣੀ (hb_uint32_ਅਦਲਾ_ਬਦਲੀ (ਟੈਗ_2), &ਅੰਕੜੇ,
					&ਲੰਮਾਈ, &ਸਾਰਣੀ_context, &exists)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (!ਅੰਕੜੇ || !exists || !ਲੰਮਾਈ)
  {
    dw_face->Release_Font_ਸਾਰਣੀ (ਸਾਰਣੀ_context);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_directwrite_font_ਸਾਰਣੀ_context *context = (hb_directwrite_font_ਸਾਰਣੀ_context *) hb_malloc (ਆਕਾਰ (hb_directwrite_font_ਸਾਰਣੀ_context));
  ਜੇ (unlikely (!context))
  {
    dw_face->Release_Font_ਸਾਰਣੀ (ਸਾਰਣੀ_context);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  context->face = dw_face;
  context->ਸਾਰਣੀ_context = ਸਾਰਣੀ_context;

  ਵਾਪਸ hb_blob_create ((ਅਚਲ ਅੱਖਰ *) ਅੰਕੜੇ, ਲੰਮਾਈ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY,
			 context, hb_directwrite_ਸਾਰਣੀ_ਅੰਕੜੇ_release);
}

ਸਥਿਰ ਖਾਲੀ
_hb_directwrite_face_release (ਖਾਲੀ *ਅੰਕੜੇ)
{
  ((IDWriteFontFace *) ਅੰਕੜੇ)->Release ();
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
  ਜੇ (!dw_face)
    ਵਾਪਸ hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  dw_face->ਜੋੜੋ_Ref ();
  hb_face_t *face = hb_face_create_for_tables (hb_directwrite_reference_ਸਾਰਣੀ,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_ਸੈੱਟ_ਸੂਚਕਾਂਕ (face, dw_face->Get_ਸੂਚਕਾਂਕ ());
  hb_face_ਸੈੱਟ_glyph_ਗਿਣਤੀ (face, dw_face->Get_Glyph_ਗਿਣਤੀ ());

  ਵਾਪਸ face;
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
hb_directwrite_face_create_from_file_or_fail (ਅਚਲ ਅੱਖਰ   *file_name,
					      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  ਸਵੈਚਾਲਿਤ *blob = hb_blob_create_from_file_or_fail (file_name);
  ਜੇ (unlikely (!blob))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ hb_directwrite_face_create_from_blob_or_fail (blob, ਸੂਚਕਾਂਕ_2);
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
					      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, ਸੂਚਕਾਂਕ_2);
  ਜੇ (unlikely (!dw_face))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  ਜੇ (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    ਵਾਪਸ face;
  }

  /* Let there be dragons here... */
  face->ਅੰਕੜੇ.directwrite.cmpexch (ਖਾਲੀ_ਸੂਚਕ, (hb_directwrite_face_ਅੰਕੜੇ_t *) dw_face);

  ਵਾਪਸ face;
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
  ਵਾਪਸ (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) face->ਅੰਕੜੇ.directwrite;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED

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
  ਵਾਪਸ hb_directwrite_face_get_dw_font_face (face);
}

#ਜੇ_ਅੰਤ

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
  IDWriteFontFace5 *dw_face5 = ਖਾਲੀ_ਸੂਚਕ;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  ਜੇ (unlikely (hb_object_is_immutable (font)))
    ਵਾਪਸ font;

  /* Copy font variations */
  ਜੇ (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (ਖਾਲੀ**) &dw_face5)))
  {
    ਜੇ (dw_face5->HasVariations ())
    {
      hb_ਸਦਿਸ਼_t<DWRITE_FONT_AXIS_ਮੁੱਲ> values;
      uint32_t ਗਿਣਤੀ_2 = dw_face5->Get_Font_Axis_ਮੁੱਲ_ਗਿਣਤੀ ();
      ਜੇ (likely (values.resize_exact (ਗਿਣਤੀ_2)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_ਮੁੱਲਾਂ (values.arrayZ, ਗਿਣਤੀ_2)))
      {
	hb_ਸਦਿਸ਼_t<hb_variation_t> vars;
	ਜੇ (likely (vars.resize_exact (ਗਿਣਤੀ_2)))
	{
	  ਲਈ (uint32_t i = 0; i < ਗਿਣਤੀ_2; ++i)
	  {
	    hb_ਟੈਗ_t ਟੈਗ_2 = hb_uint32_ਅਦਲਾ_ਬਦਲੀ (values[i].axis_ਟੈਗ);
	    ਦਸ਼ਮਲਵ value = values[i].value;
	    vars[i] = {ਟੈਗ_2, value};
	  }
	  hb_font_ਸੈੱਟ_variations (font, vars.arrayZ, vars.ਲੰਮਾਈ);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->ਜੋੜੋ_Ref ();
  font->ਅੰਕੜੇ.directwrite.cmpexch (ਖਾਲੀ_ਸੂਚਕ, (hb_directwrite_font_ਅੰਕੜੇ_t *) dw_face);

  ਵਾਪਸ font;
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
  ਵਾਪਸ (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED

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
  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
