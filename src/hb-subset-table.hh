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

#اگر_غیر_تعریف_شدہ HB_SUBSET_جدول_HH
#تعریف_کرو HB_SUBSET_جدول_HH


#شامل "hb.hh"

#شامل "hb-subset.hh"
#شامل "hb-repacker.hh"


سانچہ<قسم_نام جدول_Type>
ساکن منطقی
hb_subset_جدول_try (ثابت جدول_Type *جدول,
		      hb_سمتیہ_t<حرف>* buf,
		      hb_subset_context_t* c /*برآمد*/)
{
  c->serializer->آغاز_serialize ();
  اگر (c->serializer->اندر_error ()) واپس جھوٹ;

  منطقی needed = جدول->subset (c);
  اگر (!c->serializer->ran_برآمد_کا_گنجائش ())
  {
    c->serializer->انتہا_serialize ();
    واپس needed;
  }

  غیرعلامتی buf_سائز = buf->allocated;
  buf_سائز = buf_سائز * 2 + 16;




  DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->جدول_لیبل_2), buf_سائز);

  اگر (unlikely (buf_سائز > c->ماخذ_blob->لمبائی * 256 ||
		!buf->alloc_exact (buf_سائز)))
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->جدول_لیبل_2), buf_سائز);
    واپس needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  واپس hb_subset_جدول_try (جدول, buf, c);
}

ساکن HB_غیر_مستعمل غیرعلامتی
hb_subset_estimate_جدول_سائز (hb_subset_plan_t *plan,
				غیرعلامتی جدول_len,
				hb_لیبل_t جدول_لیبل_2)
{
  غیرعلامتی src_glyphs = plan->ماخذ->get_عدد_glyphs ();
  غیرعلامتی dst_glyphs = plan->glyphset ()->get_population ();

  غیرعلامتی bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  منطقی same_سائز = جدول_لیبل_2 == HB_لیبل('G','S','U','B') ||
		   جدول_لیبل_2 == HB_لیبل('G','P','O','S') ||
		   جدول_لیبل_2 == HB_لیبل('G','D','E','F') ||
		   جدول_لیبل_2 == HB_لیبل('n','a','m','e');

  اگر (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    اگر (جدول_لیبل_2 == HB_لیبل('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    ورنہ اگر (جدول_لیبل_2 == HB_لیبل('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  اگر (unlikely (!src_glyphs) || same_سائز)
    واپس bulk + جدول_len;

  واپس bulk + (غیرعلامتی) (جدول_len * جذر ((دوہرا) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ساکن HB_غیر_مستعمل hb_blob_t*
_hb_subset_repack (hb_لیبل_t لیبل_2, ثابت hb_serialize_context_t& c)
{
  اگر (!c.انحراف_حد_سے_تجاوز ())
    واپس c.copy_blob ();

  hb_blob_t* نتیجہ = hb_resolve_overflows (c.object_گراف (), لیبل_2);

  اگر (unlikely (!نتیجہ))
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (لیبل_2));
    واپس خالی_اشاریہ;
  }

  واپس نتیجہ;
}

سانچہ <قسم_نام T>
ساکن HB_غیر_مستعمل خودکار _hb_do_destroy (T &t, hb_ترجیح<1>) HB_RETURN (خالی, t.destroy ())

سانچہ <قسم_نام T>
ساکن HB_غیر_مستعمل خالی _hb_do_destroy (T &t, hb_ترجیح<0>) {}

سانچہ<قسم_نام جدول_Type>
ساکن منطقی
hb_subset_جدول (hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf)
{
  خودکار &&ماخذ_blob = plan->ماخذ_جدول<جدول_Type> ();
  خودکار *جدول = ماخذ_blob.get ();

  hb_لیبل_t لیبل_2 = جدول_Type::جدول_لیبل;
  hb_blob_t *blob = ماخذ_blob.get_blob();
  اگر (unlikely (!blob || !blob->ڈیٹا))
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (لیبل_2));
    _hb_do_destroy (ماخذ_blob, hb_prioritize);
    واپس جھوٹ;
  }

  غیرعلامتی buf_سائز = hb_subset_estimate_جدول_سائز (plan, blob->لمبائی, جدول_Type::جدول_لیبل);
  DEBUG_پیغام (SUBSET, خالی_اشاریہ,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (لیبل_2), buf_سائز);
  اگر (unlikely (!buf.alloc (buf_سائز)))
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (لیبل_2), buf_سائز);
    _hb_do_destroy (ماخذ_blob, hb_prioritize);
    واپس جھوٹ;
  }

  منطقی needed = جھوٹ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, لیبل_2);
    needed = hb_subset_جدول_try (جدول, &buf, &c);
  }
  _hb_do_destroy (ماخذ_blob, hb_prioritize);

  اگر (serializer.اندر_error () && !serializer.only_انحراف_حد_سے_تجاوز ())
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (لیبل_2));
    واپس جھوٹ;
  }

  اگر (!needed)
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (لیبل_2));
    واپس سچ;
  }

  منطقی نتیجہ = جھوٹ;
  hb_blob_t *dest_blob = _hb_subset_repack (لیبل_2, serializer);
  اگر (dest_blob)
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (لیبل_2), dest_blob->لمبائی);
    نتیجہ = plan->جوڑو_جدول (لیبل_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_پیغام (SUBSET, خالی_اشاریہ, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (لیبل_2), نتیجہ ? "success" : "FAILED!");
  واپس نتیجہ;
}

ساکن HB_غیر_مستعمل منطقی
hb_subset_جدول_passthrough (hb_subset_plan_t *plan, hb_لیبل_t لیبل_2)
{
  hb_blob_t *ماخذ_جدول = hb_face_reference_جدول (plan->ماخذ, لیبل_2);
  منطقی نتیجہ = plan->جوڑو_جدول (لیبل_2, ماخذ_جدول);
  hb_blob_destroy (ماخذ_جدول);
  واپس نتیجہ;
}


HB_اندرونی منطقی hb_subset_جدول_layout	(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success);
HB_اندرونی منطقی hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success);
HB_اندرونی منطقی hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success);
HB_اندرونی منطقی hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success);
HB_اندرونی منطقی hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success);


#اگر_ختم /* HB_SUBSET_TABLE_HH */
