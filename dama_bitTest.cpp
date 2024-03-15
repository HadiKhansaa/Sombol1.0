#include "BitmaskBoard.hpp"

using namespace std;



//counters
int movesSeen = 0;
int counter = 0;
int cacheHits = 0;

//hashing stuff
std::mt19937_64 rng(std::random_device{ }());
std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());
std::unordered_map<uint64_t, std::pair<uint64_t, int>> transpositionTable;

const int NUM_PIECES = 5;  // Number of piece types
const int NUM_SQUARES = 64;  // Number of squares on the board
const int NUM_TURNS = 2;  // Number of possible turns

uint64_t lookupTable[NUM_PIECES][NUM_SQUARES][NUM_TURNS];

void initializeLookupTable() {
    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (int square = 0; square < NUM_SQUARES; ++square) {
            for (int turn = 0; turn < NUM_TURNS; ++turn) {
                lookupTable[piece][square][turn] = distribution(rng);
            }
        }
    }
}

// Zorbist hashing
// uint64_t zobristTable[4][64]; // 4 types of pieces: black pawns, white pawns, black kings, white kings

// hardcoded zobrist table
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

// Function to initialize Zobrist table with random values
void initializeZobristTable() {
    std::mt19937_64 rng(std::random_device{ }());
    for (int pieceType = 0; pieceType < 4; ++pieceType) {
        for (int position = 0; position < 64; ++position) {
            zobristTable[pieceType][position] = rng();
        }
    }
}

uint64_t calculateHashKey(BitmaskBoard boardLayout, char turn) {
    // uint64_t hashKey = 0;
    // for (int row = 0; row < 8; ++row) {
    //     for (int col = 0; col < 8; ++col) {
    //         int square = row * 8 + col;
    //         int piece = boardLayout.get(row, col);  // Assuming the board layout stores piece characters (e.g., '1', '2', '3', '4')

    //         if (true) {
    //             hashKey ^= lookupTable[piece][square][turn - 1];
    //         }
    //     }
    // }
    return boardLayout.calculateHashKey(turn, zobristTable);
}

void writeTranspositionTableToFile(const std::unordered_map<uint64_t, std::pair<uint64_t, int>>& transpositionTable, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& entry : transpositionTable) {
        outFile << entry.first << " " << entry.second.first << " " << entry.second.second << "\n";
    }

    outFile.close();
}

// Function to load the transposition table from a file
void loadTranspositionTableFromFile(std::unordered_map<uint64_t, std::pair<uint64_t, int>>& transpositionTable, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    uint64_t key;
    uint64_t valueFirst;
    int valueSecond;

    while (inFile >> key >> valueFirst >> valueSecond) {
        transpositionTable[key] = {valueFirst, valueSecond};
    }

    inFile.close();
}

// TODO: needs fixing its wrong
bool kol(BitmaskBoard board_layout, char row, char col, char color){
    if(color==1){
        if(row+1<8 && (row+1 == 2 || row+1 == 4) && row-1>0 && row-1==0) return true; //3am t2elo kol forwards
        if(col+1<8 && (col+1 == 2 || col+1 == 4) && col-1>0 && col-1==0) return true; //3am t2elo kol right
        if(col-1>0 && (col-1 == 2 || col-1 == 4) && col+1<8 && col+1==0) return true; //3am t2elo kol left
    }else if (color==2){
        if(row-1>0 && (row-1 == 1 || row-1 == 3) && row+1<8 && row+1==0) return true; //3am t2elo kol forwards
        if(col+1<8 && (col+1 == 1 || col+1 == 3) && col-1>0 && col-1==0) return true; //3am t2elo kol right
        if(col-1>0 && (col-1 == 1 || col-1 == 3) && col+1<8 && col+1==0) return true; //3am t2elo kol left
    }
    return false;
}

bool check_dama_has_akel(char row, char col, char color, BitmaskBoard board_layout, char eat_direction)
{
    bool ate_up = (eat_direction == 'u');
    bool ate_down = (eat_direction == 'd');
    bool ate_left = (eat_direction == 'l');
    bool ate_right = (eat_direction == 'r');

    if(color == 3)
    {
        //right
        char i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
                break;
            if (col+i<8 && board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
            {
                char pieceToEat = board_layout.get(row, col+i);
                char j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }

        //left
        i=1;
        while(true && !ate_right)
        {
            if (col-i < 0 || col>7 || row>7)
                break;
            if (board_layout.check_index_has_blackKing(row, col-i) || board_layout.check_index_has_blackPawn(row, col-i))
                break;
            if (board_layout.check_index_has_whiteKing(row, col-i) || board_layout.check_index_has_whitePawn(row, col-i))
            {
                char pieceToEat = board_layout.get(row, col-i);
                char j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row, col-i-j) || board_layout.check_index_has_whitePawn(row, col-i-j) || board_layout.check_index_has_blackKing(row, col-i-j) || board_layout.check_index_has_blackPawn(row, col-i-j))
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }
        
        i=1; //down
        while(true && !ate_up)
        {
            if (row+i>=8 || col>=8 || row+i<0 || col<0)
                break;
            if (board_layout.check_index_has_blackKing(row+i, col) || board_layout.check_index_has_blackPawn(row+i, col))
                break;
            if (board_layout.check_index_has_whiteKing(row+i, col) || board_layout.check_index_has_whitePawn(row+i, col))
            {
                char pieceToEat = board_layout.get(row+i, col);
                char j=1;
                while(true)
                {

                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row+i+j, col) || board_layout.check_index_has_whitePawn(row+i+j, col) || board_layout.check_index_has_blackKing(row+i+j, col) || board_layout.check_index_has_blackPawn(row+i+j, col))
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }
            
        i=1; //up
        while(true && !ate_down)
        {
            if (row-i<0)
                break;
            if (board_layout.check_index_has_blackKing(row-i, col) || board_layout.check_index_has_blackPawn(row-i, col))
                break;
            if (board_layout.check_index_has_whiteKing(row-i, col) || board_layout.check_index_has_whitePawn(row-i, col))
            {
                char pieceToEat = board_layout.get(row-i, col);
                char j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row-i-j, col) || board_layout.check_index_has_whitePawn(row-i-j, col) || board_layout.check_index_has_blackKing(row-i-j, col) || board_layout.check_index_has_blackPawn(row-i-j, col))
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }

        return false;
    }
    else
    {
        //right
        char i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
                break;
            if (col+i<8 && board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
            {
                char pieceToEat = board_layout.get(row, col+i);
                char j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col+i+j) != 0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }

        //left
        i=1;
        while(true && !ate_right)
        {
            if (col-i < 0)
                break;
            if (board_layout.check_index_has_whiteKing(row, col-i) || board_layout.check_index_has_whitePawn(row, col-i))
                break;
            if (board_layout.check_index_has_blackKing(row, col-i) || board_layout.check_index_has_blackPawn(row, col-i))
            {
                char pieceToEat = board_layout.get(row, col-i);
                char j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col-i-j)!=0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }
        
        i=1; //down
        while(true && !ate_up)
        {
            if (row+i>7)
                break;
            if (board_layout.check_index_has_whiteKing(row+i, col) || board_layout.check_index_has_whitePawn(row+i, col))
                break;
            if (board_layout.check_index_has_blackKing(row+i, col) || board_layout.check_index_has_blackPawn(row+i, col))
            {
                char pieceToEat = board_layout.get(row+i, col);
                char j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row+i+j, col)!=0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }
            
        i=1; //up
        while(true && !ate_down)
        {
            if (row-i<0)
                break;
            if (board_layout.check_index_has_whiteKing(row-i, col) || board_layout.check_index_has_whitePawn(row-i, col))
                break;
            if (board_layout.check_index_has_blackKing(row-i, col) || board_layout.check_index_has_blackPawn(row-i, col))
            {
                char pieceToEat = board_layout.get(row-i, col);
                char j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row-i-j, col) != 0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }
        return false;
    }
    return false;
}

pair<vector<char*>, char> get_valid_moves(char row,char col, char color, char streak, vector<char*> valid_moves, BitmaskBoard board_layout, char eat_direction)
{
    if(row>7 || col>7 || row<0 || col<0)
    {
        pair<vector<char*>, char> validMovesAndEatingPiece;
        validMovesAndEatingPiece.first = { };
        validMovesAndEatingPiece.second = 0;
        return validMovesAndEatingPiece;
    }
    bool ate_up = (eat_direction == 'u');
    bool ate_down = (eat_direction == 'd');
    bool ate_left = (eat_direction == 'l');
    bool ate_right = (eat_direction == 'r');

    char *v_move= new char[2];

    char eating_piece=0;
    
    //black dama
    if(color == 3)
    {
        //right
        char i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
                break;
            if (col+i<8 && board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
            {
                char pieceToEat = board_layout.get(row, col+i);
                char j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col+i+j;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }

        //left
        i=1;
        while(true && !ate_right)
        {
            if (col-i < 0 || col>7 || row>7)
                break;
            if (board_layout.check_index_has_blackKing(row, col-i) || board_layout.check_index_has_blackPawn(row, col-i))
                break;
            if (board_layout.check_index_has_whiteKing(row, col-i) || board_layout.check_index_has_whitePawn(row, col-i))
            {
                char pieceToEat = board_layout.get(row, col-i);
                char j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row, col-i-j) || board_layout.check_index_has_whitePawn(row, col-i-j) || board_layout.check_index_has_blackKing(row, col-i-j) || board_layout.check_index_has_blackPawn(row, col-i-j))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col-i-j;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        i=1; //down
        while(true && !ate_up)
        {
            if (row+i>=8 || col>=8 || row+i<0 || col<0)
                break;
            if (board_layout.check_index_has_blackKing(row+i, col) || board_layout.check_index_has_blackPawn(row+i, col))
                break;
            if (board_layout.check_index_has_whiteKing(row+i, col) || board_layout.check_index_has_whitePawn(row+i, col))
            {
                char pieceToEat = board_layout.get(row+i, col);
                char j=1;
                while(true)
                {

                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row+i+j, col) || board_layout.check_index_has_whitePawn(row+i+j, col) || board_layout.check_index_has_blackKing(row+i+j, col) || board_layout.check_index_has_blackPawn(row+i+j, col))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row+i+j;
                        v_move[1] = col;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
            
        i=1; //up
        while(true && !ate_down)
        {
            if (row-i<0)
                break;
            if (board_layout.check_index_has_blackKing(row-i, col) || board_layout.check_index_has_blackPawn(row-i, col))
                break;
            if (board_layout.check_index_has_whiteKing(row-i, col) || board_layout.check_index_has_whitePawn(row-i, col))
            {
                char pieceToEat = board_layout.get(row-i, col);
                char j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row-i-j, col) || board_layout.check_index_has_whitePawn(row-i-j, col) || board_layout.check_index_has_blackKing(row-i-j, col) || board_layout.check_index_has_blackPawn(row-i-j, col))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row-i-j;
                        v_move[1] = col;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        if (eating_piece==0)
        {
            i=1;
            while(row+i<8 && board_layout.get(row+i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row+i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout.get(row-i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row-i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(col+i<8 && board_layout.get(row, col+i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && row>0 && row<8 && col-i<8 && board_layout.get(row, col-i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-i;
                valid_moves.push_back(v_move);
                i+=1;
            }

            
        }
    }

    if(color == 4)
    {
        //right
        char i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
                break;
            if (col+i<8 && board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
            {
                char pieceToEat = board_layout.get(row, col+i);
                char j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col+i+j) != 0)
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col+i+j;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }

        //left
        i=1;
        while(true && !ate_right)
        {
            if (col-i < 0)
                break;
            if (board_layout.check_index_has_whiteKing(row, col-i) || board_layout.check_index_has_whitePawn(row, col-i))
                break;
            if (board_layout.check_index_has_blackKing(row, col-i) || board_layout.check_index_has_blackPawn(row, col-i))
            {
                char pieceToEat = board_layout.get(row, col-i);
                char j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col-i-j)!=0)
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col-i-j;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        i=1; //down
        while(true && !ate_up)
        {
            if (row+i>7)
                break;
            if (board_layout.check_index_has_whiteKing(row+i, col) || board_layout.check_index_has_whitePawn(row+i, col))
                break;
            if (board_layout.check_index_has_blackKing(row+i, col) || board_layout.check_index_has_blackPawn(row+i, col))
            {
                char pieceToEat = board_layout.get(row+i, col);
                char j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row+i+j, col)!=0)
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row+i+j;
                        v_move[1] = col;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
            
        i=1; //up
        while(true && !ate_down)
        {
            if (row-i<0)
                break;
            if (board_layout.check_index_has_whiteKing(row-i, col) || board_layout.check_index_has_whitePawn(row-i, col))
                break;
            if (board_layout.check_index_has_blackKing(row-i, col) || board_layout.check_index_has_blackPawn(row-i, col))
            {
                char pieceToEat = board_layout.get(row-i, col);
                char j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row-i-j, col) != 0)
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        v_move = new char[2];
                        v_move[0] = row-i-j;
                        v_move[1] = col;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        if (eating_piece==0)
        {
            i=1;
            while(row+i<8 && board_layout.get(row+i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row+i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout.get(row-i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row-i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(col+i<8 && board_layout.get(row, col+i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && board_layout.get(row, col-i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
        }
    }

    if(color == 1)
    {
        //streak
        if (row+2<8 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)) && board_layout.get(row+2, col)==0 && streak>=0)
        {
            char pieceToEat = board_layout.get(row+1, col);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+2;
            v_move[1] = col;
            if(pieceToEat == 2)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }   

        if (col+2<8 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)) && board_layout.get(row, col+2)==0 && streak>=0 && !ate_left)
        {
            char pieceToEat = board_layout.get(row, col+1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col+2;
            if(pieceToEat == 2)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }
            

        if (col-2>=0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)) && board_layout.get(row, col-2)==0 && streak>=0 &&  !ate_right)
        {
            char pieceToEat = board_layout.get(row, col-1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col-2;
            if(pieceToEat == 2)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }
            
        //ordering of moves
        if(row == 1 || row == 2){ //right and left moves are more important
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;

                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-1;
                valid_moves.push_back(v_move);
            }
            if (row+1<8 && board_layout.get(row+1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row+1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }

            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // }
            
        }
        else{
            if (row+1<8 && board_layout.get(row+1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row+1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;
                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-1;
                valid_moves.push_back(v_move);
            }

            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // }
            
        }
    }
    else if (color == 2)
    {
        //streak
        
        if (row-2>=0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)) && board_layout.get(row-2, col)==0 && streak>=0)
        {
            char pieceToEat = board_layout.get(row-1, col);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row-2;
            v_move[1] = col;
            if(pieceToEat == 1)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }

        if (col+2<8 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)) && board_layout.get(row, col+2)==0 && streak>=0 && !ate_left)
        {
            char pieceToEat = board_layout.get(row, col+1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col+2;
            if(pieceToEat == 1)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        } 

        if (col-2>=0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)) && board_layout.get(row, col-2)==0 && streak>=0 &&  !ate_right)
        {
            char pieceToEat = board_layout.get(row, col-1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col-2;
            if(pieceToEat == 1)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }

        if(row == 6 || row == 5){ //right and left moves are more important
            if (row-1>=0 && board_layout.get(row-1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move = new char[2];
                v_move[0] = row-1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;
                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] =  col-1;
                valid_moves.push_back(v_move);
            }
            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // } 
        }
        else{
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;
                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] =  col-1;
                valid_moves.push_back(v_move);
            }
            if (row-1>=0 && board_layout.get(row-1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move = new char[2];
                v_move[0] = row-1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }
            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // }
        }
    
    }
    
    pair<vector<char*>, char> validMovesAndEatingPiece;
    validMovesAndEatingPiece.first = valid_moves;
    validMovesAndEatingPiece.second = eating_piece;

    return validMovesAndEatingPiece;
}

char get_eat_direction(char row1, char col1, char row2, char col2)
{
    char eat_direction = 0;
    if (col1==col2)
    {
        if (row1>row2)
            eat_direction = 'd';
        else
            eat_direction = 'u';
    }
    else
    {
        if (col1>col2)
            eat_direction = 'r';
        else
            eat_direction = 'l';
    }
    return eat_direction;
}

pair<BitmaskBoard, char> eat_piece_if_possible(BitmaskBoard board_layout, char row, char col, char r, char c, char color)
{
    char eat_direction = 0;
    char sumRowOver2;
    char sumColOver2;
    if (color == 1 || color == 2)
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
        
        eat_direction = get_eat_direction(r, c, row, col);
    }

    if (color == 1)
    {
        if (board_layout.check_index_has_whitePawn(sumRowOver2, col) || board_layout.check_index_has_whiteKing(sumRowOver2, col))
        {
            board_layout.set(sumRowOver2, col, 0);
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout.check_index_has_whitePawn(row, sumColOver2) || board_layout.check_index_has_whiteKing(row, sumColOver2))
        {
            board_layout.set(row, sumColOver2, 0);
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 2)
    {
        if (board_layout.check_index_has_blackPawn(sumRowOver2, col) || board_layout.check_index_has_blackKing(sumRowOver2, col))
        {
            board_layout.set(sumRowOver2, col, 0);
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout.check_index_has_blackPawn(row, sumColOver2) || board_layout.check_index_has_blackKing(row, sumColOver2))
        {
            board_layout.set(row, sumColOver2, 0);
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 3)
    {
        if (c==col)
        {
            char big, small;
            if (r>row)
            {
                big = r;
                small = row;
                eat_direction = 'd';
            }
            else
            {
                big = row;
                small = r;
                eat_direction = 'u';
            }
            for(char k = small; k<big; k++)
            {
                if (board_layout.check_index_has_whitePawn(k, c) || board_layout.check_index_has_whiteKing(k, c))
                {
                    board_layout.set(k, c, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
        else if (row ==r)
        {
            char small, big;
            if (c>col)
            {
                big = c;
                small = col;
                eat_direction = 'r';
            }
            else
            {
                big = col;
                small = c;
                eat_direction = 'l';
            }
            for(char k=small; k<big; k++)
            {
                if (board_layout.check_index_has_whitePawn(r, k) || board_layout.check_index_has_whiteKing(r, k))
                {
                    board_layout.set(r, k, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }                
    else if (color == 4)
    {
        if (c==col)
        {
            char big, small;
            if (r>row)
            {
                big = r;
                small = row;
                eat_direction = 'd';
            }
            else
            {
                big = row;
                small = r;
                eat_direction = 'u';
            }
            for(char k = small; k<big; k++)
            {
                if (board_layout.check_index_has_blackPawn(k, c) || board_layout.check_index_has_blackKing(k, c))
                {
                    board_layout.set(k, c, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
        else if (row ==r)
        {
            char small, big;
            if (c>col)
            {
                big = c;
                small = col;
                eat_direction = 'r';
            }
            else
            {
                big = col;
                small = c;
                eat_direction = 'l';
            }
            for(char k=small; k<big; k++)
            {
                if (board_layout.check_index_has_blackPawn(r, k) || board_layout.check_index_has_blackKing(r, k))
                {
                    board_layout.set(r, k, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }
    return make_pair(board_layout, eat_direction);
}

bool equals(vector<char*> a, vector<char*> b)
{
    for(char i=0; i<(char)a.size(); i++)
        if(a[i][0]!=b[i][0] || a[i][1]!=b[i][1])
            return false;
    return true;
}

bool doesnt_have(vector <vector<char*>> a, vector<char*> b){
    for(auto element:a)
        if(equals(element, b))
            return false;
    return true;
}

std::vector<std::vector<char*>> eat_max2(char row, char col,BitmaskBoard board_layout,  vector<vector<char*>> parent_list, char color, char eat_direction) {
    // Call get_valid_moves function
    vector<char*> emptyV;
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, emptyV, board_layout, eat_direction);

    bool dama_nom = (eating_piece == 'd');
    
    // impossible
    // if(!dama_nom)
    //     return parent_list;

    if (dama_nom) {
        std::vector<char*> new_list;
        std::vector<std::vector<char*>> big_dama_list;
        big_dama_list.clear();
        new_list.clear();
        for (auto move : valid_moves) {
            dama_nom = false;
            
            if (move[1] == col) {
                if (move[0] > row) {
                    eat_direction = 'd';
                } else {
                    eat_direction = 'u';
                }
            } else {
                if (move[1] > col) {
                    eat_direction = 'r';
                } else {
                    eat_direction = 'l';
                }
            }
            // Call get_valid_moves function
            // auto  eating_piece = get_valid_moves(move[0], move[1], color, 0, {}, board_layout, eat_direction).second;

            // dama_nom = (eating_piece == 'd');
            dama_nom = check_dama_has_akel(move[0], move[1], color, board_layout, eat_direction);

            if (dama_nom) {
                char* parent = new char[2];  parent[0] = row; parent[1] = col;
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (parent[0] == value.back()[0] && parent[1] == value.back()[1]) {
                        std::vector<char*> big_dama = value;
                        big_dama.push_back(move);
                        big_dama_list.push_back(big_dama);
                    }
                }
            }
        }
        if (!big_dama_list.empty()) {
            parent_list = big_dama_list;
        } 
        if (new_list.empty()) {
            std::vector<std::vector<char*>> aux_list2;
            aux_list2.clear();
            char counter = 0;
            for (vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[char(value.size())-2];

                // char sizeLast = char(sizeof(last)/sizeof(last[0]));
                // char sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

                if (last[1] == pre_last[1]) {
                    if (last[0] > pre_last[0]) {
                        eat_direction = 'd';
                    } else {
                        eat_direction = 'u';
                    }
                } else {
                    if (last[1] > pre_last[1]) {
                        eat_direction = 'r';
                    } else {
                        eat_direction = 'l';
                    }
                }
                // Call get_valid_moves function
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0], last[1], color, 0, {}, board_layout, eat_direction);

                dama_nom = (eating_piece == 'd');

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
            
            }
            // if(!aux_list2.empty())
            //     return aux_list2;
            parent_list = aux_list2;
            return parent_list;
            
        } else {
            valid_moves = new_list;
            std::vector<std::vector<char*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                BitmaskBoard board_layout2 = board_layout;
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color);
                auto p2 = eat_max2(move[0], move[1], new_board_layout, parent_list, color, eat_direction);
                
                if (maxp.empty()) {
                    maxp = p2;
                } else {
                    if (!p2.empty() && p2[0].size() > maxp[0].size()) {
                        maxp = p2;
                    } else if (!p2.empty() && p2[0].size() == maxp[0].size()) {
                        for (auto li : p2) {
                            if (doesnt_have(maxp, li)) {
                                maxp.push_back(li);
                            }
                        }
                    }
                }
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty vector
    }
}

std::vector<std::vector<char*>> eat_max2_not_dama(char row, char col,BitmaskBoard board_layout,  vector<vector<char*>> parent_list, char color, char eat_direction) {
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);
    bool dama_nom = (eating_piece != 0);
    if (dama_nom) {
        std::vector<char*> new_list;
        std::vector<std::vector<char*>> big_dama_list;
        big_dama_list.clear();
        new_list.clear();
        for (auto move : valid_moves) {
            dama_nom = false;
            
            if (move[1] == col) {
                if (move[0] > row) {
                    eat_direction = 'd';
                } else {
                    eat_direction = 'u';
                }
            } else {
                if (move[1] > col) {
                    eat_direction = 'r';
                } else {
                    eat_direction = 'l';
                }
            }
            // Call get_valid_moves function
            auto eating_piece = get_valid_moves(move[0], move[1], color, 0, {}, board_layout, eat_direction).second;
            dama_nom = (eating_piece != 0);
    
            if (dama_nom) {
                char* parent = new char[2];  parent[0] = row; parent[1] = col;
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (parent[0] ==  value.back()[0] && parent[1] ==  value.back()[1]) {
                        std::vector<char*> big_dama = value;
                        big_dama.push_back(move);
                        big_dama_list.push_back(big_dama);
                    }
                }
            }
        }
        if (!big_dama_list.empty()) {
            parent_list = big_dama_list;
        } 
        if (new_list.empty()) {
            std::vector<std::vector<char*>> aux_list2;
            aux_list2.clear();
            char counter = 0;
            for (vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[char(value.size())-2];

                char sizeLast = char(sizeof(last)/sizeof(last[0]));
                char sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

                if (last[1] == pre_last[1]) {
                    if (last[0] > pre_last[0]) {
                        eat_direction = 'd';
                    } else {
                        eat_direction = 'u';
                    }
                } else {
                    if (last[1] > pre_last[1]) {
                        eat_direction = 'r';
                    } else {
                        eat_direction = 'l';
                    }
                }
                // Call get_valid_moves function
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0], last[1], color, 0, {}, board_layout, eat_direction);

                dama_nom = (eating_piece!=0);

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
                else
                    return parent_list;
            }
            parent_list = aux_list2;
            return parent_list;
            
        } else {
            valid_moves = new_list;
            std::vector<std::vector<char*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                BitmaskBoard board_layout2 = board_layout;
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color);
                auto p2 = eat_max2_not_dama(move[0], move[1], new_board_layout, parent_list, color, eat_direction);
                if(maxp.empty() || p2[0].size() > maxp[0].size())
                    maxp = p2;
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty vector
    }
}

bool passage_is_clear(BitmaskBoard board_layout, char row, char col, char turn) {
    if (turn == 1) {
        if (col == 7) {
            if (!board_layout.check_index_has_whitePawn(6, 7) && !board_layout.check_index_has_whitePawn(6, 6) && !board_layout.check_index_has_whiteKing(6, 7) && !board_layout.check_index_has_whiteKing(6, 6)) {
                return true;
            }
            return false;
        }
        if (col == 0) {
            if (!board_layout.check_index_has_whitePawn(6, 0) && !board_layout.check_index_has_whitePawn(6, 1) && !board_layout.check_index_has_whiteKing(6, 0) && !board_layout.check_index_has_whiteKing(6, 1)) {
                return true;
            }
            return false;
        }
        if ((board_layout.check_index_has_whitePawn(row + 1, col + 1) || board_layout.check_index_has_whiteKing(row + 1, col + 1)) ^
            (board_layout.check_index_has_whitePawn(row + 1, col - 1) || board_layout.check_index_has_whiteKing(row + 1, col - 1))) {
            return false;
        }
        for (char j = -2; j <= 2; j++) {
            if (col + j < 0) {
                continue;
            }
            if (col + j > 7) {
                break;
            }
            if (board_layout.check_index_has_whitePawn(row + 2, col + j) || board_layout.check_index_has_whiteKing(row + 2, col + j)) {
                return false;
            }
        }
        return true;
    }
    if (turn == 2) {
        if (col == 7) {
            if (!board_layout.check_index_has_blackPawn(1, 7) && !board_layout.check_index_has_blackPawn(1, 6) && !board_layout.check_index_has_blackKing(1, 7) && !board_layout.check_index_has_blackKing(1, 6)) {
                return true;
            }
            return false;
        }
        if (col == 0) {
            if (!board_layout.check_index_has_blackPawn(1, 0) && !board_layout.check_index_has_blackPawn(1, 1) && !board_layout.check_index_has_blackKing(1, 0) && !board_layout.check_index_has_blackKing(1, 1)) {
                return true;
            }
            return false;
        }
        if ((board_layout.check_index_has_blackPawn(row - 1, col + 1) || board_layout.check_index_has_blackKing(row - 1, col + 1)) ^
            (board_layout.check_index_has_blackPawn(row - 1, col - 1) || board_layout.check_index_has_blackKing(row - 1, col - 1))) {
            return false;
        }
        for (char j = -2; j <= 2; j++) {
            if (col + j < 0) {
                continue;
            }
            if (col + j > 7) {
                break;
            }
            if (board_layout.check_index_has_blackPawn(row - 2, col + j) || board_layout.check_index_has_blackKing(row - 2, col + j)) {
                return false;
            }
        }
        return true;
    }
    return false; // Return false for any other turn value
}

// bitwise optimized: tested
vector<char*> get_all_pieces(BitmaskBoard board_layout, char color)
{
    vector<char*> pieces = vector<char*>();
    char i, j;
    if(color == 1)
    {
        uint64_t black_pieces = board_layout.get_color_mask(1);
        for(i=7; i>=0; i--)
        {
            for(j=0; j<8; j++)
            {
                if(black_pieces & (1ULL << (i*8+j)))
                {
                    //make piece
                    char* piece = new char[2];
                    piece[0] = i;
                    piece[1] = j;
                    pieces.push_back(piece);
                }
            }
        }
    }
    else
    {
        uint64_t white_pieces = board_layout.get_color_mask(2);
        for(i=0; i<8; i++)
        {
            for(j=0; j<8; j++)
            {
                if(white_pieces & (1ULL << (i*8+j)))
                {
                    //make piece
                    char* piece = new char[2];
                    piece[0] = i;
                    piece[1] = j;
                    pieces.push_back(piece);
                }
            }
        }
    }
    return pieces;
}

pair< vector<char*>, bool > check_for_possible_capture(BitmaskBoard board_layout, char turn)
{
    vector<char*> pieces = get_all_pieces(board_layout, turn);
    for(auto piece:pieces){
        char row = piece[0];
        char col = piece[1];
        if((board_layout.check_index_has_blackKing(row, col) && turn==1) || (board_layout.check_index_has_whiteKing(row, col) && turn == 2))
        {
            char eating_piece = get_valid_moves(row, col, board_layout.get(row, col), 0, {}, board_layout, 0).second;
            if(eating_piece!=0)
                return make_pair(pieces, true);
        }
        else if(turn == 2){
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)))
                return make_pair(pieces, true);
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)))
                return make_pair(pieces, true);
            if(row-2>=0 && board_layout.get(row-2, col)==0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)))
                return make_pair(pieces, true);
        }
        else{
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)))
                return make_pair(pieces, true);
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)))
                return make_pair(pieces, true);
            if(row+2<8 && board_layout.get(row+2, col)==0 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)))
                return make_pair(pieces, true);
        }
    }
    return make_pair(pieces, false);
}

bool check_if_piece_can_capture(char row, char col, BitmaskBoard board_layout, char turn){
    if((board_layout.check_index_has_blackKing(row, col) && turn==1) || (board_layout.check_index_has_whiteKing(row, col) && turn == 2))
        {
            char eating_piece = get_valid_moves(row, col, board_layout.get(row, col), 0, {}, board_layout, 0).second;
            if(eating_piece!=0)
                return true;
        }
        else if(turn == 2){
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)))
                return true;
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)))
                return true;
            if(row-2>=0 && board_layout.get(row-2, col)==0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)))
                return true;
        }
        else{
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)))
                return true;
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)))
                return true;
            if(row+2<8 && board_layout.get(row+2, col)==0 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)))
                return true;
        }
    return false;
}

vector<char*> check_for_force(BitmaskBoard board_layout, char turn, vector<char*> *pieces = { })
{
    auto [pieces2, check] = check_for_possible_capture(board_layout, turn);
    *pieces = pieces2;
    if(check == false)
        return {};
    bool aktar_flag = false;
    vector<char*> aktar_list;
    vector<char*> force_list;
    char maxLength = 3, listSize = 0;

    for(char* piece : pieces2)
    {
        char row2=piece[0], col2=piece[1];
        if(!check_if_piece_can_capture(row2, col2, board_layout, turn))
            continue;
        char color = board_layout.get(row2, col2);
        if(((color==2 || color==4) && turn==2) || ((color==1 || color==3) && turn==1))
        {
            bool fff = false;
            vector<vector<char*>> parent_list2;
            
            //get_valid_moves stuff
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(row2, col2, color, 0, {}, board_layout, 0);
            
            char eating_piece = validMovesAndEatingPiece.second;
            bool dama_nom = eating_piece == 'd';
            fff = eating_piece!=0;

            if(fff)
            {
                vector<char*> valid_moves = validMovesAndEatingPiece.first;
                //vector<char*> parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                parent_list2 = vector<vector<char*>>();
                for(char* move : valid_moves)
                {
                    //parent_list2.append([(row2, col2), move])
                    char* temp = new char[2];
                    vector<char*> tempValue = vector<char*>();
                    temp[0] = row2; temp[1] = col2;
                    tempValue.push_back(temp);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }
                if(color == 1 || color == 2)
                {
                    vector<vector<char*>> parent_list;
                    parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                    if(!parent_list.empty())
                        listSize = parent_list[0].size();
                    if(!parent_list.empty() && listSize>=maxLength)
                    {
                        if(listSize>maxLength)
                            aktar_list.clear();
                        maxLength = listSize;
                        aktar_flag = true;
                        for(vector<char*> value : parent_list)
                            aktar_list.push_back(value[0]);
                        
                    }
                }
                else if (color == 3 || color == 4)
                {
                    vector<vector<char*>> parent_list;
                    // cout<<"parent_list2 size: "<<valid_moves.size()<<endl;
                    // for(auto value : parent_list2)
                        // cout<<value[0]<<value[1];
                    parent_list = eat_max2(row2, col2, board_layout, parent_list2, color, 0);
                    if(!parent_list.empty())
                        listSize = parent_list[0].size();
                    if(!parent_list.empty() && listSize>=maxLength)
                    {
                        if(listSize>maxLength)
                            aktar_list.clear();
                        maxLength = listSize;
                        aktar_flag = true;
                        for(vector<char*> value : parent_list)
                            aktar_list.push_back(value[0]);
                    }
                }

                if(!aktar_flag)
                {
                    char* piece = new char[2];
                    piece[0] = row2; piece[1] = col2;
                    force_list.push_back(piece);
                }
            }
        }
    }
    if(aktar_flag)
        force_list = aktar_list;
    return force_list;
}
/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

BitmaskBoard move_piece(char* piece, char* move, BitmaskBoard board_layout, vector<vector<char*>> parent_list, char color)
{
    char row = piece[0];
    char col = piece[1];

    char r = move[0];
    char c = move[1];

    char listSize = parent_list.size();
    char index=0;
    char value_length;

    if(!parent_list.empty())
    {

        for(vector<char*> value : parent_list)
        {
            value_length = value.size();
            auto temp = value[value_length-1];
            if(temp[0]==r && temp[1]==c)
            {
                char z=0;
                for(z = 0; z<value_length; z++)
                {
                    if(z+1 >= value_length) break;
                    board_layout = eat_piece_if_possible(board_layout, value[z][0], value[z][1], value[z+1][0], value[z+1][1], color).first;
                }
                break;
            }
            index++;
        }
        parent_list.erase(parent_list.begin()+index);
        if(value_length==0)
            return board_layout;
    }
    else
        board_layout = eat_piece_if_possible(board_layout, row, col, r, c, color).first;

    char aux;
    aux = board_layout.get(row, col);
    board_layout.set(row, col, board_layout.get(r, c));
    board_layout.set(r, c, aux);

    //check for dama promotion
    if(r==0 && color==2)
        board_layout.set_whiteKing(r, c);
    if(r==7 && color==1)
        board_layout.set_blackKing(r, c);
    
    return board_layout;
}

char evaluate_int2(BitmaskBoard board_layout, char turn)
{
    // auto [pieces, check] = check_for_possible_capture(board_layout, turn);
    // if(check){
    //     if(turn == 1){
    //         auto force_list = check_for_force(board_layout, 1, &pieces);
    //         if(force_list.size() == 1){
    //             auto forced_piece = force_list[0];
    //             auto [valid_moves, parent_list] = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]]);
    //             if(valid_moves.size()==1)
    //             {
    //                 auto forced_move = valid_moves[0];
    //                 board_layout = move_piece(forced_piece, forced_move, board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]]);
    //                 turn = 2;
    //             }
    //         }
    //     }else{
    //         auto force_list = check_for_force(board_layout, 2, &pieces);
    //         if(force_list.size() == 1){
    //             auto forced_piece = force_list[0];
    //             auto [valid_moves, parent_list] = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]]);
    //             if(valid_moves.size()==1)
    //             {
    //                 auto forced_move = valid_moves[0];
    //                 board_layout = move_piece(forced_piece, forced_move, board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]]);
    //                 turn = 1;
    //             }
    //         }
    //     }
    // }
    movesSeen++;
    char sum = 0, balance_black = 0, balance_red = 0, all_pieces=0, red_pieces=0, black_pieces = 0;
    for(char i=0; i<8; i++)
        for(char j=0; j<8; j++)
        {
            char piece = board_layout.get(i, j);

            if(piece == 1)
            {
                black_pieces+=1;
                if(j==0 || j==7)
                    sum+=7;
                //spreading pieces
                if(j<=3)
                    balance_black+=1;
                else
                    balance_black-=1;

                sum+=100; //piece = 1 pnt

                sum+=i; //the hiegher the better

                if(i == 6)
                {
                    sum+=150;
                    if(turn == 1)
                        sum+=40;
                }
                if(i==5)
                    sum+=40;
                if(i==4 && turn ==1)
                    sum+=30;
                    if(j==0 || j==7)
                        sum+=10;

                //4 important squares
                if(j==0 or j==7)
                {
                    if (i == 5 and turn == 1)
                        sum+=80;
                    if (i == 5 and turn == 2)
                        sum+=20;
                    if (i == 6 and turn == 1)
                        sum+=50;
                }

                //if on 5th row and passage is clear the piece is dangerous
                if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
                    sum+=70;
                
            }
            else if(piece == 2)
            {

                red_pieces+=1;
                if(j==0 || j==7)
                    sum-=7;
                //spreading pieces
                if(j<=3)
                    balance_red+=1;
                else
                    balance_red-=1;

                sum-=100; //piece = 1 pnt

                sum-=(7-i); //the hiegher the better

                if(i == 1)
                {
                    sum-=150;
                    if(turn == 2)
                        sum-=40;
                }
                if(i==2)
                    sum-=40;
                if(i==3 && turn ==2)
                    sum-=30;
                    if(j==0 || j==7)
                        sum-=10;

                //4 important squares
                if(j==0 or j==7)
                {
                    if (i == 2 and turn == 2)
                        sum-=80;
                    if (i == 2 and turn == 1)
                        sum-=20;
                    if (i == 1 and turn == 2)
                        sum-=50;
                }

                //if on 5th row and passage is clear the piece is dangerous
                if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
                    sum-=70;
            }
            else if(piece==3)
            {
                black_pieces+=1;
                sum+=450;
            }
            else if(piece==4)
            {
                red_pieces+=1;
                sum-=450;
            }

        }
    
    sum = sum - 100*abs(balance_black)/20 + 100*abs(balance_red)/20;

    all_pieces = black_pieces + red_pieces;

    if (red_pieces>black_pieces)
        sum += all_pieces/2;
    else if (black_pieces>red_pieces)
        sum -= all_pieces/2;

    if (black_pieces==1 and red_pieces==1)
        sum = 0;

    return sum;
}

int count_all_pieces(BitmaskBoard board_layout, char turn)
{
    int sum = 0;
    for(char i=0; i<8; i++)
        for(char j=0; j<8; j++)
        {
            char piece = board_layout.get(i, j);
            if(turn == 1){
                if(piece == 1 || piece == 3)
                    sum+=1;
            }
            else{
                if(piece == 2 || piece == 4)
                    sum+=1;
            }
        }
    return sum;
}

int evaluate_int(BitmaskBoard board_layout, char turn)
{
    movesSeen++;
    return board_layout.evaluate_board();

    // int sum = 0, balance_black = 0, balance_red = 0, all_pieces=0, red_pieces=0, black_pieces = 0;
    // int all_pieces_red = count_all_pieces(board_layout, 2);
    // int all_pieces_black = count_all_pieces(board_layout, 1);
    // int red_pawns = 0, black_pawns = 0;
    // int red_damas = 0, black_damas = 0;
    // for(char i=0; i<8; i++)
    //     for(char j=0; j<8; j++)
    //     {
    //         char piece = board_layout.get(i, j);
    //         if(piece == 0)
    //             continue;

    //         if(false && all_pieces_red + all_pieces_black <=9) //end game
    //         {
    //             if(piece == 1)
    //             {
    //                 black_pieces+=1;
    //                 black_pawns+=1;
    //                 sum+=100; //piece = 1 pnt
    //                 sum+=i; //the hiegher the better

    //                 if(i == 6)
    //                 {
    //                     sum+=150;
    //                     if(turn == 1)
    //                         sum+=40;
    //                 }
    //                 if(i==5)
    //                     sum+=40;
    //                 if(i==4 && turn ==1)
    //                     sum+=30;
    //                     if(j==0 || j==7)
    //                         sum+=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 5 and turn == 1)
    //                         sum+=80;
    //                     if (i == 5 and turn == 2)
    //                         sum+=20;
    //                     if (i == 6 and turn == 1)
    //                         sum+=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
    //                     sum+=70;
                    
    //             }
    //             else if(piece == 2)
    //             {
    //                 red_pawns+=1;
    //                 red_pieces+=1;
    //                 sum-=100; //piece = 1 pnt
    //                 sum-=(7-i); //the hiegher the better

    //                 if(i == 1)
    //                 {
    //                     sum-=150;
    //                     if(turn == 2)
    //                         sum-=40;
    //                 }
    //                 if(i==2)
    //                     sum-=40;
    //                 if(i==3 && turn ==2)
    //                     sum-=30;
    //                     if(j==0 || j==7)
    //                         sum-=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 2 and turn == 2)
    //                         sum-=80;
    //                     if (i == 2 and turn == 1)
    //                         sum-=20;
    //                     if (i == 1 and turn == 2)
    //                         sum-=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
    //                     sum-=70;
    //             }
    //             else if(piece==3)
    //             {
    //                 black_damas+=1;
    //                 black_pieces+=1;
    //                 sum+=350;
    //             }
    //             else if(piece==4)
    //             {
    //                 red_damas+=1;
    //                 red_pieces+=1;
    //                 sum-=350;
    //             }
    //         }
    //         else{
    //             if(piece == 1)
    //             {
    //                 black_pieces+=1;
    //                 if(j==0 || j==7)
    //                     sum+=7;
    //                 //spreading pieces
    //                 if(j<=3)
    //                     balance_black+=1;
    //                 else
    //                     balance_black-=1;

    //                 sum+=100; //piece = 1 pnt
    //                 sum+=i; //the hiegher the better

    //                 if(i == 6)
    //                 {
    //                     sum+=150;
    //                     if(turn == 1)
    //                         sum+=40;
    //                 }
    //                 if(i==5)
    //                     sum+=40;
    //                 if(i==4 && turn ==1)
    //                     sum+=30;
    //                     if(j==0 || j==7)
    //                         sum+=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 5 and turn == 1)
    //                         sum+=80;
    //                     if (i == 5 and turn == 2)
    //                         sum+=20;
    //                     if (i == 6 and turn == 1)
    //                         sum+=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
    //                     sum+=70;
                    
    //             }
    //             else if(piece == 2)
    //             {

    //                 red_pieces+=1;
    //                 if(j==0 || j==7)
    //                     sum-=7;
    //                 //spreading pieces
    //                 if(j<=3)
    //                     balance_red+=1;
    //                 else
    //                     balance_red-=1;

    //                 sum-=100; //piece = 1 pnt
    //                 sum-=(7-i); //the hiegher the better

    //                 if(i == 1)
    //                 {
    //                     sum-=150;
    //                     if(turn == 2)
    //                         sum-=40;
    //                 }
    //                 if(i==2)
    //                     sum-=40;
    //                 if(i==3 && turn ==2)
    //                     sum-=30;
    //                     if(j==0 || j==7)
    //                         sum-=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 2 and turn == 2)
    //                         sum-=80;
    //                     if (i == 2 and turn == 1)
    //                         sum-=20;
    //                     if (i == 1 and turn == 2)
    //                         sum-=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
    //                     sum-=70;
    //             }
    //             else if(piece==3)
    //             {
    //                 black_pieces+=1;
    //                 sum+=450;
    //                 // if(i==0 && j==0) //corners in home for dama are good
    //                 // {
    //                 //     if(board_layout.check_index_has_blackPawn(1, 0) && board_layout.check_index_has_blackPawn(2, 0) && board_layout.check_index_has_blackPawn(3, 0))
    //                 //         sum+=30;
    //                 //     if(board_layout.check_index_has_blackPawn(1, 0) && board_layout.check_index_has_blackPawn(2, 0))
    //                 //         sum+=20;
    //                 // }
    //                 // if(i==0 && j==7){
    //                 //     if(board_layout.check_index_has_blackPawn(1, 7) && board_layout.check_index_has_blackPawn(2, 7) && board_layout.check_index_has_blackPawn(3, 7))
    //                 //         sum+=30;
    //                 //     if(board_layout.check_index_has_blackPawn(1, 7) && board_layout.check_index_has_blackPawn(2, 7))
    //                 //         sum+=20;
    //                 // }
    //                 // if(i!=0 && i!=7 && j!=0 && j!=7) //dama is bad in center
    //                 //     sum+=10;
    //             }
    //             else if(piece==4)
    //             {
    //                 red_pieces+=1;
    //                 sum-=450;
    //                 // if(i==7 && j==7) //corners in home for dama are good
    //                 // {
    //                 //     if(board_layout.check_index_has_whitePawn(6, 7) && board_layout.check_index_has_whitePawn(5, 7) && board_layout.check_index_has_whitePawn(4, 7))
    //                 //         sum-=10;
    //                 //     if(board_layout.check_index_has_whitePawn(6, 7) && board_layout.check_index_has_whitePawn(5, 7))
    //                 //         sum-=5;
    //                 // }
    //                 // if(i==7 && j==0){
    //                 //     if(board_layout.check_index_has_whitePawn(6, 0) && board_layout.check_index_has_whitePawn(5, 0) && board_layout.check_index_has_whitePawn(4, 0))
    //                 //         sum-=10;
    //                 //     if(board_layout.check_index_has_whitePawn(6, 0) && board_layout.check_index_has_whitePawn(5, 0))
    //                 //         sum-=5;
    //                 // }
    //                 // if(i!=0 && i!=7 && j!=0 && j!=7) //dama is bad in center
    //                 //     sum-=10;
    //             }
    //         }
    //     }
    
    // // if(all_pieces_red + all_pieces_black <= 9){

    // //     if(black_damas==2 && red_damas==1 && red_pawns<=3){
    // //         sum = 0;
    // //     }
    // //     if(red_damas==2 && black_damas==1 && black_pawns<=3){
    // //         sum = 0;
    // //     }
    // // }

    //     sum = sum - 100*abs(balance_black)/20 + 100*abs(balance_red)/20;

    //     all_pieces = black_pieces + red_pieces;

    //     if (red_pieces>black_pieces)
    //         sum += all_pieces/2;
    //     else if (black_pieces>red_pieces)
    //         sum -= all_pieces/2;
    

    // if (black_pieces==1 && red_pieces==1)
    //     sum = 0;

    // return sum;
}

pair<vector<BitmaskBoard>, bool> get_all_moves(BitmaskBoard board_layout, char color, vector<BitmaskBoard> *moves = { })
{
    vector<char *> pieces;
    counter++;
    //create force list
    vector<char *> force_list;
    vector<char *> valid_moves;
    bool forceListEmpty = false;
    vector<vector<char *>> parent_list;
    bool dama_nom = false, fff = false;

    force_list = check_for_force(board_layout, color, &pieces);

    if(!force_list.empty())
        pieces = force_list;
    else
        forceListEmpty = true;
    
    for(char* piece : pieces)
    {
        char color2 = board_layout.get(piece[0], piece[1]);
        fff = dama_nom = false;
        if(color2 == 1 || color2 == 2)
        {
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            fff = eating_piece!=0;

            if(fff)
            {
                vector<vector<char*>> parent_list2;
                for(char* move : valid_moves)
                {
                    vector<char*> tempValue = vector<char*>();
                    tempValue.push_back(piece);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }

                parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                vector<char*> aux_list;
                if(!parent_list.empty())
                {
                    for(vector<char*> value: parent_list)
                        aux_list.push_back(value[char(value.size()) - 1]);
                    valid_moves = aux_list;
                }

            }
        }

        if (color2 ==3 || color2 == 4)
        {
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            dama_nom = eating_piece=='d';

            if(dama_nom)
            {
                vector<vector<char*>> parent_list2;
                for(char* move : valid_moves)
                {
                    vector<char*> tempValue = vector<char*>();
                    tempValue.push_back(piece);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }

                parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                vector<char*> aux_list;
                if(!parent_list.empty())
                {
                    for(vector<char*> value: parent_list)
                        aux_list.push_back(value[char(value.size()) - 1]);
                    valid_moves = aux_list;
                }
            }
        }

        for(char* move : valid_moves)
        {
            // moves.push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            BitmaskBoard board_layout2 = board_layout;
            moves->push_back(move_piece(piece, move, board_layout2, parent_list, color2));
        }
    }

    return make_pair(*moves, forceListEmpty);
}

void printBoard(BitmaskBoard board_layout)
{
    for(char i=0; i<8; i++)
    {
        for(char j=0;j<8; j++)
        {
            std::cout<<int(board_layout.get(i, j))<<" ";
        }
        std::cout<<endl;
    }
}

bool playerWon(BitmaskBoard board_layout){
    bool red = false, black = false;
    for(char i=0; i<8; i++)
        for(char j=0; j<8; j++)
        {
            if(board_layout.check_index_has_blackPawn(i, j) || board_layout.check_index_has_blackKing(i, j))
                black = true;
            if(board_layout.check_index_has_whitePawn(i, j) || board_layout.check_index_has_whiteKing(i, j))
                red = true;
            if(red && black)
                return false;
        }
    return true;
}

pair<int, BitmaskBoard> normal_minimax_based(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            // cout<<"found in hash table"<<endl;
            cacheHits++;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        
        vector<char *> pieces;
        counter++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 1, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }

                }
            }

            if (color2 ==3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);
                evaluation = normal_minimax_based(depth-1, false, current_board, alpha, beta).first;
                if(evaluation>maxEval)
                {
                    maxEval = evaluation;
                    best_move = current_board;
                }

                alpha = max(alpha, maxEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        vector<char *> pieces;
        counter++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 2, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }

                }
            }

            if (color2 ==3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);
                evaluation = normal_minimax_based(depth-1, true, current_board, alpha, beta).first;
                if(evaluation<minEval)
                {
                    minEval = evaluation;
                    best_move = current_board;
                }

                beta = min(beta, minEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return make_pair(minEval, best_move);
    }
}

//function to get all moves(8x8 boards) sevaluate them using evalueate_int and sort them based on the evaluation
void sort_moves(vector<BitmaskBoard> *moves, char turn)
{
    vector<pair<int, BitmaskBoard>> moves_and_eval;
    for(BitmaskBoard move : *moves)
    {
        int eval = evaluate_int(move, turn);
        // transpositionTable[calculateHashKey(move, turn)] = std::make_pair(1, eval);
        moves_and_eval.push_back(make_pair(eval, move));
    }
    if(turn == 1)
        sort(moves_and_eval.begin(), moves_and_eval.end(), greater<pair<int, BitmaskBoard>>());
    else
        sort(moves_and_eval.begin(), moves_and_eval.end());
    moves->clear();
    for(pair<int, BitmaskBoard> move : moves_and_eval)
    {
        // cout<<move.first<<" ";
        moves->push_back(move.second);
    }
    // cout<<endl;
}

std::pair<int, BitmaskBoard> minimax_pro2_hash(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta, char akel_depth, char akel_player, char akling)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            cacheHits++;
            // cout<<"found in hash table"<<endl;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if (depth <= 0)
    {
        char turn;
        if (max_player)
            turn = 1;
        else
            turn = 2;
        int eval = evaluate_int(board_layout, turn);
        transpositionTable[hashKey] = std::make_pair(0, eval);
        return std::make_pair(eval, board_layout);
    }

    if (max_player)
    {
        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool> allandForce = get_all_moves(board_layout, 1, &all_moves);
        sort_moves(&all_moves, 1);
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth < 5)
                evaluation = minimax_pro2_hash(depth, false, move, alpha, beta, akel_depth + 1, false, true).first;
            else
            {
                if (akel_player == false && akel_depth > 2)
                    evaluation = minimax_pro2_hash(0, false, move, alpha, beta, 100, false, false).first;
                else
                    evaluation = minimax_pro2_hash(depth - 1, false, move, alpha, beta, 0, false, false).first;
            }

            if (evaluation > maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = std::max(alpha, maxEval);
            if (beta <= alpha)
                break;
        }

        transpositionTable[hashKey] = std::make_pair(depth, maxEval);

        return std::make_pair(maxEval, best_move);
    }
    else
    {
        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2, &all_moves);
        sort_moves(&all_moves, 2);
        all_moves = allandForce.first;
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth < 5)
                evaluation = minimax_pro2_hash(depth, true, move, alpha, beta, akel_depth + 1, true, true).first;
            else
            {
                if ((akel_player == true) && akel_depth > 2)
                    evaluation = minimax_pro2_hash(0, true, move, alpha, beta, 100, true, false).first;
                else
                    evaluation = minimax_pro2_hash(depth - 1, true, move, alpha, beta, 0, true, false).first;
            }

            if (evaluation < minEval)
            {
                minEval = evaluation;
                best_move = move;
            }

            beta = std::min(beta, minEval);
            if (beta <= alpha)
                break;
        }

        transpositionTable[hashKey] = std::make_pair(depth, minEval);

        return std::make_pair(minEval, best_move);
    }
}

pair<int, BitmaskBoard> minimax_pro2(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta, char akel_depth, char akel_player, char akling)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool> allandForce = get_all_moves(board_layout, 1, &all_moves);
        // sort_moves(&all_moves, 1);
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth<5)
                evaluation = minimax_pro2(depth, false, move, alpha, beta, akel_depth+1, false, true).first;
            else
            {
                if( akel_player == false && akel_depth>2)
                    evaluation = minimax_pro2(0, false, move, alpha, beta, 100, false, false).first;
                else
                    evaluation = minimax_pro2(depth-1, false, move, alpha, beta, 0, false, false).first;
            }

            if(evaluation>maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = max(alpha, maxEval);
            if(beta<=alpha)
                break;
        }
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2, &all_moves);
        // sort_moves(&all_moves, 2);
        all_moves = allandForce.first;
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth<5)
                evaluation = minimax_pro2(depth, true, move, alpha, beta, akel_depth+1, true, true).first;
            else
            {
                if( (akel_player == true) && akel_depth>2)
                    evaluation = minimax_pro2(0, true, move, alpha, beta, 100, true, false).first;
                else
                    evaluation = minimax_pro2(depth-1, true, move, alpha, beta, 0, true, false).first;
            }

            if(evaluation<minEval)
            {
                minEval = evaluation;
                best_move = move;
            }

            beta = min(beta, minEval);
            if(beta<=alpha)
                break;
        }
        return make_pair(minEval, best_move);
    }
}

pair<int, BitmaskBoard> normal_minimax(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            // cout<<"found in hash table"<<endl;
            cacheHits++;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 1, &all_moves);
        bool force_list = allandForce.second;

        for (auto move : all_moves)
        {
            evaluation = normal_minimax(depth-1, false, move, alpha, beta).first;
            if(evaluation>maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = max(alpha, maxEval);
            if(beta<=alpha)
                break;

        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2, &all_moves);
        all_moves = allandForce.first;
        bool force_list = allandForce.second;

        for (auto move : all_moves)
        {
            evaluation = normal_minimax(depth-1, true, move, alpha, beta).first;
            if(evaluation<minEval)
            {
                minEval = evaluation;
                best_move = move;
            }

            beta = min(beta, minEval);
            if(beta<=alpha)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return make_pair(minEval, best_move);
    }
}

pair<int, BitmaskBoard> minimax_pro2_based(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta, char akel_player, char akel_depth, char akling)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            // if(depth>4)
            //     cout<<int(depth)<<endl;
            // cout<<"found in hash table"<<endl;
            cacheHits++;
            movesSeen++;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if(depth <=0)
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        int eval = evaluate_int(board_layout, turn);
        // transpositionTable[hashKey] = std::make_pair(0, eval);
        return std::make_pair(eval, board_layout);
        // return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        
        vector<char *> pieces;
        counter++;
        movesSeen++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 1, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    bool fff2 = false;
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                        if(check_if_piece_can_capture(move[0], move[1], board_layout, color2))
                            fff2 = true;
                    }

                    if(fff2)
                    {
                        parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                        vector<char*> aux_list;
                        if(!parent_list.empty())
                        {
                            for(vector<char*> value: parent_list)
                                aux_list.push_back(value[char(value.size()) - 1]);
                            valid_moves = aux_list;
                        }
                    }

                }
            }

            if (color2 ==3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    bool dama_nom2 = false;
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);

                        auto eat_direction = get_eat_direction(piece[0], piece[1], move[0], move[1]);

                        if(check_dama_has_akel(move[0], move[1], color2, board_layout, eat_direction))
                            dama_nom2 = true;
                        // cout<<move[0]<<" "<<move[1]<<endl;
                    }
                    if(dama_nom2)
                    {
                        parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                        vector<char*> aux_list;

                        if(!parent_list.empty())
                        {
                            for(vector<char*> value: parent_list)
                                aux_list.push_back(value[char(value.size()) - 1]);
                            
                            valid_moves = aux_list;
                        }
                    }   
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);

                if (!force_list.empty() && akel_depth<5)
                    evaluation = minimax_pro2_based(depth, false, current_board, alpha, beta, akel_depth+1, false, true).first;
                else
                {
                    if(akel_player == false && akel_depth>2)
                        evaluation = minimax_pro2_based(0, false, current_board, alpha, beta, 100, false, false).first;
                    else
                        evaluation = minimax_pro2_based(depth-1, false, current_board, alpha, beta, 0, false, false).first;
                }

                if(evaluation>maxEval)
                {
                    maxEval = evaluation;
                    best_move = current_board;
                }

                alpha = max(alpha, maxEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);
        movesSeen++;

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        vector<char *> pieces;
        counter++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 2, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    bool fff2 = false;
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                        if(check_if_piece_can_capture(move[0], move[1], board_layout, color2))
                            fff2 = true;
                    }

                    if(fff2)
                    {
                        parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                        vector<char*> aux_list;
                        if(!parent_list.empty())
                        {
                            for(vector<char*> value: parent_list)
                                aux_list.push_back(value[char(value.size()) - 1]);
                            valid_moves = aux_list;
                        }
                    }
                    
                }
            }

            if (color2 == 3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    bool dama_nom2 = false;
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);

                        auto eat_direction = get_eat_direction(piece[0], piece[1], move[0], move[1]);

                        if(check_dama_has_akel(move[0], move[1], color2, board_layout, eat_direction))
                            dama_nom2 = true;
                    }
                    if(dama_nom2)
                    {
                        parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                        vector<char*> aux_list;
                        if(!parent_list.empty())
                        {
                            for(vector<char*> value: parent_list)
                                aux_list.push_back(value[char(value.size()) - 1]);
                            valid_moves = aux_list;
                        }
                    }
                    
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);
                if (!force_list.empty() && akel_depth<5)
                    evaluation = minimax_pro2_based(depth, true, current_board, alpha, beta, akel_depth+1, true, true).first;
                else
                {
                    if( (akel_player == true) && akel_depth>2)
                        evaluation = minimax_pro2_based(0, true, current_board, alpha, beta, 100, true, false).first;
                    else
                        evaluation = minimax_pro2_based(depth-1, true, current_board, alpha, beta, 0, true, false).first;
                }

                if(evaluation<minEval)
                {
                    minEval = evaluation;
                    best_move = current_board;
                }

                beta = min(beta, minEval);
                if(beta<=alpha){
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return make_pair(minEval, best_move);
    }
}

/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

int main()
{
    // initializeLookupTable();
    // initializeZobristTable();

    // sombolvsSombol(100, 8);
    double time_spent = 0.0;

    vector<vector<char*>> parent_list2;
    char a[2] = {6, 0};
    char b[2] = {4, 0};
    vector<char*> v;
    v.push_back(a);
    v.push_back(b);
    parent_list2.push_back(v);

    char* test2[8];  
    std::ifstream file("minimaxResult.txt");
    if (file.is_open()) {
        for (int i = 0; i < 8; ++i) {
            test2[i] = new char[8];
            for (int j = 0; j < 8; ++j) {
                if (!(file >> test2[i][j])) {
                    // Handle error if unable to read a character
                    std::cerr << "Error reading from file.\n";
                    return 1;
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file.\n";
        return 1;
    }

    auto test3 = test2;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            test2[i][j]-='0';
        }
    pair<int, BitmaskBoard> minimaxResult;
    

    
    BitmaskBoard curr_board;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            curr_board.set(i, j, test2[i][j]);
    //fetching TT from  file
    // loadTranspositionTableFromFile(transpositionTable, "transposition_table.txt");

    clock_t begin = clock();
    // for(char i=0; i<1000000; i++)
    //     deepcopy2_char(test2);
    // minimaxResult = minimax_pro2(6, true, test2, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = minimax_pro2_hash(6, true, test2, INT_MIN, INT_MAX, 0, true, false);
    minimaxResult = minimax_pro2_based(7, true, curr_board, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = normal_minimax_based(9, true, test2, INT_MIN, INT_MAX);
    // minimaxResult = normal_minimax(9, true, curr_board, INT_MIN, INT_MAX);

    // save TT to file
    // writeTranspositionTableToFile(transpositionTable, "transposition_table.txt");

    //start
    clock_t end = clock();
    std::cout<<"Evaluation: "<<minimaxResult.first/100.0<<endl;
    BitmaskBoard boardResult = minimaxResult.second;
    printBoard(boardResult);
    std::cout<<"Positions Seen: "<<movesSeen<<endl;

    std::ofstream outfile("minimaxResult.txt");
    if (!outfile.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }

    // convert to char**
    char* curr_boardResult[8];
    for(int i=0; i<8; i++)
        curr_boardResult[i] = new char[8];
    
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            curr_boardResult[i][j] = boardResult.get(i, j);

    for(int i=0; i<8; i++)
    {
        for(int j=0;j<8; j++)
        {
            outfile << curr_boardResult[i][j]+'0'-'0';
            outfile<<" ";
        }
        outfile<<endl;
    }

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"Positions/sec: "<<movesSeen/time_spent<<endl;
    std::cout<<"get_all_moves: "<<counter<<endl;
    std::cout<<"cacheHits: "<<cacheHits<<endl;
    std::cout<<"Time: "<<time_spent<<endl<<endl;
    //end

    return 0;
}

