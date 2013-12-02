#ifndef __WsSearchView__
#define __WsSearchView__

#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WTableView>

#include <User/WsUser.h>

class WsSearchItem : public Wt::WStandardItem {
public :
  WsSearchItem();
  WsSearchItem(const Wt::WString& text);
  void            setNode(WsAbstractNode* pNode);
  WsAbstractNode* node();

private :
  WsAbstractNode* m_node;
};

class WsSearchView : public Wt::WTableView {
public:
  WsSearchView(std::string sSearch, Wt::WContainerWidget* parent = 0);
  ~WsSearchView();
  Wt::WWidget*     navBar();

public slots:
  void onViewSelectionChanged();
  void onViewDblClick(Wt::WModelIndex, Wt::WMouseEvent);

private:
  Wt::WStandardItemModel*         m_pModelView;
  std::string                     m_sRootPath;    // /var/www/demo_site
  bool                            m_bLogSearch;
  WWidget*                        m_wNavBar;
};

#endif // __WsSearchView__

