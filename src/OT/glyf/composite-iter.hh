#నిర్వచితం_కాకపోతే OT_GLYF_సంయుక్త_ITER_HH
#నిర్వచించు OT_GLYF_సంయుక్త_ITER_HH


#అన్తర్భావ "../../hb.hh"


నామాకాశ OT {
నామాకాశ glyf_impl {


మూస <రకం_పేరు సంయుక్త_Glyph_రికార్డు>
నిర్మాణం సంయుక్త_iter_tmpl : hb_iter_with_fallback_t<సంయుక్త_iter_tmpl<సంయుక్త_Glyph_రికార్డు>,
						     స్థిరరాశి సంయుక్త_Glyph_రికార్డు &>
{
  రకనిర్వచనం స్థిరరాశి సంయుక్త_Glyph_రికార్డు *__item_t__;
  సంయుక్త_iter_tmpl (hb_బైట్లు_t glyph_, __item_t__ current_, తార్కిక extended_) :
      glyph (glyph_), current (శూన్యసూచిక), current_కొలత (0), extended (extended_)
  {
    సమితి_current (current_);
  }

  సంయుక్త_iter_tmpl () : glyph (hb_బైట్లు_t ()), current (శూన్యసూచిక), current_కొలత (0), extended (అబద్ధం) {}

  స్థిరరాశి సంయుక్త_Glyph_రికార్డు & __item__ () స్థిరరాశి { తిరిగి *current; }
  తార్కిక __more__ () స్థిరరాశి { తిరిగి current; }
  శూన్య __next__ ()
  {
    ఒకవేళ (!current->has_మరిన్ని ()) { current = శూన్యసూచిక; తిరిగి; }

    సమితి_current (&Struct_At_స్థానభ్రంశం<సంయుక్త_Glyph_రికార్డు> (current, current_కొలత));
  }
  సంయుక్త_iter_tmpl __end__ () స్థిరరాశి { తిరిగి సంయుక్త_iter_tmpl (); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి సంయుక్త_iter_tmpl& o) స్థిరరాశి
  { తిరిగి current != o.current; }


  శూన్య సమితి_current (__item_t__ current_)
  {
    ఒకవేళ (!glyph.check_range (current_, సంయుక్త_Glyph_రికార్డు::min_కొలత))
    {
      current = శూన్యసూచిక;
      current_కొలత = 0;
      తిరిగి;
    }
    గుర్తులేని కొలత = current_->get_కొలత (extended);
    ఒకవేళ (!glyph.check_range (current_, కొలత))
    {
      current = శూన్యసూచిక;
      current_కొలత = 0;
      తిరిగి;
    }

    current = current_;
    current_కొలత = కొలత;
  }

  అంతరంగ:
  hb_బైట్లు_t glyph;
  __item_t__ current;
  గుర్తులేని current_కొలత;
  తార్కిక extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#అయితే_ముగింపు /* OT_GLYF_COMPOSITE_ITER_HH */
