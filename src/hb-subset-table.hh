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

#جيڪڏهن_وصف_نه_ٿيل HB_SUBSET_جدول_HH
#وصف_ڪريو HB_SUBSET_جدول_HH


#ضمن "hb.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-repacker.hh"


سانچو<قسم_نالو جدول_Type>
ساكن منطقي
hb_subset_جدول_try (ثابت جدول_Type *جدول,
		      hb_ويڪٽر_t<حرف>* buf,
		      hb_subset_context_t* c /*ٻاهريون*/)
{
  c->serializer->شروعات_serialize ();
  إذا (c->serializer->اندريون_error ()) أرجع خطأ;

  منطقي needed = جدول->subset (c);
  إذا (!c->serializer->ran_ٻاهريون_جو_گنجائش ())
  {
    c->serializer->پڇاڙي_serialize ();
    أرجع needed;
  }

  بنانشان buf_سائيز = buf->allocated;
  buf_سائيز = buf_سائيز * 2 + 16;




  DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->جدول_ٽيگ_2), buf_سائيز);

  إذا (unlikely (buf_سائيز > c->ذريعو_blob->ڊيگهائي * 256 ||
		!buf->alloc_exact (buf_سائيز)))
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->جدول_ٽيگ_2), buf_سائيز);
    أرجع needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  أرجع hb_subset_جدول_try (جدول, buf, c);
}

ساكن HB_اڻ_استعمال_ٿيل بنانشان
hb_subset_estimate_جدول_سائيز (hb_subset_plan_t *plan,
				بنانشان جدول_len,
				hb_ٽيگ_t جدول_ٽيگ_2)
{
  بنانشان src_glyphs = plan->ذريعو->get_انگ_glyphs ();
  بنانشان dst_glyphs = plan->glyphset ()->get_population ();

  بنانشان bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  منطقي same_سائيز = جدول_ٽيگ_2 == HB_ٽيگ('G','S','U','B') ||
		   جدول_ٽيگ_2 == HB_ٽيگ('G','P','O','S') ||
		   جدول_ٽيگ_2 == HB_ٽيگ('G','D','E','F') ||
		   جدول_ٽيگ_2 == HB_ٽيگ('n','a','m','e');

  إذا (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    إذا (جدول_ٽيگ_2 == HB_ٽيگ('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    وإلا إذا (جدول_ٽيگ_2 == HB_ٽيگ('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  إذا (unlikely (!src_glyphs) || same_سائيز)
    أرجع bulk + جدول_len;

  أرجع bulk + (بنانشان) (جدول_len * جذر ((مزدوج) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ساكن HB_اڻ_استعمال_ٿيل hb_blob_t*
_hb_subset_repack (hb_ٽيگ_t ٽيگ_2, ثابت hb_serialize_context_t& c)
{
  إذا (!c.وٿي_حد_کان_مٿي ())
    أرجع c.copy_blob ();

  hb_blob_t* نتيجو = hb_resolve_overflows (c.object_گراف (), ٽيگ_2);

  إذا (unlikely (!نتيجو))
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (ٽيگ_2));
    أرجع خالي_اشارو;
  }

  أرجع نتيجو;
}

سانچو <قسم_نالو T>
ساكن HB_اڻ_استعمال_ٿيل خودڪار _hb_do_destroy (T &t, hb_ترجيح<1>) HB_RETURN (فراغ, t.destroy ())

سانچو <قسم_نالو T>
ساكن HB_اڻ_استعمال_ٿيل فراغ _hb_do_destroy (T &t, hb_ترجيح<0>) {}

سانچو<قسم_نالو جدول_Type>
ساكن منطقي
hb_subset_جدول (hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf)
{
  خودڪار &&ذريعو_blob = plan->ذريعو_جدول<جدول_Type> ();
  خودڪار *جدول = ذريعو_blob.get ();

  hb_ٽيگ_t ٽيگ_2 = جدول_Type::جدول_ٽيگ;
  hb_blob_t *blob = ذريعو_blob.get_blob();
  إذا (unlikely (!blob || !blob->معلومات))
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (ٽيگ_2));
    _hb_do_destroy (ذريعو_blob, hb_prioritize);
    أرجع خطأ;
  }

  بنانشان buf_سائيز = hb_subset_estimate_جدول_سائيز (plan, blob->ڊيگهائي, جدول_Type::جدول_ٽيگ);
  DEBUG_نياپو (SUBSET, خالي_اشارو,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (ٽيگ_2), buf_سائيز);
  إذا (unlikely (!buf.alloc (buf_سائيز)))
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (ٽيگ_2), buf_سائيز);
    _hb_do_destroy (ذريعو_blob, hb_prioritize);
    أرجع خطأ;
  }

  منطقي needed = خطأ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, ٽيگ_2);
    needed = hb_subset_جدول_try (جدول, &buf, &c);
  }
  _hb_do_destroy (ذريعو_blob, hb_prioritize);

  إذا (serializer.اندريون_error () && !serializer.only_وٿي_حد_کان_مٿي ())
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (ٽيگ_2));
    أرجع خطأ;
  }

  إذا (!needed)
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (ٽيگ_2));
    أرجع سچ;
  }

  منطقي نتيجو = خطأ;
  hb_blob_t *dest_blob = _hb_subset_repack (ٽيگ_2, serializer);
  إذا (dest_blob)
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (ٽيگ_2), dest_blob->ڊيگهائي);
    نتيجو = plan->جوڙيو_جدول (ٽيگ_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_نياپو (SUBSET, خالي_اشارو, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (ٽيگ_2), نتيجو ? "success" : "FAILED!");
  أرجع نتيجو;
}

ساكن HB_اڻ_استعمال_ٿيل منطقي
hb_subset_جدول_passthrough (hb_subset_plan_t *plan, hb_ٽيگ_t ٽيگ_2)
{
  hb_blob_t *ذريعو_جدول = hb_face_reference_جدول (plan->ذريعو, ٽيگ_2);
  منطقي نتيجو = plan->جوڙيو_جدول (ٽيگ_2, ذريعو_جدول);
  hb_blob_destroy (ذريعو_جدول);
  أرجع نتيجو;
}


HB_اندروني منطقي hb_subset_جدول_layout	(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success);
HB_اندروني منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success);
HB_اندروني منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success);
HB_اندروني منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success);
HB_اندروني منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success);


#جيڪڏهن_پورو /* HB_SUBSET_TABLE_HH */
