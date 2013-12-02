#include <Wt/WBreak>
#include <Wt/WDate>
#include <Wt/WText>
#include <Wt/WString>
#include <Wt/WRandom>
#include <Wt/Dbo/WtSqlTraits>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Sqlite3>

#include "Wt/Mail/Message"
#include <Wt/Auth/HashFunction>

#include <string>
#include <memory>

#include "EmailToken.h"
#include "WsConfirmEmail.h"

WsConfimEmail::WsConfimEmail(const std::string &token,
                             WContainerWidget *parent) :
  WContainerWidget(parent)
{
  //Init DB
  sqlite3_ =  new dbo::backend::Sqlite3("ws_test.db");
  sqlite3_->setProperty("show-queries", "true");

  session_.setConnection(*sqlite3_);

  session_.mapClass<EmailToken>("user");

  Auth::SHA1HashFunction hf;
  std::string hash = hf.compute(token, std::string());

  //Print debug info to the screen.

  new WBreak(this);
  new WText(token, this);
  new WBreak(this);
  new WText(hash, this);
  new WBreak(this);

  //Search token in db

  {
    dbo::Transaction transaction(session_);

    EmailTokens eTokens = session_.find<EmailToken>().where("token = ?").bind(hash);
    if(eTokens.size() == 1){
      // call API stub from Guy
      dbo::ptr<EmailToken> et =eTokens.front();
      new WText(" Hey ", this);
      new WText(et->email, this);
      new WBreak(this);
      et.remove();
    } else{
      new WText("Error - token not found (maybe this mail is confirmed?)", this);
      new WBreak(this);
    }
  }
}

WsConfimEmail::~WsConfimEmail()
{
  delete sqlite3_;
}
