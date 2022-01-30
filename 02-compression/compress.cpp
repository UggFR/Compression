#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> load(std::string fichier) {
  std::vector<std::string> v={};
  std::ifstream file(fichier);
  std::string ligne;
  while(std::getline(file, ligne)){
    v.push_back(ligne);
  }
  return v;
}

typedef std::map<std::string, int> occs;

occs count(std::vector<std::string> const &words) {
  occs map_count;
  for(size_t i=0;i<words.size();i++){
    auto it = map_count.find(words.at(i));
    if(it == map_count.end()){
      map_count.insert({words.at(i),1});
    }
    else{
      map_count.find(words.at(i))->second += 1;
    }
  }
  return map_count;
}

typedef std::map<std::string, std::string> codes;

std::pair<std::string, std::string> prefix(std::pair<std::string, std::string> const &root, std::string const &pref) {
  std::pair<std::string, std::string> concat;
  concat.first = root.first;
  concat.second = pref + root.second;
  return concat;
}

codes merge(codes const &x, codes const &y) {
  codes reuni;
  for(auto mot=x.begin();mot != x.end();mot++){
    reuni.insert(prefix(*mot,"0"));
  }
  for(auto mot=y.begin();mot != y.end();mot++){
    reuni.insert(prefix(*mot,"1"));
  }
  return reuni;
}

typedef std::multimap<int, codes> partial_codes;

auto extract(partial_codes &pc) {
  auto ip = pc.begin();
  auto s = *ip;
  pc.erase(ip);
  return s;
}

void reduce(partial_codes &res) {
  auto s1 = extract(res);
  auto s2 = extract(res);
  res.insert({s1.first+s2.first,merge(s1.second,s2.second)});
}

codes create(occs const &occ) {
  partial_codes occ_partial;
  for(auto ip=occ.begin();ip != occ.end();ip++){
    //(*ip).second pour accéder au second membre d'une map en déférencement
    codes codes_partial; 
    codes_partial.insert({ip->first,""});
    occ_partial.insert({ip->second,codes_partial});
  } 
  while(occ_partial.begin() != --occ_partial.end()){
    reduce(occ_partial);
  }
  auto sec_occ_part = occ_partial.begin()->second;
  return sec_occ_part;
}

std::string compress(std::vector<std::string> const &words, codes &c) {
  std::string filename("compression_conv.txt");
  std::string res;
  std::ofstream file(filename.c_str(),std::ios::in);
  //file.open(filename);
  if(file){
    for(size_t i=0;i<words.size();i++){
    std::string s = c[words[i]];
    file << s + "\n" ;
    res += s;
    }
    file.close();
  }
  else{
    std::cerr << "Impossible d'ouvrir le fichier ! \n";
  }
  return res;
}

codes shorten(codes const &c) {
  // A FAIRE
}

// A NE PAS DEPLACER
//#include "check_1984.hpp"

int main() {
  // FAITES VOS TESTS PERSOS ICI
  std::vector<std::string> v=load("1984.txt");
  std::cout << v.size();
  occs map_count = count(v);
  // A NE PAS EFFACER
  //run_tests();
  codes crea = create(map_count);
  auto comp_str = compress(v,crea);
  std::cout << comp_str;
  return 0;
}
