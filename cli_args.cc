#include "cli_args.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

// Option strings.
#define NT_S "nt"
#define EDGES_S "edges"
#define NODES_S "nodes"

using namespace std;
namespace po = boost::program_options;

template <typename T>
void requires (const po::variables_map& vm,
               const char* for_arg, const T for_val,
               const char* req)
{
  // Make sure the argument we are looking for is present.
  if (vm.count (for_arg))
    // Make sure the argument has the right value.
    if (vm[for_arg].as<T> () == for_val)
      // Throw, if the required argument is not present.
      if (vm.count (req) == 0)
        {
          ostringstream os;
          os << "Option '" << for_arg 
             << "' that is equal to '" << for_val
             << "' requires option '" << req << "'.";

          throw logic_error (os.str());
        }
}

void requires (const po::variables_map& vm,
               const char* for_arg, const char* req)
{
  // Make sure the argument we are looking for is present.
  if (vm.count (for_arg))
    // Throw, if the required argument is not present.
    if (vm.count (req) == 0)
      throw logic_error (string ("Option '") + for_arg 
                         + "' requires option '" + req + "'.");
}

cli_args
process_cli_args(int argc, const char *argv[])
{
  cli_args result;

  try
    {
      // General options.
      po::options_description gen("General options");
      gen.add_options()
        ("help,h", "produce help message");

      // Network options.
      po::options_description net("Network options");
      net.add_options()
        (NT_S, po::value<string>()->required(),
         "the network type")

        (NODES_S, po::value<int>(),
         "the number of nodes to generate")

        (EDGES_S, po::value<int>(),
         "the number of edges to generate");

      // Some other options.
      po::options_description soo("Simulation options");
      soo.add_options()
        ("seed", po::value<int>()->default_value(1),
         "the seed of the random number generator");

      po::options_description all("Allowed options");
      all.add(gen).add(net).add(soo);
      
      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(all).run(), vm);

      // Dependencies.
      requires(vm, NT_S, NODES_S);
      requires(vm, NT_S, string("random"), EDGES_S);

      if (vm.count("help"))
        {
          cout << all << "\n";
          exit(0);
        }

      // If there is something wrong with parameters, we will get
      // notified automatically and the program will exit.
      po::notify(vm);

      // The network options.
      result.nt = vm[NT_S].as<string>();
      result.nr_nodes = vm[NODES_S].as<int>();

      if (vm.count(EDGES_S))
        result.nr_edges = vm[EDGES_S].as<int>();
      
      // The simulation options.
      result.seed = vm["seed"].as<int>();
    }
  catch(const std::exception& e)
    {
      cerr << e.what() << std::endl;
      exit(1);
    }

  return result;
}
