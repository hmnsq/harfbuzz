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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


ନାମାକାଶ OT {

ଗଣନା
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
  ELIDABLE_AXIS_ମୂଲ୍ୟ_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ସ୍ଥିର ତାର୍କିକ axis_ମୂଲ୍ୟ_is_outside_axis_range (hb_tag_t axis_tag, ଦଶମିକ axis_ମୂଲ୍ୟ,
                                              ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  ଯଦି (!user_axes_location->has (axis_tag))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  Triple axis_range = user_axes_location->get (axis_tag);
  ଫେରନ୍ତୁ (axis_ମୂଲ୍ୟ < axis_range.minimum || axis_ମୂଲ୍ୟ > axis_range.maximum);
}

ସଂରଚନା StatAxisRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t key) ଅଚଳ { ଫେରନ୍ତୁ tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ଅଚଳ { ଫେରନ୍ତୁ nameID; }

  hb_tag_t get_axis_tag () ଅଚଳ { ଫେରନ୍ତୁ tag; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି)));
  }

  ସୁରକ୍ଷିତ:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ_Format1
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_index () ଅଚଳ { ଫେରନ୍ତୁ axisIndex; }
  ଦଶମିକ get_ମୂଲ୍ୟ ()             ଅଚଳ { ଫେରନ୍ତୁ value.to_float (); }

  hb_ot_name_id_t get_ମୂଲ୍ୟ_name_id () ଅଚଳ { ଫେରନ୍ତୁ ମୂଲ୍ୟ_Name_ID; }

  hb_tag_t get_axis_tag (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    unsigned axis_idx = get_axis_index ();
    ଫେରନ୍ତୁ axis_records[axis_idx].get_axis_tag ();
  }

  ତାର୍କିକ keep_axis_ମୂଲ୍ୟ (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records,
                        ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ଅଚଳ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ଦଶମିକ axis_ମୂଲ୍ୟ = get_ମୂଲ୍ୟ ();

    ଫେରନ୍ତୁ !axis_ମୂଲ୍ୟ_is_outside_axis_range (axis_tag, axis_ମୂଲ୍ୟ, user_axes_location);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ଯଦି (keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ଏହି));

    return_trace (ମିଥ୍ୟା);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ମୂଲ୍ୟ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ_Format2
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_index () ଅଚଳ { ଫେରନ୍ତୁ axisIndex; }
  ଦଶମିକ get_ମୂଲ୍ୟ ()             ଅଚଳ { ଫେରନ୍ତୁ nominal_ମୂଲ୍ୟ.to_float (); }

  hb_ot_name_id_t get_ମୂଲ୍ୟ_name_id () ଅଚଳ { ଫେରନ୍ତୁ ମୂଲ୍ୟ_Name_ID; }

  hb_tag_t get_axis_tag (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    unsigned axis_idx = get_axis_index ();
    ଫେରନ୍ତୁ axis_records[axis_idx].get_axis_tag ();
  }

  ତାର୍କିକ keep_axis_ମୂଲ୍ୟ (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records,
                        ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ଅଚଳ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ଦଶମିକ axis_ମୂଲ୍ୟ = get_ମୂଲ୍ୟ ();

    ଫେରନ୍ତୁ !axis_ମୂଲ୍ୟ_is_outside_axis_range (axis_tag, axis_ମୂଲ୍ୟ, user_axes_location);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ଯଦି (keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ଏହି));

    return_trace (ମିଥ୍ୟା);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ମୂଲ୍ୟ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_ମୂଲ୍ୟ;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_ମୂଲ୍ୟ;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_ମୂଲ୍ୟ;	/* The maximum value for a range associated
				 * with the specified name ID. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (20);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ_Format3
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_index () ଅଚଳ { ଫେରନ୍ତୁ axisIndex; }
  ଦଶମିକ get_ମୂଲ୍ୟ ()             ଅଚଳ { ଫେରନ୍ତୁ value.to_float (); }

  hb_ot_name_id_t get_ମୂଲ୍ୟ_name_id () ଅଚଳ { ଫେରନ୍ତୁ ମୂଲ୍ୟ_Name_ID; }

  hb_tag_t get_axis_tag (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    unsigned axis_idx = get_axis_index ();
    ଫେରନ୍ତୁ axis_records[axis_idx].get_axis_tag ();
  }

  ତାର୍କିକ keep_axis_ମୂଲ୍ୟ (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records,
                        ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ଅଚଳ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ଦଶମିକ axis_ମୂଲ୍ୟ = get_ମୂଲ୍ୟ ();

    ଫେରନ୍ତୁ !axis_ମୂଲ୍ୟ_is_outside_axis_range (axis_tag, axis_ମୂଲ୍ୟ, user_axes_location);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ଯଦି (keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location))
      return_trace (c->serializer->embed (ଏହି));

    return_trace (ମିଥ୍ୟା);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ମୂଲ୍ୟ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_ମୂଲ୍ୟ;	/* The numeric value for a style-linked mapping
				 * from this value. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (16);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ_Record
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_index () ଅଚଳ { ଫେରନ୍ତୁ axisIndex; }
  ଦଶମିକ get_ମୂଲ୍ୟ ()             ଅଚଳ { ଫେରନ୍ତୁ value.to_float (); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ_Format4
{
  ଅଚଳ Axis_ମୂଲ୍ୟ_Record &get_axis_record (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_index) ଅଚଳ
  { ଫେରନ୍ତୁ axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2.as_array (axis_ଗଣନା_2)[axis_index]; }

  ତାର୍କିକ keep_axis_ମୂଲ୍ୟ (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records,
                        ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ଅଚଳ
  {
    hb_array_t<ଅଚଳ Axis_ମୂଲ୍ୟ_Record> axis_ମୂଲ୍ୟ_records = axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2.as_array (axis_ଗଣନା_2);

    ପାଇଁ (ଅଚଳ auto& rec : axis_ମୂଲ୍ୟ_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      ଦଶମିକ axis_ମୂଲ୍ୟ = rec.get_ମୂଲ୍ୟ ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      ଯଦି (axis_ମୂଲ୍ୟ_is_outside_axis_range (axis_tag, axis_ମୂଲ୍ୟ, user_axes_location))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    ଯଦି (!keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location))
      return_trace (ମିଥ୍ୟା);

    unsigned total_size = min_size + axis_ଗଣନା_2 * Axis_ମୂଲ୍ୟ_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_ମୂଲ୍ୟ_Format4> (total_size);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    hb_memcpy (out, ଏହି, total_size);
    return_trace (ସତ୍ୟ);
  }

  hb_ot_name_id_t get_ମୂଲ୍ୟ_name_id () ଅଚଳ { ଫେରନ୍ତୁ ମୂଲ୍ୟ_Name_ID; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
                          axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2.sanitize (c, axis_ଗଣନା_2)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_ଗଣନା_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ମୂଲ୍ୟ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_ମୂଲ୍ୟ_Record>
		axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, axis_ମୂଲ୍ୟଗୁଡ଼ିକ_2);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ
{
  ଦଶମିକ get_ମୂଲ୍ୟ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_index) ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_ମୂଲ୍ୟ ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_ମୂଲ୍ୟ ();
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_ମୂଲ୍ୟ ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_axis_record (axis_index).get_ମୂଲ୍ୟ ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0.f;
    }
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_index () ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_axis_index ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_axis_index ();
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ -1;
    }
  }

  hb_ot_name_id_t get_ମୂଲ୍ୟ_name_id () ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_ମୂଲ୍ୟ_name_id ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_ମୂଲ୍ୟ_name_id ();
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_ମୂଲ୍ୟ_name_id ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_ମୂଲ୍ୟ_name_id ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ keep_axis_ମୂଲ୍ୟ (ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ଅଚଳ
  {
    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.keep_axis_ମୂଲ୍ୟ (axis_records, user_axes_location);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ
  {
  ସଂରଚନା { HBUINT16 v; }	format;
  Axis_ମୂଲ୍ୟ_Format1	format1;
  Axis_ମୂଲ୍ୟ_Format2	format2;
  Axis_ମୂଲ୍ୟ_Format3	format3;
  Axis_ମୂଲ୍ୟ_Format4	format4;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};

ସଂରଚନା Axis_ମୂଲ୍ୟ_Offset_Array: UnsizedArrayOf<Offset16To<Axis_ମୂଲ୍ୟ>>
{
  ତାର୍କିକ subset (hb_subset_context_t *c,
               unsigned axis_ମୂଲ୍ୟ_ଗଣନା,
               unsigned& ଗଣନା_2,
               ଅଚଳ hb_array_t<ଅଚଳ StatAxisRecord> axis_records) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto axis_ମୂଲ୍ୟ_Offsets = as_array (axis_ମୂଲ୍ୟ_ଗଣନା);
    ଗଣନା_2 = 0;
    ପାଇଁ (ଅଚଳ auto& offset : axis_ମୂଲ୍ୟ_Offsets)
    {
      ଯଦି (!offset) ଜାରି;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      ଯଦି (!o) return_trace (ମିଥ୍ୟା);
      ଯଦି (!o->serialize_subset (c, offset, ଏହି, axis_records))
      {
        c->serializer->revert (o_snap);
        ଜାରି;
      }
      ଗଣନା_2++;
    }

    return_trace (ଗଣନା_2);
  }
};

ସଂରଚନା STAT
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  ତାର୍କିକ get_ମୂଲ୍ୟ (hb_tag_t tag, ଦଶମିକ *value) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_index;
    ଯଦି (!get_design_axes ().lfind (tag, &axis_index)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_array_t<ଅଚଳ Offset16To<Axis_ମୂଲ୍ୟ>> axis_ମୂଲ୍ୟଗୁଡ଼ିକ = get_axis_ମୂଲ୍ୟ_offsets ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < axis_ମୂଲ୍ୟଗୁଡ଼ିକ.length; i++)
    {
      ଅଚଳ Axis_ମୂଲ୍ୟ& axis_ମୂଲ୍ୟ = ଏହି+offset_To_Axis_ମୂଲ୍ୟ_Offsets+axis_ମୂଲ୍ୟଗୁଡ଼ିକ[i];
      ଯଦି (axis_ମୂଲ୍ୟ.get_axis_index () == axis_index)
      {
	ଯଦି (value)
	  *value = axis_ମୂଲ୍ୟ.get_ମୂଲ୍ୟ (axis_index);
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  unsigned get_design_axis_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ design_Axis_ଗଣନା; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ଅଚଳ
  {
    ଯଦି (unlikely (axis_record_index >= design_Axis_ଗଣନା)) ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    ଅଚଳ StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    ଫେରନ୍ତୁ axis_record.get_name_id ();
  }

  unsigned get_axis_ମୂଲ୍ୟ_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ axis_ମୂଲ୍ୟ_ଗଣନା; }

  hb_ot_name_id_t get_axis_ମୂଲ୍ୟ_name_id (unsigned axis_ମୂଲ୍ୟ_index) ଅଚଳ
  {
    ଯଦି (unlikely (axis_ମୂଲ୍ୟ_index >= axis_ମୂଲ୍ୟ_ଗଣନା)) ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    ଅଚଳ Axis_ମୂଲ୍ୟ &axis_ମୂଲ୍ୟ = (ଏହି + get_axis_ମୂଲ୍ୟ_offsets ()[axis_ମୂଲ୍ୟ_index]);
    ଫେରନ୍ତୁ axis_ମୂଲ୍ୟ.get_ମୂଲ୍ୟ_name_id ();
  }

  ଶୂନ୍ୟ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  {
    ଯଦି (!has_data ()) ଫେରନ୍ତୁ;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_ମୂଲ୍ୟ_offsets ()
    | hb_map (hb_add (&(ଏହି + offset_To_Axis_ମୂଲ୍ୟ_Offsets)))
    | hb_filter ([&] (ଅଚଳ Axis_ମୂଲ୍ୟ& _)
                 { ଫେରନ୍ତୁ _.keep_axis_ମୂଲ୍ୟ (designAxes, user_axes_location); })
    | hb_map (&Axis_ମୂଲ୍ୟ::get_ମୂଲ୍ୟ_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    STAT *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    auto designAxes = get_design_axes ();
    ପାଇଁ (unsigned i = 0; i < (unsigned)design_Axis_ଗଣନା; i++)
      ଯଦି (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (ମିଥ୍ୟା);

    ଯଦି (design_Axis_ଗଣନା)
      c->serializer->check_assign (out->designAxesOffset, ଏହି->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned ଗଣନା_2 = 0;
    out->offset_To_Axis_ମୂଲ୍ୟ_Offsets.serialize_subset (c, offset_To_Axis_ମୂଲ୍ୟ_Offsets, ଏହି,
                                                    axis_ମୂଲ୍ୟ_ଗଣନା, ଗଣନା_2, designAxes);
    return_trace (c->serializer->check_assign (out->axis_ମୂଲ୍ୟ_ଗଣନା, ଗଣନା_2, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, ଏହି, design_Axis_ଗଣନା) &&
			  offset_To_Axis_ମୂଲ୍ୟ_Offsets.sanitize (c, ଏହି, axis_ମୂଲ୍ୟ_ଗଣନା, &(ଏହି+offset_To_Axis_ମୂଲ୍ୟ_Offsets))));
  }

  ସୁରକ୍ଷିତ:
  hb_array_t<ଅଚଳ StatAxisRecord> ଅଚଳ get_design_axes () ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+designAxesOffset).as_array (design_Axis_ଗଣନା); }

  hb_array_t<ଅଚଳ Offset16To<Axis_ମୂଲ୍ୟ>> ଅଚଳ get_axis_ମୂଲ୍ୟ_offsets () ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+offset_To_Axis_ମୂଲ୍ୟ_Offsets).as_array (axis_ମୂଲ୍ୟ_ଗଣନା); }


  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_ଗଣନା;/* The number of design axis records. In a
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
  HBUINT16	axis_ମୂଲ୍ୟ_ଗଣନା;	/* The number of axis value tables. */
  NNOffset32To<Axis_ମୂଲ୍ୟ_Offset_Array>
		offset_To_Axis_ମୂଲ୍ୟ_Offsets;
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
