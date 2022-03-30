/**
 * @file Notebook.cpp
 * @author Ohad Maday (Maday.ohad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Notebook.hpp"

namespace ariel{

    /**
     * @brief Construct a new Notebook:: Notebook object
     * > Initiate ALL indexes the 0 and starting page is 0 as requested.
     * 
     */
    Notebook::Notebook(){
        this->min_row_idx = START_SIZE; // Starting row index
        this->max_row_idx = START_SIZE; // Ending row index
        this->min_col_idx = START_SIZE; // Starting col index
        this->max_col_idx = START_SIZE; // Ending col index
        this->min_page    = START_SIZE; // Minimum page
        this->max_page    = START_SIZE; // Current maximum page 
    }

    void Notebook::write(int PAGE, int ROW, int COL, Direction dir, const std::string &st){
        std::string temp = st;
        
        ValChecker(PAGE,ROW,COL,dir,0); // Check for Negative rows,page,col

        BadChars(st); // Check if the string contains bad chars in it

        const int string_length = st.length();

        // Update both min page and max page
        if (this->min_page > PAGE){
            this->min_page = PAGE;
        }

        if (this->max_page < PAGE){
            this->max_page = PAGE;
            Notebook(); // We are in a new page -  initiate all indexes again
        }

        // Update minimum/maximum Row & Col indexes
        if(this->min_col_idx > COL){
            this->min_col_idx = COL;
        }
        if(this->min_row_idx > ROW){
            this->min_row_idx = ROW;
        }
        if (this->max_row_idx < ROW){
            this->max_row_idx = ROW;
        }
        if (this->max_col_idx < COL){
            this->max_col_idx = COL;
        }

        int new_length = 0;
        if (dir == HORI){ // Write horizontally
            new_length = string_length + COL;
            if (new_length >= MAX_ALLOWED_LENGTH){ // Can't pass 100 words in the same row 
                throw std::invalid_argument("String too long to be written horizontally");
            }
            // Update maximum row index 
            if (this->max_col_idx < new_length){
                this->max_col_idx = new_length;
            }
        }else { // Write vertically
            new_length = string_length + ROW;
            // Update Maximum row & col indexes 
            if (this->max_row_idx < new_length){
                this->max_row_idx = new_length;
            }
        }

        unsigned long i = 0;
        while (i < string_length) {  
            char current = this->notebook[PAGE][ROW][COL].c; 
            if (current == '_'){ // Only if tile is empty (holds '_')
                this->notebook[PAGE][ROW][COL].c = temp[i];
                dir == HORI ? COL++ : ROW++; // Writing Horizontally or Vertically and increment each
            }else{ // Tile is not empty
                throw std::invalid_argument("Cannot write on erased/written place!");
            }
            i++;
        }

    }

    std::string Notebook::read(int PAGE, int ROW, int COL, Direction dir, int len){ 
        
        ValChecker(PAGE,ROW,COL,dir,len); // Validate arguments

        std::string sentence; // The returned string
        
        if (dir == HORI && (len + COL > MAX_ALLOWED_LENGTH)){
            throw std::invalid_argument("Trying to read too much horizontally");
        }
        int i = 0;
        while(i < len){ // Walk over the length of the wanted word
            sentence.push_back(this->notebook[PAGE][ROW][COL].c); // Push it into returned string 
            dir == HORI ? COL++ : ROW++; // Reading Horizontally or Vertically
            i++;
        }
        return sentence;
    }


 void Notebook::show(int page){
        if(page < 0){ // Page cannot be negative (Ever seen a folder with negative amount of pages?)
            throw std::invalid_argument("Negative page number");
        }

            {   // Cool assignment logo
                std::cout<< R"( 
                        _   ______  ________________  ____  ____  __ __
                       / | / / __ \/_  __/ ____/ __ )/ __ \/ __ \/ //_/
                      /  |/ / / / / / / / __/ / __  / / / / / / / ,<   
                     / /|  / /_/ / / / / /___/ /_/ / /_/ / /_/ / /| |  
                    /_/ |_/\____/ /_/ /_____/_____/\____/\____/_/ |_|
                )" << "\n";
            }
            std::cout << "\t\t==Page No. " << page << " ==============C++ 5782================= \n";
        
        int amount_of_digits = 3; // The number 126 has 3 digits
        for(int i = 1; i<=MAX_ALLOWED_CHAR; i++){
            /*  - Lines gaps - 
            *       3 - 0 = 3
            *       3 - 1 = 2
            *       3 - 2 = 1
            */  

            int diff = amount_of_digits - (CountDigits(i) - 1); // The gap from all numbers is the same
                std::cout << i << std::setw(diff) << ':'<<" ";
                for(int j = 0; j<MAX_ALLOWED_LENGTH; j++){
                    std::cout<<this->notebook[page][i][j].c;
                }
                std::cout<<"\n";
        }
    }
    
    void Notebook::erase(int PAGE, int ROW, int COL, Direction dir, int len){
        ValChecker(PAGE,ROW,COL,dir,len); // Valdiates values 

        // Update minimum Col and Row indexes
        if(this->min_col_idx > COL){
            this->min_col_idx = COL;
        }
        if(this->min_row_idx > ROW){
            this->min_row_idx = ROW;
        }

        // Update maximum col and Row indexes
        if (this->max_row_idx < ROW){
            this->max_row_idx = ROW;
        }
        if (this->max_col_idx < COL){
            this->max_col_idx = COL;
        }

        if (dir == HORI) { // Direction is Horizontally
            if (COL + len > MAX_ALLOWED_LENGTH)
            {
                throw std::invalid_argument("Cannot erase horizontally beyond the limit");
            }
            // Update horizontally maximum indexes 
            if (this->max_col_idx > (COL - len)){ 
                this->max_col_idx = COL - len;
            }
        }else { // Update vertically maximum indexes 
            if (this->max_row_idx > (ROW - len)){
                this->max_row_idx = ROW - len;
            }
        }

        int i = 0;
        while (i < len){
            this->notebook[PAGE][ROW][COL].c = '~';
            dir == HORI ? COL++ : ROW++; // Horizontally increments COL and Vertically increments ROW
            i++;
        }
    }

    /**
     * @brief Validate that the inputs aren't negative or row is not above the allowed limit
     * 
     * @param PAGE  - page number
     * @param ROW   - row
     * @param COL   - column
     * @param dir   - direction -> horizontal or vertical
     * @param LEN   - length of the word
     */
    void ValChecker(int PAGE, int ROW, int COL,ariel::Direction dir, int LEN){
        // Negative values or COL pass the allowed limit (100)
        if (PAGE < 0 || ROW < 0 || COL < 0 || COL >= MAX_ALLOWED_LENGTH || LEN < 0) {
            throw std::invalid_argument("Negative values in arguments");
        }

        // Bad direction
        if (dir != HORI && dir != VERT) 
        {
            throw std::invalid_argument("Bad direction");
        } 
    }

    /**
     * @brief Checking bad chars such as '~','\n',.... And that they're not above ascii 31 & below 127
     * 
     * @param str 
     */
    void BadChars(const std::string& str){
        const int size = 5; // Size of the array
        std::array <char, size> chars = {'\n','\r','~','\t','\v'}; // Forbidden chars
        for (size_t i = 0; i < size; i++){   
            if (str.find(chars.at(i)) != std::string::npos){
                throw std::invalid_argument(R"(String contains bad chars '~','\n' ,....)");
            } 
        }

        for (auto ch : str){
            if (!(ch >= MIN_ALLOWED_CHAR && ch <= MAX_ALLOWED_CHAR)){ // Pass the allowed min & max chars
                throw std::invalid_argument("String contains bad chars below 32 & above 126");
            }
        }
        
    }

    /**
     * @brief Count amount of digits - used in show() method
     * For a better showing format
     * @param number 
     * @return amount of digits 
     */
    int CountDigits(int number){
        int total = 0;
        const int division = 10;
	    while (number != 0){
		    total++;
		    number /= division;
	    }
        return total;
    }
}