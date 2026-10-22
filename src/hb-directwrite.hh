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

ढाँचा <typename Type, typename ...Ts>
स्थिर Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, परिमाण (Type));
  यदि (unlikely (!obj))
    फर्काऊ nullptr;

  फर्काऊ नूतन (obj) Type (std::forward<Ts> (args)...);
}

ढाँचा <typename Type>
स्थिर शून्य
hb_directwrite_object_destroy (Type *obj)
{
  यदि (!obj)
    फर्काऊ;

  obj->~Type ();
  hb_free (obj);
}

वर्ग DWriteFontFileLoader final : सार्वजनिक IDWriteFontFileLoader
{
गुप्त:
  hb_reference_गन्ती_t m_Ref_गन्ती;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_साँचो = 0;
सार्वजनिक:
  DWriteFontFileLoader ()
  {
    m_Ref_गन्ती.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_साँचो, fontFileStream);
    फर्काऊ m_Next_Font_File_साँचो++;
  }
  शून्य UnregisterFontFileStream (uint64_t font_File_साँचो)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_साँचो);
    यदि (stream)
    {
      mFontStreams.del (font_File_साँचो);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID अचर& iid, OUT शून्य** ppObject)
  { फर्काऊ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    फर्काऊ m_Ref_गन्ती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_गन्ती = m_Ref_गन्ती.dec () - 1;
    प्रतिज्ञा (ref_गन्ती >= 0);
    यदि (ref_गन्ती)
      फर्काऊ ref_गन्ती;
    hb_directwrite_object_destroy (एतत्);
    फर्काऊ 0;
  }

  // IDWriteFontFileLoader methods
  आभास HRESULT STDMETHODCALLTYPE
  Create_Stream_From_साँचो (शून्य अचर* font_File_Reference_साँचो,
		       uint32_t font_File_Reference_साँचो_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    यदि (font_File_Reference_साँचो_Size != परिमाण (uint64_t))
      फर्काऊ E_INVALIDARG;
    uint64_t font_File_साँचो = * (uint64_t *) font_File_Reference_साँचो;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_साँचो);
    यदि (!stream)
      फर्काऊ E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    फर्काऊ S_OK;
  }

  ~DWriteFontFileLoader()
  {
    लागि (स्वतः v : mFontStreams.values ())
      v->Release ();
  }
};

वर्ग DWriteFontFileStream final : सार्वजनिक IDWriteFontFileStream
{
गुप्त:
  hb_reference_गन्ती_t m_Ref_गन्ती;
  hb_blob_t *mBlob;
  uint8_t *mData;
  अचिन्हित mSize;
  DWriteFontFileLoader *mLoader;
सार्वजनिक:
  uint64_t font_File_साँचो;
सार्वजनिक:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID अचर& iid, OUT शून्य** ppObject)
  { फर्काऊ S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    फर्काऊ m_Ref_गन्ती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_गन्ती = m_Ref_गन्ती.dec () - 1;
    प्रतिज्ञा (ref_गन्ती >= 0);
    यदि (ref_गन्ती)
      फर्काऊ ref_गन्ती;
    hb_directwrite_object_destroy (एतत्);
    फर्काऊ 0;
  }

  // IDWriteFontFileStream methods
  आभास HRESULT STDMETHODCALLTYPE
  ReadFileFragment (शून्य अचर** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT शून्य** fragmentContext)
  {
    // We are required to do bounds checking.
    यदि (fileOffset + fragmentSize > mSize) फर्काऊ E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    माप_प्रकार index = static_cast<माप_प्रकार> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    फर्काऊ S_OK;
  }

  आभास शून्य STDMETHODCALLTYPE
  ReleaseFileFragment (शून्य* fragmentContext) {}

  आभास HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    फर्काऊ S_OK;
  }

  आभास HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { फर्काऊ E_NOTIMPL; }

  ~DWriteFontFileStream();
};

संरचना hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    यदि (unlikely (hr != S_OK))
      फर्काऊ;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    यदि (unlikely (!fontFileLoader))
      फर्काऊ;
    यदि (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      फर्काऊ;

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
dw_face_create (hb_blob_t *blob, अचिन्हित index);


#endif /* HB_DIRECTWRITE_HH */
