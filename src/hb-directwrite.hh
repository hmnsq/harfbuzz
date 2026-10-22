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


#اگر_غیر_تعریف_شدہ HB_DIRECTWRITE_HH
#تعریف_کرو HB_DIRECTWRITE_HH

#شامل "hb.hh"

#شامل "hb-directwrite.h"

#شامل "hb-mutex.hh"
#شامل "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
قسمتعریف HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

سانچہ <قسم_نام Type, قسم_نام ...Ts>
ساکن Type *
hb_directwrite_object_create (Ts&&... دلائل)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));
  اگر (unlikely (!obj))
    واپس خالی_اشاریہ;

  واپس نیا (obj) Type (std::forward<Ts> (دلائل)...);
}

سانچہ <قسم_نام Type>
ساکن خالی
hb_directwrite_object_destroy (Type *obj)
{
  اگر (!obj)
    واپس;

  obj->~Type ();
  hb_free (obj);
}

جماعت DWriteFontFileLoader final : عوامی IDWriteFontFileLoader
{
نجی:
  hb_reference_گنتی_t m_Ref_گنتی;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_دھارا *> mFontStreams;
  uint64_t m_اگلا_Font_File_کلید = 0;
عوامی:
  DWriteFontFileLoader ()
  {
    m_Ref_گنتی.init ();
  }

  uint64_t رجسٹر_Font_File_دھارا (IDWrite_Font_File_دھارا *font_File_دھارا)
  {
    font_File_دھارا->جوڑو_Ref ();
    hb_تالا_t lock {mutex};
    mFontStreams.سیٹ (m_اگلا_Font_File_کلید, font_File_دھارا);
    واپس m_اگلا_Font_File_کلید++;
  }
  خالی Unregister_Font_File_دھارا (uint64_t font_File_کلید)
  {
    hb_تالا_t lock {mutex};
    IDWrite_Font_File_دھارا *دھارا = mFontStreams.get (font_File_کلید);
    اگر (دھارا)
    {
      mFontStreams.del (font_File_کلید);
      دھارا->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, برآمد_3 خالی** ppObject)
  { واپس S_OK; }
  IFACEMETHOD_ (ULONG, جوڑو_Ref) ()
  {
    واپس m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    علامتی ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    اگر (ref_گنتی)
      واپس ref_گنتی;
    hb_directwrite_object_destroy (یہ);
    واپس 0;
  }

  // IDWriteFontFileLoader methods
  مجازی HRESULT STDMETHODCALLTYPE
  Create_دھارا_From_کلید (خالی ثابت* font_File_Reference_کلید,
		       uint32_t font_File_Reference_کلید_سائز,
		       برآمد_3 IDWrite_Font_File_دھارا** font_File_دھارا)
  {
    اگر (font_File_Reference_کلید_سائز != حجم (uint64_t))
      واپس E_INVALIDARG;
    uint64_t font_File_کلید = * (uint64_t *) font_File_Reference_کلید;
    IDWrite_Font_File_دھارا *دھارا = mFontStreams.get (font_File_کلید);
    اگر (!دھارا)
      واپس E_FAIL;
    دھارا->جوڑو_Ref ();
    *font_File_دھارا = دھارا;
    واپس S_OK;
  }

  ~DWriteFontFileLoader()
  {
    برائے (خودکار v : mFontStreams.values ())
      v->Release ();
  }
};

جماعت DWrite_Font_File_دھارا final : عوامی IDWrite_Font_File_دھارا
{
نجی:
  hb_reference_گنتی_t m_Ref_گنتی;
  hb_blob_t *mBlob;
  uint8_t *m_ڈیٹا;
  غیرعلامتی m_سائز;
  DWriteFontFileLoader *mLoader;
عوامی:
  uint64_t font_File_کلید;
عوامی:
  DWrite_Font_File_دھارا (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, برآمد_3 خالی** ppObject)
  { واپس S_OK; }
  IFACEMETHOD_ (ULONG, جوڑو_Ref) ()
  {
    واپس m_Ref_گنتی.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    علامتی ref_گنتی = m_Ref_گنتی.dec () - 1;
    أكد (ref_گنتی >= 0);
    اگر (ref_گنتی)
      واپس ref_گنتی;
    hb_directwrite_object_destroy (یہ);
    واپس 0;
  }

  // IDWriteFontFileStream methods
  مجازی HRESULT STDMETHODCALLTYPE
  ReadFileFragment (خالی ثابت** fragment_آغاز_2,
		    UINT64 file_انحراف,
		    UINT64 fragment_سائز,
		    برآمد_3 خالی** fragmentContext)
  {
    // We are required to do bounds checking.
    اگر (file_انحراف + fragment_سائز > m_سائز) واپس E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t اشاریہ_2 = static_cast<size_t> (file_انحراف);

    // We should be alive for the duration of this.
    *fragment_آغاز_2 = &m_ڈیٹا[اشاریہ_2];
    *fragmentContext = خالی_اشاریہ;
    واپس S_OK;
  }

  مجازی خالی STDMETHODCALLTYPE
  ReleaseFileFragment (خالی* fragmentContext) {}

  مجازی HRESULT STDMETHODCALLTYPE
  Get_File_سائز (برآمد_3 UINT64* file_سائز_2)
  {
    *file_سائز_2 = m_سائز;
    واپس S_OK;
  }

  مجازی HRESULT STDMETHODCALLTYPE
  Get_آخری_Write_وقت (برآمد_3 UINT64* آخری_Write_وقت) { واپس E_NOTIMPL; }

  ~DWrite_Font_File_دھارا();
};

ساخت hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    اگر (unlikely (hr != S_OK))
      واپس;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    اگر (unlikely (!fontFileLoader))
      واپس;
    اگر (unlikely (FAILED (dwriteFactory->رجسٹر_Font_File_Loader (fontFileLoader))))
      واپس;

    success = سچ;
  }
  ~hb_directwrite_global_t ()
  {
    اگر (fontFileLoader)
      fontFileLoader->Release ();
    اگر (dwriteFactory)
      dwriteFactory->Release ();
  }

  منطقی success = جھوٹ;
  IDWriteFactory *dwriteFactory = خالی_اشاریہ;
  DWriteFontFileLoader *fontFileLoader = خالی_اشاریہ;
};


HB_اندرونی hb_directwrite_global_t *
get_directwrite_global ();

HB_اندرونی IDWriteFontFace *
dw_face_create (hb_blob_t *blob, غیرعلامتی اشاریہ_2);


#اگر_ختم /* HB_DIRECTWRITE_HH */
