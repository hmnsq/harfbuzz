#अगर_अघोषित OT_VAR_VARC_COORD_SETTER_HH
#घोषणा OT_VAR_VARC_COORD_SETTER_HH


#समावेश "../../../hb.hh"


namespace OT {
//namespace Var {


काष्ठा coord_setter_t
{
  coord_setter_t (hb_array_t<स्थिर पूर्णांक> coords_)
  {
    लंबाई = coords_.लंबाई;
    अगर (लंबाई <= ARRAY_लंबाई (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, लंबाई * माप (पूर्णांक));
    अन्यथा
      dynamic_coords.extend (coords_);
  }

  पूर्णांक& चालक [] (अचिन्हित idx)
  {
    अगर (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      वापस Crap(पूर्णांक);

    अगर (लंबाई <= ARRAY_लंबाई (static_coords))
    {
      अगर (idx < ARRAY_लंबाई (static_coords))
      {
        जबतक (लंबाई <= idx)
	  static_coords[लंबाई++] = 0;
	वापस static_coords[idx];
      }
      अन्यथा
        dynamic_coords.extend (hb_array (static_coords, लंबाई));
    }

    अगर (dynamic_coords.लंबाई <= idx)
    {
      अगर (unlikely (!dynamic_coords.resize (idx + 1)))
	वापस Crap(पूर्णांक);
      लंबाई = idx + 1;
    }
    वापस dynamic_coords.arrayZ[idx];
  }

  hb_array_t<पूर्णांक> get_coords ()
  { वापस लंबाई <= ARRAY_लंबाई (static_coords) ? hb_array (static_coords, लंबाई) : dynamic_coords.as_array (); }

  निजी:
  hb_vector_t<पूर्णांक> dynamic_coords;
  अचिन्हित लंबाई;
  पूर्णांक static_coords[माप (व्योम *) * 8];
};


//} // namespace Var

} // namespace OT

#पूर्ण_अगर /* OT_VAR_VARC_COORD_SETTER_HH */
