#include "cli_args.hpp"
#include "generate.hpp"
#include "graph.hpp"
#include "stats.hpp"
#include "utils.hpp"

#include <iostream>
#include <random>

void
print()
{
}

int
main(int argc, const char* argv[])
{
  cli_args args = process_cli_args(argc, argv);

  dbl_acc hop_acc;
  dbl_acc len_acc;

  for (int i = 1; i <= 100; ++i)
    {
      default_random_engine eng;
      eng.seed(args.seed);
      graph g = generate_graph(args, eng);
      calc_sp_stats(g, hop_acc, len_acc);
    }

  cout
}
