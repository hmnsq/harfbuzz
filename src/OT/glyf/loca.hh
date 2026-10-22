#నిర్వచితం_కాకపోతే OT_GLYF_LOCA_HH
#నిర్వచించు OT_GLYF_LOCA_HH


#అన్తర్భావ "../../hb-open-type.hh"


నామాకాశ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#నిర్వచించు HB_OT_ట్యాగ్_loca HB_ట్యాగ్('l','o','c','a')
#నిర్వచించు HB_OT_ట్యాగ్_LOCA HB_ట్యాగ్('L','O','C','A')

నిర్మాణం loca
{
  మిత్ర నిర్మాణం glyf;
  మిత్ర నిర్మాణం glyf_accelerator_t;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_loca;

  తార్కిక sanitize (hb_sanitize_context_t *c HB_వాడని) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (నిజం);
  }

  రక్షిత:
  Unsized_Array_యొక్క<HBUINT8>
		దత్తాంశం_Z;	/* Location data. */
  బహిరంగ:
  DEFINE_కొలత_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_LOCA_HH */
