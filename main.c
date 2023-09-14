//------------------------------------------------------------------------------
// a2.c
//
// In this program, we get an input which is a string with which we can
// do different types of printing and changes based on the command that we give.
//
//
// Group: Lena Klambauer
//
// Author: 11937605
//------------------------------------------------------------------------------
//
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define NO_SENTENCE 1
#define WRONG_SENTENCE 2
#define NO_MEMORY 3
#define WRONG_COMMAND (-1)
#define FIRST_INPUT 1
#define SECOND_INPUT 2
#define WITH_ROW 1
#define WITHOUT_ROW 2

// forward declarations
void startPrint();
char getCommandInput();

int getDecimalInput(int input_type, int sentence_nr, int restricted_nr);

char* getTextInput(int* return_val);

void getTextStatistics(const char* input_content);
void printText(char** text_array, int array_size, int type);
int countSentence(const char* input_content);
char** getArraySentence(const char* input_content, int* array_size, int* return_val);
char* concatinateSentence(int* return_val, char** text_array, int array_size);
char* swapSentence(int* return_val, char** text_array, int array_size);

//------------------------------------------------------------------------------
///
/// In the main function. We are implementing our program, that is used to get
/// the statistic, swapping , concatenating of input sentences and showing
/// their printings.
///
/// @return SUCCESS meaning the code ended without a problem
/// @return NO_SENTENCE, WRONG_SENTENCE, NO_MEMORY meaning the code had a
/// problem in memory allocation or the input wasn't right.
//
int main()
{
  int return_val = SUCCESS;
  char* input_content = getTextInput(&return_val);
  if(return_val != SUCCESS)
    return return_val;

  int print_start = 1;
  int loop = 1;
  while(loop)
  {
    if(print_start)
      startPrint();

    printf("Ihre Wahl: ");
    char input_command = getCommandInput();
    if(input_command == WRONG_COMMAND)
    {
      print_start = 0;
      continue;
    }
    print_start = 1;

    int array_size = 0;
    char** text_array = getArraySentence(input_content,
                                         &array_size, &return_val);
    if(return_val != SUCCESS)
    {
      free(input_content);
      if(text_array == NULL)
        return return_val;
      for(int elem = 0; elem < array_size; elem++)
        free(text_array[elem]);
      free(text_array);
      return return_val;
    }

    int wrong_command = 0;
    if(input_command == 't')
    {
      getTextStatistics(input_content);
    }
    else if(input_command == 's')
    {
      char* text_temp = swapSentence(&return_val, text_array, array_size);
      if(return_val != SUCCESS)
      {
        loop = 0;
      }
      else
      {
        if(text_temp == NULL)
          wrong_command = 1;
        if(!wrong_command)
        {
          free(input_content);
          input_content = text_temp;
        }
      }
    }
    else if(input_command == 'c')
    {
      char* text_temp = concatinateSentence(&return_val,
                                            text_array, array_size);
      if(return_val != SUCCESS)
      {
        loop = 0;
      }
      else
      {
        if(text_temp == NULL)
          wrong_command = 1;
        if(!wrong_command)
        {
          free(input_content);
          input_content = text_temp;
        }
      }
    }
    else if(input_command == 'p')
      printText(text_array, array_size, WITH_ROW);
    else if(input_command == 'o')
      printText(text_array, array_size, WITHOUT_ROW);
    else if(input_command == 'e')
    {
      printf("Programmende!\n");
      loop = 0;
    }
    else
      print_start = 0;

    // cleaning text_array after usage
    for(int elem = 0; elem < array_size; elem++)
      free(text_array[elem]);
    free(text_array);
  }

  free(input_content);
  return return_val;
}

//------------------------------------------------------------------------------
///
/// In the getInput function, we get an input from the console using getchar().
/// The input which is a string, is then cleared of all unneeded spaces and
/// checked for the right punctuation, as well as for memory allocation
/// problems.
///
/// @param reutrn_value used to return a certain exit code in case of problems.
///
/// @return input_content which is the string
/// @return NULL in case of memory not allocated.
//
char* getTextInput(int* return_val)
{
  int init_malloc = 1;
  char* input_content = (char*)malloc(init_malloc * sizeof(char));
  if(input_content == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_val = NO_MEMORY;
    return NULL;
  }

  char space = ' ';
  char point = '.';
  char qu_mark = '?';
  char exc_point = '!';
  int point_check = 1;
  char eos = '\0';
  char new_line = '\n';
  int cunt = 0;
  int input_char = 1;
  printf("Bitte Text eingeben:\n");
  while((input_char = getchar()) != new_line)
  {
    // ignore spaces after "." "?" or "!"
    if((cunt != 0) &&
       ((char)input_char == space) &&
       ((input_content[cunt-1] == point) ||
        (input_content[cunt-1] == qu_mark) ||
        (input_content[cunt-1] == exc_point)))
    {
      continue;
    }
    // ignore spaces at start
    if((cunt == 0) && ((char)input_char== space))
      continue;

    input_content[cunt] = (char)input_char;
    cunt++;
    char* temp_pointer = (char*)realloc(input_content,
                                        (cunt+1) * sizeof(char));
    if(temp_pointer == NULL)
    {
      printf("Zu wenig Speicher vorhanden!\n");
      free(input_content);
      *return_val = NO_MEMORY;
      return input_content;
    }
    input_content = temp_pointer;

    if(((char)input_char == point) ||
       ((char)input_char == qu_mark) ||
       ((char)input_char == exc_point))
    {
      point_check = 0;
    }
  }

  if(point_check)
  {
    printf("Kein Satz gefunden!\n");
    free(input_content);
    *return_val = NO_SENTENCE;
    return NULL;
  }

  cunt--;

  if((input_content[cunt] != point) &&
     (input_content[cunt] != qu_mark) &&
     (input_content[cunt] != exc_point))
  {
    printf("Der Text endet ohne Satzzeichen!\n");
    free(input_content);
    *return_val = WRONG_SENTENCE;
    return NULL;
  }
  input_content[cunt+1] = eos;

  *return_val = SUCCESS;
  return input_content;
}

//------------------------------------------------------------------------------
///
/// In the startPrint function we only print the options menu.
///
/// @return
//
void startPrint()
{
  printf("\nWählen Sie eine Option:\n"
         "  t: Text-Statistik\n"
         "  s: Zwei Sätze vertauschen\n"
         "  c: Zwei Sätze zusammenketten\n"
         "  p: Text mit Zeilennummer ausgeben\n"
         "  o: Text ohne Zeilennummer ausgeben\n"
         "  e: Programm beenden\n\n");
}

//------------------------------------------------------------------------------
///
/// In the getCommandInput we get an input from the console using getchar().
/// Here we only check if the command has more then one char as to return
/// WRONG_COMMAND, every other check is done in the main.
///
/// @return output_command in case there is only one inputted char.
//
char getCommandInput()
{
  char output_command = 0;
  int allowed_char_nr = 1;
  char new_line = '\n';
  int input_char = 0;
  int cunt = 0;
  while((input_char = getchar()) != new_line)
  {
    output_command = (char)input_char;
    cunt++;
  }
  if(cunt > allowed_char_nr)
    return WRONG_COMMAND;

  return output_command;
}

//------------------------------------------------------------------------------
///
/// In the getTextStatistics we count every element of the string (excluding
/// spaces) and give a number for all the sentences, big and small letters,
/// all letters and symbols.
/// From this function we don't return anything, since this is only a print
/// function.
///
/// @param input_content the whole string.
///
/// @return
//
void getTextStatistics(const char* input_content)
{
  char eos = '\0';
  char space = ' ';
  char point = '.';
  char qu_mark = '?';
  char exc_point = '!';
  int sentence_nr = 0;
  int symbol_nr = 0;
  int word_nr = 0;
  int big_word_nr = 0;
  int small_word_nr = 0;
  int cunt = 0;
  while(input_content[cunt] != eos)
  {
    char input_char = input_content[cunt];

    if(input_char == space)
    {
      cunt++;
      continue;
    }
    else if(input_char == point || input_char == qu_mark ||
            input_char == exc_point)
    {
      sentence_nr++;
      // including point
      symbol_nr++;
    }
    else if(input_char >= 'A' && input_char <= 'Z')
    {
      word_nr++;
      big_word_nr++;
      symbol_nr++;
    }
    else if(input_char >= 'a' && input_char <= 'z')
    {
      word_nr++;
      small_word_nr++;
      symbol_nr++;
    }
    else
      symbol_nr++;

    cunt++;
  }
  printf("Text Statistik:\n"
         "%d Sätze, %d Zeichen\n"
         "%d Buchstaben (%d Großbuchstaben, %d Kleinbuchstaben)\n",
         sentence_nr, symbol_nr, word_nr, big_word_nr, small_word_nr);
}

//------------------------------------------------------------------------------
///
/// In the printText function we simply print the string. The way the string
/// is printed is dependent on the @param type which tells the code if we
/// should print the whole string, or split it in an orderly way with the
/// corresponding number for each sentence.
/// From this function we don't return anything, since this is only a print
/// function.
///
/// @param text_array we use the string (in a double array format).
/// @param array_size the number of sentences.
/// @param type decides the workflow.
///
/// @return
//
void printText(char** text_array, int array_size, int type)
{
  if(type == WITH_ROW)
  {
    printf(" Nr. | Satz\n"
           "-----+----------------------------------------\n");

    for(int elem = 0; elem < array_size; elem++)
    {
      printf("%4d | %s\n", (char)(elem +1), text_array[elem]);
    }
  }
  else if(type == WITHOUT_ROW)
  {
    printf("Text:\n");
    for(int elem = 0; elem < array_size; elem++)
    {
      printf("%s ",text_array[elem]);
    }
    printf("\n");
  }
}

//------------------------------------------------------------------------------
///
/// In the function countSentences we get the number of sentences from the
/// string.
///
/// @param input_content the whole string.
///
/// @return sentence_nr the number of sentences.
//
int countSentence(const char* input_content)
{
  char point = '.';
  char qu_mark = '?';
  char exc_point = '!';
  char eos = '\0';
  int sentence_nr = 0;
  int inp_cunt = 0;
  while(input_content[inp_cunt] != eos)
  {
    if((input_content[inp_cunt] == point) ||
       (input_content[inp_cunt] == qu_mark) ||
       (input_content[inp_cunt] == exc_point))
    {
      sentence_nr++;
    }
    inp_cunt++;
  }
  return sentence_nr;
}

//------------------------------------------------------------------------------
///
/// In the getArraySentence function we split the string which we inputted in
/// an double array which allocates enough memory for the amount of sentences
/// and amount of letters in each sentence. We make sure that nothing changes
/// from the initial string and add "\0" at the end of each string.
///
///
/// @param input_content the whole string but as a const char* this time.
/// @param array_size the number of sentences.
/// @param return_val to check for errors with memory allocation.
///
/// @return array_sentence which is the filled double array.
/// @return NULL in case of problems with the memory allocation.
//
char** getArraySentence(const char* input_content, int* array_size,
                        int* return_val)
{
  char one = 1;
  char point = '.';
  char qu_mark = '?';
  char exc_point = '!';
  char eos = '\0';

  int sentence_nr = countSentence(input_content);
  *array_size = sentence_nr;
  char** array_sentence = (char**)malloc(sentence_nr * sizeof(char*));
  if(array_sentence  == NULL)
  {
    *array_size = 0;
    printf("Zu wenig Speicher vorhanden!\n");
    *return_val = NO_MEMORY;
    return NULL;
  }

  int run_loop_out = 1;
  int inp_cunt = 0;
  int arr_cunt = 0;
  while(run_loop_out)
  {
    int sen_cunt = 0;
    int run_loop_in = 1;
    char* sentence = (char*)malloc(one * sizeof(char));
    if(sentence  == NULL)
    {
      *array_size = arr_cunt;
      printf("Zu wenig Speicher vorhanden!\n");
      *return_val = NO_MEMORY;
      return array_sentence;
    }
    while(run_loop_in)
    {
      char* temp_pointer = (char*)realloc(sentence,
                                          (one + sen_cunt + 1) * sizeof(char));
      if(temp_pointer  == NULL)
      {
        *array_size = arr_cunt + 1;
        printf("Zu wenig Speicher vorhanden!\n");
        *return_val = NO_MEMORY;
        return array_sentence;
      }
      sentence = temp_pointer;

      sentence[sen_cunt] = input_content[inp_cunt];
      if(((input_content[inp_cunt] == point) ||
          (input_content[inp_cunt] == qu_mark) ||
          (input_content[inp_cunt] == exc_point)))
      {
        run_loop_in = 0;
      }
      sen_cunt++;
      inp_cunt++;
    }
    if(input_content[inp_cunt] == eos)
      run_loop_out = 0;
    sentence[sen_cunt] = eos;
    array_sentence[arr_cunt] = sentence;
    arr_cunt++;
  }
  return array_sentence;
}

//------------------------------------------------------------------------------
///
/// In the getDecimalInput function, we get an input from the console using
/// getchar().
/// We parse that input, which is a character type and convert it to integer.
/// Depending on the value of the "input_type" parameter, the function
/// will print the appropriate outputs in the console and in case the output
/// is higher than the number of sentences or is equal to an already used
/// number, we ask again for a new number.
///
/// @param input_type used to decide the workflow of the function.
/// @param sentence_nr gives the number of sentences.
/// @param restricted_nr gives the number of the sentence which has been
/// used already.
///
/// @return sum the converted input
//
int getDecimalInput(int input_type, int sentence_nr, int restricted_nr)
{
  int zero = 48;
  int nine = 57;
  int char_to_int = 48;
  int decimal = 10;
  int sum = 0;

  int input_char = 0;
  int loop = 1;
  while(loop)
  {
    if(input_type == FIRST_INPUT)
      printf("Nummer des ersten Satzes: ");
    else if(input_type == SECOND_INPUT)
      printf("Nummer des zweiten Satzes: ");

    int wrong_input = 0;
    sum = 0;
    while ((input_char = getchar()) != '\n')
    {
      if (input_char < zero || input_char > nine)
        wrong_input = 1;
      if (wrong_input)
        continue;

      // calculating digit value
      int digit_value = input_char - char_to_int;
      sum *= decimal;
      sum += digit_value;
    }
    loop = 0;
    if(wrong_input || (sum == restricted_nr) || (sum > sentence_nr) ||
       (sum == 0))
      loop = 1;
  }
  return sum;
}

//------------------------------------------------------------------------------
/// In the function concatinateSentences we concat two sentences together.
/// The new sentence that we receive will be saved in the text_array and
/// the number of sentences will become smaller. The sentences will be
/// connected through " und " and the spaces and sentence ending punctuations
/// will be replaced with " und ".
///
/// @param return_val to check for errors with memory allocation.
/// @param text_array we use the string (in a double array format).
/// @param array_size the number of sentences.
///
/// @return new_input_content which is the updated text.
/// @return NULL in case of problems with the memory allocation.
//
char* concatinateSentence(int* return_val, char** text_array, int array_size)
{
  char eos = '\0';
  int one = 1;
  int min_sentence_amount = 1;

  if(array_size <= min_sentence_amount)
  {
    printf("Nur noch ein Satz vorhanden!\n");
    return NULL;
  }

  int restricted_nr = 0;
  int sentence_1 = getDecimalInput(FIRST_INPUT, array_size, restricted_nr);
  int sentence_2 = getDecimalInput(SECOND_INPUT, array_size, sentence_1);
  char connecting_word[] = " und ";
  int connecting_word_size = 5;

  // text length
  int text_length = 0;
  for(int o_elem = 0; o_elem < array_size; o_elem++)
  {
    int i_elem = 0;
    while(text_array[o_elem][i_elem] != eos)
    {
      text_length++;
      i_elem++;
    }
  }

  char* new_input_content = (char*)malloc(
    (text_length + connecting_word_size + one) * sizeof(char));
  if(new_input_content == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_val = NO_MEMORY;
    return NULL;
  }

  char* concat_words = (char*)malloc(
    (text_length + connecting_word_size + one) * sizeof(char));
  if(concat_words == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    free(new_input_content);
    *return_val = NO_MEMORY;
    return NULL;
  }

  // write first sentence
  int mix_idx = 0;
  int conc_idx = 0;
  while (text_array[sentence_1 - 1][conc_idx] != eos)
  {
    concat_words[mix_idx] = text_array[sentence_1 - 1][conc_idx];
    mix_idx++;
    conc_idx++;
  }
  // delete the . ! or ?
  mix_idx--;

  for(int c_idx = 0; c_idx < connecting_word_size; c_idx++)
  {
    concat_words[mix_idx] = connecting_word[c_idx];
    mix_idx++;
  }

  //write second sentence
  conc_idx = 0;
  while (text_array[sentence_2 - 1][conc_idx] != eos)
  {
    concat_words[mix_idx] = text_array[sentence_2 - 1][conc_idx];
    mix_idx++;
    conc_idx++;
  }
  concat_words[mix_idx] = eos;

  // write the rest
  int cunt = 0;
  for(int o_elem = 0; o_elem < array_size; o_elem++)
  {
    if(o_elem == (sentence_1 - 1))
    {
      int i_elem = 0;
      while(concat_words[i_elem] != eos)
      {
        new_input_content[cunt] = concat_words[i_elem];
        cunt++;
        i_elem++;
      }
    }
    else if((o_elem != (sentence_1 - 1)) && (o_elem != (sentence_2 - 1)))
    {
      int i_elem = 0;
      while(text_array[o_elem][i_elem] != eos)
      {
        new_input_content[cunt] = text_array[o_elem][i_elem];
        cunt++;
        i_elem++;
      }
    }
  }
  new_input_content[cunt] = eos;
  free(concat_words);
  return new_input_content;
}

//------------------------------------------------------------------------------
///
/// In the swapSentence function we change the position of the chosen
/// sentences. We change the position of the first chosen sentence with the
/// second chosen sentence. After the changes are done we update the text.
///
/// @param return_val to check for errors with memory allocation.
/// @param text_array we use the string (in a double array format).
/// @param array_size the number of sentences.
///
/// @return new_input_content which is the updated text.
/// @return NULL in case of problems with the memory allocation or too less
/// sentences.
//
char* swapSentence(int* return_val, char** text_array, int array_size)
{
  char eos = '\0';
  int one = 1;
  int min_sentence_amount = 1;

  if(array_size <= min_sentence_amount)
  {
    printf("Nur noch ein Satz vorhanden!\n");
    return NULL;
  }

  int restricted_nr = 0;
  int sentence_1 = getDecimalInput(FIRST_INPUT, array_size, restricted_nr);
  int sentence_2 = getDecimalInput(SECOND_INPUT, array_size, sentence_1);

  // text length
  int text_length = 0;
  for(int o_elem = 0; o_elem < array_size; o_elem++)
  {
    int i_elem = 0;
    while(text_array[o_elem][i_elem] != eos)
    {
      text_length++;
      i_elem++;
    }
  }

  char* new_input_content = (char*)malloc(
    (text_length + one) * sizeof(char));
  if(new_input_content == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_val = NO_MEMORY;
    return NULL;
  }

  int cunt = 0;
  for(int o_elem = 0; o_elem < array_size; o_elem++)
  {
    if(o_elem == (sentence_1 - 1))
    {
      int i_elem = 0;
      while(text_array[sentence_2 - 1][i_elem] != eos)
      {
        new_input_content[cunt] = text_array[sentence_2 - 1][i_elem];
        cunt++;
        i_elem++;
      }
    }
    else if(o_elem == (sentence_2 - 1))
    {
      int i_elem = 0;
      while(text_array[sentence_1 - 1][i_elem] != eos)
      {
        new_input_content[cunt] = text_array[sentence_1 - 1][i_elem];
        cunt++;
        i_elem++;
      }
    }
    else
    {
      int i_elem = 0;
      while(text_array[o_elem][i_elem] != eos)
      {
        new_input_content[cunt] = text_array[o_elem][i_elem];
        cunt++;
        i_elem++;
      }
    }
  }
  new_input_content[cunt] = eos;
  return new_input_content;
}
