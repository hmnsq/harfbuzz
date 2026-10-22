#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const ཧྲིལ> coords_)
  {
    length = coords_.length;
    གལ་སྲིད (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (ཧྲིལ));
    དེ་མིན
      dynamic_coords.extend (coords_);
  }

  ཧྲིལ& operator [] (unsigned idx)
  {
    གལ་སྲིད (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ལོག Crap(ཧྲིལ);

    གལ་སྲིད (length <= ARRAY_LENGTH (static_coords))
    {
      གལ་སྲིད (idx < ARRAY_LENGTH (static_coords))
      {
        སྐབས (length <= idx)
	  static_coords[length++] = 0;
	ལོག static_coords[idx];
      }
      དེ་མིན
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    གལ་སྲིད (dynamic_coords.length <= idx)
    {
      གལ་སྲིད (unlikely (!dynamic_coords.resize (idx + 1)))
	ལོག Crap(ཧྲིལ);
      length = idx + 1;
    }
    ལོག dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ཧྲིལ> get_coords ()
  { ལོག length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<ཧྲིལ> dynamic_coords;
  unsigned length;
  ཧྲིལ static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
