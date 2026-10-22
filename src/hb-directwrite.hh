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


#جے_بنا_تعریف HB_DIRECTWRITE_HH
#تعریف_کرو HB_DIRECTWRITE_HH

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

سانچا <قسم_ناں Type, قسم_ناں ...Ts>
ساكن Type *
hb_directwrite_object_create (Ts&&... دلیلاں)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));
  إذا (unlikely (!obj))
    أرجع خالی_سوچک;

  أرجع جديد (obj) Type (std::forward<Ts> (دلیلاں)...);
}

سانچا <قسم_ناں Type>
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
  hb_reference_گنتی_t m_Ref_گنتی;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_دھارا *> mFontStreams;
  uint64_t m_اگلا_Font_File_کلید = 0;
عام:
  DWriteFontFileLoader ()
  {
    m_Ref_گنتی.init ();
  }

  uint64_t رجسٹر_Font_File_دھارا (IDWrite_Font_File_دھارا *font_File_دھارا)
  {
    font_File_دھارا->جوڑو_Ref ();
    hb_جندرا_t lock {mutex};
    mFontStreams.سیٹ (m_اگلا_Font_File_کلید, font_File_دھارا);
    أرجع m_اگلا_Font_File_کلید++;
  }
  فراغ Unregister_Font_File_دھارا (uint64_t font_File_کلید)
  {
    hb_جندرا_t lock {mutex};
    IDWrite_Font_File_دھارا *دھارا = mFontStreams.get (font_File_کلید);
    إذا (دھارا)
    {
      mFontStreams.del (font_File_کلید);
      دھارا->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, باہرلا_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جوڑو_Ref) ()
  {
    أرجع m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نشانیوالا ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    إذا (ref_گنتی)
      أرجع ref_گنتی;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileLoader methods
  مجازی HRESULT STDMETHODCALLTYPE
  Create_دھارا_From_کلید (فراغ ثابت* font_File_Reference_کلید,
		       uint32_t font_File_Reference_کلید_ناپ,
		       باہرلا_3 IDWrite_Font_File_دھارا** font_File_دھارا)
  {
    إذا (font_File_Reference_کلید_ناپ != حجم (uint64_t))
      أرجع E_INVALIDARG;
    uint64_t font_File_کلید = * (uint64_t *) font_File_Reference_کلید;
    IDWrite_Font_File_دھارا *دھارا = mFontStreams.get (font_File_کلید);
    إذا (!دھارا)
      أرجع E_FAIL;
    دھارا->جوڑو_Ref ();
    *font_File_دھارا = دھارا;
    أرجع S_OK;
  }

  ~DWriteFontFileLoader()
  {
    لكل (خودکار v : mFontStreams.values ())
      v->Release ();
  }
};

صنف DWrite_Font_File_دھارا final : عام IDWrite_Font_File_دھارا
{
خاص:
  hb_reference_گنتی_t m_Ref_گنتی;
  hb_blob_t *mBlob;
  uint8_t *m_انکڑے;
  بنانشانی m_ناپ;
  DWriteFontFileLoader *mLoader;
عام:
  uint64_t font_File_کلید;
عام:
  DWrite_Font_File_دھارا (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, باہرلا_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جوڑو_Ref) ()
  {
    أرجع m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نشانیوالا ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    إذا (ref_گنتی)
      أرجع ref_گنتی;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileStream methods
  مجازی HRESULT STDMETHODCALLTYPE
  ReadFileFragment (فراغ ثابت** fragment_شروع_2,
		    UINT64 file_وتھ,
		    UINT64 fragment_ناپ,
		    باہرلا_3 فراغ** fragmentContext)
  {
    // We are required to do bounds checking.
    إذا (file_وتھ + fragment_ناپ > m_ناپ) أرجع E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t اشاریہ_2 = static_cast<size_t> (file_وتھ);

    // We should be alive for the duration of this.
    *fragment_شروع_2 = &m_انکڑے[اشاریہ_2];
    *fragmentContext = خالی_سوچک;
    أرجع S_OK;
  }

  مجازی فراغ STDMETHODCALLTYPE
  ReleaseFileFragment (فراغ* fragmentContext) {}

  مجازی HRESULT STDMETHODCALLTYPE
  Get_File_ناپ (باہرلا_3 UINT64* file_ناپ_2)
  {
    *file_ناپ_2 = m_ناپ;
    أرجع S_OK;
  }

  مجازی HRESULT STDMETHODCALLTYPE
  Get_آخری_Write_ویلا (باہرلا_3 UINT64* آخری_Write_ویلا) { أرجع E_NOTIMPL; }

  ~DWrite_Font_File_دھارا();
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
    إذا (unlikely (FAILED (dwriteFactory->رجسٹر_Font_File_Loader (fontFileLoader))))
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
  IDWriteFactory *dwriteFactory = خالی_سوچک;
  DWriteFontFileLoader *fontFileLoader = خالی_سوچک;
};


HB_اندرونی hb_directwrite_global_t *
get_directwrite_global ();

HB_اندرونی IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بنانشانی اشاریہ_2);


#جے_انت /* HB_DIRECTWRITE_HH */
