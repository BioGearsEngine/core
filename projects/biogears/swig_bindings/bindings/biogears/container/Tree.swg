%{
#include <stdexcept>
#include <exception>
#include <biogears/container/Tree.h>
#include <biogears/container/Tree.tci.h>
using biogears::Tree;
%}


%import <biogears/exports.swg>

%nspace biogears::Tree;


%rename (at) biogears::Tree::operator[]( size_t pos );
%rename (CopyFrom) biogears::Tree::operator=(const Tree&);

namespace biogears {
template <typename Data>
class Tree {
public:
  using value_type = Data;
  using reference = value_type&;
  using child_vector = std::vector<Tree<Data>>;
  using iterator = typename child_vector::iterator;

  Tree() = default ;
  Tree(const Data& ); 
  Tree(const Tree&);

  template< typename ... PARAMS >
  explicit Tree(const PARAMS&... params);
  virtual ~Tree() = default;

  Data& value();  
  void  value( const Data& ) &;

  child_vector& children();
  void children( const std::vector<Tree>& );

  Tree& operator[]( size_t pos );
  Tree& operator=(const Tree&);
};
}
%nspace biogears::Tree<char const *>;
%nspace std::vector<biogears::Tree<char const*>>;
%template(vector_tree_char) std::vector<biogears::Tree<char const *>>;
%template(Tree_cString) biogears::Tree<char const *>;