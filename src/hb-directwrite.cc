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

#ضمن "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ضمن "hb-directwrite.hh"

#ضمن "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

ساكن مضمن فراغ free_static_directwrite_global ();

ساكن بنية hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  ساكن hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    إذا (unlikely (!global))
      إرجاع nullptr;
    إذا (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      إرجاع nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    إرجاع global;
  }
  ساكن فراغ destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  ساكن hb_directwrite_global_t * get_null ()
  {
    إرجاع nullptr;
  }
} static_directwrite_global;


ساكن مضمن
فراغ free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  إرجاع static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  auto *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_تعداد.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  font_File_مفتاح = mLoader->RegisterFontFileStream (هذا);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_مفتاح);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_رسالة (DIRECTWRITE, nullptr, __VA_ARGS__); \
    إرجاع nullptr; \
  } HB_STMT_END

  auto *global = get_directwrite_global ();
  إذا (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  إذا (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  auto hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_مفتاح, حجم (fontFileStream->font_File_مفتاح),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  إذا (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t العدد_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &العدد_Of_Faces);
  إذا (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  إرجاع fontFace;
}

بنية _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  فراغ *table_context;
};

ساكن فراغ
_hb_directwrite_table_data_release (فراغ *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

ساكن hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_تعداد;
  إذا (FAILED (dw_face->GetFiles(&file_تعداد, عدم)))
    إرجاع nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  إذا (unlikely (!files.resize_exact (file_تعداد)))
    إرجاع nullptr;
  إذا (FAILED (dw_face->GetFiles(&file_تعداد, files.arrayZ)))
    إرجاع nullptr;

  hb_blob_t *blob = nullptr;
  لكل (UINT32 i = 0; i < file_تعداد; i++)
  {
    LPCVOID reference_مفتاح;
    UINT32 reference_مفتاح_size;
    إذا (FAILED (files[i]->Get_Reference_مفتاح(&reference_مفتاح, &reference_مفتاح_size)))
      استمر;

    IDWriteFontFileLoader *loader;
    إذا (FAILED (files[i]->GetLoader(&loader)))
      استمر;
    
    IDWriteFontFileStream *stream;
    إذا (FAILED (loader->Create_Stream_From_مفتاح (reference_مفتاح, reference_مفتاح_size, &stream)))
    {
      loader->Release ();
      استمر;
    }

    UINT64 file_size;
    ثابت فراغ *fragment;
    فراغ *context;
    إذا (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      استمر;
    }
    blob = hb_blob_create ((ثابت حرف *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, عدم, عدم);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    اكسر;
  }

  إرجاع blob;
}

ساكن hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, فراغ *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  ثابت فراغ *data;
  uint32_t length;
  فراغ *table_context;
  BOOL exists;
  إذا (tag == HB_TAG_NONE)
    إرجاع _hb_directwrite_get_file_blob (dw_face);

  إذا (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    إرجاع nullptr;

  إذا (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    إرجاع nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (حجم (_hb_directwrite_font_table_context));
  إذا (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    إرجاع nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  إرجاع hb_blob_create ((ثابت حرف *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

ساكن فراغ
_hb_directwrite_face_release (فراغ *data)
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
  إذا (!dw_face)
    إرجاع hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_تعداد (face, dw_face->Get_Glyph_تعداد ());

  إرجاع face;
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
hb_directwrite_face_create_from_file_or_fail (ثابت حرف   *file_name,
					      unsigned صحيح  index)
{
  auto *blob = hb_blob_create_from_file_or_fail (file_name);
  إذا (unlikely (!blob))
    إرجاع nullptr;

  إرجاع hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      unsigned صحيح  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  إذا (unlikely (!dw_face))
    إرجاع nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  إذا (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    إرجاع face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  إرجاع face;
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
  إرجاع (IDWriteFontFace *) (ثابت فراغ *) face->data.directwrite;
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
  إرجاع hb_directwrite_face_get_dw_font_face (face);
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

  إذا (unlikely (hb_object_is_immutable (font)))
    إرجاع font;

  /* Copy font variations */
  إذا (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ**) &dw_face5)))
  {
    إذا (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_قيمة> values;
      uint32_t تعداد_2 = dw_face5->Get_Font_Axis_قيمة_تعداد ();
      إذا (likely (values.resize_exact (تعداد_2)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_قيم (values.arrayZ, تعداد_2)))
      {
	hb_vector_t<hb_variation_t> vars;
	إذا (likely (vars.resize_exact (تعداد_2)))
	{
	  لكل (uint32_t i = 0; i < تعداد_2; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    عائم value = values[i].value;
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

  إرجاع font;
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
  إرجاع (IDWriteFontFace *) (ثابت فراغ *) font->data.directwrite;
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
  إرجاع nullptr;
}

#endif

#endif
