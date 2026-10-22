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

#अगर_अघोषित HB_SUBSET_TABLE_HH
#घोषणा HB_SUBSET_TABLE_HH


#समावेश "hb.hh"

#समावेश "hb-subset.hh"
#समावेश "hb-repacker.hh"


ढाँचा<typename TableType>
अटल bool
_hb_subset_table_try (स्थिर TableType *table,
		      hb_vector_t<अक्षर>* buf,
		      hb_subset_context_t* c /* OUT */)
{
  c->serializer->start_serialize ();
  अगर (c->serializer->in_error ()) वापस false;

  bool needed = table->subset (c);
  अगर (!c->serializer->ran_out_of_room ())
  {
    c->serializer->end_serialize ();
    वापस needed;
  }

  अचिन्हित buf_आकार = buf->allocated;
  buf_आकार = buf_आकार * 2 + 16;




  DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c ran out of room; reallocating to %u bytes.",
             HB_UNTAG (c->table_tag), buf_आकार);

  अगर (unlikely (buf_आकार > c->source_blob->लंबाई * 256 ||
		!buf->alloc_exact (buf_आकार)))
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c failed to reallocate %u bytes.",
               HB_UNTAG (c->table_tag), buf_आकार);
    वापस needed;
  }

  c->serializer->reset (buf->arrayZ, buf->allocated);
  वापस _hb_subset_table_try (table, buf, c);
}

अटल HB_UNUSED अचिन्हित
hb_subset_estimate_table_आकार (hb_subset_plan_t *plan,
				अचिन्हित table_len,
				hb_tag_t table_tag)
{
  अचिन्हित src_glyphs = plan->source->get_अंक_glyphs ();
  अचिन्हित dst_glyphs = plan->glyphset ()->get_population ();

  अचिन्हित bulk = 8192;
  /* Tables that we want to allocate same space as the source table. For GSUB/GPOS it's
   * because those are expensive to subset, so giving them more room is fine. */
  bool same_आकार = table_tag == HB_TAG('G','S','U','B') ||
		   table_tag == HB_TAG('G','P','O','S') ||
		   table_tag == HB_TAG('G','D','E','F') ||
		   table_tag == HB_TAG('n','a','m','e');

  अगर (plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS)
  {
    अगर (table_tag == HB_TAG('C','F','F',' '))
    {
      /* Add some extra room for the CFF charset. */
      bulk += src_glyphs * 16;
    }
    अन्यथा अगर (table_tag == HB_TAG('C','F','F','2'))
    {
      /* Just extra CharString offsets. */
      bulk += src_glyphs * 4;
    }
  }

  अगर (unlikely (!src_glyphs) || same_आकार)
    वापस bulk + table_len;

  वापस bulk + (अचिन्हित) (table_len * वर्ग_मूल ((द्विगुन) dst_glyphs / src_glyphs));
}

/*
 * Repack the serialization buffer if any offset overflows exist.
 */
अटल HB_UNUSED hb_blob_t*
_hb_subset_repack (hb_tag_t tag, स्थिर hb_serialize_context_t& c)
{
  अगर (!c.offset_overflow ())
    वापस c.copy_blob ();

  hb_blob_t* परिणाम = hb_resolve_overflows (c.object_graph (), tag);

  अगर (unlikely (!परिणाम))
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c offset overflow resolution failed.",
               HB_UNTAG (tag));
    वापस nullptr;
  }

  वापस परिणाम;
}

ढाँचा <typename T>
अटल HB_UNUSED स्वतः _hb_do_destroy (T &t, hb_priority<1>) HB_RETURN (व्योम, t.destroy ())

ढाँचा <typename T>
अटल HB_UNUSED व्योम _hb_do_destroy (T &t, hb_priority<0>) {}

ढाँचा<typename TableType>
अटल bool
_hb_subset_table (hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf)
{
  स्वतः &&source_blob = plan->source_table<TableType> ();
  स्वतः *table = source_blob.लो ();

  hb_tag_t tag = TableType::tableTag;
  hb_blob_t *blob = source_blob.get_blob();
  अगर (unlikely (!blob || !blob->तथ्य))
  {
    DEBUG_संदेश (SUBSET, nullptr,
               "OT::%c%c%c%c::subset sanitize failed on source table.", HB_UNTAG (tag));
    _hb_do_destroy (source_blob, hb_prioritize);
    वापस false;
  }

  अचिन्हित buf_आकार = hb_subset_estimate_table_आकार (plan, blob->लंबाई, TableType::tableTag);
  DEBUG_संदेश (SUBSET, nullptr,
             "OT::%c%c%c%c initial estimated table size: %u bytes.", HB_UNTAG (tag), buf_आकार);
  अगर (unlikely (!buf.alloc (buf_आकार)))
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c failed to allocate %u bytes.", HB_UNTAG (tag), buf_आकार);
    _hb_do_destroy (source_blob, hb_prioritize);
    वापस false;
  }

  bool needed = false;
  hb_serialize_context_t serializer (buf.arrayZ, buf.allocated);
  {
    hb_subset_context_t c (blob, plan, &serializer, tag);
    needed = _hb_subset_table_try (table, &buf, &c);
  }
  _hb_do_destroy (source_blob, hb_prioritize);

  अगर (serializer.in_error () && !serializer.only_offset_overflow ())
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c::subset FAILED!", HB_UNTAG (tag));
    वापस false;
  }

  अगर (!needed)
  {
    DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c::subset table subsetted to empty.", HB_UNTAG (tag));
    वापस true;
  }

  bool परिणाम = false;
  hb_blob_t *dest_blob = _hb_subset_repack (tag, serializer);
  अगर (dest_blob)
  {
    DEBUG_संदेश (SUBSET, nullptr,
               "OT::%c%c%c%c final subset table size: %u bytes.",
               HB_UNTAG (tag), dest_blob->लंबाई);
    परिणाम = plan->add_table (tag, dest_blob);
    hb_blob_destroy (dest_blob);
  }

  DEBUG_संदेश (SUBSET, nullptr, "OT::%c%c%c%c::subset %s",
             HB_UNTAG (tag), परिणाम ? "success" : "FAILED!");
  वापस परिणाम;
}

अटल HB_UNUSED bool
_hb_subset_table_passthrough (hb_subset_plan_t *plan, hb_tag_t tag)
{
  hb_blob_t *source_table = hb_face_reference_table (plan->source, tag);
  bool परिणाम = plan->add_table (tag, source_table);
  hb_blob_destroy (source_table);
  वापस परिणाम;
}


HB_INTERNAL bool _hb_subset_table_layout	(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success);
HB_INTERNAL bool _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success);
HB_INTERNAL bool _hb_subset_table_cff		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success);
HB_INTERNAL bool _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success);
HB_INTERNAL bool _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success);


#पूर्ण_अगर /* HB_SUBSET_TABLE_HH */
