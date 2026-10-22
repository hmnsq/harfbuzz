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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_DIRECTWRITE

#अन्तर्भाव "hb-directwrite.hh"

#अन्तर्भाव "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

स्थिर अन्तःस्थ शून्य free_static_directwrite_global ();

स्थिर संरचना hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  स्थिर hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    यदि (unlikely (!global))
      फर्काऊ nullptr;
    यदि (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      फर्काऊ nullptr;
    }

    hb_atexit (free_static_directwrite_global);

    फर्काऊ global;
  }
  स्थिर शून्य destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  स्थिर hb_directwrite_global_t * get_null ()
  {
    फर्काऊ nullptr;
  }
} static_directwrite_global;


स्थिर अन्तःस्थ
शून्य free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  फर्काऊ static_directwrite_global.get_unconst ();
}

DWriteFontFileStream::DWriteFontFileStream (hb_blob_t *blob)
{
  स्वतः *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_गन्ती.init ();
  mLoader->AddRef ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  mData = (uint8_t *) hb_blob_get_data (blob, &mSize);
  font_File_साँचो = mLoader->RegisterFontFileStream (एतत्);
}

DWriteFontFileStream::~DWriteFontFileStream()
{
  mLoader->UnregisterFontFileStream (font_File_साँचो);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, अचिन्हित index)
{
#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_सन्देश (DIRECTWRITE, nullptr, __VA_ARGS__); \
    फर्काऊ nullptr; \
  } HB_STMT_END

  स्वतः *global = get_directwrite_global ();
  यदि (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWriteFontFileStream *fontFileStream = hb_directwrite_object_create<DWriteFontFileStream> (blob);
  यदि (unlikely (!fontFileStream))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  स्वतः hr = global->dwriteFactory->CreateCustomFontFileReference (&fontFileStream->font_File_साँचो, परिमाण (fontFileStream->font_File_साँचो),
								  global->fontFileLoader, &fontFile);

  fontFileStream->Release ();

  यदि (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t सङ्ख्या_Of_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &सङ्ख्या_Of_Faces);
  यदि (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#undef FAIL

  IDWriteFontFace *fontFace = nullptr;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, index,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  फर्काऊ fontFace;
}

संरचना _hb_directwrite_font_table_context {
  IDWriteFontFace *face;
  शून्य *table_context;
};

स्थिर शून्य
_hb_directwrite_table_data_release (शून्य *data)
{
  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) data;
  context->face->ReleaseFontTable (context->table_context);
  hb_free (context);
}

स्थिर hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_गन्ती;
  यदि (FAILED (dw_face->GetFiles(&file_गन्ती, रिक्त)))
    फर्काऊ nullptr;

  hb_vector_t<IDWriteFontFile *> files;
  यदि (unlikely (!files.resize_exact (file_गन्ती)))
    फर्काऊ nullptr;
  यदि (FAILED (dw_face->GetFiles(&file_गन्ती, files.arrayZ)))
    फर्काऊ nullptr;

  hb_blob_t *blob = nullptr;
  लागि (UINT32 i = 0; i < file_गन्ती; i++)
  {
    LPCVOID reference_साँचो;
    UINT32 reference_साँचो_size;
    यदि (FAILED (files[i]->Get_Reference_साँचो(&reference_साँचो, &reference_साँचो_size)))
      जारी;

    IDWriteFontFileLoader *loader;
    यदि (FAILED (files[i]->GetLoader(&loader)))
      जारी;
    
    IDWriteFontFileStream *stream;
    यदि (FAILED (loader->Create_Stream_From_साँचो (reference_साँचो, reference_साँचो_size, &stream)))
    {
      loader->Release ();
      जारी;
    }

    UINT64 file_size;
    अचर शून्य *fragment;
    शून्य *context;
    यदि (FAILED (stream->GetFileSize(&file_size)) ||
        FAILED (stream->ReadFileFragment (&fragment, 0, file_size, &context)))
    {
      loader->Release ();
      जारी;
    }
    blob = hb_blob_create ((अचर वर्ण *) fragment, file_size, HB_MEMORY_MODE_DUPLICATE, रिक्त, रिक्त);
    stream->ReleaseFileFragment (context);
    loader->Release ();
    रोक;
  }

  फर्काऊ blob;
}

स्थिर hb_blob_t *
_hb_directwrite_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, शून्य *user_data)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_data);
  अचर शून्य *data;
  uint32_t length;
  शून्य *table_context;
  BOOL exists;
  यदि (tag == HB_TAG_NONE)
    फर्काऊ _hb_directwrite_get_file_blob (dw_face);

  यदि (FAILED (dw_face->TryGetFontTable (hb_uint32_swap (tag), &data,
					&length, &table_context, &exists)))
    फर्काऊ nullptr;

  यदि (!data || !exists || !length)
  {
    dw_face->ReleaseFontTable (table_context);
    फर्काऊ nullptr;
  }

  _hb_directwrite_font_table_context *context = (_hb_directwrite_font_table_context *) hb_malloc (परिमाण (_hb_directwrite_font_table_context));
  यदि (unlikely (!context))
  {
    dw_face->ReleaseFontTable (table_context);
    फर्काऊ nullptr;
  }
  context->face = dw_face;
  context->table_context = table_context;

  फर्काऊ hb_blob_create ((अचर वर्ण *) data, length, HB_MEMORY_MODE_READONLY,
			 context, _hb_directwrite_table_data_release);
}

स्थिर शून्य
_hb_directwrite_face_release (शून्य *data)
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
  यदि (!dw_face)
    फर्काऊ hb_face_get_empty ();

  dw_face->AddRef ();
  hb_face_t *face = hb_face_create_for_tables (_hb_directwrite_reference_table,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_set_index (face, dw_face->GetIndex ());
  hb_face_set_glyph_गन्ती (face, dw_face->Get_Glyph_गन्ती ());

  फर्काऊ face;
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
hb_directwrite_face_create_from_file_or_fail (अचर वर्ण   *file_name,
					      अचिन्हित पूर्णाङ्क  index)
{
  स्वतः *blob = hb_blob_create_from_file_or_fail (file_name);
  यदि (unlikely (!blob))
    फर्काऊ nullptr;

  फर्काऊ hb_directwrite_face_create_from_blob_or_fail (blob, index);
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
					      अचिन्हित पूर्णाङ्क  index)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, index);
  यदि (unlikely (!dw_face))
    फर्काऊ nullptr;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  यदि (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    फर्काऊ face;
  }

  /* Let there be dragons here... */
  face->data.directwrite.cmpexch (nullptr, (hb_directwrite_face_data_t *) dw_face);

  फर्काऊ face;
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
  फर्काऊ (IDWriteFontFace *) (अचर शून्य *) face->data.directwrite;
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
  फर्काऊ hb_directwrite_face_get_dw_font_face (face);
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

  यदि (unlikely (hb_object_is_immutable (font)))
    फर्काऊ font;

  /* Copy font variations */
  यदि (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (शून्य**) &dw_face5)))
  {
    यदि (dw_face5->HasVariations ())
    {
      hb_vector_t<DWRITE_FONT_AXIS_मान> values;
      uint32_t गन्ती = dw_face5->Get_Font_Axis_मान_गन्ती ();
      यदि (likely (values.resize_exact (गन्ती)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_मानहरू (values.arrayZ, गन्ती)))
      {
	hb_vector_t<hb_variation_t> vars;
	यदि (likely (vars.resize_exact (गन्ती)))
	{
	  लागि (uint32_t i = 0; i < गन्ती; ++i)
	  {
	    hb_tag_t tag = hb_uint32_swap (values[i].axisTag);
	    भिन्न value = values[i].value;
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

  फर्काऊ font;
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
  फर्काऊ (IDWriteFontFace *) (अचर शून्य *) font->data.directwrite;
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
  फर्काऊ nullptr;
}

#endif

#endif
