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


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DIRECTWRITE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DIRECTWRITE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-directwrite.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-mutex.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ Type *
hb_directwrite_object_create (Ts&&... ਦਲੀਲਾਂ)
{
  Type *obj = (Type *) hb_calloc (1, ਆਕਾਰ (Type));
  ਜੇ (unlikely (!obj))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ ਨਵਾਂ (obj) Type (std::forward<Ts> (ਦਲੀਲਾਂ)...);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਖਾਲੀ
hb_directwrite_object_destroy (Type *obj)
{
  ਜੇ (!obj)
    ਵਾਪਸ;

  obj->~Type ();
  hb_free (obj);
}

ਵਰਗ DWriteFontFileLoader final : ਜਨਤਕ IDWriteFontFileLoader
{
ਨਿਜੀ:
  hb_reference_ਗਿਣਤੀ_t m_Ref_ਗਿਣਤੀ;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_ਧਾਰਾ *> mFontStreams;
  uint64_t m_ਅਗਲਾ_Font_File_ਕੁੰਜੀ = 0;
ਜਨਤਕ:
  DWriteFontFileLoader ()
  {
    m_Ref_ਗਿਣਤੀ.init ();
  }

  uint64_t ਰਜਿਸਟਰ_Font_File_ਧਾਰਾ (IDWrite_Font_File_ਧਾਰਾ *font_File_ਧਾਰਾ)
  {
    font_File_ਧਾਰਾ->ਜੋੜੋ_Ref ();
    hb_ਤਾਲਾ_t lock {mutex};
    mFontStreams.ਸੈੱਟ (m_ਅਗਲਾ_Font_File_ਕੁੰਜੀ, font_File_ਧਾਰਾ);
    ਵਾਪਸ m_ਅਗਲਾ_Font_File_ਕੁੰਜੀ++;
  }
  ਖਾਲੀ Unregister_Font_File_ਧਾਰਾ (uint64_t font_File_ਕੁੰਜੀ)
  {
    hb_ਤਾਲਾ_t lock {mutex};
    IDWrite_Font_File_ਧਾਰਾ *ਧਾਰਾ = mFontStreams.get (font_File_ਕੁੰਜੀ);
    ਜੇ (ਧਾਰਾ)
    {
      mFontStreams.del (font_File_ਕੁੰਜੀ);
      ਧਾਰਾ->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ਅਚਲ& iid, ਬਾਹਰਲਾ_3 ਖਾਲੀ** ppObject)
  { ਵਾਪਸ S_OK; }
  IFACEMETHOD_ (ULONG, ਜੋੜੋ_Ref) ()
  {
    ਵਾਪਸ m_Ref_ਗਿਣਤੀ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    ਨਿਸ਼ਾਨੀਵਾਲਾ ref_ਗਿਣਤੀ = m_Ref_ਗਿਣਤੀ.dec () - 1;
    ਪ੍ਰਤਿਜ੍ਞਾ (ref_ਗਿਣਤੀ >= 0);
    ਜੇ (ref_ਗਿਣਤੀ)
      ਵਾਪਸ ref_ਗਿਣਤੀ;
    hb_directwrite_object_destroy (ਇਹ);
    ਵਾਪਸ 0;
  }

  // IDWriteFontFileLoader methods
  ਆਭਾਸੀ HRESULT STDMETHODCALLTYPE
  Create_ਧਾਰਾ_From_ਕੁੰਜੀ (ਖਾਲੀ ਅਚਲ* font_File_Reference_ਕੁੰਜੀ,
		       uint32_t font_File_Reference_ਕੁੰਜੀ_ਮਾਪ,
		       ਬਾਹਰਲਾ_3 IDWrite_Font_File_ਧਾਰਾ** font_File_ਧਾਰਾ)
  {
    ਜੇ (font_File_Reference_ਕੁੰਜੀ_ਮਾਪ != ਆਕਾਰ (uint64_t))
      ਵਾਪਸ E_INVALIDARG;
    uint64_t font_File_ਕੁੰਜੀ = * (uint64_t *) font_File_Reference_ਕੁੰਜੀ;
    IDWrite_Font_File_ਧਾਰਾ *ਧਾਰਾ = mFontStreams.get (font_File_ਕੁੰਜੀ);
    ਜੇ (!ਧਾਰਾ)
      ਵਾਪਸ E_FAIL;
    ਧਾਰਾ->ਜੋੜੋ_Ref ();
    *font_File_ਧਾਰਾ = ਧਾਰਾ;
    ਵਾਪਸ S_OK;
  }

  ~DWriteFontFileLoader()
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ v : mFontStreams.values ())
      v->Release ();
  }
};

ਵਰਗ DWrite_Font_File_ਧਾਰਾ final : ਜਨਤਕ IDWrite_Font_File_ਧਾਰਾ
{
ਨਿਜੀ:
  hb_reference_ਗਿਣਤੀ_t m_Ref_ਗਿਣਤੀ;
  hb_blob_t *mBlob;
  uint8_t *m_ਅੰਕੜੇ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ m_ਮਾਪ;
  DWriteFontFileLoader *mLoader;
ਜਨਤਕ:
  uint64_t font_File_ਕੁੰਜੀ;
ਜਨਤਕ:
  DWrite_Font_File_ਧਾਰਾ (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID ਅਚਲ& iid, ਬਾਹਰਲਾ_3 ਖਾਲੀ** ppObject)
  { ਵਾਪਸ S_OK; }
  IFACEMETHOD_ (ULONG, ਜੋੜੋ_Ref) ()
  {
    ਵਾਪਸ m_Ref_ਗਿਣਤੀ.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    ਨਿਸ਼ਾਨੀਵਾਲਾ ref_ਗਿਣਤੀ = m_Ref_ਗਿਣਤੀ.dec () - 1;
    ਪ੍ਰਤਿਜ੍ਞਾ (ref_ਗਿਣਤੀ >= 0);
    ਜੇ (ref_ਗਿਣਤੀ)
      ਵਾਪਸ ref_ਗਿਣਤੀ;
    hb_directwrite_object_destroy (ਇਹ);
    ਵਾਪਸ 0;
  }

  // IDWriteFontFileStream methods
  ਆਭਾਸੀ HRESULT STDMETHODCALLTYPE
  ReadFileFragment (ਖਾਲੀ ਅਚਲ** fragment_ਸ਼ੁਰੂ_2,
		    UINT64 file_ਵਿੱਥ,
		    UINT64 fragment_ਮਾਪ,
		    ਬਾਹਰਲਾ_3 ਖਾਲੀ** fragmentContext)
  {
    // We are required to do bounds checking.
    ਜੇ (file_ਵਿੱਥ + fragment_ਮਾਪ > m_ਮਾਪ) ਵਾਪਸ E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t ਸੂਚਕਾਂਕ_2 = static_cast<size_t> (file_ਵਿੱਥ);

    // We should be alive for the duration of this.
    *fragment_ਸ਼ੁਰੂ_2 = &m_ਅੰਕੜੇ[ਸੂਚਕਾਂਕ_2];
    *fragmentContext = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ S_OK;
  }

  ਆਭਾਸੀ ਖਾਲੀ STDMETHODCALLTYPE
  ReleaseFileFragment (ਖਾਲੀ* fragmentContext) {}

  ਆਭਾਸੀ HRESULT STDMETHODCALLTYPE
  Get_File_ਮਾਪ (ਬਾਹਰਲਾ_3 UINT64* file_ਮਾਪ_2)
  {
    *file_ਮਾਪ_2 = m_ਮਾਪ;
    ਵਾਪਸ S_OK;
  }

  ਆਭਾਸੀ HRESULT STDMETHODCALLTYPE
  Get_ਆਖਰੀ_Write_ਸਮਾਂ (ਬਾਹਰਲਾ_3 UINT64* ਆਖਰੀ_Write_ਸਮਾਂ) { ਵਾਪਸ E_NOTIMPL; }

  ~DWrite_Font_File_ਧਾਰਾ();
};

ਬਣਤਰ hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    ਜੇ (unlikely (hr != S_OK))
      ਵਾਪਸ;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    ਜੇ (unlikely (!fontFileLoader))
      ਵਾਪਸ;
    ਜੇ (unlikely (FAILED (dwriteFactory->ਰਜਿਸਟਰ_Font_File_Loader (fontFileLoader))))
      ਵਾਪਸ;

    success = ਸੱਚ;
  }
  ~hb_directwrite_global_t ()
  {
    ਜੇ (fontFileLoader)
      fontFileLoader->Release ();
    ਜੇ (dwriteFactory)
      dwriteFactory->Release ();
  }

  ਤਾਰਕਿਕ success = ਝੂਠ;
  IDWriteFactory *dwriteFactory = ਖਾਲੀ_ਸੂਚਕ;
  DWriteFontFileLoader *fontFileLoader = ਖਾਲੀ_ਸੂਚਕ;
};


HB_ਅੰਦਰੂਨੀ hb_directwrite_global_t *
get_directwrite_global ();

HB_ਅੰਦਰੂਨੀ IDWriteFontFace *
dw_face_create (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2);


#ਜੇ_ਅੰਤ /* HB_DIRECTWRITE_HH */
