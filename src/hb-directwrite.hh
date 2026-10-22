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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-directwrite.h"

#ಅನ್ತರ್ಭಾವ "hb-mutex.hh"
#ಅನ್ತರ್ಭಾವ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
ಪ್ರಕಾರನಿರ್ದೇಶ HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
ಸ್ಥಿರ Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, ಪರಿಮಾಣ (Type));
  ಯದಿ (unlikely (!obj))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  ಪ್ರತ್ಯಾಗಮ ನೂತನ (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
ಸ್ಥಿರ ಶೂನ್ಯ
hb_directwrite_object_destroy (Type *obj)
{
  ಯದಿ (!obj)
    ಪ್ರತ್ಯಾಗಮ;

  obj->~Type ();
  hb_free (obj);
}

ವರ್ಗ DWriteFontFileLoader final : ಸಾರ್ವಜನಿಕ IDWriteFontFileLoader
{
ಗುಪ್ತ:
  hb_reference_ಎಣಿಕೆ_t m_Ref_ಎಣಿಕೆ;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_ಕೀಲಿ = 0;
ಸಾರ್ವಜನಿಕ:
  DWriteFontFileLoader ()
  {
    m_Ref_ಎಣಿಕೆ.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_ಕೀಲಿ, fontFileStream);
    ಪ್ರತ್ಯಾಗಮ m_Next_Font_File_ಕೀಲಿ++;
  }
  ಶೂನ್ಯ UnregisterFontFileStream (uint64_t font_File_ಕೀಲಿ)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_ಕೀಲಿ);
    ಯದಿ (stream)
    {
      mFontStreams.del (font_File_ಕೀಲಿ);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ಧ್ರುವ& iid, OUT ಶೂನ್ಯ** ppObject)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ಪ್ರತ್ಯಾಗಮ m_Ref_ಎಣಿಕೆ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ಎಣಿಕೆ = m_Ref_ಎಣಿಕೆ.dec () - 1;
    ಪ್ರತಿಜ್ಞಾ (ref_ಎಣಿಕೆ >= 0);
    ಯದಿ (ref_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ;
    hb_directwrite_object_destroy (ಏತತ್);
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  // IDWriteFontFileLoader methods
  ಆಭಾಸ HRESULT STDMETHODCALLTYPE
  Create_Stream_From_ಕೀಲಿ (ಶೂನ್ಯ ಧ್ರುವ* font_File_Reference_ಕೀಲಿ,
		       uint32_t font_File_Reference_ಕೀಲಿ_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    ಯದಿ (font_File_Reference_ಕೀಲಿ_Size != ಪರಿಮಾಣ (uint64_t))
      ಪ್ರತ್ಯಾಗಮ E_INVALIDARG;
    uint64_t font_File_ಕೀಲಿ = * (uint64_t *) font_File_Reference_ಕೀಲಿ;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_ಕೀಲಿ);
    ಯದಿ (!stream)
      ಪ್ರತ್ಯಾಗಮ E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  ~DWriteFontFileLoader()
  {
    ಪ್ರತಿ (auto v : mFontStreams.values ())
      v->Release ();
  }
};

ವರ್ಗ DWriteFontFileStream final : ಸಾರ್ವಜನಿಕ IDWriteFontFileStream
{
ಗುಪ್ತ:
  hb_reference_ಎಣಿಕೆ_t m_Ref_ಎಣಿಕೆ;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
ಸಾರ್ವಜನಿಕ:
  uint64_t font_File_ಕೀಲಿ;
ಸಾರ್ವಜನಿಕ:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ಧ್ರುವ& iid, OUT ಶೂನ್ಯ** ppObject)
  { ಪ್ರತ್ಯಾಗಮ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    ಪ್ರತ್ಯಾಗಮ m_Ref_ಎಣಿಕೆ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_ಎಣಿಕೆ = m_Ref_ಎಣಿಕೆ.dec () - 1;
    ಪ್ರತಿಜ್ಞಾ (ref_ಎಣಿಕೆ >= 0);
    ಯದಿ (ref_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ ref_ಎಣಿಕೆ;
    hb_directwrite_object_destroy (ಏತತ್);
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  // IDWriteFontFileStream methods
  ಆಭಾಸ HRESULT STDMETHODCALLTYPE
  ReadFileFragment (ಶೂನ್ಯ ಧ್ರುವ** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT ಶೂನ್ಯ** fragmentContext)
  {
    // We are required to do bounds checking.
    ಯದಿ (fileOffset + fragmentSize > mSize) ಪ್ರತ್ಯಾಗಮ E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  ಆಭಾಸ ಶೂನ್ಯ STDMETHODCALLTYPE
  ReleaseFileFragment (ಶೂನ್ಯ* fragmentContext) {}

  ಆಭಾಸ HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    ಪ್ರತ್ಯಾಗಮ S_OK;
  }

  ಆಭಾಸ HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { ಪ್ರತ್ಯಾಗಮ E_NOTIMPL; }

  ~DWriteFontFileStream();
};

ಸಂರಚನಾ hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    ಯದಿ (unlikely (hr != S_OK))
      ಪ್ರತ್ಯಾಗಮ;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    ಯದಿ (unlikely (!fontFileLoader))
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      ಪ್ರತ್ಯಾಗಮ;

    success = ಸತ್ಯ;
  }
  ~hb_directwrite_global_t ()
  {
    ಯದಿ (fontFileLoader)
      fontFileLoader->Release ();
    ಯದಿ (dwriteFactory)
      dwriteFactory->Release ();
  }

  ತಾರ್ಕಿಕ success = ಅಸತ್ಯ;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
