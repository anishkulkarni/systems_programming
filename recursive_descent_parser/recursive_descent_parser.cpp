/*
Consider the following grammar
S -> Aa | b
A -> Ac | Sd | epsillon

It is a left recursive grammar
Removing S from right hand side of A
S -> Aa | b
A -> Ac | Aad | bd | epsillon

Removing left recursion from A
S -> Aa | b
A -> bdB | B
B -> cB | adB | epsillon
*/

#include<iostream>

using namespace std;

int S();
int A();
int B();

int i=0;
char input[50];

int main()
{
        cout << "Enter String: ";
        cin >> input;
	cout << "Grammar is:\nS -> Aa | b\nA -> bdB | B\nB -> cB | adB | epsillon";
        if(S() == 1)
        {
                cout << "\nExpression is valid";
        }
        else
        {
                cout << "\nExpression is invalid";
        }
        cout<<"\n";
        return 0;
}

int S()
{
        cout << "\nChecking derivation of S";
        if(input[i] == 'b' && input[i+1] == '\0')
        {
                cout << "\nDerivation chosen: S -> b";
                return 1;
        }
        else
        {
		cout << "\nDerivation chosen: S -> Aa";
                if(A() == 1)
                {
                        if(input[i] == 'a' && input[i+1] == '\0')
                        {
                                return 1;
                        }
                }
                else
                {
                        return 0;
                }
        }
}

int A()
{
	cout << "\nChecking derivation of A";
        if(input[i] == 'b' && input[i+1] == 'd')
        {
                cout << "\nDerivation chosen: A -> bdB";
                i+=2;
                if(B() == 1)
                {
                        return 1;
                }
        }
	else
	{
		cout << "\nDerivatiob chosen A -> B";
		if(B() == 1)
		{
			return 1;   
		}
		else
		{
			return 0;
		}
	}
}

int B()
{
	cout << "\nChecking derivation of B";
        if(input[i] == 'c')
        {
                cout << "\nDerivation chosen: B -> cB";
                i++;
                if(B() == 1)
                {
                        return 1;
                }
        }
	else if(input[i] == 'a' && input[i+1] == 'd')
	{
		cout << "\nDerivation chosen: B -> adB";
		i+=2;
		if(B() == 1)
		{
			return 1;
		}
	}
	else
	{
		cout << "\nDerivation chosen: B -> epsillon";
		return 1;
	}
}