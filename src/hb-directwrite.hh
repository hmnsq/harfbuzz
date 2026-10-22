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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-directwrite.h"

#अन्तर्भाव "hb-mutex.hh"
#अन्तर्भाव "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
प्रकारनिर्देश HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
स्थिर Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, परिमाण (Type));
  सचे (unlikely (!obj))
    पटिनिवत्त nullptr;

  पटिनिवत्त नूतन (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
स्थिर शून्य
hb_directwrite_object_destroy (Type *obj)
{
  सचे (!obj)
    पटिनिवत्त;

  obj->~Type ();
  hb_free (obj);
}

वग्ग DWriteFontFileLoader final : सार्वजनिक IDWriteFontFileLoader
{
गुप्त:
  hb_reference_गणनम्_t m_Ref_गणनम्;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_कुञ्चिका = 0;
सार्वजनिक:
  DWriteFontFileLoader ()
  {
    m_Ref_गणनम्.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_कुञ्चिका, fontFileStream);
    पटिनिवत्त m_Next_Font_File_कुञ्चिका++;
  }
  शून्य UnregisterFontFileStream (uint64_t font_File_कुञ्चिका)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_कुञ्चिका);
    सचे (stream)
    {
      mFontStreams.del (font_File_कुञ्चिका);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ध्रुव& iid, OUT शून्य** ppObject)
  { पटिनिवत्त S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    पटिनिवत्त m_Ref_गणनम्.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_गणनम् = m_Ref_गणनम्.dec () - 1;
    प्रतिज्ञा (ref_गणनम् >= 0);
    सचे (ref_गणनम्)
      पटिनिवत्त ref_गणनम्;
    hb_directwrite_object_destroy (एतत्);
    पटिनिवत्त 0;
  }

  // IDWriteFontFileLoader methods
  आभास HRESULT STDMETHODCALLTYPE
  Create_Stream_From_कुञ्चिका (शून्य ध्रुव* font_File_Reference_कुञ्चिका,
		       uint32_t font_File_Reference_कुञ्चिका_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    सचे (font_File_Reference_कुञ्चिका_Size != परिमाण (uint64_t))
      पटिनिवत्त E_INVALIDARG;
    uint64_t font_File_कुञ्चिका = * (uint64_t *) font_File_Reference_कुञ्चिका;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_कुञ्चिका);
    सचे (!stream)
      पटिनिवत्त E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    पटिनिवत्त S_OK;
  }

  ~DWriteFontFileLoader()
  {
    अत्थाय (auto v : mFontStreams.values ())
      v->Release ();
  }
};

वग्ग DWriteFontFileStream final : सार्वजनिक IDWriteFontFileStream
{
गुप्त:
  hb_reference_गणनम्_t m_Ref_गणनम्;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
सार्वजनिक:
  uint64_t font_File_कुञ्चिका;
सार्वजनिक:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ध्रुव& iid, OUT शून्य** ppObject)
  { पटिनिवत्त S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    पटिनिवत्त m_Ref_गणनम्.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_गणनम् = m_Ref_गणनम्.dec () - 1;
    प्रतिज्ञा (ref_गणनम् >= 0);
    सचे (ref_गणनम्)
      पटिनिवत्त ref_गणनम्;
    hb_directwrite_object_destroy (एतत्);
    पटिनिवत्त 0;
  }

  // IDWriteFontFileStream methods
  आभास HRESULT STDMETHODCALLTYPE
  ReadFileFragment (शून्य ध्रुव** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT शून्य** fragmentContext)
  {
    // We are required to do bounds checking.
    सचे (fileOffset + fragmentSize > mSize) पटिनिवत्त E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    पटिनिवत्त S_OK;
  }

  आभास शून्य STDMETHODCALLTYPE
  ReleaseFileFragment (शून्य* fragmentContext) {}

  आभास HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    पटिनिवत्त S_OK;
  }

  आभास HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { पटिनिवत्त E_NOTIMPL; }

  ~DWriteFontFileStream();
};

संरचना hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    सचे (unlikely (hr != S_OK))
      पटिनिवत्त;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    सचे (unlikely (!fontFileLoader))
      पटिनिवत्त;
    सचे (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      पटिनिवत्त;

    success = सच्च;
  }
  ~hb_directwrite_global_t ()
  {
    सचे (fontFileLoader)
      fontFileLoader->Release ();
    सचे (dwriteFactory)
      dwriteFactory->Release ();
  }

  तार्किक success = मिच्छा;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
