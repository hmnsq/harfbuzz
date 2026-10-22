#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#अन्तर्भाव "../../../hb.hh"


नामाकाश OT {
//namespace Var {


संरचना coord_setter_t
{
  coord_setter_t (hb_array_t<ध्रुव पूर्णाङ्क> coords_)
  {
    length = coords_.length;
    जइ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * परिमाण (पूर्णाङ्क));
    अण्णहा
      dynamic_coords.extend (coords_);
  }

  पूर्णाङ्क& operator [] (unsigned idx)
  {
    जइ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      प्रत्यागम Crap(पूर्णाङ्क);

    जइ (length <= ARRAY_LENGTH (static_coords))
    {
      जइ (idx < ARRAY_LENGTH (static_coords))
      {
        जाव (length <= idx)
	  static_coords[length++] = 0;
	प्रत्यागम static_coords[idx];
      }
      अण्णहा
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    जइ (dynamic_coords.length <= idx)
    {
      जइ (unlikely (!dynamic_coords.resize (idx + 1)))
	प्रत्यागम Crap(पूर्णाङ्क);
      length = idx + 1;
    }
    प्रत्यागम dynamic_coords.arrayZ[idx];
  }

  hb_array_t<पूर्णाङ्क> get_coords ()
  { प्रत्यागम length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  गुप्त:
  hb_vector_t<पूर्णाङ्क> dynamic_coords;
  unsigned length;
  पूर्णाङ्क static_coords[परिमाण (शून्य *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
