#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#ಅನ್ತರ್ಭಾವ "../../../hb.hh"


ನಾಮಾಕಾಶ OT {
//namespace Var {


ಸಂರಚನಾ coord_setter_t
{
  coord_setter_t (hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords_)
  {
    length = coords_.length;
    ಯದಿ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ));
    ಅನ್ಯಥಾ
      dynamic_coords.extend (coords_);
  }

  ಪೂರ್ಣಾಙ್ಕ& operator [] (unsigned idx)
  {
    ಯದಿ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ಪ್ರತ್ಯಾಗಮ Crap(ಪೂರ್ಣಾಙ್ಕ);

    ಯದಿ (length <= ARRAY_LENGTH (static_coords))
    {
      ಯದಿ (idx < ARRAY_LENGTH (static_coords))
      {
        ಯಾವತ್ (length <= idx)
	  static_coords[length++] = 0;
	ಪ್ರತ್ಯಾಗಮ static_coords[idx];
      }
      ಅನ್ಯಥಾ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ಯದಿ (dynamic_coords.length <= idx)
    {
      ಯದಿ (unlikely (!dynamic_coords.resize (idx + 1)))
	ಪ್ರತ್ಯಾಗಮ Crap(ಪೂರ್ಣಾಙ್ಕ);
      length = idx + 1;
    }
    ಪ್ರತ್ಯಾಗಮ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ಪೂರ್ಣಾಙ್ಕ> get_coords ()
  { ಪ್ರತ್ಯಾಗಮ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  ಗುಪ್ತ:
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> dynamic_coords;
  unsigned length;
  ಪೂರ್ಣಾಙ್ಕ static_coords[ಪರಿಮಾಣ (ಶೂನ್ಯ *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
