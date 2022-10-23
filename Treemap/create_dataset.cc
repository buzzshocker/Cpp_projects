#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>

static std::vector<std::string> donors = {
  // List of pioneers in CS (ordered by lastname)
  // <https://en.wikipedia.org/wiki/List_of_pioneers_in_computer_science>
  "Howard Aiken",
  "Al-Jazari",
  "Al-KhwarizmI",
  "Frances E. Allen",
  "John Atanasoff",
  "Charles Babbage",
  "John Backus",
  "Banū Mūsā",
  "Tim Berners-Lee",
  "Corrado Böhm",
  "George Boole",
  "Kathleen Booth",
  "Per Brinch Hansen",
  "Fred Brooks",
  "Vannevar Bush",
  "David Caminer",
  "Vint Cerf",
  "Noam Chomsky",
  "Alonzo Church",
  "Edmund M. Clarke",
  "Wesley A. Clark",
  "Edgar F. Codd",
  "Lynn Conway",
  "Stephen Cook",
  "James Cooley",
  "Ole-Johan Dahl",
  "Edsger Dijkstra",
  "J. Presper Eckert",
  "E. Allen Emerson",
  "Douglas Engelbart",
  "Federico Faggin",
  "Elizabeth Feinler",
  "Tommy Flowers",
  "Sally Floyd",
  "Gottlob Frege",
  "Stephen Furber",
  "Seymour Ginsburg",
  "Kurt Gödel",
  "Susan L. Graham",
  "Jim Gray",
  "Barbara Grosz",
  "Margaret Hamilton",
  "Richard Hamming",
  "Geoffrey Hinton",
  "C.A.R. Hoare",
  "Betty Holberton",
  "Herman Hollerith",
  "Grace Hopper",
  "Hsu Feng-hsiung",
  "Cuthbert Hurd",
  "Harry Huskey",
  "Kenneth Iverson",
  "Joseph Marie Jacquard",
  "Maurice Karnaugh",
  "Jacek Karpinski",
  "Alan Kay",
  "Stephen Cole Kleene",
  "Donald Knuth",
  "Leslie Lamport",
  "Sergei Alekseyevich Lebedev",
  "Gottfried Leibniz",
  "J. C. R. Licklider",
  "Barbara Liskov",
  "Ramon Llull",
  "Ada Lovelace",
  "John Mauchly",
  "John McCarthy",
  "Edward J. McCluskey",
  "Marvin Minsky",
  "Nakamatsu Yoshirō",
  "Nakashima Akira",
  "Peter Naur",
  "John von Neumann",
  "Allen Newell",
  "Max Newman",
  "Kristen Nygaard",
  "Blaise Pascal",
  "Alan Perlis",
  "Radia Perlman",
  "Pier Giorgio Perotto",
  "Péter Rózsa",
  "Rosalind Picard",
  "Emil L. Post",
  "Dennis Ritchie",
  "Saul Rosen",
  "Bertrand Russell",
  "Gerard Salton",
  "Jean E. Sammet",
  "Charles Sanders Peirce",
  "Sasaki Tadashi",
  "Claude Shannon",
  "Shima Masatoshi",
  "Herbert A. Simon",
  "Karen Spärck Jones",
  "Richard Stallman",
  "Michael Stonebraker",
  "Bjarne Stroustrup",
  "Ivan Sutherland",
  "André Truong Trong Thi",
  "Ken Thompson",
  "Toh Chai Keong",
  "Leonardo Torres Quevedo",
  "Linus Torvalds",
  "John W. Tukey",
  "Alan Turing",
  "Wang An",
  "Willis Ware",
  "Adriaan van Wijngaarden",
  "Maurice Wilkes",
  "Sophie Wilson",
  "Niklaus Wirth",
  "Konrad Zuse",
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
        << " <num_donations> [seed]"
        << std::endl;
    exit(1);
  }

  size_t ndonations = std::stoi(argv[1]);
  // At least one donor, but no more than the number of possible donors
  if (ndonations == 0 || ndonations > donors.size()) {
    std::cerr << "Error: wrong donations number" << std::endl;
    exit(1);
  }

  // Prep output file
  std::ofstream fdonations;
  fdonations.open(std::string("donations_") + argv[1] + ".dat",
                  std::ofstream::trunc);
  if (!fdonations.good()) {
    std::cerr << "Error: cannot open output file" << std::endl;
    exit(1);
  }

  // Prep random generator
  std::random_device rd;
  std::mt19937 mt(rd());
  // Random number seed was specific
  if (argc == 3) {
    int seed = std::stoi(argv[2]);
    if (seed < 0) {
      std::cerr << "Error: wrong seed" << std::endl;
      exit(1);
    }
    mt.seed(seed);
  }

  // Generate unique donation amounts between 1000 and (ndonations * 1000)
  std::vector<int> amounts;
  for (size_t i = 0; i < ndonations; i++)
    amounts.push_back((i + 1) * 1000);

  // Shuffle donors and donation amounts
  std::shuffle(donors.begin(), donors.end(), mt);
  std::shuffle(amounts.begin(), amounts.end(), mt);

  // Write donors and their donations in output file
  for (size_t i = 0; i < ndonations; i++)
    fdonations << donors[i] << "," << amounts[i] << std::endl;

  fdonations.close();
  return 0;
}
