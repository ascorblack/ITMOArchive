#include <lib/SearchEngine.hpp>
#include <lib/modules/arg_parser/ArgParser.h>


int main(int argc, char** argv) {
  ArgumentParser::ArgParser arg_parser("Поисковик");
  arg_parser.AddStringArgument('i', "input", "Путь к папке, в которой хранится индекс.").Default("./data");
  arg_parser.AddIntArgument('k', "top_k", "Количество выводимых результатов из топа.").Default(5);
  arg_parser.AddHelp('h', "help", "Данное приложение позволяет производить поиск по индексу.");
  arg_parser.Parse(argc, argv);

  if (arg_parser.FinishWithHelp())
    return 0;

  SearchEngine search_engine(arg_parser.GetStringValue("i"), arg_parser.GetIntValue("k"));

  while (true) {
    std::string query;
    printf("Введите запрос: ");
    std::getline(std::cin, query);

    if (query == "!" || query == "q")
      break;

    search_engine.search(query);
  }

};
