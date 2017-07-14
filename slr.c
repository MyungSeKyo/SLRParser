#include<stdio.h>
#include <ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAX_RULE_NUM 20
#define MAX_RULE_SIZE 20
#define MAX_NONE_TERMINAL_NUM 20
#define MAX_TERMINAL_NUM 20
#define MAX_STATE_NUM 20
#define MAX_USER_INPUT 20
#define ENOUGH_BIG_NUMBER 20
#define MAX_STACK_SIZE 30
#define EPSILON '#'

typedef struct none_terminal {
	char nt;
	char first[MAX_NONE_TERMINAL_NUM];
	char follow[MAX_NONE_TERMINAL_NUM];
}none_terminal;

typedef struct state {
	int num_of_item;
	char items[MAX_RULE_NUM][MAX_RULE_SIZE];
}state;

char action_table[MAX_STATE_NUM][MAX_TERMINAL_NUM][4] = { {{'\0'}} };
int goto_table[MAX_STATE_NUM][MAX_TERMINAL_NUM] = { 0 };

state states[MAX_STATE_NUM] = { {0,{{'\0'}} } };
int num_of_state = 0;

char rules[MAX_RULE_NUM][MAX_RULE_SIZE];
int num_of_rule = 1;

none_terminal none_terminals[MAX_NONE_TERMINAL_NUM] = { {'\0',{'\0'},{'\0'}} };
int num_of_none_terminal = 0;

char terminals[MAX_TERMINAL_NUM];
int num_of_terminal = 0;

void user_input();
void check_grammer(char * input);
void check_grammer_print(char * input, int * stack, int stack_top);
void save_rule(char * file_name);
none_terminal * get_none_terminal(char nt);
void save_v();
void insert(char *ar, int idx, char ch);
void delete(char *ar, int idx);
void append(char *ar, char ch);
void ringsum(char *a, char *b, char *c);
void set_sum(char * a, char * b);
int has_epsilon(char * charr);
void add_to_set(char * set, char c);
void first();
void follow();
void construct_table();
void goto_function(state s);
int get_rule_index(char * rule);
int get_terminal_index(char ch);
int get_none_terminal_index(char ch);
void move_dot(char * a, char * b);
char get_mark_symbol(char * str);
void closure(char * item, state * state);
int has_item(state st, char * item);
int exist_state(state state);
int equal_state(state a, state b);

void main(int argc, char * argv[]) {
	printf("<Sekyo's SLR parser 2017-6-11>\n\n");
	printf("[Precaution]\n");
	printf("None-terminals are in upper case.\n");
	printf("Epsilon is '#'.\n");
	printf("Non-terminals should not contain 'Z'.\n\n");
	printf("[How to use]\n");
	printf("FIRST\tShow first of all non-terminal.\n");
	printf("FOLLOW\tShow follow of all non-terminal.\n");
	printf("ACTION\tShow action-table.\n");
	printf("GOTO\tShow goto-table.\n");
	printf("TABLE\tShow SLR parsing table.\n");
	printf("In\tShow items in state n.\n");
	printf("string\tTells whether the input string is grammatically correct.\n");
	printf("exit\tExit the program.\n\n");
	save_rule(argv[1]);
	save_v();
	first();
	follow();
	construct_table();
	user_input();

}
void user_input() {
	int i, j;
	char input[MAX_USER_INPUT];
	while (1) {
		printf(">>");
		fscanf(stdin, "%s", input);

		if (strcmp(input, "FIRST") == 0) {
			printf("\n");
			for (i = 1; i < num_of_none_terminal; i++) {
				printf("%c : %s\n", none_terminals[i].nt, none_terminals[i].first);
			}
			printf("\n");
		}
		else if (strcmp(input, "FOLLOW") == 0) {
			printf("\n");
			for (i = 1; i < num_of_none_terminal; i++) {
				printf("%c : %s\n", none_terminals[i].nt, none_terminals[i].follow);
			}
			printf("\n");
		}
		else if (strcmp(input, "TABLE") == 0) {
			printf("\n");
			for (i = 0; i < num_of_terminal; i++) {

				printf("\t%c", terminals[i]);

			}

			for (int i = 1; i < num_of_none_terminal; i++) {

				printf("\t%c", none_terminals[i].nt);

			}

			printf("\n");

			for (i = 1; i < num_of_terminal + num_of_none_terminal; i++) {

				printf("\t-");

			}

			for (i = 0; i < num_of_state; i++) {

				printf("\n");

				printf("I%d", i);

				for (j = 0; j < num_of_terminal; j++) {

					printf("\t%s", action_table[i][j]);

				}
				for (int j = 1; j < num_of_none_terminal; j++) {
					if (goto_table[i][j] != 0)
						printf("\t%d", goto_table[i][j]);
					else
						printf("\t");
				}
			}
			printf("\n\n");

		}
		else if (strcmp(input, "ACTION") == 0) {
			printf("\n");
			for (i = 0; i < num_of_terminal; i++) {
				printf("\t%c", terminals[i]);
			}
			printf("\n");
			for (i = 0; i < num_of_terminal; i++) {
				printf("\t-");
			}
			for (i = 0; i < num_of_state; i++) {
				printf("\n");
				printf("I%d", i);
				for (j = 0; j < num_of_terminal; j++) {
					printf("\t%s", action_table[i][j]);
				}
			}
			printf("\n\n");
		}
		else if (strcmp(input, "GOTO") == 0) {
			printf("\n");
			for (int i = 1; i < num_of_none_terminal; i++) {
				printf("\t%c", none_terminals[i].nt);
			}
			printf("\n");
			for (int i = 1; i < num_of_none_terminal; i++) {
				printf("\t-");
			}
			for (int i = 0; i < num_of_state; i++) {
				printf("\n");
				printf("I%d", i);
				for (int j = 1; j < num_of_none_terminal; j++) {
					if (goto_table[i][j] != 0)
						printf("\t%d", goto_table[i][j]);
					else
						printf("\t");
				}
			}
			printf("\n\n");
		}
		else if (strncmp(input, "I", 1) == 0) {
			printf("\n");
			for (i = 0; i < states[atoi(&input[1])].num_of_item; i++) {
				printf("%s\n", states[atoi(&input[1])].items[i]);
			}
			printf("\n");
		}
		else if (strcmp(input, "exit") == 0) {
			printf("Good bye :)\n");
			exit(0);
		}
		else {
			check_grammer(input);
		}
	}
}
void check_grammer(char * input) {
	char input_berffer[MAX_STACK_SIZE];

	int stack[MAX_STACK_SIZE] = { 0 };
	int stack_top = -1;

	strcpy(input_berffer, input);
	append(input_berffer,'$');

	stack_top++;
	stack[stack_top] = 0;
	printf("\t\t\t");
	check_grammer_print(input_berffer, stack, stack_top);
	while (1) {
		if (strncmp(action_table[stack[stack_top]][get_terminal_index(input_berffer[0])],"s",1) == 0) {
			stack_top++;
			stack[stack_top] = (int)input_berffer[0];

			delete(input_berffer,0);

			stack_top++;
			stack[stack_top] = atoi(&action_table[stack[stack_top - 2]][get_terminal_index((char)stack[stack_top - 1])][1]);

			printf("[%s]\t\t\t", action_table[stack[stack_top - 2]][get_terminal_index((char)stack[stack_top - 1])]);
			check_grammer_print(input_berffer, stack, stack_top);
		}
		else if (strncmp(action_table[stack[stack_top]][get_terminal_index(input_berffer[0])], "r", 1) == 0) {
			char rule[MAX_RULE_SIZE];
			printf("[%s, GOTO", action_table[stack[stack_top]][get_terminal_index(input_berffer[0])]);

			strcpy(rule,rules[atoi(&action_table[stack[stack_top]][get_terminal_index(input_berffer[0])][1])]);			

			stack_top -= strlen(&rule[2]) * 2;

			stack_top++;
			stack[stack_top] = (int)rule[0];

			stack[stack_top + 1] = goto_table[stack[stack_top - 1]][get_none_terminal_index((char)stack[stack_top])];
			stack_top++;
			printf("%d]\t\t",stack[stack_top]);
			check_grammer_print(input_berffer, stack, stack_top);
		}
		else if (strncmp(action_table[stack[stack_top]][get_terminal_index(input_berffer[0])], "OK", 2) == 0){
			printf("\n>>ACCEPT\n");
			break;
		}
		else {
			printf("\n>>ERROR\n");
			break;
		}
	}
}
void check_grammer_print(char * input, int * stack, int stack_top) {
	int i;
	printf("(%d",stack[0]);
	for (i = 1; i <= stack_top; i+=2) {
		printf("%c%d",stack[i], stack[i + 1]);
	}
	printf("    %s)\n",input);
}
void save_rule(char * file_name) {

	FILE * fp = fopen(file_name, "r");

	if (fp == '\0') {
		printf("There are no %s.\n",file_name);
	}
	while (!feof(fp)) {
		fscanf(fp, "%s", rules[num_of_rule]);
		fscanf(fp, "%s", rules[num_of_rule]);
		if(isupper(rules[num_of_rule][0]))
			num_of_rule++;
	}
	fclose(fp);
	sprintf(rules[0], "Z>%c", rules[1][0]);
	none_terminals[0].nt = rules[0][0];
	num_of_none_terminal++;
}
none_terminal * get_none_terminal(char nt) {
	int i;
	for (i = 0; i < num_of_none_terminal; i++) {
		if (none_terminals[i].nt == nt) {
			return &none_terminals[i];
		}
	}
}
void save_v() {
	int i, j, k;
	for (i = 0; i < num_of_rule; i++) {
		for (j = 0; j < strlen(rules[i]); j++) {
			if (isupper(rules[i][j])) {
				int has = 0;
				for (k = 0; k < num_of_none_terminal; k++) {
					if (none_terminals[k].nt == rules[i][j]) {
						has = 1;
						break;
					}
				}
				if (!has) {
					none_terminals[num_of_none_terminal].nt = rules[i][j];
					num_of_none_terminal++;
				}
			}
			else if (rules[i][j] != EPSILON && rules[i][j] != '>') {
				int has = 0;
				for (k = 0; k < num_of_terminal; k++) {
					if (terminals[k] == rules[i][j]) {
						has = 1;
						break;
					}
				}
				if (!has) {
					terminals[k] = rules[i][j];
					num_of_terminal++;
				}
			}
		}
	}
	terminals[num_of_terminal] = '$';
	num_of_terminal++;
}
void insert(char *ar, int idx, char ch) {
	memmove(ar + idx + 1, ar + idx, strlen(ar) - idx + 1);
	ar[idx] = ch;
}
void delete(char *ar, int idx) {
	memmove(ar + idx, ar + idx + 1, strlen(ar) - idx);
}
void append(char *ar, char ch) {
	insert(ar, strlen(ar), ch);
}
void ringsum(char *a, char *b, char *c) {
	strcpy(c, a);

	if (has_epsilon(a) == -1) {
		return;
	}
	delete(c, has_epsilon(a));
	set_sum(c, b);
	return;
}
void set_sum(char * a, char * b) {
	int i;
	for (i = 0; i < strlen(b); i++) {
		add_to_set(a, b[i]);
	}
}
int has_epsilon(char * charr) {
	int i;
	for (i = 0; i < strlen(charr); i++) {
		if (charr[i] == EPSILON)
			return i;
	}
	return -1;
}
void add_to_set(char * set, char c) {
	int i;
	for (i = 0; i < strlen(set); i++)
		if (set[i] == c)
			return;
	append(set, c);
}
void first() {

	int i, j, k;
	printf("[Production rule]\n");
	for(i = 0;i < num_of_rule;i++)
		printf("%s\n",rules[i]);
	printf("\n");
	for (i = 0; i < num_of_rule; i++) {
		if (!isupper(rules[i][2])) {
			add_to_set(get_none_terminal(rules[i][0])->first, rules[i][2]);
		}
	}
	for (k = 0; k < ENOUGH_BIG_NUMBER; k++) {
		for (i = 0; i < num_of_rule; i++) {
			if (isupper(rules[i][2])) {
				if (rules[i][3] == '\0') {
					set_sum(get_none_terminal(rules[i][0])->first, get_none_terminal(rules[i][2])->first);
				}
				else {
					char temp[MAX_TERMINAL_NUM] = { '\0' };
					set_sum(temp, get_none_terminal(rules[i][2])->first);

					for (j = 3; j < strlen(rules[i]); j++) {
						if (isupper(rules[i][j])) {
							ringsum(temp, get_none_terminal(rules[i][j - 1])->first, temp);
						}
						else {
							if (has_epsilon(temp) != -1) {
								delete(temp, has_epsilon(temp));
								add_to_set(temp, rules[i][j]);
							}
							break;
						}
					}
					set_sum(get_none_terminal(rules[i][0])->first, temp);
				}
			}
		}
	}
}
void follow() {
	int i, j, k;

	append(none_terminals[0].follow, '$');
	for (k = 0; k < ENOUGH_BIG_NUMBER; k++) {
		for (i = 0; i < num_of_rule; i++) {
			for (j = 2; j < strlen(rules[i]); j++) {
				if (isupper(rules[i][j])) {
					if (isupper(rules[i][j + 1])) {
						set_sum(get_none_terminal(rules[i][j])->follow, get_none_terminal(rules[i][j + 1])->first);
						if (has_epsilon(get_none_terminal(rules[i][j + 1])->first) != -1) {
							delete(get_none_terminal(rules[i][j])->follow, has_epsilon(get_none_terminal(rules[i][j])->follow));
							set_sum(get_none_terminal(rules[i][j])->follow, get_none_terminal(rules[i][j + 1])->follow);
						}
					}
					else if (rules[i][j + 1] == '\0') {
						set_sum(get_none_terminal(rules[i][j])->follow, get_none_terminal(rules[i][0])->follow);
					}
					else {
						add_to_set(get_none_terminal(rules[i][j])->follow, rules[i][j + 1]);
					}
				}
			}
		}
	}
}
void construct_table() {
	int i, j, k;
	char augment[MAX_RULE_SIZE];
	state state_0 = { 0,{{'\0'}} };

	strcpy(augment, rules[0]);
	insert(augment, 2, '.');

	closure(augment, &state_0);

	memcpy(&states[num_of_state], &state_0, sizeof(state_0));
	num_of_state++;

	goto_function(state_0);
}
void goto_function(state s) {
	int i, j, k;

	for (i = 0; i < s.num_of_item; i++) {
		for (j = 0; j < strlen(s.items[i]); j++) {
			if (s.items[i][j] == '.' && s.items[i][j + 1] != '\0') {
				state temp_state = { 0,{ { '\0' } } };
				for (k = 0; k < s.num_of_item; k++) {
					if (s.items[i][j + 1] == get_mark_symbol(s.items[k])) {
						char temp_str[MAX_RULE_SIZE];
						move_dot(s.items[k],temp_str);
						closure(temp_str, &temp_state);
					}
				}
				if (exist_state(temp_state) == -1) {
					memcpy(&states[num_of_state], &temp_state, sizeof(temp_state));
					num_of_state++;
					goto_function(states[num_of_state - 1]);
				}
				if (isupper(s.items[i][j + 1])) {//ACTION
					goto_table[exist_state(s)][get_none_terminal_index(s.items[i][j + 1])] = exist_state(temp_state);
				}
				else {//GOTO
					char temp_str[4];
					sprintf(temp_str,"%d", exist_state(temp_state));
					insert(temp_str,0,'s');
					strcpy(action_table[exist_state(s)][get_terminal_index(s.items[i][j + 1])],temp_str);
				}
			}
			else if (s.items[i][j] == '.' && s.items[i][j + 1] == '\0') {//REDUCE
				char temp_rule[MAX_RULE_SIZE];
				char temp_str[4];
				strcpy(temp_rule, s.items[i]);
				for (k = 0; k < strlen(temp_rule); k++) {
					if (temp_rule[k] == '.')
						delete(temp_rule, k);
				}
				sprintf(temp_str,"%d", get_rule_index(temp_rule));
				insert(temp_str, 0, 'r');

				for (k = 0; k < strlen(get_none_terminal(temp_rule[0])->follow); k++) {
					if(strcmp(temp_str, "r0") == 0)
						strcpy(action_table[exist_state(s)][get_terminal_index(get_none_terminal(temp_rule[0])->follow[k])], "OK");
					else
						strcpy(action_table[exist_state(s)][get_terminal_index(get_none_terminal(temp_rule[0])->follow[k])], temp_str);
				}
			}
		}
	}
}
int get_rule_index(char * rule) {
	int i;
	for (i = 0; i < num_of_rule; i++) {
		if (strcmp(rules[i], rule) == 0) {
			return i;
		}
	}
	return -1;
}
int get_terminal_index(char ch) {
	int i;
	for (i = 0; i < num_of_terminal; i++) {
		if (terminals[i] == ch) {
			return i;
		}
	}
	return -1;
}
int get_none_terminal_index(char ch) {
	int i;
	for (i = 0; i < num_of_none_terminal; i++) {
		if (none_terminals[i].nt == ch)
			return i;
	}
	return -1;
}
void move_dot(char * a,char * b) {
	int i;
	strcpy(b,a);

	for (i = 0; i < strlen(b); i++) {
		if (b[i] == '.') {
			delete(b, i);
			insert(b, i + 1, '.');
			break;
		}
	}
}
char get_mark_symbol(char * str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (str[i] == '.')
			return str[i + 1];
	}
	return '\0';
}
void closure(char * item, state * state) {
	int i, j, k;
	if (!has_item(*state, item)) {
		strcpy(state->items[state->num_of_item], item);
		state->num_of_item++;
	}
	for (i = 2; i < strlen(item); i++) {
		if (item[i] == '.' && isupper(item[i + 1])) {
			for (j = 0; j < num_of_rule; j++) {
				if (rules[j][0] == item[i + 1]) {
					char temp_str[MAX_RULE_SIZE];
					strcpy(temp_str, rules[j]);

					if (rules[j][2] == EPSILON) {
						append(temp_str, '.');
					}
					else {
						insert(temp_str, 2, '.');
					}

					if (!has_item(*state, temp_str)) {
						closure(temp_str, state);
					}
				}
			}
		}
	}
}
int has_item(state st, char * item) {
	int i;
	for (i = 0; i < st.num_of_item; i++) {
		if (strcmp(item,st.items[i]) == 0) {
			return 1;
		}
	}
	return 0;

}
int exist_state(state state) {
	int i;
	for (i = 0; i < num_of_state; i++) {
		if (equal_state(state, states[i]))
			return i;
	}
	return -1;
}
int equal_state(state a, state b) {
	if (a.num_of_item == b.num_of_item) {
		int i, j;
		int same = 0;
		for (i = 0; i < a.num_of_item; i++) {
			for (j = 0; j < b.num_of_item; j++) {
				if (strcmp(a.items[i], b.items[i]) == 0) {
					same++;
					break;
				}
			}
		}
		if (same == a.num_of_item)
			return 1;
	}
	return 0;
}
