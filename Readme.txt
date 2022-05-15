Complie:
    gcc -Wall main.c stat.c var.c -o myAnalyzer

    ./myAnalyzer prog1.c

    Theoretically，it can analyze multiple files like ./myAnalyzer prog1.c prog2.c
    sepreated by //////////////////////////////.

Idea:
    For variables, basic idea is to use a struct to store all the variable info in a certain line,
    and another struct to combine all the info together though the entire file.

    For other information, I used a struct called struct to store it while reading the file.

    Typically, for store scopes, when scan file by order, the function scope always comes first,
    as the function scope changed, the following variable changed also.

    I used strings to store one type of information, like variable name, variable type, and variable scope,
    and strtok/strtok_r them by this delimiter " ". I will also add this delimiter when store the info to the string.
    After, just print them seperately in the main.c

Reason don't use an array list:
    I have to figure out the list array size when build an array list(list of string)
    so I have to determine how many variables it has in a file first.
    when scaning a file line by line, I can extract variable name, scope, and number of varibles
    at the same time. So I don't want to scan a file first and just for getting # of vars and scan it again to get the rest of info.
    However, when I realized I have to use list array to find the type or size of certain variable, it was too late!
    I've done all the parts except heap, and passed the test.

Case considered:
    String literal: 
        - char *i = "Hello";
        - char *i;
          i = "Hello";
    Heap:
        - type *i = malloc/calloc(...);
        - type *i; (also when variable declared as parameters)
        i = malloc/calloc(...);
        - It might not achieve the size of malloc/calloc(strlen(...));
          but still a number comes up and I don't know if it's right.
    Variable:
        - type a;
        - type b = a;
        - type c = b, d = a;
        - type a, b, c;
        - type a = 1, b = 2, c = 3;
        - type *a, *b, *c;
        - type a, b[], *c;
        - type **a, **b, **c;
        - type a[] = {...};
        - type a[] = "...";
        - type a[N] = {...};
        All the variable displayed correctly through the test cases.
        Maybe there are more cases that this analyzer can pass but never be stated in this document.

    In the end, I always try to reduce the scanning file times. My DREAME was to only scan the input file once 
    (that's why I don't use list of array at first), but didn't find a way to achieve it.

For TAs;
    Hope this documentation is enough. I rushed to wirte the document, because I put tons of time on implementation,
    if you think this documentation is too short, feel free reach me out by email devin.liu@mail.utoronto.ca.





