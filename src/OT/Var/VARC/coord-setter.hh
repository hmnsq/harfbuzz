#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#অন্তর্ভাৱ "../../../hb.hh"


নামাকাশ OT {
//namespace Var {


সংরচনা coord_setter_t
{
  coord_setter_t (hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords_)
  {
    length = coords_.length;
    যদি (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * পরিমাণ (পূর্ণাঙ্ক));
    অন্যথা
      dynamic_coords.extend (coords_);
  }

  পূর্ণাঙ্ক& operator [] (unsigned idx)
  {
    যদি (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ঘূৰাই Crap(পূর্ণাঙ্ক);

    যদি (length <= ARRAY_LENGTH (static_coords))
    {
      যদি (idx < ARRAY_LENGTH (static_coords))
      {
        যেতিয়ালৈ (length <= idx)
	  static_coords[length++] = 0;
	ঘূৰাই static_coords[idx];
      }
      অন্যথা
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    যদি (dynamic_coords.length <= idx)
    {
      যদি (unlikely (!dynamic_coords.resize (idx + 1)))
	ঘূৰাই Crap(পূর্ণাঙ্ক);
      length = idx + 1;
    }
    ঘূৰাই dynamic_coords.arrayZ[idx];
  }

  hb_array_t<পূর্ণাঙ্ক> get_coords ()
  { ঘূৰাই length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  গুপ্ত:
  hb_vector_t<পূর্ণাঙ্ক> dynamic_coords;
  unsigned length;
  পূর্ণাঙ্ক static_coords[পরিমাণ (শূন্য *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
