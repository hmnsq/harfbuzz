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

#অন্তর্ভাৱ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#অন্তর্ভাৱ "hb-directwrite.hh"

#অন্তর্ভাৱ "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

স্থির অন্তঃস্থ শূন্য free_static_directwrite_global ();

স্থির সংরচনা hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  স্থির hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    যদি (unlikely (!global))
      ঘূৰাই nullptr;
    যদি (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      ঘূৰাই nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    ঘূৰাই global;
  }
  স্থির শূন্য destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  স্থির hb_directwrite_global_t * get_null ()
  {
    ঘূৰাই nullptr;
  }
} static_directwrite_global;


স্থির অন্তঃস্থ
শূন্য free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  ঘূৰাই static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  auto *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_গুনতি.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  font_File_চাবি = mLoader->RegisterFontFileStream (এতত্);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_চাবি);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_বাৰ্তা (DIRECTWRITE, nullptr, __VA_ARGS__); \
    ঘূৰাই nullptr; \
  } HB_STMT_END

  auto *global = get_directwrite_global ();
  যদি (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  যদি (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  auto hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_চাবি, পরিমাণ (fontFileStream->font_File_চাবি),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  যদি (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t সংখ্যা_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &সংখ্যা_Of_Faces);
  যদি (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  ঘূৰাই fontFace;
}

সংরচনা _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  শূন্য *table_context;
};

স্থির শূন্য
_hb_directwrite_table_data_release (শূন্য *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

স্থির hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_গুনতি;
  যদি (FAILED (dw_face->GetFiles(&file_গুনতি, রিক্ত)))
    ঘূৰাই nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  যদি (unlikely (!files.resize_exact (file_গুনতি)))
    ঘূৰাই nullptr;
  যদি (FAILED (dw_face->GetFiles(&file_গুনতি, files.arrayZ)))
    ঘূৰাই nullptr;

  hb_blob_t *blob = nullptr;
  বাবে (UINT32 i = 0; i < file_গুনতি; i++)
  {
    LPCVOID reference_চাবি;
    UINT32 reference_চাবি_size;
    যদি (FAILED (files[i]->Get_Reference_চাবি(&reference_চাবি, &reference_চাবি_size)))
      অনুৱর্ত;

    IDWriteFontFileLoader *loader;
    যদি (FAILED (files[i]->GetLoader(&loader)))
      অনুৱর্ত;
    
    IDWriteFontFileStream *stream;
    যদি (FAILED (loader->Create_Stream_From_চাবি (reference_চাবি, reference_চাবি_size, &stream)))
    {
      loader->Release ();
      অনুৱর্ত;
    }

    UINT64 file_size;
    ধ্রুৱ শূন্য *fragment;
    শূন্য *context;
    যদি (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      অনুৱর্ত;
    }
    blob = hb_blob_create ((ধ্রুৱ ৱর্ণ *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, রিক্ত, রিক্ত);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    ভঙ্গ;
  }

  ঘূৰাই blob;
}

স্থির hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, শূন্য *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  ধ্রুৱ শূন্য *data;
  uint32_t length;
  শূন্য *table_context;
  BOOL exists;
  যদি (tag == HB_TAG_NONE)
    ঘূৰাই _hb_directwrite_get_file_blob (dw_face);

  যদি (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    ঘূৰাই nullptr;

  যদি (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    ঘূৰাই nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (পরিমাণ (_hb_directwrite_font_table_context));
  যদি (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    ঘূৰাই nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  ঘূৰাই hb_blob_create ((ধ্রুৱ ৱর্ণ *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

স্থির শূন্য
_hb_directwrite_face_release (শূন্য *data)
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
  যদি (!dw_face)
    ঘূৰাই hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_গুনতি (face, dw_face->Get_Glyph_গুনতি ());

  ঘূৰাই face;
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
hb_directwrite_face_create_from_file_or_fail (ধ্রুৱ ৱর্ণ   *file_name,
					      unsigned পূর্ণাঙ্ক  index)
{
  auto *blob = hb_blob_create_from_file_or_fail (file_name);
  যদি (unlikely (!blob))
    ঘূৰাই nullptr;

  ঘূৰাই hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      unsigned পূর্ণাঙ্ক  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  যদি (unlikely (!dw_face))
    ঘূৰাই nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  যদি (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    ঘূৰাই face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  ঘূৰাই face;
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
  ঘূৰাই (IDWriteFontFace *) (ধ্রুৱ শূন্য *) face->data.directwrite;
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
  ঘূৰাই hb_directwrite_face_get_dw_font_face (face);
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

  যদি (unlikely (hb_object_is_immutable (font)))
    ঘূৰাই font;

  /* Copy font variations */
  যদি (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (শূন্য**) &dw_face5)))
  {
    যদি (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_মান> values;
      uint32_t গুনতি = dw_face5->Get_Font_Axis_মান_গুনতি ();
      যদি (likely (values.resize_exact (গুনতি)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_মানসমূহ (values.arrayZ, গুনতি)))
      {
	hb_vector_t<hb_variation_t> vars;
	যদি (likely (vars.resize_exact (গুনতি)))
	{
	  বাবে (uint32_t i = 0; i < গুনতি; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    ভিন্ন value = values[i].value;
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

  ঘূৰাই font;
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
  ঘূৰাই (IDWriteFontFace *) (ধ্রুৱ শূন্য *) font->data.directwrite;
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
  ঘূৰাই nullptr;
}

#endif

#endif
