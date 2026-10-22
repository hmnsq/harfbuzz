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


#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-subset.hh"
#অন্তর্ভাব "hb-repacker.hh"


template<typename TableType>
স্থির তার্কিক
_hb_subset_table_try (ধ্রুব TableType *table,
		      hb_vector_t<বর্ণ>* buf,
		      hb_subset_context_t* c /* OUT */)
{
  c->serializer->start_serialize ();
  যদি (c->serializer->in_error ()) প্রত্যাগম অসত্য;

  তার্কিক needed = table->subset (c);
  যদি (!c->serializer->ran_out_of_room ())
  {
    c->serializer->end_serialize ();
    প্রত্যাগম needed;
  }

  unsigned buf_size = buf->allocated;
  buf_size = buf_size * 2 + 16;




  DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->table_tag), buf_size);

  যদি (unlikely (buf_size > c->source_blob->length * 256 ||
		!buf->alloc_exact (buf_size)))
  {
    DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->table_tag), buf_size);
    প্রত্যাগম needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  প্রত্যাগম _hb_subset_table_try (table, buf, c);
}

স্থির HB_UNUSED unsigned
_hb_subset_estimate_table_size (hb_subset_plan_t *plan,
				unsigned table_len,
				hb_tag_t table_tag)
{
  unsigned src_glyphs = plan->source->get_অঙ্ক_glyphs ();
  unsigned dst_glyphs = plan->glyphset ()->get_population ();

  unsigned bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  তার্কিক same_size = table_tag == HB_TAG('G','S','U','B') ||
		   table_tag == HB_TAG('G','P','O','S') ||
		   table_tag == HB_TAG('G','D','E','F') ||
		   table_tag == HB_TAG('n','a','m','e');

  যদি (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    যদি (table_tag == HB_TAG('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    অন্যথা যদি (table_tag == HB_TAG('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  যদি (unlikely (!src_glyphs) || same_size)
    প্রত্যাগম bulk + table_len;

  প্রত্যাগম bulk + (unsigned) (table_len * sqrt ((দ্বিগুণ) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
স্থির HB_UNUSED hb_blob_t*
_hb_subset_repack (hb_tag_t tag, ধ্রুব hb_serialize_context_t& c)
{
  যদি (!c.offset_overflow ())
    প্রত্যাগম c.copy_blob ();

  hb_blob_t* result = hb_resolve_overflows (c.object_graph (), tag);

  যদি (unlikely (!result))
  {
    DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (tag));
    প্রত্যাগম nullptr;
  }

  প্রত্যাগম result;
}

template <typename T>
স্থির HB_UNUSED auto _hb_do_destroy (T &t, hb_priority<1>) HB_RETURN (শূন্য, t.destroy ())

template <typename T>
স্থির HB_UNUSED শূন্য _hb_do_destroy (T &t, hb_priority<0>) {}

template<typename TableType>
স্থির তার্কিক
_hb_subset_table (hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf)
{
  auto &&source_blob = plan->source_table<TableType> ();
  auto *table = source_blob.get ();

  hb_tag_t tag = TableType::tableTag;
  hb_blob_t *blob = source_blob.get_blob();
  যদি (unlikely (!blob || !blob->data))
  {
    DEBUG_বার্তা (SUBSET, nullptr,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (tag));
    _hb_do_destroy (source_blob, hb_prioritize);
    প্রত্যাগম অসত্য;
  }

  unsigned buf_size = _hb_subset_estimate_table_size (plan, blob->length, TableType::tableTag);
  DEBUG_বার্তা (SUBSET, nullptr,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (tag), buf_size);
  যদি (unlikely (!buf.alloc (buf_size)))
  {
    DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (tag), buf_size);
    _hb_do_destroy (source_blob, hb_prioritize);
    প্রত্যাগম অসত্য;
  }

  তার্কিক needed = অসত্য;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, tag);
    needed = _hb_subset_table_try (table, &buf, &c);
  }
  _hb_do_destroy (source_blob, hb_prioritize);

  যদি (serializer.in_error () && !serializer.only_offset_overflow ())
  {
    DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (tag));
    প্রত্যাগম অসত্য;
  }

  যদি (!needed)
  {
    DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (tag));
    প্রত্যাগম সত্য;
  }

  তার্কিক result = অসত্য;
  hb_blob_t *dest_blob = _hb_subset_repack (tag, serializer);
  যদি (dest_blob)
  {
    DEBUG_বার্তা (SUBSET, nullptr,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (tag), dest_blob->length);
    result = plan->add_table (tag, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_বার্তা (SUBSET, nullptr, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (tag), result ? "success" : "FAILED!");
  প্রত্যাগম result;
}

স্থির HB_UNUSED তার্কিক
_hb_subset_table_passthrough (hb_subset_plan_t *plan, hb_tag_t tag)
{
  hb_blob_t *source_table = hb_face_reference_table (plan->source, tag);
  তার্কিক result = plan->add_table (tag, source_table);
  hb_blob_destroy (source_table);
  প্রত্যাগম result;
}


HB_INTERNAL তার্কিক _hb_subset_table_layout	(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success);
HB_INTERNAL তার্কিক _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success);
HB_INTERNAL তার্কিক _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success);
HB_INTERNAL তার্কিক _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success);
HB_INTERNAL তার্কিক _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success);


#endif /* HB_SUBSET_TABLE_HH */
