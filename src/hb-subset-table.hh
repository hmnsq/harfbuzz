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


#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-subset.hh"
#अन्तर्भाव "hb-repacker.hh"


template<typename TableType>
स्थिर तार्किक
_hb_subset_table_try (ध्रुव TableType *table,
		      hb_vector_t<वर्ण>* buf,
		      hb_subset_context_t* c /* OUT */)
{
  c->serializer->start_serialize ();
  यदि (c->serializer->in_error ()) प्रत्यागम असत्य;

  तार्किक needed = table->subset (c);
  यदि (!c->serializer->ran_out_of_room ())
  {
    c->serializer->end_serialize ();
    प्रत्यागम needed;
  }

  unsigned buf_size = buf->allocated;
  buf_size = buf_size * 2 + 16;




  DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->table_tag), buf_size);

  यदि (unlikely (buf_size > c->source_blob->length * 256 ||
		!buf->alloc_exact (buf_size)))
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->table_tag), buf_size);
    प्रत्यागम needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  प्रत्यागम _hb_subset_table_try (table, buf, c);
}

स्थिर HB_UNUSED unsigned
_hb_subset_estimate_table_size (hb_subset_plan_t *plan,
				unsigned table_len,
				hb_tag_t table_tag)
{
  unsigned src_glyphs = plan->source->get_अंक_glyphs ();
  unsigned dst_glyphs = plan->glyphset ()->get_population ();

  unsigned bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  तार्किक same_size = table_tag == HB_TAG('G','S','U','B') ||
		   table_tag == HB_TAG('G','P','O','S') ||
		   table_tag == HB_TAG('G','D','E','F') ||
		   table_tag == HB_TAG('n','a','m','e');

  यदि (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    यदि (table_tag == HB_TAG('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    अन्यथा यदि (table_tag == HB_TAG('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  यदि (unlikely (!src_glyphs) || same_size)
    प्रत्यागम bulk + table_len;

  प्रत्यागम bulk + (unsigned) (table_len * sqrt ((द्विगुण) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
स्थिर HB_UNUSED hb_blob_t*
_hb_subset_repack (hb_tag_t tag, ध्रुव hb_serialize_context_t& c)
{
  यदि (!c.offset_overflow ())
    प्रत्यागम c.copy_blob ();

  hb_blob_t* result = hb_resolve_overflows (c.object_graph (), tag);

  यदि (unlikely (!result))
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (tag));
    प्रत्यागम nullptr;
  }

  प्रत्यागम result;
}

template <typename T>
स्थिर HB_UNUSED auto _hb_do_destroy (T &t, hb_priority<1>) HB_RETURN (शून्य, t.destroy ())

template <typename T>
स्थिर HB_UNUSED शून्य _hb_do_destroy (T &t, hb_priority<0>) {}

template<typename TableType>
स्थिर तार्किक
_hb_subset_table (hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf)
{
  auto &&source_blob = plan->source_table<TableType> ();
  auto *table = source_blob.get ();

  hb_tag_t tag = TableType::tableTag;
  hb_blob_t *blob = source_blob.get_blob();
  यदि (unlikely (!blob || !blob->data))
  {
    DEBUG_संदेश (SUBSET, nullptr,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (tag));
    _hb_do_destroy (source_blob, hb_prioritize);
    प्रत्यागम असत्य;
  }

  unsigned buf_size = _hb_subset_estimate_table_size (plan, blob->length, TableType::tableTag);
  DEBUG_संदेश (SUBSET, nullptr,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (tag), buf_size);
  यदि (unlikely (!buf.alloc (buf_size)))
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (tag), buf_size);
    _hb_do_destroy (source_blob, hb_prioritize);
    प्रत्यागम असत्य;
  }

  तार्किक needed = असत्य;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, tag);
    needed = _hb_subset_table_try (table, &buf, &c);
  }
  _hb_do_destroy (source_blob, hb_prioritize);

  यदि (serializer.in_error () && !serializer.only_offset_overflow ())
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (tag));
    प्रत्यागम असत्य;
  }

  यदि (!needed)
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (tag));
    प्रत्यागम सत्य;
  }

  तार्किक result = असत्य;
  hb_blob_t *dest_blob = _hb_subset_repack (tag, serializer);
  यदि (dest_blob)
  {
    DEBUG_संदेश (SUBSET, nullptr,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (tag), dest_blob->length);
    result = plan->add_table (tag, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (tag), result ? "success" : "FAILED!");
  प्रत्यागम result;
}

स्थिर HB_UNUSED तार्किक
_hb_subset_table_passthrough (hb_subset_plan_t *plan, hb_tag_t tag)
{
  hb_blob_t *source_table = hb_face_reference_table (plan->source, tag);
  तार्किक result = plan->add_table (tag, source_table);
  hb_blob_destroy (source_table);
  प्रत्यागम result;
}


HB_INTERNAL तार्किक _hb_subset_table_layout	(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success);
HB_INTERNAL तार्किक _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success);
HB_INTERNAL तार्किक _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success);
HB_INTERNAL तार्किक _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success);
HB_INTERNAL तार्किक _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success);


#endif /* HB_SUBSET_TABLE_HH */
