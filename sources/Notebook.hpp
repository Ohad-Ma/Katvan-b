/**
 * @file Notebook.hpp
 * @author Ohad Maday (Maday.ohad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "Direction.hpp"
#include <string>
#include <unordered_map>
#include <bits/stdc++.h>
#include <stdexcept>

#define START_SIZE 0;
const int MAX_ALLOWED_LENGTH = 100; // Maximum COL length
const int MAX_ALLOWED_CHAR = 126;   // Used for ASCII chars
const int MIN_ALLOWED_CHAR = 32;    // Same as above

const ariel::Direction HORI = ariel::Direction::Horizontal; // Instead of writing again and again..
const ariel::Direction VERT = ariel::Direction::Vertical;   // Same as above

/**
 * @brief Each tile of the page is a char
 * 
 */
struct default_tile{ 
    char c = '_'; // Default char is '_'
};

namespace ariel{
/**
 * @brief A Notebook class, that has:
 *              unordered map as Data Structure 
 *              minimum row index
 *              maximum row index
 *              minimum col index
 *              maximum col index
 *              minimum page number
 *              maximum page number
 * 
 * 4 methods:
 *              write - write on a page 
 *              read  - read from a page
 *              erase - erase a specific line on the page
 *              show  - show a certain page
 * 
 */
    class Notebook{
        int min_row_idx;   // Minimum row index
        int max_row_idx;   // Maximum row index 
        int min_col_idx;   // Minimum columns index
        int max_col_idx;   // Maximum columns index
        int min_page;      // Minimum page
        int max_page;      // Maximum page

        std::unordered_map<int,std::unordered_map<int,std::unordered_map<int,default_tile>>> notebook; // Notebook
        
        public:
        Notebook(); // Default constructor 

        void write(int PAGE, int ROW, int COL, Direction dir, const std::string &st);

        std::string read(int PAGE, int ROW, int COL, ariel::Direction dir, int len);

        void erase(int PAGE, int ROW, int COL, ariel::Direction dir, int len);

        void show(int page);

    };
    // Functions are here and not class-strict so EVERYONE can use them wiii!!
    void BadChars(const std::string& str); // Check for bad chars that are not supposed to be written such as '~','\n',..
    void ValChecker(int PAGE, int ROW, int COL,ariel::Direction dir, int LEN); // Check that these arguments aren't negative
    int CountDigits(int number); // Count amount of digits - used in show() method
}