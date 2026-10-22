/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#अगर_अघोषित HB_OT_STAT_TABLE_HH
#घोषणा HB_OT_STAT_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#घोषणा HB_OT_TAG_STAT HB_TAG('S','T','A','T')


namespace OT {

क्रमागत
{
  OLDER_SIBLING_FONT_ATTRIBUTE = 0x0001,	/* If set, this axis value table
						 * provides axis value information
						 * that is applicable to other fonts
						 * within the same font family. This
						 * is used if the other fonts were
						 * released earlier and did not include
						 * information about values for some axis.
						 * If newer versions of the other
						 * fonts include the information
						 * themselves and are present,
						 * then this record is ignored. */
  ELIDABLE_AXIS_मूल्य_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

अटल bool axis_मूल्य_is_outside_axis_range (hb_tag_t axis_tag, भग्न axis_मूल्य,
                                              स्थिर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  अगर (!user_axes_location->has (axis_tag))
    वापस false;

  Triple axis_range = user_axes_location->लो (axis_tag);
  वापस (axis_मूल्य < axis_range.minimum || axis_मूल्य > axis_range.maximum);
}

काष्ठा StatAxisRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिर { वापस tag.cmp (key); }

  hb_ot_name_id_t get_name_id () स्थिर { वापस nameID; }

  hb_tag_t get_axis_tag () स्थिर { वापस tag; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह)));
  }

  रक्षित:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा Axis_मूल्य_Format1
{
  अचिन्हित पूर्णांक get_axis_सूचकांक () स्थिर { वापस axis_सूचकांक_2; }
  भग्न get_मूल्य ()             स्थिर { वापस value.to_float (); }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिर { वापस मूल्य_Name_ID; }

  hb_tag_t get_axis_tag (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    अचिन्हित axis_idx = get_axis_सूचकांक ();
    वापस axis_records[axis_idx].get_axis_tag ();
  }

  bool keep_axis_मूल्य (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records,
                        स्थिर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिर
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भग्न axis_मूल्य = get_मूल्य ();

    वापस !axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    अगर (keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (c->serializer->embed (यह));

    return_trace (false);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axis_सूचकांक_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  खुला:
  DEFINE_आकार_STATIC (12);
};

काष्ठा Axis_मूल्य_Format2
{
  अचिन्हित पूर्णांक get_axis_सूचकांक () स्थिर { वापस axis_सूचकांक_2; }
  भग्न get_मूल्य ()             स्थिर { वापस nominal_मूल्य.to_float (); }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिर { वापस मूल्य_Name_ID; }

  hb_tag_t get_axis_tag (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    अचिन्हित axis_idx = get_axis_सूचकांक ();
    वापस axis_records[axis_idx].get_axis_tag ();
  }

  bool keep_axis_मूल्य (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records,
                        स्थिर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिर
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भग्न axis_मूल्य = get_मूल्य ();

    वापस !axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    अगर (keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (c->serializer->embed (यह));

    return_trace (false);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axis_सूचकांक_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_मूल्य;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_मूल्य;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_मूल्य;	/* The maximum value for a range associated
				 * with the specified name ID. */
  खुला:
  DEFINE_आकार_STATIC (20);
};

काष्ठा Axis_मूल्य_Format3
{
  अचिन्हित पूर्णांक get_axis_सूचकांक () स्थिर { वापस axis_सूचकांक_2; }
  भग्न get_मूल्य ()             स्थिर { वापस value.to_float (); }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिर { वापस मूल्य_Name_ID; }

  hb_tag_t get_axis_tag (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    अचिन्हित axis_idx = get_axis_सूचकांक ();
    वापस axis_records[axis_idx].get_axis_tag ();
  }

  bool keep_axis_मूल्य (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records,
                        स्थिर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिर
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भग्न axis_मूल्य = get_मूल्य ();

    वापस !axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    अगर (keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (c->serializer->embed (यह));

    return_trace (false);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axis_सूचकांक_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_मूल्य;	/* The numeric value for a style-linked mapping
				 * from this value. */
  खुला:
  DEFINE_आकार_STATIC (16);
};

काष्ठा Axis_मूल्य_Record
{
  अचिन्हित पूर्णांक get_axis_सूचकांक () स्थिर { वापस axis_सूचकांक_2; }
  भग्न get_मूल्य ()             स्थिर { वापस value.to_float (); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	axis_सूचकांक_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा Axis_मूल्य_Format4
{
  स्थिर Axis_मूल्य_Record &get_axis_record (अचिन्हित पूर्णांक axis_सूचकांक) स्थिर
  { वापस axis_मूल्यावली_2.as_array (axis_गिनती_2)[axis_सूचकांक]; }

  bool keep_axis_मूल्य (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records,
                        स्थिर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिर
  {
    hb_array_t<स्थिर Axis_मूल्य_Record> axis_मूल्य_records = axis_मूल्यावली_2.as_array (axis_गिनती_2);

    क्रम (स्थिर स्वतः& rec : axis_मूल्य_records)
    {
      अचिन्हित axis_idx = rec.get_axis_सूचकांक ();
      भग्न axis_मूल्य = rec.get_मूल्य ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      अगर (axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location))
        वापस false;
    }

    वापस true;
  }

  bool subset (hb_subset_context_t *c,
               स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    अगर (!keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (false);

    अचिन्हित योग_आकार = min_आकार + axis_गिनती_2 * Axis_मूल्य_Record::static_आकार;
    स्वतः *out = c->serializer->allocate_आकार<Axis_मूल्य_Format4> (योग_आकार);
    अगर (unlikely (!out)) return_trace (false);
    hb_memcpy (out, यह, योग_आकार);
    return_trace (true);
  }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिर { वापस मूल्य_Name_ID; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
                          axis_मूल्यावली_2.sanitize (c, axis_गिनती_2)));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_गिनती_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_मूल्य_Record>
		axis_मूल्यावली_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  खुला:
  DEFINE_आकार_ARRAY (8, axis_मूल्यावली_2);
};

काष्ठा Axis_मूल्य
{
  भग्न get_मूल्य (अचिन्हित पूर्णांक axis_सूचकांक) स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.get_मूल्य ();
    हाल 2: hb_barrier (); वापस u.format2.get_मूल्य ();
    हाल 3: hb_barrier (); वापस u.format3.get_मूल्य ();
    हाल 4: hb_barrier (); वापस u.format4.get_axis_record (axis_सूचकांक).get_मूल्य ();
    शेष:वापस 0.f;
    }
  }

  अचिन्हित पूर्णांक get_axis_सूचकांक () स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.get_axis_सूचकांक ();
    हाल 2: hb_barrier (); वापस u.format2.get_axis_सूचकांक ();
    हाल 3: hb_barrier (); वापस u.format3.get_axis_सूचकांक ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    शेष:वापस -1;
    }
  }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.get_मूल्य_name_id ();
    हाल 2: hb_barrier (); वापस u.format2.get_मूल्य_name_id ();
    हाल 3: hb_barrier (); वापस u.format3.get_मूल्य_name_id ();
    हाल 4: hb_barrier (); वापस u.format4.get_मूल्य_name_id ();
    शेष:वापस HB_OT_NAME_ID_INVALID;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  bool keep_axis_मूल्य (स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिर
  {
    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); वापस u.format1.keep_axis_मूल्य (axis_records, user_axes_location);
    हाल 2: hb_barrier (); वापस u.format2.keep_axis_मूल्य (axis_records, user_axes_location);
    हाल 3: hb_barrier (); वापस u.format3.keep_axis_मूल्य (axis_records, user_axes_location);
    हाल 4: hb_barrier (); वापस u.format4.keep_axis_मूल्य (axis_records, user_axes_location);
    शेष:वापस false;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();

    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    शेष:return_trace (true);
    }
  }

  रक्षित:
  जोड़
  {
  काष्ठा { HBUINT16 v; }	format;
  Axis_मूल्य_Format1	format1;
  Axis_मूल्य_Format2	format2;
  Axis_मूल्य_Format3	format3;
  Axis_मूल्य_Format4	format4;
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};

काष्ठा Axis_मूल्य_Offset_Array: UnsizedArrayOf<Offset16To<Axis_मूल्य>>
{
  bool subset (hb_subset_context_t *c,
               अचिन्हित axis_मूल्य_गिनती,
               अचिन्हित& गिनती,
               स्थिर hb_array_t<स्थिर StatAxisRecord> axis_records) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः axis_मूल्य_Offsets = as_array (axis_मूल्य_गिनती);
    गिनती = 0;
    क्रम (स्थिर स्वतः& offset : axis_मूल्य_Offsets)
    {
      अगर (!offset) जारी;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = c->serializer->embed (offset);
      अगर (!o) return_trace (false);
      अगर (!o->serialize_subset (c, offset, यह, axis_records))
      {
        c->serializer->revert (o_snap);
        जारी;
      }
      गिनती++;
    }

    return_trace (गिनती);
  }
};

काष्ठा STAT
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  bool get_मूल्य (hb_tag_t tag, भग्न *value) स्थिर
  {
    अचिन्हित पूर्णांक axis_सूचकांक;
    अगर (!get_design_axes ().lfind (tag, &axis_सूचकांक)) वापस false;

    hb_array_t<स्थिर Offset16To<Axis_मूल्य>> axis_मूल्यावली = get_axis_मूल्य_offsets ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < axis_मूल्यावली.लंबाई; i++)
    {
      स्थिर Axis_मूल्य& axis_मूल्य = यह+offset_To_Axis_मूल्य_Offsets+axis_मूल्यावली[i];
      अगर (axis_मूल्य.get_axis_सूचकांक () == axis_सूचकांक)
      {
	अगर (value)
	  *value = axis_मूल्य.get_मूल्य (axis_सूचकांक);
	वापस true;
      }
    }
    वापस false;
  }

  अचिन्हित get_design_axis_गिनती () स्थिर { वापस design_Axis_गिनती; }

  hb_ot_name_id_t get_axis_record_name_id (अचिन्हित axis_record_सूचकांक) स्थिर
  {
    अगर (unlikely (axis_record_सूचकांक >= design_Axis_गिनती)) वापस HB_OT_NAME_ID_INVALID;
    स्थिर StatAxisRecord &axis_record = get_design_axes ()[axis_record_सूचकांक];
    वापस axis_record.get_name_id ();
  }

  अचिन्हित get_axis_मूल्य_गिनती () स्थिर { वापस axis_मूल्य_गिनती; }

  hb_ot_name_id_t get_axis_मूल्य_name_id (अचिन्हित axis_मूल्य_सूचकांक) स्थिर
  {
    अगर (unlikely (axis_मूल्य_सूचकांक >= axis_मूल्य_गिनती)) वापस HB_OT_NAME_ID_INVALID;
    स्थिर Axis_मूल्य &axis_मूल्य = (यह + get_axis_मूल्य_offsets ()[axis_मूल्य_सूचकांक]);
    वापस axis_मूल्य.get_मूल्य_name_id ();
  }

  व्योम collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) स्थिर
  {
    अगर (!has_तथ्य ()) वापस;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    स्वतः designAxes = get_design_axes ();

    + get_axis_मूल्य_offsets ()
    | hb_map (hb_add (&(यह + offset_To_Axis_मूल्य_Offsets)))
    | hb_filter ([&] (स्थिर Axis_मूल्य& _)
                 { वापस _.keep_axis_मूल्य (designAxes, user_axes_location); })
    | hb_map (&Axis_मूल्य::get_मूल्य_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    STAT *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    स्वतः designAxes = get_design_axes ();
    क्रम (अचिन्हित i = 0; i < (अचिन्हित)design_Axis_गिनती; i++)
      अगर (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (false);

    अगर (design_Axis_गिनती)
      c->serializer->check_assign (out->designAxesOffset, यह->get_आकार (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    अचिन्हित गिनती = 0;
    out->offset_To_Axis_मूल्य_Offsets.serialize_subset (c, offset_To_Axis_मूल्य_Offsets, यह,
                                                    axis_मूल्य_गिनती, गिनती, designAxes);
    return_trace (c->serializer->check_assign (out->axis_मूल्य_गिनती, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, यह, design_Axis_गिनती) &&
			  offset_To_Axis_मूल्य_Offsets.sanitize (c, यह, axis_मूल्य_गिनती, &(यह+offset_To_Axis_मूल्य_Offsets))));
  }

  रक्षित:
  hb_array_t<स्थिर StatAxisRecord> स्थिर get_design_axes () स्थिर
  { वापस (यह+designAxesOffset).as_array (design_Axis_गिनती); }

  hb_array_t<स्थिर Offset16To<Axis_मूल्य>> स्थिर get_axis_मूल्य_offsets () स्थिर
  { वापस (यह+offset_To_Axis_मूल्य_Offsets).as_array (axis_मूल्य_गिनती); }


  रक्षित:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_आकार;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_गिनती;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32To<UnsizedArrayOf<StatAxisRecord>>
		designAxesOffset;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_मूल्य_गिनती;	/* The number of axis value tables. */
  NNOffset32To<Axis_मूल्य_Offset_Array>
		offset_To_Axis_मूल्य_Offsets;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes value offsets array. If axisValueCount
				 * is zero, set to zero; if axisValueCount is
				 * greater than zero, must be greater than zero. */
  NameID	elidedFallbackNameID;
				/* Name ID used as fallback when projection of
				 * names into a particular font model produces
				 * a subfamily name containing only elidable
				 * elements. */
  खुला:
  DEFINE_आकार_STATIC (20);
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_STAT_TABLE_HH */
