#pragma once

#include <iostream>
#include <tuple>
#include <fstream>
#include <sstream>
#include <cstring>
#include <random>
#include <time.h> 
#include <vector>
#include <memory>
#include <windows.h>
#include <cstdlib>   // for rand() and srand()
#include <ctime>
#include <fstream>
#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>   
// #include <unordered_map>
#include <functional>

//counters
int movesSeen = 0;
int counter = 0;
int cacheHits = 0;

const bool AI_IS_WHITE = true;

const int NUM_PIECES = 5;  // Number of piece types
const int NUM_SQUARES = 64;  // Number of squares on the board
const int NUM_TURNS = 2;  // Number of possible turns

const int MAX_DEPTH = 20;  // Depth of the search tree

const int MAX_TIME_SECONDS = 16;  // Maximum time allowed for the AI to make a move

uint64_t lookupTable[NUM_PIECES][NUM_SQUARES][NUM_TURNS];

std::array<std::array<uint64_t, 64>, 4> zobristTable = {{
    {
        0xc151df7d6ee5e2d6ULL,
        0xa3978fb9b92502a8ULL,
        0xc08c967f0e5e7b0aULL,
        0x22e2c43f8a1ad34eULL,
        0xe73ca28e4d361955ULL,
        0x1814dc629c7f4f7cULL,
        0x93170a1965d42420ULL,
        0x5f75917a3eb7b900ULL,
        0x461c9cf62eb9fcb6ULL,
        0x63e8cae041677d61ULL,
        0x32b846d0bbd3f4bULL,
        0x861191c96090b446ULL,
        0xaf6df0655c6891d8ULL,
        0xa32897ae188a782eULL,
        0xd398c3c9b02b56cdULL,
        0xf2194bc7d5976b28ULL,
        0xc0d2eda553d1bc36ULL,
        0x72ec29f7fc409872ULL,
        0xbfb48552d60ec21ULL,
        0x10894433f9475527ULL,
        0xbf62e22b9623fb4bULL,
        0x2639bc290a9eeb52ULL,
        0x6cdd812c68f90519ULL,
        0x199cad8504e80ac9ULL,
        0x24d78d1d1b2520c5ULL,
        0x181baef38522f794ULL,
        0x87b8a689f6b57129ULL,
        0x7239f67bad2506ecULL,
        0xc5e48fc8e6f31d96ULL,
        0x1f5d587b2a99fb54ULL,
        0xbec32aaf5dd5e327ULL,
        0x52e1d21451942857ULL,
        0xc03f877c1570460aULL,
        0xcde8baa8d4b7aafcULL,
        0xb23f634f6408decULL,
        0xed150d8debe66847ULL,
        0xbe5fc1f88f25ea10ULL,
        0x6a82296420640d5eULL,
        0x4c317a47961d9218ULL,
        0x4f0dca8b02d0c43ULL,
        0xb32a05a58b5024d7ULL,
        0x759dddab6e391eb0ULL,
        0x897443c2f9f697f5ULL,
        0x7d261409f88748f9ULL,
        0x36316c73bfff2278ULL,
        0x9b703d3ea8a33821ULL,
        0x7e3a388ffa85872eULL,
        0x624a46bcb0932aa3ULL,
        0xaac752b8d69267d9ULL,
        0x6a7d9e89a2a8dfbeULL,
        0x6f3c8ecd3d54bc48ULL,
        0x1773cba004ec6d5fULL,
        0x1301665790d975c4ULL,
        0xd20d5cd7b0d1e499ULL,
        0x7a3e0cb3db38f9f3ULL,
        0x71850568e494f356ULL,
        0xfed5bb629d4922f1ULL,
        0x984e0108fb06303dULL,
        0x9e795eeb3d4ad1fbULL,
        0x93a9703bf5ddf9c8ULL,
        0xdc7bfdac74d42c55ULL,
        0x79c8bd3b7c5667ecULL,
        0xd2e2ae0d1a3eb2b1ULL,
        0x7de5b97a534feed7ULL
    },
    {
        0x71d1e37062dd0887ULL,
        0x4f5e02a1ffb1d16dULL,
        0x4c0b837adde56ec1ULL,
        0x85d5b800b5eebc0dULL,
        0x5d682face1d91cfeULL,
        0xc388414f03e224f9ULL,
        0x4e343e19658fad25ULL,
        0xafa13bb2f9560a92ULL,
        0xbaa602f9af703e45ULL,
        0xa6955e3e45a326b5ULL,
        0xd0f64ceef810ed6cULL,
        0xc981391ce8427210ULL,
        0xa6ef782f0bf1146dULL,
        0xcb399e14a3dbe0c4ULL,
        0x670e15a0af9013f4ULL,
        0xefc5f70e9f8cc5c8ULL,
        0x5907059e2371a8f4ULL,
        0x4ca98403ada2dff9ULL,
        0xbcf6430458eac477ULL,
        0x98088ed375f60a22ULL,
        0x5a46c424f4f4df37ULL,
        0x2380de90cd34d6ccULL,
        0x9fea209d883cc671ULL,
        0x97bca181150e774aULL,
        0x85885ad6ed1226efULL,
        0x9ce7e5668c3f6d37ULL,
        0xf260c46fe63cba1ULL,
        0x5df0d016443302cbULL,
        0xbc3f2277fa0a5233ULL,
        0x9c9c2513e7e211fbULL,
        0x654028ee86f47566ULL,
        0xeeaf3186e0eccb37ULL,
        0xbd9af33f4361fba4ULL,
        0x3ae9656310431dd8ULL,
        0x8774640f04c66526ULL,
        0x1fb4b7a1ef3a77b1ULL,
        0xc99a99d82b338793ULL,
        0x3c185b19276fdba6ULL,
        0x17a9e44dd7fec4f1ULL,
        0xe3d278c08c008050ULL,
        0x78d78496e22fc7ddULL,
        0x1cf4dcb5962daa5cULL,
        0xb143afe423a350b8ULL,
        0x2091d498a12c5821ULL,
        0xb85ad2a67be7e2b3ULL,
        0x9ad2460ef13afea6ULL,
        0x5b20acd104d03671ULL,
        0xc4d934cf92904d12ULL,
        0xeddc6d68d717f94eULL,
        0xff0c0a8e431346b2ULL,
        0x63fb670814135febULL,
        0x5f14958f73b018d8ULL,
        0xd50feefb4e6393bcULL,
        0xc3d2462a526736e5ULL,
        0xa8a2530e4b3dd99cULL,
        0xe2260ab5045a7c48ULL,
        0x10ce0bab0d31681fULL,
        0x53103f7d9e6b0af9ULL,
        0x4b0b96f7309e81f2ULL,
        0x6a9e6c37a0b80db5ULL,
        0x291b9fc4577f6cdaULL,
        0xda0a0376d63de5b6ULL,
        0x7c61725b63f7a126ULL,
        0x4a793bcca72c0c17ULL
    },
    {
        0xd1a16c077908af9bULL,
        0x6dbde60aebf5cbe4ULL,
        0x1475ee02bbc875adULL,
        0xe03e89c3800045f4ULL,
        0xcabdbf1e42d853dfULL,
        0xe73c868824e9bc00ULL,
        0xdada80ce44ad991bULL,
        0x6d12906e07f5c2dULL,
        0xbea2629f3d40beb4ULL,
        0x11ee7606dda7179cULL,
        0x34d2804ea3dc3069ULL,
        0xa14c2b610297de46ULL,
        0x715b2bb5aa7a4a10ULL,
        0x5531816d146724b9ULL,
        0x5b56811e9841396eULL,
        0xb45085a764018d6aULL,
        0x56a418a651b0f73bULL,
        0xbe1c8f0a0f62b0a2ULL,
        0x29f155774506a8fbULL,
        0xa6f073f2a76276c4ULL,
        0x14c8c88942da71feULL,
        0x66849549d0e0671aULL,
        0x5bc10ef75cc8a817ULL,
        0x8449bd8349a47adaULL,
        0xabedfdf15a691a5aULL,
        0xb8c793e0dfe19368ULL,
        0x8073c9d47bbb9320ULL,
        0x85d6a0fd6fdc71f6ULL,
        0x65b5a97d3d16a62fULL,
        0xcaad9ecb5880bf6fULL,
        0x3007675ae523ac53ULL,
        0x5984ae05ff5e68f7ULL,
        0x8df37436a3ad9d0eULL,
        0x14cb199eb6c174f3ULL,
        0x3bb7aee645444166ULL,
        0x6eb4dce227c86385ULL,
        0x792784f163e4cd83ULL,
        0x37bb925a37221fdeULL,
        0x99267a716c2bcf4bULL,
        0xfda24117168c817dULL,
        0xe716519345f99c9bULL,
        0x6147309e3e76c6eeULL,
        0x7b72be688e3c734bULL,
        0x4a6512d680d3628dULL,
        0x73d73462d932675dULL,
        0x4f72e0bc23efc8b4ULL,
        0x49fde5a5333060dcULL,
        0x73543fd4e8b2b445ULL,
        0xa425bc383ec06e48ULL,
        0x1002e3f0e61d8802ULL,
        0x82bdf82d8c203068ULL,
        0x59fb9cb0b19517c4ULL,
        0xc376899f5b7afb34ULL,
        0x6deb569d22f538ebULL,
        0x3309e77e61f4441fULL,
        0x2f1afaa25a1949ULL,
        0xa362b384b3f0f039ULL,
        0x45874539e3d019afULL,
        0x2deeedaa74df00f1ULL,
        0xe635218c586e85a3ULL,
        0xcc1e55fe7773f56fULL,
        0x68929ddafb94f3beULL,
        0x19d236e53fee9fc4ULL,
        0xacb54a3512057e6aULL
    },
    {
        0xf729c31add801051ULL,
        0x455cf0d9b3800271ULL,
        0x130a1b509c2a24d8ULL,
        0xa1e77652003f01daULL,
        0x92f390d9e187eaf8ULL,
        0x2d99c35f87c0130eULL,
        0xe37a71027fb942b7ULL,
        0x7229a2135d8b5ee4ULL,
        0x1a419956cdacf5b7ULL,
        0x299efcfa3f6216c1ULL,
        0xe901f710d0d3c802ULL,
        0x7484c2a7666e7ca6ULL,
        0xd1026db6513962f8ULL,
        0x6afd0ba9f96f66c6ULL,
        0x6db1119d54199e32ULL,
        0x52d0065fa15bac18ULL,
        0x1a5a91393ec5d7d0ULL,
        0x31bf6b3d005e47bbULL,
        0x3d967b74da1b8f21ULL,
        0xe113dfacbe62b98dULL,
        0x71b4c620ab34e1b2ULL,
        0x3517e1c8b8110264ULL,
        0x298c5d4dc6b9fbc6ULL,
        0x2c58435b64456644ULL,
        0x2898f1f292b0d4bfULL,
        0x1f6c97cd21a4dfd6ULL,
        0x35f69217ad61adfdULL,
        0xf3ac6acd92b01e97ULL,
        0x6d0dda73503eecc7ULL,
        0x64a52b3bf9bebb5fULL,
        0x22485883b5fdbb63ULL,
        0x21116379a0723312ULL,
        0x665d1f98e5205fe0ULL,
        0x32acfaa70b418300ULL,
        0x296f8bace5aa97a4ULL,
        0x55c6317db91538acULL,
        0xd878a424b7e3d4f0ULL,
        0xa6a3199914ac5468ULL,
        0x53691744aa2ed4b9ULL,
        0xed1c18a2a7939349ULL,
        0x6383d008802c7915ULL,
        0x972c7165ccd006b9ULL,
        0x82010cd94b8cb19aULL,
        0x6c1ebf8302b00d7aULL,
        0xa1e234030e779a90ULL,
        0xef4997768cbd1eb5ULL,
        0x2df7f65b6760b16cULL,
        0x6ccb20eef4005198ULL,
        0xd3a0c6226789dd2dULL,
        0xa49c0328a7a57b41ULL,
        0x149fcb8820d3aca8ULL,
        0xc6f3f60de7b8ec2bULL,
        0xa1f91257d9a120ceULL,
        0xe8a5bfc71df99756ULL,
        0x3b06905fdccc8153ULL,
        0xddc964fcea69cee1ULL,
        0x37707ea0b29af4edULL,
        0x690fef5e3202bff2ULL,
        0x6a68e90964f0759aULL,
        0xa0cc58813c1152f7ULL,
        0xec8639df999556e0ULL,
        0xb95f658565aafe7cULL,
        0x48f8bd1407f14301ULL,
        0x2e72e0f43bb3cad0ULL
    }
}};
