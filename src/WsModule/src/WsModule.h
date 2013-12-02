/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WS_MODULE_H__
#define WS_MODULE_H__    1

#include <string>

#include <boost/any.hpp>

#include <Wt/WContainerWidget>
#include <Wt/WWidget>

/*!
   \file WsModule.h
   \brief base class for building a wittyShare module

 */

class WsContentButtonsBar;

class WsOption {
public :
  WsOption(const std::string& attribute, const boost::any value);
  const std::string&                 attribute() const;
  void                               setValue(const boost::any& value);
  const boost::any&                  value() const;

private :
  std::string                        m_sAttribute;
  boost::any                         m_value;
};

class WsEditorWidget : public Wt::WContainerWidget {
public :
  WsEditorWidget(Wt::WContainerWidget* parent = 0);
  ~WsEditorWidget();
  virtual void          load();
  virtual bool          saveEditor();
};

class WsOptions {
public :
  // CTor
  WsOptions();
  WsOptions(const std::vector<WsOption>& vOptions);
  // Copy constructor
  WsOptions(const WsOptions& src);

  /*! \brief Set an options if previously set, update the value. */
  void                               setOption(const std::string& attribute, boost::any value);
  /*! \brief Set all options. */
  void                               setOptions(const std::vector<WsOption>& vOptions);
  /*! \brief Get an options value. */
  const boost::any&                  option(const std::string& attribute) const;
  /*! \brief Get all options. */
  const std::vector<WsOption>&       options() const;
  /*! \brief Output all options. */
  void                               outOptions(const std::string message);


private :
  std::vector<WsOption>              m_vOptions;
  boost::any                         m_noOptions;
};


class WsModule : public Wt::WObject, public WsOptions {
public:
  WsModule();
  /*! \brief Create the functionalities. */
  virtual Wt::WWidget*               createContentsMenuBar(Wt::WContainerWidget* parent = 0) const = 0;
  /*! \brief Create the contents. */
  virtual Wt::WWidget*               createContents(Wt::WContainerWidget* parent = 0) const = 0;
  /*! \brief Create the contents for an editor (create a view of options). */
  virtual WsEditorWidget*            createEditor(Wt::WContainerWidget* parent = 0) const = 0;
  /*! \brief Create the contents for an administrator. */
  virtual Wt::WWidget*               createAdmin(Wt::WContainerWidget* parent = 0) const = 0;
  /*! \brief Return the description of the module. */
  virtual std::string                description() const = 0;
  /*! \brief Return the currentPath, or another path when some action is required for the currentPath
              example /NewsLetter/Subscribe/<token>, check the token, if error return a path to the error page else return the path to the subscription form
              by default return the currentPath. */
  virtual std::string                checkPath(const std::string& currentPath);

  /*! \brief Set/Get the module name. */
  void                               setModuleName(const std::string& name);
  const std::string&                 moduleName() const;

  /*! \brief Set/Get the module so name or path ex. libgd.so or /usr/lib/libgd.so. */
  void                               setSoName(const std::string& soName);
  const std::string&                 soName() const;

  /*! \brief Set/Get the file name that trigger the module : exemple "Staff List" */
  // TODO : add multiple values for the fileName ?
  void                               setFileName(const std::string& fileName);
  const std::string&                 fileName() const;

  /*! \brief Set/Get the file extension managed by the module : exemple .ods|.odp. */
  void                               setExtensions(const std::string& extensions);
  const std::string&                 extension(const std::string& extension = "") const;

  /*! \brief Set/Get prefix for a virtual path, like "/SiteMap" */
  void                               setPrefix(const std::string& prefix);
  const std::string&                 prefix() const;
  /*! \brief return the relative path without the prefix : example pathWithoutPrefix("/SiteMap/Test") return "/Test" */
  std::string                        pathWithoutPrefix(const std::string& path);
  /*! \brief return the relative path with the prefix : example pathWithPrefix("/Test") return "/SiteMap/Test" */
  std::string                        pathWithPrefix(const std::string& path);
  /*! \brief proccess the prefix, return true if processed */
  virtual bool                       processPrefix(const std::string& path);

  /*! \brief Set/Get the flag to specified if the images is hided. */
  // TODO : put this in the config management system
  void                               setHideImages(const bool bHide);
  bool                               hideImages();

  /*! \brief Set/Get the system path (example the selected file /var/www/mysite/about/me.fhtml). */
  void                               setSysPath(const std::string& sysPath);
  const std::string&                 sysPath() const ;

  /*! \brief Set/Get the difference between the web server (example Apache /var/www) root path and the wittishare root path (example /var/www/demo_site).
   *   the returned value = /demo_site. */
  void                               setDiffPath(const std::string& diffPath);
  const std::string&                 diffPath() const ;

  /*! \brief WittyShare provide a container to add some buttons like export page in pdf, ...,
   *          This function allow a module to add this own button to this container */
  WsContentButtonsBar*               contentButtonsBar();
  virtual void                       destroyContentsMenuBar();

  /*! \brief Some modules can be loaded on startup, no more actions is required by the content widget */
  void                               setLoaded();
  bool                               isLoaded();

private :
  std::string                        m_moduleName;
  std::string                        m_soName;
  std::string                        m_fileName;
  std::string                        m_extension;
  std::string                        m_prefix;
  std::vector<std::string>           m_extensions;
  std::string                        m_sysPath;
  std::string                        m_diffPath;
  bool                               m_bLoaded;
  bool                               m_bHideImages;
};

/*! \brief Example of the main function
extern "C" {
WsModule* buildModule() {
  return new WsModule();
 }
}
*/

typedef WsModule* (*pf_wsModule)           ();

#endif // ifndef WsModule_H__

