#اگر_غیر_تعریف_شدہ OT_VAR_VARC_COORD_SETTER_HH
#تعریف_کرو OT_VAR_VARC_COORD_SETTER_HH


#شامل "../../../hb.hh"


نامخلا OT {
//namespace Var {


ساخت coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحیح> coords_)
  {
    لمبائی = coords_.لمبائی;
    اگر (لمبائی <= ARRAY_لمبائی (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, لمبائی * حجم (صحیح));
    ورنہ
      dynamic_coords.extend (coords_);
  }

  صحیح& عامل [] (غیرعلامتی idx)
  {
    اگر (unlikely (idx >= HB_VAR_مرکب_MAX_AXES))
      واپس Crap(صحیح);

    اگر (لمبائی <= ARRAY_لمبائی (static_coords))
    {
      اگر (idx < ARRAY_لمبائی (static_coords))
      {
        جب (لمبائی <= idx)
	  static_coords[لمبائی++] = 0;
	واپس static_coords[idx];
      }
      ورنہ
        dynamic_coords.extend (hb_array (static_coords, لمبائی));
    }

    اگر (dynamic_coords.لمبائی <= idx)
    {
      اگر (unlikely (!dynamic_coords.resize (idx + 1)))
	واپس Crap(صحیح);
      لمبائی = idx + 1;
    }
    واپس dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحیح> get_coords ()
  { واپس لمبائی <= ARRAY_لمبائی (static_coords) ? hb_array (static_coords, لمبائی) : dynamic_coords.as_array (); }

  نجی:
  hb_سمتیہ_t<صحیح> dynamic_coords;
  غیرعلامتی لمبائی;
  صحیح static_coords[حجم (خالی *) * 8];
};


//} // namespace Var

} // namespace OT

#اگر_ختم /* OT_VAR_VARC_COORD_SETTER_HH */
