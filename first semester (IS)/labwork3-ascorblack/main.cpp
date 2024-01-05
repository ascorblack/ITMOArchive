#include "src/tsv_reader.h"
#include "src/bmp_writer.h"
#include "src/args_parser.h"


#ifdef _WIN32
  #include <windows.h>
  #define CLEAR_CONSOLE() system("cls")
#elif __linux__
  #include <cstdlib>
  #define CLEAR_CONSOLE() system("clear")
#else
  #define CLEAR_CONSOLE() std::cout << "\n\n"
#endif

void clearConsole() {
  CLEAR_CONSOLE();
}


int main(int arg_count, char** argv) {

  ArgsParser parse_args(arg_count, argv);
  parse_args.Parse();

  Options user_arguments = parse_args.GetUserArgs();

  std::cout << "input_filepath=" << user_arguments.input_filepath << "\n";
  std::cout << "output_dir=" << user_arguments.output_dir << "\n";
  std::cout << "max_iter=" << user_arguments.max_iter << "\n";
  std::cout << "frequency=" << user_arguments.frequency << "\n";
  std::cout << "print_console=" << user_arguments.print_console << "\n\n";

  TsvReader input_tsv = TsvReader(user_arguments.input_filepath);

//  SandyMatrix* sandy_matrix = input_tsv.Parse();
  while(true) {
      SandyMatrix* sandy_matrix = input_tsv.Parse();
      delete sandy_matrix;
  }

//  bool change = true;
//  BMPWriter bmp_writer(user_arguments);
//
//  while (sandy_matrix->step < user_arguments.max_iter && change) {
//    if (sandy_matrix->step % 10 == 0) {
//      printf("Step: %d\r", sandy_matrix->step);
//    }
//
//    if (user_arguments.frequency != 0 && sandy_matrix->step % user_arguments.frequency == 0) {
//      bmp_writer.Write(sandy_matrix);
//
//      if (user_arguments.print_console) {
//        clearConsole();
//        sandy_matrix->printMatrix(" ");
//      }
//
//    }
//
//    change = sandy_matrix->calcStep();
//  }
//  printf("Step: %d\r\n", sandy_matrix->step);
//
//  if (user_arguments.print_console) {
//    clearConsole();
//    sandy_matrix->printMatrix(" ");
//  }
//
//  bmp_writer.Write(sandy_matrix);
//
//  delete &bmp_writer;
//  delete sandy_matrix;
}
