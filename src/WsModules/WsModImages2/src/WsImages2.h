/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsImages2_H__
#define WsImages2_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WTimer>
#include <Wt/WEvent>
#include <Wt/WAnchor>

#include <WsModule/WsModule.h>

class wsi2Anchor : public Wt::WAnchor {
public :
  wsi2Anchor(const Wt::WLink& link, const Wt::WString& text, Wt::WContainerWidget* parent = 0);
  void                             setImageIndex(int imageIndex);

public slots:
  void                             doClicked(Wt::WMouseEvent mEv);

public :
  Wt::Signal<int>&                 imageSelected();

private :
  Wt::Signal<int>                  m_sigImageSelected;
  int                              m_nImageIndex;
};

/*!
   \file WsImages2.h
   \brief a wittyShare class that render images randomly

 */

/*! \class WsImage2
 *  \brief Ws object to render images.
 *
 *  This object support some options :
 *    debug                   : true/false default false Output a more messages for debugging.
 *    plusWidget              : true/false default false add a + widget on the bottom right corner of the image.
 *    textWidget              : true/false default true  add the text widget;
 *    reactWhenPathChange     : true/false default true  change the image when an internal path change, only when the image is on the foreground.
 *    imageInBackground       : true/false default false push the image on the background
 *    image_delay             : 0 or > 0   default 5000  Specify the delay for images rotation.
 *    setDirectoryNameAsTitle : true/false default false if true set a title that contain the directory name.
 *    imagesPath              :            default "wApp->theme()->resourcesUrl() + "wittyshare/Images/WsModImages2" this is a relative url
 *                                          example : /wt/resources/themes/wittyshare/wittyshare/Images/WsModImages2
 *                                          add WsApp->docRoot() before this path to access the full system file.
 *                                          example : /var/www/wt/resources/themes/wittyshare/wittyshare/Images/WsModImages2
 *                                         This path content the .png,.jpg or .jpeg file + 2 files with the same name but with extension :
 *                                           - .url   : the link to the document to display when the text is clicked.
 *                                           - .fhtml : the text displayed in the image.
 */

class WsImages2 : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsImages2(Wt::WContainerWidget* parent = 0);
  /*! \brief DTor. */
  ~WsImages2();
  /*! \brief Load the widget (traditional wt virtual function). */
  virtual void                     load();

  /*! \brief Load the image with the current index. */
  void                             loadImage();

  /*! \brief Build the vector. */
  void                             build();

  /*! \brief Return the number of images. */
  int                              count();

  /*! \brief Return the text of the current image. */
  const std::string&               text();

public slots :
  void                             doTimeout();
  void                             doPathChanged();
  void                             doImageClicked();
  void                             doCounterClicked(int index);

private :
  bool                             m_bDebug;
  bool                             m_bReactWhenPathChange;
  long                             m_lDelay;
  bool                             m_bLoaded;

  std::vector<std::string>         m_sImagesVect;
  std::vector<std::string>         m_sImagesText;
  std::vector<std::string>         m_sImagesLink;
  std::vector<std::string>         m_sImagesLinkType;
  std::vector<std::string>         m_sImagesTitle;

  bool                             m_bOnLoad;
  int                              m_nCurrentImage;
  Wt::WTimer*                      m_pTimer;
};

#endif // ifndef WsImages2_H__

