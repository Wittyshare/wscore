#ifndef __WsSiteMapView__
#define __WsSiteMapView__

#include <Wt/WContainerWidget>
#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WTreeView>
#include <Wt/WMenu>
#include <Wt/WMessageBox>

#include "gdwtcore/gdToolbar.h"
#include "gdwtcore/gdWFileView.h"

#include <User/WsUser.h>
#include <WsModule/WsModule.h>

class WsSiteMapItem : public Wt::WStandardItem {
public :
  WsSiteMapItem();
  WsSiteMapItem(const Wt::WString& text);
  void            setNode(NodePtr pNode);
  NodePtr node();

private :
  NodePtr m_node;
};






class WsTreeView : public Wt::WTreeView {
public :
  WsTreeView();
  void            layoutSizeChanged(int width, int height);
};








class WsSiteMapView : public Wt::WContainerWidget, public WsOptions {
public:
  WsSiteMapView(gdwtcore::viewType dlgType, gdWFileView::tbFlags flags = gdWFileView::tbDefault, Wt::WContainerWidget* parent = 0);
  ~WsSiteMapView();
  virtual void    load();

  std::string     path2SiteMapPath(const std::string& path);        // Replace /var/www/demo_site/index.html         by /var/www/demo_site/SiteMap/index.html
  std::string     SiteMapPath2path(const std::string& siteMapPath); // Replace /var/www/demo_site/SiteMap/index.html by /var/www/demo_site/index.html
  void            setRelativePath(const std::string& relativePath);
  void            init(NodePtr pNode);
  bool            treeTraverse(Wt::WStandardItem* rootItem, const std::vector<std::string>& vPath, int nLevel);
  void            treeMatchPath(NodePtr pNode);
  bool            loadTree(Wt::WStandardItem* currentItem, NodePtr pNode, gdWFileView::skipMode skipParentDir = gdWFileView::noSkipParent);
  bool            loadFolder(Wt::WStandardItem* currentItem, const std::string& newPath, NodePtr curNode, bool bEdit = false);
  bool            loadFile(Wt::WStandardItem* currentItem, const std::string& currentPath, NodePtr curNode);
  void            setCurrentPath(const std::string& currentPath);
  std::string     selectedDirectory();
  std::string     selectedFile();
  std::string     selectedPath();
  std::string     currentFolderPath();
  void            makeToolbar();

public :
  Wt::Signal<gdWFileView::signalType, std::string>& fileSelected() {
    return fileSelected_;
  };

private :
  Wt::Signal<gdWFileView::signalType, std::string> fileSelected_;

public slots:
  void onViewDblClick(Wt::WModelIndex idx, Wt::WMouseEvent mouseEvent);
  void onTreeSelectionChanged();
  void onTreeExpanded(Wt::WModelIndex idx);
  void onViewSelectionChanged();
  void onViewExpanded(Wt::WModelIndex idx);
  void doRename();
  void doConfirmDelete();
  void doDelete(Wt::StandardButton button);
  void doNewFolder();
  void onDirectoryNameChanged(Wt::WStandardItem* pItem);

private:
  gdwtcore::viewType              m_viewType;
  gdWFileView::tbFlags            m_tbFlags;
  Wt::WStandardItemModel*         m_pModelTree;
  Wt::WStandardItemModel*         m_pModelView;
  Wt::WTreeView*                  m_pTVTree;
  Wt::WTreeView*                  m_pTVView;
  gdToolbar*                      m_pTb;
  bool                            m_bOnTreeSelect;

  bool                            m_bOnDataChanged;
  Wt::WMenu*                      m_pMenu;
  bool                            m_bOnRename;
  NodePtr                 m_pTreecurNode;
  gdToolbarItem*                  m_tbiDelete;
  gdToolbarItem*                  m_tbiNewFolder;
  gdToolbarItem*                  m_tbiRename;
  std::string                     m_selectedFile;
  std::string                     m_selectedDirectory;
  std::string                     m_sRootPath;    // /var/www/demo_site
  std::string                     m_sSiteMap;     // /SiteMap
  std::string                     m_sRootSiteMap; // /var/www/demo_site/SiteMap
  Wt::WMessageBox*                m_pMb;
  bool                            m_bLogSiteMap;
};

#endif // __WsSiteMapView__

