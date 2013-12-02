#ifndef _WsFileResource_H_
#define _WsFileResource_H_

#include <vector>

#include <Wt/WFileResource>

class WsFileResource : public Wt::WFileResource {
public:
  WsFileResource(const std::string& strFile, Wt::WObject* parent = 0);
  void                       deleteAfterProcessed(bool bDelete);

protected :
  virtual void               handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);

private :
  bool                       m_bDelete;
  const std::string          m_strFile;
};

#endif // ifdef _WsFileResource_H_

