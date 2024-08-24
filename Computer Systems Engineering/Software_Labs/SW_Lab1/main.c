#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//1a
//Creating the struct
typedef struct imaginary{double a; double b;} img;

void printImg(img z);
//1b
img conjugate(img z);
//1c
img add(img z1, img z2);
img sub(img z1, img z2);
img multiplication(img z1, img z2);
img division(img z1, img z2);
//1d
img reciprocal(img z);
//1e
double magnitude(img z);

//Function tests
void tests();

int main()
{
    //tests();
    int numbers = 2;
    img comps[numbers];
    double a = 0;
    double b = 0;
    for(int i = 0; i < numbers; i++)
    {
        printf("Complex number %d\n",i+1);
        printf("Real part: ");
        scanf("%lf", &a);
        printf("Imaginary part: ");
        scanf("%lf", &b);
        printf("\n\n");
        comps[i].a = a;
        comps[i].b = b;
    }
    printf("(");
    printImg(comps[0]);
    printf(") + (");
    printImg(comps[1]);
    printf(") = ");
    printImg(add(comps[0], comps[1]));
    printf("\n");

    printf("(");
    printImg(comps[0]);
    printf(") - (");
    printImg(comps[1]);
    printf(") = ");
    printImg(sub(comps[0],comps[1]));
    printf("\n");

    printf("(");
    printImg(comps[0]);
    printf(") * (");
    printImg(comps[1]);
    printf(") = ");
    printImg(multiplication(comps[0],comps[1]));
    printf("\n");

    printf("(");
    printImg(comps[0]);
    printf(") / (");
    printImg(comps[1]);
    printf(") = ");
    printImg(division(comps[0],comps[1]));
    printf("\n");

    return 0;
}

//Function tests
void tests()
{
    img comp1, comp2;
    comp1.a = 1;
    comp1.b = 1;
    comp2.a = 4;
    comp2.b = -2;
    printImg(division(comp1, comp2));
    printImg(reciprocal(comp1));
    printf("Magnitude: %f", magnitude(comp2));
}

//Prints the complex number
void printImg(img z)
{
    if(z.b >= 0)
    {
        printf("%g + %gi",z.a,z.b);
    }
    else
    {
        printf("%g%gi",z.a,z.b);
    }
}

//1b
//Creates and returns the conjugate of a complex number
img conjugate(img z)
{
    img conj = z;
    conj.b *= -1;

    return conj;
}

//1c
//Complex Addition
img add(img z1, img z2)
{
    img added = z1;
    added.a += z2.a;
    added.b += z2.b;
    return added;
}
//Complex Subtraction
img sub(img z1, img z2)
{
    img subbed = z1;
    subbed.a -= z2.a;
    subbed.b -= z2.b;
    return subbed;
}
//Complex multiplication
img multiplication(img z1, img z2)
{
    img mult;
    mult.a = (z1.a*z2.a) - (z1.b*z2.b);
    mult.b = (z1.a*z2.b) + (z1.b*z2.a);

    return mult;
}
//Complex division
img division(img z1, img z2)
{
    img divi;
    img numerator = multiplication(z1, conjugate(z2));
    img denominator = multiplication(z2, conjugate(z2));

    divi.a = numerator.a/denominator.a;
    divi.b = numerator.b/denominator.a;

    return divi;
}

//1d
//Returns the reciprocal of the complex number
img reciprocal(img z)
{
    img one;
    one.a = 1;
    one.b = 0;
    return division(one, z);
}

//1e
//Returns the magnitude of the complex number
double magnitude(img z)
{
    return sqrt(pow(z.a,2)+pow(z.b,2));
}
