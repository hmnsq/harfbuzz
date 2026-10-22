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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ಅನ್ತರ್ಭಾವ "hb-directwrite.hh"

#ಅನ್ತರ್ಭಾವ "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_directwrite_global ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  ಸ್ಥಿರ hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    ಯದಿ (unlikely (!global))
      ಪ್ರತ್ಯಾಗಮ nullptr;
    ಯದಿ (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    ಪ್ರತ್ಯಾಗಮ global;
  }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  ಸ್ಥಿರ hb_directwrite_global_t * get_null ()
  {
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
} static_directwrite_global;


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  ಪ್ರತ್ಯಾಗಮ static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  auto *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_ಎಣಿಕೆ.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  font_File_ಕೀಲಿ = mLoader->RegisterFontFileStream (ಏತತ್);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_ಕೀಲಿ);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_ಸಂದೇಶ (DIRECTWRITE, nullptr, __VA_ARGS__); \
    ಪ್ರತ್ಯಾಗಮ nullptr; \
  } HB_STMT_END

  auto *global = get_directwrite_global ();
  ಯದಿ (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  ಯದಿ (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  auto hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_ಕೀಲಿ, ಪರಿಮಾಣ (fontFileStream->font_File_ಕೀಲಿ),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  ಯದಿ (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t ಸಂಖ್ಯೆ_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &ಸಂಖ್ಯೆ_Of_Faces);
  ಯದಿ (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  ಪ್ರತ್ಯಾಗಮ fontFace;
}

ಸಂರಚನಾ _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  ಶೂನ್ಯ *table_context;
};

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_directwrite_table_data_release (ಶೂನ್ಯ *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

ಸ್ಥಿರ hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_ಎಣಿಕೆ;
  ಯದಿ (FAILED (dw_face->GetFiles(&file_ಎಣಿಕೆ, ರಿಕ್ತ)))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  ಯದಿ (unlikely (!files.resize_exact (file_ಎಣಿಕೆ)))
    ಪ್ರತ್ಯಾಗಮ nullptr;
  ಯದಿ (FAILED (dw_face->GetFiles(&file_ಎಣಿಕೆ, files.arrayZ)))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_blob_t *blob = nullptr;
  ಪ್ರತಿ (UINT32 i = 0; i < file_ಎಣಿಕೆ; i++)
  {
    LPCVOID reference_ಕೀಲಿ;
    UINT32 reference_ಕೀಲಿ_size;
    ಯದಿ (FAILED (files[i]->Get_Reference_ಕೀಲಿ(&reference_ಕೀಲಿ, &reference_ಕೀಲಿ_size)))
      ಅನುವರ್ತ;

    IDWriteFontFileLoader *loader;
    ಯದಿ (FAILED (files[i]->GetLoader(&loader)))
      ಅನುವರ್ತ;
    
    IDWriteFontFileStream *stream;
    ಯದಿ (FAILED (loader->Create_Stream_From_ಕೀಲಿ (reference_ಕೀಲಿ, reference_ಕೀಲಿ_size, &stream)))
    {
      loader->Release ();
      ಅನುವರ್ತ;
    }

    UINT64 file_size;
    ಧ್ರುವ ಶೂನ್ಯ *fragment;
    ಶೂನ್ಯ *context;
    ಯದಿ (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      ಅನುವರ್ತ;
    }
    blob = hb_blob_create ((ಧ್ರುವ ವರ್ಣ *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, ರಿಕ್ತ, ರಿಕ್ತ);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    ಭಙ್ಗ;
  }

  ಪ್ರತ್ಯಾಗಮ blob;
}

ಸ್ಥಿರ hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, ಶೂನ್ಯ *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  ಧ್ರುವ ಶೂನ್ಯ *data;
  uint32_t length;
  ಶೂನ್ಯ *table_context;
  BOOL exists;
  ಯದಿ (tag == HB_TAG_NONE)
    ಪ್ರತ್ಯಾಗಮ _hb_directwrite_get_file_blob (dw_face);

  ಯದಿ (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  ಯದಿ (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (ಪರಿಮಾಣ (_hb_directwrite_font_table_context));
  ಯದಿ (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  ಪ್ರತ್ಯಾಗಮ hb_blob_create ((ಧ್ರುವ ವರ್ಣ *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_directwrite_face_release (ಶೂನ್ಯ *data)
{
  ((IDWriteFontFace *) data)->Release ();
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
  ಯದಿ (!dw_face)
    ಪ್ರತ್ಯಾಗಮ hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_ಎಣಿಕೆ (face, dw_face->Get_Glyph_ಎಣಿಕೆ ());

  ಪ್ರತ್ಯಾಗಮ face;
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
hb_directwrite_face_create_from_file_or_fail (ಧ್ರುವ ವರ್ಣ   *file_name,
					      unsigned ಪೂರ್ಣಾಙ್ಕ  index)
{
  auto *blob = hb_blob_create_from_file_or_fail (file_name);
  ಯದಿ (unlikely (!blob))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  ಪ್ರತ್ಯಾಗಮ hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      unsigned ಪೂರ್ಣಾಙ್ಕ  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  ಯದಿ (unlikely (!dw_face))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  ಯದಿ (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    ಪ್ರತ್ಯಾಗಮ face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  ಪ್ರತ್ಯಾಗಮ face;
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
  ಪ್ರತ್ಯಾಗಮ (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) face->data.directwrite;
}

#ifndef HB_DISABLE_DEPRECATED

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
  ಪ್ರತ್ಯಾಗಮ hb_directwrite_face_get_dw_font_face (face);
}

#endif

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

  ಯದಿ (unlikely (hb_object_is_immutable (font)))
    ಪ್ರತ್ಯಾಗಮ font;

  /* Copy font variations */
  ಯದಿ (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (ಶೂನ್ಯ**) &dw_face5)))
  {
    ಯದಿ (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_ಮೌಲ್ಯ> values;
      uint32_t ಎಣಿಕೆ = dw_face5->Get_Font_Axis_ಮೌಲ್ಯ_ಎಣಿಕೆ ();
      ಯದಿ (likely (values.resize_exact (ಎಣಿಕೆ)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_ಮೌಲ್ಯಗಳು (values.arrayZ, ಎಣಿಕೆ)))
      {
	hb_vector_t<hb_variation_t> vars;
	ಯದಿ (likely (vars.resize_exact (ಎಣಿಕೆ)))
	{
	  ಪ್ರತಿ (uint32_t i = 0; i < ಎಣಿಕೆ; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    ಭಿನ್ನ value = values[i].value;
	    vars[i] = {tag, value};
	  }
	  hb_font_set_variations (font, vars.arrayZ, vars.length);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->AddRef ();
  font->data.directwrite.cmpexch (nullptr, (hb_directwrite_font_data_t *) dw_face);

  ಪ್ರತ್ಯಾಗಮ font;
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
  ಪ್ರತ್ಯಾಗಮ (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;
}

#ifndef HB_DISABLE_DEPRECATED

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
  ಪ್ರತ್ಯಾಗಮ nullptr;
}

#endif

#endif
