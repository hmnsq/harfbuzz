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


#నిర్వచితం_కాకపోతే HB_DIRECTWRITE_HH
#నిర్వచించు HB_DIRECTWRITE_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-directwrite.h"

#అన్తర్భావ "hb-mutex.hh"
#అన్తర్భావ "hb-map.hh"

/*
 * DirectWrite font stream helpers
 */

// Have a look at to NativeFontResourceDWrite.cpp in Mozilla


/* Declare object creator for dynamic support of DWRITE */
రకనిర్వచనం HRESULT (WINAPI *t_DWriteCreateFactory)(
  DWRITE_FACTORY_TYPE factoryType,
  REFIID              iid,
  IUnknown            **factory
);

మూస <రకం_పేరు Type, రకం_పేరు ...Ts>
స్థిర Type *
hb_directwrite_object_create (Ts&&... వాదనలు)
{
  Type *obj = (Type *) hb_calloc (1, పరిమాణం (Type));
  ఒకవేళ (unlikely (!obj))
    తిరిగి శూన్యసూచిక;

  తిరిగి కొత్త (obj) Type (std::forward<Ts> (వాదనలు)...);
}

మూస <రకం_పేరు Type>
స్థిర శూన్య
hb_directwrite_object_destroy (Type *obj)
{
  ఒకవేళ (!obj)
    తిరిగి;

  obj->~Type ();
  hb_free (obj);
}

తరగతి DWriteFontFileLoader final : బహిరంగ IDWriteFontFileLoader
{
అంతరంగ:
  hb_reference_లెక్క_t m_Ref_లెక్క;
  hb_mutex_t mutex;
  hb_hashmap_t<uint64_t, IDWrite_Font_File_ప్రవాహం *> mFontStreams;
  uint64_t m_తదుపరి_Font_File_కీలకం = 0;
బహిరంగ:
  DWriteFontFileLoader ()
  {
    m_Ref_లెక్క.init ();
  }

  uint64_t రిజిస్టరు_Font_File_ప్రవాహం (IDWrite_Font_File_ప్రవాహం *font_File_ప్రవాహం)
  {
    font_File_ప్రవాహం->కూడు_Ref ();
    hb_తాళం_t lock {mutex};
    mFontStreams.సమితి (m_తదుపరి_Font_File_కీలకం, font_File_ప్రవాహం);
    తిరిగి m_తదుపరి_Font_File_కీలకం++;
  }
  శూన్య Unregister_Font_File_ప్రవాహం (uint64_t font_File_కీలకం)
  {
    hb_తాళం_t lock {mutex};
    IDWrite_Font_File_ప్రవాహం *ప్రవాహం = mFontStreams.get (font_File_కీలకం);
    ఒకవేళ (ప్రవాహం)
    {
      mFontStreams.del (font_File_కీలకం);
      ప్రవాహం->Release ();
    }
  }

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID స్థిరరాశి& iid, బహిర్గతం_3 శూన్య** ppObject)
  { తిరిగి S_OK; }
  IFACEMETHOD_ (ULONG, కూడు_Ref) ()
  {
    తిరిగి m_Ref_లెక్క.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    గుర్తుగల ref_లెక్క = m_Ref_లెక్క.dec () - 1;
    ప్రతిజ్ఞా (ref_లెక్క >= 0);
    ఒకవేళ (ref_లెక్క)
      తిరిగి ref_లెక్క;
    hb_directwrite_object_destroy (ఇది);
    తిరిగి 0;
  }

  // IDWriteFontFileLoader methods
  మాయా HRESULT STDMETHODCALLTYPE
  Create_ప్రవాహం_From_కీలకం (శూన్య స్థిరరాశి* font_File_Reference_కీలకం,
		       uint32_t font_File_Reference_కీలకం_కొలత,
		       బహిర్గతం_3 IDWrite_Font_File_ప్రవాహం** font_File_ప్రవాహం)
  {
    ఒకవేళ (font_File_Reference_కీలకం_కొలత != పరిమాణం (uint64_t))
      తిరిగి E_INVALIDARG;
    uint64_t font_File_కీలకం = * (uint64_t *) font_File_Reference_కీలకం;
    IDWrite_Font_File_ప్రవాహం *ప్రవాహం = mFontStreams.get (font_File_కీలకం);
    ఒకవేళ (!ప్రవాహం)
      తిరిగి E_FAIL;
    ప్రవాహం->కూడు_Ref ();
    *font_File_ప్రవాహం = ప్రవాహం;
    తిరిగి S_OK;
  }

  ~DWriteFontFileLoader()
  {
    కొరకు (స్వయంచాలక v : mFontStreams.values ())
      v->Release ();
  }
};

తరగతి DWrite_Font_File_ప్రవాహం final : బహిరంగ IDWrite_Font_File_ప్రవాహం
{
అంతరంగ:
  hb_reference_లెక్క_t m_Ref_లెక్క;
  hb_blob_t *mBlob;
  uint8_t *m_దత్తాంశం;
  గుర్తులేని m_కొలత;
  DWriteFontFileLoader *mLoader;
బహిరంగ:
  uint64_t font_File_కీలకం;
బహిరంగ:
  DWrite_Font_File_ప్రవాహం (hb_blob_t *blob);

  // IUnknown interface
  IFACEMETHOD (QueryInterface) (IID స్థిరరాశి& iid, బహిర్గతం_3 శూన్య** ppObject)
  { తిరిగి S_OK; }
  IFACEMETHOD_ (ULONG, కూడు_Ref) ()
  {
    తిరిగి m_Ref_లెక్క.inc () + 1;
  }
  IFACEMETHOD_ (ULONG, Release) ()
  {
    గుర్తుగల ref_లెక్క = m_Ref_లెక్క.dec () - 1;
    ప్రతిజ్ఞా (ref_లెక్క >= 0);
    ఒకవేళ (ref_లెక్క)
      తిరిగి ref_లెక్క;
    hb_directwrite_object_destroy (ఇది);
    తిరిగి 0;
  }

  // IDWriteFontFileStream methods
  మాయా HRESULT STDMETHODCALLTYPE
  ReadFileFragment (శూన్య స్థిరరాశి** fragment_ప్రారంభం_2,
		    UINT64 file_స్థానభ్రంశం,
		    UINT64 fragment_కొలత,
		    బహిర్గతం_3 శూన్య** fragmentContext)
  {
    // We are required to do bounds checking.
    ఒకవేళ (file_స్థానభ్రంశం + fragment_కొలత > m_కొలత) తిరిగి E_FAIL;

    // truncate the 64 bit fileOffset to size_t sized index into mData
    size_t సూచీ_2 = static_cast<size_t> (file_స్థానభ్రంశం);

    // We should be alive for the duration of this.
    *fragment_ప్రారంభం_2 = &m_దత్తాంశం[సూచీ_2];
    *fragmentContext = శూన్యసూచిక;
    తిరిగి S_OK;
  }

  మాయా శూన్య STDMETHODCALLTYPE
  ReleaseFileFragment (శూన్య* fragmentContext) {}

  మాయా HRESULT STDMETHODCALLTYPE
  Get_File_కొలత (బహిర్గతం_3 UINT64* file_కొలత_2)
  {
    *file_కొలత_2 = m_కొలత;
    తిరిగి S_OK;
  }

  మాయా HRESULT STDMETHODCALLTYPE
  Get_చివరి_Write_సమయం (బహిర్గతం_3 UINT64* చివరి_Write_సమయం) { తిరిగి E_NOTIMPL; }

  ~DWrite_Font_File_ప్రవాహం();
};

నిర్మాణం hb_directwrite_global_t
{
  hb_directwrite_global_t ()
  {
    HRESULT hr = DWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
				      (IUnknown**) &dwriteFactory);

    ఒకవేళ (unlikely (hr != S_OK))
      తిరిగి;

    fontFileLoader = hb_directwrite_object_create<DWriteFontFileLoader> ();
    ఒకవేళ (unlikely (!fontFileLoader))
      తిరిగి;
    ఒకవేళ (unlikely (FAILED (dwriteFactory->రిజిస్టరు_Font_File_Loader (fontFileLoader))))
      తిరిగి;

    success = నిజం;
  }
  ~hb_directwrite_global_t ()
  {
    ఒకవేళ (fontFileLoader)
      fontFileLoader->Release ();
    ఒకవేళ (dwriteFactory)
      dwriteFactory->Release ();
  }

  తార్కిక success = అబద్ధం;
  IDWriteFactory *dwriteFactory = శూన్యసూచిక;
  DWriteFontFileLoader *fontFileLoader = శూన్యసూచిక;
};


HB_అంతర్గత hb_directwrite_global_t *
get_directwrite_global ();

HB_అంతర్గత IDWriteFontFace *
dw_face_create (hb_blob_t *blob, గుర్తులేని సూచీ_2);


#అయితే_ముగింపు /* HB_DIRECTWRITE_HH */
