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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-directwrite.h"

#ଅନ୍ତର୍ଭାଵ "hb-mutex.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
ପ୍ରକାରବ୍ୟାଖ୍ୟା HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
ସ୍ଥିର Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, ଆକାର (Type));
  ଯଦି (unlikely (!obj))
    ଫେରନ୍ତୁ nullptr;

  ଫେରନ୍ତୁ ନୂତନ (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
ସ୍ଥିର ଶୂନ୍ୟ
hb_directwrite_object_destroy (Type *obj)
{
  ଯଦି (!obj)
    ଫେରନ୍ତୁ;

  obj->~Type ();
  hb_free (obj);
}

ଶ୍ରେଣୀ DWriteFontFileLoader final : ସର୍ବସାଧାରଣ IDWriteFontFileLoader
{
ବ୍ୟକ୍ତିଗତ:
  hb_reference_ଗଣନା_t m_Ref_ଗଣନା;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_ଚାବି = 0;
ସର୍ବସାଧାରଣ:
  DWriteFontFileLoader ()
  {
    m_Ref_ଗଣନା.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_ଚାବି, fontFileStream);
    ଫେରନ୍ତୁ m_Next_Font_File_ଚାବି++;
  }
  ଶୂନ୍ୟ UnregisterFontFileStream (uint64_t font_File_ଚାବି)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_ଚାବି);
    ଯଦି (stream)
    {
      mFontStreams.del (font_File_ଚାବି);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ଅଚଳ& iid, OUT ଶୂନ୍ୟ** ppObject)
  { ଫେରନ୍ତୁ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ଫେରନ୍ତୁ m_Ref_ଗଣନା.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ଗଣନା = m_Ref_ଗଣନା.dec () - 1;
    ପ୍ରତିଜ୍ଞା (ref_ଗଣନା >= 0);
    ଯଦି (ref_ଗଣନା)
      ଫେରନ୍ତୁ ref_ଗଣନା;
    hb_directwrite_object_destroy (ଏହି);
    ଫେରନ୍ତୁ 0;
  }

  // IDWriteFontFileLoader methods
  ଆଭାସୀ HRESULT STDMETHODCALLTYPE
  Create_Stream_From_ଚାବି (ଶୂନ୍ୟ ଅଚଳ* font_File_Reference_ଚାବି,
		       uint32_t font_File_Reference_ଚାବି_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    ଯଦି (font_File_Reference_ଚାବି_Size != ଆକାର (uint64_t))
      ଫେରନ୍ତୁ E_INVALIDARG;
    uint64_t font_File_ଚାବି = * (uint64_t *) font_File_Reference_ଚାବି;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_ଚାବି);
    ଯଦି (!stream)
      ଫେରନ୍ତୁ E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    ଫେରନ୍ତୁ S_OK;
  }

  ~DWriteFontFileLoader()
  {
    ପାଇଁ (auto v : mFontStreams.values ())
      v->Release ();
  }
};

ଶ୍ରେଣୀ DWriteFontFileStream final : ସର୍ବସାଧାରଣ IDWriteFontFileStream
{
ବ୍ୟକ୍ତିଗତ:
  hb_reference_ଗଣନା_t m_Ref_ଗଣନା;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
ସର୍ବସାଧାରଣ:
  uint64_t font_File_ଚାବି;
ସର୍ବସାଧାରଣ:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ଅଚଳ& iid, OUT ଶୂନ୍ୟ** ppObject)
  { ଫେରନ୍ତୁ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ଫେରନ୍ତୁ m_Ref_ଗଣନା.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ଗଣନା = m_Ref_ଗଣନା.dec () - 1;
    ପ୍ରତିଜ୍ଞା (ref_ଗଣନା >= 0);
    ଯଦି (ref_ଗଣନା)
      ଫେରନ୍ତୁ ref_ଗଣନା;
    hb_directwrite_object_destroy (ଏହି);
    ଫେରନ୍ତୁ 0;
  }

  // IDWriteFontFileStream methods
  ଆଭାସୀ HRESULT STDMETHODCALLTYPE
  ReadFileFragment (ଶୂନ୍ୟ ଅଚଳ** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT ଶୂନ୍ୟ** fragmentContext)
  {
    // We are required to do bounds checking.
    ଯଦି (fileOffset + fragmentSize > mSize) ଫେରନ୍ତୁ E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    ଫେରନ୍ତୁ S_OK;
  }

  ଆଭାସୀ ଶୂନ୍ୟ STDMETHODCALLTYPE
  ReleaseFileFragment (ଶୂନ୍ୟ* fragmentContext) {}

  ଆଭାସୀ HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    ଫେରନ୍ତୁ S_OK;
  }

  ଆଭାସୀ HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { ଫେରନ୍ତୁ E_NOTIMPL; }

  ~DWriteFontFileStream();
};

ସଂରଚନା hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    ଯଦି (unlikely (hr != S_OK))
      ଫେରନ୍ତୁ;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    ଯଦି (unlikely (!fontFileLoader))
      ଫେରନ୍ତୁ;
    ଯଦି (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      ଫେରନ୍ତୁ;

    success = ସତ୍ୟ;
  }
  ~hb_directwrite_global_t ()
  {
    ଯଦି (fontFileLoader)
      fontFileLoader->Release ();
    ଯଦି (dwriteFactory)
      dwriteFactory->Release ();
  }

  ତାର୍କିକ success = ମିଥ୍ୟା;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
