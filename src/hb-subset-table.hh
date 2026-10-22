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


#ضمن "hb.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-repacker.hh"


template<typename TableType>
ساكن منطقي
_hb_subset_table_try (ثابت TableType *table,
		      hb_vector_t<حرف>* buf,
		      hb_subset_context_t* c /* OUT */)
{
  c->serializer->start_serialize ();
  𐡄𐡍 (c->serializer->in_error ()) أرجع خطأ;

  منطقي needed = table->subset (c);
  𐡄𐡍 (!c->serializer->ran_out_of_room ())
  {
    c->serializer->end_serialize ();
    أرجع needed;
  }

  unsigned buf_size = buf->allocated;
  buf_size = buf_size * 2 + 16;




  DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->table_tag), buf_size);

  𐡄𐡍 (unlikely (buf_size > c->source_blob->length * 256 ||
		!buf->alloc_exact (buf_size)))
  {
    DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->table_tag), buf_size);
    أرجع needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  أرجع _hb_subset_table_try (table, buf, c);
}

ساكن HB_UNUSED unsigned
_hb_subset_estimate_table_size (hb_subset_plan_t *plan,
				unsigned table_len,
				hb_tag_t table_tag)
{
  unsigned src_glyphs = plan->source->get_num_glyphs ();
  unsigned dst_glyphs = plan->glyphset ()->get_population ();

  unsigned bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  منطقي same_size = table_tag == HB_TAG('G','S','U','B') ||
		   table_tag == HB_TAG('G','P','O','S') ||
		   table_tag == HB_TAG('G','D','E','F') ||
		   table_tag == HB_TAG('n','a','m','e');

  𐡄𐡍 (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    𐡄𐡍 (table_tag == HB_TAG('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    وإلا 𐡄𐡍 (table_tag == HB_TAG('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  𐡄𐡍 (unlikely (!src_glyphs) || same_size)
    أرجع bulk + table_len;

  أرجع bulk + (unsigned) (table_len * sqrt ((مزدوج) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
ساكن HB_UNUSED hb_blob_t*
_hb_subset_repack (hb_tag_t tag, ثابت hb_serialize_context_t& c)
{
  𐡄𐡍 (!c.offset_overflow ())
    أرجع c.copy_blob ();

  hb_blob_t* result = hb_resolve_overflows (c.object_graph (), tag);

  𐡄𐡍 (unlikely (!result))
  {
    DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (tag));
    أرجع nullptr;
  }

  أرجع result;
}

template <typename T>
ساكن HB_UNUSED auto _hb_do_destroy (T &t, hb_priority<1>) HB_RETURN (فراغ, t.destroy ())

template <typename T>
ساكن HB_UNUSED فراغ _hb_do_destroy (T &t, hb_priority<0>) {}

template<typename TableType>
ساكن منطقي
_hb_subset_table (hb_subset_plan_t *plan, hb_vector_t<حرف> &buf)
{
  auto &&source_blob = plan->source_table<TableType> ();
  auto *table = source_blob.get ();

  hb_tag_t tag = TableType::tableTag;
  hb_blob_t *blob = source_blob.get_blob();
  𐡄𐡍 (unlikely (!blob || !blob->data))
  {
    DEBUG_MSG (SUBSET, nullptr,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (tag));
    _hb_do_destroy (source_blob, hb_prioritize);
    أرجع خطأ;
  }

  unsigned buf_size = _hb_subset_estimate_table_size (plan, blob->length, TableType::tableTag);
  DEBUG_MSG (SUBSET, nullptr,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (tag), buf_size);
  𐡄𐡍 (unlikely (!buf.alloc (buf_size)))
  {
    DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (tag), buf_size);
    _hb_do_destroy (source_blob, hb_prioritize);
    أرجع خطأ;
  }

  منطقي needed = خطأ;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, tag);
    needed = _hb_subset_table_try (table, &buf, &c);
  }
  _hb_do_destroy (source_blob, hb_prioritize);

  𐡄𐡍 (serializer.in_error () && !serializer.only_offset_overflow ())
  {
    DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (tag));
    أرجع خطأ;
  }

  𐡄𐡍 (!needed)
  {
    DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (tag));
    أرجع 𐡒𐡔𐡈;
  }

  منطقي result = خطأ;
  hb_blob_t *dest_blob = _hb_subset_repack (tag, serializer);
  𐡄𐡍 (dest_blob)
  {
    DEBUG_MSG (SUBSET, nullptr,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (tag), dest_blob->length);
    result = plan->add_table (tag, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_MSG (SUBSET, nullptr, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (tag), result ? "success" : "FAILED!");
  أرجع result;
}

ساكن HB_UNUSED منطقي
_hb_subset_table_passthrough (hb_subset_plan_t *plan, hb_tag_t tag)
{
  hb_blob_t *source_table = hb_face_reference_table (plan->source, tag);
  منطقي result = plan->add_table (tag, source_table);
  hb_blob_destroy (source_table);
  أرجع result;
}


HB_INTERNAL منطقي _hb_subset_table_layout	(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success);
HB_INTERNAL منطقي _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success);
HB_INTERNAL منطقي _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success);
HB_INTERNAL منطقي _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success);
HB_INTERNAL منطقي _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<حرف> &buf, hb_tag_t tag, منطقي *success);


#endif /* HB_SUBSET_TABLE_HH */
