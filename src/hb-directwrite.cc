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

#جيڪڏهن_وصف_ٿيل HAVE_DIRECTWRITE

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
      أرجع خالي_اشارو;
    إذا (unlikely (!global->success))
    {
      hb_directwrite_object_destroy (global);
      أرجع خالي_اشارو;
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
    أرجع خالي_اشارو;
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

DWrite_Font_File_وهڪرو::DWrite_Font_File_وهڪرو (hb_blob_t *blob)
{
  خودڪار *global = get_directwrite_global ();
  mLoader = global->fontFileLoader;
  m_Ref_ڳڻپ.init ();
  mLoader->جوڙيو_Ref ();
  hb_blob_make_immutable (blob);
  mBlob = hb_blob_reference (blob);
  m_معلومات = (uint8_t *) hb_blob_get_معلومات (blob, &m_سائيز);
  font_File_ڪنجي = mLoader->رجسٽر_Font_File_وهڪرو (هذا);
}

DWrite_Font_File_وهڪرو::~DWrite_Font_File_وهڪرو()
{
  mLoader->Unregister_Font_File_وهڪرو (font_File_ڪنجي);
  mLoader->Release ();
  hb_blob_destroy (mBlob);
}

IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بنانشان اشاريو_2)
{
#وصف_ڪريو FAIL(...) \
  HB_STMT_شروعات { \
    DEBUG_نياپو (DIRECTWRITE, خالي_اشارو, __VA_ARGS__); \
    أرجع خالي_اشارو; \
  } HB_STMT_پڇاڙي

  خودڪار *global = get_directwrite_global ();
  إذا (unlikely (!global))
    FAIL ("Couldn't load DirectWrite!");

  DWrite_Font_File_وهڪرو *font_File_وهڪرو = hb_directwrite_object_create<DWrite_Font_File_وهڪرو> (blob);
  إذا (unlikely (!font_File_وهڪرو))
    FAIL ("Failed to allocate font file stream!");

  IDWriteFontFile *fontFile;
  خودڪار hr = global->dwriteFactory->CreateCustomFontFileReference (&font_File_وهڪرو->font_File_ڪنجي, حجم (font_File_وهڪرو->font_File_ڪنجي),
								  global->fontFileLoader, &fontFile);

  font_File_وهڪرو->Release ();

  إذا (FAILED (hr))
    FAIL ("Failed to load font file from data!");

  BOOL isSupported;
  DWRITE_FONT_FILE_TYPE fileType;
  DWRITE_FONT_FACE_TYPE faceType;
  uint32_t عدد_جو_Faces;
  hr = fontFile->Analyze (&isSupported, &fileType, &faceType, &عدد_جو_Faces);
  إذا (FAILED (hr) || !isSupported)
  {
    fontFile->Release ();
    FAIL ("Font file is not supported.");
  }

#وصف_رد FAIL

  IDWriteFontFace *fontFace = خالي_اشارو;
  global->dwriteFactory->CreateFontFace (faceType, 1, &fontFile, اشاريو_2,
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
  UINT32 file_ڳڻپ;
  إذا (FAILED (dw_face->GetFiles(&file_ڳڻپ, عدم)))
    أرجع خالي_اشارو;

  hb_ويڪٽر_t<IDWriteFontFile *> files;
  إذا (unlikely (!files.resize_exact (file_ڳڻپ)))
    أرجع خالي_اشارو;
  إذا (FAILED (dw_face->GetFiles(&file_ڳڻپ, files.arrayZ)))
    أرجع خالي_اشارو;

  hb_blob_t *blob = خالي_اشارو;
  لكل (UINT32 i = 0; i < file_ڳڻپ; i++)
  {
    LPCVOID reference_ڪنجي;
    UINT32 reference_ڪنجي_سائيز;
    إذا (FAILED (files[i]->Get_Reference_ڪنجي(&reference_ڪنجي, &reference_ڪنجي_سائيز)))
      استمر;

    IDWriteFontFileLoader *loader;
    إذا (FAILED (files[i]->GetLoader(&loader)))
      استمر;
    
    IDWrite_Font_File_وهڪرو *وهڪرو;
    إذا (FAILED (loader->Create_وهڪرو_From_ڪنجي (reference_ڪنجي, reference_ڪنجي_سائيز, &وهڪرو)))
    {
      loader->Release ();
      استمر;
    }

    UINT64 file_سائيز;
    ثابت فراغ *fragment;
    فراغ *context;
    إذا (FAILED (وهڪرو->Get_File_سائيز(&file_سائيز)) ||
        FAILED (وهڪرو->ReadFileFragment (&fragment, 0, file_سائيز, &context)))
    {
      loader->Release ();
      استمر;
    }
    blob = hb_blob_create ((ثابت حرف *) fragment, file_سائيز, HB_يادگيري_گهڻائي_DUPLICATE, عدم, عدم);
    وهڪرو->ReleaseFileFragment (context);
    loader->Release ();
    اكسر;
  }

  أرجع blob;
}

ساكن hb_blob_t *
hb_directwrite_reference_جدول (hb_face_t *face HB_اڻ_استعمال_ٿيل, hb_ٽيگ_t ٽيگ_2, فراغ *user_معلومات)
{
  IDWriteFontFace *dw_face = ((IDWriteFontFace *) user_معلومات);
  ثابت فراغ *معلومات;
  uint32_t ڊيگهائي;
  فراغ *جدول_context;
  BOOL exists;
  إذا (ٽيگ_2 == HB_ٽيگ_NONE)
    أرجع _hb_directwrite_get_file_blob (dw_face);

  إذا (FAILED (dw_face->Try_Get_Font_جدول (hb_uint32_مٽا_سٽا (ٽيگ_2), &معلومات,
					&ڊيگهائي, &جدول_context, &exists)))
    أرجع خالي_اشارو;

  إذا (!معلومات || !exists || !ڊيگهائي)
  {
    dw_face->Release_Font_جدول (جدول_context);
    أرجع خالي_اشارو;
  }

  hb_directwrite_font_جدول_context *context = (hb_directwrite_font_جدول_context *) hb_malloc (حجم (hb_directwrite_font_جدول_context));
  إذا (unlikely (!context))
  {
    dw_face->Release_Font_جدول (جدول_context);
    أرجع خالي_اشارو;
  }
  context->face = dw_face;
  context->جدول_context = جدول_context;

  أرجع hb_blob_create ((ثابت حرف *) معلومات, ڊيگهائي, HB_يادگيري_گهڻائي_READONLY,
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
    أرجع hb_face_get_خالي_جايون ();

  dw_face->جوڙيو_Ref ();
  hb_face_t *face = hb_face_create_for_tables (hb_directwrite_reference_جدول,
					       dw_face,
					       _hb_directwrite_face_release);

  hb_face_سيٽ_اشاريو (face, dw_face->Get_اشاريو ());
  hb_face_سيٽ_glyph_ڳڻپ (face, dw_face->Get_Glyph_ڳڻپ ());

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
					      بنانشان صحيح  اشاريو_2)
{
  خودڪار *blob = hb_blob_create_from_file_or_fail (file_name);
  إذا (unlikely (!blob))
    أرجع خالي_اشارو;

  أرجع hb_directwrite_face_create_from_blob_or_fail (blob, اشاريو_2);
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
					      بنانشان صحيح  اشاريو_2)
{
  IDWriteFontFace *dw_face = dw_face_create (blob, اشاريو_2);
  إذا (unlikely (!dw_face))
    أرجع خالي_اشارو;

  hb_face_t *face = hb_directwrite_face_create (dw_face);
  إذا (unlikely (hb_object_is_immutable (face)))
  {
    dw_face->Release ();
    أرجع face;
  }

  /* Let there be dragons here... */
  face->معلومات.directwrite.cmpexch (خالي_اشارو, (hb_directwrite_face_معلومات_t *) dw_face);

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

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED

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

#جيڪڏهن_پورو

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
  IDWriteFontFace5 *dw_face5 = خالي_اشارو;

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
      hb_ويڪٽر_t<DWRITE_FONT_AXIS_قدر> values;
      uint32_t ڳڻپ = dw_face5->Get_Font_Axis_قدر_ڳڻپ ();
      إذا (likely (values.resize_exact (ڳڻپ)) &&
	  SUCCEEDED (dw_face5->Get_Font_Axis_قدرون (values.arrayZ, ڳڻپ)))
      {
	hb_ويڪٽر_t<hb_variation_t> vars;
	إذا (likely (vars.resize_exact (ڳڻپ)))
	{
	  لكل (uint32_t i = 0; i < ڳڻپ; ++i)
	  {
	    hb_ٽيگ_t ٽيگ_2 = hb_uint32_مٽا_سٽا (values[i].axis_ٽيگ);
	    عائم value = values[i].value;
	    vars[i] = {ٽيگ_2, value};
	  }
	  hb_font_سيٽ_variations (font, vars.arrayZ, vars.ڊيگهائي);
	}
      }
    }
    dw_face5->Release ();
  }

  /* Let there be dragons here... */
  dw_face->جوڙيو_Ref ();
  font->معلومات.directwrite.cmpexch (خالي_اشارو, (hb_directwrite_font_معلومات_t *) dw_face);

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

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED

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
  أرجع خالي_اشارو;
}

#جيڪڏهن_پورو

#جيڪڏهن_پورو
