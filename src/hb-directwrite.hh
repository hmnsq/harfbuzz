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

#include "hb.hh"

#include "hb-directwrite.h"

#include "hb-mutex.hh"
#include "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
typedef HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
static Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, parimaa_na (Type));
  නම් (unlikely (!obj))
    ආපසු nullptr;

  ආපසු new (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
static void
hb_directwrite_object_destroy (Type *obj)
{
  නම් (!obj)
    ආපසු;

  obj->~Type ();
  hb_free (obj);
}

class DWriteFontFileLoader final : public IDWriteFontFileLoader
{
private:
  hb_reference_count_t mRefCount;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t mNextFontFileKey = 0;
public:
  DWriteFontFileLoader ()
  {
    mRefCount.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (mNextFontFileKey, fontFileStream);
    ආපසු mNextFontFileKey++;
  }
  void UnregisterFontFileStream (uint64_t fontFileKey)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (fontFileKey);
    නම් (stream)
    {
      mFontStreams.del (fontFileKey);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID const& iid, OUT void** ppObject)
  { ආපසු S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ආපසු mRefCount.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed refCount = mRefCount.dec () - 1;
    assert (refCount >= 0);
    නම් (refCount)
      ආපසු refCount;
    hb_directwrite_object_destroy (this);
    ආපසු 0;
  }

  // IDWriteFontFileLoader methods
  virtual HRESULT STDMETHODCALLTYPE
  CreateStreamFromKey (void const* fontFileReferenceKey,
		       uint32_t fontFileReferenceKeySize,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    නම් (fontFileReferenceKeySize != parimaa_na (uint64_t))
      ආපසු E_INVALIDARG;
    uint64_t fontFileKey = * (uint64_t *) fontFileReferenceKey;
    IDWriteFontFileStream *stream = mFontStreams.get (fontFileKey);
    නම් (!stream)
      ආපසු E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    ආපසු S_OK;
  }

  ~DWriteFontFileLoader()
  {
    සඳහා (auto v : mFontStreams.values ())
      v->Release ();
  }
};

class DWriteFontFileStream final : public IDWriteFontFileStream
{
private:
  hb_reference_count_t mRefCount;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
public:
  uint64_t fontFileKey;
public:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID const& iid, OUT void** ppObject)
  { ආපසු S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ආපසු mRefCount.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed refCount = mRefCount.dec () - 1;
    assert (refCount >= 0);
    නම් (refCount)
      ආපසු refCount;
    hb_directwrite_object_destroy (this);
    ආපසු 0;
  }

  // IDWriteFontFileStream methods
  virtual HRESULT STDMETHODCALLTYPE
  ReadFileFragment (void const** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT void** fragmentContext)
  {
    // We are required to do bounds checking.
    නම් (fileOffset + fragmentSize > mSize) ආපසු E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    ආපසු S_OK;
  }

  virtual void STDMETHODCALLTYPE
  ReleaseFileFragment (void* fragmentContext) {}

  virtual HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    ආපසු S_OK;
  }

  virtual HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { ආපසු E_NOTIMPL; }

  ~DWriteFontFileStream();
};

sa_nracanaa hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    නම් (unlikely (hr != S_OK))
      ආපසු;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    නම් (unlikely (!fontFileLoader))
      ආපසු;
    නම් (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      ආපසු;

    success = true;
  }
  ~hb_directwrite_global_t ()
  {
    නම් (fontFileLoader)
      fontFileLoader->Release ();
    නම් (dwriteFactory)
      dwriteFactory->Release ();
  }

  bool success = false;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
