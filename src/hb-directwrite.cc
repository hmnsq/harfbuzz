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

#include "hb.hh"

#ifdef HAVE_DIRECTWRITE

#include "hb-directwrite.hh"

#include "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

static anta_hstha void free_static_directwrite_global ();

static sa_nracanaa hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  static hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    အကယ်၍ (unlikely (!global))
      ပြန်ပေး nullptr;
    အကယ်၍ (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      ပြန်ပေး nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    ပြန်ပေး global;
  }
  static void destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  static hb_directwrite_global_t * get_null ()
  {
    ပြန်ပေး nullptr;
  }
} static_directwrite_global;


static anta_hstha
void free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  ပြန်ပေး static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  auto *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  mRefCount.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  fontFileKey = mLoader->RegisterFontFileStream (this);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (fontFileKey);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_MSG (DIRECTWRITE, nullptr, __VA_ARGS__); \
    ပြန်ပေး nullptr; \
  } HB_STMT_END

  auto *global = get_directwrite_global ();
  အကယ်၍ (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  အကယ်၍ (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  auto hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->fontFileKey, parimaa_na (fontFileStream->fontFileKey),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  အကယ်၍ (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t numberOfFaces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &numberOfFaces);
  အကယ်၍ (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  ပြန်ပေး fontFace;
}

sa_nracanaa _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  void *table_context;
};

static void
_hb_directwrite_table_data_release (void *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

static hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_count;
  အကယ်၍ (FAILED (dw_face->GetFiles(&file_count, NULL)))
    ပြန်ပေး nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  အကယ်၍ (unlikely (!files.resize_exact (file_count)))
    ပြန်ပေး nullptr;
  အကယ်၍ (FAILED (dw_face->GetFiles(&file_count, files.arrayZ)))
    ပြန်ပေး nullptr;

  hb_blob_t *blob = nullptr;
  အတွက် (UINT32 i = 0; i < file_count; i++)
  {
    LPCVOID reference_key;
    UINT32 reference_key_size;
    အကယ်၍ (FAILED (files[i]->GetReferenceKey(&reference_key, &reference_key_size)))
      continue;

    IDWriteFontFileLoader *loader;
    အကယ်၍ (FAILED (files[i]->GetLoader(&loader)))
      continue;
    
    IDWriteFontFileStream *stream;
    အကယ်၍ (FAILED (loader->CreateStreamFromKey (reference_key, reference_key_size, &stream)))
    {
      loader->Release ();
      continue;
    }

    UINT64 file_size;
    const void *fragment;
    void *context;
    အကယ်၍ (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      continue;
    }
    blob = hb_blob_create ((const var_na *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, NULL, NULL);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    bha_nga;
  }

  ပြန်ပေး blob;
}

static hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, void *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  const void *data;
  uint32_t length;
  void *table_context;
  BOOL exists;
  အကယ်၍ (tag == HB_TAG_NONE)
    ပြန်ပေး _hb_directwrite_get_file_blob (dw_face);

  အကယ်၍ (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    ပြန်ပေး nullptr;

  အကယ်၍ (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    ပြန်ပေး nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (parimaa_na (_hb_directwrite_font_table_context));
  အကယ်၍ (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    ပြန်ပေး nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  ပြန်ပေး hb_blob_create ((const var_na *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

static void
_hb_directwrite_face_release (void *data)
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
  အကယ်၍ (!dw_face)
    ပြန်ပေး hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_count (face, dw_face->GetGlyphCount ());

  ပြန်ပေး face;
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
hb_directwrite_face_create_from_file_or_fail (const var_na   *file_name,
					      unsigned ကိန်းပြည့်  index)
{
  auto *blob = hb_blob_create_from_file_or_fail (file_name);
  အကယ်၍ (unlikely (!blob))
    ပြန်ပေး nullptr;

  ပြန်ပေး hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      unsigned ကိန်းပြည့်  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  အကယ်၍ (unlikely (!dw_face))
    ပြန်ပေး nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  အကယ်၍ (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    ပြန်ပေး face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  ပြန်ပေး face;
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
  ပြန်ပေး (IDWriteFontFace *) (const void *) face->data.directwrite;
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
  ပြန်ပေး hb_directwrite_face_get_dw_font_face (face);
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

  အကယ်၍ (unlikely (hb_object_is_immutable (font)))
    ပြန်ပေး font;

  /* Copy font variations */
  အကယ်၍ (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (void**) &dw_face5)))
  {
    အကယ်၍ (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_VALUE> values;
      uint32_t count = dw_face5->GetFontAxisValueCount ();
      အကယ်၍ (likely (values.resize_exact (count)) &&
	  SUCCEEDED (dw_face5->GetFontAxisValues (values.arrayZ, count)))
      {
	hb_vector_t<hb_variation_t> vars;
	အကယ်၍ (likely (vars.resize_exact (count)))
	{
	  အတွက် (uint32_t i = 0; i < count; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    float value = values[i].value;
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

  ပြန်ပေး font;
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
  ပြန်ပေး (IDWriteFontFace *) (const void *) font->data.directwrite;
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
  ပြန်ပေး nullptr;
}

#endif

#endif
