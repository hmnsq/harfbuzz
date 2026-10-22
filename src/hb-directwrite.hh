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


#اگر_تعریف_نشده HB_DIRECTWRITE_HH
#تعریف_کن HB_DIRECTWRITE_HH

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

قالب <نام_نوع Type, نام_نوع ...Ts>
ساكن Type *
hb_directwrite_object_create (Ts&&... آرگومانها)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));
  إذا (unlikely (!obj))
    أرجع اشاره_تهی;

  أرجع جديد (obj) Type (std::forward<Ts> (آرگومانها)...);
}

قالب <نام_نوع Type>
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
  hb_reference_شمار_t m_Ref_شمار;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_جریان *> mFontStreams;
  uint64_t m_بعدی_Font_File_کلید = 0;
عام:
  DWriteFontFileLoader ()
  {
    m_Ref_شمار.init ();
  }

  uint64_t ثبات_Font_File_جریان (IDWrite_Font_File_جریان *font_File_جریان)
  {
    font_File_جریان->جمع_کن_Ref ();
    hb_قفل_t lock {mutex};
    mFontStreams.مجموعه (m_بعدی_Font_File_کلید, font_File_جریان);
    أرجع m_بعدی_Font_File_کلید++;
  }
  فراغ Unregister_Font_File_جریان (uint64_t font_File_کلید)
  {
    hb_قفل_t lock {mutex};
    IDWrite_Font_File_جریان *جریان = mFontStreams.get (font_File_کلید);
    إذا (جریان)
    {
      mFontStreams.del (font_File_کلید);
      جریان->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, بیرون_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جمع_کن_Ref) ()
  {
    أرجع m_Ref_شمار.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    علامتدار ref_شمار = m_Ref_شمار.dec () - 1;
    أكد (ref_شمار >= 0);
    إذا (ref_شمار)
      أرجع ref_شمار;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileLoader methods
  مجازی HRESULT STDMETHODCALLTYPE
  Create_جریان_From_کلید (فراغ ثابت* font_File_Reference_کلید,
		       uint32_t font_File_Reference_کلید_اندازه,
		       بیرون_3 IDWrite_Font_File_جریان** font_File_جریان)
  {
    إذا (font_File_Reference_کلید_اندازه != حجم (uint64_t))
      أرجع E_INVALIDARG;
    uint64_t font_File_کلید = * (uint64_t *) font_File_Reference_کلید;
    IDWrite_Font_File_جریان *جریان = mFontStreams.get (font_File_کلید);
    إذا (!جریان)
      أرجع E_FAIL;
    جریان->جمع_کن_Ref ();
    *font_File_جریان = جریان;
    أرجع S_OK;
  }

  ~DWriteFontFileLoader()
  {
    لكل (خودکار v : mFontStreams.values ())
      v->Release ();
  }
};

صنف DWrite_Font_File_جریان final : عام IDWrite_Font_File_جریان
{
خاص:
  hb_reference_شمار_t m_Ref_شمار;
  hb_blob_t *mBlob;
  uint8_t *m_داده;
  بیعلامت m_اندازه;
  DWriteFontFileLoader *mLoader;
عام:
  uint64_t font_File_کلید;
عام:
  DWrite_Font_File_جریان (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, بیرون_3 فراغ** ppObject)
  { أرجع S_OK; }
  IFACEMETHOD_ (ULONG, جمع_کن_Ref) ()
  {
    أرجع m_Ref_شمار.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    علامتدار ref_شمار = m_Ref_شمار.dec () - 1;
    أكد (ref_شمار >= 0);
    إذا (ref_شمار)
      أرجع ref_شمار;
    hb_directwrite_object_destroy (هذا);
    أرجع 0;
  }

  // IDWriteFontFileStream methods
  مجازی HRESULT STDMETHODCALLTYPE
  ReadFileFragment (فراغ ثابت** fragment_شروع_2,
		    UINT64 file_جابه_جایی,
		    UINT64 fragment_اندازه,
		    بیرون_3 فراغ** fragmentContext)
  {
    // We are required to do bounds checking.
    إذا (file_جابه_جایی + fragment_اندازه > m_اندازه) أرجع E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t شاخص_2 = static_cast<size_t> (file_جابه_جایی);

    // We should be alive for the duration of this.
    *fragment_شروع_2 = &m_داده[شاخص_2];
    *fragmentContext = اشاره_تهی;
    أرجع S_OK;
  }

  مجازی فراغ STDMETHODCALLTYPE
  ReleaseFileFragment (فراغ* fragmentContext) {}

  مجازی HRESULT STDMETHODCALLTYPE
  Get_File_اندازه (بیرون_3 UINT64* file_اندازه_2)
  {
    *file_اندازه_2 = m_اندازه;
    أرجع S_OK;
  }

  مجازی HRESULT STDMETHODCALLTYPE
  Get_آخرین_Write_زمان (بیرون_3 UINT64* آخرین_Write_زمان) { أرجع E_NOTIMPL; }

  ~DWrite_Font_File_جریان();
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
    إذا (unlikely (FAILED (dwriteFactory->ثبات_Font_File_Loader (fontFileLoader))))
      أرجع;

    success = درست;
  }
  ~hb_directwrite_global_t ()
  {
    إذا (fontFileLoader)
      fontFileLoader->Release ();
    إذا (dwriteFactory)
      dwriteFactory->Release ();
  }

  منطقي success = خطأ;
  IDWriteFactory *dwriteFactory = اشاره_تهی;
  DWriteFontFileLoader *fontFileLoader = اشاره_تهی;
};


HB_داخلی hb_directwrite_global_t *
get_directwrite_global ();

HB_داخلی IDWriteFontFace *
dw_face_create (hb_blob_t *blob, بیعلامت شاخص_2);


#پایان_اگر /* HB_DIRECTWRITE_HH */
