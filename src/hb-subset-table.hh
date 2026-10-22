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

#اگر_تعریف_نشده HB_SUBSET_جدول_HH
#تعریف_کن HB_SUBSET_جدول_HH


#ضمن "hb.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-repacker.hh"


قالب<نام_نوع جدول_Type>
ساكن منطقي
hb_subset_جدول_try (ثابت جدول_Type *جدول,
		      hb_وکتور_t<حرف>* buf,
		      hb_subset_context_t* c /*بیرون*/)
{
  c->serializer->شروع_serialize ();
  إذا (c->serializer->درون_error ()) أرجع خطأ;

  منطقي needed = جدول->subset (c);
  إذا (!c->serializer->ran_بیرون_از_جا ())
  {
    c->serializer->انتها_serialize ();
    أرجع needed;
  }

  بیعلامت buf_اندازه = buf->allocated;
  buf_اندازه = buf_اندازه * 2 + 16;




  DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->جدول_برچسب_2), buf_اندازه);

  إذا (unlikely (buf_اندازه > c->منبع_blob->درازی * 256 ||
		!buf->alloc_exact (buf_اندازه)))
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->جدول_برچسب_2), buf_اندازه);
    أرجع needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  أرجع hb_subset_جدول_try (جدول, buf, c);
}

ساكن HB_استفادهنشده بیعلامت
hb_subset_estimate_جدول_اندازه (hb_subset_plan_t *plan,
				بیعلامت جدول_len,
				hb_برچسب_t جدول_برچسب_2)
{
  بیعلامت src_glyphs = plan->منبع->get_رقم_glyphs ();
  بیعلامت dst_glyphs = plan->glyphset ()->get_population ();

  بیعلامت bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  منطقي same_اندازه = جدول_برچسب_2 == HB_برچسب('G','S','U','B') ||
		   جدول_برچسب_2 == HB_برچسب('G','P','O','S') ||
		   جدول_برچسب_2 == HB_برچسب('G','D','E','F') ||
		   جدول_برچسب_2 == HB_برچسب('n','a','m','e');

  إذا (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    إذا (جدول_برچسب_2 == HB_برچسب('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    وإلا إذا (جدول_برچسب_2 == HB_برچسب('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  إذا (unlikely (!src_glyphs) || same_اندازه)
    أرجع bulk + جدول_len;

  أرجع bulk + (بیعلامت) (جدول_len * جذر ((مزدوج) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ساكن HB_استفادهنشده hb_blob_t*
_hb_subset_repack (hb_برچسب_t برچسب_2, ثابت hb_serialize_context_t& c)
{
  إذا (!c.جابه_جایی_سرریز ())
    أرجع c.copy_blob ();

  hb_blob_t* نتیجه = hb_resolve_overflows (c.object_گراف (), برچسب_2);

  إذا (unlikely (!نتیجه))
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (برچسب_2));
    أرجع اشاره_تهی;
  }

  أرجع نتیجه;
}

قالب <نام_نوع T>
ساكن HB_استفادهنشده خودکار _hb_do_destroy (T &t, hb_اولویت<1>) HB_RETURN (فراغ, t.destroy ())

قالب <نام_نوع T>
ساكن HB_استفادهنشده فراغ _hb_do_destroy (T &t, hb_اولویت<0>) {}

قالب<نام_نوع جدول_Type>
ساكن منطقي
hb_subset_جدول (hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf)
{
  خودکار &&منبع_blob = plan->منبع_جدول<جدول_Type> ();
  خودکار *جدول = منبع_blob.get ();

  hb_برچسب_t برچسب_2 = جدول_Type::جدول_برچسب;
  hb_blob_t *blob = منبع_blob.get_blob();
  إذا (unlikely (!blob || !blob->داده))
  {
    DEBUG_پیام (SUBSET, اشاره_تهی,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (برچسب_2));
    _hb_do_destroy (منبع_blob, hb_prioritize);
    أرجع خطأ;
  }

  بیعلامت buf_اندازه = hb_subset_estimate_جدول_اندازه (plan, blob->درازی, جدول_Type::جدول_برچسب);
  DEBUG_پیام (SUBSET, اشاره_تهی,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (برچسب_2), buf_اندازه);
  إذا (unlikely (!buf.alloc (buf_اندازه)))
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (برچسب_2), buf_اندازه);
    _hb_do_destroy (منبع_blob, hb_prioritize);
    أرجع خطأ;
  }

  منطقي needed = خطأ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, برچسب_2);
    needed = hb_subset_جدول_try (جدول, &buf, &c);
  }
  _hb_do_destroy (منبع_blob, hb_prioritize);

  إذا (serializer.درون_error () && !serializer.only_جابه_جایی_سرریز ())
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (برچسب_2));
    أرجع خطأ;
  }

  إذا (!needed)
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (برچسب_2));
    أرجع درست;
  }

  منطقي نتیجه = خطأ;
  hb_blob_t *dest_blob = _hb_subset_repack (برچسب_2, serializer);
  إذا (dest_blob)
  {
    DEBUG_پیام (SUBSET, اشاره_تهی,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (برچسب_2), dest_blob->درازی);
    نتیجه = plan->جمع_کن_جدول (برچسب_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_پیام (SUBSET, اشاره_تهی, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (برچسب_2), نتیجه ? "success" : "FAILED!");
  أرجع نتیجه;
}

ساكن HB_استفادهنشده منطقي
hb_subset_جدول_passthrough (hb_subset_plan_t *plan, hb_برچسب_t برچسب_2)
{
  hb_blob_t *منبع_جدول = hb_face_reference_جدول (plan->منبع, برچسب_2);
  منطقي نتیجه = plan->جمع_کن_جدول (برچسب_2, منبع_جدول);
  hb_blob_destroy (منبع_جدول);
  أرجع نتیجه;
}


HB_داخلی منطقي hb_subset_جدول_layout	(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success);
HB_داخلی منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success);
HB_داخلی منطقي hb_subset_جدول_cff		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success);
HB_داخلی منطقي hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success);
HB_داخلی منطقي hb_subset_جدول_other		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success);


#پایان_اگر /* HB_SUBSET_TABLE_HH */
