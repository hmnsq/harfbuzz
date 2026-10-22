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
  यदि (unlikely (!obj))
    प्रत्यागम nullptr;

  प्रत्यागम नूतन (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
स्थिर शून्य
hb_directwrite_object_destroy (Type *obj)
{
  यदि (!obj)
    प्रत्यागम;

  obj->~Type ();
  hb_free (obj);
}

वर्ग DWriteFontFileLoader final : सार्वजनिक IDWriteFontFileLoader
{
गुप्त:
  hb_reference_गिनती_t m_Ref_गिनती;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_कुंजी = 0;
सार्वजनिक:
  DWriteFontFileLoader ()
  {
    m_Ref_गिनती.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_कुंजी, fontFileStream);
    प्रत्यागम m_Next_Font_File_कुंजी++;
  }
  शून्य UnregisterFontFileStream (uint64_t font_File_कुंजी)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_कुंजी);
    यदि (stream)
    {
      mFontStreams.del (font_File_कुंजी);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ध्रुव& iid, OUT शून्य** ppObject)
  { प्रत्यागम S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    प्रत्यागम m_Ref_गिनती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_गिनती = m_Ref_गिनती.dec () - 1;
    प्रतिज्ञा (ref_गिनती >= 0);
    यदि (ref_गिनती)
      प्रत्यागम ref_गिनती;
    hb_directwrite_object_destroy (एतत्);
    प्रत्यागम 0;
  }

  // IDWriteFontFileLoader methods
  आभास HRESULT STDMETHODCALLTYPE
  Create_Stream_From_कुंजी (शून्य ध्रुव* font_File_Reference_कुंजी,
		       uint32_t font_File_Reference_कुंजी_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    यदि (font_File_Reference_कुंजी_Size != परिमाण (uint64_t))
      प्रत्यागम E_INVALIDARG;
    uint64_t font_File_कुंजी = * (uint64_t *) font_File_Reference_कुंजी;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_कुंजी);
    यदि (!stream)
      प्रत्यागम E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    प्रत्यागम S_OK;
  }

  ~DWriteFontFileLoader()
  {
    प्रति (auto v : mFontStreams.values ())
      v->Release ();
  }
};

वर्ग DWriteFontFileStream final : सार्वजनिक IDWriteFontFileStream
{
गुप्त:
  hb_reference_गिनती_t m_Ref_गिनती;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
सार्वजनिक:
  uint64_t font_File_कुंजी;
सार्वजनिक:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ध्रुव& iid, OUT शून्य** ppObject)
  { प्रत्यागम S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    प्रत्यागम m_Ref_गिनती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_गिनती = m_Ref_गिनती.dec () - 1;
    प्रतिज्ञा (ref_गिनती >= 0);
    यदि (ref_गिनती)
      प्रत्यागम ref_गिनती;
    hb_directwrite_object_destroy (एतत्);
    प्रत्यागम 0;
  }

  // IDWriteFontFileStream methods
  आभास HRESULT STDMETHODCALLTYPE
  ReadFileFragment (शून्य ध्रुव** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT शून्य** fragmentContext)
  {
    // We are required to do bounds checking.
    यदि (fileOffset + fragmentSize > mSize) प्रत्यागम E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    प्रत्यागम S_OK;
  }

  आभास शून्य STDMETHODCALLTYPE
  ReleaseFileFragment (शून्य* fragmentContext) {}

  आभास HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    प्रत्यागम S_OK;
  }

  आभास HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { प्रत्यागम E_NOTIMPL; }

  ~DWriteFontFileStream();
};

संरचना hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    यदि (unlikely (hr != S_OK))
      प्रत्यागम;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    यदि (unlikely (!fontFileLoader))
      प्रत्यागम;
    यदि (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      प्रत्यागम;

    success = सत्य;
  }
  ~hb_directwrite_global_t ()
  {
    यदि (fontFileLoader)
      fontFileLoader->Release ();
    यदि (dwriteFactory)
      dwriteFactory->Release ();
  }

  तार्किक success = असत्य;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
