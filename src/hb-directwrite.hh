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

template <typename Type, typename ...Ts>
ساكن Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, حجم (Type));
  אם (unlikely (!obj))
    החזר nullptr;

  החזר جديد (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
ساكن فراغ
hb_directwrite_object_destroy (Type *obj)
{
  אם (!obj)
    החזר;

  obj->~Type ();
  hb_free (obj);
}

מחלקה DWriteFontFileLoader final : عام IDWriteFontFileLoader
{
خاص:
  hb_reference_count_t mRefCount;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t mNextFontFileKey = 0;
عام:
  DWriteFontFileLoader ()
  {
    mRefCount.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (mNextFontFileKey, fontFileStream);
    החזר mNextFontFileKey++;
  }
  فراغ UnregisterFontFileStream (uint64_t fontFileKey)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (fontFileKey);
    אם (stream)
    {
      mFontStreams.del (fontFileKey);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, OUT فراغ** ppObject)
  { החזר S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    החזר mRefCount.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed refCount = mRefCount.dec () - 1;
    أكد (refCount >= 0);
    אם (refCount)
      החזר refCount;
    hb_directwrite_object_destroy (هذا);
    החזר 0;
  }

  // IDWriteFontFileLoader methods
  virtual HRESULT STDMETHODCALLTYPE
  CreateStreamFromKey (فراغ ثابت* fontFileReferenceKey,
		       uint32_t fontFileReferenceKeySize,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    אם (fontFileReferenceKeySize != حجم (uint64_t))
      החזר E_INVALIDARG;
    uint64_t fontFileKey = * (uint64_t *) fontFileReferenceKey;
    IDWriteFontFileStream *stream = mFontStreams.get (fontFileKey);
    אם (!stream)
      החזר E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    החזר S_OK;
  }

  ~DWriteFontFileLoader()
  {
    עבור (auto v : mFontStreams.values ())
      v->Release ();
  }
};

מחלקה DWriteFontFileStream final : عام IDWriteFontFileStream
{
خاص:
  hb_reference_count_t mRefCount;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
عام:
  uint64_t fontFileKey;
عام:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ثابت& iid, OUT فراغ** ppObject)
  { החזר S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    החזר mRefCount.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed refCount = mRefCount.dec () - 1;
    أكد (refCount >= 0);
    אם (refCount)
      החזר refCount;
    hb_directwrite_object_destroy (هذا);
    החזר 0;
  }

  // IDWriteFontFileStream methods
  virtual HRESULT STDMETHODCALLTYPE
  ReadFileFragment (فراغ ثابت** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT فراغ** fragmentContext)
  {
    // We are required to do bounds checking.
    אם (fileOffset + fragmentSize > mSize) החזר E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    החזר S_OK;
  }

  virtual فراغ STDMETHODCALLTYPE
  ReleaseFileFragment (فراغ* fragmentContext) {}

  virtual HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    החזר S_OK;
  }

  virtual HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { החזר E_NOTIMPL; }

  ~DWriteFontFileStream();
};

بنية hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    אם (unlikely (hr != S_OK))
      החזר;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    אם (unlikely (!fontFileLoader))
      החזר;
    אם (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      החזר;

    success = אמת;
  }
  ~hb_directwrite_global_t ()
  {
    אם (fontFileLoader)
      fontFileLoader->Release ();
    אם (dwriteFactory)
      dwriteFactory->Release ();
  }

  منطقي success = שקר;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
