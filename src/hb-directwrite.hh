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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-directwrite.h"

#અન્તર્ભાવ "hb-mutex.hh"
#અન્તર્ભાવ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
પ્રકારવ્યાખ્યા HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
સ્થિર Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, કદ (Type));
  જો (unlikely (!obj))
    પાછા nullptr;

  પાછા નવું (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
સ્થિર શૂન્ય
hb_directwrite_object_destroy (Type *obj)
{
  જો (!obj)
    પાછા;

  obj->~Type ();
  hb_free (obj);
}

વર્ગ DWriteFontFileLoader final : જાહેર IDWriteFontFileLoader
{
ખાનગી:
  hb_reference_ગણતરી_t m_Ref_ગણતરી;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_ચાવી = 0;
જાહેર:
  DWriteFontFileLoader ()
  {
    m_Ref_ગણતરી.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_ચાવી, fontFileStream);
    પાછા m_Next_Font_File_ચાવી++;
  }
  શૂન્ય UnregisterFontFileStream (uint64_t font_File_ચાવી)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_ચાવી);
    જો (stream)
    {
      mFontStreams.del (font_File_ચાવી);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID અચલ& iid, OUT શૂન્ય** ppObject)
  { પાછા S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    પાછા m_Ref_ગણતરી.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ગણતરી = m_Ref_ગણતરી.dec () - 1;
    પ્રતિજ્ઞા (ref_ગણતરી >= 0);
    જો (ref_ગણતરી)
      પાછા ref_ગણતરી;
    hb_directwrite_object_destroy (આ);
    પાછા 0;
  }

  // IDWriteFontFileLoader methods
  આભાસી HRESULT STDMETHODCALLTYPE
  Create_Stream_From_ચાવી (શૂન્ય અચલ* font_File_Reference_ચાવી,
		       uint32_t font_File_Reference_ચાવી_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    જો (font_File_Reference_ચાવી_Size != કદ (uint64_t))
      પાછા E_INVALIDARG;
    uint64_t font_File_ચાવી = * (uint64_t *) font_File_Reference_ચાવી;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_ચાવી);
    જો (!stream)
      પાછા E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    પાછા S_OK;
  }

  ~DWriteFontFileLoader()
  {
    માટે (auto v : mFontStreams.values ())
      v->Release ();
  }
};

વર્ગ DWriteFontFileStream final : જાહેર IDWriteFontFileStream
{
ખાનગી:
  hb_reference_ગણતરી_t m_Ref_ગણતરી;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
જાહેર:
  uint64_t font_File_ચાવી;
જાહેર:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID અચલ& iid, OUT શૂન્ય** ppObject)
  { પાછા S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    પાછા m_Ref_ગણતરી.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ગણતરી = m_Ref_ગણતરી.dec () - 1;
    પ્રતિજ્ઞા (ref_ગણતરી >= 0);
    જો (ref_ગણતરી)
      પાછા ref_ગણતરી;
    hb_directwrite_object_destroy (આ);
    પાછા 0;
  }

  // IDWriteFontFileStream methods
  આભાસી HRESULT STDMETHODCALLTYPE
  ReadFileFragment (શૂન્ય અચલ** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT શૂન્ય** fragmentContext)
  {
    // We are required to do bounds checking.
    જો (fileOffset + fragmentSize > mSize) પાછા E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    પાછા S_OK;
  }

  આભાસી શૂન્ય STDMETHODCALLTYPE
  ReleaseFileFragment (શૂન્ય* fragmentContext) {}

  આભાસી HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    પાછા S_OK;
  }

  આભાસી HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { પાછા E_NOTIMPL; }

  ~DWriteFontFileStream();
};

સંરચના hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    જો (unlikely (hr != S_OK))
      પાછા;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    જો (unlikely (!fontFileLoader))
      પાછા;
    જો (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      પાછા;

    success = સાચું;
  }
  ~hb_directwrite_global_t ()
  {
    જો (fontFileLoader)
      fontFileLoader->Release ();
    જો (dwriteFactory)
      dwriteFactory->Release ();
  }

  તાર્કિક success = ખોટું;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
