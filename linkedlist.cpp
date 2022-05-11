//TODO: use smart_pointers upon Node creation
//      FIX clear()

#include <iostream>
#include <string>

template <typename T>
class Linked
{
public:
  using value_type = T;

  struct Node
  {
    value_type m_data;
    Node* m_nextNode = nullptr;

    Node(const value_type& val): m_data(val) {}
  };

  Linked(){head = curr_ptr = new Node(value_type());}
  ~Linked() { clear();}
  
  void push(const value_type& val)
  {
    Node* new_node = new Node(val); 
    if(curr_ptr){
      curr_ptr->m_nextNode = new_node;
    }
    if(!head) {head=new_node;}
    curr_ptr = new_node;
  }
  
  void delAfter(Node* before_del)
  {
    Node* del_ptr = before_del->m_nextNode;
    before_del->m_nextNode = del_ptr->m_nextNode;
    delete del_ptr;
  }
  void clear()
  {
    while(head->m_nextNode) delAfter(head); 
    delete head;
  }

  class iterator
  {
    Node* m_ptr;
  public:
    explicit iterator(Node* __ptr): m_ptr(__ptr) {}
    iterator& operator++() { m_ptr = m_ptr->m_nextNode; return *this;}
    bool operator==(iterator other)const{ return m_ptr==other.m_ptr;}
    bool operator!=(iterator other)const{ return !(*this==other);}
    value_type& operator*() const { return m_ptr->m_nextNode->m_data;}
  };
  
  iterator begin() { return iterator(head);}
  iterator end() { return iterator(curr_ptr);} 

private:
  Node* head = nullptr;
  Node* curr_ptr = nullptr; 
};

int main()
{
  Linked<std::string> ll; 
  ll.push("first");
  ll.push("second");
  ll.push("third");
  ll.clear();
  ll.push("after 1");
  ll.push("after 2");
  for(auto i: ll)
    std::cout << i << std::endl;
  return 0;
}
