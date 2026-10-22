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

#جے_بنا_تعریف HB_SUBSET_جدول_HH
#تعریف_کرو HB_SUBSET_جدول_HH


#ضمن "hb.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-repacker.hh"


سانچا<قسم_ناں جدول_Type>
ساكن منطقي
hb_subset_جدول_try (ثابت جدول_Type *جدول,
		      hb_سمتیہ_t<حرف>* buf,
		      hb_subset_context_t* c /*باہرلا*/)
{
  c->serializer->شروع_serialize ();
  إذا (c->serializer->وچ_error ()) أرجع خطأ;

  منطقي needed = جدول->subset (c);
  إذا (!c->serializer->ran_باہرلا_دا_گنجائش ())
  {
    c->serializer->سرا_serialize ();
    أرجع needed;
  }

  بنانشانی buf_ناپ = buf->allocated;
  buf_ناپ = buf_ناپ * 2 + 16;




  DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->جدول_ٹیگ_2), buf_ناپ);

  إذا (unlikely (buf_ناپ > c->سروت_blob->لمائی * 256 ||
		!buf->alloc_exact (buf_ناپ)))
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->جدول_ٹیگ_2), buf_ناپ);
    أرجع needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  أرجع hb_subset_جدول_try (جدول, buf, c);
}

ساكن HB_بن_ورتیا بنانشانی
hb_subset_estimate_جدول_ناپ (hb_subset_plan_t *plan,
				بنانشانی جدول_len,
				hb_ٹیگ_t جدول_ٹیگ_2)
{
  بنانشانی src_glyphs = plan->سروت->get_عدد_glyphs ();
  بنانشانی dst_glyphs = plan->glyphset ()->get_population ();

  بنانشانی bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  منطقي same_ناپ = جدول_ٹیگ_2 == HB_ٹیگ('G','S','U','B') ||
		   جدول_ٹیگ_2 == HB_ٹیگ('G','P','O','S') ||
		   جدول_ٹیگ_2 == HB_ٹیگ('G','D','E','F') ||
		   جدول_ٹیگ_2 == HB_ٹیگ('n','a','m','e');

  إذا (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    إذا (جدول_ٹیگ_2 == HB_ٹیگ('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    وإلا إذا (جدول_ٹیگ_2 == HB_ٹیگ('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  إذا (unlikely (!src_glyphs) || same_ناپ)
    أرجع bulk + جدول_len;

  أرجع bulk + (بنانشانی) (جدول_len * جذر ((مزدوج) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ساكن HB_بن_ورتیا hb_blob_t*
_hb_subset_repack (hb_ٹیگ_t ٹیگ_2, ثابت hb_serialize_context_t& c)
{
  إذا (!c.وتھ_حد_توں_ودھ ())
    أرجع c.copy_blob ();

  hb_blob_t* نتیجہ = hb_resolve_overflows (c.object_گراف (), ٹیگ_2);

  إذا (unlikely (!نتیجہ))
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (ٹیگ_2));
    أرجع خالی_سوچک;
  }

  أرجع نتیجہ;
}

سانچا <قسم_ناں T>
ساكن HB_بن_ورتیا خودکار _hb_do_destroy (T &t, hb_ترجیح<1>) HB_RETURN (فراغ, t.destroy ())

سانچا <قسم_ناں T>
ساكن HB_بن_ورتیا فراغ _hb_do_destroy (T &t, hb_ترجیح<0>) {}

سانچا<قسم_ناں جدول_Type>
ساكن منطقي
hb_subset_جدول (hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf)
{
  خودکار &&سروت_blob = plan->سروت_جدول<جدول_Type> ();
  خودکار *جدول = سروت_blob.get ();

  hb_ٹیگ_t ٹیگ_2 = جدول_Type::جدول_ٹیگ;
  hb_blob_t *blob = سروت_blob.get_blob();
  إذا (unlikely (!blob || !blob->انکڑے))
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (ٹیگ_2));
    _hb_do_destroy (سروت_blob, hb_prioritize);
    أرجع خطأ;
  }

  بنانشانی buf_ناپ = hb_subset_estimate_جدول_ناپ (plan, blob->لمائی, جدول_Type::جدول_ٹیگ);
  DEBUG_پیغام (SUBSET, خالی_سوچک,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (ٹیگ_2), buf_ناپ);
  إذا (unlikely (!buf.alloc (buf_ناپ)))
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (ٹیگ_2), buf_ناپ);
    _hb_do_destroy (سروت_blob, hb_prioritize);
    أرجع خطأ;
  }

  منطقي needed = خطأ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, ٹیگ_2);
    needed = hb_subset_جدول_try (جدول, &buf, &c);
  }
  _hb_do_destroy (سروت_blob, hb_prioritize);

  إذا (serializer.وچ_error () && !serializer.only_وتھ_حد_توں_ودھ ())
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (ٹیگ_2));
    أرجع خطأ;
  }

  إذا (!needed)
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (ٹیگ_2));
    أرجع سچ;
  }

  منطقي نتیجہ = خطأ;
  hb_blob_t *dest_blob = _hb_subset_repack (ٹیگ_2, serializer);
  إذا (dest_blob)
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (ٹیگ_2), dest_blob->لمائی);
    نتیجہ = plan->جوڑو_جدول (ٹیگ_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_پیغام (SUBSET, خالی_سوچک, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (ٹیگ_2), نتیجہ ? "success" : "FAILED!");
  أرجع نتیجہ;
}

ساكن HB_بن_ورتیا منطقي
hb_subset_جدول_passthrough (hb_subset_plan_t *plan, hb_ٹیگ_t ٹیگ_2)
{
  hb_blob_t *سروت_جدول = hb_face_reference_جدول (plan->سروت, ٹیگ_2);
  منطقي نتیجہ = plan->جوڑو_جدول (ٹیگ_2, سروت_جدول);
  hb_blob_destroy (سروت_جدول);
  أرجع نتیجہ;
}


HB_اندرونی منطقي hb_subset_جدول_layout	(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success);
HB_اندرونی منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success);
HB_اندرونی منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success);
HB_اندرونی منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success);
HB_اندرونی منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success);


#جے_انت /* HB_SUBSET_TABLE_HH */
