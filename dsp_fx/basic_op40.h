/**********************************************************************
Each of the companies; Qualcomm, and Lucent (hereinafter 
referred to individually as "Source" or collectively as "Sources") do 
hereby state:

To the extent to which the Source(s) may legally and freely do so, the 
Source(s), upon submission of a Contribution, grant(s) a free, 
irrevocable, non-exclusive, license to the Third Generation Partnership 
Project 2 (3GPP2) and its Organizational Partners: ARIB, CCSA, TIA, TTA, 
and TTC, under the Source's copyright or copyright license rights in the 
Contribution, to, in whole or in part, copy, make derivative works, 
perform, display and distribute the Contribution and derivative works 
thereof consistent with 3GPP2's and each Organizational Partner's 
policies and procedures, with the right to (i) sublicense the foregoing 
rights consistent with 3GPP2's and each Organizational Partner's  policies 
and procedures and (ii) copyright and sell, if applicable) in 3GPP2's name 
or each Organizational Partner's name any 3GPP2 or transposed Publication 
even though this Publication may contain the Contribution or a derivative 
work thereof.  The Contribution shall disclose any known limitations on 
the Source's rights to license as herein provided.

When a Contribution is submitted by the Source(s) to assist the 
formulating groups of 3GPP2 or any of its Organizational Partners, it 
is proposed to the Committee as a basis for discussion and is not to 
be construed as a binding proposal on the Source(s).  The Source(s) 
specifically reserve(s) the right to amend or modify the material 
contained in the Contribution. Nothing contained in the Contribution 
shall, except as herein expressly provided, be construed as conferring 
by implication, estoppel or otherwise, any license or right under (i) 
any existing or later issuing patent, whether or not the use of 
information in the document necessarily employs an invention of any 
existing or later issued patent, (ii) any copyright, (iii) any 
trademark, or (iv) any other intellectual property right.

With respect to the Software necessary for the practice of any or 
all Normative portions of the QCELP-13 Variable Rate Speech Codec as 
it exists on the date of submittal of this form, should the QCELP-13 be 
approved as a Specification or Report by 3GPP2, or as a transposed 
Standard by any of the 3GPP2's Organizational Partners, the Source(s) 
state(s) that a worldwide license to reproduce, use and distribute the 
Software, the license rights to which are held by the Source(s), will 
be made available to applicants under terms and conditions that are 
reasonable and non-discriminatory, which may include monetary compensation, 
and only to the extent necessary for the practice of any or all of the 
Normative portions of the QCELP-13 or the field of use of practice of the 
QCELP-13 Specification, Report, or Standard.  The statement contained above 
is irrevocable and shall be binding upon the Source(s).  In the event 
the rights of the Source(s) in and to copyright or copyright license 
rights subject to such commitment are assigned or transferred, the 
Source(s) shall notify the assignee or transferee of the existence of 
such commitments.
*******************************************************************/
/*===================================================================*/
/* FILE: basic_op40.c                                                */
/*-------------------------------------------------------------------*/
/* PURPOSE : Modified TIA basic operations for 40-bit accumulators   */
/*===================================================================*/

/***********************************************************************
 *
 *
 *   DESCRIPTION:
 *     
 *     Modified TIA basic operations.  Bit-exact simulation of a
 *     generic 32 bit accumulator DSP chip (fractional math).  This
 *     version has a latching overflow bit (giOverflow) and
 *     non-compound MAC's (One where initial mult does not saturate)
 *     
 *     J. Klein, Conexant Systems, Inc.      11/30/00
 ***********************************************************************/

#ifndef __MATH40
#define __MATH40

#include "typedef_fx.h"

#define MAX_40 ((Word40) 549755813887.0)
#define MIN_40 ((Word40)-549755813888.0)

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

/*_________________________________________________________________________
 |                                                                         |
 |                            Function Prototypes                          |
 |_________________________________________________________________________|
*/

/* 40-bit instructions */
/*******************************/
Word40 L_add40(Word40 L_var1, Word32 L_var2);
Word40 L_sub40(Word40 L_var1, Word32 L_var2);
Word40 L_mac40(Word40 L_var3, Word16 var1, Word16 var2); 
Word40 L_msu40(Word40 L_var3, Word16 var1, Word16 var2); 
Word40 L_shr40(Word40 L_var1, Word16 var2);				   
Word40 L_shl40(Word40 L_var1, Word16 var2);				   
Word40 L_sat32_40(Word40 L_var1);							   
Word16 norm32_l40(Word40 L_var1);						Word16 extract_l40(Word40 L_var1);
Word40 L_deposit_l40(Word40 L_var1, Word16 var_in);
   

#endif

/*============================================================================*/
/*---------------------------------- END -------------------------------------*/
/*============================================================================*/
