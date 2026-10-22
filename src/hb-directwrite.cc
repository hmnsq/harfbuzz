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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#அந்தர்பாவ "hb-directwrite.hh"

#அந்தர்பாவ "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

நிலையான உள்வரி வெறுமை free_static_directwrite_global ();

நிலையான கட்டமைப்பு hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  நிலையான hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    எனில் (unlikely (!global))
      திரும்பு nullptr;
    எனில் (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      திரும்பு nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    திரும்பு global;
  }
  நிலையான வெறுமை destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  நிலையான hb_directwrite_global_t * get_null ()
  {
    திரும்பு nullptr;
  }
} static_directwrite_global;


நிலையான உள்வரி
வெறுமை free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  திரும்பு static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  auto *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_மொத்தம்.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  font_File_சாவி = mLoader->RegisterFontFileStream (இது);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_சாவி);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_செய்தி (DIRECTWRITE, nullptr, __VA_ARGS__); \
    திரும்பு nullptr; \
  } HB_STMT_END

  auto *global = get_directwrite_global ();
  எனில் (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  எனில் (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  auto hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_சாவி, அளவு (fontFileStream->font_File_சாவி),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  எனில் (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t எண்_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &எண்_Of_Faces);
  எனில் (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  திரும்பு fontFace;
}

கட்டமைப்பு _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  வெறுமை *table_context;
};

நிலையான வெறுமை
_hb_directwrite_table_data_release (வெறுமை *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

நிலையான hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_மொத்தம்;
  எனில் (FAILED (dw_face->GetFiles(&file_மொத்தம், வெற்று)))
    திரும்பு nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  எனில் (unlikely (!files.resize_exact (file_மொத்தம்)))
    திரும்பு nullptr;
  எனில் (FAILED (dw_face->GetFiles(&file_மொத்தம், files.arrayZ)))
    திரும்பு nullptr;

  hb_blob_t *blob = nullptr;
  க்கு (UINT32 i = 0; i < file_மொத்தம்; i++)
  {
    LPCVOID reference_சாவி;
    UINT32 reference_சாவி_size;
    எனில் (FAILED (files[i]->Get_Reference_சாவி(&reference_சாவி, &reference_சாவி_size)))
      தொடர்;

    IDWriteFontFileLoader *loader;
    எனில் (FAILED (files[i]->GetLoader(&loader)))
      தொடர்;
    
    IDWriteFontFileStream *stream;
    எனில் (FAILED (loader->Create_Stream_From_சாவி (reference_சாவி, reference_சாவி_size, &stream)))
    {
      loader->Release ();
      தொடர்;
    }

    UINT64 file_size;
    மாறிலி வெறுமை *fragment;
    வெறுமை *context;
    எனில் (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      தொடர்;
    }
    blob = hb_blob_create ((மாறிலி எழுத்து *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, வெற்று, வெற்று);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    நிறுத்து;
  }

  திரும்பு blob;
}

நிலையான hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, வெறுமை *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  மாறிலி வெறுமை *data;
  uint32_t length;
  வெறுமை *table_context;
  BOOL exists;
  எனில் (tag == HB_TAG_NONE)
    திரும்பு _hb_directwrite_get_file_blob (dw_face);

  எனில் (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    திரும்பு nullptr;

  எனில் (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    திரும்பு nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (அளவு (_hb_directwrite_font_table_context));
  எனில் (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    திரும்பு nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  திரும்பு hb_blob_create ((மாறிலி எழுத்து *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

நிலையான வெறுமை
_hb_directwrite_face_release (வெறுமை *data)
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
  எனில் (!dw_face)
    திரும்பு hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_மொத்தம் (face, dw_face->Get_Glyph_மொத்தம் ());

  திரும்பு face;
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
hb_directwrite_face_create_from_file_or_fail (மாறிலி எழுத்து   *file_name,
					      unsigned முழு  index)
{
  auto *blob = hb_blob_create_from_file_or_fail (file_name);
  எனில் (unlikely (!blob))
    திரும்பு nullptr;

  திரும்பு hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      unsigned முழு  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  எனில் (unlikely (!dw_face))
    திரும்பு nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  எனில் (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    திரும்பு face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  திரும்பு face;
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
  திரும்பு (IDWriteFontFace *) (மாறிலி வெறுமை *) face->data.directwrite;
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
  திரும்பு hb_directwrite_face_get_dw_font_face (face);
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

  எனில் (unlikely (hb_object_is_immutable (font)))
    திரும்பு font;

  /* Copy font variations */
  எனில் (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (வெறுமை**) &dw_face5)))
  {
    எனில் (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_மதிப்பு> values;
      uint32_t மொத்தம் = dw_face5->Get_Font_Axis_மதிப்பு_மொத்தம் ();
      எனில் (likely (values.resize_exact (மொத்தம்)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_மதிப்புகள் (values.arrayZ, மொத்தம்)))
      {
	hb_vector_t<hb_variation_t> vars;
	எனில் (likely (vars.resize_exact (மொத்தம்)))
	{
	  க்கு (uint32_t i = 0; i < மொத்தம்; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    தசம value = values[i].value;
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

  திரும்பு font;
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
  திரும்பு (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;
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
  திரும்பு nullptr;
}

#endif

#endif
