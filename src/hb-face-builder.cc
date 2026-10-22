/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-face.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-file.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-serialize.hh"


/*
 * face-builder: A face that has add_table().
 */

ਬਣਤਰ face_ਸਾਰਣੀ_info_t
{
  hb_blob_t* ਅੰਕੜੇ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕ੍ਰਮ;
};

ਬਣਤਰ hb_face_builder_ਅੰਕੜੇ_t
{
  hb_hashmap_t<hb_ਟੈਗ_t, face_ਸਾਰਣੀ_info_t> tables;
};

ਸਥਿਰ ਪੂਰਨ compare_entries (ਅਚਲ ਖਾਲੀ* pa, ਅਚਲ ਖਾਲੀ* pb)
{
  ਅਚਲ ਸਵੈਚਾਲਿਤ& a = * (ਅਚਲ hb_pair_t<hb_ਟੈਗ_t, face_ਸਾਰਣੀ_info_t> *) pa;
  ਅਚਲ ਸਵੈਚਾਲਿਤ& b = * (ਅਚਲ hb_pair_t<hb_ਟੈਗ_t, face_ਸਾਰਣੀ_info_t> *) pb;

  /* Order by blob size first (smallest to largest) and then table tag */

  ਜੇ (a.second.ਕ੍ਰਮ != b.second.ਕ੍ਰਮ)
    ਵਾਪਸ a.second.ਕ੍ਰਮ < b.second.ਕ੍ਰਮ ? -1 : +1;

  ਜੇ (a.second.ਅੰਕੜੇ->ਲੰਮਾਈ != b.second.ਅੰਕੜੇ->ਲੰਮਾਈ)
    ਵਾਪਸ a.second.ਅੰਕੜੇ->ਲੰਮਾਈ < b.second.ਅੰਕੜੇ->ਲੰਮਾਈ ? -1 : +1;

  ਵਾਪਸ a.ਪਹਿਲਾ < b.ਪਹਿਲਾ ? -1 : a.ਪਹਿਲਾ == b.ਪਹਿਲਾ ? 0 : +1;
}

ਸਥਿਰ hb_face_builder_ਅੰਕੜੇ_t *
hb_face_builder_ਅੰਕੜੇ_create ()
{
  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_face_builder_ਅੰਕੜੇ_t *) hb_calloc (1, ਆਕਾਰ (hb_face_builder_ਅੰਕੜੇ_t));
  ਜੇ (unlikely (!ਅੰਕੜੇ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅੰਕੜੇ->tables.init ();

  ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ
hb_face_builder_ਅੰਕੜੇ_destroy (ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_face_builder_ਅੰਕੜੇ_t *) user_ਅੰਕੜੇ;

  ਲਈ (ਸਵੈਚਾਲਿਤ info : ਅੰਕੜੇ->tables.values())
    hb_blob_destroy (info.ਅੰਕੜੇ);

  ਅੰਕੜੇ->tables.fini ();

  hb_free (ਅੰਕੜੇ);
}

ਸਥਿਰ hb_blob_t *
hb_face_builder_ਅੰਕੜੇ_reference_blob (hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਗਿਣਤੀ_2 = ਅੰਕੜੇ->tables.get_population ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ face_ਲੰਮਾਈ = ਸਾਰਣੀ_ਗਿਣਤੀ_2 * 16 + 12;

  ਲਈ (ਸਵੈਚਾਲਿਤ info : ਅੰਕੜੇ->tables.values())
    face_ਲੰਮਾਈ += hb_ceil_ਮੰਜ਼ਿਲ_4 (hb_blob_get_ਲੰਮਾਈ (info.ਅੰਕੜੇ));

  ਅੱਖਰ *buf = (ਅੱਖਰ *) hb_malloc (face_ਲੰਮਾਈ);
  ਜੇ (unlikely (!buf))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_serialize_context_t c (buf, face_ਲੰਮਾਈ);
  c.propagate_error (ਅੰਕੜੇ->tables);
  OT::OpenTypeFontFile *f = c.ਸ਼ੁਰੂ_serialize<OT::OpenTypeFontFile> ();

  ਤਾਰਕਿਕ is_cff = (ਅੰਕੜੇ->tables.has (HB_ਟੈਗ ('C','F','F',' '))
                 || ਅੰਕੜੇ->tables.has (HB_ਟੈਗ ('C','F','F','2')));
  hb_ਟੈਗ_t sfnt_ਟੈਗ = is_cff ? OT::OpenTypeFontFile::CFFTag : OT::OpenTypeFontFile::True_Type_ਟੈਗ;

  // Sort the tags so that produced face is deterministic.
  hb_ਸਦਿਸ਼_t<hb_pair_t <hb_ਟੈਗ_t, face_ਸਾਰਣੀ_info_t>> sorted_entries;
  ਅੰਕੜੇ->tables.iter () | hb_sink (sorted_entries);
  ਜੇ (unlikely (sorted_entries.ਵਿੱਚ_error ()))
  {
    hb_free (buf);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  sorted_entries.qsort (compare_entries);

  ਤਾਰਕਿਕ ret = f->serialize_single (&c,
                                  sfnt_ਟੈਗ,
                                  + sorted_entries.iter()
                                  | hb_map ([&] (hb_pair_t<hb_ਟੈਗ_t, face_ਸਾਰਣੀ_info_t> _) {
                                    ਵਾਪਸ hb_pair_t<hb_ਟੈਗ_t, hb_blob_t*> (_.ਪਹਿਲਾ, _.second.ਅੰਕੜੇ);
                                  }));

  c.ਸਿਰਾ_serialize ();

  ਜੇ (unlikely (!ret))
  {
    hb_free (buf);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ hb_blob_create (buf, face_ਲੰਮਾਈ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, buf, hb_free);
}

ਸਥਿਰ hb_blob_t *
hb_face_builder_reference_ਸਾਰਣੀ (hb_face_t *face HB_ਅਣਵਰਤਿਆ, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_face_builder_ਅੰਕੜੇ_t *) user_ਅੰਕੜੇ;

  ਜੇ (!ਟੈਗ_2)
    ਵਾਪਸ hb_face_builder_ਅੰਕੜੇ_reference_blob (ਅੰਕੜੇ);

  ਵਾਪਸ hb_blob_reference (ਅੰਕੜੇ->tables[ਟੈਗ_2].ਅੰਕੜੇ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_face_builder_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਅਚਲ hb_face_t *face HB_ਅਣਵਰਤਿਆ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2,
				 hb_ਟੈਗ_t *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ,
				 ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_face_builder_ਅੰਕੜੇ_t *) user_ਅੰਕੜੇ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ population = ਅੰਕੜੇ->tables.get_population ();

  ਜੇ (!ਸਾਰਣੀ_ਗਿਣਤੀ_2)
    ਵਾਪਸ population;

  ਜੇ (unlikely (ਸ਼ੁਰੂ_ਵਿੱਥ >= population))
  {
    *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ population;
  }

  // Sort the tags.
  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> sorted_ਟੈਗ_ਸੂਚੀ;
  ਅੰਕੜੇ->tables.ਕੁੰਜੀਆਂ () | hb_sink (sorted_ਟੈਗ_ਸੂਚੀ);
  ਜੇ (unlikely (sorted_ਟੈਗ_ਸੂਚੀ.ਵਿੱਚ_error ()))
  {
    // Not much to do...
  }
  sorted_ਟੈਗ_ਸੂਚੀ.qsort ([] (ਅਚਲ hb_ਟੈਗ_t &a, ਅਚਲ hb_ਟੈਗ_t &b) {
    ਵਾਪਸ (a > b) - (a < b);
  });

  ਸਵੈਚਾਲਿਤ array = sorted_ਟੈਗ_ਸੂਚੀ.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਸਾਰਣੀ_ਗਿਣਤੀ_2);
  ਜੇ (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ)
  {
    ਸਵੈਚਾਲਿਤ ਬਾਹਰਲਾ = hb_array (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ, *ਸਾਰਣੀ_ਗਿਣਤੀ_2);

    + array.iter ()
    | hb_sink (ਬਾਹਰਲਾ)
    ;
  }

  ਵਾਪਸ population;
}


/**
 * hb_face_builder_create:
 *
 * Creates a #hb_face_t that can be used with hb_face_builder_add_table().
 * After tables are added to the face, it can be compiled to a binary
 * font file by calling hb_face_reference_blob().
 *
 * Return value: (transfer full): New face.
 *
 * Since: 1.9.0
 **/
hb_face_t *
hb_face_builder_create ()
{
  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = hb_face_builder_ਅੰਕੜੇ_create ();
  ਜੇ (unlikely (!ਅੰਕੜੇ)) ਵਾਪਸ hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_face_t *face = hb_face_create_for_tables (hb_face_builder_reference_ਸਾਰਣੀ,
					       ਅੰਕੜੇ,
					       hb_face_builder_ਅੰਕੜੇ_destroy);

  hb_face_ਸੈੱਟ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (face,
				   hb_face_builder_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ,
				   ਅੰਕੜੇ,
				   ਖਾਲੀ_ਸੂਚਕ);

  ਵਾਪਸ face;
}

/**
 * hb_face_builder_add_table:
 * @face: A face object created with hb_face_builder_create()
 * @tag: The #hb_tag_t of the table to add
 * @blob: The blob containing the table data to add
 *
 * Add table for @tag with data provided by @blob to the face.  @face must
 * be created using hb_face_builder_create().
 *
 * Since: 1.9.0
 **/
hb_bool_t
hb_face_builder_ਜੋੜੋ_ਸਾਰਣੀ (hb_face_t *face, hb_ਟੈਗ_t ਟੈਗ_2, hb_blob_t *blob)
{
  ਜੇ (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_ਅੰਕੜੇ_destroy))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਟੈਗ_2 == HB_MAP_ਮੁੱਲ_INVALID)
    ਵਾਪਸ ਝੂਠ;

  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_face_builder_ਅੰਕੜੇ_t *) face->user_ਅੰਕੜੇ;

  hb_blob_t* ਪਿਛਲਾ = ਅੰਕੜੇ->tables.get (ਟੈਗ_2).ਅੰਕੜੇ;
  ਜੇ (!ਅੰਕੜੇ->tables.ਸੈੱਟ (ਟੈਗ_2, face_ਸਾਰਣੀ_info_t {hb_blob_reference (blob), (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1}))
  {
    hb_blob_destroy (blob);
    ਵਾਪਸ ਝੂਠ;
  }

  hb_blob_destroy (ਪਿਛਲਾ);
  ਵਾਪਸ ਸੱਚ;
}

/**
 * hb_face_builder_sort_tables:
 * @face: A face object created with hb_face_builder_create()
 * @tags: (array zero-terminated=1): ordered list of table tags terminated by
 *   %HB_TAG_NONE
 *
 * Set the ordering of tables for serialization. Any tables not
 * specified in the tags list will be ordered after the tables in
 * tags, ordered by the default sort ordering.
 *
 * Since: 5.3.0
 **/
ਖਾਲੀ
hb_face_builder_ਛਾਂਟੋ_tables (hb_face_t *face,
                             ਅਚਲ hb_ਟੈਗ_t  *ਟੈਗ_ਸੂਚੀ)
{
  ਜੇ (unlikely (face->destroy != (hb_destroy_func_t) hb_face_builder_ਅੰਕੜੇ_destroy))
    ਵਾਪਸ;

  hb_face_builder_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_face_builder_ਅੰਕੜੇ_t *) face->user_ਅੰਕੜੇ;

  // Sort all unspecified tables after any specified tables.
  ਲਈ (ਸਵੈਚਾਲਿਤ& info : ਅੰਕੜੇ->tables.ਮੁੱਲਾਂ_ref())
    info.ਕ੍ਰਮ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;

  ਨਿਸ਼ਾਨੀਵਾਲਾ ਕ੍ਰਮ = 0;
  ਲਈ (ਅਚਲ hb_ਟੈਗ_t* ਟੈਗ_2 = ਟੈਗ_ਸੂਚੀ;
       *ਟੈਗ_2;
       ਟੈਗ_2++)
  {
    face_ਸਾਰਣੀ_info_t* info;
    ਜੇ (!ਅੰਕੜੇ->tables.has (*ਟੈਗ_2, &info)) ਜਾਰੀ;
    info->ਕ੍ਰਮ = ਕ੍ਰਮ++;
  }
}
