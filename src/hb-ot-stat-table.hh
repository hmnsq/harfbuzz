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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


ನಾಮಾಕಾಶ OT {

ಗಣನಾ
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
  ELIDABLE_AXIS_ಮೌಲ್ಯ_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ axis_ಮೌಲ್ಯ_is_outside_axis_range (hb_tag_t axis_tag, ಭಿನ್ನ axis_ಮೌಲ್ಯ,
                                              ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  ಯದಿ (!user_axes_location->has (axis_tag))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  Triple axis_range = user_axes_location->get (axis_tag);
  ಪ್ರತ್ಯಾಗಮ (axis_ಮೌಲ್ಯ < axis_range.minimum || axis_ಮೌಲ್ಯ > axis_range.maximum);
}

ಸಂರಚನಾ StatAxisRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t key) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ nameID; }

  hb_tag_t get_axis_tag () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Format1
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_index () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisIndex; }
  ಭಿನ್ನ get_ಮೌಲ್ಯ ()             ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ value.to_float (); }

  hb_ot_name_id_t get_ಮೌಲ್ಯ_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_Name_ID; }

  hb_tag_t get_axis_tag (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    unsigned axis_idx = get_axis_index ();
    ಪ್ರತ್ಯಾಗಮ axis_records[axis_idx].get_axis_tag ();
  }

  ತಾರ್ಕಿಕ keep_axis_ಮೌಲ್ಯ (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records,
                        ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ಧ್ರುವ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ಭಿನ್ನ axis_ಮೌಲ್ಯ = get_ಮೌಲ್ಯ ();

    ಪ್ರತ್ಯಾಗಮ !axis_ಮೌಲ್ಯ_is_outside_axis_range (axis_tag, axis_ಮೌಲ್ಯ, user_axes_location);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ಯದಿ (keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ಏತತ್));

    return_trace (ಅಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ಮೌಲ್ಯ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Format2
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_index () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisIndex; }
  ಭಿನ್ನ get_ಮೌಲ್ಯ ()             ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ nominal_ಮೌಲ್ಯ.to_float (); }

  hb_ot_name_id_t get_ಮೌಲ್ಯ_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_Name_ID; }

  hb_tag_t get_axis_tag (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    unsigned axis_idx = get_axis_index ();
    ಪ್ರತ್ಯಾಗಮ axis_records[axis_idx].get_axis_tag ();
  }

  ತಾರ್ಕಿಕ keep_axis_ಮೌಲ್ಯ (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records,
                        ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ಧ್ರುವ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ಭಿನ್ನ axis_ಮೌಲ್ಯ = get_ಮೌಲ್ಯ ();

    ಪ್ರತ್ಯಾಗಮ !axis_ಮೌಲ್ಯ_is_outside_axis_range (axis_tag, axis_ಮೌಲ್ಯ, user_axes_location);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ಯದಿ (keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ಏತತ್));

    return_trace (ಅಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ಮೌಲ್ಯ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_ಮೌಲ್ಯ;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_ಮೌಲ್ಯ;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_ಮೌಲ್ಯ;	/* The maximum value for a range associated
				 * with the specified name ID. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (20);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Format3
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_index () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisIndex; }
  ಭಿನ್ನ get_ಮೌಲ್ಯ ()             ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ value.to_float (); }

  hb_ot_name_id_t get_ಮೌಲ್ಯ_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_Name_ID; }

  hb_tag_t get_axis_tag (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    unsigned axis_idx = get_axis_index ();
    ಪ್ರತ್ಯಾಗಮ axis_records[axis_idx].get_axis_tag ();
  }

  ತಾರ್ಕಿಕ keep_axis_ಮೌಲ್ಯ (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records,
                        ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ಧ್ರುವ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ಭಿನ್ನ axis_ಮೌಲ್ಯ = get_ಮೌಲ್ಯ ();

    ಪ್ರತ್ಯಾಗಮ !axis_ಮೌಲ್ಯ_is_outside_axis_range (axis_tag, axis_ಮೌಲ್ಯ, user_axes_location);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ಯದಿ (keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ಏತತ್));

    return_trace (ಅಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ಮೌಲ್ಯ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_ಮೌಲ್ಯ;	/* The numeric value for a style-linked mapping
				 * from this value. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (16);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Record
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_index () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisIndex; }
  ಭಿನ್ನ get_ಮೌಲ್ಯ ()             ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ value.to_float (); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Format4
{
  ಧ್ರುವ Axis_ಮೌಲ್ಯ_Record &get_axis_record (unsigned ಪೂರ್ಣಾಙ್ಕ axis_index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ axis_ಮೌಲ್ಯಗಳು_2.as_array (axis_ಎಣಿಕೆ_2)[axis_index]; }

  ತಾರ್ಕಿಕ keep_axis_ಮೌಲ್ಯ (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records,
                        ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ಧ್ರುವ
  {
    hb_array_t<ಧ್ರುವ Axis_ಮೌಲ್ಯ_Record> axis_ಮೌಲ್ಯ_records = axis_ಮೌಲ್ಯಗಳು_2.as_array (axis_ಎಣಿಕೆ_2);

    ಪ್ರತಿ (ಧ್ರುವ auto& rec : axis_ಮೌಲ್ಯ_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      ಭಿನ್ನ axis_ಮೌಲ್ಯ = rec.get_ಮೌಲ್ಯ ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      ಯದಿ (axis_ಮೌಲ್ಯ_is_outside_axis_range (axis_tag, axis_ಮೌಲ್ಯ, user_axes_location))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    ಯದಿ (!keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location))
      return_trace (ಅಸತ್ಯ);

    unsigned total_size = min_size + axis_ಎಣಿಕೆ_2 * Axis_ಮೌಲ್ಯ_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_ಮೌಲ್ಯ_Format4> (total_size);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    hb_memcpy (out, ಏತತ್, total_size);
    return_trace (ಸತ್ಯ);
  }

  hb_ot_name_id_t get_ಮೌಲ್ಯ_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_Name_ID; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
                          axis_ಮೌಲ್ಯಗಳು_2.sanitize (c, axis_ಎಣಿಕೆ_2)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_ಎಣಿಕೆ_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ಮೌಲ್ಯ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_ಮೌಲ್ಯ_Record>
		axis_ಮೌಲ್ಯಗಳು_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, axis_ಮೌಲ್ಯಗಳು_2);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ
{
  ಭಿನ್ನ get_ಮೌಲ್ಯ (unsigned ಪೂರ್ಣಾಙ್ಕ axis_index) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_ಮೌಲ್ಯ ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_ಮೌಲ್ಯ ();
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_ಮೌಲ್ಯ ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_axis_record (axis_index).get_ಮೌಲ್ಯ ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0.f;
    }
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_index () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_axis_index ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_axis_index ();
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ -1;
    }
  }

  hb_ot_name_id_t get_ಮೌಲ್ಯ_name_id () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_ಮೌಲ್ಯ_name_id ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_ಮೌಲ್ಯ_name_id ();
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_ಮೌಲ್ಯ_name_id ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_ಮೌಲ್ಯ_name_id ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ keep_axis_ಮೌಲ್ಯ (ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.keep_axis_ಮೌಲ್ಯ (axis_records, user_axes_location);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ
  {
  ಸಂರಚನಾ { HBUINT16 v; }	format;
  Axis_ಮೌಲ್ಯ_Format1	format1;
  Axis_ಮೌಲ್ಯ_Format2	format2;
  Axis_ಮೌಲ್ಯ_Format3	format3;
  Axis_ಮೌಲ್ಯ_Format4	format4;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};

ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Offset_Array: UnsizedArrayOf<Offset16To<Axis_ಮೌಲ್ಯ>>
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               unsigned axis_ಮೌಲ್ಯ_ಎಣಿಕೆ,
               unsigned& ಎಣಿಕೆ,
               ಧ್ರುವ hb_array_t<ಧ್ರುವ StatAxisRecord> axis_records) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    auto axis_ಮೌಲ್ಯ_Offsets = as_array (axis_ಮೌಲ್ಯ_ಎಣಿಕೆ);
    ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& offset : axis_ಮೌಲ್ಯ_Offsets)
    {
      ಯದಿ (!offset) ಅನುವರ್ತ;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      ಯದಿ (!o) return_trace (ಅಸತ್ಯ);
      ಯದಿ (!o->serialize_subset (c, offset, ಏತತ್, axis_records))
      {
        c->serializer->revert (o_snap);
        ಅನುವರ್ತ;
      }
      ಎಣಿಕೆ++;
    }

    return_trace (ಎಣಿಕೆ);
  }
};

ಸಂರಚನಾ STAT
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  ತಾರ್ಕಿಕ get_ಮೌಲ್ಯ (hb_tag_t tag, ಭಿನ್ನ *value) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ axis_index;
    ಯದಿ (!get_design_axes ().lfind (tag, &axis_index)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_array_t<ಧ್ರುವ Offset16To<Axis_ಮೌಲ್ಯ>> axis_ಮೌಲ್ಯಗಳು = get_axis_ಮೌಲ್ಯ_offsets ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < axis_ಮೌಲ್ಯಗಳು.length; i++)
    {
      ಧ್ರುವ Axis_ಮೌಲ್ಯ& axis_ಮೌಲ್ಯ = ಏತತ್+offset_To_Axis_ಮೌಲ್ಯ_Offsets+axis_ಮೌಲ್ಯಗಳು[i];
      ಯದಿ (axis_ಮೌಲ್ಯ.get_axis_index () == axis_index)
      {
	ಯದಿ (value)
	  *value = axis_ಮೌಲ್ಯ.get_ಮೌಲ್ಯ (axis_index);
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  unsigned get_design_axis_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ design_Axis_ಎಣಿಕೆ; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ಧ್ರುವ
  {
    ಯದಿ (unlikely (axis_record_index >= design_Axis_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    ಧ್ರುವ StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    ಪ್ರತ್ಯಾಗಮ axis_record.get_name_id ();
  }

  unsigned get_axis_ಮೌಲ್ಯ_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axis_ಮೌಲ್ಯ_ಎಣಿಕೆ; }

  hb_ot_name_id_t get_axis_ಮೌಲ್ಯ_name_id (unsigned axis_ಮೌಲ್ಯ_index) ಧ್ರುವ
  {
    ಯದಿ (unlikely (axis_ಮೌಲ್ಯ_index >= axis_ಮೌಲ್ಯ_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    ಧ್ರುವ Axis_ಮೌಲ್ಯ &axis_ಮೌಲ್ಯ = (ಏತತ್ + get_axis_ಮೌಲ್ಯ_offsets ()[axis_ಮೌಲ್ಯ_index]);
    ಪ್ರತ್ಯಾಗಮ axis_ಮೌಲ್ಯ.get_ಮೌಲ್ಯ_name_id ();
  }

  ಶೂನ್ಯ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_ಮೌಲ್ಯ_offsets ()
    | hb_map (hb_add (&(ಏತತ್ + offset_To_Axis_ಮೌಲ್ಯ_Offsets)))
    | hb_filter ([&] (ಧ್ರುವ Axis_ಮೌಲ್ಯ& _)
                 { ಪ್ರತ್ಯಾಗಮ _.keep_axis_ಮೌಲ್ಯ (designAxes, user_axes_location); })
    | hb_map (&Axis_ಮೌಲ್ಯ::get_ಮೌಲ್ಯ_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    STAT *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    auto designAxes = get_design_axes ();
    ಪ್ರತಿ (unsigned i = 0; i < (unsigned)design_Axis_ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (ಅಸತ್ಯ);

    ಯದಿ (design_Axis_ಎಣಿಕೆ)
      c->serializer->check_assign (out->designAxesOffset, ಏತತ್->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned ಎಣಿಕೆ = 0;
    out->offset_To_Axis_ಮೌಲ್ಯ_Offsets.serialize_subset (c, offset_To_Axis_ಮೌಲ್ಯ_Offsets, ಏತತ್,
                                                    axis_ಮೌಲ್ಯ_ಎಣಿಕೆ, ಎಣಿಕೆ, designAxes);
    return_trace (c->serializer->check_assign (out->axis_ಮೌಲ್ಯ_ಎಣಿಕೆ, ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, ಏತತ್, design_Axis_ಎಣಿಕೆ) &&
			  offset_To_Axis_ಮೌಲ್ಯ_Offsets.sanitize (c, ಏತತ್, axis_ಮೌಲ್ಯ_ಎಣಿಕೆ, &(ಏತತ್+offset_To_Axis_ಮೌಲ್ಯ_Offsets))));
  }

  ರಕ್ಷಿತ:
  hb_array_t<ಧ್ರುವ StatAxisRecord> ಧ್ರುವ get_design_axes () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+designAxesOffset).as_array (design_Axis_ಎಣಿಕೆ); }

  hb_array_t<ಧ್ರುವ Offset16To<Axis_ಮೌಲ್ಯ>> ಧ್ರುವ get_axis_ಮೌಲ್ಯ_offsets () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+offset_To_Axis_ಮೌಲ್ಯ_Offsets).as_array (axis_ಮೌಲ್ಯ_ಎಣಿಕೆ); }


  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_ಎಣಿಕೆ;/* The number of design axis records. In a
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
  HBUINT16	axis_ಮೌಲ್ಯ_ಎಣಿಕೆ;	/* The number of axis value tables. */
  NNOffset32To<Axis_ಮೌಲ್ಯ_Offset_Array>
		offset_To_Axis_ಮೌಲ್ಯ_Offsets;
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
