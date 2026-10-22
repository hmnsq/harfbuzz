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

#నిర్వచితం_కాకపోతే HB_SUBSET_పట్టిక_HH
#నిర్వచించు HB_SUBSET_పట్టిక_HH


#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-subset.hh"
#అన్తర్భావ "hb-repacker.hh"


మూస<రకం_పేరు పట్టిక_Type>
స్థిర తార్కిక
hb_subset_పట్టిక_try (స్థిరరాశి పట్టిక_Type *పట్టిక,
		      hb_సదిశ_t<అక్షరం>* buf,
		      hb_subset_context_t* c /*బహిర్గతం*/)
{
  c->serializer->ప్రారంభం_serialize ();
  ఒకవేళ (c->serializer->లోపల_error ()) తిరిగి అబద్ధం;

  తార్కిక needed = పట్టిక->subset (c);
  ఒకవేళ (!c->serializer->ran_బహిర్గతం_యొక్క_స్థలం ())
  {
    c->serializer->చివర_serialize ();
    తిరిగి needed;
  }

  గుర్తులేని buf_కొలత = buf->allocated;
  buf_కొలత = buf_కొలత * 2 + 16;




  DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->పట్టిక_ట్యాగ్_2), buf_కొలత);

  ఒకవేళ (unlikely (buf_కొలత > c->ఆధారం_blob->నిడివి * 256 ||
		!buf->alloc_exact (buf_కొలత)))
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->పట్టిక_ట్యాగ్_2), buf_కొలత);
    తిరిగి needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  తిరిగి hb_subset_పట్టిక_try (పట్టిక, buf, c);
}

స్థిర HB_వాడని గుర్తులేని
hb_subset_estimate_పట్టిక_కొలత (hb_subset_plan_t *plan,
				గుర్తులేని పట్టిక_len,
				hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2)
{
  గుర్తులేని src_glyphs = plan->ఆధారం->get_అంకె_glyphs ();
  గుర్తులేని dst_glyphs = plan->glyphset ()->get_population ();

  గుర్తులేని bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  తార్కిక same_కొలత = పట్టిక_ట్యాగ్_2 == HB_ట్యాగ్('G','S','U','B') ||
		   పట్టిక_ట్యాగ్_2 == HB_ట్యాగ్('G','P','O','S') ||
		   పట్టిక_ట్యాగ్_2 == HB_ట్యాగ్('G','D','E','F') ||
		   పట్టిక_ట్యాగ్_2 == HB_ట్యాగ్('n','a','m','e');

  ఒకవేళ (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    ఒకవేళ (పట్టిక_ట్యాగ్_2 == HB_ట్యాగ్('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    లేకపోతే ఒకవేళ (పట్టిక_ట్యాగ్_2 == HB_ట్యాగ్('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  ఒకవేళ (unlikely (!src_glyphs) || same_కొలత)
    తిరిగి bulk + పట్టిక_len;

  తిరిగి bulk + (గుర్తులేని) (పట్టిక_len * వర్గమూలం ((ద్విగుణ) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
స్థిర HB_వాడని hb_blob_t*
_hb_subset_repack (hb_ట్యాగ్_t ట్యాగ్_2, స్థిరరాశి hb_serialize_context_t& c)
{
  ఒకవేళ (!c.స్థానభ్రంశం_అధికప్రవాహం ())
    తిరిగి c.copy_blob ();

  hb_blob_t* ఫలితం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), ట్యాగ్_2);

  ఒకవేళ (unlikely (!ఫలితం))
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (ట్యాగ్_2));
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి ఫలితం;
}

మూస <రకం_పేరు T>
స్థిర HB_వాడని స్వయంచాలక _hb_do_destroy (T &t, hb_ప్రాధాన్యత<1>) HB_RETURN (శూన్య, t.destroy ())

మూస <రకం_పేరు T>
స్థిర HB_వాడని శూన్య _hb_do_destroy (T &t, hb_ప్రాధాన్యత<0>) {}

మూస<రకం_పేరు పట్టిక_Type>
స్థిర తార్కిక
hb_subset_పట్టిక (hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf)
{
  స్వయంచాలక &&ఆధారం_blob = plan->ఆధారం_పట్టిక<పట్టిక_Type> ();
  స్వయంచాలక *పట్టిక = ఆధారం_blob.get ();

  hb_ట్యాగ్_t ట్యాగ్_2 = పట్టిక_Type::పట్టిక_ట్యాగ్;
  hb_blob_t *blob = ఆధారం_blob.get_blob();
  ఒకవేళ (unlikely (!blob || !blob->దత్తాంశం))
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (ట్యాగ్_2));
    _hb_do_destroy (ఆధారం_blob, hb_prioritize);
    తిరిగి అబద్ధం;
  }

  గుర్తులేని buf_కొలత = hb_subset_estimate_పట్టిక_కొలత (plan, blob->నిడివి, పట్టిక_Type::పట్టిక_ట్యాగ్);
  DEBUG_సందేశం (SUBSET, శూన్యసూచిక,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (ట్యాగ్_2), buf_కొలత);
  ఒకవేళ (unlikely (!buf.alloc (buf_కొలత)))
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (ట్యాగ్_2), buf_కొలత);
    _hb_do_destroy (ఆధారం_blob, hb_prioritize);
    తిరిగి అబద్ధం;
  }

  తార్కిక needed = అబద్ధం;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, ట్యాగ్_2);
    needed = hb_subset_పట్టిక_try (పట్టిక, &buf, &c);
  }
  _hb_do_destroy (ఆధారం_blob, hb_prioritize);

  ఒకవేళ (serializer.లోపల_error () && !serializer.only_స్థానభ్రంశం_అధికప్రవాహం ())
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (ట్యాగ్_2));
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (!needed)
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (ట్యాగ్_2));
    తిరిగి నిజం;
  }

  తార్కిక ఫలితం = అబద్ధం;
  hb_blob_t *dest_blob = _hb_subset_repack (ట్యాగ్_2, serializer);
  ఒకవేళ (dest_blob)
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (ట్యాగ్_2), dest_blob->నిడివి);
    ఫలితం = plan->కూడు_పట్టిక (ట్యాగ్_2, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (ట్యాగ్_2), ఫలితం ? "success" : "FAILED!");
  తిరిగి ఫలితం;
}

స్థిర HB_వాడని తార్కిక
hb_subset_పట్టిక_passthrough (hb_subset_plan_t *plan, hb_ట్యాగ్_t ట్యాగ్_2)
{
  hb_blob_t *ఆధారం_పట్టిక = hb_face_reference_పట్టిక (plan->ఆధారం, ట్యాగ్_2);
  తార్కిక ఫలితం = plan->కూడు_పట్టిక (ట్యాగ్_2, ఆధారం_పట్టిక);
  hb_blob_destroy (ఆధారం_పట్టిక);
  తిరిగి ఫలితం;
}


HB_అంతర్గత తార్కిక hb_subset_పట్టిక_layout	(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success);
HB_అంతర్గత తార్కిక hb_subset_పట్టిక_var		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success);
HB_అంతర్గత తార్కిక hb_subset_పట్టిక_cff		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success);
HB_అంతర్గత తార్కిక hb_subset_పట్టిక_color		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success);
HB_అంతర్గత తార్కిక hb_subset_పట్టిక_other		(hb_subset_plan_t *plan, hb_సదిశ_t<అక్షరం> &buf, hb_ట్యాగ్_t ట్యాగ్_2, తార్కిక *success);


#అయితే_ముగింపు /* HB_SUBSET_TABLE_HH */
