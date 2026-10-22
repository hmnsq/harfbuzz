#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#شامل "../../../hb.hh"


فضاینام OT {
//namespace Var {


ساختار coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحیح> coords_)
  {
    length = coords_.length;
    اگر (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * اندازه (صحیح));
    وگرنه
      dynamic_coords.extend (coords_);
  }

  صحیح& operator [] (unsigned idx)
  {
    اگر (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      بازگشت Crap(صحیح);

    اگر (length <= ARRAY_LENGTH (static_coords))
    {
      اگر (idx < ARRAY_LENGTH (static_coords))
      {
        وقتی (length <= idx)
	  static_coords[length++] = 0;
	بازگشت static_coords[idx];
      }
      وگرنه
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    اگر (dynamic_coords.length <= idx)
    {
      اگر (unlikely (!dynamic_coords.resize (idx + 1)))
	بازگشت Crap(صحیح);
      length = idx + 1;
    }
    بازگشت dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحیح> get_coords ()
  { بازگشت length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  خصوصی:
  hb_vector_t<صحیح> dynamic_coords;
  unsigned length;
  صحیح static_coords[اندازه (تهی *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
