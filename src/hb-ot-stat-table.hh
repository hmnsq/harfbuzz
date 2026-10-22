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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


നാമാകാശ OT {

എണ്ണൽ
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
  ELIDABLE_AXIS_മൂല്യം_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

സ്ഥിതം തർക്കം axis_മൂല്യം_is_outside_axis_range (hb_tag_t axis_tag, ദശാംശം axis_മൂല്യം,
                                              സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  എങ്കിൽ (!user_axes_location->has (axis_tag))
    തിരികെ അസത്യം;

  Triple axis_range = user_axes_location->get (axis_tag);
  തിരികെ (axis_മൂല്യം < axis_range.minimum || axis_മൂല്യം > axis_range.maximum);
}

ഘടന StatAxisRecord
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t key) സ്ഥിരം { തിരികെ tag.cmp (key); }

  hb_ot_name_id_t get_name_id () സ്ഥിരം { തിരികെ nameID; }

  hb_tag_t get_axis_tag () സ്ഥിരം { തിരികെ tag; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്)));
  }

  സംരക്ഷിത:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന Axis_മൂല്യം_Format1
{
  unsigned പൂർണ്ണസംഖ്യ get_axis_index () സ്ഥിരം { തിരികെ axisIndex; }
  ദശാംശം get_മൂല്യം ()             സ്ഥിരം { തിരികെ value.to_float (); }

  hb_ot_name_id_t get_മൂല്യം_name_id () സ്ഥിരം { തിരികെ മൂല്യം_Name_ID; }

  hb_tag_t get_axis_tag (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    unsigned axis_idx = get_axis_index ();
    തിരികെ axis_records[axis_idx].get_axis_tag ();
  }

  തർക്കം keep_axis_മൂല്യം (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records,
                        സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) സ്ഥിരം
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ദശാംശം axis_മൂല്യം = get_മൂല്യം ();

    തിരികെ !axis_മൂല്യം_is_outside_axis_range (axis_tag, axis_മൂല്യം, user_axes_location);
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    എങ്കിൽ (keep_axis_മൂല്യം (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ഇത്));

    return_trace (അസത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	മൂല്യം_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

ഘടന Axis_മൂല്യം_Format2
{
  unsigned പൂർണ്ണസംഖ്യ get_axis_index () സ്ഥിരം { തിരികെ axisIndex; }
  ദശാംശം get_മൂല്യം ()             സ്ഥിരം { തിരികെ nominal_മൂല്യം.to_float (); }

  hb_ot_name_id_t get_മൂല്യം_name_id () സ്ഥിരം { തിരികെ മൂല്യം_Name_ID; }

  hb_tag_t get_axis_tag (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    unsigned axis_idx = get_axis_index ();
    തിരികെ axis_records[axis_idx].get_axis_tag ();
  }

  തർക്കം keep_axis_മൂല്യം (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records,
                        സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) സ്ഥിരം
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ദശാംശം axis_മൂല്യം = get_മൂല്യം ();

    തിരികെ !axis_മൂല്യം_is_outside_axis_range (axis_tag, axis_മൂല്യം, user_axes_location);
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    എങ്കിൽ (keep_axis_മൂല്യം (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ഇത്));

    return_trace (അസത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	മൂല്യം_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_മൂല്യം;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_മൂല്യം;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_മൂല്യം;	/* The maximum value for a range associated
				 * with the specified name ID. */
  പൊതു:
  DEFINE_SIZE_STATIC (20);
};

ഘടന Axis_മൂല്യം_Format3
{
  unsigned പൂർണ്ണസംഖ്യ get_axis_index () സ്ഥിരം { തിരികെ axisIndex; }
  ദശാംശം get_മൂല്യം ()             സ്ഥിരം { തിരികെ value.to_float (); }

  hb_ot_name_id_t get_മൂല്യം_name_id () സ്ഥിരം { തിരികെ മൂല്യം_Name_ID; }

  hb_tag_t get_axis_tag (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    unsigned axis_idx = get_axis_index ();
    തിരികെ axis_records[axis_idx].get_axis_tag ();
  }

  തർക്കം keep_axis_മൂല്യം (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records,
                        സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) സ്ഥിരം
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ദശാംശം axis_മൂല്യം = get_മൂല്യം ();

    തിരികെ !axis_മൂല്യം_is_outside_axis_range (axis_tag, axis_മൂല്യം, user_axes_location);
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    എങ്കിൽ (keep_axis_മൂല്യം (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ഇത്));

    return_trace (അസത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	മൂല്യം_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_മൂല്യം;	/* The numeric value for a style-linked mapping
				 * from this value. */
  പൊതു:
  DEFINE_SIZE_STATIC (16);
};

ഘടന Axis_മൂല്യം_Record
{
  unsigned പൂർണ്ണസംഖ്യ get_axis_index () സ്ഥിരം { തിരികെ axisIndex; }
  ദശാംശം get_മൂല്യം ()             സ്ഥിരം { തിരികെ value.to_float (); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന Axis_മൂല്യം_Format4
{
  സ്ഥിരം Axis_മൂല്യം_Record &get_axis_record (unsigned പൂർണ്ണസംഖ്യ axis_index) സ്ഥിരം
  { തിരികെ axis_മൂല്യങ്ങൾ_2.as_array (axis_എണ്ണം_2)[axis_index]; }

  തർക്കം keep_axis_മൂല്യം (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records,
                        സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) സ്ഥിരം
  {
    hb_array_t<സ്ഥിരം Axis_മൂല്യം_Record> axis_മൂല്യം_records = axis_മൂല്യങ്ങൾ_2.as_array (axis_എണ്ണം_2);

    വേണ്ടി (സ്ഥിരം auto& rec : axis_മൂല്യം_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      ദശാംശം axis_മൂല്യം = rec.get_മൂല്യം ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      എങ്കിൽ (axis_മൂല്യം_is_outside_axis_range (axis_tag, axis_മൂല്യം, user_axes_location))
        തിരികെ അസത്യം;
    }

    തിരികെ സത്യം;
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    എങ്കിൽ (!keep_axis_മൂല്യം (axis_records, user_axes_location))
      return_trace (അസത്യം);

    unsigned total_size = min_size + axis_എണ്ണം_2 * Axis_മൂല്യം_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_മൂല്യം_Format4> (total_size);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
    hb_memcpy (out, ഇത്, total_size);
    return_trace (സത്യം);
  }

  hb_ot_name_id_t get_മൂല്യം_name_id () സ്ഥിരം { തിരികെ മൂല്യം_Name_ID; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
                          axis_മൂല്യങ്ങൾ_2.sanitize (c, axis_എണ്ണം_2)));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_എണ്ണം_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	മൂല്യം_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_മൂല്യം_Record>
		axis_മൂല്യങ്ങൾ_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  പൊതു:
  DEFINE_SIZE_ARRAY (8, axis_മൂല്യങ്ങൾ_2);
};

ഘടന Axis_മൂല്യം
{
  ദശാംശം get_മൂല്യം (unsigned പൂർണ്ണസംഖ്യ axis_index) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_മൂല്യം ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_മൂല്യം ();
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_മൂല്യം ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_axis_record (axis_index).get_മൂല്യം ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0.f;
    }
  }

  unsigned പൂർണ്ണസംഖ്യ get_axis_index () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_axis_index ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_axis_index ();
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    സ്ഥിരസ്ഥിതി:തിരികെ -1;
    }
  }

  hb_ot_name_id_t get_മൂല്യം_name_id () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_മൂല്യം_name_id ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_മൂല്യം_name_id ();
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_മൂല്യം_name_id ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_മൂല്യം_name_id ();
    സ്ഥിരസ്ഥിതി:തിരികെ HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം keep_axis_മൂല്യം (സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.keep_axis_മൂല്യം (axis_records, user_axes_location);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.keep_axis_മൂല്യം (axis_records, user_axes_location);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.keep_axis_മൂല്യം (axis_records, user_axes_location);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.keep_axis_മൂല്യം (axis_records, user_axes_location);
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം
  {
  ഘടന { HBUINT16 v; }	format;
  Axis_മൂല്യം_Format1	format1;
  Axis_മൂല്യം_Format2	format2;
  Axis_മൂല്യം_Format3	format3;
  Axis_മൂല്യം_Format4	format4;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};

ഘടന Axis_മൂല്യം_Offset_Array: UnsizedArrayOf<Offset16To<Axis_മൂല്യം>>
{
  തർക്കം subset (hb_subset_context_t *c,
               unsigned axis_മൂല്യം_എണ്ണം,
               unsigned& എണ്ണം,
               സ്ഥിരം hb_array_t<സ്ഥിരം StatAxisRecord> axis_records) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto axis_മൂല്യം_Offsets = as_array (axis_മൂല്യം_എണ്ണം);
    എണ്ണം = 0;
    വേണ്ടി (സ്ഥിരം auto& offset : axis_മൂല്യം_Offsets)
    {
      എങ്കിൽ (!offset) തുടരുക;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      എങ്കിൽ (!o) return_trace (അസത്യം);
      എങ്കിൽ (!o->serialize_subset (c, offset, ഇത്, axis_records))
      {
        c->serializer->revert (o_snap);
        തുടരുക;
      }
      എണ്ണം++;
    }

    return_trace (എണ്ണം);
  }
};

ഘടന STAT
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  തർക്കം get_മൂല്യം (hb_tag_t tag, ദശാംശം *value) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ axis_index;
    എങ്കിൽ (!get_design_axes ().lfind (tag, &axis_index)) തിരികെ അസത്യം;

    hb_array_t<സ്ഥിരം Offset16To<Axis_മൂല്യം>> axis_മൂല്യങ്ങൾ = get_axis_മൂല്യം_offsets ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < axis_മൂല്യങ്ങൾ.length; i++)
    {
      സ്ഥിരം Axis_മൂല്യം& axis_മൂല്യം = ഇത്+offset_To_Axis_മൂല്യം_Offsets+axis_മൂല്യങ്ങൾ[i];
      എങ്കിൽ (axis_മൂല്യം.get_axis_index () == axis_index)
      {
	എങ്കിൽ (value)
	  *value = axis_മൂല്യം.get_മൂല്യം (axis_index);
	തിരികെ സത്യം;
      }
    }
    തിരികെ അസത്യം;
  }

  unsigned get_design_axis_എണ്ണം () സ്ഥിരം { തിരികെ design_Axis_എണ്ണം; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (axis_record_index >= design_Axis_എണ്ണം)) തിരികെ HB_OT_NAME_ID_INVALID;
    സ്ഥിരം StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    തിരികെ axis_record.get_name_id ();
  }

  unsigned get_axis_മൂല്യം_എണ്ണം () സ്ഥിരം { തിരികെ axis_മൂല്യം_എണ്ണം; }

  hb_ot_name_id_t get_axis_മൂല്യം_name_id (unsigned axis_മൂല്യം_index) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (axis_മൂല്യം_index >= axis_മൂല്യം_എണ്ണം)) തിരികെ HB_OT_NAME_ID_INVALID;
    സ്ഥിരം Axis_മൂല്യം &axis_മൂല്യം = (ഇത് + get_axis_മൂല്യം_offsets ()[axis_മൂല്യം_index]);
    തിരികെ axis_മൂല്യം.get_മൂല്യം_name_id ();
  }

  ശൂന്യം collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ()) തിരികെ;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_മൂല്യം_offsets ()
    | hb_map (hb_add (&(ഇത് + offset_To_Axis_മൂല്യം_Offsets)))
    | hb_filter ([&] (സ്ഥിരം Axis_മൂല്യം& _)
                 { തിരികെ _.keep_axis_മൂല്യം (designAxes, user_axes_location); })
    | hb_map (&Axis_മൂല്യം::get_മൂല്യം_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    STAT *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    auto designAxes = get_design_axes ();
    വേണ്ടി (unsigned i = 0; i < (unsigned)design_Axis_എണ്ണം; i++)
      എങ്കിൽ (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (അസത്യം);

    എങ്കിൽ (design_Axis_എണ്ണം)
      c->serializer->check_assign (out->designAxesOffset, ഇത്->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned എണ്ണം = 0;
    out->offset_To_Axis_മൂല്യം_Offsets.serialize_subset (c, offset_To_Axis_മൂല്യം_Offsets, ഇത്,
                                                    axis_മൂല്യം_എണ്ണം, എണ്ണം, designAxes);
    return_trace (c->serializer->check_assign (out->axis_മൂല്യം_എണ്ണം, എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, ഇത്, design_Axis_എണ്ണം) &&
			  offset_To_Axis_മൂല്യം_Offsets.sanitize (c, ഇത്, axis_മൂല്യം_എണ്ണം, &(ഇത്+offset_To_Axis_മൂല്യം_Offsets))));
  }

  സംരക്ഷിത:
  hb_array_t<സ്ഥിരം StatAxisRecord> സ്ഥിരം get_design_axes () സ്ഥിരം
  { തിരികെ (ഇത്+designAxesOffset).as_array (design_Axis_എണ്ണം); }

  hb_array_t<സ്ഥിരം Offset16To<Axis_മൂല്യം>> സ്ഥിരം get_axis_മൂല്യം_offsets () സ്ഥിരം
  { തിരികെ (ഇത്+offset_To_Axis_മൂല്യം_Offsets).as_array (axis_മൂല്യം_എണ്ണം); }


  സംരക്ഷിത:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_എണ്ണം;/* The number of design axis records. In a
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
  HBUINT16	axis_മൂല്യം_എണ്ണം;	/* The number of axis value tables. */
  NNOffset32To<Axis_മൂല്യം_Offset_Array>
		offset_To_Axis_മൂല്യം_Offsets;
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
  പൊതു:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
