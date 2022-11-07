#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <Windows.h> 

#define MAX_INPUT_STRING_LENGTH 2500
#define MAX_VALUE_SIZE 100
#define IS_NEGATIVE_NUMBER '~'
#define FALSE 0
#define TRUE 1


/*
* Structure stack for operating with big numbers
*/
struct stack_node
{
	char data;
	struct stack_node* ptr_next;
};


/*
* Operation 'push' to the stack
*/
void push(struct stack_node** top_ptr, const char symbol)
{
	struct stack_node* new_ptr = NULL;

	new_ptr = (struct stack_node*)malloc(sizeof(struct stack_node));

	if (new_ptr != NULL)
	{
		memset(new_ptr, 0x00, sizeof(struct stack_node));
		new_ptr->data = symbol;
		new_ptr->ptr_next = *top_ptr;
		*top_ptr = new_ptr;
	}
}

/*
* Operation 'pop' to the stack
* Returns the value which is 'pop'ed'
*/
char pop(struct stack_node** top_ptr)
{
	struct stack_node* temp_ptr = NULL;
	char pop_symbol = '\0';

	if (*top_ptr == NULL)
	{
		return EXIT_SUCCESS; // stack is empty
	}

	temp_ptr = *top_ptr;
	pop_symbol = (*top_ptr)->data;
	*top_ptr = (*top_ptr)->ptr_next;

	free(temp_ptr);

	return pop_symbol;
}

// +-   */%  ^
bool priority(const char action1, const char action2)
{
	int priority1 = 0;
	int priority2 = 0;

	if (action1 == '^')
	{
		priority1 = 1;
	}
	if (action2 == '^')
	{
		priority2 = 1;
	}
	if (action2 == '%' || action2 == '/' || action2 == '*')
	{
		priority2 = 2;
	}
	if (action1 == '%' || action1 == '/' || action1 == '*')
	{
		priority1 = 2;
	}
	if (action1 == '+' || action1 == '-')
	{
		priority1 = 3;
	}
	if (action2 == '+' || action2 == '-')
	{
		priority2 = 3;
	}
	if (action1 == '(')
	{
		priority1 = 4;
	}
	if (action1 == ')')
	{
		priority1 = 0;
	}

	if (action1 == 0)
	{
		return true;
	}
	if (action2 == '(' || action2 == ')')
	{
		return true;
	}
	if (action1 == action2)
	{
		return false;
	}
	if (priority1 == priority2)
	{
		return false;
	}
	if (priority1 < priority2)
	{
		return false;
	}
	else
	{
		return true;
	}

}

/*
* Check for the stack top value
* If it is '(' then we 'pop' it and return false
* Otherwise return true
*/
bool check(struct stack_node** top_ptr)
{
	if ((*top_ptr)->data == '(')
	{
		pop(top_ptr);
		return false;
	}
	else
	{
		return true;
	}
}

void comparsion(struct stack_node** top_ptr, const char symbol, unsigned char* out, int* index)
{
	bool flag = true;
	while (flag)
	{
		if (priority((*top_ptr)->data, symbol))
		{
			push(top_ptr, symbol);
			flag = false;
		}
		else
		{
			int i = *index;
			out[*index] = (*top_ptr)->data;
			i++;
			pop(top_ptr);
			(*index)++;
		}
	}
	if ((*top_ptr)->data == ')')
	{
		pop(top_ptr);
		while (check(top_ptr))
		{
			out[*index] = (*top_ptr)->data;
			pop(top_ptr);
			(*index)++;
		}
	}
}

/*
* creates an entry based
* on reverse polish and 
* pushes the necessary characters
* into the stack
*/
void Read(unsigned char* math_expr, unsigned char* out, struct stack_node** top_ptr)
{
	int i = 0;
	int j = 0;
	while (math_expr[i] != 0)
	{
		if ((math_expr[i] >= '0' && math_expr[i] <= '9') || math_expr[i] == '.')
		{
			out[j] = math_expr[i];
			j++;
			i++;
		}
		else
		{
			if ((out[j - 1] >= '0' && out[j - 1] <= '9'))
			{
				out[j] = ' ';
				j++;
			}
			else
			{
				comparsion(top_ptr, math_expr[i], out, &j);
				i++;
			}
		}
	}
}

void print_stack(struct stack_node* start_ptr_list)
{
	if (start_ptr_list == NULL)
	{
		printf("Stack is null\n");
	}
	else
	{
		printf("Current Stack is:\n");
		while (start_ptr_list != NULL)
		{
			printf("%c --> ", start_ptr_list->data);
			start_ptr_list = start_ptr_list->ptr_next;
		}
	}
	printf("null \n");
}

int rounding_for_result_str(int j,char* result_str) {
	
	if (result_str[0] >= '5')
	{
		if (result_str[0] == '9')
		{
			while (result_str[j] == '9')
			{
				result_str[j] = 0;
				j++;
			}
			result_str[j] = result_str[j] + 1;
			j--;
		}
		else
		{
			result_str[1] = result_str[1] + 1;
			result_str[0] = 0;
		}

	}

	result_str[0] = 0;
	return j;
}

void swap(int i,char* result_str) {

	for (int n = 0, m = i - 1; n < i / 2; n++, m--)
	{
		char temp = result_str[n];
		result_str[n] = result_str[m];
		result_str[m] = temp;
	}
}

int convert_char_to_int(char* number, int result,int i) {
	while (result != 0)
	{
		number[i] = result % 10 + '0';
		result = result / 10;
		i++;
	}
	return i;
}

/* converts double
*  to character array
*/
void convertion(double result_number, char* result_str)
{
	unsigned char flag = FALSE;
	int i = 0;
	unsigned long long int temp_result_number = 0;
	result_number = result_number * 1000;
	if (result_number < 0)
	{
		result_number = -result_number;
		flag = IS_NEGATIVE_NUMBER;
	}

	temp_result_number = (unsigned long long int)result_number;

	if (temp_result_number == 0)
	{
		memset(result_str, 0x00, MAX_VALUE_SIZE);
		result_str[i] = '0';
		i++;
		return;
	}
	i = convert_char_to_int(result_str, temp_result_number,i);


	// rounding
	int j = 0;
	j = rounding_for_result_str(j,result_str);
	swap(i,result_str);


	size_t Size = strlen(result_str);
	int count_of_digits = 2 - j;
	int k = 1;
	int c = 0;
	while (count_of_digits != 0)
	{
		result_str[Size - c] = result_str[Size - k];
		k++;
		c++;
		count_of_digits--;
	}
	result_str[Size - 2 + j] = '.';
	while (result_str[Size] == '0')
	{
		result_str[Size] = 0;
		Size--;
	}
	if (result_str[Size] == '.')
	{
		result_str[Size] = 0;
	}
	Size = strlen(result_str);
	if (flag == IS_NEGATIVE_NUMBER) result_str[Size] = flag;
		

}

/* a function that performs
*  an action and returns an array
*  with the calculated number*/
char* arithmetics(char* first_number, char* second_number, const char action, char* result_str)
{
	double first = 0;
	double second = 0;
	double result_number = 0;

	size_t Size1 = strlen(first_number) - 1;
	size_t Size2 = strlen(second_number) - 1;

	if (first_number[Size1] == IS_NEGATIVE_NUMBER || second_number[Size2] == IS_NEGATIVE_NUMBER)
	{
		if (first_number[Size1] == IS_NEGATIVE_NUMBER)
		{
			first_number[Size1] = 0;
			first = atof(first_number);
			first = -first;
		}
		else
		{
			first = atof(first_number);
		}

		if (second_number[Size2] == '~')
		{
			second_number[Size2] = 0;
			second = atof(second_number);
			second = -second;
		}
		else
		{
			second = atof(second_number);
		}
	}

	else
	{
		first = atof(first_number);
		second = atof(second_number);
	}


	if (action == '^')
	{
		result_number = pow(first, second);
	}

	if (action == '*')
	{
		result_number = first * second;
	}

	if (action == '/')
	{
		if (second != 0.00000)
		{
			result_number = first / second;
		}
		else
		{
			printf("You can't devide by zero");
			return "error";
		}
	}

	if (action == '+')
	{
		result_number = first + second;
	}

	if (action == '-')
	{
		result_number = first - second;
	}

	if (action == '%')
	{
		unsigned long long int a = (unsigned long long int)first;
		unsigned long long int b = (unsigned long long int)second;
		result_number = (double)(a % b);
	}

	// 
	convertion(result_number, result_str);
	return result_str;
}

/* reads a string
 * and performs
 * arithmetic operations
 */
int Read_string(unsigned char* out)
{
	size_t i = strlen((const char*)out) - 1;

	char first_number[MAX_VALUE_SIZE] = { 0 };
	char second_number[MAX_VALUE_SIZE] = { 0 };

	char action = 0;

	char* result_str = (char*)malloc(MAX_VALUE_SIZE * sizeof(char));
	memset(result_str, 0, MAX_VALUE_SIZE);

	while (i != -1)
	{
		int index_stop = 0;
		int index_start = 0;

		if (out[i] == ' ' && !((out[i + 1] >= '0' && out[i + 1] <= '9') || out[i + 1] == 0))
		{
			action = out[i + 1];
			if (action == 0)
			{
				break;
			}
			index_stop = (int)(i + 2);
			i--;
			// search second integer
			int j = 0;
			memset(second_number, 0, MAX_VALUE_SIZE);
			while ((out[i] >= '0' && out[i] <= '9') || out[i] == '.' || out[i] == '~')
			{

				second_number[j] = out[i];
				j++;
				i--;
			}
			_strrev(second_number);
			if (out[i] != ' ')
			{
				continue;
			}
			i--;
			// search first integer
			j = 0;
			memset(first_number, 0, MAX_VALUE_SIZE);
			while ((out[i] >= '0' && out[i] <= '9') || out[i] == '.' || out[i] == '~')
			{

				first_number[j] = out[i];
				j++;
				i--;
			}
			_strrev(first_number);
			index_start = (int)(i + 1);

			result_str = arithmetics(first_number, second_number, action, result_str);
			if (strcmp(result_str, "error") == 0)
			{
				return -1;
			}
			size_t Sizerez = strlen(result_str);

			int sdvig = 0;

			if (index_start + (int)Sizerez >= index_stop)
			{
				char* A = (char*)malloc(sizeof(char) * (strlen((const char*)out) - index_stop + 1));
				memset(A, 0x00, (strlen((const char*)out) - index_stop + 1));

				int SizeA = 0;
				for (int n = 0, r = index_stop; r < (int)strlen((const char*)out); n++, r++)
				{
					A[n] = out[r];
					SizeA++;
				}
				A[SizeA] = 0;
				for (int k = index_start, j = 0; j < (int)Sizerez; k++, j++)
				{
					out[k] = result_str[j];
					index_stop = k + 1;
				}
				out[index_stop] = ' ';
				index_stop++;

				for (int j = index_stop, k = 0; k < SizeA; j++, k++)
				{
					out[j] = A[k];
				}
			}
			else
			{
				for (int k = index_start, j = 0; j < (int)Sizerez; k++, j++)
				{
					out[k] = result_str[j];
					index_start = k;
				}
				index_start++;
				out[index_start] = ' ';

				for (int j = index_start + 1, k = index_stop; k < (int)strlen((const char*)out); j++, k++)
				{
					out[j] = out[k];
				}
				sdvig = index_stop - index_start;
				size_t stop = strlen((const char*)out) - sdvig;
				for (size_t j = strlen((const char*)out) - 1; j > stop; j--)
				{
					out[j] = 0;
				}
			}
			i = strlen((const char*)out) + 1;
		}
		i--;
	}
	return 0;
}


void check_stack(unsigned char* math_expr)
{
	size_t length = strlen((const char*)math_expr);
	int i = (int)(length - 1);
	while (i >= 0)
	{
		if (math_expr[i] == '-' && (!(math_expr[i - 1] >= '0' && math_expr[i - 1] <= '9') && math_expr[i - 1] != ')'))
		{
			for (size_t j = length, k = length - 1; (int)j > i; j--, k--)
			{
				math_expr[j] = math_expr[k];
			}
			math_expr[i] = '0';
			length++;
			i++;
		}
		i--;
	}
}


int check_input_string(unsigned char* math_expr) {

	const char* alphabet = (const char*)"0123456789.%/+-^*()";

	int left_brackets_count = 0;
	int right_brackets_count = 0;


	size_t length = strlen((const char*)math_expr);

	for (size_t i = 0; i < length; i++)
	{
		if (math_expr[i] == '(')
		{
			left_brackets_count++;
		}
		else if (math_expr[i] == ')')
		{
			right_brackets_count++;
		}
		if (strchr(alphabet, math_expr[i]) == NULL)
		{
			return FALSE;
		}
	}

	if (left_brackets_count == right_brackets_count)
	{
		return TRUE;
	}

	return FALSE;
}


int main(int argc, char* argv[])
{

	unsigned char* math_expr = (unsigned char*)malloc(MAX_INPUT_STRING_LENGTH * sizeof(unsigned char));
	if (math_expr == NULL)
	{
		return EXIT_FAILURE;
	}

	memset(math_expr, 0x00, MAX_INPUT_STRING_LENGTH);

	if (argc > 1)
	{
		if (strlen(argv[1]) > MAX_INPUT_STRING_LENGTH)
		{
			printf("Input string is too long\n");
			return EXIT_FAILURE;
		}
		strncpy((char*)math_expr, argv[1], strlen(argv[1]));
	}
	else
	{
		int scanf_res = scanf("%s", math_expr);
		if (scanf_res < 0) return EXIT_FAILURE;
	}


	// work time calculation
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
	/////////////////////////////////////


	int status = check_input_string(math_expr);

	if (status == FALSE)
	{
		fprintf(stderr, "Invalid input...\n\n");
		return EXIT_FAILURE;
	}

	check_stack(math_expr);

	unsigned char* out = (unsigned char*)malloc(MAX_INPUT_STRING_LENGTH * sizeof(unsigned char));
	if (out == NULL) return EXIT_FAILURE;
	memset(out, 0x00, MAX_INPUT_STRING_LENGTH);

	struct stack_node* stack_pop = NULL;
	stack_pop = (struct stack_node*)malloc(sizeof(struct stack_node));
	if (stack_pop == NULL) return EXIT_FAILURE;
	memset(stack_pop, 0x00, sizeof(struct stack_node));

	Read(math_expr, out, &stack_pop);

	int check = Read_string(out);
	if (check == -1)
	{
		return -1;
	}
	int counter = (int)strlen((const char*)out);

	char ch;
	if (out[counter - 1] != ' ')
	{
		out[counter] = ' ';
		counter++;
	}
	while (((out[counter] = pop(&stack_pop))) != 0) {
		ch = out[counter];
		out[counter] = ch;
		counter++;
	}
	check = 0;
	check = Read_string(out);
	if (check == -1)
	{
		return -1;
	}
	counter = (int)strlen((const char*)out);
	out[counter - 1] = 0;
	counter--;

	if (out[counter - 1] == IS_NEGATIVE_NUMBER)
	{
		for (int i = counter - 1, j = counter - 2; i >= 0; i--, j--)
		{
			out[i] = out[j];
		}
		out[0] = '-';
	}

	printf("\n");
	puts((const char*)out);


	// work time calculation
	QueryPerformanceCounter(&EndingTime);
	(double)ElapsedMicroseconds.QuadPart = (double)EndingTime.QuadPart - (double)StartingTime.QuadPart;
	(double)ElapsedMicroseconds.QuadPart *= 1000000;
	(double)ElapsedMicroseconds.QuadPart /= (double)Frequency.QuadPart;
	(double)ElapsedMicroseconds.QuadPart /= 1000000;
	printf("Time: %f sec\n", ElapsedMicroseconds);

}