%{
#include <stdexcept>
#include <exception>
#include <biogears/config.h>
#include <biogears/container/Tree.h>
#include <biogears/container/Tree.tci.h>
#if !(BIOGEARS_WITHOUT_NAMESPACE)
using BIOGEARS_NAMESPACE Tree;
#endif 
#include <biogears/cdm/properties/SEScalarTypes.h>
%}


%import <biogears/exports.swg>
%import <biogears/config.h>

#if !(BIOGEARS_WITHOUT_NAMESPACE)
%nspace BIOGEARS_NAMESPACE Tree;
#endif 

%rename (at) BIOGEARS_NAMESPACE Tree::operator[]( size_t pos );
%rename (CopyFrom) BIOGEARS_NAMESPACE Tree::operator=(const Tree&);

OPEN_BIOGEARS_NAMESPACE
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
CLOSE_BIOGEARS_NAMESPACE

%nspace BIOGEARS_NAMESPACE Tree<char const *>;
//%nspace std::vector<BIOGEARS_NAMESPACE Tree<char const*>>;

%template(Tree_cString) BIOGEARS_NAMESPACE Tree<char const *>;
//%template(vector_tree_char) std::vector<BIOGEARS_NAMESPACE Tree<char const *>>;