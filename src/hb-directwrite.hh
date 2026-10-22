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


#ifndef HB_DIRECTWRITE_HH
#define HB_DIRECTWRITE_HH

#شامل "hb.hh"

#شامل "hb-directwrite.h"

#شامل "hb-mutex.hh"
#شامل "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
تعریفنوع HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
ایستا Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, اندازه (Type));
  اگر (unlikely (!obj))
    بازگشت nullptr;

  بازگشت جدید (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
ایستا تهی
hb_directwrite_object_destroy (Type *obj)
{
  اگر (!obj)
    بازگشت;

  obj->~Type ();
  hb_free (obj);
}

رده DWriteFontFileLoader final : عمومی IDWriteFontFileLoader
{
خصوصی:
  hb_reference_شمار_t m_Ref_شمار;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_کلید = 0;
عمومی:
  DWriteFontFileLoader ()
  {
    m_Ref_شمار.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_کلید, fontFileStream);
    بازگشت m_Next_Font_File_کلید++;
  }
  تهی UnregisterFontFileStream (uint64_t font_File_کلید)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_کلید);
    اگر (stream)
    {
      mFontStreams.del (font_File_کلید);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, OUT تهی** ppObject)
  { بازگشت S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    بازگشت m_Ref_شمار.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_شمار = m_Ref_شمار.dec () - 1;
    أكد (ref_شمار >= 0);
    اگر (ref_شمار)
      بازگشت ref_شمار;
    hb_directwrite_object_destroy (این);
    بازگشت 0;
  }

  // IDWriteFontFileLoader methods
  مجازی HRESULT STDMETHODCALLTYPE
  Create_Stream_From_کلید (تهی ثابت* font_File_Reference_کلید,
		       uint32_t font_File_Reference_کلید_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    اگر (font_File_Reference_کلید_Size != اندازه (uint64_t))
      بازگشت E_INVALIDARG;
    uint64_t font_File_کلید = * (uint64_t *) font_File_Reference_کلید;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_کلید);
    اگر (!stream)
      بازگشت E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    بازگشت S_OK;
  }

  ~DWriteFontFileLoader()
  {
    برای (auto v : mFontStreams.values ())
      v->Release ();
  }
};

رده DWriteFontFileStream final : عمومی IDWriteFontFileStream
{
خصوصی:
  hb_reference_شمار_t m_Ref_شمار;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
عمومی:
  uint64_t font_File_کلید;
عمومی:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, OUT تهی** ppObject)
  { بازگشت S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    بازگشت m_Ref_شمار.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_شمار = m_Ref_شمار.dec () - 1;
    أكد (ref_شمار >= 0);
    اگر (ref_شمار)
      بازگشت ref_شمار;
    hb_directwrite_object_destroy (این);
    بازگشت 0;
  }

  // IDWriteFontFileStream methods
  مجازی HRESULT STDMETHODCALLTYPE
  ReadFileFragment (تهی ثابت** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT تهی** fragmentContext)
  {
    // We are required to do bounds checking.
    اگر (fileOffset + fragmentSize > mSize) بازگشت E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    بازگشت S_OK;
  }

  مجازی تهی STDMETHODCALLTYPE
  ReleaseFileFragment (تهی* fragmentContext) {}

  مجازی HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    بازگشت S_OK;
  }

  مجازی HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { بازگشت E_NOTIMPL; }

  ~DWriteFontFileStream();
};

ساختار hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    اگر (unlikely (hr != S_OK))
      بازگشت;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    اگر (unlikely (!fontFileLoader))
      بازگشت;
    اگر (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      بازگشت;

    success = درست;
  }
  ~hb_directwrite_global_t ()
  {
    اگر (fontFileLoader)
      fontFileLoader->Release ();
    اگر (dwriteFactory)
      dwriteFactory->Release ();
  }

  منطقی success = نادرست;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
