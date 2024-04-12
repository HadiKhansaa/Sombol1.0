#pragma once
#include <cstdint>
#include <iostream>
#include "robin_hood.h"

class Piece {
private:
    uint8_t data;

    static const uint8_t rowMask = 0xE0; // 1110 0000
    static const uint8_t colMask = 0x1C; // 0001 1100
    static const uint8_t colorMask = 0x03; // 0000 0011

public:
    Piece() : data(0) {}

    Piece(uint8_t row, uint8_t col, uint8_t color) : data(0) {
        setPosition(row, col);
        setColor(color);
    }

    void setPosition(uint8_t row, uint8_t col) {
        data = (data & ~rowMask) | ((row << 5) & rowMask);
        data = (data & ~colMask) | ((col << 2) & colMask);
    }

    void setColor(uint8_t color) {
        --color;
        data = (data & ~colorMask) | (color & colorMask);
    }

    uint8_t getRow() const {
        return (data & rowMask) >> 5;
    }

    uint8_t getCol() const {
        return (data & colMask) >> 2;
    }

    uint8_t getColor() const {
        return (data & colorMask) + 1;
        
    }

    // overaride the == operator
    bool operator==(const Piece& other) const {
        return data == other.data;
    }

    // hash function
    uint64_t hash() const {
        return data;
    }
};

class Move {
private:
    uint8_t from;
    uint8_t to;

    static const uint8_t rowMask = 0xE0; // 1110 0000
    static const uint8_t colMask = 0x1C; // 0001 1100

public:
    Move() : from(0), to(0) {}

    Move(uint8_t fromRow, uint8_t fromCol, uint8_t toRow, uint8_t toCol) {
        setPosition(from, fromRow, fromCol);
        setPosition(to, toRow, toCol);
    }

    void setPosition(uint8_t& position, uint8_t row, uint8_t col) {
        position = ((row << 5) & rowMask) | ((col << 2) & colMask);
    }

    void setFrom(uint8_t row, uint8_t col) {
        setPosition(from, row, col);
    }

    void setTo(uint8_t row, uint8_t col) {
        setPosition(to, row, col);
    }

    uint8_t getFromRow() const {
        return (from & rowMask) >> 5;
    }

    uint8_t getFromCol() const {
        return (from & colMask) >> 2;
    }

    uint8_t getToRow() const {
        return (to & rowMask) >> 5;
    }

    uint8_t getToCol() const {
        return (to & colMask) >> 2;
    }

    // overaride the == operator
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to;
    }

    // hash function
    uint64_t hash() const {
        return (uint64_t(from) << 8) | to;
    }
};

namespace robin_hood {
    template<>
    struct hash<Move> {
        std::size_t operator()(const Move& move) const noexcept {
            return move.hash(); // Use the hash method of BitmaskBoard
        }
    };
}

namespace robin_hood {
    template<>
    struct hash<Piece> {
        std::size_t operator()(const Piece& piece) const noexcept {
            return piece.hash(); // Use the hash method of BitmaskBoard
        }
    };
}



