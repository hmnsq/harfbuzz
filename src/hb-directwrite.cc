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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_DIRECTWRITE

#شامل "hb-directwrite.hh"

#شامل "hb-font.hh"


/**
 * SECTION:hb-directwrite
 * @title: hb-directwrite
 * @short_description: DirectWrite integration
 * @include: hb-directwrite.h
 *
 * Functions for using HarfBuzz with DirectWrite fonts.
 **/

ساکن اندرونی خالی free_static_directwrite_global ();

ساکن ساخت hb_directwrite_global_lazy_loader_t : hb_lazy_loader_t<hb_directwrite_global_t,
								     hb_directwrite_global_lazy_loader_t>
{
  ساکن hb_directwrite_global_t * create ()
  {
    hb_directwrite_global_t *global = hb_directwrite_object_create<hb_directwrite_global_t> ();

    اگر (unlikely (!global))
      واپس خالی_اشاریہ;
    اگر (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      واپس خالی_اشاریہ;
    }

    hb_atexit (free_static_directwrite_global);

    واپس global;
  }
  ساکن خالی destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  ساکن hb_directwrite_global_t * get_null ()
  {
    واپس خالی_اشاریہ;
  }
} static_directwrite_global;


ساکن اندرونی
خالی free_static_directwrite_global ()
{
  static_directwrite_global.free_instance ();
}

hb_directwrite_global_t *
get_directwrite_global ()
{
  واپس static_directwrite_global.get_unconst ();
}

DWrite_Font_File_دھارا::DWrite_Font_File_دھارا (hb_blob_t *blob)
{
  خودکار *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_گنتی.init ();
  mLoader->جوڑو_Ref ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_ڈیٹا = (uint8_t *) hb_blob_get_ڈیٹا (blob, &m_سائز);
  font_File_کلید = mLoader->رجسٹر_Font_File_دھارا (یہ);
}

DWrite_Font_File_دھارا::~DWrite_Font_File_دھارا()
{
  mLoader->Unregister_Font_File_دھارا (font_File_کلید);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, غیرعلامتی اشاریہ_2)
{
#تعریف_کرو FAIL(...) \
  HB_STMT_آغاز { \
    DEBUG_پیغام (DIRECTWRITE, خالی_اشاریہ, __VA_ARGS__); \
    واپس خالی_اشاریہ; \
  } HB_STMT_انتہا

  خودکار *global = get_directwrite_global ();
  اگر (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWrite_Font_File_دھارا *font_File_دھارا = hb_directwrite_object_create<DWrite_Font_File_دھارا> (blob);
  اگر (unlikely (!font_File_دھارا))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  خودکار hr = global->dwriteFactory->CreateCustomFontFileReference (&font_File_دھارا->font_File_کلید, حجم (font_File_دھارا->font_File_کلید),
								  global->fontFileLoader, &fontFile);

  font_File_دھارا->Release ();

  اگر (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t نمبر_کا_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &نمبر_کا_Faces);
  اگر (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#تعریف_منسوخ FAIL

  IDWriteFontFace *fontFace = خالی_اشاریہ;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, اشاریہ_2,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  واپس fontFace;
}

ساخت hb_directwrite_font_جدول_context {
  IDWriteFontFace *face;
  خالی *جدول_context;
};

ساکن خالی
hb_directwrite_جدول_ڈیٹا_release (خالی *ڈیٹا)
{
  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) ڈیٹا;
  context->face->Release_Font_جدول (context->جدول_context);
  hb_free (context);
}

ساکن hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_گنتی;
  اگر (FAILED (dw_face->GetFiles(&file_گنتی, تہی)))
    واپس خالی_اشاریہ;

  hb_سمتیہ_t<IDWriteFontFile *> files;
  اگر (unlikely (!files.resize_exact (file_گنتی)))
    واپس خالی_اشاریہ;
  اگر (FAILED (dw_face->GetFiles(&file_گنتی, files.arrayZ)))
    واپس خالی_اشاریہ;

  hb_blob_t *blob = خالی_اشاریہ;
  برائے (UINT32 i = 0; i < file_گنتی; i++)
  {
    LPCVOID reference_کلید;
    UINT32 reference_کلید_سائز;
    اگر (FAILED (files[i]->Get_Reference_کلید(&reference_کلید, &reference_کلید_سائز)))
      جاری;

    IDWriteFontFileLoader *loader;
    اگر (FAILED (files[i]->GetLoader(&loader)))
      جاری;
    
    IDWrite_Font_File_دھارا *دھارا;
    اگر (FAILED (loader->Create_دھارا_From_کلید (reference_کلید, reference_کلید_سائز, &دھارا)))
    {
      loader->Release ();
      جاری;
    }

    UINT64 file_سائز;
    ثابت خالی *fragment;
    خالی *context;
    اگر (FAILED (دھارا->Get_File_سائز(&file_سائز)) ||
        FAILED (دھارا->ReadFileFragment (&fragment, 0, file_سائز, &context)))
    {
      loader->Release ();
      جاری;
    }
    blob = hb_blob_create ((ثابت حرف *) fragment, file_سائز, HB_حافظہ_منوال_DUPLICATE, تہی, تہی);
    دھارا->ReleaseFileFragment (context);
    loader->Release ();
    توڑو;
  }

  واپس blob;
}

ساکن hb_blob_t *
hb_directwrite_reference_جدول (hb_face_t *face HB_غیر_مستعمل, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_ڈیٹا);
  ثابت خالی *ڈیٹا;
  uint32_t لمبائی;
  خالی *جدول_context;
  BOOL exists;
  اگر (لیبل_2 == HB_لیبل_NONE)
    واپس _hb_directwrite_get_file_blob (dw_face);

  اگر (FAILED (dw_face->Try_Get_Font_جدول (hb_uint32_تبادلہ (لیبل_2), &ڈیٹا,
					&لمبائی, &جدول_context, &exists)))
    واپس خالی_اشاریہ;

  اگر (!ڈیٹا || !exists || !لمبائی)
  {
    dw_face->Release_Font_جدول (جدول_context);
    واپس خالی_اشاریہ;
  }

  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) hb_malloc (حجم (hb_directwrite_font_جدول_context));
  اگر (unlikely (!context))
  {
    dw_face->Release_Font_جدول (جدول_context);
    واپس خالی_اشاریہ;
  }
  context->face = dw_face;
  context->جدول_context = جدول_context;

  واپس hb_blob_create ((ثابت حرف *) ڈیٹا, لمبائی, HB_حافظہ_منوال_READONLY,
			 context, hb_directwrite_جدول_ڈیٹا_release);
}

ساکن خالی
_hb_directwrite_face_release (خالی *ڈیٹا)
{
  ((IDWriteFontFace *) ڈیٹا)->Release ();
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
  اگر (!dw_face)
    واپس hb_face_get_خالی_خانے ();

  dw_face->جوڑو_Ref ();
  hb_face_t *face = hb_face_create_for_tables (hb_directwrite_reference_جدول,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_سیٹ_اشاریہ (face, dw_face->Get_اشاریہ ());
  hb_face_سیٹ_glyph_گنتی (face, dw_face->Get_Glyph_گنتی ());

  واپس face;
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
					      غیرعلامتی صحیح  اشاریہ_2)
{
  خودکار *blob = hb_blob_create_from_file_or_fail (file_name);
  اگر (unlikely (!blob))
    واپس خالی_اشاریہ;

  واپس hb_directwrite_face_create_from_blob_or_fail (blob, اشاریہ_2);
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
					      غیرعلامتی صحیح  اشاریہ_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, اشاریہ_2);
  اگر (unlikely (!dw_face))
    واپس خالی_اشاریہ;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  اگر (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    واپس face;
  }

  /* Let there be dragons here... */
  face->ڈیٹا.directwrite.cmpexch (خالی_اشاریہ, (hb_directwrite_face_ڈیٹا_t *) dw_face);

  واپس face;
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
  واپس (IDWriteFontFace *) (ثابت خالی *) face->ڈیٹا.directwrite;
}

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED

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
  واپس hb_directwrite_face_get_dw_font_face (face);
}

#اگر_ختم

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
  IDWriteFontFace5 *dw_face5 = خالی_اشاریہ;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  اگر (unlikely (hb_object_is_immutable (font)))
    واپس font;

  /* Copy font variations */
  اگر (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (خالی**) &dw_face5)))
  {
    اگر (dw_face5->HasVariations ())
    {
      hb_سمتیہ_t<DWRITE_FONT_AXIS_قدر> values;
      uint32_t گنتی = dw_face5->Get_Font_Axis_قدر_گنتی ();
      اگر (likely (values.resize_exact (گنتی)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_اقدار (values.arrayZ, گنتی)))
      {
	hb_سمتیہ_t<hb_variation_t> vars;
	اگر (likely (vars.resize_exact (گنتی)))
	{
	  برائے (uint32_t i = 0; i < گنتی; ++i)
	  {
	    hb_لیبل_t لیبل_2 = hb_uint32_تبادلہ (values[i].axis_لیبل);
	    اعشاری value = values[i].value;
	    vars[i] = {لیبل_2, value};
	  }
	  hb_font_سیٹ_variations (font, vars.arrayZ, vars.لمبائی);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->جوڑو_Ref ();
  font->ڈیٹا.directwrite.cmpexch (خالی_اشاریہ, (hb_directwrite_font_ڈیٹا_t *) dw_face);

  واپس font;
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
  واپس (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;
}

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED

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
  واپس خالی_اشاریہ;
}

#اگر_ختم

#اگر_ختم
