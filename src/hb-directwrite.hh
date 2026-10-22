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


#که_نه_تعریف_شوی HB_DIRECTWRITE_HH
#تعریف_کړه HB_DIRECTWRITE_HH

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

کينډۍ <ډول_نوم Type, ډول_نوم ...Ts>
ساكن Type *
hb_directwrite_object_create (Ts&&... دلیلونه)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));
  إذا (unlikely (!obj))
    أرجع تش_اشاره;

  أرجع جديد (obj) Type (std::forward<Ts> (دلیلونه)...);
}

کينډۍ <ډول_نوم Type>
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
  hb_reference_شمېر_t m_Ref_شمېر;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_بهیر *> mFontStreams;
  uint64_t m_بل_Font_File_کلی = 0;
عام:
  DWriteFontFileLoader ()
  {
    m_Ref_شمېر.init ();
  }

  uint64_t ثبتوونکی_Font_File_بهیر (IDWrite_Font_File_بهیر *font_File_بهیر)
  {
    font_File_بهیر->جمع_کړه_Ref ();
    hb_قلف_t lock {mutex};
    mFontStreams.سېټ (m_بل_Font_File_کلی, font_File_بهیر);
    أرجع m_بل_Font_File_کلی++;
  }
  فراغ Unregister_Font_File_بهیر (uint64_t font_File_کلی)
  {
    hb_قلف_t lock {mutex};
    IDWrite_Font_File_بهیر *بهیر = mFontStreams.get (font_File_کلی);
    إذا (بهیر)
    {
      mFontStreams.del (font_File_کلی);
      بهیر->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, بهر_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جمع_کړه_Ref) ()
  {
    أرجع m_Ref_شمېر.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نښهدار ref_شمېر = m_Ref_شمېر.dec () - 1;
    أكد (ref_شمېر >= 0);
    إذا (ref_شمېر)
      أرجع ref_شمېر;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileLoader methods
  مجازي HRESULT STDMETHODCALLTYPE
  Create_بهیر_From_کلی (فراغ ثابت* font_File_Reference_کلی,
		       uint32_t font_File_Reference_کلی_اندازه,
		       بهر_3 IDWrite_Font_File_بهیر** font_File_بهیر)
  {
    إذا (font_File_Reference_کلی_اندازه != حجم (uint64_t))
      أرجع E_INVALIDARG;
    uint64_t font_File_کلی = * (uint64_t *) font_File_Reference_کلی;
    IDWrite_Font_File_بهیر *بهیر = mFontStreams.get (font_File_کلی);
    إذا (!بهیر)
      أرجع E_FAIL;
    بهیر->جمع_کړه_Ref ();
    *font_File_بهیر = بهیر;
    أرجع S_OK;
  }

  ~DWriteFontFileLoader()
  {
    لكل (خپلکاری v : mFontStreams.values ())
      v->Release ();
  }
};

صنف DWrite_Font_File_بهیر final : عام IDWrite_Font_File_بهیر
{
خاص:
  hb_reference_شمېر_t m_Ref_شمېر;
  hb_blob_t *mBlob;
  uint8_t *m_معلومات;
  بینښه m_اندازه;
  DWriteFontFileLoader *mLoader;
عام:
  uint64_t font_File_کلی;
عام:
  DWrite_Font_File_بهیر (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, بهر_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جمع_کړه_Ref) ()
  {
    أرجع m_Ref_شمېر.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    نښهدار ref_شمېر = m_Ref_شمېر.dec () - 1;
    أكد (ref_شمېر >= 0);
    إذا (ref_شمېر)
      أرجع ref_شمېر;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileStream methods
  مجازي HRESULT STDMETHODCALLTYPE
  ReadFileFragment (فراغ ثابت** fragment_پیل_2,
		    UINT64 file_دننه_واټن,
		    UINT64 fragment_اندازه,
		    بهر_3 فراغ** fragmentContext)
  {
    // We are required to do bounds checking.
    إذا (file_دننه_واټن + fragment_اندازه > m_اندازه) أرجع E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t شاخص_2 = static_cast<size_t> (file_دننه_واټن);

    // We should be alive for the duration of this.
    *fragment_پیل_2 = &m_معلومات[شاخص_2];
    *fragmentContext = تش_اشاره;
    أرجع S_OK;
  }

  مجازي فراغ STDMETHODCALLTYPE
  ReleaseFileFragment (فراغ* fragmentContext) {}

  مجازي HRESULT STDMETHODCALLTYPE
  Get_File_اندازه (بهر_3 UINT64* file_اندازه_2)
  {
    *file_اندازه_2 = m_اندازه;
    أرجع S_OK;
  }

  مجازي HRESULT STDMETHODCALLTYPE
  Get_وروستی_Write_وخت (بهر_3 UINT64* وروستی_Write_وخت) { أرجع E_NOTIMPL; }

  ~DWrite_Font_File_بهیر();
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
    إذا (unlikely (FAILED (dwriteFactory->ثبتوونکی_Font_File_Loader (fontFileLoader))))
      أرجع;

    success = ریښتیا;
  }
  ~hb_directwrite_global_t ()
  {
    إذا (fontFileLoader)
      fontFileLoader->Release ();
    إذا (dwriteFactory)
      dwriteFactory->Release ();
  }

  منطقي success = خطأ;
  IDWriteFactory *dwriteFactory = تش_اشاره;
  DWriteFontFileLoader *fontFileLoader = تش_اشاره;
};


HB_دننی hb_directwrite_global_t *
get_directwrite_global ();

HB_دننی IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بینښه شاخص_2);


#د_که_پای /* HB_DIRECTWRITE_HH */
