Nicholas DeNobrega CS375 Programming Assignment 1 Write Up

1. Provide a program package, a readme file to explain how to run your program, and show your code and results for both B.1 and B.2:
    -To run the program, first type make in console to build executable.
    -Then type ./submission inputFile.txt outputFile.txt outputFile2.txt, which take inputFile.txt
      as the input, and writes the output of B1 to outputFile.txt, and output of B2 to outputFile2.txt
    -The format of my input file tests are written as below, the numbers before -1 are for B1 array, 
      the numbers after -1 are for B2 array, and the last number is S, where is the target sum were looking 
      for in B2. The -1 is used to indicate where the input for B1 ends and the input for B2 begins

      -General form of my input file (x is integer, and S is target sum for B2):
        x
        x
        x
        x
        -1
        x
        x
        x
        x
        S

        -Example input file:
        1
        2
        3
        7
        8
        15
        22
        -1 
        1
        2
        3
        7
        8
        15
        22
        10

    - My results for B1 with this input is:
        1 2 1
        2 3 1
        7 8 1
        1 3 2
        1 8 7
        8 15 7
        15 22 7
        7 15 8
        7 22 15
        Running time: 3.5e-05 seconds

    -My results for B2 with this input is: 
        10 8 2
        10 7 3
        Running time: 9e-06 seconds

2. Explain your implemented algorithms and the worst-case time complexity for both B.1 and B.2
    Explination of B1:
        1. Take the input array, and put it into a set, i.e this set contains all keys
           and allows us to look for a specific key in constant time.
        2. Loop through every combination of keys in the array with double for loop
        3. In each iteration, we check if the difference of the two keys is in the set
        4. If it is, we write to output file, and if not we continue
        Overall, adding the array to the set is O(n) time, looping through every combination of
        keys in the array is O(n^2), and checking if the difference of the 2 keys is in the set is O(1),
        so worst case is O(n^2)
    Explination of B2:
        1. Take the input array and sort in ascending order, allowing us to use a two-pointer technique to
           find desired pairs that add to target sum.
        2. Set the left pointer to left most key, and right pointer to right most key.
        3. Because of the sorting, we can loop through the array in linear time and update the left and right 
            pointers accordingly based on how close the keys at those pointers are to equaling the sum
        4. If the sum of the keys at the current positions is greater then the desired sum, we decrement right
            pointer. If sum of keys at current positions is less then desired sum, we increment left pointer, and
            if sum of keys at current positons equals sum, we write to output file and increment left pointer and 
            decrement right pointer
        Overall, sorting the array takes O(nlogn) time, and looping through the array takes O(n) time, so worst case
        is O(nlogn) time complexity.




    