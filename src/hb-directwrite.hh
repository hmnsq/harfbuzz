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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-directwrite.h"

#அந்தர்பாவ "hb-mutex.hh"
#அந்தர்பாவ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
வகைவரை HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

template <typename Type, typename ...Ts>
நிலையான Type *
hb_directwrite_object_create (Ts&&... args)
{
  Type *obj = (Type *) hb_calloc (1, அளவு (Type));
  எனில் (unlikely (!obj))
    திரும்பு nullptr;

  திரும்பு புதிய (obj) Type (std::forward<Ts> (args)...);
}

template <typename Type>
நிலையான வெறுமை
hb_directwrite_object_destroy (Type *obj)
{
  எனில் (!obj)
    திரும்பு;

  obj->~Type ();
  hb_free (obj);
}

வகுப்பு DWriteFontFileLoader final : பொது IDWriteFontFileLoader
{
தனிப்பட்ட:
  hb_reference_மொத்தம்_t m_Ref_மொத்தம்;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWriteFontFileStream *> mFontStreams;
  uint64_t m_Next_Font_File_சாவி = 0;
பொது:
  DWriteFontFileLoader ()
  {
    m_Ref_மொத்தம்.init ();
  }

  uint64_t RegisterFontFileStream (IDWriteFontFileStream *fontFileStream)
  {
    fontFileStream->AddRef ();
    hb_lock_t lock {mutex};
    mFontStreams.set (m_Next_Font_File_சாவி, fontFileStream);
    திரும்பு m_Next_Font_File_சாவி++;
  }
  வெறுமை UnregisterFontFileStream (uint64_t font_File_சாவி)
  {
    hb_lock_t lock {mutex};
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_சாவி);
    எனில் (stream)
    {
      mFontStreams.del (font_File_சாவி);
      stream->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID மாறிலி& iid, OUT வெறுமை** ppObject)
  { திரும்பு S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    திரும்பு m_Ref_மொத்தம்.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_மொத்தம் = m_Ref_மொத்தம்.dec () - 1;
    ப்ரதிஜ்ஞா (ref_மொத்தம் >= 0);
    எனில் (ref_மொத்தம்)
      திரும்பு ref_மொத்தம்;
    hb_directwrite_object_destroy (இது);
    திரும்பு 0;
  }

  // IDWriteFontFileLoader methods
  மெய்நிகர் HRESULT STDMETHODCALLTYPE
  Create_Stream_From_சாவி (வெறுமை மாறிலி* font_File_Reference_சாவி,
		       uint32_t font_File_Reference_சாவி_Size,
		       OUT IDWriteFontFileStream** fontFileStream)
  {
    எனில் (font_File_Reference_சாவி_Size != அளவு (uint64_t))
      திரும்பு E_INVALIDARG;
    uint64_t font_File_சாவி = * (uint64_t *) font_File_Reference_சாவி;
    IDWriteFontFileStream *stream = mFontStreams.get (font_File_சாவி);
    எனில் (!stream)
      திரும்பு E_FAIL;
    stream->AddRef ();
    *fontFileStream = stream;
    திரும்பு S_OK;
  }

  ~DWriteFontFileLoader()
  {
    க்கு (auto v : mFontStreams.values ())
      v->Release ();
  }
};

வகுப்பு DWriteFontFileStream final : பொது IDWriteFontFileStream
{
தனிப்பட்ட:
  hb_reference_மொத்தம்_t m_Ref_மொத்தம்;
  hb_blob_t *mBlob;
  uint8_t *mData;
  unsigned mSize;
  DWriteFontFileLoader *mLoader;
பொது:
  uint64_t font_File_சாவி;
பொது:
  DWriteFontFileStream (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID மாறிலி& iid, OUT வெறுமை** ppObject)
  { திரும்பு S_OK; }
  IFACEMETHOD_ (ULONG, AddRef) ()
  {
    திரும்பு m_Ref_மொத்தம்.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    signed ref_மொத்தம் = m_Ref_மொத்தம்.dec () - 1;
    ப்ரதிஜ்ஞா (ref_மொத்தம் >= 0);
    எனில் (ref_மொத்தம்)
      திரும்பு ref_மொத்தம்;
    hb_directwrite_object_destroy (இது);
    திரும்பு 0;
  }

  // IDWriteFontFileStream methods
  மெய்நிகர் HRESULT STDMETHODCALLTYPE
  ReadFileFragment (வெறுமை மாறிலி** fragmentStart,
		    UINT64 fileOffset,
		    UINT64 fragmentSize,
		    OUT வெறுமை** fragmentContext)
  {
    // We are required to do bounds checking.
    எனில் (fileOffset + fragmentSize > mSize) திரும்பு E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t index = static_cast<size_t> (fileOffset);

    // We should be alive for the duration of this.
    *fragmentStart = &mData[index];
    *fragmentContext = nullptr;
    திரும்பு S_OK;
  }

  மெய்நிகர் வெறுமை STDMETHODCALLTYPE
  ReleaseFileFragment (வெறுமை* fragmentContext) {}

  மெய்நிகர் HRESULT STDMETHODCALLTYPE
  GetFileSize (OUT UINT64* fileSize)
  {
    *fileSize = mSize;
    திரும்பு S_OK;
  }

  மெய்நிகர் HRESULT STDMETHODCALLTYPE
  GetLastWriteTime (OUT UINT64* lastWriteTime) { திரும்பு E_NOTIMPL; }

  ~DWriteFontFileStream();
};

கட்டமைப்பு hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    எனில் (unlikely (hr != S_OK))
      திரும்பு;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    எனில் (unlikely (!fontFileLoader))
      திரும்பு;
    எனில் (unlikely (FAILED (dwriteFactory->RegisterFontFileLoader (fontFileLoader))))
      திரும்பு;

    success = உண்மை;
  }
  ~hb_directwrite_global_t ()
  {
    எனில் (fontFileLoader)
      fontFileLoader->Release ();
    எனில் (dwriteFactory)
      dwriteFactory->Release ();
  }

  தருக்க success = பொய்;
  IDWriteFactory *dwriteFactory = nullptr;
  DWriteFontFileLoader *fontFileLoader = nullptr;
};


HB_INTERNAL hb_directwrite_global_t *
get_directwrite_global ();

HB_INTERNAL IDWriteFontFace *
dw_face_create (hb_blob_t *blob, unsigned index);


#endif /* HB_DIRECTWRITE_HH */
