#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#অন্তর্ভাব "../../../hb.hh"


নামাকাশ OT {
//namespace Var {


কাঠামো coord_setter_t
{
  coord_setter_t (hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords_)
  {
    দৈর্ঘ্য = coords_.দৈর্ঘ্য;
    যদি (দৈর্ঘ্য <= ARRAY_দৈর্ঘ্য (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, দৈর্ঘ্য * আকার (পূর্ণসংখ্যা));
    অন্যথা
      dynamic_coords.extend (coords_);
  }

  পূর্ণসংখ্যা& operator [] (unsigned idx)
  {
    যদি (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ফেরত Crap(পূর্ণসংখ্যা);

    যদি (দৈর্ঘ্য <= ARRAY_দৈর্ঘ্য (static_coords))
    {
      যদি (idx < ARRAY_দৈর্ঘ্য (static_coords))
      {
        যখন (দৈর্ঘ্য <= idx)
	  static_coords[দৈর্ঘ্য++] = 0;
	ফেরত static_coords[idx];
      }
      অন্যথা
        dynamic_coords.extend (hb_array (static_coords, দৈর্ঘ্য));
    }

    যদি (dynamic_coords.দৈর্ঘ্য <= idx)
    {
      যদি (unlikely (!dynamic_coords.resize (idx + 1)))
	ফেরত Crap(পূর্ণসংখ্যা);
      দৈর্ঘ্য = idx + 1;
    }
    ফেরত dynamic_coords.arrayZ[idx];
  }

  hb_array_t<পূর্ণসংখ্যা> get_coords ()
  { ফেরত দৈর্ঘ্য <= ARRAY_দৈর্ঘ্য (static_coords) ? hb_array (static_coords, দৈর্ঘ্য) : dynamic_coords.as_array (); }

  ব্যক্তিগত:
  hb_vector_t<পূর্ণসংখ্যা> dynamic_coords;
  unsigned দৈর্ঘ্য;
  পূর্ণসংখ্যা static_coords[আকার (শূন্য *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
