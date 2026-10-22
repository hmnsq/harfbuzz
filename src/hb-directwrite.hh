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
प्रकारव्याख्या HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

ढाँचा <typename Type, typename ...Ts>
स्थिर Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, आकार (Type));
  जर (unlikely (!obj))
    परत nullptr;

  परत नवीन (obj) Type (std::forward<Ts> (args)...);
}

ढाँचा <typename Type>
स्थिर शून्य
hb_directwrite_object_destroy (Type *obj)
{
  जर (!obj)
    परत;

  obj->~Type ();
  hb_free (obj);
}

वर्ग DWriteFontFileLoader final : सार्वजनिक IDWriteFontFileLoader
{
खाजगी:
  hb_reference_मोजणी_t m_Ref_मोजणी;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_किल्ली = 0;
सार्वजनिक:
  DWriteFontFileLoader ()
  {
    m_Ref_मोजणी.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_किल्ली, fontFileStream);
    परत m_Next_Font_File_किल्ली++;
  }
  शून्य UnregisterFontFileStream (uint64_t font_File_किल्ली)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_किल्ली);
    जर (stream)
    {
      mFontStreams.del (font_File_किल्ली);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID स्थिरांक& iid, OUT शून्य** ppObject)
  { परत S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    परत m_Ref_मोजणी.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_मोजणी = m_Ref_मोजणी.dec () - 1;
    प्रतिज्ञा (ref_मोजणी >= 0);
    जर (ref_मोजणी)
      परत ref_मोजणी;
    hb_directwrite_object_destroy (हे);
    परत 0;
  }

  // IDWriteFontFileLoader methods
  आभासी HRESULT STDMETHODCALLTYPE
  Create_Stream_From_किल्ली (शून्य स्थिरांक* font_File_Reference_किल्ली,
		       uint32_t font_File_Reference_किल्ली_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    जर (font_File_Reference_किल्ली_Size != आकार (uint64_t))
      परत E_INVALIDARG;
    uint64_t font_File_किल्ली = * (uint64_t *) font_File_Reference_किल्ली;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_किल्ली);
    जर (!stream)
      परत E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    परत S_OK;
  }

  ~DWriteFontFileLoader()
  {
    साठी (स्वतः v : mFontStreams.values ())
      v->Release ();
  }
};

वर्ग DWriteFontFileStream final : सार्वजनिक IDWriteFontFileStream
{
खाजगी:
  hb_reference_मोजणी_t m_Ref_मोजणी;
  hb_blob_t *mBlob;
  uint8_t *mData;
  अचिन्हित mSize;
  DWriteFontFileLoader *mLoader;
सार्वजनिक:
  uint64_t font_File_किल्ली;
सार्वजनिक:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID स्थिरांक& iid, OUT शून्य** ppObject)
  { परत S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    परत m_Ref_मोजणी.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_मोजणी = m_Ref_मोजणी.dec () - 1;
    प्रतिज्ञा (ref_मोजणी >= 0);
    जर (ref_मोजणी)
      परत ref_मोजणी;
    hb_directwrite_object_destroy (हे);
    परत 0;
  }

  // IDWriteFontFileStream methods
  आभासी HRESULT STDMETHODCALLTYPE
  ReadFileFragment (शून्य स्थिरांक** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT शून्य** fragmentContext)
  {
    // We are required to do bounds checking.
    जर (fileOffset + fragmentSize > mSize) परत E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    माप_प्रकार index = static_cast<माप_प्रकार> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    परत S_OK;
  }

  आभासी शून्य STDMETHODCALLTYPE
  ReleaseFileFragment (शून्य* fragmentContext) {}

  आभासी HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    परत S_OK;
  }

  आभासी HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { परत E_NOTIMPL; }

  ~DWriteFontFileStream();
};

संरचना hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    जर (unlikely (hr != S_OK))
      परत;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    जर (unlikely (!fontFileLoader))
      परत;
    जर (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      परत;

    success = सत्य;
  }
  ~hb_directwrite_global_t ()
  {
    जर (fontFileLoader)
      fontFileLoader->Release ();
    जर (dwriteFactory)
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
