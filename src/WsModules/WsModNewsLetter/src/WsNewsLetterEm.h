#include <string>
#include <Wt/WContainerWidget>

#include "EmailToken.h"

#ifndef SEND_H_
#define SEND_H_

using namespace Wt;
namespace dbo = Wt::Dbo;

class WsNewsLetter : public WContainerWidget {
public:
  WsNewsLetter(WContainerWidget* parent);
  ~WsNewsLetter();

private:
  WText* info_;
  WLineEdit* emailLE_;
  WPushButton* button_;
  dbo::Session session_;
  dbo::backend::Sqlite3* sqlite3_;

  void createUser();
  void sendConfirmMail(const std::string& email,
                       const std::string& userToken) const;
  void sendMail(const Mail::Message& message) const;
};

#endif //SEND_H_
