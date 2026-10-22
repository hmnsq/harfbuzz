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

#که_نه_تعریف_شوی HB_SUBSET_جدول_HH
#تعریف_کړه HB_SUBSET_جدول_HH


#ضمن "hb.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-repacker.hh"


کينډۍ<ډول_نوم جدول_Type>
ساكن منطقي
hb_subset_جدول_try (ثابت جدول_Type *جدول,
		      hb_ویکټور_t<حرف>* buf,
		      hb_subset_context_t* c /*بهر*/)
{
  c->serializer->پیل_serialize ();
  إذا (c->serializer->دننه_error ()) أرجع خطأ;

  منطقي needed = جدول->subset (c);
  إذا (!c->serializer->ran_بهر_د_ځای ())
  {
    c->serializer->پای_ټکی_serialize ();
    أرجع needed;
  }

  بینښه buf_اندازه = buf->allocated;
  buf_اندازه = buf_اندازه * 2 + 16;




  DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->جدول_نښان_2), buf_اندازه);

  إذا (unlikely (buf_اندازه > c->سرچینه_blob->اوږدتیا * 256 ||
		!buf->alloc_exact (buf_اندازه)))
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->جدول_نښان_2), buf_اندازه);
    أرجع needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  أرجع hb_subset_جدول_try (جدول, buf, c);
}

ساكن HB_ناکارول_شوی بینښه
hb_subset_estimate_جدول_اندازه (hb_subset_plan_t *plan,
				بینښه جدول_len,
				hb_نښان_t جدول_نښان_2)
{
  بینښه src_glyphs = plan->سرچینه->get_عدد_glyphs ();
  بینښه dst_glyphs = plan->glyphset ()->get_population ();

  بینښه bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  منطقي same_اندازه = جدول_نښان_2 == HB_نښان('G','S','U','B') ||
		   جدول_نښان_2 == HB_نښان('G','P','O','S') ||
		   جدول_نښان_2 == HB_نښان('G','D','E','F') ||
		   جدول_نښان_2 == HB_نښان('n','a','m','e');

  إذا (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    إذا (جدول_نښان_2 == HB_نښان('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    وإلا إذا (جدول_نښان_2 == HB_نښان('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  إذا (unlikely (!src_glyphs) || same_اندازه)
    أرجع bulk + جدول_len;

  أرجع bulk + (بینښه) (جدول_len * جذر ((مزدوج) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ساكن HB_ناکارول_شوی hb_blob_t*
_hb_subset_repack (hb_نښان_t نښان_2, ثابت hb_serialize_context_t& c)
{
  إذا (!c.دننه_واټن_ډکېدنه ())
    أرجع c.copy_blob ();

  hb_blob_t* پایله = hb_resolve_overflows (c.object_ګراف (), نښان_2);

  إذا (unlikely (!پایله))
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (نښان_2));
    أرجع تش_اشاره;
  }

  أرجع پایله;
}

کينډۍ <ډول_نوم T>
ساكن HB_ناکارول_شوی خپلکاری _hb_do_destroy (T &t, hb_لومړیتوب<1>) HB_RETURN (فراغ, t.destroy ())

کينډۍ <ډول_نوم T>
ساكن HB_ناکارول_شوی فراغ _hb_do_destroy (T &t, hb_لومړیتوب<0>) {}

کينډۍ<ډول_نوم جدول_Type>
ساكن منطقي
hb_subset_جدول (hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf)
{
  خپلکاری &&سرچینه_blob = plan->سرچینه_جدول<جدول_Type> ();
  خپلکاری *جدول = سرچینه_blob.get ();

  hb_نښان_t نښان_2 = جدول_Type::جدول_نښان;
  hb_blob_t *blob = سرچینه_blob.get_blob();
  إذا (unlikely (!blob || !blob->معلومات))
  {
    DEBUG_پیغام (SUBSET, تش_اشاره,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (نښان_2));
    _hb_do_destroy (سرچینه_blob, hb_prioritize);
    أرجع خطأ;
  }

  بینښه buf_اندازه = hb_subset_estimate_جدول_اندازه (plan, blob->اوږدتیا, جدول_Type::جدول_نښان);
  DEBUG_پیغام (SUBSET, تش_اشاره,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (نښان_2), buf_اندازه);
  إذا (unlikely (!buf.alloc (buf_اندازه)))
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (نښان_2), buf_اندازه);
    _hb_do_destroy (سرچینه_blob, hb_prioritize);
    أرجع خطأ;
  }

  منطقي needed = خطأ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, نښان_2);
    needed = hb_subset_جدول_try (جدول, &buf, &c);
  }
  _hb_do_destroy (سرچینه_blob, hb_prioritize);

  إذا (serializer.دننه_error () && !serializer.only_دننه_واټن_ډکېدنه ())
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (نښان_2));
    أرجع خطأ;
  }

  إذا (!needed)
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (نښان_2));
    أرجع ریښتیا;
  }

  منطقي پایله = خطأ;
  hb_blob_t *dest_blob = _hb_subset_repack (نښان_2, serializer);
  إذا (dest_blob)
  {
    DEBUG_پیغام (SUBSET, تش_اشاره,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (نښان_2), dest_blob->اوږدتیا);
    پایله = plan->جمع_کړه_جدول (نښان_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_پیغام (SUBSET, تش_اشاره, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (نښان_2), پایله ? "success" : "FAILED!");
  أرجع پایله;
}

ساكن HB_ناکارول_شوی منطقي
hb_subset_جدول_passthrough (hb_subset_plan_t *plan, hb_نښان_t نښان_2)
{
  hb_blob_t *سرچینه_جدول = hb_face_reference_جدول (plan->سرچینه, نښان_2);
  منطقي پایله = plan->جمع_کړه_جدول (نښان_2, سرچینه_جدول);
  hb_blob_destroy (سرچینه_جدول);
  أرجع پایله;
}


HB_دننی منطقي hb_subset_جدول_layout	(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success);
HB_دننی منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success);
HB_دننی منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success);
HB_دننی منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success);
HB_دننی منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success);


#د_که_پای /* HB_SUBSET_TABLE_HH */
