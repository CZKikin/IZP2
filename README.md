# IZP2
 Second IZP project

## Cell selection commands
- [R,C] - Cell on row R and column C 
- [R,_] - Whole R row 
- [_,C] - Whole C column 
- [R1,C1,R2,C2] - Window beggining at R1 row and C1 columnt to R2 and C2 
- [_,_] - Whole table
- [min] - In existing selection find minimal value cell 
- [max] - In existing selection find maximal value cell 
- [find STR] - In existing selection find cell with STR value
- [_] - Gets selection from variable 

## Table structure commands
- irow - Inserts empty row on top of selected row 
- arow - Inserts empty row under selected row 
- drow - Deletes row 
- icol - Inserts empty column to the left of selected column
- acol - Inserts empty column to the right of selected column
- dcol - Deletes column 

## Data change commands
- set STR - Sets value to STR. STR may be enclosed \"STR\" and may cosist of special characters like \"\n\"
- clear - Clears values of cells
- swap [R,C] - Swaps selected cell with cell on row R and column C 
- sum [R,C] - Sum of selected cells will be saved to cell on row R and C column C 
- avg [R,C] - Average of selected cell will be saved to cell on row R and C column
- count [R,C] - Count of epty cells will be save to cell on row R and C column
- len [R,C] - Len of string in selected cell will be saved to cell on row R and C column

## Varibale commands
- def _X - Value of selected cell will be saved to variable _X (_0 to _9)
- use _X - Set cell to selected value 
- inc _X - Increase value, if value is numeric
- [set] - Sets cell selection to variable
