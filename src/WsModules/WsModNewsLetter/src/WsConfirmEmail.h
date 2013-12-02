#include <string>
#include <Wt/WContainerWidget>

#include "EmailToken.h"

#ifndef RECEIVE_H_
#define RECEIVE_H_

using namespace Wt;
namespace dbo = Wt::Dbo;

class WsConfimEmail : public WContainerWidget {
public:
  WsConfimEmail(const std::string& token, WContainerWidget* parent);
  ~WsConfimEmail();

private:
  dbo::Session session_;
  dbo::backend::Sqlite3* sqlite3_;
};

#endif //RECEIVE_H_
