#నిర్వచితం_కాకపోతే OT_VAR_VARC_COORD_SETTER_HH
#నిర్వచించు OT_VAR_VARC_COORD_SETTER_HH


#అన్తర్భావ "../../../hb.hh"


నామాకాశ OT {
//namespace Var {


నిర్మాణం coord_setter_t
{
  coord_setter_t (hb_array_t<స్థిరరాశి పూర్ణాంకం> coords_)
  {
    నిడివి = coords_.నిడివి;
    ఒకవేళ (నిడివి <= ARRAY_నిడివి (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, నిడివి * పరిమాణం (పూర్ణాంకం));
    లేకపోతే
      dynamic_coords.extend (coords_);
  }

  పూర్ణాంకం& క్రియాకారకం [] (గుర్తులేని idx)
  {
    ఒకవేళ (unlikely (idx >= HB_VAR_సంయుక్త_MAX_AXES))
      తిరిగి Crap(పూర్ణాంకం);

    ఒకవేళ (నిడివి <= ARRAY_నిడివి (static_coords))
    {
      ఒకవేళ (idx < ARRAY_నిడివి (static_coords))
      {
        అయితే (నిడివి <= idx)
	  static_coords[నిడివి++] = 0;
	తిరిగి static_coords[idx];
      }
      లేకపోతే
        dynamic_coords.extend (hb_array (static_coords, నిడివి));
    }

    ఒకవేళ (dynamic_coords.నిడివి <= idx)
    {
      ఒకవేళ (unlikely (!dynamic_coords.resize (idx + 1)))
	తిరిగి Crap(పూర్ణాంకం);
      నిడివి = idx + 1;
    }
    తిరిగి dynamic_coords.arrayZ[idx];
  }

  hb_array_t<పూర్ణాంకం> get_coords ()
  { తిరిగి నిడివి <= ARRAY_నిడివి (static_coords) ? hb_array (static_coords, నిడివి) : dynamic_coords.as_array (); }

  అంతరంగ:
  hb_సదిశ_t<పూర్ణాంకం> dynamic_coords;
  గుర్తులేని నిడివి;
  పూర్ణాంకం static_coords[పరిమాణం (శూన్య *) * 8];
};


//} // namespace Var

} // namespace OT

#అయితే_ముగింపు /* OT_VAR_VARC_COORD_SETTER_HH */
