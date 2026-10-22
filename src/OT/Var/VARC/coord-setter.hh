#جيڪڏهن_وصف_نه_ٿيل OT_VAR_VARC_COORD_SETTER_HH
#وصف_ڪريو OT_VAR_VARC_COORD_SETTER_HH


#ضمن "../../../hb.hh"


نطاق OT {
//namespace Var {


بنية coord_setter_t
{
  coord_setter_t (hb_array_t<ثابت صحيح> coords_)
  {
    ڊيگهائي = coords_.ڊيگهائي;
    إذا (ڊيگهائي <= ARRAY_ڊيگهائي (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, ڊيگهائي * حجم (صحيح));
    وإلا
      dynamic_coords.extend (coords_);
  }

  صحيح& عامل [] (بنانشان idx)
  {
    إذا (unlikely (idx >= HB_VAR_مرکب_MAX_AXES))
      أرجع Crap(صحيح);

    إذا (ڊيگهائي <= ARRAY_ڊيگهائي (static_coords))
    {
      إذا (idx < ARRAY_ڊيگهائي (static_coords))
      {
        طالما (ڊيگهائي <= idx)
	  static_coords[ڊيگهائي++] = 0;
	أرجع static_coords[idx];
      }
      وإلا
        dynamic_coords.extend (hb_array (static_coords, ڊيگهائي));
    }

    إذا (dynamic_coords.ڊيگهائي <= idx)
    {
      إذا (unlikely (!dynamic_coords.resize (idx + 1)))
	أرجع Crap(صحيح);
      ڊيگهائي = idx + 1;
    }
    أرجع dynamic_coords.arrayZ[idx];
  }

  hb_array_t<صحيح> get_coords ()
  { أرجع ڊيگهائي <= ARRAY_ڊيگهائي (static_coords) ? hb_array (static_coords, ڊيگهائي) : dynamic_coords.as_array (); }

  خاص:
  hb_ويڪٽر_t<صحيح> dynamic_coords;
  بنانشان ڊيگهائي;
  صحيح static_coords[حجم (فراغ *) * 8];
};


//} // namespace Var

} // namespace OT

#جيڪڏهن_پورو /* OT_VAR_VARC_COORD_SETTER_HH */
