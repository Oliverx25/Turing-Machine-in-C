// Libraries that we need
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Constants
#define MAX_LONG_W 100
#define MAX_LONG_INFO 100

// Prototype of the function to get the string that the user wants to use in the Turing Machine
int GetString(char *Input_String, char Fill_Character, int Long_Cadena);

// Main function
int main () {
  // Variables
  char Name_File[10], Fill_Character = ' ', Aux_Storage[MAX_LONG_INFO], Aux_token;
  int Line_Counter = 1, State_Counter = 0, Flag_Unknown_Char = 0;
  int Long_W_MT = 0, Number_Of_States = 0, Alphabet_Length = 0, Aux_Alphabet_Position = 0;
  int Header = 0, Current_State = 0, State_Transition_Index, Defined = 1;
  char *TM_String = calloc (MAX_LONG_W, sizeof(char));
  char *Head_String = calloc (MAX_LONG_W, sizeof(char));
  char *Alphabet = calloc (Alphabet_Length, sizeof(char));
  // Check if the memory was assigned correctly
  if (TM_String != NULL && Head_String != NULL) {
    memset(TM_String, Fill_Character, MAX_LONG_W);
    memset(Head_String, ' ', MAX_LONG_W);
  } else {
    printf("Memory wasn't assigned correctly...");
    exit(EXIT_FAILURE);
  }
  // Create the table of transitions (Matrix of 3 dimensions)
  char ***State_Transition_Table = (char***) calloc (1, sizeof(char**));
  State_Transition_Table[0] = (char**) calloc (1, sizeof(char*));
  State_Transition_Table[0][0] = (char*) calloc (5, sizeof(char));
  // Pointer to the file that we are going to use
  FILE *TM_Description_File = NULL;

  // Read and store the information of the Turing Machine from the file
    // Set file to use in the Turing Machine
  printf("File to use in the Turing Machine: ");
  scanf("%s", Name_File);
  TM_Description_File = fopen(Name_File, "r");
  getchar();
  if (TM_Description_File == NULL) {
    printf("Error opening the file...");
    exit(EXIT_FAILURE);
  }
    // Store the states of the Turing Machine
  while (fgets(Aux_Storage, MAX_LONG_INFO, TM_Description_File) != NULL) {
    Aux_Storage[strcspn(Aux_Storage, "\n")] = '\0';
    printf("Reading and store => %s\n", Aux_Storage);
      // Set the number of states
    if (Line_Counter == 1) {
      char *token = strtok(Aux_Storage, ":");
      while (token != NULL) {
        token = strtok(NULL, "\n");
        token != NULL ? Number_Of_States = atoi(token) : 0;
      }
      printf("Number of states => %d\n\n", Number_Of_States);
      char ***Resize_Long_States = (char ***) calloc (Number_Of_States, sizeof(char**));
      if (Resize_Long_States == NULL) {
        printf("Memory couldn't be assigned for the transitions...");
        exit(EXIT_FAILURE);
      }
      State_Transition_Table = Resize_Long_States;
    }
      // Set the number of characters of the alphabet
    if (Line_Counter == 2) {
      char *token = strtok(Aux_Storage, ":");
      while (token != NULL) {
        token = strtok(NULL, "\n");
        token != NULL ? Alphabet_Length = atoi(token) : 0;
      }
      printf("Number of characters of the Alphabet => %d\n\n", Alphabet_Length);
      for (int State_Index = 0; State_Index < Number_Of_States; State_Index++) {
        char **Resize_Long_Alphabet = (char **) calloc (Alphabet_Length, sizeof(char*));
        if (Resize_Long_Alphabet == NULL) {
          printf("Memory couldn't be assigned for the transitions...");
          exit(EXIT_FAILURE);
        }
        State_Transition_Table[State_Index] = Resize_Long_Alphabet;
      }
    }
      // Store the alphabet
    if (Line_Counter == 3) {
      char *Resize_Alphabet = (char *) realloc (Alphabet, Alphabet_Length * sizeof(char));
      if (Alphabet == NULL) {
        printf("Memory couldn't be assigned for the Alphabet...");
        exit(EXIT_FAILURE);
      }
      Alphabet = Resize_Alphabet;
      char *token = strtok(Aux_Storage, ":");
      while (token != NULL) {
        token = strtok(NULL, "|");
        if (token != NULL) {
          Aux_token = token[0];
          Alphabet[Aux_Alphabet_Position++] = Aux_token;
        }
      }
    }
      // Store the transitions on the table
    if (Line_Counter > 3) {
      char *token = strtok(Aux_Storage, ":");
      for (int Trans_Index_State = 0; Trans_Index_State < Alphabet_Length; Trans_Index_State++) {
        token = strtok(NULL, "|");
        if (token != NULL) {
          char *Aux_Transition = strdup(token);
          State_Transition_Table[State_Counter][Trans_Index_State] = Aux_Transition;
        }
      }
      printf("=> Transitions of state Q%d stored.\n\n", State_Counter);
      State_Counter++;
    }
    Line_Counter++;
  }
  fclose(TM_Description_File);
    // Input the string that the user wants to use in the Turing Machine
  Long_W_MT = GetString(TM_String, Fill_Character, Long_W_MT);
    // Check string characters with the alphabet
  for (int String_Index = 0; String_Index < Long_W_MT; String_Index++) {
    Flag_Unknown_Char = 1;
    for (int Check_Char = 0; Check_Char < Alphabet_Length; Check_Char++) {
      if (TM_String[String_Index] == Alphabet[Check_Char]) {
        Flag_Unknown_Char = 0;
        break;
      }
    }
    if (Flag_Unknown_Char == 1) {
      printf("The string contains an unknown character (%c) in the alphabet...\n", TM_String[String_Index]);
      exit(EXIT_FAILURE);
    }
  }
    // Perform the Turing Machine
  while (Defined == 1) {
    system("clear");
      // Print the tape of the Turing Machine
    for (int Header_Index = 0, String_Index = 0; Header_Index < MAX_LONG_W; Header_Index++) {
      printf((Header == Header_Index) ? "/-^-\\" : "  %c  ", Head_String[Header_Index]);
      if ((Header_Index + 1) % 25 == 0) {
        printf("\n");
        for (String_Index; String_Index < MAX_LONG_W; String_Index++) {
          printf("[ %c ]", TM_String[String_Index]);
          if ((String_Index + 1) % 25 == 0) {
            printf("\n\n");
            String_Index++;
            break;
          }
        }
      }
    }
      // Print the current state of the Turing Machine
    printf("Current State => Q%d\n", Current_State);
    for (int Alphabet_Index = 0; Alphabet_Index < Alphabet_Length; Alphabet_Index++) {
      if (TM_String[Header] == State_Transition_Table[Current_State][Alphabet_Index][0]) {
        State_Transition_Index = Alphabet_Index;
        printf("Transition found => %s\n", State_Transition_Table[Current_State][State_Transition_Index]);
        break;
      }
    }
      // Check if the transition exists
    if (State_Transition_Table[Current_State][State_Transition_Index][4] == '0') {
      printf("The transition doesn't exist...\n");
      Defined = State_Transition_Table[Current_State][State_Transition_Index][4];
    } else {
        // Replace the character in the tape of the Turing Machine
      TM_String[Header] = State_Transition_Table[Current_State][State_Transition_Index][2];
        // Move the header
      printf("The header is in this position => %d\n", Header);
      if (State_Transition_Table[Current_State][State_Transition_Index][3] == '1') {
        Header++;
        if (Header >= MAX_LONG_W)
          Header = 0;
      } else {
        Header--;
        if (Header < 0)
          Header = MAX_LONG_W - 1;
      }
      printf("The header moved to this position => %d\n", Header);
        // Change the current state to the next state
      char Aux_Char_State = State_Transition_Table[Current_State][State_Transition_Index][1];
      Aux_Char_State = Aux_Char_State - 48;
      Current_State = Aux_Char_State;
      printf("Current State => Q%d\n", Current_State);
        // Delay of 500 ms to see the changes
      usleep(500000);
    }
  }
    // Print the final step of the Turing Machine
  system("clear");
  for (int Header_Index = 0, String_Index = 0; Header_Index < MAX_LONG_W; Header_Index++) {
    printf((TM_String[Header_Index] != Fill_Character) ? "  *  " : "  %c  ", Head_String[Header_Index]);
    if ((Header_Index + 1) % 25 == 0) {
      printf("\n");
      for (String_Index; String_Index < MAX_LONG_W; String_Index++) {
        printf("[ %c ]", TM_String[String_Index]);
        if ((String_Index + 1) % 25 == 0) {
          printf("\n\n");
          String_Index++;
          break;
        }
      }
    }
  }
  printf("Final State => Q%d\n", Current_State);
    // Free the memory
  printf("\n ===  Turing Machine  ===  \n");
  free(TM_String);
  free(Head_String);
  for (int Index_i = 0; Index_i < Number_Of_States; Index_i++) {
    for (int Index_j = 0; Index_j < Alphabet_Length; Index_j++) {
        free(State_Transition_Table[Index_i][Index_j]);
    }
    free(State_Transition_Table[Index_i]);
  }
  free(State_Transition_Table);
  free(Alphabet);
  printf("\n ===    Machine Off   ===  \n");
  return 0;
}

int GetString(char *Input_String, char Fill_Character, int Long_Cadena) {
  printf("Input a string to operate: ");
  fgets(Input_String, MAX_LONG_W + 3, stdin);
  Long_Cadena = strlen(Input_String) - 1;
    // Control of the length of the string
  if (Long_Cadena > MAX_LONG_W) {
    printf("The string is too long, the maximum length is %d characters...\n", MAX_LONG_W);
    exit(EXIT_FAILURE);
  }
  // Change the last character of the string to the fill character
  if (Input_String[strlen(Input_String) - 1] == '\n') {
    Input_String[strlen(Input_String) - 1] = Fill_Character;
    Input_String[Long_Cadena + 1] = Fill_Character;
  }
  return Long_Cadena;
}