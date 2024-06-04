#include <lib/modules/indexator/Indexator.hpp>
#include <lib/modules/arg_parser/ArgParser.h>

using namespace ArgumentParser;

int main(int argc, char** argv) {
  ArgParser arg_parser("Индексатор");
  arg_parser.AddStringArgument('i', "input", "Путь к папке, которую надо проиндексировать.");
  arg_parser.AddStringArgument('o', "output", "Путь к папке, в которой сохраняться бинарные файлы индекса.").Default("./data");
  arg_parser.AddStringArgument('s', "stopwords", "Путь к текстовому файлу со стоп словами.").Default("./data/stopwords.txt");
  arg_parser.AddHelp('h', "help", "Данное приложение создаёт индекс из локальных файлов для дальнейшего локального поиска по нему.");
  arg_parser.Parse(argc, argv);


  Indexer indexer(arg_parser.GetStringValue("i"), arg_parser.GetStringValue("s"));
  indexer.indexation();
  indexer.save_to_file(arg_parser.GetStringValue("o"));
};
