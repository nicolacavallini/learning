#include <iostream>
#include <memory>
#include <string>



class TD {};

class S : public TD {

public:

  S(const std::string &l);

private:
  std::string label;
};

S::S(const std::string &l) : label(l)
{}

class T {
public:

  static std::shared_ptr<T> create(std::unique_ptr<TD> &in);

  void setExt(std::unique_ptr<TD>& in);

private:
  T();

  T(std::unique_ptr<TD> & in);

  std::unique_ptr<TD> ext;
};

T::T(std::unique_ptr<TD> & in) {
  ext = std::move(in);
}

T::T() {}

std::shared_ptr<T> T::create(std::unique_ptr<TD> &in){//

  std::shared_ptr<T> tag(new T(in));
  //std::shared_ptr<T> tag = std::make_shared<T>(in); needs a public constructor
  //tag->setExt(in);

  return tag;
}


int main(){
  std::unique_ptr<TD> s(new S("specific"));

  std::shared_ptr<T> t = T::create(s);

  return 0;
}
