/*
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
 *
 * Author(s): Behdad Esfahbod
 */


#جيڪڏهن_وصف_نه_ٿيل HB_DIRECTWRITE_HH
#وصف_ڪريو HB_DIRECTWRITE_HH

#ضمن "hb.hh"

#ضمن "hb-directwrite.h"

#ضمن "hb-mutex.hh"
#ضمن "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
تعريفنوع HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

سانچو <قسم_نالو Type, قسم_نالو ...Ts>
ساكن Type *
hb_directwrite_object_create (Ts&&... دليلون)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));
  إذا (unlikely (!obj))
    أرجع خالي_اشارو;

  أرجع جديد (obj) Type (std::forward<Ts> (دليلون)...);
}

سانچو <قسم_نالو Type>
ساكن فراغ
hb_directwrite_object_destroy (Type *obj)
{
  إذا (!obj)
    أرجع;

  obj->~Type ();
  hb_free (obj);
}

صنف DWriteFontFileLoader final : عام IDWriteFontFileLoader
{
خاص:
  hb_reference_ڳڻپ_t m_Ref_ڳڻپ;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_وهڪرو *> mFontStreams;
  uint64_t m_ايندڙ_Font_File_ڪنجي = 0;
عام:
  DWriteFontFileLoader ()
  {
    m_Ref_ڳڻپ.init ();
  }

  uint64_t رجسٽر_Font_File_وهڪرو (IDWrite_Font_File_وهڪرو *font_File_وهڪرو)
  {
    font_File_وهڪرو->جوڙيو_Ref ();
    hb_تالو_t lock {mutex};
    mFontStreams.سيٽ (m_ايندڙ_Font_File_ڪنجي, font_File_وهڪرو);
    أرجع m_ايندڙ_Font_File_ڪنجي++;
  }
  فراغ Unregister_Font_File_وهڪرو (uint64_t font_File_ڪنجي)
  {
    hb_تالو_t lock {mutex};
    IDWrite_Font_File_وهڪرو *وهڪرو = mFontStreams.get (font_File_ڪنجي);
    إذا (وهڪرو)
    {
      mFontStreams.del (font_File_ڪنجي);
      وهڪرو->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, ٻاهريون_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جوڙيو_Ref) ()
  {
    أرجع m_Ref_ڳڻپ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نشانوارو ref_ڳڻپ = m_Ref_ڳڻپ.dec () - 1;
    أكد (ref_ڳڻپ >= 0);
    إذا (ref_ڳڻپ)
      أرجع ref_ڳڻپ;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileLoader methods
  مجازي HRESULT STDMETHODCALLTYPE
  Create_وهڪرو_From_ڪنجي (فراغ ثابت* font_File_Reference_ڪنجي,
		       uint32_t font_File_Reference_ڪنجي_سائيز,
		       ٻاهريون_3 IDWrite_Font_File_وهڪرو** font_File_وهڪرو)
  {
    إذا (font_File_Reference_ڪنجي_سائيز != حجم (uint64_t))
      أرجع E_INVALIDARG;
    uint64_t font_File_ڪنجي = * (uint64_t *) font_File_Reference_ڪنجي;
    IDWrite_Font_File_وهڪرو *وهڪرو = mFontStreams.get (font_File_ڪنجي);
    إذا (!وهڪرو)
      أرجع E_FAIL;
    وهڪرو->جوڙيو_Ref ();
    *font_File_وهڪرو = وهڪرو;
    أرجع S_OK;
  }

  ~DWriteFontFileLoader()
  {
    لكل (خودڪار v : mFontStreams.values ())
      v->Release ();
  }
};

صنف DWrite_Font_File_وهڪرو final : عام IDWrite_Font_File_وهڪرو
{
خاص:
  hb_reference_ڳڻپ_t m_Ref_ڳڻپ;
  hb_blob_t *mBlob;
  uint8_t *m_معلومات;
  بنانشان m_سائيز;
  DWriteFontFileLoader *mLoader;
عام:
  uint64_t font_File_ڪنجي;
عام:
  DWrite_Font_File_وهڪرو (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, ٻاهريون_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جوڙيو_Ref) ()
  {
    أرجع m_Ref_ڳڻپ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نشانوارو ref_ڳڻپ = m_Ref_ڳڻپ.dec () - 1;
    أكد (ref_ڳڻپ >= 0);
    إذا (ref_ڳڻپ)
      أرجع ref_ڳڻپ;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileStream methods
  مجازي HRESULT STDMETHODCALLTYPE
  ReadFileFragment (فراغ ثابت** fragment_شروعات_2,
		    UINT64 file_وٿي,
		    UINT64 fragment_سائيز,
		    ٻاهريون_3 فراغ** fragmentContext)
  {
    // We are required to do bounds checking.
    إذا (file_وٿي + fragment_سائيز > m_سائيز) أرجع E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t اشاريو_2 = static_cast<size_t> (file_وٿي);

    // We should be alive for the duration of this.
    *fragment_شروعات_2 = &m_معلومات[اشاريو_2];
    *fragmentContext = خالي_اشارو;
    أرجع S_OK;
  }

  مجازي فراغ STDMETHODCALLTYPE
  ReleaseFileFragment (فراغ* fragmentContext) {}

  مجازي HRESULT STDMETHODCALLTYPE
  Get_File_سائيز (ٻاهريون_3 UINT64* file_سائيز_2)
  {
    *file_سائيز_2 = m_سائيز;
    أرجع S_OK;
  }

  مجازي HRESULT STDMETHODCALLTYPE
  Get_آخري_Write_وقت (ٻاهريون_3 UINT64* آخري_Write_وقت) { أرجع E_NOTIMPL; }

  ~DWrite_Font_File_وهڪرو();
};

بنية hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    إذا (unlikely (hr != S_OK))
      أرجع;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    إذا (unlikely (!fontFileLoader))
      أرجع;
    إذا (unlikely (FAILED (dwriteFactory->رجسٽر_Font_File_Loader (fontFileLoader))))
      أرجع;

    success = سچ;
  }
  ~hb_directwrite_global_t ()
  {
    إذا (fontFileLoader)
      fontFileLoader->Release ();
    إذا (dwriteFactory)
      dwriteFactory->Release ();
  }

  منطقي success = خطأ;
  IDWriteFactory *dwriteFactory = خالي_اشارو;
  DWriteFontFileLoader *fontFileLoader = خالي_اشارو;
};


HB_اندروني hb_directwrite_global_t *
get_directwrite_global ();

HB_اندروني IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بنانشان اشاريو_2);


#جيڪڏهن_پورو /* HB_DIRECTWRITE_HH */
