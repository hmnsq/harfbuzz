#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#अन्तर्भाव "../../../hb.hh"


नामाकाश OT {
//namespace Var {


संरचना coord_setter_t
{
  coord_setter_t (hb_array_t<अचर पूर्णाङ्क> coords_)
  {
    length = coords_.length;
    यदि (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * परिमाण (पूर्णाङ्क));
    अन्यथा
      dynamic_coords.extend (coords_);
  }

  पूर्णाङ्क& चालक [] (अचिन्हित idx)
  {
    यदि (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      फर्काऊ Crap(पूर्णाङ्क);

    यदि (length <= ARRAY_LENGTH (static_coords))
    {
      यदि (idx < ARRAY_LENGTH (static_coords))
      {
        जबसम्म (length <= idx)
	  static_coords[length++] = 0;
	फर्काऊ static_coords[idx];
      }
      अन्यथा
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    यदि (dynamic_coords.length <= idx)
    {
      यदि (unlikely (!dynamic_coords.resize (idx + 1)))
	फर्काऊ Crap(पूर्णाङ्क);
      length = idx + 1;
    }
    फर्काऊ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<पूर्णाङ्क> get_coords ()
  { फर्काऊ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  गुप्त:
  hb_vector_t<पूर्णाङ्क> dynamic_coords;
  अचिन्हित length;
  पूर्णाङ्क static_coords[परिमाण (शून्य *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
