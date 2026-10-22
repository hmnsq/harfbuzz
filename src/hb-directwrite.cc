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

#اگر_تعریف_شده HAVE_DIRECTWRITE

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
      أرجع اشاره_تهی;
    إذا (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      أرجع اشاره_تهی;
    }

    hb_atexit (free_static_directwrite_global);

    أرجع global;
  }
  ساكن فراغ destroy (hb_directwrite_global_t *l)
  {
    hb_directwrite_object_destroy (l);
  }
  ساكن hb_directwrite_global_t * get_null ()
  {
    أرجع اشاره_تهی;
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
  أرجع static_directwrite_global.get_unconst ();
}

DWrite_Font_File_جریان::DWrite_Font_File_جریان (hb_blob_t *blob)
{
  خودکار *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_شمار.init ();
  mLoader->جمع_کن_Ref ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_داده = (uint8_t *) hb_blob_get_داده (blob, &m_اندازه);
  font_File_کلید = mLoader->ثبات_Font_File_جریان (هذا);
}

DWrite_Font_File_جریان::~DWrite_Font_File_جریان()
{
  mLoader->Unregister_Font_File_جریان (font_File_کلید);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بیعلامت شاخص_2)
{
#تعریف_کن FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیام (DIRECTWRITE, اشاره_تهی, __VA_ARGS__); \
    أرجع اشاره_تهی; \
  } HB_STMT_انتها

  خودکار *global = get_directwrite_global ();
  إذا (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWrite_Font_File_جریان *font_File_جریان = hb_directwrite_object_create<DWrite_Font_File_جریان> (blob);
  إذا (unlikely (!font_File_جریان))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  خودکار hr = global->dwriteFactory->CreateCustomFontFileReference (&font_File_جریان->font_File_کلید, حجم (font_File_جریان->font_File_کلید),
								  global->fontFileLoader, &fontFile);

  font_File_جریان->Release ();

  إذا (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t عدد_از_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &عدد_از_Faces);
  إذا (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#لغو_تعریف FAIL

  IDWriteFontFace *fontFace = اشاره_تهی;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, شاخص_2,
					 DWRITE_FONT_SIMULATIONS_NONE, &fontFace);
  fontFile->Release ();

  أرجع fontFace;
}

بنية hb_directwrite_font_جدول_context {
  IDWriteFontFace *face;
  فراغ *جدول_context;
};

ساكن فراغ
hb_directwrite_جدول_داده_release (فراغ *داده)
{
  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) داده;
  context->face->Release_Font_جدول (context->جدول_context);
  hb_free (context);
}

ساكن hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_شمار;
  إذا (FAILED (dw_face->GetFiles(&file_شمار, عدم)))
    أرجع اشاره_تهی;

  hb_وکتور_t<IDWriteFontFile *> files;
  إذا (unlikely (!files.resize_exact (file_شمار)))
    أرجع اشاره_تهی;
  إذا (FAILED (dw_face->GetFiles(&file_شمار, files.arrayZ)))
    أرجع اشاره_تهی;

  hb_blob_t *blob = اشاره_تهی;
  لكل (UINT32 i = 0; i < file_شمار; i++)
  {
    LPCVOID reference_کلید;
    UINT32 reference_کلید_اندازه;
    إذا (FAILED (files[i]->Get_Reference_کلید(&reference_کلید, &reference_کلید_اندازه)))
      استمر;

    IDWriteFontFileLoader *loader;
    إذا (FAILED (files[i]->GetLoader(&loader)))
      استمر;
    
    IDWrite_Font_File_جریان *جریان;
    إذا (FAILED (loader->Create_جریان_From_کلید (reference_کلید, reference_کلید_اندازه, &جریان)))
    {
      loader->Release ();
      استمر;
    }

    UINT64 file_اندازه;
    ثابت فراغ *fragment;
    فراغ *context;
    إذا (FAILED (جریان->Get_File_اندازه(&file_اندازه)) ||
        FAILED (جریان->ReadFileFragment (&fragment, 0, file_اندازه, &context)))
    {
      loader->Release ();
      استمر;
    }
    blob = hb_blob_create ((ثابت حرف *) fragment, file_اندازه, HB_حافظه_نما_DUPLICATE, عدم, عدم);
    جریان->ReleaseFileFragment (context);
    loader->Release ();
    اكسر;
  }

  أرجع blob;
}

ساكن hb_blob_t *
hb_directwrite_reference_جدول (hb_face_t *face HB_استفادهنشده, hb_برچسب_t برچسب_2, فراغ *user_داده)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_داده);
  ثابت فراغ *داده;
  uint32_t درازی;
  فراغ *جدول_context;
  BOOL exists;
  إذا (برچسب_2 == HB_برچسب_NONE)
    أرجع _hb_directwrite_get_file_blob (dw_face);

  إذا (FAILED (dw_face->Try_Get_Font_جدول (hb_uint32_جابجا (برچسب_2), &داده,
					&درازی, &جدول_context, &exists)))
    أرجع اشاره_تهی;

  إذا (!داده || !exists || !درازی)
  {
    dw_face->Release_Font_جدول (جدول_context);
    أرجع اشاره_تهی;
  }

  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) hb_malloc (حجم (hb_directwrite_font_جدول_context));
  إذا (unlikely (!context))
  {
    dw_face->Release_Font_جدول (جدول_context);
    أرجع اشاره_تهی;
  }
  context->face = dw_face;
  context->جدول_context = جدول_context;

  أرجع hb_blob_create ((ثابت حرف *) داده, درازی, HB_حافظه_نما_READONLY,
			 context, hb_directwrite_جدول_داده_release);
}

ساكن فراغ
_hb_directwrite_face_release (فراغ *داده)
{
  ((IDWriteFontFace *) داده)->Release ();
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
    أرجع hb_face_get_خالی ();

  dw_face->جمع_کن_Ref ();
  hb_face_t *face = hb_face_create_for_tables (hb_directwrite_reference_جدول,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_مجموعه_شاخص (face, dw_face->Get_شاخص ());
  hb_face_مجموعه_glyph_شمار (face, dw_face->Get_Glyph_شمار ());

  أرجع face;
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
					      بیعلامت صحيح  شاخص_2)
{
  خودکار *blob = hb_blob_create_from_file_or_fail (file_name);
  إذا (unlikely (!blob))
    أرجع اشاره_تهی;

  أرجع hb_directwrite_face_create_from_blob_or_fail (blob, شاخص_2);
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
					      بیعلامت صحيح  شاخص_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, شاخص_2);
  إذا (unlikely (!dw_face))
    أرجع اشاره_تهی;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  إذا (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    أرجع face;
  }

  /* Let there be dragons here... */
  face->داده.directwrite.cmpexch (اشاره_تهی, (hb_directwrite_face_داده_t *) dw_face);

  أرجع face;
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
  أرجع (IDWriteFontFace *) (ثابت فراغ *) face->داده.directwrite;
}

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED

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
  أرجع hb_directwrite_face_get_dw_font_face (face);
}

#پایان_اگر

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
  IDWriteFontFace5 *dw_face5 = اشاره_تهی;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع font;

  /* Copy font variations */
  إذا (SUCCEEDED (dw_face->QueryInterface (__uuidof (IDWriteFontFace5), (فراغ**) &dw_face5)))
  {
    إذا (dw_face5->HasVariations ())
    {
      hb_وکتور_t<DWRITE_FONT_AXIS_مقدار> values;
      uint32_t شمار = dw_face5->Get_Font_Axis_مقدار_شمار ();
      إذا (likely (values.resize_exact (شمار)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_مقادیر (values.arrayZ, شمار)))
      {
	hb_وکتور_t<hb_variation_t> vars;
	إذا (likely (vars.resize_exact (شمار)))
	{
	  لكل (uint32_t i = 0; i < شمار; ++i)
	  {
	    hb_برچسب_t برچسب_2 = hb_uint32_جابجا (values[i].axis_برچسب);
	    عائم value = values[i].value;
	    vars[i] = {برچسب_2, value};
	  }
	  hb_font_مجموعه_variations (font, vars.arrayZ, vars.درازی);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->جمع_کن_Ref ();
  font->داده.directwrite.cmpexch (اشاره_تهی, (hb_directwrite_font_داده_t *) dw_face);

  أرجع font;
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
  أرجع (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;
}

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED

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
  أرجع اشاره_تهی;
}

#پایان_اگر

#پایان_اگر
