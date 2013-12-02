/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModImagesGallery_H__
#define WsModImagesGallery_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <gdcore/gdImage.h>

#include <WsModule/src/WsModule.h>

// TODO : pas top je sais
#include "WsModules/WsModImages2/src/WsImages2.h"

/*!
   \file WsModImagesGallery.h
   \brief a wittyShare module

 */

/*! \class WsMenuImage
 *  \brief Ws object to render a menu with images and title.
 *
 *  When the load() method is called, this modules retrieve the current directory and inspect all sub directories.
 *   For each directories, if the properties in_menu_image is true then WsImagesGallery load the content file called "image.jpg" in the <sub_directory>/.config
 *
 *
 *  Options for this widget :
 */

class WsImagesGallery : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsImagesGallery(Wt::WContainerWidget* parent = 0);
  ~WsImagesGallery();
  void                load();
  void                buildVector();
  void                buildThumbnails();
  void                renderButtons();
  void                renderThumb();
  void                renderImage();

  /*! \brief Set the number of images in the menu. If the number of images is more than this value a timer rotate the images*/
  void                             setMaxImages(int nMaxImages);

public slots:
  void                             doTimeoutThumb();
  void                             doTimeoutImage();
  void                             doPrevious(Wt::WPushButton* pButton);
  void                             doPlay(Wt::WPushButton* pButton);
  void                             doNext(Wt::WPushButton* pButton);
  void                             doThumbClicked(int nImageId);

private :
  std::vector<std::string>         m_vImages;
  //std::vector<std::string>         m_vThumbnails;
  std::vector<Magick::Blob>        m_vThumbnails;
  std::vector<std::string>         m_vDisplayNames;
  int                              m_nStartImage;
  int                              m_nMaxImages;
  int                              m_nCurrentImage;
  Wt::WTimer*                      m_pTimerThumb;
  Wt::WTimer*                      m_pTimerImage;
  WsImages2*                       m_pImages2;
  Wt::WText*                       m_pImages2Text;
  WContainerWidget*                m_cwThumb;
};

class WsModImagesGallery : public WsModule {
public :
  /*! \brief CTor. */
  WsModImagesGallery();
  ~WsModImagesGallery();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool                saveEditor() const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModImagesGalleryInit(void) __attribute__((constructor));

  WsModImagesGallery* buildModule()
  {
    return new WsModImagesGallery();
  }
}

#endif // ifndef WsModImagesGallery_H__

