
#ifndef __WsVideo__H__
#define __WsVideo__H__

#include <Wt/WContainerWidget>


class WsVideo : public Wt::WContainerWidget, public WsOptions {
public :
  WsVideo(Wt::WContainerWidget* parent = 0);
  ~WsVideo();
  void            load();
};

#endif // __WsVideo__H__

