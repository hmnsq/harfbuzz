#جے_بنا_تعریف OT_VAR_VARC_COORD_SETTER_HH
#تعریف_کرو OT_VAR_VARC_COORD_SETTER_HH


#ضمن "../../../hb.hh"


نطاق OT {
//namespace Var {


بنية coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحيح> coords_)
  {
    لمائی = coords_.لمائی;
    إذا (لمائی <= ARRAY_لمائی (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, لمائی * حجم (صحيح));
    وإلا
      dynamic_coords.extend (coords_);
  }

  صحيح& عامل [] (بنانشانی idx)
  {
    إذا (unlikely (idx >= HB_VAR_مرکب_MAX_AXES))
      أرجع Crap(صحيح);

    إذا (لمائی <= ARRAY_لمائی (static_coords))
    {
      إذا (idx < ARRAY_لمائی (static_coords))
      {
        طالما (لمائی <= idx)
	  static_coords[لمائی++] = 0;
	أرجع static_coords[idx];
      }
      وإلا
        dynamic_coords.extend (hb_array (static_coords, لمائی));
    }

    إذا (dynamic_coords.لمائی <= idx)
    {
      إذا (unlikely (!dynamic_coords.resize (idx + 1)))
	أرجع Crap(صحيح);
      لمائی = idx + 1;
    }
    أرجع dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { أرجع لمائی <= ARRAY_لمائی (static_coords) ? hb_array (static_coords, لمائی) : dynamic_coords.as_array (); }

  خاص:
  hb_سمتیہ_t<صحيح> dynamic_coords;
  بنانشانی لمائی;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#جے_انت /* OT_VAR_VARC_COORD_SETTER_HH */
