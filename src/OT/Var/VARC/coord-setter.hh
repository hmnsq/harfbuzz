#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#अन्तर्भाव "../../../hb.hh"


नामाकाश OT {
//namespace Var {


संरचना coord_setter_t
{
  coord_setter_t (hb_array_t<स्थिरांक पूर्णांक> coords_)
  {
    length = coords_.length;
    जर (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * आकार (पूर्णांक));
    अन्यथा
      dynamic_coords.extend (coords_);
  }

  पूर्णांक& चालक [] (अचिन्हित idx)
  {
    जर (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      परत Crap(पूर्णांक);

    जर (length <= ARRAY_LENGTH (static_coords))
    {
      जर (idx < ARRAY_LENGTH (static_coords))
      {
        पर्यंत (length <= idx)
	  static_coords[length++] = 0;
	परत static_coords[idx];
      }
      अन्यथा
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    जर (dynamic_coords.length <= idx)
    {
      जर (unlikely (!dynamic_coords.resize (idx + 1)))
	परत Crap(पूर्णांक);
      length = idx + 1;
    }
    परत dynamic_coords.arrayZ[idx];
  }

  hb_array_t<पूर्णांक> get_coords ()
  { परत length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  खाजगी:
  hb_vector_t<पूर्णांक> dynamic_coords;
  अचिन्हित length;
  पूर्णांक static_coords[आकार (शून्य *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
