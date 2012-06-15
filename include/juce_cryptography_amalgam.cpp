/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#define JUCE_AMALGAMATED_INCLUDE 1
#if !defined(__APPLE__) || defined(__OBJC__)

/*** Start of inlined file: juce_cryptography.cpp ***/
#if defined (__JUCE_CRYPTOGRAPHY_JUCEHEADER__) && ! JUCE_AMALGAMATED_INCLUDE
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
	already included any other headers - just put it inside a file on its own, possibly with your config
	flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
	header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"

#include "juce_cryptography_amalgam.h"

namespace juce
{

// START_AUTOINCLUDE encryption/*.cpp, hashing/*.cpp

/*** Start of inlined file: juce_BlowFish.cpp ***/
BlowFish::BlowFish (const void* const keyData, const int keyBytes)
{
	jassert (keyData != nullptr);
	jassert (keyBytes > 0);

	static const uint32 initialPValues [18] =
	{
		0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344, 0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
		0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c, 0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
		0x9216d5d9, 0x8979fb1b
	};

	static const uint32 initialSValues [4 * 256] =
	{
		0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7, 0xb8e1afed, 0x6a267e96, 0xba7c9045, 0xf12c7f99,
		0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16, 0x636920d8, 0x71574e69, 0xa458fea3, 0xf4933d7e,
		0x0d95748f, 0x728eb658, 0x718bcd58, 0x82154aee, 0x7b54a41d, 0xc25a59b5, 0x9c30d539, 0x2af26013,
		0xc5d1b023, 0x286085f0, 0xca417918, 0xb8db38ef, 0x8e79dcb0, 0x603a180e, 0x6c9e0e8b, 0xb01e8a3e,
		0xd71577c1, 0xbd314b27, 0x78af2fda, 0x55605c60, 0xe65525f3, 0xaa55ab94, 0x57489862, 0x63e81440,
		0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce, 0xa15486af, 0x7c72e993, 0xb3ee1411, 0x636fbc2a,
		0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e, 0xafd6ba33, 0x6c24cf5c, 0x7a325381, 0x28958677,
		0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193, 0x61d809cc, 0xfb21a991, 0x487cac60, 0x5dec8032,
		0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88, 0x23893e81, 0xd396acc5, 0x0f6d6ff3, 0x83f44239,
		0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e, 0x21c66842, 0xf6e96c9a, 0x670c9c61, 0xabd388f0,
		0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3, 0x6eef0b6c, 0x137a3be4, 0xba3bf050, 0x7efb2a98,
		0xa1f1651d, 0x39af0176, 0x66ca593e, 0x82430e88, 0x8cee8619, 0x456f9fb4, 0x7d84a5c3, 0x3b8b5ebe,
		0xe06f75d8, 0x85c12073, 0x401a449f, 0x56c16aa6, 0x4ed3aa62, 0x363f7706, 0x1bfedf72, 0x429b023d,
		0x37d0d724, 0xd00a1248, 0xdb0fead3, 0x49f1c09b, 0x075372c9, 0x80991b7b, 0x25d479d8, 0xf6e8def7,
		0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba, 0xc1a94fb6, 0x409f60c4, 0x5e5c9ec2, 0x196a2463,
		0x68fb6faf, 0x3e6c53b5, 0x1339b2eb, 0x3b52ec6f, 0x6dfc511f, 0x9b30952c, 0xcc814544, 0xaf5ebd09,
		0xbee3d004, 0xde334afd, 0x660f2807, 0x192e4bb3, 0xc0cba857, 0x45c8740f, 0xd20b5f39, 0xb9d3fbdb,
		0x5579c0bd, 0x1a60320a, 0xd6a100c6, 0x402c7279, 0x679f25fe, 0xfb1fa3cc, 0x8ea5e9f8, 0xdb3222f8,
		0x3c7516df, 0xfd616b15, 0x2f501ec8, 0xad0552ab, 0x323db5fa, 0xfd238760, 0x53317b48, 0x3e00df82,
		0x9e5c57bb, 0xca6f8ca0, 0x1a87562e, 0xdf1769db, 0xd542a8f6, 0x287effc3, 0xac6732c6, 0x8c4f5573,
		0x695b27b0, 0xbbca58c8, 0xe1ffa35d, 0xb8f011a0, 0x10fa3d98, 0xfd2183b8, 0x4afcb56c, 0x2dd1d35b,
		0x9a53e479, 0xb6f84565, 0xd28e49bc, 0x4bfb9790, 0xe1ddf2da, 0xa4cb7e33, 0x62fb1341, 0xcee4c6e8,
		0xef20cada, 0x36774c01, 0xd07e9efe, 0x2bf11fb4, 0x95dbda4d, 0xae909198, 0xeaad8e71, 0x6b93d5a0,
		0xd08ed1d0, 0xafc725e0, 0x8e3c5b2f, 0x8e7594b7, 0x8ff6e2fb, 0xf2122b64, 0x8888b812, 0x900df01c,
		0x4fad5ea0, 0x688fc31c, 0xd1cff191, 0xb3a8c1ad, 0x2f2f2218, 0xbe0e1777, 0xea752dfe, 0x8b021fa1,
		0xe5a0cc0f, 0xb56f74e8, 0x18acf3d6, 0xce89e299, 0xb4a84fe0, 0xfd13e0b7, 0x7cc43b81, 0xd2ada8d9,
		0x165fa266, 0x80957705, 0x93cc7314, 0x211a1477, 0xe6ad2065, 0x77b5fa86, 0xc75442f5, 0xfb9d35cf,
		0xebcdaf0c, 0x7b3e89a0, 0xd6411bd3, 0xae1e7e49, 0x00250e2d, 0x2071b35e, 0x226800bb, 0x57b8e0af,
		0x2464369b, 0xf009b91e, 0x5563911d, 0x59dfa6aa, 0x78c14389, 0xd95a537f, 0x207d5ba2, 0x02e5b9c5,
		0x83260376, 0x6295cfa9, 0x11c81968, 0x4e734a41, 0xb3472dca, 0x7b14a94a, 0x1b510052, 0x9a532915,
		0xd60f573f, 0xbc9bc6e4, 0x2b60a476, 0x81e67400, 0x08ba6fb5, 0x571be91f, 0xf296ec6b, 0x2a0dd915,
		0xb6636521, 0xe7b9f9b6, 0xff34052e, 0xc5855664, 0x53b02d5d, 0xa99f8fa1, 0x08ba4799, 0x6e85076a,
		0x4b7a70e9, 0xb5b32944, 0xdb75092e, 0xc4192623, 0xad6ea6b0, 0x49a7df7d, 0x9cee60b8, 0x8fedb266,
		0xecaa8c71, 0x699a17ff, 0x5664526c, 0xc2b19ee1, 0x193602a5, 0x75094c29, 0xa0591340, 0xe4183a3e,
		0x3f54989a, 0x5b429d65, 0x6b8fe4d6, 0x99f73fd6, 0xa1d29c07, 0xefe830f5, 0x4d2d38e6, 0xf0255dc1,
		0x4cdd2086, 0x8470eb26, 0x6382e9c6, 0x021ecc5e, 0x09686b3f, 0x3ebaefc9, 0x3c971814, 0x6b6a70a1,
		0x687f3584, 0x52a0e286, 0xb79c5305, 0xaa500737, 0x3e07841c, 0x7fdeae5c, 0x8e7d44ec, 0x5716f2b8,
		0xb03ada37, 0xf0500c0d, 0xf01c1f04, 0x0200b3ff, 0xae0cf51a, 0x3cb574b2, 0x25837a58, 0xdc0921bd,
		0xd19113f9, 0x7ca92ff6, 0x94324773, 0x22f54701, 0x3ae5e581, 0x37c2dadc, 0xc8b57634, 0x9af3dda7,
		0xa9446146, 0x0fd0030e, 0xecc8c73e, 0xa4751e41, 0xe238cd99, 0x3bea0e2f, 0x3280bba1, 0x183eb331,
		0x4e548b38, 0x4f6db908, 0x6f420d03, 0xf60a04bf, 0x2cb81290, 0x24977c79, 0x5679b072, 0xbcaf89af,
		0xde9a771f, 0xd9930810, 0xb38bae12, 0xdccf3f2e, 0x5512721f, 0x2e6b7124, 0x501adde6, 0x9f84cd87,
		0x7a584718, 0x7408da17, 0xbc9f9abc, 0xe94b7d8c, 0xec7aec3a, 0xdb851dfa, 0x63094366, 0xc464c3d2,
		0xef1c1847, 0x3215d908, 0xdd433b37, 0x24c2ba16, 0x12a14d43, 0x2a65c451, 0x50940002, 0x133ae4dd,
		0x71dff89e, 0x10314e55, 0x81ac77d6, 0x5f11199b, 0x043556f1, 0xd7a3c76b, 0x3c11183b, 0x5924a509,
		0xf28fe6ed, 0x97f1fbfa, 0x9ebabf2c, 0x1e153c6e, 0x86e34570, 0xeae96fb1, 0x860e5e0a, 0x5a3e2ab3,
		0x771fe71c, 0x4e3d06fa, 0x2965dcb9, 0x99e71d0f, 0x803e89d6, 0x5266c825, 0x2e4cc978, 0x9c10b36a,
		0xc6150eba, 0x94e2ea78, 0xa5fc3c53, 0x1e0a2df4, 0xf2f74ea7, 0x361d2b3d, 0x1939260f, 0x19c27960,
		0x5223a708, 0xf71312b6, 0xebadfe6e, 0xeac31f66, 0xe3bc4595, 0xa67bc883, 0xb17f37d1, 0x018cff28,
		0xc332ddef, 0xbe6c5aa5, 0x65582185, 0x68ab9802, 0xeecea50f, 0xdb2f953b, 0x2aef7dad, 0x5b6e2f84,
		0x1521b628, 0x29076170, 0xecdd4775, 0x619f1510, 0x13cca830, 0xeb61bd96, 0x0334fe1e, 0xaa0363cf,
		0xb5735c90, 0x4c70a239, 0xd59e9e0b, 0xcbaade14, 0xeecc86bc, 0x60622ca7, 0x9cab5cab, 0xb2f3846e,
		0x648b1eaf, 0x19bdf0ca, 0xa02369b9, 0x655abb50, 0x40685a32, 0x3c2ab4b3, 0x319ee9d5, 0xc021b8f7,
		0x9b540b19, 0x875fa099, 0x95f7997e, 0x623d7da8, 0xf837889a, 0x97e32d77, 0x11ed935f, 0x16681281,
		0x0e358829, 0xc7e61fd6, 0x96dedfa1, 0x7858ba99, 0x57f584a5, 0x1b227263, 0x9b83c3ff, 0x1ac24696,
		0xcdb30aeb, 0x532e3054, 0x8fd948e4, 0x6dbc3128, 0x58ebf2ef, 0x34c6ffea, 0xfe28ed61, 0xee7c3c73,
		0x5d4a14d9, 0xe864b7e3, 0x42105d14, 0x203e13e0, 0x45eee2b6, 0xa3aaabea, 0xdb6c4f15, 0xfacb4fd0,
		0xc742f442, 0xef6abbb5, 0x654f3b1d, 0x41cd2105, 0xd81e799e, 0x86854dc7, 0xe44b476a, 0x3d816250,
		0xcf62a1f2, 0x5b8d2646, 0xfc8883a0, 0xc1c7b6a3, 0x7f1524c3, 0x69cb7492, 0x47848a0b, 0x5692b285,
		0x095bbf00, 0xad19489d, 0x1462b174, 0x23820e00, 0x58428d2a, 0x0c55f5ea, 0x1dadf43e, 0x233f7061,
		0x3372f092, 0x8d937e41, 0xd65fecf1, 0x6c223bdb, 0x7cde3759, 0xcbee7460, 0x4085f2a7, 0xce77326e,
		0xa6078084, 0x19f8509e, 0xe8efd855, 0x61d99735, 0xa969a7aa, 0xc50c06c2, 0x5a04abfc, 0x800bcadc,
		0x9e447a2e, 0xc3453484, 0xfdd56705, 0x0e1e9ec9, 0xdb73dbd3, 0x105588cd, 0x675fda79, 0xe3674340,
		0xc5c43465, 0x713e38d8, 0x3d28f89e, 0xf16dff20, 0x153e21e7, 0x8fb03d4a, 0xe6e39f2b, 0xdb83adf7,
		0xe93d5a68, 0x948140f7, 0xf64c261c, 0x94692934, 0x411520f7, 0x7602d4f7, 0xbcf46b2e, 0xd4a20068,
		0xd4082471, 0x3320f46a, 0x43b7d4b7, 0x500061af, 0x1e39f62e, 0x97244546, 0x14214f74, 0xbf8b8840,
		0x4d95fc1d, 0x96b591af, 0x70f4ddd3, 0x66a02f45, 0xbfbc09ec, 0x03bd9785, 0x7fac6dd0, 0x31cb8504,
		0x96eb27b3, 0x55fd3941, 0xda2547e6, 0xabca0a9a, 0x28507825, 0x530429f4, 0x0a2c86da, 0xe9b66dfb,
		0x68dc1462, 0xd7486900, 0x680ec0a4, 0x27a18dee, 0x4f3ffea2, 0xe887ad8c, 0xb58ce006, 0x7af4d6b6,
		0xaace1e7c, 0xd3375fec, 0xce78a399, 0x406b2a42, 0x20fe9e35, 0xd9f385b9, 0xee39d7ab, 0x3b124e8b,
		0x1dc9faf7, 0x4b6d1856, 0x26a36631, 0xeae397b2, 0x3a6efa74, 0xdd5b4332, 0x6841e7f7, 0xca7820fb,
		0xfb0af54e, 0xd8feb397, 0x454056ac, 0xba489527, 0x55533a3a, 0x20838d87, 0xfe6ba9b7, 0xd096954b,
		0x55a867bc, 0xa1159a58, 0xcca92963, 0x99e1db33, 0xa62a4a56, 0x3f3125f9, 0x5ef47e1c, 0x9029317c,
		0xfdf8e802, 0x04272f70, 0x80bb155c, 0x05282ce3, 0x95c11548, 0xe4c66d22, 0x48c1133f, 0xc70f86dc,
		0x07f9c9ee, 0x41041f0f, 0x404779a4, 0x5d886e17, 0x325f51eb, 0xd59bc0d1, 0xf2bcc18f, 0x41113564,
		0x257b7834, 0x602a9c60, 0xdff8e8a3, 0x1f636c1b, 0x0e12b4c2, 0x02e1329e, 0xaf664fd1, 0xcad18115,
		0x6b2395e0, 0x333e92e1, 0x3b240b62, 0xeebeb922, 0x85b2a20e, 0xe6ba0d99, 0xde720c8c, 0x2da2f728,
		0xd0127845, 0x95b794fd, 0x647d0862, 0xe7ccf5f0, 0x5449a36f, 0x877d48fa, 0xc39dfd27, 0xf33e8d1e,
		0x0a476341, 0x992eff74, 0x3a6f6eab, 0xf4f8fd37, 0xa812dc60, 0xa1ebddf8, 0x991be14c, 0xdb6e6b0d,
		0xc67b5510, 0x6d672c37, 0x2765d43b, 0xdcd0e804, 0xf1290dc7, 0xcc00ffa3, 0xb5390f92, 0x690fed0b,
		0x667b9ffb, 0xcedb7d9c, 0xa091cf0b, 0xd9155ea3, 0xbb132f88, 0x515bad24, 0x7b9479bf, 0x763bd6eb,
		0x37392eb3, 0xcc115979, 0x8026e297, 0xf42e312d, 0x6842ada7, 0xc66a2b3b, 0x12754ccc, 0x782ef11c,
		0x6a124237, 0xb79251e7, 0x06a1bbe6, 0x4bfb6350, 0x1a6b1018, 0x11caedfa, 0x3d25bdd8, 0xe2e1c3c9,
		0x44421659, 0x0a121386, 0xd90cec6e, 0xd5abea2a, 0x64af674e, 0xda86a85f, 0xbebfe988, 0x64e4c3fe,
		0x9dbc8057, 0xf0f7c086, 0x60787bf8, 0x6003604d, 0xd1fd8346, 0xf6381fb0, 0x7745ae04, 0xd736fccc,
		0x83426b33, 0xf01eab71, 0xb0804187, 0x3c005e5f, 0x77a057be, 0xbde8ae24, 0x55464299, 0xbf582e61,
		0x4e58f48f, 0xf2ddfda2, 0xf474ef38, 0x8789bdc2, 0x5366f9c3, 0xc8b38e74, 0xb475f255, 0x46fcd9b9,
		0x7aeb2661, 0x8b1ddf84, 0x846a0e79, 0x915f95e2, 0x466e598e, 0x20b45770, 0x8cd55591, 0xc902de4c,
		0xb90bace1, 0xbb8205d0, 0x11a86248, 0x7574a99e, 0xb77f19b6, 0xe0a9dc09, 0x662d09a1, 0xc4324633,
		0xe85a1f02, 0x09f0be8c, 0x4a99a025, 0x1d6efe10, 0x1ab93d1d, 0x0ba5a4df, 0xa186f20f, 0x2868f169,
		0xdcb7da83, 0x573906fe, 0xa1e2ce9b, 0x4fcd7f52, 0x50115e01, 0xa70683fa, 0xa002b5c4, 0x0de6d027,
		0x9af88c27, 0x773f8641, 0xc3604c06, 0x61a806b5, 0xf0177a28, 0xc0f586e0, 0x006058aa, 0x30dc7d62,
		0x11e69ed7, 0x2338ea63, 0x53c2dd94, 0xc2c21634, 0xbbcbee56, 0x90bcb6de, 0xebfc7da1, 0xce591d76,
		0x6f05e409, 0x4b7c0188, 0x39720a3d, 0x7c927c24, 0x86e3725f, 0x724d9db9, 0x1ac15bb4, 0xd39eb8fc,
		0xed545578, 0x08fca5b5, 0xd83d7cd3, 0x4dad0fc4, 0x1e50ef5e, 0xb161e6f8, 0xa28514d9, 0x6c51133c,
		0x6fd5c7e7, 0x56e14ec4, 0x362abfce, 0xddc6c837, 0xd79a3234, 0x92638212, 0x670efa8e, 0x406000e0,
		0x3a39ce37, 0xd3faf5cf, 0xabc27737, 0x5ac52d1b, 0x5cb0679e, 0x4fa33742, 0xd3822740, 0x99bc9bbe,
		0xd5118e9d, 0xbf0f7315, 0xd62d1c7e, 0xc700c47b, 0xb78c1b6b, 0x21a19045, 0xb26eb1be, 0x6a366eb4,
		0x5748ab2f, 0xbc946e79, 0xc6a376d2, 0x6549c2c8, 0x530ff8ee, 0x468dde7d, 0xd5730a1d, 0x4cd04dc6,
		0x2939bbdb, 0xa9ba4650, 0xac9526e8, 0xbe5ee304, 0xa1fad5f0, 0x6a2d519a, 0x63ef8ce2, 0x9a86ee22,
		0xc089c2b8, 0x43242ef6, 0xa51e03aa, 0x9cf2d0a4, 0x83c061ba, 0x9be96a4d, 0x8fe51550, 0xba645bd6,
		0x2826a2f9, 0xa73a3ae1, 0x4ba99586, 0xef5562e9, 0xc72fefd3, 0xf752f7da, 0x3f046f69, 0x77fa0a59,
		0x80e4a915, 0x87b08601, 0x9b09e6ad, 0x3b3ee593, 0xe990fd5a, 0x9e34d797, 0x2cf0b7d9, 0x022b8b51,
		0x96d5ac3a, 0x017da67d, 0xd1cf3ed6, 0x7c7d2d28, 0x1f9f25cf, 0xadf2b89b, 0x5ad6b472, 0x5a88f54c,
		0xe029ac71, 0xe019a5e6, 0x47b0acfd, 0xed93fa9b, 0xe8d3c48d, 0x283b57cc, 0xf8d56629, 0x79132e28,
		0x785f0191, 0xed756055, 0xf7960e44, 0xe3d35e8c, 0x15056dd4, 0x88f46dba, 0x03a16125, 0x0564f0bd,
		0xc3eb9e15, 0x3c9057a2, 0x97271aec, 0xa93a072a, 0x1b3f6d9b, 0x1e6321f5, 0xf59c66fb, 0x26dcf319,
		0x7533d928, 0xb155fdf5, 0x03563482, 0x8aba3cbb, 0x28517711, 0xc20ad9f8, 0xabcc5167, 0xccad925f,
		0x4de81751, 0x3830dc8e, 0x379d5862, 0x9320f991, 0xea7a90c2, 0xfb3e7bce, 0x5121ce64, 0x774fbe32,
		0xa8b6e37e, 0xc3293d46, 0x48de5369, 0x6413e680, 0xa2ae0810, 0xdd6db224, 0x69852dfd, 0x09072166,
		0xb39a460a, 0x6445c0dd, 0x586cdecf, 0x1c20c8ae, 0x5bbef7dd, 0x1b588d40, 0xccd2017f, 0x6bb4e3bb,
		0xdda26a7e, 0x3a59ff45, 0x3e350a44, 0xbcb4cdd5, 0x72eacea8, 0xfa6484bb, 0x8d6612ae, 0xbf3c6f47,
		0xd29be463, 0x542f5d9e, 0xaec2771b, 0xf64e6370, 0x740e0d8d, 0xe75b1357, 0xf8721671, 0xaf537d5d,
		0x4040cb08, 0x4eb4e2cc, 0x34d2466a, 0x0115af84, 0xe1b00428, 0x95983a1d, 0x06b89fb4, 0xce6ea048,
		0x6f3f3b82, 0x3520ab82, 0x011a1d4b, 0x277227f8, 0x611560b1, 0xe7933fdc, 0xbb3a792b, 0x344525bd,
		0xa08839e1, 0x51ce794b, 0x2f32c9b7, 0xa01fbac9, 0xe01cc87e, 0xbcc7d1f6, 0xcf0111c3, 0xa1e8aac7,
		0x1a908749, 0xd44fbd9a, 0xd0dadecb, 0xd50ada38, 0x0339c32a, 0xc6913667, 0x8df9317c, 0xe0b12b4f,
		0xf79e59b7, 0x43f5bb3a, 0xf2d519ff, 0x27d9459c, 0xbf97222c, 0x15e6fc2a, 0x0f91fc71, 0x9b941525,
		0xfae59361, 0xceb69ceb, 0xc2a86459, 0x12baa8d1, 0xb6c1075e, 0xe3056a0c, 0x10d25065, 0xcb03a442,
		0xe0ec6e0e, 0x1698db3b, 0x4c98a0be, 0x3278e964, 0x9f1f9532, 0xe0d392df, 0xd3a0342b, 0x8971f21e,
		0x1b0a7441, 0x4ba3348c, 0xc5be7120, 0xc37632d8, 0xdf359f8d, 0x9b992f2e, 0xe60b6f47, 0x0fe3f11d,
		0xe54cda54, 0x1edad891, 0xce6279cf, 0xcd3e7e6f, 0x1618b166, 0xfd2c1d05, 0x848fd2c5, 0xf6fb2299,
		0xf523f357, 0xa6327623, 0x93a83531, 0x56cccd02, 0xacf08162, 0x5a75ebb5, 0x6e163697, 0x88d273cc,
		0xde966292, 0x81b949d0, 0x4c50901b, 0x71c65614, 0xe6c6c7bd, 0x327a140a, 0x45e1d006, 0xc3f27b9a,
		0xc9aa53fd, 0x62a80f00, 0xbb25bfe2, 0x35bdd2f6, 0x71126905, 0xb2040222, 0xb6cbcf7c, 0xcd769c2b,
		0x53113ec0, 0x1640e3d3, 0x38abbd60, 0x2547adf0, 0xba38209c, 0xf746ce76, 0x77afa1c5, 0x20756060,
		0x85cbfe4e, 0x8ae88dd8, 0x7aaaf9b0, 0x4cf9aa7e, 0x1948c25c, 0x02fb8a8c, 0x01c36ae4, 0xd6ebe1f9,
		0x90d4f869, 0xa65cdea0, 0x3f09252d, 0xc208e69f, 0xb74e6132, 0xce77e25b, 0x578fdfe3, 0x3ac372e6
	};

	memcpy (p, initialPValues, sizeof (p));

	int i, j = 0;
	for (i = 4; --i >= 0;)
	{
		s[i].malloc (256);
		memcpy (s[i], initialSValues + i * 256, 256 * sizeof (uint32));
	}

	for (i = 0; i < 18; ++i)
	{
		uint32 d = 0;

		for (int k = 0; k < 4; ++k)
		{
			d = (d << 8) | static_cast <const uint8*> (keyData)[j];

			if (++j >= keyBytes)
				j = 0;
		}

		p[i] = initialPValues[i] ^ d;
	}

	uint32 l = 0, r = 0;

	for (i = 0; i < 18; i += 2)
	{
		encrypt (l, r);

		p[i] = l;
		p[i + 1] = r;
	}

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 256; j += 2)
		{
			encrypt (l, r);

			s[i][j] = l;
			s[i][j + 1] = r;
		}
	}
}

BlowFish::BlowFish (const BlowFish& other)
{
	for (int i = 4; --i >= 0;)
		s[i].malloc (256);

	operator= (other);
}

BlowFish& BlowFish::operator= (const BlowFish& other) noexcept
{
	memcpy (p, other.p, sizeof (p));

	for (int i = 4; --i >= 0;)
		memcpy (s[i], other.s[i], 256 * sizeof (uint32));

	return *this;
}

BlowFish::~BlowFish() noexcept {}

uint32 BlowFish::F (const uint32 x) const noexcept
{
	return ((s[0][(x >> 24) & 0xff] + s[1][(x >> 16) & 0xff])
				^ s[2][(x >> 8) & 0xff]) + s[3][x & 0xff];
}

void BlowFish::encrypt (uint32& data1, uint32& data2) const noexcept
{
	uint32 l = data1;
	uint32 r = data2;

	for (int i = 0; i < 16; ++i)
	{
		l ^= p[i];
		r ^= F(l);
		std::swap (l, r);
	}

	data1 = r ^ p[17];
	data2 = l ^ p[16];
}

void BlowFish::decrypt (uint32& data1, uint32& data2) const noexcept
{
	uint32 l = data1;
	uint32 r = data2;

	for (int i = 17; i > 1; --i)
	{
		l ^= p[i];
		r ^= F(l);
		std::swap (l, r);
	}

	data1 = r ^ p[0];
	data2 = l ^ p[1];
}

/*** End of inlined file: juce_BlowFish.cpp ***/



/*** Start of inlined file: juce_Primes.cpp ***/
namespace PrimesHelpers
{
	static void createSmallSieve (const int numBits, BigInteger& result)
	{
		result.setBit (numBits);
		result.clearBit (numBits); // to enlarge the array

		result.setBit (0);
		int n = 2;

		do
		{
			for (int i = n + n; i < numBits; i += n)
				result.setBit (i);

			n = result.findNextClearBit (n + 1);
		}
		while (n <= (numBits >> 1));
	}

	static void bigSieve (const BigInteger& base, const int numBits, BigInteger& result,
						  const BigInteger& smallSieve, const int smallSieveSize)
	{
		jassert (! base[0]); // must be even!

		result.setBit (numBits);
		result.clearBit (numBits);  // to enlarge the array

		int index = smallSieve.findNextClearBit (0);

		do
		{
			const unsigned int prime = ((unsigned int) index << 1) + 1;

			BigInteger r (base), remainder;
			r.divideBy (prime, remainder);

			unsigned int i = prime - remainder.getBitRangeAsInt (0, 32);

			if (r.isZero())
				i += prime;

			if ((i & 1) == 0)
				i += prime;

			i = (i - 1) >> 1;

			while (i < (unsigned int) numBits)
			{
				result.setBit ((int) i);
				i += prime;
			}

			index = smallSieve.findNextClearBit (index + 1);
		}
		while (index < smallSieveSize);
	}

	static bool findCandidate (const BigInteger& base, const BigInteger& sieve,
							   const int numBits, BigInteger& result, const int certainty)
	{
		for (int i = 0; i < numBits; ++i)
		{
			if (! sieve[i])
			{
				result = base + (unsigned int) ((i << 1) + 1);

				if (Primes::isProbablyPrime (result, certainty))
					return true;
			}
		}

		return false;
	}

	static bool passesMillerRabin (const BigInteger& n, int iterations)
	{
		const BigInteger one (1), two (2);
		const BigInteger nMinusOne (n - one);

		BigInteger d (nMinusOne);
		const int s = d.findNextSetBit (0);
		d >>= s;

		BigInteger smallPrimes;
		int numBitsInSmallPrimes = 0;

		for (;;)
		{
			numBitsInSmallPrimes += 256;
			createSmallSieve (numBitsInSmallPrimes, smallPrimes);

			const int numPrimesFound = numBitsInSmallPrimes - smallPrimes.countNumberOfSetBits();

			if (numPrimesFound > iterations + 1)
				break;
		}

		int smallPrime = 2;

		while (--iterations >= 0)
		{
			smallPrime = smallPrimes.findNextClearBit (smallPrime + 1);

			BigInteger r (smallPrime);
			r.exponentModulo (d, n);

			if (r != one && r != nMinusOne)
			{
				for (int j = 0; j < s; ++j)
				{
					r.exponentModulo (two, n);

					if (r == nMinusOne)
						break;
				}

				if (r != nMinusOne)
					return false;
			}
		}

		return true;
	}
}

BigInteger Primes::createProbablePrime (const int bitLength,
										const int certainty,
										const int* randomSeeds,
										int numRandomSeeds)
{
	using namespace PrimesHelpers;
	int defaultSeeds [16];

	if (numRandomSeeds <= 0)
	{
		randomSeeds = defaultSeeds;
		numRandomSeeds = numElementsInArray (defaultSeeds);
		Random r1, r2;

		for (int j = 10; --j >= 0;)
		{
			r1.setSeedRandomly();

			for (int i = numRandomSeeds; --i >= 0;)
				defaultSeeds[i] ^= r1.nextInt() ^ r2.nextInt();
		}
	}

	BigInteger smallSieve;
	const int smallSieveSize = 15000;
	createSmallSieve (smallSieveSize, smallSieve);

	BigInteger p;

	for (int i = numRandomSeeds; --i >= 0;)
	{
		BigInteger p2;

		Random r (randomSeeds[i]);
		r.fillBitsRandomly (p2, 0, bitLength);

		p ^= p2;
	}

	p.setBit (bitLength - 1);
	p.clearBit (0);

	const int searchLen = jmax (1024, (bitLength / 20) * 64);

	while (p.getHighestBit() < bitLength)
	{
		p += 2 * searchLen;

		BigInteger sieve;
		bigSieve (p, searchLen, sieve,
				  smallSieve, smallSieveSize);

		BigInteger candidate;

		if (findCandidate (p, sieve, searchLen, candidate, certainty))
			return candidate;
	}

	jassertfalse;
	return BigInteger();
}

bool Primes::isProbablyPrime (const BigInteger& number, const int certainty)
{
	using namespace PrimesHelpers;

	if (! number[0])
		return false;

	if (number.getHighestBit() <= 10)
	{
		const unsigned int num = number.getBitRangeAsInt (0, 10);

		for (unsigned int i = num / 2; --i > 1;)
			if (num % i == 0)
				return false;

		return true;
	}
	else
	{
		if (number.findGreatestCommonDivisor (2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23) != 1)
			return false;

		return passesMillerRabin (number, certainty);
	}
}

/*** End of inlined file: juce_Primes.cpp ***/


/*** Start of inlined file: juce_RSAKey.cpp ***/
RSAKey::RSAKey()
{
}

RSAKey::RSAKey (const String& s)
{
	if (s.containsChar (','))
	{
		part1.parseString (s.upToFirstOccurrenceOf (",", false, false), 16);
		part2.parseString (s.fromFirstOccurrenceOf (",", false, false), 16);
	}
	else
	{
		// the string needs to be two hex numbers, comma-separated..
		jassertfalse;
	}
}

RSAKey::~RSAKey()
{
}

bool RSAKey::operator== (const RSAKey& other) const noexcept
{
	return part1 == other.part1 && part2 == other.part2;
}

bool RSAKey::operator!= (const RSAKey& other) const noexcept
{
	return ! operator== (other);
}

String RSAKey::toString() const
{
	return part1.toString (16) + "," + part2.toString (16);
}

bool RSAKey::applyToValue (BigInteger& value) const
{
	if (part1.isZero() || part2.isZero() || value <= 0)
	{
		jassertfalse;   // using an uninitialised key
		value.clear();
		return false;
	}

	BigInteger result;

	while (! value.isZero())
	{
		result *= part2;

		BigInteger remainder;
		value.divideBy (part2, remainder);

		remainder.exponentModulo (part1, part2);

		result += remainder;
	}

	value.swapWith (result);
	return true;
}

BigInteger RSAKey::findBestCommonDivisor (const BigInteger& p, const BigInteger& q)
{
	// try 3, 5, 9, 17, etc first because these only contain 2 bits and so
	// are fast to divide + multiply
	for (int i = 2; i <= 65536; i *= 2)
	{
		const BigInteger e (1 + i);

		if (e.findGreatestCommonDivisor (p).isOne() && e.findGreatestCommonDivisor (q).isOne())
			return e;
	}

	BigInteger e (4);

	while (! (e.findGreatestCommonDivisor (p).isOne() && e.findGreatestCommonDivisor (q).isOne()))
		++e;

	return e;
}

void RSAKey::createKeyPair (RSAKey& publicKey, RSAKey& privateKey,
							const int numBits, const int* randomSeeds, const int numRandomSeeds)
{
	jassert (numBits > 16); // not much point using less than this..
	jassert (numRandomSeeds == 0 || numRandomSeeds >= 2); // you need to provide plenty of seeds here!

	BigInteger p (Primes::createProbablePrime (numBits / 2, 30, randomSeeds, numRandomSeeds / 2));
	BigInteger q (Primes::createProbablePrime (numBits - numBits / 2, 30, randomSeeds == nullptr ? 0 : (randomSeeds + numRandomSeeds / 2), numRandomSeeds - numRandomSeeds / 2));

	const BigInteger n (p * q);
	const BigInteger m (--p * --q);
	const BigInteger e (findBestCommonDivisor (p, q));

	BigInteger d (e);
	d.inverseModulo (m);

	publicKey.part1 = e;
	publicKey.part2 = n;

	privateKey.part1 = d;
	privateKey.part2 = n;
}

/*** End of inlined file: juce_RSAKey.cpp ***/


/*** Start of inlined file: juce_MD5.cpp ***/
class MD5Generator
{
public:
	MD5Generator() noexcept
	{
		state[0] = 0x67452301;
		state[1] = 0xefcdab89;
		state[2] = 0x98badcfe;
		state[3] = 0x10325476;

		count[0] = 0;
		count[1] = 0;
	}

	void processBlock (const void* data, size_t dataSize) noexcept
	{
		int bufferPos = ((count[0] >> 3) & 0x3F);

		count[0] += (uint32) (dataSize << 3);

		if (count[0] < ((uint32) dataSize << 3))
			count[1]++;

		count[1] += (uint32) (dataSize >> 29);

		const size_t spaceLeft = 64 - (size_t) bufferPos;
		size_t i = 0;

		if (dataSize >= spaceLeft)
		{
			memcpy (buffer + bufferPos, data, spaceLeft);
			transform (buffer);

			for (i = spaceLeft; i + 64 <= dataSize; i += 64)
				transform (static_cast <const char*> (data) + i);

			bufferPos = 0;
		}

		memcpy (buffer + bufferPos, static_cast <const char*> (data) + i, dataSize - i);
	}

	void transform (const void* bufferToTransform) noexcept
	{
		uint32 a = state[0];
		uint32 b = state[1];
		uint32 c = state[2];
		uint32 d = state[3];
		uint32 x[16];

		encode (x, bufferToTransform, 64);

		enum Constants
		{
			S11 = 7, S12 = 12, S13 = 17, S14 = 22, S21 = 5, S22 = 9,  S23 = 14, S24 = 20,
			S31 = 4, S32 = 11, S33 = 16, S34 = 23, S41 = 6, S42 = 10, S43 = 15, S44 = 21
		};

		FF (a, b, c, d, x[ 0], S11, 0xd76aa478);     FF (d, a, b, c, x[ 1], S12, 0xe8c7b756);
		FF (c, d, a, b, x[ 2], S13, 0x242070db);     FF (b, c, d, a, x[ 3], S14, 0xc1bdceee);
		FF (a, b, c, d, x[ 4], S11, 0xf57c0faf);     FF (d, a, b, c, x[ 5], S12, 0x4787c62a);
		FF (c, d, a, b, x[ 6], S13, 0xa8304613);     FF (b, c, d, a, x[ 7], S14, 0xfd469501);
		FF (a, b, c, d, x[ 8], S11, 0x698098d8);     FF (d, a, b, c, x[ 9], S12, 0x8b44f7af);
		FF (c, d, a, b, x[10], S13, 0xffff5bb1);     FF (b, c, d, a, x[11], S14, 0x895cd7be);
		FF (a, b, c, d, x[12], S11, 0x6b901122);     FF (d, a, b, c, x[13], S12, 0xfd987193);
		FF (c, d, a, b, x[14], S13, 0xa679438e);     FF (b, c, d, a, x[15], S14, 0x49b40821);

		GG (a, b, c, d, x[ 1], S21, 0xf61e2562);     GG (d, a, b, c, x[ 6], S22, 0xc040b340);
		GG (c, d, a, b, x[11], S23, 0x265e5a51);     GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
		GG (a, b, c, d, x[ 5], S21, 0xd62f105d);     GG (d, a, b, c, x[10], S22, 0x02441453);
		GG (c, d, a, b, x[15], S23, 0xd8a1e681);     GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
		GG (a, b, c, d, x[ 9], S21, 0x21e1cde6);     GG (d, a, b, c, x[14], S22, 0xc33707d6);
		GG (c, d, a, b, x[ 3], S23, 0xf4d50d87);     GG (b, c, d, a, x[ 8], S24, 0x455a14ed);
		GG (a, b, c, d, x[13], S21, 0xa9e3e905);     GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8);
		GG (c, d, a, b, x[ 7], S23, 0x676f02d9);     GG (b, c, d, a, x[12], S24, 0x8d2a4c8a);

		HH (a, b, c, d, x[ 5], S31, 0xfffa3942);     HH (d, a, b, c, x[ 8], S32, 0x8771f681);
		HH (c, d, a, b, x[11], S33, 0x6d9d6122);     HH (b, c, d, a, x[14], S34, 0xfde5380c);
		HH (a, b, c, d, x[ 1], S31, 0xa4beea44);     HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9);
		HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60);     HH (b, c, d, a, x[10], S34, 0xbebfbc70);
		HH (a, b, c, d, x[13], S31, 0x289b7ec6);     HH (d, a, b, c, x[ 0], S32, 0xeaa127fa);
		HH (c, d, a, b, x[ 3], S33, 0xd4ef3085);     HH (b, c, d, a, x[ 6], S34, 0x04881d05);
		HH (a, b, c, d, x[ 9], S31, 0xd9d4d039);     HH (d, a, b, c, x[12], S32, 0xe6db99e5);
		HH (c, d, a, b, x[15], S33, 0x1fa27cf8);     HH (b, c, d, a, x[ 2], S34, 0xc4ac5665);

		II (a, b, c, d, x[ 0], S41, 0xf4292244);     II (d, a, b, c, x[ 7], S42, 0x432aff97);
		II (c, d, a, b, x[14], S43, 0xab9423a7);     II (b, c, d, a, x[ 5], S44, 0xfc93a039);
		II (a, b, c, d, x[12], S41, 0x655b59c3);     II (d, a, b, c, x[ 3], S42, 0x8f0ccc92);
		II (c, d, a, b, x[10], S43, 0xffeff47d);     II (b, c, d, a, x[ 1], S44, 0x85845dd1);
		II (a, b, c, d, x[ 8], S41, 0x6fa87e4f);     II (d, a, b, c, x[15], S42, 0xfe2ce6e0);
		II (c, d, a, b, x[ 6], S43, 0xa3014314);     II (b, c, d, a, x[13], S44, 0x4e0811a1);
		II (a, b, c, d, x[ 4], S41, 0xf7537e82);     II (d, a, b, c, x[11], S42, 0xbd3af235);
		II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb);     II (b, c, d, a, x[ 9], S44, 0xeb86d391);

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;

		zerostruct (x);
	}

	void finish (void* result) noexcept
	{
		unsigned char encodedLength[8];
		encode (encodedLength, count, 8);

		// Pad out to 56 mod 64.
		const int index = (count[0] >> 3) & 0x3f;

		const int paddingLength = (index < 56) ? (56 - index)
											   : (120 - index);

		uint8 paddingBuffer[64] = { 0x80 }; // first byte is 0x80, remaining bytes are zero.
		processBlock (paddingBuffer, (size_t) paddingLength);

		processBlock (encodedLength, 8);

		encode (result, state, 16);
		zerostruct (buffer);
	}

private:
	uint8 buffer [64];
	uint32 state [4];
	uint32 count [2];

	static void encode (void* const output, const void* const input, const int numBytes) noexcept
	{
		for (int i = 0; i < (numBytes >> 2); ++i)
			static_cast<uint32*> (output)[i] = ByteOrder::swapIfBigEndian (static_cast<const uint32*> (input) [i]);
	}

	static inline uint32 rotateLeft (const uint32 x, const uint32 n) noexcept          { return (x << n) | (x >> (32 - n)); }

	static inline uint32 F (const uint32 x, const uint32 y, const uint32 z) noexcept   { return (x & y) | (~x & z); }
	static inline uint32 G (const uint32 x, const uint32 y, const uint32 z) noexcept   { return (x & z) | (y & ~z); }
	static inline uint32 H (const uint32 x, const uint32 y, const uint32 z) noexcept   { return x ^ y ^ z; }
	static inline uint32 I (const uint32 x, const uint32 y, const uint32 z) noexcept   { return y ^ (x | ~z); }

	static void FF (uint32& a, const uint32 b, const uint32 c, const uint32 d, const uint32 x, const uint32 s, const uint32 ac) noexcept
	{
		a += F (b, c, d) + x + ac;
		a = rotateLeft (a, s) + b;
	}

	static void GG (uint32& a, const uint32 b, const uint32 c, const uint32 d, const uint32 x, const uint32 s, const uint32 ac) noexcept
	{
		a += G (b, c, d) + x + ac;
		a = rotateLeft (a, s) + b;
	}

	static void HH (uint32& a, const uint32 b, const uint32 c, const uint32 d, const uint32 x, const uint32 s, const uint32 ac) noexcept
	{
		a += H (b, c, d) + x + ac;
		a = rotateLeft (a, s) + b;
	}

	static void II (uint32& a, const uint32 b, const uint32 c, const uint32 d, const uint32 x, const uint32 s, const uint32 ac) noexcept
	{
		a += I (b, c, d) + x + ac;
		a = rotateLeft (a, s) + b;
	}
};

MD5::MD5() noexcept
{
	zerostruct (result);
}

MD5::MD5 (const MD5& other) noexcept
{
	memcpy (result, other.result, sizeof (result));
}

MD5& MD5::operator= (const MD5& other) noexcept
{
	memcpy (result, other.result, sizeof (result));
	return *this;
}

MD5::MD5 (const MemoryBlock& data) noexcept
{
	processData (data.getData(), data.getSize());
}

MD5::MD5 (const void* data, const size_t numBytes) noexcept
{
	processData (data, numBytes);
}

MD5::MD5 (const CharPointer_UTF8& utf8) noexcept
{
	jassert (utf8.getAddress() != nullptr);
	processData (utf8.getAddress(), utf8.sizeInBytes() - 1);
}

MD5 MD5::fromUTF32 (const String& text)
{
	MD5Generator generator;
	String::CharPointerType t (text.getCharPointer());

	while (! t.isEmpty())
	{
		uint32 unicodeChar = ByteOrder::swapIfBigEndian ((uint32) t.getAndAdvance());
		generator.processBlock (&unicodeChar, sizeof (unicodeChar));
	}

	MD5 m;
	generator.finish (m.result);
	return m;
}

MD5::MD5 (InputStream& input, int64 numBytesToRead)
{
	processStream (input, numBytesToRead);
}

MD5::MD5 (const File& file)
{
	FileInputStream fin (file);

	if (fin.getStatus().wasOk())
		processStream (fin, -1);
	else
		zerostruct (result);
}

MD5::~MD5() noexcept {}

void MD5::processData (const void* data, size_t numBytes) noexcept
{
	MD5Generator generator;
	generator.processBlock (data, numBytes);
	generator.finish (result);
}

void MD5::processStream (InputStream& input, int64 numBytesToRead)
{
	MD5Generator generator;

	if (numBytesToRead < 0)
		numBytesToRead = std::numeric_limits<int64>::max();

	while (numBytesToRead > 0)
	{
		uint8 tempBuffer [512];
		const int bytesRead = input.read (tempBuffer, (int) jmin (numBytesToRead, (int64) sizeof (tempBuffer)));

		if (bytesRead <= 0)
			break;

		numBytesToRead -= bytesRead;
		generator.processBlock (tempBuffer, (size_t) bytesRead);
	}

	generator.finish (result);
}

MemoryBlock MD5::getRawChecksumData() const
{
	return MemoryBlock (result, sizeof (result));
}

String MD5::toHexString() const
{
	return String::toHexString (result, sizeof (result), 0);
}

bool MD5::operator== (const MD5& other) const noexcept   { return memcmp (result, other.result, sizeof (result)) == 0; }
bool MD5::operator!= (const MD5& other) const noexcept   { return ! operator== (other); }

/*** End of inlined file: juce_MD5.cpp ***/


/*** Start of inlined file: juce_SHA256.cpp ***/
class SHA256Processor
{
public:
	SHA256Processor() noexcept
		: length (0)
	{
		state[0] = 0x6a09e667;
		state[1] = 0xbb67ae85;
		state[2] = 0x3c6ef372;
		state[3] = 0xa54ff53a;
		state[4] = 0x510e527f;
		state[5] = 0x9b05688c;
		state[6] = 0x1f83d9ab;
		state[7] = 0x5be0cd19;
	}

	// expects 64 bytes of data
	void processFullBlock (const void* const data) noexcept
	{
		const uint32 constants[] =
		{
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
		};

		uint32 block[16], s[8];
		memcpy (s, state, sizeof (s));

		for (uint32 j = 0; j < 64; j += 16)
		{
			#define JUCE_SHA256(i) s[(7 - i) & 7] += S1 (s[(4 - i) & 7]) + ch (s[(4 - i) & 7], s[(5 - i) & 7], s[(6 - i) & 7]) + constants[i + j] \
														+ (j != 0 ? (block[i & 15] += s1 (block[(i - 2) & 15]) + block[(i - 7) & 15] + s0 (block[(i - 15) & 15])) \
																  : (block[i] = ByteOrder::bigEndianInt (addBytesToPointer (data, i * 4)))); \
								   s[(3 - i) & 7] += s[(7 - i) & 7]; \
								   s[(7 - i) & 7] += S0 (s[(0 - i) & 7]) + maj (s[(0 - i) & 7], s[(1 - i) & 7], s[(2 - i) & 7])

			JUCE_SHA256(0);  JUCE_SHA256(1);  JUCE_SHA256(2);  JUCE_SHA256(3);  JUCE_SHA256(4);  JUCE_SHA256(5);  JUCE_SHA256(6);  JUCE_SHA256(7);
			JUCE_SHA256(8);  JUCE_SHA256(9);  JUCE_SHA256(10); JUCE_SHA256(11); JUCE_SHA256(12); JUCE_SHA256(13); JUCE_SHA256(14); JUCE_SHA256(15);
			#undef JUCE_SHA256
		}

		for (int i = 0; i < 8; ++i)
			state[i] += s[i];

		length += 64;
	}

	void processFinalBlock (const void* const data, unsigned int numBytes) noexcept
	{
		jassert (numBytes < 64);

		length += numBytes;
		length *= 8; // (the length is stored as a count of bits, not bytes)

		uint8 finalBlocks[128];

		memcpy (finalBlocks, data, numBytes);
		finalBlocks [numBytes++] = 128; // append a '1' bit

		while (numBytes != 56 && numBytes < 64 + 56)
			finalBlocks [numBytes++] = 0; // pad with zeros..

		for (int i = 8; --i >= 0;)
			finalBlocks [numBytes++] = (uint8) (length >> (i * 8)); // append the length.

		jassert (numBytes == 64 || numBytes == 128);

		processFullBlock (finalBlocks);

		if (numBytes > 64)
			processFullBlock (finalBlocks + 64);
	}

	void copyResult (uint8* result) const noexcept
	{
		for (int i = 0; i < 8; ++i)
		{
			*result++ = (uint8) (state[i] >> 24);
			*result++ = (uint8) (state[i] >> 16);
			*result++ = (uint8) (state[i] >> 8);
			*result++ = (uint8) state[i];
		}
	}

	void processStream (InputStream& input, int64 numBytesToRead, uint8* const result)
	{
		if (numBytesToRead < 0)
			numBytesToRead = std::numeric_limits<int64>::max();

		for (;;)
		{
			uint8 buffer [64];
			const int bytesRead = input.read (buffer, (int) jmin (numBytesToRead, (int64) sizeof (buffer)));

			if (bytesRead < (int) sizeof (buffer))
			{
				processFinalBlock (buffer, (unsigned int) bytesRead);
				break;
			}

			numBytesToRead -= sizeof (buffer);
			processFullBlock (buffer);
		}

		copyResult (result);
	}

private:
	uint32 state[8];
	uint64 length;

	static inline uint32 rotate (const uint32 x, const uint32 y) noexcept                { return (x >> y) | (x << (32 - y)); }
	static inline uint32 ch  (const uint32 x, const uint32 y, const uint32 z) noexcept   { return z ^ ((y ^ z) & x); }
	static inline uint32 maj (const uint32 x, const uint32 y, const uint32 z) noexcept   { return y ^ ((y ^ z) & (x ^ y)); }

	static inline uint32 s0 (const uint32 x) noexcept     { return rotate (x, 7)  ^ rotate (x, 18) ^ (x >> 3); }
	static inline uint32 s1 (const uint32 x) noexcept     { return rotate (x, 17) ^ rotate (x, 19) ^ (x >> 10); }
	static inline uint32 S0 (const uint32 x) noexcept     { return rotate (x, 2)  ^ rotate (x, 13) ^ rotate (x, 22); }
	static inline uint32 S1 (const uint32 x) noexcept     { return rotate (x, 6)  ^ rotate (x, 11) ^ rotate (x, 25); }

	JUCE_DECLARE_NON_COPYABLE (SHA256Processor);
};

SHA256::SHA256() noexcept
{
	zerostruct (result);
}

SHA256::~SHA256() noexcept {}

SHA256::SHA256 (const SHA256& other) noexcept
{
	memcpy (result, other.result, sizeof (result));
}

SHA256& SHA256::operator= (const SHA256& other) noexcept
{
	memcpy (result, other.result, sizeof (result));
	return *this;
}

SHA256::SHA256 (const MemoryBlock& data)
{
	process (data.getData(), data.getSize());
}

SHA256::SHA256 (const void* const data, const size_t numBytes)
{
	process (data, numBytes);
}

SHA256::SHA256 (InputStream& input, const int64 numBytesToRead)
{
	SHA256Processor processor;
	processor.processStream (input, numBytesToRead, result);
}

SHA256::SHA256 (const File& file)
{
	FileInputStream fin (file);

	if (fin.getStatus().wasOk())
	{
		SHA256Processor processor;
		processor.processStream (fin, -1, result);
	}
	else
	{
		zerostruct (result);
	}
}

SHA256::SHA256 (const CharPointer_UTF8& utf8) noexcept
{
	jassert (utf8.getAddress() != nullptr);
	process (utf8.getAddress(), utf8.sizeInBytes() - 1);
}

void SHA256::process (const void* const data, size_t numBytes)
{
	MemoryInputStream m (data, numBytes, false);
	SHA256Processor processor;
	processor.processStream (m, -1, result);
}

MemoryBlock SHA256::getRawData() const
{
	return MemoryBlock (result, sizeof (result));
}

String SHA256::toHexString() const
{
	return String::toHexString (result, sizeof (result), 0);
}

bool SHA256::operator== (const SHA256& other) const noexcept  { return memcmp (result, other.result, sizeof (result)) == 0; }
bool SHA256::operator!= (const SHA256& other) const noexcept  { return ! operator== (other); }

#if JUCE_UNIT_TESTS

class SHA256Tests  : public UnitTest
{
public:
	SHA256Tests() : UnitTest ("SHA-256") {}

	void runTest()
	{
		beginTest ("SHA-256");

		{
			char n[] = "";
			SHA256 sha (n, 0);
			expectEquals (sha.toHexString(), String ("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
		}

		{
			const char n[] = "The quick brown fox jumps over the lazy dog";
			SHA256 sha (n, sizeof (n) - 1);
			expectEquals (sha.toHexString(), String ("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"));
		}

		{
			const char n[] = "The quick brown fox jumps over the lazy dog.";
			SHA256 sha (n, sizeof (n) - 1);
			expectEquals (sha.toHexString(), String ("ef537f25c895bfa782526529a9b63d97aa631564d5d789c2b765448c8635fb6c"));
		}
	}
};

static SHA256Tests sha256UnitTests;

#endif

/*** End of inlined file: juce_SHA256.cpp ***/

// END_AUTOINCLUDE

}

/*** End of inlined file: juce_cryptography.cpp ***/


#endif

