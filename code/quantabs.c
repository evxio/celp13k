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
#include "celp.h"
#include "coderate.h"
#include "quantize.h"
#include "cb.h"


int G_QUANT_TYPE[NUMMODES] = {LINEAR,LINEAR,LINEAR,LINEAR,LINEAR};

float MING[NUMMODES]={0.0,0.0,0.0,0.0,0.0};   /* -6 */
float MAXG[NUMMODES]={60.0,60.0,60.0,60.0,60.0}; /* 24 */
int NUMBER_OF_G_LEVELS[NUMMODES]={16, 16, 16, 16, 16};

int RELNUMBER_OF_G_LEVELS[NUMMODES]={8, 8, 8, 8, 8};

float GPRED_COEFF[GORDER] = {1.0, 1.0, 1.0};

/* For quantizing codebook Gain for EIGHTH rate frames */
int QG8[4] = {-4, -2, 0, 2};

int FG[73] = 
{  -2, -2, -2, -2, -1,  0,  0,  0,  1,  2,  3,  4,  5,
    6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 
   18, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 27, 28, 
   29, 30, 31, 32, 33, 34, 35, 36, 36, 37, 38, 39, 40, 
   41, 42, 43, 44, 45, 45, 46, 47, 48, 49, 50, 51, 52, 
   53, 54, 54, 55, 56, 57, 58, 58};

/* 10^(i/20) */
float GA[73] = 
{   0.5, 0.5, 0.625, 0.75, 0.75, 0.875, 1.0, 1.125, 1.25, 1.375, 1.625, 
    1.75, 2.0, 2.25, 2.5, 2.875, 3.125, 3.5, 4.0, 4.5, 5.0, 
    5.625, 6.25, 7.125, 8.0, 8.875, 10.0, 11.25, 12.625, 14.125, 
    15.875, 17.75, 20.0, 22.375, 25.125, 28.125, 31.625,
    35.5, 39.75, 44.625, 50.125, 56.25, 63.125, 70.75, 79.375,
    89.125, 100.0, 112.25, 125.875, 141.25, 158.5, 177.875, 199.5, 
    223.875, 251.25, 281.875, 316.25, 354.875, 398.125, 446.625,
    501.125, 562.375, 631.0, 708.0, 794.375, 891.25, 1000.0, 1122.0,
    1258.875, 1412.5, 1584.875, 1778.25, 1995.25};




/* Begin quantize */
int LSP_QTYPE[NUMMODES]       = {VQT, PRED, VQT, VQT, VQT};
float LSP_DPCM_DECAY[NUMMODES]= {0.0, 0.90625, 0.0, 0.0, 0.0};

float MIN_DELTA_LSP[NUMMODES][LPCORDER] =
{{ -.05, -.08, -.14, -.14, -.12, -.12, -.10, -.10, -.08, -.08 },
{-.02, -.02, -.02, -.02, -.02, -.02, -.02, -.02, -.02, -.02 },
{ -.05, -.08, -.14, -.14, -.12, -.12, -.10, -.10, -.08, -.08 },
{ -.05, -.08, -.14, -.14, -.12, -.12, -.10, -.10, -.08, -.08 }, 
{ -.05, -.08, -.14, -.14, -.12, -.12, -.10, -.10, -.08, -.08 }};

float MAX_DELTA_LSP[NUMMODES][LPCORDER] =
{{  .05,  .08,  .14,  .14,  .12,  .12,  .10,  .10,  .08,  .08 },
{ .02,  .02,  .02,  .02,  .02,  .02,  .02,  .02,  .02,  .02 },
{  .05,  .08,  .14,  .14,  .12,  .12,  .10,  .10,  .08,  .08 },
{  .05,  .08,  .14,  .14,  .12,  .12,  .10,  .10,  .08,  .08 },
{  .05,  .08,  .14,  .14,  .12,  .12,  .10,  .10,  .08,  .08 }};

int NUM_LSP_QLEVELS[NUMMODES][LPCORDER] =
{{  16, 16, 16, 16, 16, 16, 16, 16, 16, 16 }, 
{  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },
{  16, 16, 16, 16, 16, 16, 16, 16, 16, 16 }, 
{  16, 16, 16, 16, 16, 16, 16, 16, 16, 16 }, 
{  16, 16, 16, 16, 16, 16, 16, 16, 16, 16 }};

float LSPVQSIZE[5]={64, 128, 128, 64, 64};


float LSPVQ0[][2]= {
{  0.0327,  0.0118},
{  0.0919,  0.0111},
{  0.0427,  0.0440},
{  0.1327,  0.0185},
{  0.0469,  0.0050},
{  0.1272,  0.0091},
{  0.0892,  0.0059},
{  0.1771,  0.0193},
{  0.0222,  0.0158},
{  0.1100,  0.0127},
{  0.0827,  0.0055},
{  0.0978,  0.0791},
{  0.0665,  0.0047},
{  0.0700,  0.1401},
{  0.0670,  0.0859},
{  0.1913,  0.1048},
{  0.0471,  0.0215},
{  0.1046,  0.0125},
{  0.0645,  0.0298},
{  0.1599,  0.0160},
{  0.0593,  0.0039},
{  0.1187,  0.0462},
{  0.0749,  0.0341},
{  0.1520,  0.0511},
{  0.0290,  0.0792},
{  0.0909,  0.0362},
{  0.0753,  0.0081},
{  0.1111,  0.1058},
{  0.0519,  0.0253},
{  0.0828,  0.0839},
{  0.0685,  0.0541},
{  0.1421,  0.1258},
{  0.0386,  0.0130},
{  0.0962,  0.0119},
{  0.0542,  0.0387},
{  0.1431,  0.0185},
{  0.0526,  0.0051},
{  0.1175,  0.0260},
{  0.0831,  0.0167},
{  0.1728,  0.0510},
{  0.0273,  0.0437},
{  0.1172,  0.0113},
{  0.0771,  0.0144},
{  0.1122,  0.0751},
{  0.0619,  0.0119},
{  0.0492,  0.1276},
{  0.0658,  0.0695},
{  0.1882,  0.0615},
{  0.0415,  0.0200},
{  0.1018,  0.0088},
{  0.0681,  0.0339},
{  0.1436,  0.0325},
{  0.0555,  0.0122},
{  0.1042,  0.0485},
{  0.0826,  0.0345},
{  0.1374,  0.0743},
{  0.0383,  0.1018},
{  0.1005,  0.0358},
{  0.0704,  0.0086},
{  0.1301,  0.0586},
{  0.0597,  0.0241},
{  0.0832,  0.0621},
{  0.0555,  0.0573},
{  0.1504,  0.0839},
};


float LSPVQ1[][2]= {
{  0.0255,  0.0293},
{  0.0904,  0.0219},
{  0.0151,  0.1211},
{  0.1447,  0.0498},
{  0.0470,  0.0253},
{  0.1559,  0.0177},
{  0.1547,  0.0994},
{  0.2394,  0.0242},
{  0.0091,  0.0813},
{  0.0857,  0.0590},
{  0.0934,  0.1326},
{  0.1889,  0.0282},
{  0.0813,  0.0472},
{  0.1057,  0.1494},
{  0.0450,  0.3315},
{  0.2163,  0.1895},
{  0.0538,  0.0532},
{  0.1399,  0.0218},
{  0.0146,  0.1552},
{  0.1755,  0.0626},
{  0.0822,  0.0202},
{  0.1299,  0.0663},
{  0.0706,  0.1732},
{  0.2656,  0.0401},
{  0.0418,  0.0745},
{  0.0762,  0.1038},
{  0.0583,  0.1748},
{  0.1746,  0.1285},
{  0.0527,  0.1169},
{  0.1314,  0.0830},
{  0.0556,  0.2116},
{  0.1073,  0.2321},
{  0.0297,  0.0570},
{  0.0981,  0.0403},
{  0.0468,  0.1103},
{  0.1740,  0.0243},
{  0.0725,  0.0179},
{  0.1255,  0.0474},
{  0.1374,  0.1362},
{  0.1922,  0.0912},
{  0.0285,  0.0947},
{  0.0930,  0.0700},
{  0.0593,  0.1372},
{  0.1909,  0.0576},
{  0.0588,  0.0916},
{  0.1110,  0.1116},
{  0.0224,  0.2719},
{  0.1633,  0.2220},
{  0.0402,  0.0520},
{  0.1061,  0.0448},
{  0.0402,  0.1352},
{  0.1499,  0.0775},
{  0.0664,  0.0589},
{  0.1081,  0.0727},
{  0.0801,  0.2206},
{  0.2165,  0.1157},
{  0.0566,  0.0802},
{  0.0911,  0.1116},
{  0.0306,  0.1703},
{  0.1792,  0.0836},
{  0.0655,  0.0999},
{  0.1061,  0.1038},
{  0.0298,  0.2089},
{  0.1110,  0.1753},
{  0.0361,  0.0311},
{  0.0970,  0.0239},
{  0.0265,  0.1231},
{  0.1495,  0.0573},
{  0.0566,  0.0262},
{  0.1569,  0.0293},
{  0.1341,  0.1144},
{  0.2271,  0.0544},
{  0.0214,  0.0877},
{  0.0847,  0.0719},
{  0.0794,  0.1384},
{  0.2067,  0.0274},
{  0.0703,  0.0688},
{  0.1099,  0.1306},
{  0.0391,  0.2947},
{  0.2024,  0.1670},
{  0.0471,  0.0525},
{  0.1245,  0.0290},
{  0.0264,  0.1557},
{  0.1568,  0.0807},
{  0.0718,  0.0399},
{  0.1193,  0.0685},
{  0.0883,  0.1594},
{  0.2729,  0.0764},
{  0.0500,  0.0754},
{  0.0809,  0.1108},
{  0.0541,  0.1648},
{  0.1523,  0.1385},
{  0.0614,  0.1196},
{  0.1209,  0.0847},
{  0.0345,  0.2242},
{  0.1442,  0.1747},
{  0.0199,  0.0560},
{  0.1092,  0.0194},
{  0.0349,  0.1253},
{  0.1653,  0.0507},
{  0.0625,  0.0354},
{  0.1376,  0.0431},
{  0.1187,  0.1465},
{  0.2164,  0.0872},
{  0.0360,  0.0974},
{  0.1008,  0.0698},
{  0.0704,  0.1346},
{  0.2114,  0.0452},
{  0.0720,  0.0816},
{  0.1240,  0.1089},
{  0.0439,  0.2475},
{  0.1498,  0.2040},
{  0.0336,  0.0718},
{  0.1213,  0.0187},
{  0.0451,  0.1450},
{  0.1368,  0.0885},
{  0.0592,  0.0578},
{  0.1131,  0.0531},
{  0.0861,  0.1855},
{  0.1764,  0.1500},
{  0.0444,  0.0970},
{  0.0935,  0.0903},
{  0.0424,  0.1687},
{  0.1633,  0.1102},
{  0.0793,  0.0897},
{  0.1060,  0.0897},
{  0.0185,  0.2011},
{  0.1205,  0.1855},
};

float LSPVQ2[][2]= {
{  0.0255,  0.0283},
{  0.1296,  0.0355},
{  0.0543,  0.0343},
{  0.2073,  0.0274},
{  0.0204,  0.1099},
{  0.1562,  0.0523},
{  0.1388,  0.0161},
{  0.2784,  0.0274},
{  0.0112,  0.0849},
{  0.1870,  0.0175},
{  0.1189,  0.0160},
{  0.1490,  0.1088},
{  0.0969,  0.1115},
{  0.0659,  0.3322},
{  0.1158,  0.1073},
{  0.3183,  0.1363},
{  0.0517,  0.0223},
{  0.1740,  0.0223},
{  0.0704,  0.0387},
{  0.2637,  0.0234},
{  0.0692,  0.1005},
{  0.1287,  0.1610},
{  0.0952,  0.0532},
{  0.2393,  0.0646},
{  0.0490,  0.0552},
{  0.1619,  0.0657},
{  0.0845,  0.0670},
{  0.1784,  0.2280},
{  0.0191,  0.1775},
{  0.0272,  0.2868},
{  0.0942,  0.0952},
{  0.2628,  0.1479},
{  0.0278,  0.0579},
{  0.1565,  0.0218},
{  0.0814,  0.0180},
{  0.2379,  0.0187},
{  0.0276,  0.1444},
{  0.1199,  0.1223},
{  0.1200,  0.0349},
{  0.3009,  0.0307},
{  0.0312,  0.0844},
{  0.1898,  0.0306},
{  0.0863,  0.0470},
{  0.1685,  0.1241},
{  0.0513,  0.1727},
{  0.0711,  0.2233},
{  0.1085,  0.0864},
{  0.3398,  0.0527},
{  0.0414,  0.0440},
{  0.1356,  0.0612},
{  0.0964,  0.0147},
{  0.2173,  0.0738},
{  0.0465,  0.1292},
{  0.0877,  0.1749},
{  0.1104,  0.0689},
{  0.2105,  0.1311},
{  0.0580,  0.0864},
{  0.1895,  0.0752},
{  0.0652,  0.0609},
{  0.1485,  0.1699},
{  0.0514,  0.1400},
{  0.0386,  0.2131},
{  0.0933,  0.0798},
{  0.2473,  0.0986},
{  0.0334,  0.0360},
{  0.1375,  0.0398},
{  0.0621,  0.0276},
{  0.2183,  0.0280},
{  0.0311,  0.1114},
{  0.1382,  0.0807},
{  0.1284,  0.0175},
{  0.2605,  0.0636},
{  0.0230,  0.0816},
{  0.1739,  0.0408},
{  0.1074,  0.0176},
{  0.1619,  0.1120},
{  0.0784,  0.1371},
{  0.0448,  0.3050},
{  0.1189,  0.0880},
{  0.3039,  0.1165},
{  0.0424,  0.0241},
{  0.1672,  0.0186},
{  0.0815,  0.0333},
{  0.2432,  0.0324},
{  0.0584,  0.1029},
{  0.1137,  0.1546},
{  0.1015,  0.0585},
{  0.2198,  0.0995},
{  0.0574,  0.0581},
{  0.1746,  0.0647},
{  0.0733,  0.0740},
{  0.1938,  0.1737},
{  0.0347,  0.1710},
{  0.0373,  0.2429},
{  0.0787,  0.1061},
{  0.2439,  0.1438},
{  0.0185,  0.0536},
{  0.1489,  0.0178},
{  0.0703,  0.0216},
{  0.2178,  0.0482},
{  0.0154,  0.1421},
{  0.1414,  0.0994},
{  0.1103,  0.0352},
{  0.3072,  0.0473},
{  0.0408,  0.0819},
{  0.2055,  0.0168},
{  0.0998,  0.0354},
{  0.1917,  0.1140},
{  0.0665,  0.1799},
{  0.0993,  0.2213},
{  0.1234,  0.0631},
{  0.3003,  0.0762},
{  0.0373,  0.0620},
{  0.1518,  0.0425},
{  0.0913,  0.0300},
{  0.1966,  0.0836},
{  0.0402,  0.1185},
{  0.0948,  0.1385},
{  0.1121,  0.0555},
{  0.1802,  0.1509},
{  0.0474,  0.0886},
{  0.1888,  0.0610},
{  0.0739,  0.0585},
{  0.1231,  0.2379},
{  0.0661,  0.1335},
{  0.0205,  0.2211},
{  0.0823,  0.0822},
{  0.2480,  0.1179},
};

float LSPVQ3[][2]= {
{  0.0348,  0.0311},
{  0.0812,  0.1145},
{  0.0552,  0.0461},
{  0.1826,  0.0263},
{  0.0601,  0.0675},
{  0.1730,  0.0172},
{  0.1523,  0.0193},
{  0.2449,  0.0277},
{  0.0334,  0.0668},
{  0.0805,  0.1441},
{  0.1319,  0.0207},
{  0.1684,  0.0910},
{  0.0582,  0.1318},
{  0.1403,  0.1098},
{  0.0979,  0.0832},
{  0.2700,  0.1359},
{  0.0624,  0.0228},
{  0.1292,  0.0979},
{  0.0800,  0.0195},
{  0.2226,  0.0285},
{  0.0730,  0.0862},
{  0.1537,  0.0601},
{  0.1115,  0.0509},
{  0.2720,  0.0354},
{  0.0218,  0.1167},
{  0.1212,  0.1538},
{  0.1074,  0.0247},
{  0.1674,  0.1710},
{  0.0322,  0.2142},
{  0.1263,  0.0777},
{  0.0981,  0.0556},
{  0.2119,  0.1710},
{  0.0193,  0.0596},
{  0.1035,  0.0957},
{  0.0694,  0.0397},
{  0.1997,  0.0253},
{  0.0743,  0.0603},
{  0.1584,  0.0321},
{  0.1346,  0.0346},
{  0.2221,  0.0708},
{  0.0451,  0.0732},
{  0.1040,  0.1415},
{  0.1184,  0.0230},
{  0.1853,  0.0919},
{  0.0310,  0.1661},
{  0.1625,  0.0706},
{  0.0856,  0.0843},
{  0.2902,  0.0702},
{  0.0467,  0.0348},
{  0.1108,  0.1048},
{  0.0859,  0.0306},
{  0.1964,  0.0463},
{  0.0560,  0.1013},
{  0.1425,  0.0533},
{  0.1142,  0.0634},
{  0.2391,  0.0879},
{  0.0397,  0.1084},
{  0.1345,  0.1700},
{  0.0976,  0.0248},
{  0.1887,  0.1189},
{  0.0644,  0.2087},
{  0.1262,  0.0603},
{  0.0877,  0.0550},
{  0.2203,  0.1307},
};

float LSPVQ4[][2]= {
{  0.0360,  0.0222},
{  0.0820,  0.1097},
{  0.0601,  0.0319},
{  0.1656,  0.0198},
{  0.0604,  0.0513},
{  0.1552,  0.0141},
{  0.1391,  0.0155},
{  0.2474,  0.0261},
{  0.0269,  0.0785},
{  0.1463,  0.0646},
{  0.1123,  0.0191},
{  0.2015,  0.0223},
{  0.0785,  0.0844},
{  0.1202,  0.1011},
{  0.0980,  0.0807},
{  0.3014,  0.0793},
{  0.0570,  0.0180},
{  0.1135,  0.1382},
{  0.0778,  0.0256},
{  0.1901,  0.0179},
{  0.0807,  0.0622},
{  0.1461,  0.0458},
{  0.1231,  0.0178},
{  0.2028,  0.0821},
{  0.0387,  0.0927},
{  0.1496,  0.1004},
{  0.0888,  0.0392},
{  0.2246,  0.0341},
{  0.0295,  0.1462},
{  0.1156,  0.0694},
{  0.1022,  0.0473},
{  0.2226,  0.1364},
{  0.0210,  0.0478},
{  0.1029,  0.1020},
{  0.0722,  0.0181},
{  0.1731,  0.0251},
{  0.0730,  0.0488},
{  0.1465,  0.0293},
{  0.1303,  0.0326},
{  0.2595,  0.0387},
{  0.0458,  0.0584},
{  0.1569,  0.0742},
{  0.1029,  0.0173},
{  0.1910,  0.0495},
{  0.0605,  0.1159},
{  0.1268,  0.0719},
{  0.0973,  0.0646},
{  0.2872,  0.0428},
{  0.0443,  0.0334},
{  0.0835,  0.1465},
{  0.0912,  0.0138},
{  0.1716,  0.0442},
{  0.0620,  0.0778},
{  0.1316,  0.0450},
{  0.1186,  0.0335},
{  0.1446,  0.1665},
{  0.0486,  0.1050},
{  0.1675,  0.1019},
{  0.0880,  0.0278},
{  0.2214,  0.0202},
{  0.0539,  0.1564},
{  0.1142,  0.0533},
{  0.0984,  0.0391},
{  0.2130,  0.1089},
};
/* End quantize */