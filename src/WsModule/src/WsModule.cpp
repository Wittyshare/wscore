/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <iostream>

#include <Wt/WLogger>

#include <gdcore/gdCore.h>

#include <Main/WsApplication.h>

#include "WsModule.h"

WsOption::WsOption(const std::string& attribute, const boost::any value)
  : m_sAttribute(attribute), m_value(value)
{ }

const std::string& WsOption::attribute() const
{
  return m_sAttribute;
}

void WsOption::setValue(const boost::any& value)
{
  m_value = value;
}

const boost::any& WsOption::value() const
{
  return m_value;
}

WsOptions::WsOptions()
{ }

WsOptions::WsOptions(const std::vector<WsOption>& vOptions)
  : m_vOptions(vOptions)
{ }

WsOptions::WsOptions(const WsOptions& src)
{
  m_vOptions = src.options();
}

void WsOptions::setOption(const std::string& attribute, boost::any value)
{
  for (int iV = 0; iV < m_vOptions.size(); ++iV) {
    if ( m_vOptions[iV].attribute() != attribute ) continue;
    m_vOptions[iV].setValue(value);
    return;
  }
  m_vOptions.push_back(WsOption(attribute, value));
}

void WsOptions::setOptions(const std::vector<WsOption>& vOptions)
{
  m_vOptions = vOptions;
}

const boost::any& WsOptions::option(const std::string& attribute) const
{
  for (int iV = 0; iV < m_vOptions.size(); ++iV)
    if ( m_vOptions[iV].attribute() == attribute )
      return m_vOptions[iV].value();
  return m_noOptions;
}

const  std::vector<WsOption>& WsOptions::options() const
{
  return m_vOptions;
}

void WsOptions::outOptions(const std::string message)
{
  wApp->log("notice") <<  message << " WsOptions::outOptions() starting options loop ";
  for (int iV = 0; iV < m_vOptions.size(); ++iV)
    wApp->log("notice") <<  message << " WsOptions::outOptions() Attribute : " << m_vOptions[iV].attribute() << ", Value : " << Wt::asString(m_vOptions[iV].value());
  wApp->log("notice") <<  message << " WsOptions::outOptions() end options loop ";
}


WsEditorWidget::WsEditorWidget(WContainerWidget* parent)
  : WContainerWidget(parent)
{ }

WsEditorWidget::~WsEditorWidget()
{ }

void WsEditorWidget::load()
{
  WContainerWidget::load();
}

bool WsEditorWidget::saveEditor()
{
  return true;
}






WsModule::WsModule()
  : Wt::WObject(Wt::WApplication::instance()), m_bLoaded(false), m_bHideImages(false)
{ }

bool WsModule::processPrefix(const std::string& path)
{
  return false;
}

std::string WsModule::checkPath(const std::string& currentPath)
{
  return currentPath;
}

void WsModule::setModuleName(const std::string& moduleName)
{
  m_moduleName = moduleName;
}

const std::string& WsModule::moduleName() const
{
  return m_moduleName;
}

void WsModule::setSoName(const std::string& soName)
{
  m_soName = soName;
}

const std::string& WsModule::soName() const
{
  return m_soName;
}

void WsModule::setFileName(const std::string& fileName)
{
  m_fileName = fileName;
}

const std::string& WsModule::fileName() const
{
  return m_fileName;
}

void WsModule::setExtensions(const std::string& extensions)
{
  m_extension = extensions;
  gd_core_string2vector(m_extensions, m_extension.c_str(), "|");
}

const std::string& WsModule::extension(const std::string& extension) const
{
  if ( extension.size() > 0 ) {
    for (int iExt = 0; iExt < m_extensions.size(); ++iExt)
      if ( m_extensions[iExt] == extension ) return extension;
  }
  return m_extension;
}

void WsModule::setPrefix(const std::string& prefix)
{
  m_prefix = prefix;
}

const std::string& WsModule::prefix() const
{
  return m_prefix;
}

std::string WsModule::pathWithoutPrefix(const std::string& path)
{
  std::string str(path);
  boost::algorithm::replace_first(str, prefix(), "");
  return str;
}

std::string WsModule::pathWithPrefix(const std::string& path)
{
  std::string str(path);
  str.insert(0, prefix());
  return str;
}

void WsModule::setHideImages(const bool bHide)
{
  m_bHideImages = bHide;
}

bool WsModule::hideImages()
{
  return m_bHideImages;
}

void WsModule::setSysPath(const std::string& sysPath)
{
  m_sysPath = sysPath;
}

const std::string& WsModule::sysPath() const
{
  return m_sysPath;
}

void WsModule::setDiffPath(const std::string& diffPath)
{
  m_diffPath = diffPath;
}

const std::string& WsModule::diffPath() const
{
  return m_diffPath;
}

WsContentButtonsBar* WsModule::contentButtonsBar()
{
  WsApplication::wsInstance()->contentButtonsBar();
}

void WsModule::destroyContentsMenuBar()
{ }

void WsModule::setLoaded()
{
  m_bLoaded = true;
}

bool WsModule::isLoaded()
{
  return m_bLoaded;
}

