/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_ਸਾਰਣੀ_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-repacker.hh"


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਸਾਰਣੀ_Type>
ਸਥਿਰ ਤਾਰਕਿਕ
hb_subset_ਸਾਰਣੀ_try (ਅਚਲ ਸਾਰਣੀ_Type *ਸਾਰਣੀ,
		      hb_ਸਦਿਸ਼_t<ਅੱਖਰ>* buf,
		      hb_subset_context_t* c /*ਬਾਹਰਲਾ*/)
{
  c->serializer->ਸ਼ੁਰੂ_serialize ();
  ਜੇ (c->serializer->ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;

  ਤਾਰਕਿਕ needed = ਸਾਰਣੀ->subset (c);
  ਜੇ (!c->serializer->ran_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼ ())
  {
    c->serializer->ਸਿਰਾ_serialize ();
    ਵਾਪਸ needed;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ buf_ਮਾਪ = buf->allocated;
  buf_ਮਾਪ = buf_ਮਾਪ * 2 + 16;




  DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->ਸਾਰਣੀ_ਟੈਗ_2), buf_ਮਾਪ);

  ਜੇ (unlikely (buf_ਮਾਪ > c->ਸਰੋਤ_blob->ਲੰਮਾਈ * 256 ||
		!buf->alloc_exact (buf_ਮਾਪ)))
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->ਸਾਰਣੀ_ਟੈਗ_2), buf_ਮਾਪ);
    ਵਾਪਸ needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  ਵਾਪਸ hb_subset_ਸਾਰਣੀ_try (ਸਾਰਣੀ, buf, c);
}

ਸਥਿਰ HB_ਅਣਵਰਤਿਆ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_subset_estimate_ਸਾਰਣੀ_ਮਾਪ (hb_subset_plan_t *plan,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_len,
				hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ src_glyphs = plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ dst_glyphs = plan->glyphset ()->get_population ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  ਤਾਰਕਿਕ same_ਮਾਪ = ਸਾਰਣੀ_ਟੈਗ_2 == HB_ਟੈਗ('G','S','U','B') ||
		   ਸਾਰਣੀ_ਟੈਗ_2 == HB_ਟੈਗ('G','P','O','S') ||
		   ਸਾਰਣੀ_ਟੈਗ_2 == HB_ਟੈਗ('G','D','E','F') ||
		   ਸਾਰਣੀ_ਟੈਗ_2 == HB_ਟੈਗ('n','a','m','e');

  ਜੇ (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    ਜੇ (ਸਾਰਣੀ_ਟੈਗ_2 == HB_ਟੈਗ('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਸਾਰਣੀ_ਟੈਗ_2 == HB_ਟੈਗ('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  ਜੇ (unlikely (!src_glyphs) || same_ਮਾਪ)
    ਵਾਪਸ bulk + ਸਾਰਣੀ_len;

  ਵਾਪਸ bulk + (ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਸਾਰਣੀ_len * ਵਰਗਮੂਲ ((ਦੁੱਗਣਾ) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ਸਥਿਰ HB_ਅਣਵਰਤਿਆ hb_blob_t*
_hb_subset_repack (hb_ਟੈਗ_t ਟੈਗ_2, ਅਚਲ hb_serialize_context_t& c)
{
  ਜੇ (!c.ਵਿੱਥ_ਉਛਾਲ ())
    ਵਾਪਸ c.copy_blob ();

  hb_blob_t* ਨਤੀਜਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), ਟੈਗ_2);

  ਜੇ (unlikely (!ਨਤੀਜਾ))
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (ਟੈਗ_2));
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ਅਣਵਰਤਿਆ ਸਵੈਚਾਲਿਤ _hb_do_destroy (T &t, hb_ਤਰਜੀਹ<1>) HB_RETURN (ਖਾਲੀ, t.destroy ())

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ HB_ਅਣਵਰਤਿਆ ਖਾਲੀ _hb_do_destroy (T &t, hb_ਤਰਜੀਹ<0>) {}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ਸਾਰਣੀ_Type>
ਸਥਿਰ ਤਾਰਕਿਕ
hb_subset_ਸਾਰਣੀ (hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf)
{
  ਸਵੈਚਾਲਿਤ &&ਸਰੋਤ_blob = plan->ਸਰੋਤ_ਸਾਰਣੀ<ਸਾਰਣੀ_Type> ();
  ਸਵੈਚਾਲਿਤ *ਸਾਰਣੀ = ਸਰੋਤ_blob.get ();

  hb_ਟੈਗ_t ਟੈਗ_2 = ਸਾਰਣੀ_Type::ਸਾਰਣੀ_ਟੈਗ;
  hb_blob_t *blob = ਸਰੋਤ_blob.get_blob();
  ਜੇ (unlikely (!blob || !blob->ਅੰਕੜੇ))
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (ਟੈਗ_2));
    _hb_do_destroy (ਸਰੋਤ_blob, hb_prioritize);
    ਵਾਪਸ ਝੂਠ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ buf_ਮਾਪ = hb_subset_estimate_ਸਾਰਣੀ_ਮਾਪ (plan, blob->ਲੰਮਾਈ, ਸਾਰਣੀ_Type::ਸਾਰਣੀ_ਟੈਗ);
  DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (ਟੈਗ_2), buf_ਮਾਪ);
  ਜੇ (unlikely (!buf.alloc (buf_ਮਾਪ)))
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (ਟੈਗ_2), buf_ਮਾਪ);
    _hb_do_destroy (ਸਰੋਤ_blob, hb_prioritize);
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ needed = ਝੂਠ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, ਟੈਗ_2);
    needed = hb_subset_ਸਾਰਣੀ_try (ਸਾਰਣੀ, &buf, &c);
  }
  _hb_do_destroy (ਸਰੋਤ_blob, hb_prioritize);

  ਜੇ (serializer.ਵਿੱਚ_error () && !serializer.only_ਵਿੱਥ_ਉਛਾਲ ())
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (ਟੈਗ_2));
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (!needed)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (ਟੈਗ_2));
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ ਨਤੀਜਾ = ਝੂਠ;
  hb_blob_t *dest_blob = _hb_subset_repack (ਟੈਗ_2, serializer);
  ਜੇ (dest_blob)
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (ਟੈਗ_2), dest_blob->ਲੰਮਾਈ);
    ਨਤੀਜਾ = plan->ਜੋੜੋ_ਸਾਰਣੀ (ਟੈਗ_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (ਟੈਗ_2), ਨਤੀਜਾ ? "success" : "FAILED!");
  ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ HB_ਅਣਵਰਤਿਆ ਤਾਰਕਿਕ
hb_subset_ਸਾਰਣੀ_passthrough (hb_subset_plan_t *plan, hb_ਟੈਗ_t ਟੈਗ_2)
{
  hb_blob_t *ਸਰੋਤ_ਸਾਰਣੀ = hb_face_reference_ਸਾਰਣੀ (plan->ਸਰੋਤ, ਟੈਗ_2);
  ਤਾਰਕਿਕ ਨਤੀਜਾ = plan->ਜੋੜੋ_ਸਾਰਣੀ (ਟੈਗ_2, ਸਰੋਤ_ਸਾਰਣੀ);
  hb_blob_destroy (ਸਰੋਤ_ਸਾਰਣੀ);
  ਵਾਪਸ ਨਤੀਜਾ;
}


HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_layout	(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success);
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_var		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success);
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_cff		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success);
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_color		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success);
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_other		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success);


#ਜੇ_ਅੰਤ /* HB_SUBSET_TABLE_HH */
