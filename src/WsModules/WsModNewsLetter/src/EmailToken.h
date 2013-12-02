
#ifndef EMAILTOKEN_H_
#define EMAILTOKEN_H_

#include <Wt/WDate>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Auth/Dbo/AuthInfo>

#include <string>

using namespace Wt;
namespace dbo = Wt::Dbo;

class EmailToken {
public:

  //EmailToken();

  std::string email;
  std::string token;
  Wt::WDate   currentDate;
  int         registered;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, email,       "email");
    dbo::field(a, token,       "token");
    dbo::field(a, currentDate, "date");
    dbo::field(a, registered,  "registered");
  }
};
typedef Wt::Dbo::collection< Wt::Dbo::ptr<EmailToken> > EmailTokens;

#endif // EMAILTOKEN_H_
