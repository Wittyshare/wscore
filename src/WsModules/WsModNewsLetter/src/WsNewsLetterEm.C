#include "Wt/WApplication"
#include <Wt/WBreak>
#include <Wt/WDate>
#include <Wt/WTime>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WString>
#include <Wt/WRandom>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/WRegExpValidator>
#include <Wt/WValidator>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Sqlite3>

#include <Wt/Mail/Message>
#include <Wt/Mail/Mailbox>
#include <Wt/Mail/Client>

#include <Wt/Auth/HashFunction>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <string>
#include <memory>

#include "EmailToken.h"
#include "WsNewsLetter.h"


WsNewsLetter::WsNewsLetter(WContainerWidget *parent) :
  WContainerWidget(parent)
{
  //Init DB
  sqlite3_ =  new dbo::backend::Sqlite3("ws_test.db");
  sqlite3_->setProperty("show-queries", "true");
  session_.setConnection(*sqlite3_);

  session_.mapClass<EmailToken>("user");
  try{
    session_.createTables(); //Use it to create the DB.
    std::cerr << "Created database." << std::endl;
  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Using existing database";
  }

  //Init UI
  emailLE_ = new WLineEdit(this);
  button_ = new WPushButton("Send", this);
  info_ = new WText(this);
  button_->clicked().connect(this, &WsNewsLetter::createUser);

  // an email address validator
  Wt::WRegExpValidator *validator = new Wt::WRegExpValidator(
        "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");
  emailLE_->setValidator(validator);
  emailLE_->setText("michael@emweb.be");
}

WsNewsLetter::~WsNewsLetter()
{
  //session_.dropTables();

  delete sqlite3_;
}

void WsNewsLetter::createUser()
{
  bool knownEmail = false;
  //API call known email. Set the bool knownEmail
  if(knownEmail)
    return;

  if(emailLE_->validate() == WValidator::Invalid){
    new WText("Invalid mail.", this);
    return;
  }
  WString text = "";

  std::string random = WRandom::generateId(10);
  Auth::SHA1HashFunction hf;
  std::string hash = hf.compute(random, std::string());

  {
    dbo::Transaction transaction(session_);

    EmailToken *eToken = new EmailToken();
    eToken->email = emailLE_->text().toUTF8();
    eToken->token = hash;
    eToken->currentDate = WDate::currentDate();

    EmailTokens eTokens = session_.find<EmailToken>().where("email = ?").
        bind(emailLE_->text().toUTF8());
    session_.add(eToken);

    if(eTokens.size() != 0){
      //Print debug info to the screen.

      text += "rendom = ";
      text += random;
      text += " hash = ";
      text += hash;
      text += " Email token is already in db - it will be overwriten?";
      new WText(text, this);
    }
  }

  sendConfirmMail(emailLE_->text().toUTF8(),random);
}

void WsNewsLetter::sendConfirmMail(const std::string& email,
                                       const std::string& userToken) const
{
  std::string url = "/newsletter/subscribe/";
  WApplication *app = WApplication::instance();
  url = app->makeAbsoluteUrl(app->bookmarkUrl(url)) + userToken;

  Mail::Message message;

  message.addRecipient(Mail::To, Mail::Mailbox(email));
  message.setSubject(WString::tr("subject"));
  message.setBody(WString::tr("body").arg(email).arg(url));
  message.addHtmlBody(WString::tr("htmlbody").arg(email).arg(url));

  sendMail(message);
}

void WsNewsLetter::sendMail(const Mail::Message& message) const
{
  Mail::Message m = message;

  if (m.from().empty()) {
    std::string senderName = "Ws letter module";
    std::string senderAddress = "michael@emweb.be";

    m.setFrom(Mail::Mailbox(senderAddress, WString::fromUTF8(senderName)));
  }

  m.write(std::cout);

  Mail::Client client;
  client.connect();
  client.send(m);
}

