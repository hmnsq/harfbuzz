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

#ifndef HB_OT_STAT_TABLE_HH
#define HB_OT_STAT_TABLE_HH

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


નામાકાશ OT {

ગણના
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
  ELIDABLE_AXIS_કિંમત_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

સ્થિર તાર્કિક axis_કિંમત_is_outside_axis_range (hb_tag_t axis_tag, દશાંશ axis_કિંમત,
                                              અચલ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  જો (!user_axes_location->has (axis_tag))
    પાછા ખોટું;

  Triple axis_range = user_axes_location->get (axis_tag);
  પાછા (axis_કિંમત < axis_range.minimum || axis_કિંમત > axis_range.maximum);
}

સંરચના StatAxisRecord
{
  પૂર્ણાંક cmp (hb_tag_t key) અચલ { પાછા tag.cmp (key); }

  hb_ot_name_id_t get_name_id () અચલ { પાછા nameID; }

  hb_tag_t get_axis_tag () અચલ { પાછા tag; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ)));
  }

  સુરક્ષિત:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના Axis_કિંમત_Format1
{
  unsigned પૂર્ણાંક get_axis_index () અચલ { પાછા axisIndex; }
  દશાંશ get_કિંમત ()             અચલ { પાછા value.to_float (); }

  hb_ot_name_id_t get_કિંમત_name_id () અચલ { પાછા કિંમત_Name_ID; }

  hb_tag_t get_axis_tag (અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    unsigned axis_idx = get_axis_index ();
    પાછા axis_records[axis_idx].get_axis_tag ();
  }

  તાર્કિક keep_axis_કિંમત (અચલ hb_array_t<અચલ StatAxisRecord> axis_records,
                        અચલ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) અચલ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    દશાંશ axis_કિંમત = get_કિંમત ();

    પાછા !axis_કિંમત_is_outside_axis_range (axis_tag, axis_કિંમત, user_axes_location);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    જો (keep_axis_કિંમત (axis_records, user_axes_location))
      return_trace (c->serializer->embed (આ));

    return_trace (ખોટું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	કિંમત_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

સંરચના Axis_કિંમત_Format2
{
  unsigned પૂર્ણાંક get_axis_index () અચલ { પાછા axisIndex; }
  દશાંશ get_કિંમત ()             અચલ { પાછા nominal_કિંમત.to_float (); }

  hb_ot_name_id_t get_કિંમત_name_id () અચલ { પાછા કિંમત_Name_ID; }

  hb_tag_t get_axis_tag (અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    unsigned axis_idx = get_axis_index ();
    પાછા axis_records[axis_idx].get_axis_tag ();
  }

  તાર્કિક keep_axis_કિંમત (અચલ hb_array_t<અચલ StatAxisRecord> axis_records,
                        અચલ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) અચલ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    દશાંશ axis_કિંમત = get_કિંમત ();

    પાછા !axis_કિંમત_is_outside_axis_range (axis_tag, axis_કિંમત, user_axes_location);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    જો (keep_axis_કિંમત (axis_records, user_axes_location))
      return_trace (c->serializer->embed (આ));

    return_trace (ખોટું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	કિંમત_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_કિંમત;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_કિંમત;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_કિંમત;	/* The maximum value for a range associated
				 * with the specified name ID. */
  જાહેર:
  DEFINE_SIZE_STATIC (20);
};

સંરચના Axis_કિંમત_Format3
{
  unsigned પૂર્ણાંક get_axis_index () અચલ { પાછા axisIndex; }
  દશાંશ get_કિંમત ()             અચલ { પાછા value.to_float (); }

  hb_ot_name_id_t get_કિંમત_name_id () અચલ { પાછા કિંમત_Name_ID; }

  hb_tag_t get_axis_tag (અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    unsigned axis_idx = get_axis_index ();
    પાછા axis_records[axis_idx].get_axis_tag ();
  }

  તાર્કિક keep_axis_કિંમત (અચલ hb_array_t<અચલ StatAxisRecord> axis_records,
                        અચલ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) અચલ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    દશાંશ axis_કિંમત = get_કિંમત ();

    પાછા !axis_કિંમત_is_outside_axis_range (axis_tag, axis_કિંમત, user_axes_location);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    જો (keep_axis_કિંમત (axis_records, user_axes_location))
      return_trace (c->serializer->embed (આ));

    return_trace (ખોટું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	કિંમત_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_કિંમત;	/* The numeric value for a style-linked mapping
				 * from this value. */
  જાહેર:
  DEFINE_SIZE_STATIC (16);
};

સંરચના Axis_કિંમત_Record
{
  unsigned પૂર્ણાંક get_axis_index () અચલ { પાછા axisIndex; }
  દશાંશ get_કિંમત ()             અચલ { પાછા value.to_float (); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના Axis_કિંમત_Format4
{
  અચલ Axis_કિંમત_Record &get_axis_record (unsigned પૂર્ણાંક axis_index) અચલ
  { પાછા axis_કિંમતો_2.as_array (axis_ગણતરી_2)[axis_index]; }

  તાર્કિક keep_axis_કિંમત (અચલ hb_array_t<અચલ StatAxisRecord> axis_records,
                        અચલ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) અચલ
  {
    hb_array_t<અચલ Axis_કિંમત_Record> axis_કિંમત_records = axis_કિંમતો_2.as_array (axis_ગણતરી_2);

    માટે (અચલ auto& rec : axis_કિંમત_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      દશાંશ axis_કિંમત = rec.get_કિંમત ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      જો (axis_કિંમત_is_outside_axis_range (axis_tag, axis_કિંમત, user_axes_location))
        પાછા ખોટું;
    }

    પાછા સાચું;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    જો (!keep_axis_કિંમત (axis_records, user_axes_location))
      return_trace (ખોટું);

    unsigned total_size = min_size + axis_ગણતરી_2 * Axis_કિંમત_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_કિંમત_Format4> (total_size);
    જો (unlikely (!out)) return_trace (ખોટું);
    hb_memcpy (out, આ, total_size);
    return_trace (સાચું);
  }

  hb_ot_name_id_t get_કિંમત_name_id () અચલ { પાછા કિંમત_Name_ID; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
                          axis_કિંમતો_2.sanitize (c, axis_ગણતરી_2)));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_ગણતરી_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	કિંમત_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_કિંમત_Record>
		axis_કિંમતો_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  જાહેર:
  DEFINE_SIZE_ARRAY (8, axis_કિંમતો_2);
};

સંરચના Axis_કિંમત
{
  દશાંશ get_કિંમત (unsigned પૂર્ણાંક axis_index) અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_કિંમત ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_કિંમત ();
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_કિંમત ();
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_axis_record (axis_index).get_કિંમત ();
    મૂળભૂત:પાછા 0.f;
    }
  }

  unsigned પૂર્ણાંક get_axis_index () અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_axis_index ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_axis_index ();
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    મૂળભૂત:પાછા -1;
    }
  }

  hb_ot_name_id_t get_કિંમત_name_id () અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_કિંમત_name_id ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_કિંમત_name_id ();
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_કિંમત_name_id ();
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_કિંમત_name_id ();
    મૂળભૂત:પાછા HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક keep_axis_કિંમત (અચલ hb_array_t<અચલ StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) અચલ
  {
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.keep_axis_કિંમત (axis_records, user_axes_location);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.keep_axis_કિંમત (axis_records, user_axes_location);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.keep_axis_કિંમત (axis_records, user_axes_location);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.keep_axis_કિંમત (axis_records, user_axes_location);
    મૂળભૂત:પાછા ખોટું;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();

    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  સંઘ
  {
  સંરચના { HBUINT16 v; }	format;
  Axis_કિંમત_Format1	format1;
  Axis_કિંમત_Format2	format2;
  Axis_કિંમત_Format3	format3;
  Axis_કિંમત_Format4	format4;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};

સંરચના Axis_કિંમત_Offset_Array: UnsizedArrayOf<Offset16To<Axis_કિંમત>>
{
  તાર્કિક subset (hb_subset_context_t *c,
               unsigned axis_કિંમત_ગણતરી,
               unsigned& ગણતરી,
               અચલ hb_array_t<અચલ StatAxisRecord> axis_records) અચલ
  {
    TRACE_SUBSET (આ);

    auto axis_કિંમત_Offsets = as_array (axis_કિંમત_ગણતરી);
    ગણતરી = 0;
    માટે (અચલ auto& offset : axis_કિંમત_Offsets)
    {
      જો (!offset) ચાલુ;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      જો (!o) return_trace (ખોટું);
      જો (!o->serialize_subset (c, offset, આ, axis_records))
      {
        c->serializer->revert (o_snap);
        ચાલુ;
      }
      ગણતરી++;
    }

    return_trace (ગણતરી);
  }
};

સંરચના STAT
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  તાર્કિક get_કિંમત (hb_tag_t tag, દશાંશ *value) અચલ
  {
    unsigned પૂર્ણાંક axis_index;
    જો (!get_design_axes ().lfind (tag, &axis_index)) પાછા ખોટું;

    hb_array_t<અચલ Offset16To<Axis_કિંમત>> axis_કિંમતો = get_axis_કિંમત_offsets ();
    માટે (unsigned પૂર્ણાંક i = 0; i < axis_કિંમતો.length; i++)
    {
      અચલ Axis_કિંમત& axis_કિંમત = આ+offset_To_Axis_કિંમત_Offsets+axis_કિંમતો[i];
      જો (axis_કિંમત.get_axis_index () == axis_index)
      {
	જો (value)
	  *value = axis_કિંમત.get_કિંમત (axis_index);
	પાછા સાચું;
      }
    }
    પાછા ખોટું;
  }

  unsigned get_design_axis_ગણતરી () અચલ { પાછા design_Axis_ગણતરી; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) અચલ
  {
    જો (unlikely (axis_record_index >= design_Axis_ગણતરી)) પાછા HB_OT_NAME_ID_INVALID;
    અચલ StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    પાછા axis_record.get_name_id ();
  }

  unsigned get_axis_કિંમત_ગણતરી () અચલ { પાછા axis_કિંમત_ગણતરી; }

  hb_ot_name_id_t get_axis_કિંમત_name_id (unsigned axis_કિંમત_index) અચલ
  {
    જો (unlikely (axis_કિંમત_index >= axis_કિંમત_ગણતરી)) પાછા HB_OT_NAME_ID_INVALID;
    અચલ Axis_કિંમત &axis_કિંમત = (આ + get_axis_કિંમત_offsets ()[axis_કિંમત_index]);
    પાછા axis_કિંમત.get_કિંમત_name_id ();
  }

  શૂન્ય collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) અચલ
  {
    જો (!has_data ()) પાછા;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_કિંમત_offsets ()
    | hb_map (hb_add (&(આ + offset_To_Axis_કિંમત_Offsets)))
    | hb_filter ([&] (અચલ Axis_કિંમત& _)
                 { પાછા _.keep_axis_કિંમત (designAxes, user_axes_location); })
    | hb_map (&Axis_કિંમત::get_કિંમત_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    STAT *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    auto designAxes = get_design_axes ();
    માટે (unsigned i = 0; i < (unsigned)design_Axis_ગણતરી; i++)
      જો (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (ખોટું);

    જો (design_Axis_ગણતરી)
      c->serializer->check_assign (out->designAxesOffset, આ->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned ગણતરી = 0;
    out->offset_To_Axis_કિંમત_Offsets.serialize_subset (c, offset_To_Axis_કિંમત_Offsets, આ,
                                                    axis_કિંમત_ગણતરી, ગણતરી, designAxes);
    return_trace (c->serializer->check_assign (out->axis_કિંમત_ગણતરી, ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, આ, design_Axis_ગણતરી) &&
			  offset_To_Axis_કિંમત_Offsets.sanitize (c, આ, axis_કિંમત_ગણતરી, &(આ+offset_To_Axis_કિંમત_Offsets))));
  }

  સુરક્ષિત:
  hb_array_t<અચલ StatAxisRecord> અચલ get_design_axes () અચલ
  { પાછા (આ+designAxesOffset).as_array (design_Axis_ગણતરી); }

  hb_array_t<અચલ Offset16To<Axis_કિંમત>> અચલ get_axis_કિંમત_offsets () અચલ
  { પાછા (આ+offset_To_Axis_કિંમત_Offsets).as_array (axis_કિંમત_ગણતરી); }


  સુરક્ષિત:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_ગણતરી;/* The number of design axis records. In a
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
  HBUINT16	axis_કિંમત_ગણતરી;	/* The number of axis value tables. */
  NNOffset32To<Axis_કિંમત_Offset_Array>
		offset_To_Axis_કિંમત_Offsets;
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
  જાહેર:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
