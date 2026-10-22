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

#समावेश "hb.hh"

#अगर_घोषित HAVE_DIRECTWRITE

#समावेश "hb-directwrite.hh"

#समावेश "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

अटल अंतरभूत व्योम free_static_directwrite_global ();

अटल काष्ठा hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  अटल hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    अगर (unlikely (!global))
      वापस nullptr;
    अगर (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      वापस nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    वापस global;
  }
  अटल व्योम destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  अटल hb_directwrite_global_t * get_null ()
  {
    वापस nullptr;
  }
} static_directwrite_global;


अटल अंतरभूत
व्योम free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  वापस static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  स्वतः *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_गिनती.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_तथ्य = (uint8_t *) hb_blob_get_तथ्य (blob, &m_आकार);
  font_File_कुंजी = mLoader->RegisterFontFileStream (यह);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_कुंजी);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, अचिन्हित सूचकांक_2)
{
#घोषणा FAIL(...) \
  HB_STMT_START { \
    DEBUG_संदेश (DIRECTWRITE, nullptr, __VA_ARGS__); \
    वापस nullptr; \
  } HB_STMT_END

  स्वतः *global = get_directwrite_global ();
  अगर (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  अगर (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  स्वतः hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_कुंजी, माप (fontFileStream->font_File_कुंजी),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  अगर (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t संख्या_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &संख्या_Of_Faces);
  अगर (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#अघोषित FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, सूचकांक_2,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  वापस fontFace;
}

काष्ठा _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  व्योम *table_context;
};

अटल व्योम
hb_directwrite_table_तथ्य_release (व्योम *तथ्य)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) तथ्य;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

अटल hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_गिनती;
  अगर (FAILED (dw_face->GetFiles(&file_गिनती, शून्य)))
    वापस nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  अगर (unlikely (!files.resize_exact (file_गिनती)))
    वापस nullptr;
  अगर (FAILED (dw_face->GetFiles(&file_गिनती, files.arrayZ)))
    वापस nullptr;

  hb_blob_t *blob = nullptr;
  क्रम (UINT32 i = 0; i < file_गिनती; i++)
  {
    LPCVOID reference_कुंजी;
    UINT32 reference_कुंजी_आकार;
    अगर (FAILED (files[i]->Get_Reference_कुंजी(&reference_कुंजी, &reference_कुंजी_आकार)))
      जारी;

    IDWriteFontFileLoader *loader;
    अगर (FAILED (files[i]->GetLoader(&loader)))
      जारी;
    
    IDWriteFontFileStream *stream;
    अगर (FAILED (loader->Create_Stream_From_कुंजी (reference_कुंजी, reference_कुंजी_आकार, &stream)))
    {
      loader->Release ();
      जारी;
    }

    UINT64 file_आकार;
    स्थिर व्योम *fragment;
    व्योम *context;
    अगर (FAILED (stream->Get_File_आकार(&file_आकार)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_आकार, &context)))
    {
      loader->Release ();
      जारी;
    }
    blob = hb_blob_create ((स्थिर अक्षर *) fragment, file_आकार, HB_MEMORY_MODE_DUPLICATE, शून्य, शून्य);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    अवरोध;
  }

  वापस blob;
}

अटल hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, व्योम *user_तथ्य)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_तथ्य);
  स्थिर व्योम *तथ्य;
  uint32_t लंबाई;
  व्योम *table_context;
  BOOL exists;
  अगर (tag == HB_TAG_NONE)
    वापस _hb_directwrite_get_file_blob (dw_face);

  अगर (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &तथ्य,
					&लंबाई, &table_context, &exists)))
    वापस nullptr;

  अगर (!तथ्य || !exists || !लंबाई)
  {
    dw_face->ReleaseFontTable (table_context);
    वापस nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (माप (_hb_directwrite_font_table_context));
  अगर (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    वापस nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  वापस hb_blob_create ((स्थिर अक्षर *) तथ्य, लंबाई, HB_MEMORY_MODE_READONLY,
			 context, hb_directwrite_table_तथ्य_release);
}

अटल व्योम
_hb_directwrite_face_release (व्योम *तथ्य)
{
  ((IDWriteFontFace *) तथ्य)->Release ();
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
  अगर (!dw_face)
    वापस hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_सूचकांक (face, dw_face->Get_सूचकांक ());
  hb_face_set_glyph_गिनती (face, dw_face->Get_Glyph_गिनती ());

  वापस face;
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
hb_directwrite_face_create_from_file_or_fail (स्थिर अक्षर   *file_name,
					      अचिन्हित पूर्णांक  सूचकांक_2)
{
  स्वतः *blob = hb_blob_create_from_file_or_fail (file_name);
  अगर (unlikely (!blob))
    वापस nullptr;

  वापस hb_directwrite_face_create_from_blob_or_fail (blob, सूचकांक_2);
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
					      अचिन्हित पूर्णांक  सूचकांक_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, सूचकांक_2);
  अगर (unlikely (!dw_face))
    वापस nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  अगर (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    वापस face;
  }

  /* Let there be dragons here... */
  face->तथ्य.directwrite.cmpexch (nullptr, (hb_directwrite_face_तथ्य_t *) dw_face);

  वापस face;
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
  वापस (IDWriteFontFace *) (स्थिर व्योम *) face->तथ्य.directwrite;
}

#अगर_अघोषित HB_DISABLE_DEPRECATED

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
  वापस hb_directwrite_face_get_dw_font_face (face);
}

#पूर्ण_अगर

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
  IDWriteFontFace5 *dw_face5 = nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  अगर (unlikely (hb_object_is_immutable (font)))
    वापस font;

  /* Copy font variations */
  अगर (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (व्योम**) &dw_face5)))
  {
    अगर (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_मूल्य> values;
      uint32_t गिनती = dw_face5->Get_Font_Axis_मूल्य_गिनती ();
      अगर (likely (values.resize_exact (गिनती)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_मूल्यावली (values.arrayZ, गिनती)))
      {
	hb_vector_t<hb_variation_t> vars;
	अगर (likely (vars.resize_exact (गिनती)))
	{
	  क्रम (uint32_t i = 0; i < गिनती; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    भग्न value = values[i].value;
	    vars[i] = {tag, value};
	  }
	  hb_font_set_variations (font, vars.arrayZ, vars.लंबाई);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->AddRef ();
  font->तथ्य.directwrite.cmpexch (nullptr, (hb_directwrite_font_तथ्य_t *) dw_face);

  वापस font;
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
  वापस (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;
}

#अगर_अघोषित HB_DISABLE_DEPRECATED

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
  वापस nullptr;
}

#पूर्ण_अगर

#पूर्ण_अगर
