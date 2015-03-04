/*****************************************************************************
 * Filename : stage2.h                                                 
 *   
 * Description : The is the implementation file for the stage2 module. 
 *
 * Original Author : Amit Rao (arao@synopsys.com)
 *
 *****************************************************************************/

#include "systemc.h"
#include "stage2.h"

//definition of multdiv method
void stage2::multdiv()
{
  double a;
  double b;

  a = sum.read();
  b = diff.read();
  if( b == 0 )
      b = 5.0;
  
  prod.write(a*b);
  quot.write(a/b);

} // end of multdiv

