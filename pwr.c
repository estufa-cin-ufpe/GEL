#include "pwr.h"

ADI_PWR_RESULT pwrSetup()
{
  ADI_PWR_RESULT eResult;

  if(ADI_PWR_SUCCESS !=(eResult=adi_pwr_Init()))
  {
    return eResult;
  }

  if(ADI_PWR_SUCCESS !=(eResult=adi_pwr_SetLFClockMux(ADI_CLOCK_MUX_LFCLK_LFXTAL)))
  {
    return eResult;
  }

  if(ADI_PWR_SUCCESS !=(eResult=adi_pwr_EnableClockSource(ADI_CLOCK_SOURCE_LFXTAL,true)))
  {
    return eResult;
  }

  if(ADI_PWR_SUCCESS !=(eResult=adi_pwr_SetClockDivider(ADI_CLOCK_HCLK,1)))
  {
    return eResult;
  }

  if(ADI_PWR_SUCCESS !=(eResult=adi_pwr_SetClockDivider(ADI_CLOCK_PCLK,1)))
  {
    return eResult;
  }

  return(eResult);
}
