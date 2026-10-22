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


#अगर_अघोषित HB_DIRECTWRITE_HH
#घोषणा HB_DIRECTWRITE_HH

#समावेश "hb.hh"

#समावेश "hb-directwrite.h"

#समावेश "hb-mutex.hh"
#समावेश "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
प्रकार HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

ढाँचा <typename Type, typename ...Ts>
अटल Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, माप (Type));
  अगर (unlikely (!obj))
    वापस nullptr;

  वापस नया (obj) Type (std::forward<Ts> (args)...);
}

ढाँचा <typename Type>
अटल व्योम
hb_directwrite_object_destroy (Type *obj)
{
  अगर (!obj)
    वापस;

  obj->~Type ();
  hb_free (obj);
}

श्रेणी DWriteFontFileLoader final : खुला IDWriteFontFileLoader
{
निजी:
  hb_reference_गिनती_t m_Ref_गिनती;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_अग्र_Font_File_कुंजी = 0;
खुला:
  DWriteFontFileLoader ()
  {
    m_Ref_गिनती.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_अग्र_Font_File_कुंजी, fontFileStream);
    वापस m_अग्र_Font_File_कुंजी++;
  }
  व्योम UnregisterFontFileStream (uint64_t font_File_कुंजी)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.लो (font_File_कुंजी);
    अगर (stream)
    {
      mFontStreams.del (font_File_कुंजी);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID स्थिर& iid, OUT व्योम** ppObject)
  { वापस S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    वापस m_Ref_गिनती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_गिनती = m_Ref_गिनती.डेसी () - 1;
    निश्चित (ref_गिनती >= 0);
    अगर (ref_गिनती)
      वापस ref_गिनती;
    hb_directwrite_object_destroy (यह);
    वापस 0;
  }

  // IDWriteFontFileLoader methods
  भव HRESULT STDMETHODCALLTYPE
  Create_Stream_From_कुंजी (व्योम स्थिर* font_File_Reference_कुंजी,
		       uint32_t font_File_Reference_कुंजी_आकार,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    अगर (font_File_Reference_कुंजी_आकार != माप (uint64_t))
      वापस E_INVALIDARG;
    uint64_t font_File_कुंजी = * (uint64_t *) font_File_Reference_कुंजी;
    IDWriteFontFileStream *stream = mFontStreams.लो (font_File_कुंजी);
    अगर (!stream)
      वापस E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    वापस S_OK;
  }

  ~DWriteFontFileLoader()
  {
    क्रम (स्वतः v : mFontStreams.values ())
      v->Release ();
  }
};

श्रेणी DWriteFontFileStream final : खुला IDWriteFontFileStream
{
निजी:
  hb_reference_गिनती_t m_Ref_गिनती;
  hb_blob_t *mBlob;
  uint8_t *m_तथ्य;
  अचिन्हित m_आकार;
  DWriteFontFileLoader *mLoader;
खुला:
  uint64_t font_File_कुंजी;
खुला:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID स्थिर& iid, OUT व्योम** ppObject)
  { वापस S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    वापस m_Ref_गिनती.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    चिन्हित ref_गिनती = m_Ref_गिनती.डेसी () - 1;
    निश्चित (ref_गिनती >= 0);
    अगर (ref_गिनती)
      वापस ref_गिनती;
    hb_directwrite_object_destroy (यह);
    वापस 0;
  }

  // IDWriteFontFileStream methods
  भव HRESULT STDMETHODCALLTYPE
  ReadFileFragment (व्योम स्थिर** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragment_आकार,
		    OUT व्योम** fragmentContext)
  {
    // We are required to do bounds checking.
    अगर (fileOffset + fragment_आकार > m_आकार) वापस E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    माप_प्रकार सूचकांक_2 = static_cast<माप_प्रकार> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &m_तथ्य[सूचकांक_2];
    *fragmentContext = nullptr;
    वापस S_OK;
  }

  भव व्योम STDMETHODCALLTYPE
  ReleaseFileFragment (व्योम* fragmentContext) {}

  भव HRESULT STDMETHODCALLTYPE
  Get_File_आकार (OUT UINT64* file_आकार_2)
  {
    *file_आकार_2 = m_आकार;
    वापस S_OK;
  }

  भव HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { वापस E_NOTIMPL; }

  ~DWriteFontFileStream();
};

काष्ठा hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    अगर (unlikely (hr != S_OK))
      वापस;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    अगर (unlikely (!fontFileLoader))
      वापस;
    अगर (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      वापस;

    success = true;
  }
  ~hb_directwrite_global_t ()
  {
    अगर (fontFileLoader)
      fontFileLoader->Release ();
    अगर (dwriteFactory)
      dwriteFactory->Release ();
  }

  bool success = false;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, अचिन्हित सूचकांक_2);


#पूर्ण_अगर /* HB_DIRECTWRITE_HH */
