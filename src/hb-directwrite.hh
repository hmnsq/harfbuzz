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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-directwrite.h"

#അന്തര്ഭാവ "hb-mutex.hh"
#അന്തര്ഭാവ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
തരനിർവചനം HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
സ്ഥിതം Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, വലുപ്പം (Type));
  എങ്കിൽ (unlikely (!obj))
    തിരികെ nullptr;

  തിരികെ പുതിയ (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
സ്ഥിതം ശൂന്യം
hb_directwrite_object_destroy (Type *obj)
{
  എങ്കിൽ (!obj)
    തിരികെ;

  obj->~Type ();
  hb_free (obj);
}

വർഗ്ഗം DWriteFontFileLoader final : പൊതു IDWriteFontFileLoader
{
സ്വകാര്യ:
  hb_reference_എണ്ണം_t m_Ref_എണ്ണം;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_താക്കോൽ = 0;
പൊതു:
  DWriteFontFileLoader ()
  {
    m_Ref_എണ്ണം.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_താക്കോൽ, fontFileStream);
    തിരികെ m_Next_Font_File_താക്കോൽ++;
  }
  ശൂന്യം UnregisterFontFileStream (uint64_t font_File_താക്കോൽ)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_താക്കോൽ);
    എങ്കിൽ (stream)
    {
      mFontStreams.del (font_File_താക്കോൽ);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID സ്ഥിരം& iid, OUT ശൂന്യം** ppObject)
  { തിരികെ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    തിരികെ m_Ref_എണ്ണം.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_എണ്ണം = m_Ref_എണ്ണം.dec () - 1;
    പ്രതിജ്ഞാ (ref_എണ്ണം >= 0);
    എങ്കിൽ (ref_എണ്ണം)
      തിരികെ ref_എണ്ണം;
    hb_directwrite_object_destroy (ഇത്);
    തിരികെ 0;
  }

  // IDWriteFontFileLoader methods
  മായാ HRESULT STDMETHODCALLTYPE
  Create_Stream_From_താക്കോൽ (ശൂന്യം സ്ഥിരം* font_File_Reference_താക്കോൽ,
		       uint32_t font_File_Reference_താക്കോൽ_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    എങ്കിൽ (font_File_Reference_താക്കോൽ_Size != വലുപ്പം (uint64_t))
      തിരികെ E_INVALIDARG;
    uint64_t font_File_താക്കോൽ = * (uint64_t *) font_File_Reference_താക്കോൽ;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_താക്കോൽ);
    എങ്കിൽ (!stream)
      തിരികെ E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    തിരികെ S_OK;
  }

  ~DWriteFontFileLoader()
  {
    വേണ്ടി (auto v : mFontStreams.values ())
      v->Release ();
  }
};

വർഗ്ഗം DWriteFontFileStream final : പൊതു IDWriteFontFileStream
{
സ്വകാര്യ:
  hb_reference_എണ്ണം_t m_Ref_എണ്ണം;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
പൊതു:
  uint64_t font_File_താക്കോൽ;
പൊതു:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID സ്ഥിരം& iid, OUT ശൂന്യം** ppObject)
  { തിരികെ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    തിരികെ m_Ref_എണ്ണം.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_എണ്ണം = m_Ref_എണ്ണം.dec () - 1;
    പ്രതിജ്ഞാ (ref_എണ്ണം >= 0);
    എങ്കിൽ (ref_എണ്ണം)
      തിരികെ ref_എണ്ണം;
    hb_directwrite_object_destroy (ഇത്);
    തിരികെ 0;
  }

  // IDWriteFontFileStream methods
  മായാ HRESULT STDMETHODCALLTYPE
  ReadFileFragment (ശൂന്യം സ്ഥിരം** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT ശൂന്യം** fragmentContext)
  {
    // We are required to do bounds checking.
    എങ്കിൽ (fileOffset + fragmentSize > mSize) തിരികെ E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    തിരികെ S_OK;
  }

  മായാ ശൂന്യം STDMETHODCALLTYPE
  ReleaseFileFragment (ശൂന്യം* fragmentContext) {}

  മായാ HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    തിരികെ S_OK;
  }

  മായാ HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { തിരികെ E_NOTIMPL; }

  ~DWriteFontFileStream();
};

ഘടന hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    എങ്കിൽ (unlikely (hr != S_OK))
      തിരികെ;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    എങ്കിൽ (unlikely (!fontFileLoader))
      തിരികെ;
    എങ്കിൽ (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      തിരികെ;

    success = സത്യം;
  }
  ~hb_directwrite_global_t ()
  {
    എങ്കിൽ (fontFileLoader)
      fontFileLoader->Release ();
    എങ്കിൽ (dwriteFactory)
      dwriteFactory->Release ();
  }

  തർക്കം success = അസത്യം;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
