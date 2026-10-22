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

#که_تعریف_شوی HAVE_DIRECTWRITE

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
      أرجع تش_اشاره;
    إذا (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      أرجع تش_اشاره;
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
    أرجع تش_اشاره;
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

DWrite_Font_File_بهیر::DWrite_Font_File_بهیر (hb_blob_t *blob)
{
  خپلکاری *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_شمېر.init ();
  mLoader->جمع_کړه_Ref ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_معلومات = (uint8_t *) hb_blob_get_معلومات (blob, &m_اندازه);
  font_File_کلی = mLoader->ثبتوونکی_Font_File_بهیر (هذا);
}

DWrite_Font_File_بهیر::~DWrite_Font_File_بهیر()
{
  mLoader->Unregister_Font_File_بهیر (font_File_کلی);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بینښه شاخص_2)
{
#تعریف_کړه FAIL(...) \
  HB_STMT_پیل { \
    DEBUG_پیغام (DIRECTWRITE, تش_اشاره, __VA_ARGS__); \
    أرجع تش_اشاره; \
  } HB_STMT_پای_ټکی

  خپلکاری *global = get_directwrite_global ();
  إذا (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWrite_Font_File_بهیر *font_File_بهیر = hb_directwrite_object_create<DWrite_Font_File_بهیر> (blob);
  إذا (unlikely (!font_File_بهیر))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  خپلکاری hr = global->dwriteFactory->CreateCustomFontFileReference (&font_File_بهیر->font_File_کلی, حجم (font_File_بهیر->font_File_کلی),
								  global->fontFileLoader, &fontFile);

  font_File_بهیر->Release ();

  إذا (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t شمېره_د_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &شمېره_د_Faces);
  إذا (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#تعریف_لغوه FAIL

  IDWriteFontFace *fontFace = تش_اشاره;
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
hb_directwrite_جدول_معلومات_release (فراغ *معلومات)
{
  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) معلومات;
  context->face->Release_Font_جدول (context->جدول_context);
  hb_free (context);
}

ساكن hb_blob_t *
_hb_directwrite_get_file_blob (IDWriteFontFace *dw_face)
{
  UINT32 file_شمېر;
  إذا (FAILED (dw_face->GetFiles(&file_شمېر, عدم)))
    أرجع تش_اشاره;

  hb_ویکټور_t<IDWriteFontFile *> files;
  إذا (unlikely (!files.resize_exact (file_شمېر)))
    أرجع تش_اشاره;
  إذا (FAILED (dw_face->GetFiles(&file_شمېر, files.arrayZ)))
    أرجع تش_اشاره;

  hb_blob_t *blob = تش_اشاره;
  لكل (UINT32 i = 0; i < file_شمېر; i++)
  {
    LPCVOID reference_کلی;
    UINT32 reference_کلی_اندازه;
    إذا (FAILED (files[i]->Get_Reference_کلی(&reference_کلی, &reference_کلی_اندازه)))
      استمر;

    IDWriteFontFileLoader *loader;
    إذا (FAILED (files[i]->GetLoader(&loader)))
      استمر;
    
    IDWrite_Font_File_بهیر *بهیر;
    إذا (FAILED (loader->Create_بهیر_From_کلی (reference_کلی, reference_کلی_اندازه, &بهیر)))
    {
      loader->Release ();
      استمر;
    }

    UINT64 file_اندازه;
    ثابت فراغ *fragment;
    فراغ *context;
    إذا (FAILED (بهیر->Get_File_اندازه(&file_اندازه)) ||
        FAILED (بهیر->ReadFileFragment (&fragment, 0, file_اندازه, &context)))
    {
      loader->Release ();
      استمر;
    }
    blob = hb_blob_create ((ثابت حرف *) fragment, file_اندازه, HB_حافظه_ډېرپېښ_DUPLICATE, عدم, عدم);
    بهیر->ReleaseFileFragment (context);
    loader->Release ();
    اكسر;
  }

  أرجع blob;
}

ساكن hb_blob_t *
hb_directwrite_reference_جدول (hb_face_t *face HB_ناکارول_شوی, hb_نښان_t نښان_2, فراغ *user_معلومات)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_معلومات);
  ثابت فراغ *معلومات;
  uint32_t اوږدتیا;
  فراغ *جدول_context;
  BOOL exists;
  إذا (نښان_2 == HB_نښان_NONE)
    أرجع _hb_directwrite_get_file_blob (dw_face);

  إذا (FAILED (dw_face->Try_Get_Font_جدول (hb_uint32_بدلون (نښان_2), &معلومات,
					&اوږدتیا, &جدول_context, &exists)))
    أرجع تش_اشاره;

  إذا (!معلومات || !exists || !اوږدتیا)
  {
    dw_face->Release_Font_جدول (جدول_context);
    أرجع تش_اشاره;
  }

  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) hb_malloc (حجم (hb_directwrite_font_جدول_context));
  إذا (unlikely (!context))
  {
    dw_face->Release_Font_جدول (جدول_context);
    أرجع تش_اشاره;
  }
  context->face = dw_face;
  context->جدول_context = جدول_context;

  أرجع hb_blob_create ((ثابت حرف *) معلومات, اوږدتیا, HB_حافظه_ډېرپېښ_READONLY,
			 context, hb_directwrite_جدول_معلومات_release);
}

ساكن فراغ
_hb_directwrite_face_release (فراغ *معلومات)
{
  ((IDWriteFontFace *) معلومات)->Release ();
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
    أرجع hb_face_get_تشه ();

  dw_face->جمع_کړه_Ref ();
  hb_face_t *face = hb_face_create_for_tables (hb_directwrite_reference_جدول,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_سېټ_شاخص (face, dw_face->Get_شاخص ());
  hb_face_سېټ_glyph_شمېر (face, dw_face->Get_Glyph_شمېر ());

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
					      بینښه صحيح  شاخص_2)
{
  خپلکاری *blob = hb_blob_create_from_file_or_fail (file_name);
  إذا (unlikely (!blob))
    أرجع تش_اشاره;

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
					      بینښه صحيح  شاخص_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, شاخص_2);
  إذا (unlikely (!dw_face))
    أرجع تش_اشاره;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  إذا (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    أرجع face;
  }

  /* Let there be dragons here... */
  face->معلومات.directwrite.cmpexch (تش_اشاره, (hb_directwrite_face_معلومات_t *) dw_face);

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
  أرجع (IDWriteFontFace *) (ثابت فراغ *) face->معلومات.directwrite;
}

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED

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

#د_که_پای

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
  IDWriteFontFace5 *dw_face5 = تش_اشاره;

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
      hb_ویکټور_t<DWRITE_FONT_AXIS_ارزښت> values;
      uint32_t شمېر = dw_face5->Get_Font_Axis_ارزښت_شمېر ();
      إذا (likely (values.resize_exact (شمېر)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_ارزښتونه (values.arrayZ, شمېر)))
      {
	hb_ویکټور_t<hb_variation_t> vars;
	إذا (likely (vars.resize_exact (شمېر)))
	{
	  لكل (uint32_t i = 0; i < شمېر; ++i)
	  {
	    hb_نښان_t نښان_2 = hb_uint32_بدلون (values[i].axis_نښان);
	    عائم value = values[i].value;
	    vars[i] = {نښان_2, value};
	  }
	  hb_font_سېټ_variations (font, vars.arrayZ, vars.اوږدتیا);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->جمع_کړه_Ref ();
  font->معلومات.directwrite.cmpexch (تش_اشاره, (hb_directwrite_font_معلومات_t *) dw_face);

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
  أرجع (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;
}

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED

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
  أرجع تش_اشاره;
}

#د_که_پای

#د_که_پای
