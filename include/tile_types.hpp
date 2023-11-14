#pragma once

enum StaticObject
{
    EMPTY = 0,
    WALL_MUD = 1,
    DOOR = 2,
    WALL_METAL = 3,
    JETPACK = 4,
    WALL_BLUE = 5,
    FIRE_1 = 6,
    FIRE_2 = 7,
    FIRE_3 = 8,
    FIRE_4 = 9,
    TROPHY_1 = 10,
    TROPHY_2 = 11,
    TROPHY_3 = 12,
    TROPHY_4 = 13,
    TROPHY_5 = 14,
    PIPE_RIGHT = 15,
    PIPE_DOWN = 16,
    WALL_RED = 17,
    WALL_MUD_2 = 18,
    WALL_LIGHT_BLUE = 19,
    GUN = 20,
    WALL_HALF_1 = 21,
    WALL_HALF_2 = 22,
    WALL_HALF_3 = 23,
    WALL_HALF_4 = 24,
    WEED_1 = 25,
    WEED_2 = 26,
    WEED_3 = 27,
    WEED_4 = 28,
    DIVIDER_PURPLE = 29,
    PLATFORM = 30,
    PLATFORM_FAKE = 31,
    GRASS_1 = 32,
    TREE_TRUNK = 33,
    GRASS_2 = 34,
    GRASS_3 = 35,
    WATER_1 = 36,
    WATER_2 = 37,
    WATER_3 = 38,
    WATER_4 = 39,
    WATER_5 = 40,
    STARS = 41,
    MOON = 42,
    TREE_TOP_LEFT = 43,
    TREE_TOP_RIGHT = 44,
    TREE_BOTTOM_LEFT = 45,
    TREE_BOTTOM_RIGHT = 46,
    POINT_BLUE = 47,
    POINT_PURPLE = 48,
    POINT_RED = 49,
    POINT_CROWN = 50,
    POINT_RING = 51,
    POINT_WAND = 52
};

enum PlayerObject
{
    PLAYER_WALK_R_1 = 53,
    PLAYER_WALK_R_2 = 54,
    PLAYER_WALK_R_3 = 55,

    PLAYER_FRONT = 56,

    PLAYER_WALK_L_1 = 57,
    PLAYER_WALK_L_2 = 58,
    PLAYER_WALK_L_3 = 59,

    PLAYER_WALK_R_1_XOR = 60,
    PLAYER_WALK_R_2_XOR = 61,
    PLAYER_WALK_R_3_XOR = 62,

    PLAYER_FRONT_XOR = 63,

    PLAYER_WALK_L_1_XOR = 64,
    PLAYER_WALK_L_2_XOR = 65,
    PLAYER_WALK_L_3_XOR = 66,

    PLAYER_JUMP_R = 67,
    PLAYER_JUMP_L = 68,
    PLAYER_JUMP_R_XOR = 69,
    PLAYER_JUMP_L_XOR = 70,

    PLAYER_CLIMB_1 = 71,
    PLAYER_CLIMB_2 = 72,
    PLAYER_CLIMB_3 = 73,
    PLAYER_CLIMB_1_XOR = 74,
    PLAYER_CLIMB_2_XOR = 75,
    PLAYER_CLIMB_3_XOR = 76,

    PLAYER_JETPACK_R_1 = 77,
    PLAYER_JETPACK_R_2 = 78,
    PLAYER_JETPACK_R_3 = 79,

    PLAYER_JETPACK_L_1 = 80,
    PLAYER_JETPACK_L_2 = 81,
    PLAYER_JETPACK_L_3 = 82,

    PLAYER_JETPACK_R_1_XOR = 83,
    PLAYER_JETPACK_R_2_XOR = 84,
    PLAYER_JETPACK_R_3_XOR = 85,

    PLAYER_JETPACK_L_1_XOR = 86,
    PLAYER_JETPACK_L_2_XOR = 87,
    PLAYER_JETPACK_L_3_XOR = 88,

    PLAYER_BULLET_R = 127,
    PLAYER_BULLET_L = 128
};

enum EnemyObject
{
    // Level 3
    SPIDER_1 = 89,
    SPIDER_2 = 90,
    SPIDER_3 = 91,
    SPIDER_4 = 92,

    // Level 4
    BLADE_1 = 93,
    BLADE_2 = 94,
    BLADE_3 = 95,
    BLADE_4 = 96,

    // Level 5
    RED_SUN_1 = 97,
    RED_SUN_2 = 98,
    RED_SUN_3 = 99,
    RED_SUN_4 = 100,

    // Level 6
    BATON_1 = 101,
    BATON_2 = 102,
    BATON_3 = 103,
    BATON_4 = 104,

    // Level 7
    DISC_1 = 105,
    DISC_2 = 106,
    DISC_3 = 107,
    DISC_4 = 108,

    // Level 8
    UFO_1 = 109,
    UFO_2 = 110,
    UFO_3 = 111,
    UFO_4 = 112,

    // Level 9
    BALL_1 = 113,
    BALL_2 = 114,
    BALL_3 = 115,
    BALL_4 = 116,

    // Level 10
    DISC_VERT_1 = 117,
    DISC_VERT_2 = 118,
    DISC_VERT_3 = 119,
    DISC_VERT_4 = 120,

    ENEMY_BULLET_R_1 = 121,
    ENEMY_BULLET_R_2 = 122,
    ENEMY_BULLET_R_3 = 123,

    ENEMY_BULLET_L_1 = 124,
    ENEMY_BULLET_L_2 = 125,
    ENEMY_BULLET_L_3 = 126,
};

enum MiscObject
{
    DEATH_1 = 129,
    DEATH_2 = 130,
    DEATH_3 = 131,
    DEATH_4 = 132,
    TEXT_JETPACK = 133,
    TEXT_GUN = 134,
    TEXT_LIVES = 135, // DAVES:
    TEXT_LEVEL = 136,
    TEXT_SCORE = 137,
    TEXT_CANEXIT = 138,
    TEXT_WARP = 139,
    TEXT_ZONE = 140,
    JETPACK_METER = 141,
    JETPACK_UNIT = 142,
    LIFE_UNIT = 143,
    BANNER_1 = 144,
    BANNER_2 = 145,
    BANNER_3 = 146,
    BANNER_4 = 147,
    DIGIT_ZERO = 148,
    DIGIT_ONE = 149,
    DIGIT_TWO = 150,
    DIGIT_THREE = 151,
    DIGIT_FOUR = 152,
    DIGIT_FIVE = 153,
    DIGIT_SIX = 154,
    DIGIT_SEVEN = 155,
    DIGIT_EIGHT = 156,
    DIGIT_NINE = 157
};

enum DIR
{
    UNSET = -1,
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
};