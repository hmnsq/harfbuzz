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

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-directwrite.h"

#অন্তর্ভাৱ "hb-mutex.hh"
#অন্তর্ভাৱ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
প্রকারনির্দেশ HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
স্থির Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, পরিমাণ (Type));
  যদি (unlikely (!obj))
    ঘূৰাই nullptr;

  ঘূৰাই নূতন (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
স্থির শূন্য
hb_directwrite_object_destroy (Type *obj)
{
  যদি (!obj)
    ঘূৰাই;

  obj->~Type ();
  hb_free (obj);
}

শ্ৰেণী DWriteFontFileLoader final : সার্ৱজনিক IDWriteFontFileLoader
{
গুপ্ত:
  hb_reference_গুনতি_t m_Ref_গুনতি;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_চাবি = 0;
সার্ৱজনিক:
  DWriteFontFileLoader ()
  {
    m_Ref_গুনতি.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_চাবি, fontFileStream);
    ঘূৰাই m_Next_Font_File_চাবি++;
  }
  শূন্য UnregisterFontFileStream (uint64_t font_File_চাবি)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_চাবি);
    যদি (stream)
    {
      mFontStreams.del (font_File_চাবি);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ধ্রুৱ& iid, OUT শূন্য** ppObject)
  { ঘূৰাই S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ঘূৰাই m_Ref_গুনতি.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_গুনতি = m_Ref_গুনতি.dec () - 1;
    প্রতিজ্ঞা (ref_গুনতি >= 0);
    যদি (ref_গুনতি)
      ঘূৰাই ref_গুনতি;
    hb_directwrite_object_destroy (এতত্);
    ঘূৰাই 0;
  }

  // IDWriteFontFileLoader methods
  আভাস HRESULT STDMETHODCALLTYPE
  Create_Stream_From_চাবি (শূন্য ধ্রুৱ* font_File_Reference_চাবি,
		       uint32_t font_File_Reference_চাবি_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    যদি (font_File_Reference_চাবি_Size != পরিমাণ (uint64_t))
      ঘূৰাই E_INVALIDARG;
    uint64_t font_File_চাবি = * (uint64_t *) font_File_Reference_চাবি;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_চাবি);
    যদি (!stream)
      ঘূৰাই E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    ঘূৰাই S_OK;
  }

  ~DWriteFontFileLoader()
  {
    বাবে (auto v : mFontStreams.values ())
      v->Release ();
  }
};

শ্ৰেণী DWriteFontFileStream final : সার্ৱজনিক IDWriteFontFileStream
{
গুপ্ত:
  hb_reference_গুনতি_t m_Ref_গুনতি;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
সার্ৱজনিক:
  uint64_t font_File_চাবি;
সার্ৱজনিক:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ধ্রুৱ& iid, OUT শূন্য** ppObject)
  { ঘূৰাই S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ঘূৰাই m_Ref_গুনতি.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_গুনতি = m_Ref_গুনতি.dec () - 1;
    প্রতিজ্ঞা (ref_গুনতি >= 0);
    যদি (ref_গুনতি)
      ঘূৰাই ref_গুনতি;
    hb_directwrite_object_destroy (এতত্);
    ঘূৰাই 0;
  }

  // IDWriteFontFileStream methods
  আভাস HRESULT STDMETHODCALLTYPE
  ReadFileFragment (শূন্য ধ্রুৱ** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT শূন্য** fragmentContext)
  {
    // We are required to do bounds checking.
    যদি (fileOffset + fragmentSize > mSize) ঘূৰাই E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    ঘূৰাই S_OK;
  }

  আভাস শূন্য STDMETHODCALLTYPE
  ReleaseFileFragment (শূন্য* fragmentContext) {}

  আভাস HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    ঘূৰাই S_OK;
  }

  আভাস HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { ঘূৰাই E_NOTIMPL; }

  ~DWriteFontFileStream();
};

সংরচনা hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    যদি (unlikely (hr != S_OK))
      ঘূৰাই;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    যদি (unlikely (!fontFileLoader))
      ঘূৰাই;
    যদি (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      ঘূৰাই;

    success = সঁচা;
  }
  ~hb_directwrite_global_t ()
  {
    যদি (fontFileLoader)
      fontFileLoader->Release ();
    যদি (dwriteFactory)
      dwriteFactory->Release ();
  }

  তার্কিক success = মিছা;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
