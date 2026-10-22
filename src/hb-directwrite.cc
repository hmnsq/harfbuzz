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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#അന്തര്ഭാവ "hb-directwrite.hh"

#അന്തര്ഭാവ "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_directwrite_global ();

സ്ഥിതം ഘടന hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  സ്ഥിതം hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    എങ്കിൽ (unlikely (!global))
      തിരികെ nullptr;
    എങ്കിൽ (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      തിരികെ nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    തിരികെ global;
  }
  സ്ഥിതം ശൂന്യം destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  സ്ഥിതം hb_directwrite_global_t * get_null ()
  {
    തിരികെ nullptr;
  }
} static_directwrite_global;


സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  തിരികെ static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  auto *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_എണ്ണം.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  font_File_താക്കോൽ = mLoader->RegisterFontFileStream (ഇത്);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_താക്കോൽ);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_സന്ദേശം (DIRECTWRITE, nullptr, __VA_ARGS__); \
    തിരികെ nullptr; \
  } HB_STMT_END

  auto *global = get_directwrite_global ();
  എങ്കിൽ (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  എങ്കിൽ (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  auto hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_താക്കോൽ, വലുപ്പം (fontFileStream->font_File_താക്കോൽ),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  എങ്കിൽ (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t സംഖ്യ_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &സംഖ്യ_Of_Faces);
  എങ്കിൽ (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  തിരികെ fontFace;
}

ഘടന _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  ശൂന്യം *table_context;
};

സ്ഥിതം ശൂന്യം
_hb_directwrite_table_data_release (ശൂന്യം *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

സ്ഥിതം hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_എണ്ണം;
  എങ്കിൽ (FAILED (dw_face->GetFiles(&file_എണ്ണം, ശൂന്യ)))
    തിരികെ nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  എങ്കിൽ (unlikely (!files.resize_exact (file_എണ്ണം)))
    തിരികെ nullptr;
  എങ്കിൽ (FAILED (dw_face->GetFiles(&file_എണ്ണം, files.arrayZ)))
    തിരികെ nullptr;

  hb_blob_t *blob = nullptr;
  വേണ്ടി (UINT32 i = 0; i < file_എണ്ണം; i++)
  {
    LPCVOID reference_താക്കോൽ;
    UINT32 reference_താക്കോൽ_size;
    എങ്കിൽ (FAILED (files[i]->Get_Reference_താക്കോൽ(&reference_താക്കോൽ, &reference_താക്കോൽ_size)))
      തുടരുക;

    IDWriteFontFileLoader *loader;
    എങ്കിൽ (FAILED (files[i]->GetLoader(&loader)))
      തുടരുക;
    
    IDWriteFontFileStream *stream;
    എങ്കിൽ (FAILED (loader->Create_Stream_From_താക്കോൽ (reference_താക്കോൽ, reference_താക്കോൽ_size, &stream)))
    {
      loader->Release ();
      തുടരുക;
    }

    UINT64 file_size;
    സ്ഥിരം ശൂന്യം *fragment;
    ശൂന്യം *context;
    എങ്കിൽ (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      തുടരുക;
    }
    blob = hb_blob_create ((സ്ഥിരം അക്ഷരം *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, ശൂന്യ, ശൂന്യ);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    നിർത്തുക;
  }

  തിരികെ blob;
}

സ്ഥിതം hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, ശൂന്യം *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  സ്ഥിരം ശൂന്യം *data;
  uint32_t length;
  ശൂന്യം *table_context;
  BOOL exists;
  എങ്കിൽ (tag == HB_TAG_NONE)
    തിരികെ _hb_directwrite_get_file_blob (dw_face);

  എങ്കിൽ (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    തിരികെ nullptr;

  എങ്കിൽ (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    തിരികെ nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (വലുപ്പം (_hb_directwrite_font_table_context));
  എങ്കിൽ (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    തിരികെ nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  തിരികെ hb_blob_create ((സ്ഥിരം അക്ഷരം *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

സ്ഥിതം ശൂന്യം
_hb_directwrite_face_release (ശൂന്യം *data)
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
  എങ്കിൽ (!dw_face)
    തിരികെ hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_എണ്ണം (face, dw_face->Get_Glyph_എണ്ണം ());

  തിരികെ face;
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
hb_directwrite_face_create_from_file_or_fail (സ്ഥിരം അക്ഷരം   *file_name,
					      unsigned പൂർണ്ണസംഖ്യ  index)
{
  auto *blob = hb_blob_create_from_file_or_fail (file_name);
  എങ്കിൽ (unlikely (!blob))
    തിരികെ nullptr;

  തിരികെ hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      unsigned പൂർണ്ണസംഖ്യ  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  എങ്കിൽ (unlikely (!dw_face))
    തിരികെ nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  എങ്കിൽ (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    തിരികെ face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  തിരികെ face;
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
  തിരികെ (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) face->data.directwrite;
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
  തിരികെ hb_directwrite_face_get_dw_font_face (face);
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

  എങ്കിൽ (unlikely (hb_object_is_immutable (font)))
    തിരികെ font;

  /* Copy font variations */
  എങ്കിൽ (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (ശൂന്യം**) &dw_face5)))
  {
    എങ്കിൽ (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_മൂല്യം> values;
      uint32_t എണ്ണം = dw_face5->Get_Font_Axis_മൂല്യം_എണ്ണം ();
      എങ്കിൽ (likely (values.resize_exact (എണ്ണം)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_മൂല്യങ്ങൾ (values.arrayZ, എണ്ണം)))
      {
	hb_vector_t<hb_variation_t> vars;
	എങ്കിൽ (likely (vars.resize_exact (എണ്ണം)))
	{
	  വേണ്ടി (uint32_t i = 0; i < എണ്ണം; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    ദശാംശം value = values[i].value;
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

  തിരികെ font;
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
  തിരികെ (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;
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
  തിരികെ nullptr;
}

#endif

#endif
