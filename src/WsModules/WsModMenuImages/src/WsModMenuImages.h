/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModMenuImages_H__
#define WsModMenuImages_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>

#include <WsModule/src/WsModule.h>

/*!
   \file WsModMenuImages.h
   \brief a wittyShare module

 */

/*! \class WsMenuImage
 *  \brief Ws object to render a menu with images and title.
 *
 *  When the load() method is called, this modules retrieve the current directory and inspect all sub directories.
 *   For each directories, if the properties in_menu_image is true then WsMenuImages load the content file called "image.jpg" in the <sub_directory>/.config
 *
 *
 *  Options for this widget :
 */

class WsMenuImages : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsMenuImages(Wt::WContainerWidget* parent = 0);
  ~WsMenuImages();
  void                load();
  void                buildVector();
  void                buildMenu();

  /*! \brief Set the number of images in the menu. If the number of images is more than this value a timer rotate the images*/
  void                             setMaxImages(int nMaxImages);

public slots:
  void                             doTimeout();

private :
  std::vector<std::string>         m_vDirectories;
  std::vector<std::string>         m_vDisplayNames;
  int                              m_nStartImage;
  int                              m_nMaxImages;
  Wt::WTimer*                      m_pTimer;
};

class WsModMenuImages : public WsModule {
public :
  /*! \brief CTor. */
  WsModMenuImages();
  ~WsModMenuImages();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool                saveEditor() const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModMenuImagesInit(void) __attribute__((constructor));

  WsModMenuImages* buildModule()
  {
    return new WsModMenuImages();
  }
}

#endif // ifndef WsModMenuImages_H__

