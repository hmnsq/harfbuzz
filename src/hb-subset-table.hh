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

#ifndef HB_SUBSET_TABLE_HH
#define HB_SUBSET_TABLE_HH


#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-subset.hh"
#ଅନ୍ତର୍ଭାଵ "hb-repacker.hh"


template<typename TableType>
ସ୍ଥିର ତାର୍କିକ
_hb_subset_table_try (ଅଚଳ TableType *table,
		      hb_vector_t<ଅକ୍ଷର>* buf,
		      hb_subset_context_t* c /* OUT */)
{
  c->serializer->start_serialize ();
  ଯଦି (c->serializer->in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ତାର୍କିକ needed = table->subset (c);
  ଯଦି (!c->serializer->ran_out_of_room ())
  {
    c->serializer->end_serialize ();
    ଫେରନ୍ତୁ needed;
  }

  unsigned buf_size = buf->allocated;
  buf_size = buf_size * 2 + 16;




  DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->table_tag), buf_size);

  ଯଦି (unlikely (buf_size > c->source_blob->length * 256 ||
		!buf->alloc_exact (buf_size)))
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->table_tag), buf_size);
    ଫେରନ୍ତୁ needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  ଫେରନ୍ତୁ _hb_subset_table_try (table, buf, c);
}

ସ୍ଥିର HB_UNUSED unsigned
_hb_subset_estimate_table_size (hb_subset_plan_t *plan,
				unsigned table_len,
				hb_tag_t table_tag)
{
  unsigned src_glyphs = plan->source->get_ଅଙ୍କ_glyphs ();
  unsigned dst_glyphs = plan->glyphset ()->get_population ();

  unsigned bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  ତାର୍କିକ same_size = table_tag == HB_TAG('G','S','U','B') ||
		   table_tag == HB_TAG('G','P','O','S') ||
		   table_tag == HB_TAG('G','D','E','F') ||
		   table_tag == HB_TAG('n','a','m','e');

  ଯଦି (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    ଯଦି (table_tag == HB_TAG('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    ଅନ୍ୟଥା ଯଦି (table_tag == HB_TAG('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  ଯଦି (unlikely (!src_glyphs) || same_size)
    ଫେରନ୍ତୁ bulk + table_len;

  ଫେରନ୍ତୁ bulk + (unsigned) (table_len * sqrt ((ଦ୍ୱିଗୁଣ) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ସ୍ଥିର HB_UNUSED hb_blob_t*
_hb_subset_repack (hb_tag_t tag, ଅଚଳ hb_serialize_context_t& c)
{
  ଯଦି (!c.offset_overflow ())
    ଫେରନ୍ତୁ c.copy_blob ();

  hb_blob_t* result = hb_resolve_overflows (c.object_graph (), tag);

  ଯଦି (unlikely (!result))
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (tag));
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ result;
}

template <typename T>
ସ୍ଥିର HB_UNUSED auto _hb_do_destroy (T &t, hb_priority<1>) HB_RETURN (ଶୂନ୍ୟ, t.destroy ())

template <typename T>
ସ୍ଥିର HB_UNUSED ଶୂନ୍ୟ _hb_do_destroy (T &t, hb_priority<0>) {}

template<typename TableType>
ସ୍ଥିର ତାର୍କିକ
_hb_subset_table (hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf)
{
  auto &&source_blob = plan->source_table<TableType> ();
  auto *table = source_blob.get ();

  hb_tag_t tag = TableType::tableTag;
  hb_blob_t *blob = source_blob.get_blob();
  ଯଦି (unlikely (!blob || !blob->data))
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (tag));
    _hb_do_destroy (source_blob, hb_prioritize);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  unsigned buf_size = _hb_subset_estimate_table_size (plan, blob->length, TableType::tableTag);
  DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (tag), buf_size);
  ଯଦି (unlikely (!buf.alloc (buf_size)))
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (tag), buf_size);
    _hb_do_destroy (source_blob, hb_prioritize);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ needed = ମିଥ୍ୟା;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, tag);
    needed = _hb_subset_table_try (table, &buf, &c);
  }
  _hb_do_destroy (source_blob, hb_prioritize);

  ଯଦି (serializer.in_error () && !serializer.only_offset_overflow ())
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (tag));
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଯଦି (!needed)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (tag));
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ result = ମିଥ୍ୟା;
  hb_blob_t *dest_blob = _hb_subset_repack (tag, serializer);
  ଯଦି (dest_blob)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (tag), dest_blob->length);
    result = plan->add_table (tag, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (tag), result ? "success" : "FAILED!");
  ଫେରନ୍ତୁ result;
}

ସ୍ଥିର HB_UNUSED ତାର୍କିକ
_hb_subset_table_passthrough (hb_subset_plan_t *plan, hb_tag_t tag)
{
  hb_blob_t *source_table = hb_face_reference_table (plan->source, tag);
  ତାର୍କିକ result = plan->add_table (tag, source_table);
  hb_blob_destroy (source_table);
  ଫେରନ୍ତୁ result;
}


HB_INTERNAL ତାର୍କିକ _hb_subset_table_layout	(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success);
HB_INTERNAL ତାର୍କିକ _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success);
HB_INTERNAL ତାର୍କିକ _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success);
HB_INTERNAL ତାର୍କିକ _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success);
HB_INTERNAL ତାର୍କିକ _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<ଅକ୍ଷର> &buf, hb_tag_t tag, ତାର୍କିକ *success);


#endif /* HB_SUBSET_TABLE_HH */
