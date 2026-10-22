/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
 *
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_DUAL_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_DUAL_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {

ਬਣਤਰ SmallTypes {
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = 2;
  ਵਰਤੋਂ large_int = uint32_t;
  ਵਰਤੋਂ HBUINT = HBUINT16;
  ਵਰਤੋਂ HBLUINT = HBUINT16;
  ਵਰਤੋਂ HBGlyphID = HBGlyphID16;
  ਵਰਤੋਂ ਵਿੱਥ_2 = Offset16;
  ਵਰਤੋਂ LOffset = Offset16;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
  ਵਰਤੋਂ ਵਿੱਥ_ਮੰਜ਼ਿਲ = OT::Offset16_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, has_null>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
  ਵਰਤੋਂ LOffset_ਮੰਜ਼ਿਲ = OT::Offset16_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, has_null>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  ਵਰਤੋਂ Array_ਦਾ = OT::Array16_ਦਾ<Type>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  ਵਰਤੋਂ Sorted_Array_ਦਾ = OT::Sorted_Array16_ਦਾ<Type>;
};

ਬਣਤਰ MediumTypes {
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = 3;
  ਵਰਤੋਂ large_int = uint64_t;
  ਵਰਤੋਂ HBUINT = HBUINT24;
  ਵਰਤੋਂ HBLUINT = HBUINT32;
  ਵਰਤੋਂ HBGlyphID = HBGlyphID24;
  ਵਰਤੋਂ ਵਿੱਥ_2 = Offset24;
  /* Long offsets for large tables. */
  ਵਰਤੋਂ LOffset = Offset32;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
  ਵਰਤੋਂ ਵਿੱਥ_ਮੰਜ਼ਿਲ = OT::Offset24_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, has_null>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ ਅਧਾਰ_Type=ਖਾਲੀ, ਤਾਰਕਿਕ has_null=ਸੱਚ>
  ਵਰਤੋਂ LOffset_ਮੰਜ਼ਿਲ = OT::Offset32_ਮੰਜ਼ਿਲ<Type, ਅਧਾਰ_Type, has_null>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  ਵਰਤੋਂ Array_ਦਾ = OT::Array24_ਦਾ<Type>;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  ਵਰਤੋਂ Sorted_Array_ਦਾ = OT::Sorted_Array24_ਦਾ<Type>;
};

}
}


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਹੇਠਲਾ_2, ਕਿਸਮ_ਨਾਂ ਉੱਤਲਾ_2>
ਬਣਤਰ hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਉੱਤਲਾ (face),
#ਜੇ_ਅੰਤ
    ਹੇਠਲਾ (
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
	   ਉੱਤਲਾ.has_ਅੰਕੜੇ () ? hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ () :
#ਜੇ_ਅੰਤ
	   face)
  {}

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (ਉੱਤਲਾ.has_ਅੰਕੜੇ ()) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਹੇਠਲਾ.has_ਅੰਕੜੇ ();
  }

  ਤਾਰਕਿਕ has_ਉੱਤਲਾ_ਅੰਕੜੇ () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਵਾਪਸ ਉੱਤਲਾ.has_ਅੰਕੜੇ ();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਉੱਤਲਾ_2 ਉੱਤਲਾ;
#ਜੇ_ਅੰਤ
  ਹੇਠਲਾ_2 ਹੇਠਲਾ;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸਾਰਣੀ_3>
ਬਣਤਰ hb_ਸਾਰਣੀ_accelerator_t
{
  hb_ਸਾਰਣੀ_accelerator_t (hb_face_t *face) :
    ਸਾਰਣੀ (hb_sanitize_context_t ().reference_ਸਾਰਣੀ<ਸਾਰਣੀ_3> (face))
  {}
  ~hb_ਸਾਰਣੀ_accelerator_t () { ਸਾਰਣੀ.destroy (); }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ->has_ਅੰਕੜੇ (); }

  hb_blob_ptr_t<ਸਾਰਣੀ_3> ਸਾਰਣੀ;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਹੇਠਲਾ_2, ਕਿਸਮ_ਨਾਂ ਉੱਤਲਾ_2>
ਬਣਤਰ hb_dual_ਸਾਰਣੀ_t : hb_dual_accelerator_t<hb_ਸਾਰਣੀ_accelerator_t<ਹੇਠਲਾ_2>,
					       hb_ਸਾਰਣੀ_accelerator_t<ਉੱਤਲਾ_2>>
{
  hb_dual_ਸਾਰਣੀ_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_ਸਾਰਣੀ_accelerator_t<ਹੇਠਲਾ_2>,
			  hb_ਸਾਰਣੀ_accelerator_t<ਉੱਤਲਾ_2>> (face) {}
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DUAL_GET(accel, ਵਿਅੰਜਕ) ((accel).ਉੱਤਲਾ.has_ਅੰਕੜੇ () ? (accel).ਉੱਤਲਾ.ਵਿਅੰਜਕ : (accel).ਹੇਠਲਾ.ਵਿਅੰਜਕ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DUAL_GET(accel, ਵਿਅੰਜਕ) ((accel).ਹੇਠਲਾ.ਵਿਅੰਜਕ)
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ  /* HB_OT_DUAL_HH */
